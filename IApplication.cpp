#include "IApplication.h"

bool IApplication::eventFilter(QObject *object, QEvent *event)
{
    if (event->type() == 1005)
    {
        countQapp++;
        objectsController::instance()->enable_objects();
        qDebug() << "MAIN THREAD WORKING " << QThread::currentThread() <<"count " << countQapp << endl;
        if(countQapp == 376)
        {
            qDebug() <<"what's up man?";
        }
    }
    return QApplication::eventFilter(object, event);
}
