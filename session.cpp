#include "session.h"
#include <QtCore/QTimer>
#include <QtCore/qeventloop.h>
#include <urlmon.h>


void do_enable_object(int numObj, int isEnabled)
{
    if (isEnabled == 1)
    {
        Objects::objects[numObj]->getNode()->release();
        Objects::objects[numObj]->getNode()->setEnabled(1);
        Objects::objects[numObj]->getNode()->grab();
    }
    else
    {

        Objects::objects[numObj]->getNode()->grab();
        Objects::objects[numObj]->getNode()->setEnabled(0);
        Objects::objects[numObj]->getNode()->release();
        //Objects::objects[numObj]->getNode()->grab();
    }
    if ( Objects::objects[numObj]->getNode()->isEnabled() )
        qDebug() <<"obj"<< numObj << "is Enabled"<<endl;
    else 
        qDebug() <<"obj"<< numObj << "is Disabled"<<endl;

}

session::session(void)
{
    AddThread();
}


session::~session(void)
{
}

void session::AddThread()
{
 //   worker *_worker = new worker();
 //   QThread *thread = new QThread;

 //QTimer *timer = new QTimer();
 //timer->setInterval( 3000 );
 //   //QTimer::singleShot(10000, _worker, SIGNAL(finished()));
 //   _worker->moveToThread(thread);
 //   connect( thread, SIGNAL( started() ), timer, SLOT( start() ) );
 //   connect(timer, SIGNAL(timeout()), _worker, SLOT(update_object())  );
 //   connect(thread, SIGNAL(started()), _worker, SLOT(do_work())   );
 //   connect(thread, SIGNAL(started()), _worker, SLOT(update_object())   );
 //   connect(_worker, SIGNAL(finished()), thread, SLOT(quit()));
 //   connect(_worker, SIGNAL(finished()), _worker, SLOT(deleteLater()));
 //   connect(_worker, SIGNAL(enable_object(int,int)), _worker, SLOT(do_enable_object(int, int)));
	//connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
 //   thread->start();

    Tthread * thread = new Tthread();
    thread->start();
}

void session::turn_on()
{
    Objects::objects[3]->getNode()->setEnabled(1);
    qDebug() << "Object 3 is enabled";
}

void worker::do_work()
{

    //emit enable_object(3, 1);
    //emit enable_object(3, false);
    //emit finished();
}

void worker::do_enable_object(int numObj, int isEnabled)
{
    Objects::objects[numObj]->getNode()->setEnabled(isEnabled);
    
    if (isEnabled == 1)
        qDebug() <<"obj"<< numObj << "is Enabled"<<endl;
    else 
        qDebug() <<"obj"<< numObj << "is Disabled"<<endl;

}

void worker::update_object()
{
    if( Objects::objects[0]->getNode()->isEnabled() == 1  )
    {
        emit enable_object(0, 0);
        emit enable_object(2, 0);
    }
    else if ( Objects::objects[0]->getNode()->isEnabled() == 0  )
    {
         emit enable_object(0, 1);
         emit enable_object(2, 1);
    }
    qDebug() << "Qthread working"<< endl; 
}

void Tthread::run()
{

    forever{
        QEventLoop loop;
        QTimer::singleShot(2000, &loop, SLOT(quit()));
        loop.exec();
        if( Objects::objects[0]->getNode()->isEnabled() == 1  )
        {
            do_enable_object(0, 0);
            //do_enable_object(2, 0);
        }
        else if ( Objects::objects[0]->getNode()->isEnabled() == 0  )
        {
            do_enable_object(0, 1);
            //do_enable_object(2, 1);
        }
        qDebug() << "Qthread working"<< endl; 
    }
}
