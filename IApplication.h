#pragma once
#include "c:\users\v.e.doronin\qt\x64\include\qtgui\qapplication.h"
#include "session.h"
#include "Qt/qlist.h"
#include <UnigineObjectMeshDynamic.h>

class IApplication :   public QApplication
{
public:
    QList<Unigine::ObjectMeshDynamicPtr> objects;


    IApplication(void);
    ~IApplication(void);
};

class Controller
{
    Q_OBJECT
public:
    Controller();
    ~Controller();

};