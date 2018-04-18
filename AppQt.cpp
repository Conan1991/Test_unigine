/* Copyright (C) 2005-2014, Unigine Corp. All rights reserved.
 *
 * File:    AppQt.cpp
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

#include <QtGui/QApplication>
#include <QtGui/QMouseEvent>
#include <QtGui/QMessageBox>
#include <QtGui/QFileDialog>

#include "AppQt.h"
#include "Objects.h"
/*
 */
AppQt::AppQt(QWidget *parent,Qt::WindowFlags flags) : QWidget(parent,flags) {
	
	initialized = 0;
	
	timer_id = 0;
	
	window_width = 0;
	window_height = 0;
	window_flags = 0;
	
	mouse_x = 0;
	mouse_y = 0;
	memset(mouse_axes,0,sizeof(mouse_axes));
	mouse_button = 0;
	mouse_release = 0;
	mouse_grab = 0;
	mouse_show = 1;
	
	memset(keys,0,sizeof(keys));
	
	setFocusPolicy(Qt::StrongFocus);
	setAttribute(Qt::WA_PaintOnScreen);
	setAttribute(Qt::WA_NoSystemBackground);
	
	setMouseTracking(true);
	grabKeyboard();
	
	setTitle("AppQt");
}

AppQt::~AppQt() {
	
}

/*
 */
void AppQt::init() {
	initialized = 1;
}

/******************************************************************************\
*
* Events
*
\******************************************************************************/

/*
 */
QPaintEngine *AppQt::paintEngine() const {
	return NULL;
}

void AppQt::paintEvent(QPaintEvent *event) {
	
	Q_UNUSED(event);
	
	mouse_button &= ~mouse_release;
	mouse_release = 0;
	
	// update engine
	if(initialized && timer_id == 0) {
		App::update();
		App::render();
		App::swap();
		swap_window();
	}
	
	// update widget
	QWidget::update();
	Objects::update();

	// clear mouse axes
	for(int i = 0; i < NUM_AXES; i++) {
		mouse_axes[i] = 0;
	}
	
	// release mouse button
	if(mouse_button & BUTTON_DCLICK) {
		mouse_button &= ~BUTTON_DCLICK;
		buttonRelease(BUTTON_DCLICK);
	}
}

void AppQt::timerEvent(QTimerEvent *event) {
	
	Q_UNUSED(event);
	
	killTimer(timer_id);
	timer_id = 0;
	
	int width = size().width();
	int height = size().height();
	if(window_width != width || window_height != height) {
		window_width = width;
		window_height = height;
		resize_window();
	}
}

void AppQt::resizeEvent(QResizeEvent *event) {
	
	Q_UNUSED(event);
	
	if(timer_id != 0) killTimer(timer_id);
	timer_id = startTimer(100);
}

/*
 */
void AppQt::mousePressEvent(QMouseEvent *event) {
	if(event->button() & Qt::LeftButton) {
		mouse_button |= BUTTON_LEFT;
		buttonPress(BUTTON_LEFT);
	}
	if(event->button() & Qt::MidButton) {
		mouse_button |= BUTTON_MIDDLE;
		buttonPress(BUTTON_MIDDLE);
	}
	if(event->button() & Qt::RightButton) {
		mouse_button |= BUTTON_RIGHT;
		buttonPress(BUTTON_RIGHT);
	}
}

void AppQt::mouseReleaseEvent(QMouseEvent *event) {
	if((event->button() & Qt::LeftButton) && (mouse_button & BUTTON_LEFT)) {
		mouse_release |= BUTTON_LEFT;
		buttonRelease(BUTTON_LEFT);
	}
	if((event->button() & Qt::MidButton) && (mouse_button & BUTTON_MIDDLE)) {
		mouse_release |= BUTTON_MIDDLE;
		buttonRelease(BUTTON_MIDDLE);
	}
	if((event->button() & Qt::RightButton) && (mouse_button & BUTTON_RIGHT)) {
		mouse_release |= BUTTON_RIGHT;
		buttonRelease(BUTTON_RIGHT);
	}
}

void AppQt::mouseDoubleClickEvent(QMouseEvent *event) {
	Q_UNUSED(event);
	mouse_button |= BUTTON_DCLICK;
	buttonPress(BUTTON_DCLICK);
}

