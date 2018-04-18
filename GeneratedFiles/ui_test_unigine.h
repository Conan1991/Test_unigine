/********************************************************************************
** Form generated from reading UI file 'test_unigine.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TEST_UNIGINE_H
#define UI_TEST_UNIGINE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Test_unigineClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *Test_unigineClass)
    {
        if (Test_unigineClass->objectName().isEmpty())
            Test_unigineClass->setObjectName(QString::fromUtf8("Test_unigineClass"));
        Test_unigineClass->resize(600, 400);
        menuBar = new QMenuBar(Test_unigineClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        Test_unigineClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(Test_unigineClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        Test_unigineClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(Test_unigineClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        Test_unigineClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(Test_unigineClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        Test_unigineClass->setStatusBar(statusBar);

        retranslateUi(Test_unigineClass);

        QMetaObject::connectSlotsByName(Test_unigineClass);
    } // setupUi

    void retranslateUi(QMainWindow *Test_unigineClass)
    {
        Test_unigineClass->setWindowTitle(QApplication::translate("Test_unigineClass", "Test_unigine", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Test_unigineClass: public Ui_Test_unigineClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TEST_UNIGINE_H
