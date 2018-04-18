/* Copyright (C) 2005-2014, Unigine Corp. All rights reserved.
 *
 * File:    QUnigine.h
 * Desc:    Unigine Qt object
 * Version: 1.01
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

#ifndef __QUNIGINE_H__
#define __QUNIGINE_H__

#include <QtCore/QTimer>
#include <QtCore/QObject>

#include <Unigine.h>

/*
 */
class QUnigine : public QObject {
		
		Q_OBJECT
		
	public:
		
		QUnigine(int argc,char **argv) {
			
			timer = new QTimer();
			
			engine = Unigine::Engine::init(UNIGINE_VERSION,argc,argv);
			
			connect(timer,SIGNAL(timeout()),this,SLOT(update()));
			timer->start(0);
		}
		virtual ~QUnigine() {
			
			delete timer;
			
			Unigine::Engine::shutdown();
		}
		
	public slots:
		
		void update() {
			
			if(engine->isDone()) {
				emit quit();
			} else {
				engine->update();
				engine->render();
				engine->swap();
			}
		}
		
	signals:
		
		void quit();
		
	private:
		
		QTimer *timer;
		Unigine::Engine *engine;
};

#endif /* __QUNIGINE_H__ */
