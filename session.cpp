#include "session.h"


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
    _worker->moveToThread(thread);
    connect(thread, SIGNAL(started()), _worker, SLOT(do_work())   );
    connect(_worker, SIGNAL(finished()), thread, SLOT(quit()));
    connect(_worker, SIGNAL(finished()), _worker, SLOT(deleteLater()));
    connect(_worker, SIGNAL(enable_object(int,bool)), _worker, SLOT(do_enable_object(int, bool)));
	connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();
}

void worker::do_work()
{
    Objects::create_objects();
    emit enable_object(0, true);
    emit enable_object(3, false);
    emit finished();
}

void worker::do_enable_object(int numObj, bool isEnabled)
{
    Objects::objects[numObj]->getNode()->setEnabled(isEnabled);
}
