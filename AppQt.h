/* Copyright (C) 2005-2014, Unigine Corp. All rights reserved.
 *
 * File:    AppQt.h
 * Desc:    App Qt widget
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

#ifndef __APP_QT_H__
#define __APP_QT_H__

#include <QtGui/QWidget>

#include <Unigine.h>
#include <UnigineApp.h>

/*
 */
class AppQt : public QWidget, public Unigine::App {
		
		Q_OBJECT
		
	public:
		
		AppQt(QWidget *parent = 0,Qt::WindowFlags flags = 0);
		virtual ~AppQt();
		
		// initialize
		void init();
		
	protected:
		
		// window events
		virtual QPaintEngine *paintEngine() const;
		virtual void paintEvent(QPaintEvent *event);
		virtual void timerEvent(QTimerEvent *event);
		virtual void resizeEvent(QResizeEvent *event);
		
		// mouse events
		virtual void mousePressEvent(QMouseEvent *event);
		virtual void mouseReleaseEvent(QMouseEvent *event);
		virtual void mouseDoubleClickEvent(QMouseEvent *event);
		virtual void mouseMoveEvent(QMouseEvent *event);
		virtual void wheelEvent(QWheelEvent *event);
		
		// keyboard events
		virtual void keyPressEvent(QKeyEvent *event);
		virtual void keyReleaseEvent(QKeyEvent *event);
		
	signals:
		
		// signals
		void closed();
		
	public:
		
		// app handle
		virtual void *getHandle();
		
		// video mode
		virtual int setVideoMode(int width,int height,int flags = 0,int refresh = 0);
		virtual int restoreVideoMode();
		virtual int setUpdate(int update);
		virtual int setGamma(float gamma);
		virtual int setTitle(const char *title);
		virtual int setIcon(const unsigned char *data,int size);
		virtual int getWidth();
		virtual int getHeight();
		virtual int getFlags();
		virtual int getUpdate();
		
		// mouse
		virtual void setMouse(int x,int y);
		virtual void setMouseGrab(int grab);
		virtual void setMouseShow(int show);
		virtual int setMouseCursor(const unsigned char *data,int size,int x = 0,int y = 0);
		virtual int getMouseX();
		virtual int getMouseY();
		virtual int getMouseGrab();
		virtual int getMouseShow();
		virtual void setMouseAxis(int axis,int value);
		virtual int getMouseAxis(int axis);
		virtual int clearMouseAxis(int axis);
		virtual void setMouseButton(int button);
		virtual int getMouseButton();
		virtual int getMouseButtonState(int button);
		virtual int clearMouseButtonState(int button);
		
		// keyboard
		virtual void setKeyState(int key,int state);
		virtual int getKeyState(int key);
		virtual int clearKeyState(int key);
		
		// main loop
		virtual int isDone();
		virtual void doUpdate();
		virtual void doRender();
		virtual void doSwap();
		virtual void exit();
		
		// dialogs
		virtual int dialogMessage(const char *title,const char *str);
		virtual int dialogFile(const char *title,char *name,int size);
		
	private:
		
		unsigned int translate_key(int key);
		
		virtual int create_context() = 0;
		virtual int destroy_context() = 0;
		virtual void resize_window() = 0;
		virtual void swap_window() = 0;
		
		int initialized;			// initialized flag
		
		int timer_id;				// timer id
		
		int window_width;			// window
		int window_height;
		int window_flags;
		
		int mouse_x;				// mouse
		int mouse_y;
		int mouse_axes[NUM_AXES];
		int mouse_button;
		int mouse_release;
		int mouse_grab;
		int mouse_show;
		
		int keys[NUM_KEYS];			// keyboard
};

#endif /* __APP_QT_H__ */
