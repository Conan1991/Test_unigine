/* Copyright (C) 2005-2014, Unigine Corp. All rights reserved.
 *
 * File:    GLAppQt.cpp
 * Desc:    OpenGL App Qt widget
 * Version: 1.03
 * Author:  Alexander Zapryagaev <frustum@unigine.com>
 *
 * This file is part of the Unigine engine (http://unigine.com/).
 *
 * Your use and or redistribution of this software in source and / or
 * binary form, with or without modification, is subject to: (i) your
 * ongoing acceptance of and compliance with the terms and conditions of
 * the Unigine License Agreement; and (ii) your inclusion of this notice
 * in any version of this software that you use or redistribute.
 * A copy of the Unigine License Agreement is available by contacting
 * Unigine Corp. at http://unigine.com/
 */

#ifdef _MACOS
	#include <Cocoa/Cocoa.h>
	#include <GL/gl.h>
#endif

#include "GLAppQt.h"

#ifdef _WIN32
	#pragma comment(lib,"gdi32.lib")
	#pragma comment(lib,"user32.lib")
	#pragma comment(lib,"opengl32.lib")
#endif

/*
 */
GLAppQt::GLAppQt(QWidget *parent,Qt::WindowFlags flags) : AppQt(parent,flags) {
	
	#ifdef _WIN32
		hdc = NULL;
		context = NULL;
		wglChoosePixelFormatARB = NULL;
	#elif _LINUX
		visual = NULL;
		context = NULL;
		window = 0;
	#elif _MACOS
		context = NULL;
	#endif
	
	setTitle("GLAppQt");
}

GLAppQt::~GLAppQt() {
	
	#ifdef _LINUX
		if(window != 0) {
			Display *display = x11Info().display();
			XUnmapWindow(display,window);
			XDestroyWindow(display,window);
			window = 0;
		}
	#endif
}

/*
 */
void *GLAppQt::getHandle() {
	
	#ifdef _WIN32
		return internalWinId();
	#elif _LINUX
		static Window window;
		window = internalWinId();
		return &window;
	#elif _MACOS
		return [(NSView*)internalWinId() window];
	#endif
}

/*
 */
const char *GLAppQt::getName() {
	return "opengl";
}

/*
 */
