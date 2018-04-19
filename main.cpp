#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QMainWindow>
#include <QtGui/QApplication>

#include <Unigine.h>
#include "Objects.h"
#include "session.h"
#include "GLAppQt.h"
#include <QtCore/QTimer>

int main(int argc, char *argv[])
{
    // initialize Qt
	QApplication app(argc,argv);
	QObject::connect(&app,SIGNAL(lastWindowClosed()),&app,SLOT(quit()));
		
	// Main Window
	QMainWindow window;
	QAction quit("&Quit",&window);
	window.menuBar()->addMenu("&File")->addAction(&quit);
	QObject::connect(&quit,SIGNAL(triggered()),qApp,SLOT(quit()));
	window.setWindowTitle("GLAppQt");
	
	// OpenGL App Qt
	GLAppQt widget(&window);
	QObject::connect(&widget,SIGNAL(closed()),qApp,SLOT(quit()));
	window.setCentralWidget(&widget);
	
	// show window
	window.show();
	
	// initialize Engine
	Unigine::Engine::init(UNIGINE_VERSION,&widget,argc,argv);
	//new Objects();
    ///Thread *thread = new Thread();
    //thread->run();
	// initialize App
    session *sess =new session();
    new Objects();
	widget.init();
	QTimer::singleShot(10000, sess, SLOT(turn_on()));
  
	// main loop
	app.exec();
	// shutdown engine
	Unigine::Engine::shutdown();
	
	return 0;
}
