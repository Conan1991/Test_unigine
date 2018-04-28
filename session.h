#pragma once
#include <QtCore/qobjectdefs.h>
#include <QtCore/qstring.h>
#include <QtCore/qobject.h>
#include <QtCore/qthread.h>
#include <QtCore/qmutex.h>
#include <UnigineObjectMeshDynamic.h>
#include <QtCore/qlist.h>
#include "Qt/qdebug.h"
#include <QtCore/qvector.h>
#include <QtCore/QTimer>
#include <QMouseEvent>

#include "singleton.h"

class session : public QObject
{
    Q_OBJECT
public:
    session(void);
    ~session(void);
    Q_SLOT void turn_on();
private:
    void AddThread();
    
};

class worker: public QObject
{
    Q_OBJECT
public:
    worker(){};
    Q_SIGNAL void finished();
    Q_SLOT void do_work();
    Q_SIGNAL void enable_object(int,int);
    Q_SLOT void do_enable_object(int,int);
    Q_SLOT void update_object();
};

struct stateSetter
{
    QVector<int> whitchObjGetOff;
    bool mode;
    int pos;

};


class objectsController: public QObject
{
    Q_OBJECT
public:
    objectsController();
    ~objectsController();
    void create_objects();
    static Unigine::ObjectMeshDynamicPtr create_object(const UNIGINE_MAT4 &transform);
    static void update_object(Unigine::ObjectMeshDynamicPtr object,float time);

    Q_SLOT void updateObjs();
    Q_SLOT void print();
    void enable_objects();

    QList<Unigine::ObjectMeshDynamicPtr> objectsNew;

    QTimer *updateTimer;
    bool CheckAllZero();
    bool CheckNonZero();

    void eraseObjects();
    bool isEnabledObjects();

    static objectsController* createInstance()
    { return new objectsController(); }
    static objectsController* instance()
    { return Singleton<objectsController>::instance(); }
private:
    QMutex mutex;
    int count;
    stateSetter setter;

};





class objectsTurnOffEvent: public QEvent
{
public:
    objectsTurnOffEvent(): QEvent((Type)1005)
    {}
    ~objectsTurnOffEvent(){};
};



