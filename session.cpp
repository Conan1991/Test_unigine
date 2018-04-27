#include "session.h"
#include <QtCore/QTimer>
#include <QtCore/qeventloop.h>
#include <urlmon.h>
#include <UnigineGame.h>
#include <UnigineMathLib.h>
#include <QtCore/qcoreapplication.h>
#include "QtCore/qlist.h"

//QVector<Unigine::ObjectMeshDynamicPtr> Controller::objectsNew;
#define UnigineObjects objectsController::instance()->getObjects()


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
    /*QTimer *timer = new QTimer();
    timer->setInterval( 3000 );
    //QTimer::singleShot(10000, _worker, SIGNAL(finished()));
    
    connect( thread, SIGNAL( started() ), timer, SLOT( start() ) );
    connect(timer, SIGNAL(timeout()), _worker, SLOT(update_object())  );*/

    connect(thread, SIGNAL(started()), _worker, SLOT(do_work())   );
    connect(thread, SIGNAL(started()), _worker, SLOT(update_object())   );
    connect(_worker, SIGNAL(finished()), thread, SLOT(quit()));
    connect(_worker, SIGNAL(finished()), _worker, SLOT(deleteLater()));
    connect(_worker, SIGNAL(enable_object(int,int)), _worker, SLOT(do_enable_object(int, int)));
	connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();

    //Tthread * thread = new Tthread();
    //thread->start();
}

void session::turn_on()
{

}

/**************/
objectsController::objectsController(void)
{
   create_objects();
   //objectsNew[0]->getNode()->setEnabled(0);
   //objectsNew[1]->getNode()->setEnabled(0);
   updateTimer = new QTimer;
   updateTimer->setInterval(10);
   connect(updateTimer, SIGNAL(timeout()), this, SLOT(updateObjs()));
   updateTimer->start();
}


objectsController::~objectsController(void)
{
}

/*************/

void worker::do_work()
{
    
    forever{
        QEventLoop loop;
        QTimer::singleShot(2000, &loop, SLOT(quit()));
        loop.exec();
       /* if(!UnigineObjects.isEmpty() )
        UnigineObjects.erase(UnigineObjects.begin() );*/
        objectsController::instance()->eraseObjects();
        /*if(UnigineObjects.isEmpty() )
            objectsController::instance()->create_objects();*/
       //if(!objectsController::instance()->isEnabledObjects() )
        //{
            objectsTurnOffEvent *_event = new objectsTurnOffEvent;
            QCoreApplication::postEvent(qApp, _event);
       // }
        qDebug() << "Qthread working"<<  QThread::currentThread() << endl; 
    }   

}

void worker::do_enable_object(int numObj, int isEnabled)
{
    //Objects::objects[numObj]->getNode()->setEnabled(isEnabled);
    //
    //if (isEnabled == 1)
    //    qDebug() <<"obj"<< numObj << "is Enabled"<<endl;
    //else 
    //    qDebug() <<"obj"<< numObj << "is Disabled"<<endl;

}

void worker::update_object()
{
    //if( Objects::objects[0]->getNode()->isEnabled() == 1  )
    //{
    //    emit enable_object(0, 0);
    //    emit enable_object(2, 0);
    //}
    //else if ( Objects::objects[0]->getNode()->isEnabled() == 0  )
    //{
    //     emit enable_object(0, 1);
    //     emit enable_object(2, 1);
    //}
    //qDebug() << "Qthread working"<< endl; 
}



void objectsController::create_objects()
{
    objectsNew.append(create_object(Unigine::translate(UNIGINE_VEC3(-16.0f,  0.0f,0.0f))));
	objectsNew.append(create_object(Unigine::translate(UNIGINE_VEC3( 16.0f,  0.0f,0.0f))));
	objectsNew.append(create_object(Unigine::translate(UNIGINE_VEC3(  0.0f,-16.0f,0.0f))));
	objectsNew.append(create_object(Unigine::translate(UNIGINE_VEC3(  0.0f, 16.0f,0.0f))));
    //objectsNew.erase(objectsNew.begin());
}


Q_SLOT void objectsController::updateObjs()
{
    if(objectsNew.isEmpty())
        return;

    // game interface
	Unigine::Game *game = Unigine::Game::get();
    float time = game->getTime();
	for(int i = 0; i < objectsNew.size(); i++) {
			update_object(objectsNew[i], time + i);
	}
}

void objectsController::print()
{
    qDebug()<< "fgfdfg"<< endl;
}

void objectsController::enable_objects()
{
    for(int i = 0; i < objectsNew.size(); i++) {
        if(objectsNew[i]->getNode()->isEnabled() )
			objectsNew[i]->getNode()->setEnabled(0);
        else
            objectsNew[i]->getNode()->setEnabled(1);
	}
    
}

void objectsController::eraseObjects()
{
    if(!objectsNew.isEmpty() )
        objectsNew.erase(objectsNew.begin());
    else 
        create_objects();
}

bool objectsController::isEnabledObjects()
{
    if(objectsNew.isEmpty())
        return true;

    if(objectsNew.first()->getNode()->isEnabled() )
        return true;
    return false;
}


Unigine::ObjectMeshDynamicPtr objectsController::create_object(const UNIGINE_MAT4 &transform) {
	
	Unigine::ObjectMeshDynamicPtr object = Unigine::ObjectMeshDynamic::create(1);
	object->setWorldTransform(transform);
	
	for(int i = 0; i < object->getNumSurfaces(); i++) {
		object->setMaterial("mesh_base",i);
		object->setProperty("surface_base",i);
	}
	
	return object;
}

/*
 */
void objectsController::update_object(Unigine::ObjectMeshDynamicPtr object,float time) {
	
	object->clearVertex();
	object->clearIndices();
	
	int num = 1024;
	float inum = 1.0f / num;
	float scale = cosf(time * 2.0f) * 3.0f;
	
	object->addTriangleFan(num + 2);
	object->addVertex(Unigine::vec3(0.0f));
	object->addTexCoord(Unigine::vec4(0.0f));
	for(int i = 0; i <= num; i++) {
		float angle = Unigine::UNIGINE_PI * 2.0f * i * inum;
		float radius = 8.0f + sinf(angle * 8.0f - time) * scale;
		float x = sinf(angle);
		float y = cosf(angle);
		object->addVertex(Unigine::vec3(y * radius,x * radius,(radius - 8.0f) * 0.5f));
		object->addTexCoord(Unigine::vec4(x,y,0.0f,0.0f));
	}
	
	object->updateBounds();
	object->updateNormals();
	object->updateTangents();
	object->flush();
	
	for(int i = 0; i < object->getNumSurfaces(); i++) {
		Unigine::MaterialPtr material = object->getMaterialInherit(i);
		int id = material->findParameter("diffuse_color");
		if(id != -1) material->setParameter(id,Unigine::vec4(Unigine::vec3(sinf(time * 1.0f),cosf(time * 2.0f),sinf(time * 3.0f)) * 0.5f + Unigine::vec3(1.0f),1.0f));
	}
}
