#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QMainWindow>
#include <QtGui/QApplication>

#include <Unigine.h>
#include "Objects.h"
#include "session.h"
#include "GLAppQt.h"
#include <QtCore/QTimer>
#include <qt\qtconcurrentrun.h>
#include "IApplication.h"

using namespace QtConcurrent;


void process(QString name)
{
    qDebug() << Objects::objects[1]->getNode()->isEnabled() << endl;
    Objects::objects[1]->getNode()->setEnabled(1);
    qDebug() << "Hello" << name << Objects::objects[1]->getNode()->isEnabled() << QThread::currentThread();
}

int main(int argc, char *argv[])
{
    // initialize Qt
	IApplication app(argc,argv);
   
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
    app.init();

    objectsController::instance();
    //new Objects();
    //Thread *thread = new Thread();
    //thread->run();
	// initialize App
    session *sess = new session();
    //QTimer::singleShot(10000, sess, SLOT(turn_on()));
    

    //QFuture<void> f1 = run(process, QString("trtr"));
    //f1.progressText();
	widget.init();
	
  
	// main loop
	app.exec();
    
    //thread->stop();
	// shutdown engine
	Unigine::Engine::shutdown();
	
	return 0;
}


