#pragma once
#include <QtCore/qobjectdefs.h>
#include <QtCore/qstring.h>
#include <QtCore/qobject.h>
#include <QtCore/qthread.h>
#include "Objects.h"


class session : public QObject
{
    Q_OBJECT
public:
    session(void);
    ~session(void);
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
    Q_SIGNAL void enable_object(int,bool);
    Q_SLOT void do_enable_object(int,bool);
};

