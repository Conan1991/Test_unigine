#include "session.h"
#include <QtCore/QTimer>
#include <QtCore/qeventloop.h>


session::session(void)
{
    AddThread();
}


session::~session(void)
{
}

void session::AddThread()
{
    worker *_worker = new worker();
    QThread *thread = new QThread;

    QTimer *timer = new QTimer();
    timer->setInterval( 1000 );
    QTimer::singleShot(30000, _worker, SIGNAL(finished()));
    _worker->moveToThread(thread);
    connect( thread, SIGNAL( started() ), timer, SLOT( start() ) );
    connect(timer, SIGNAL(timeout()), _worker, SLOT(update_object())  );
    connect(thread, SIGNAL(started()), _worker, SLOT(do_work())   );
    connect(thread, SIGNAL(started()), _worker, SLOT(update_object())   );
    connect(_worker, SIGNAL(finished()), thread, SLOT(quit()));
    connect(_worker, SIGNAL(finished()), _worker, SLOT(deleteLater()));
    connect(_worker, SIGNAL(enable_object(int,int)), _worker, SLOT(do_enable_object(int, int)));
	connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();
}

void worker::do_work()
{
    Objects::create_objects();

    emit enable_object(3, 1);
    //emit enable_object(3, false);
    //emit finished();
}

void worker::do_enable_object(int numObj, int isEnabled)
{
    Objects::objects[numObj]->getNode()->setEnabled(isEnabled);
}

void worker::update_object()
{
    if( Objects::objects[0]->getNode()->isEnabled()  )
    {
        emit enable_object(0, 0);
        emit enable_object(3, 0);
    }
    else 
    {
         emit enable_object(0, 1);
         emit enable_object(3, 1);
    }
}