void AppQt::wheelEvent(QWheelEvent *event) {
	if(event->orientation() == Qt::Horizontal) {
		mouse_axes[AXIS_X] += event->delta() / 120;
	} else {
		mouse_axes[AXIS_Y] += event->delta() / 120;
	}
}

void AppQt::mouseMoveEvent(QMouseEvent *event) {
	mouse_x = event->x();
	mouse_y = event->y();
}

/*
 */
unsigned int AppQt::translate_key(int key) {
	unsigned int ret = 0;
	switch(key) {
		case Qt::Key_Escape: ret = KEY_ESC; break;
		case Qt::Key_Tab: ret = KEY_TAB; break;
		case Qt::Key_Backspace: ret = KEY_BACKSPACE; break;
		case Qt::Key_Return: ret = KEY_RETURN; break;
		case Qt::Key_Delete: ret = KEY_DELETE; break;
		case Qt::Key_Insert: ret = KEY_INSERT; break;
		case Qt::Key_Home: ret = KEY_HOME; break;
		case Qt::Key_End: ret = KEY_END; break;
		case Qt::Key_PageUp: ret = KEY_PGUP; break;
		case Qt::Key_PageDown: ret = KEY_PGDOWN; break;
		case Qt::Key_Left: ret = KEY_LEFT; break;
		case Qt::Key_Right: ret = KEY_RIGHT; break;
		case Qt::Key_Up: ret = KEY_UP; break;
		case Qt::Key_Down: ret = KEY_DOWN; break;
		case Qt::Key_Shift: ret = KEY_SHIFT; break;
		case Qt::Key_Control: ret = KEY_CTRL; break;
		case Qt::Key_Alt: ret = KEY_ALT; break;
		case Qt::Key_F1: ret = KEY_F1; break;
		case Qt::Key_F2: ret = KEY_F2; break;
		case Qt::Key_F3: ret = KEY_F3; break;
		case Qt::Key_F4: ret = KEY_F4; break;
		case Qt::Key_F5: ret = KEY_F5; break;
		case Qt::Key_F6: ret = KEY_F6; break;
		case Qt::Key_F7: ret = KEY_F7; break;
		case Qt::Key_F8: ret = KEY_F8; break;
		case Qt::Key_F9: ret = KEY_F9; break;
		case Qt::Key_F10: ret = KEY_F10; break;
		case Qt::Key_F11: ret = KEY_F11; break;
		case Qt::Key_F12: ret = KEY_F12; break;
		default: {
			if(key < NUM_KEYS) {
				ret = key;
				if(ret >= 'A' && ret <= 'Z') ret -= 'A' - 'a';
				else if(ret == '!') ret = '1';
				else if(ret == '@') ret = '2';
				else if(ret == '#') ret = '3';
				else if(ret == '$') ret = '4';
				else if(ret == '%') ret = '5';
				else if(ret == '^') ret = '6';
				else if(ret == '&') ret = '7';
				else if(ret == '*') ret = '8';
				else if(ret == '(') ret = '9';
				else if(ret == ')') ret = '0';
				else if(ret == '_') ret = '-';
				else if(ret == '+') ret = '=';
				else if(ret == '{') ret = '[';
				else if(ret == '}') ret = ']';
				else if(ret == '|') ret = '\\';
				else if(ret == ':') ret = ';';
				else if(ret == '"') ret = '\'';
				else if(ret == '>') ret = '.';
				else if(ret == '?') ret = '/';
			}
		}
	}
	return ret;
}

void AppQt::keyPressEvent(QKeyEvent *event) {
	unsigned int key = translate_key(event->key());
	if(key < NUM_KEYS) keys[key] = 1;
	if(key >= KEY_ESC && key <= KEY_F12) {
		keyPressUnicode(key);
	} else {
		const QChar *s = event->text().unicode();
		if(s) keyPressUnicode(s->unicode());
	}
	if(key) keyPress(key);
}

void AppQt::keyReleaseEvent(QKeyEvent *event) {
	unsigned int key = translate_key(event->key());
	if(key < NUM_KEYS) keys[key] = 0;
	if(key) keyRelease(key);
}

/******************************************************************************\
*
* App interface
*
\******************************************************************************/

/*
 */
void *AppQt::getHandle() {
	#ifdef _WIN32
		return internalWinId();
	#else
		return NULL;
	#endif
}

/*
 */
