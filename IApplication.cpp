#include "IApplication.h"

bool IApplication::eventFilter(QObject *object, QEvent *event)
{
    if (event->type() == 1005)
    {
        objectsController::instance()->enable_objects();
        qDebug() << "MAIN THREAD WORKING " << QThread::currentThread();
    }
    return QApplication::eventFilter(object, event);
}
