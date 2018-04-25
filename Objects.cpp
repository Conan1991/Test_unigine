#include "Objects.h"
#include <UnigineGame.h>
#include <QtCore/QTimer>
#include <QtCore/qeventloop.h>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <functional>

#define M_PI       3.14159265358979323846
QVector<Unigine::ObjectMeshDynamicPtr> Objects::objects;


Objects::Objects(void)
{
    create_objects();
    objects[1]->getNode()->setHandled(0);
    objects[1]->getNode()->setLatest(1);
     //objects[3]->getNode()->setEnabled(0);


}

void Objects::create_objects()
{
    objects.append(create_object(translate(UNIGINE_VEC3(-16.0f,  0.0f,0.0f))));
	objects.append(create_object(translate(UNIGINE_VEC3( 16.0f,  0.0f,0.0f))));
	objects.append(create_object(translate(UNIGINE_VEC3(  0.0f,-16.0f,0.0f))));
	objects.append(create_object(translate(UNIGINE_VEC3(  0.0f, 16.0f,0.0f))));
}


Objects::~Objects(void)
{
}
void Objects::update()
{
    if(objects.isEmpty())
        return;

    // game interface
	Unigine::Game *game = Unigine::Game::get();
    float time = game->getTime();
	for(int i = 0; i < objects.size(); i++) {
			update_object(objects[i], time + i);
	}
}

void Thread::process()
{
forever{
    QEventLoop loop;
    QTimer::singleShot(2000, &loop, SLOT(quit()));
    loop.exec();
    if (Objects::objects[1]->getNode()->isEnabled() )
    {
        Objects::objects[1]->getNode()->setEnabled(0);
    }
    else
    {
        Objects::objects[1]->getNode()->setEnabled(1);
    }

    if ( Objects::objects[1]->getNode()->isEnabled() )
        qDebug() <<"obj"<< 1 << "is Enabled"<<endl;
    else 
        qDebug() <<"obj"<< 1 << "is Disabled"<<endl;
}

}

Unigine::ObjectMeshDynamicPtr Objects::create_object(const UNIGINE_MAT4 &transform) {
	
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
void Objects::update_object(Unigine::ObjectMeshDynamicPtr object,float time) {
	
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