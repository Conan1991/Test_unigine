#pragma once
#include "c:\users\v.e.doronin\qt\x64\include\qtgui\qapplication.h"
#include "Qt/qdebug.h"
#include "session.h"


class IApplication : public QApplication
{
public:

    IApplication(int& argc, char ** argv) 
        : QApplication(argc, argv)
    {
        /**/
    }

    ~IApplication()  { /***/ }
    bool eventFilter(QObject*, QEvent*) override;

    void init()
    {
        countQapp = 0;
        this->installEventFilter(this);
    }
protected:
    int countQapp;
};