int AppQt::setVideoMode(int width,int height,int flags,int refresh) {
	
	Q_UNUSED(refresh);
	
	window_width = width;
	window_height = height;
	window_flags = flags;
	Unigine::Log::message("Set %s video mode\n",getVideoModeName());
	
	if(window_flags & RESIZABLE) {
		setMinimumSize(width,height);
		setMaximumSize(1000000,1000000);
		resize(width,height);
	} else {
		setFixedSize(width,height);
		resize(width,height);
	}
	
	QCoreApplication::processEvents();
	
	return create_context();
}

int AppQt::restoreVideoMode() {
	return destroy_context();
}

int AppQt::setUpdate(int update) {
	Q_UNUSED(update);
	return 1;
}

int AppQt::setGamma(float gamma) {
	Q_UNUSED(gamma);
	return 1;
}

int AppQt::setTitle(const char *title) {
	setWindowTitle(title);
	return 1;
}

int AppQt::setIcon(const unsigned char *data,int size) {
	Q_UNUSED(data);
	Q_UNUSED(size);
	return 0;
}

int AppQt::getWidth() {
	return window_width;
}

int AppQt::getHeight() {
	return window_height;
}

int AppQt::getFlags() {
	return window_flags;
}

int AppQt::getUpdate() {
	return 0;
}

/*
 */
void AppQt::setMouse(int x,int y) {
	mouse_x = x;
	mouse_y = y;
	QCursor::setPos(mapToGlobal(QPoint(x,y)));
}

void AppQt::setMouseGrab(int grab) {
	if(mouse_grab == grab) return;
	mouse_grab = grab;
	if(grab) grabMouse();
	else releaseMouse();
}

void AppQt::setMouseShow(int show) {
	if(mouse_show == show) return;
	mouse_show = show;
	if(show) setCursor(QCursor(Qt::ArrowCursor));
	else setCursor(QCursor(Qt::BlankCursor));
}

int AppQt::setMouseCursor(const unsigned char *data,int size,int x,int y) {
	Q_UNUSED(data);
	Q_UNUSED(size);
	Q_UNUSED(x);
	Q_UNUSED(y);
	return 0;
}

int AppQt::getMouseX() {
	return mouse_x;
}

int AppQt::getMouseY() {
	return mouse_y;
}

int AppQt::getMouseGrab() {
	return mouse_grab;
}

int AppQt::getMouseShow() {
	return mouse_show;
}

void AppQt::setMouseAxis(int axis,int value) {
	if(axis < 0 || axis >= NUM_AXES) return;
	mouse_axes[axis] = value;
}

int AppQt::getMouseAxis(int axis) {
	if(axis < 0 || axis >= NUM_AXES) return 0;
	return mouse_axes[axis];
}

int AppQt::clearMouseAxis(int axis) {
	if(axis < 0 || axis >= NUM_AXES) return 0;
	int ret = mouse_axes[axis];
	mouse_axes[axis] = 0;
	return ret;
}

void AppQt::setMouseButton(int button) {
	mouse_button = button;
}

int AppQt::getMouseButton() {
	return mouse_button;
}

int AppQt::getMouseButtonState(int button) {
	return ((mouse_button & button) != 0);
}

int AppQt::clearMouseButtonState(int button) {
	int ret = ((mouse_button & button) != 0);
	mouse_button &= ~button;
	return ret;
}

/*
 */
void AppQt::setKeyState(int key,int state) {
	keys[key] = state;
}

int AppQt::getKeyState(int key) {
	return keys[key];
}

int AppQt::clearKeyState(int key) {
	int ret = keys[key];
	keys[key] = 0;
	return ret;
}

/*
 */
int AppQt::isDone() {
	return 0;
}

void AppQt::doUpdate() {
	
}

void AppQt::doRender() {
	
}

void AppQt::doSwap() {
	swap_window();
}

void AppQt::exit() {
	close();
	closed();
}

/*
 */
int AppQt::dialogMessage(const char *title,const char *str) {
	int ret = QMessageBox::warning(this,title,str,QMessageBox::Ok,QMessageBox::Cancel);
	return (ret == QMessageBox::Ok);
}

int AppQt::dialogFile(const char *title,char *name,int size) {
	QString str = QFileDialog::getOpenFileName(this,title,name,QString::null,0);
	strncpy(name,str.toUtf8().data(),size);
	name[size - 1] = '\0';
	return 1;
}
