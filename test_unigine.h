#ifndef TEST_UNIGINE_H
#define TEST_UNIGINE_H

#include <QtGui/QMainWindow>
#include "ui_test_unigine.h"

class Test_unigine : public QMainWindow
{
    Q_OBJECT

public:
    Test_unigine(QWidget *parent = 0, Qt::WFlags flags = 0);
    ~Test_unigine();

private:
    Ui::Test_unigineClass ui;
};

#endif // TEST_UNIGINE_H
