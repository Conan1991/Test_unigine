#pragma once

#include <Unigine.h>
#include <UnigineObjectMeshDynamic.h>
#include <UnigineMathLib.h>
#include <QtCore/qvector.h>
#include "UnigineThread.h"
#include "Qt/qdebug.h"

class Objects
{
public:
    Objects(void);
    
    ~Objects(void);

    static void create_objects();
    static Unigine::ObjectMeshDynamicPtr create_object(const UNIGINE_MAT4 &transform);
    static void update_object(Unigine::ObjectMeshDynamicPtr object,float time);

    static void update();
    static QVector<Unigine::ObjectMeshDynamicPtr> objects;
};

class Thread: public Unigine::Thread
{
private:
    void process();
    
};