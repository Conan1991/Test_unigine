/* Copyright (C) 2005-2014, Unigine Corp. All rights reserved.
 *
 * File:    GLAppQt.h
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

#ifndef __GL_APP_QT_H__
#define __GL_APP_QT_H__

#include <AppQt.h>

#ifdef _WIN32
	#include <windows.h>
	#include <GL/gl.h>
	#include "GL/wglext.h"
#elif _LINUX
	#include <QtGui/QX11Info>
	#include <GL/glx.h>
	#include <GL/glxext.h>
#endif

#include <Unigine.h>
#include <UnigineApp.h>

/*
 */
class GLAppQt : public AppQt {
		
		Q_OBJECT
		
	public:
		
		GLAppQt(QWidget *parent = 0,Qt::WindowFlags flags = 0);
		virtual ~GLAppQt();
		
		virtual void *getHandle();
		
		virtual const char *getName();
		
	private:
		
		virtual int create_context();
		virtual int destroy_context();
		virtual void resize_window();
		virtual void swap_window();
		
		#ifdef _WIN32
			HDC hdc;
			HGLRC context;
			PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB;
		#elif _LINUX
			XVisualInfo *visual;
			GLXContext context;
			Window window;
		#elif _MACOS
			void *context;
		#endif
};

#endif /* __GL_APP_QT_H__ */
