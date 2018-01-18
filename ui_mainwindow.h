/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QLabel *label;
    QLabel *trans_label;
    QPushButton *resetPushB;
    QPushButton *runPushB;
    QPushButton *colorPushB;
    QPushButton *brwsePushB;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(640, 534);
        MainWindow->setStyleSheet(QStringLiteral(""));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(320, 10, 61, 31));
        trans_label = new QLabel(centralWidget);
        trans_label->setObjectName(QStringLiteral("trans_label"));
        trans_label->setGeometry(QRect(420, 40, 61, 31));
        trans_label->setStyleSheet(QStringLiteral("background-color: rgba(0, 0, 0, 0);"));
        resetPushB = new QPushButton(centralWidget);
        resetPushB->setObjectName(QStringLiteral("resetPushB"));
        resetPushB->setGeometry(QRect(120, 500, 121, 30));
        resetPushB->setStyleSheet(QStringLiteral("background-color: rgb(169, 233, 224);"));
        runPushB = new QPushButton(centralWidget);
        runPushB->setObjectName(QStringLiteral("runPushB"));
        runPushB->setGeometry(QRect(0, 500, 121, 30));
        runPushB->setStyleSheet(QStringLiteral("background-color: rgb(169, 233, 224);"));
        colorPushB = new QPushButton(centralWidget);
        colorPushB->setObjectName(QStringLiteral("colorPushB"));
        colorPushB->setGeometry(QRect(120, 470, 121, 30));
        colorPushB->setStyleSheet(QStringLiteral("background-color: rgb(169, 233, 224);"));
        brwsePushB = new QPushButton(centralWidget);
        brwsePushB->setObjectName(QStringLiteral("brwsePushB"));
        brwsePushB->setGeometry(QRect(150, 350, 301, 30));
        brwsePushB->setStyleSheet(QStringLiteral("background-color: rgb(169, 233, 224);"));
        MainWindow->setCentralWidget(centralWidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        label->setText(QString());
        trans_label->setText(QString());
        resetPushB->setText(QApplication::translate("MainWindow", "Reset", 0));
        runPushB->setText(QApplication::translate("MainWindow", "Run", 0));
        colorPushB->setText(QApplication::translate("MainWindow", "Press for color", 0));
        brwsePushB->setText(QApplication::translate("MainWindow", "Browse Image", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