int GLAppQt::create_context() {
	
	if(context != NULL) shutdownGL();
	
	#ifdef _WIN32
		
		WId window = internalWinId();
		
		if((getFlags() & DESTROY) && context != NULL) {
			Unigine::App::destroy();
			wglMakeCurrent(hdc,NULL);
			wglDeleteContext(context);
			context = NULL;
		}
		
		// get new hdc
		if(hdc != NULL) {
			ReleaseDC(window,hdc);
			hdc = NULL;
		}
		hdc = GetDC(window);
		if(hdc == NULL) {
			Unigine::Log::error("GLAppQt::create_context(): GetDC(): failed\n");
			return 0;
		}
		
		// pixel format descriptor
		PIXELFORMATDESCRIPTOR pfd = {
			sizeof(PIXELFORMATDESCRIPTOR), 1, PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER | PFD_SWAP_EXCHANGE,
			PFD_TYPE_RGBA, 32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 24, 8, 0, PFD_MAIN_PLANE, 0, 0, 0, 0
		};
		
		// create simple opengl context
		if(wglChoosePixelFormatARB == NULL) {
			
			// choose pixel format
			int pixelformat = ChoosePixelFormat(hdc,&pfd);
			SetPixelFormat(hdc,pixelformat,&pfd);
			
			// create simple context
			HGLRC old_context = context;
			context = wglCreateContext(hdc);
			if(context == NULL) {
				Unigine::Log::error("GLAppQt::create_context(): wglCreateContext(): failed\n");
				ReleaseDC(window,hdc);
				hdc = NULL;
				return 0;
			}
			
			// share context
			if(old_context && wglShareLists(old_context,context) == 0) {
				Unigine::Log::error("GLAppQt::create_context(): wglShareLists(): failed\n");
				wglDeleteContext(context);
				ReleaseDC(window,hdc);
				context = NULL;
				hdc = NULL;
				return 0;
			}
			
			// set current context
			if(wglMakeCurrent(hdc,context) == 0) {
				Unigine::Log::error("GLAppQt::create_context(): wglMakeCurrent(): failed\n");
				wglDeleteContext(context);
				ReleaseDC(window,hdc);
				context = NULL;
				hdc = NULL;
				return 0;
			}
			
			// get proc address
			wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");
			if(wglChoosePixelFormatARB == NULL) {
				Unigine::Log::error("GLAppQt::create_context(): wglGetProcAddress(): failed\n");
				wglMakeCurrent(hdc,NULL);
				wglDeleteContext(context);
				ReleaseDC(window,hdc);
				context = NULL;
				hdc = NULL;
				return 0;
			}
			
			// destroy context
			wglMakeCurrent(hdc,NULL);
			wglDeleteContext(context);
			context = NULL;
		}
		
		// attributes
		GLint attributes[] = {
			WGL_DRAW_TO_WINDOW_ARB,	GL_TRUE,
			WGL_DOUBLE_BUFFER_ARB,	GL_TRUE,
			WGL_ACCELERATION_ARB,	WGL_FULL_ACCELERATION_ARB,
			WGL_COLOR_BITS_ARB,		32,
			WGL_DEPTH_BITS_ARB,		24,
			WGL_STENCIL_BITS_ARB,	8,
			0,	0,
		};
		
		// choose pixel format
		int pixelformat;
		unsigned int num_formats;
		wglChoosePixelFormatARB(hdc,attributes,NULL,1,&pixelformat,&num_formats);
		if(num_formats == 0) pixelformat = ChoosePixelFormat(hdc,&pfd);
		
		// set pixel format
		SetPixelFormat(hdc,pixelformat,&pfd);
		
		HGLRC old_context = context;
		
		// create context
		context = wglCreateContext(hdc);
		if(context == NULL) {
			Unigine::Log::error("GLAppQt::create_context(): wglCreateContext(): failed\n");
			ReleaseDC(window,hdc);
			hdc = NULL;
			return 0;
		}
		
		// share context
		if(old_context && wglShareLists(old_context,context) == 0) {
			Unigine::Log::error("GLAppQt::create_context(): wglShareLists(): failed\n");
			wglDeleteContext(context);
			ReleaseDC(window,hdc);
			context = NULL;
			hdc = NULL;
			return 0;
		}
		
		// set current context
		if(wglMakeCurrent(hdc,context) == 0) {
			Unigine::Log::error("GLAppQt::create_context(): wglMakeCurrent(): failed\n");
			wglDeleteContext(context);
			ReleaseDC(window,hdc);
			context = NULL;
			hdc = NULL;
			return 0;
		}
		
	#elif _LINUX
		
		Display *display = x11Info().display();
		int screen = x11Info().screen();
		
		// destroy window
		if(window != 0) {
			Display *display = x11Info().display();
			XUnmapWindow(display,window);
			XDestroyWindow(display,window);
			window = 0;
		}
		
		// destroy context
		if((getFlags() & DESTROY) && context != NULL) {
			Unigine::App::destroy();
			glXDestroyContext(display,context);
			context = NULL;
		}
		
		// attributes
		GLint attributes[] = {
			GLX_X_RENDERABLE,	GL_TRUE,
			GLX_DOUBLEBUFFER,	GL_TRUE,
			GLX_DRAWABLE_TYPE,	GLX_WINDOW_BIT,
			GLX_RED_SIZE,		8,
			GLX_GREEN_SIZE,		8,
			GLX_BLUE_SIZE,		8,
			GLX_ALPHA_SIZE,		8,
			GLX_DEPTH_SIZE,		24,
			GLX_STENCIL_SIZE,	8,
			0,	0,
		};
		
		// choose framebuffer config
		int num_configs = 0;
		GLXFBConfig *configs = glXChooseFBConfig(display,screen,attributes,&num_configs);
		if(configs == NULL || num_configs == 0) {
			Unigine::Log::error("GLAppQt::create_context(): glXChooseFBConfig(): failed\n");
			return 0;
		}
		GLXFBConfig config = configs[0];
		XFree(configs);
		
		// get visual
		visual = glXGetVisualFromFBConfig(display,config);
		if(visual == NULL) {
			Unigine::Log::error("GLAppQt::create_context(): glXGetVisualFromFBConfig(): failed\n");
			return 0;
		}
		
		// create context
		context = glXCreateContext(display,visual,context,GL_TRUE);
		if(context == NULL) {
			Unigine::Log::error("GLAppQt::create_context(): glXCreateContext(): failed\n");
			XFree(visual);
			visual = NULL;
			return 0;
		}
		
		// create window
		XSetWindowAttributes window_attributes;
		memset(&window_attributes,0,sizeof(window_attributes));
		window_attributes.colormap = XCreateColormap(display,RootWindow(display,screen),visual->visual,AllocNone);
		window = XCreateWindow(display,internalWinId(),0,0,width(),height(),0,visual->depth,InputOutput,visual->visual,CWBackPixel | CWBorderPixel | CWColormap,&window_attributes);
		if(window == 0) {
			Unigine::Log::error("GLAppQt::create_context(): XCreateWindow(): failed\n");
			glXDestroyContext(display,context);
			XFree(visual);
			context = NULL;
			visual = NULL;
			return 0;
		}
		XMapWindow(display,window);
		
		// set current context
		if(glXMakeCurrent(display,window,context) == 0) {
			Unigine::Log::error("GLAppQt::create_context(): glXMakeCurrent(): failed\n");
			XUnmapWindow(display,window);
			XDestroyWindow(display,window);
			glXDestroyContext(display,context);
			XFree(visual);
			context = NULL;
			visual = NULL;
			window = 0;
			return 0;
		}
		
	#elif _MACOS
		
		// destroy context
		if((getFlags() & DESTROY) && context != NULL) {
			Unigine::App::destroy();
			[(NSOpenGLContext*)context release];
			context = NULL;
		}
		
		// attributes
		GLint attributes[] = {
			NSOpenGLPFAOpenGLProfile,
			NSOpenGLProfileVersion3_2Core,
			NSOpenGLPFADoubleBuffer,
			NSOpenGLPFAAccelerated,
			NSOpenGLPFAColorSize,	24,
			NSOpenGLPFAAlphaSize,	8,
			NSOpenGLPFADepthSize,	24,
			NSOpenGLPFAStencilSize,	8,
			0,	0,
		};
		
		// choose pixelformat
		NSOpenGLPixelFormat *pixelformat = [[NSOpenGLPixelFormat alloc] initWithAttributes:(NSOpenGLPixelFormatAttribute*)attributes];
		if(pixelformat == NULL) {
			Unigine::Log::error("GLAppQt::create_context(): NSOpenGLPixelFormat::initWithAttributes(): failed\n");
			return 0;
		}
		
		// create context
		context = [[NSOpenGLContext alloc] initWithFormat:pixelformat shareContext:(NSOpenGLContext*)context];
		if(context == NULL) {
			Unigine::Log::error("GLAppQt::create_context(): NSOpenGLContext::initWithFormat(): failed\n");
			[pixelformat release];
			return 0;
		}
		
		[pixelformat release];
		
		// set context view
		[(NSOpenGLContext*)context setView:(NSView*)internalWinId()];
		
		// set current context
		[(NSOpenGLContext*)context makeCurrentContext];
		
	#endif
	
	// initialize OpenGL
	if(initGL() == 0) {
		Unigine::Log::fatal("GLAppQt::create_context(): initGL(): failed\n");
		return 0;
	}
	
	return 1;
}

int GLAppQt::destroy_context() {
	
	shutdownGL();
	
	#ifdef _WIN32
		if(context != NULL) {
			wglMakeCurrent(hdc,NULL);
			wglDeleteContext(context);
			context = NULL;
		}
		if(hdc != NULL) {
			WId window = internalWinId();
			ReleaseDC(window,hdc);
			hdc = NULL;
		}
	#elif _LINUX
		Display *display = x11Info().display();
		if(window != 0) {
			XUnmapWindow(display,window);
			XDestroyWindow(display,window);
			window = 0;
		}
		if(context != NULL) {
			glXDestroyContext(display,context);
			context = NULL;
		}
		if(visual != NULL) {
			XFree(visual);
			visual = NULL;
		}
	#elif _MACOS
		if(context != NULL) {
			[(NSOpenGLContext*)context release];
			context = NULL;
		}
	#endif
	
	return 1;
}

void GLAppQt::resize_window() {
	#ifdef _LINUX
		XResizeWindow(x11Info().display(),window,getWidth(),getHeight());
	#elif _MACOS
		[(NSOpenGLContext*)context update];
	#endif
	glViewport(0,0,getWidth(),getHeight());
}

void GLAppQt::swap_window() {
	#ifdef _WIN32
		SwapBuffers(hdc);
	#elif _LINUX
		glXSwapBuffers(x11Info().display(),window);
	#elif _MACOS
		[(NSOpenGLContext*)context flushBuffer];
	#endif
}
