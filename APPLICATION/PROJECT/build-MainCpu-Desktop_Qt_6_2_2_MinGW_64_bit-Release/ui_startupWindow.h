/********************************************************************************
** Form generated from reading UI file 'startupWindow.ui'
**
** Created by: Qt User Interface Compiler version 6.2.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_STARTUPWINDOW_H
#define UI_STARTUPWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_startupWindow
{
public:
    QFrame *frame;
    QLabel *label;
    QPushButton *pushButton;
    QPushButton *err1PushButton;
    QPushButton *err2PushButton;
    QPushButton *err3PushButton;
    QPushButton *openErrWindowPushButton;
    QPushButton *exitPushButton;

    void setupUi(QWidget *startupWindow)
    {
        if (startupWindow->objectName().isEmpty())
            startupWindow->setObjectName(QString::fromUtf8("startupWindow"));
        startupWindow->resize(600, 600);
        startupWindow->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 170, 0);"));
        frame = new QFrame(startupWindow);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setGeometry(QRect(0, 0, 600, 600));
        frame->setStyleSheet(QString::fromUtf8("border-image: url(:/Sym/Sym/ImmagineTest.png);\n"
""));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        label = new QLabel(frame);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(210, 30, 191, 61));
        label->setStyleSheet(QString::fromUtf8("border-image: url(:/Sym/Sym/a.png);\n"
"color: rgb(255, 255, 0);\n"
"background-color: rgb(85, 0, 0);"));
        label->setAlignment(Qt::AlignCenter);
        pushButton = new QPushButton(frame);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(220, 100, 171, 71));
        QFont font;
        font.setPointSize(12);
        font.setBold(true);
        pushButton->setFont(font);
        pushButton->setStyleSheet(QString::fromUtf8("border-image: url(:/Sym/Sym/a.png);\n"
"background-color: rgb(85, 85, 0);"));
        err1PushButton = new QPushButton(frame);
        err1PushButton->setObjectName(QString::fromUtf8("err1PushButton"));
        err1PushButton->setGeometry(QRect(90, 300, 111, 41));
        err1PushButton->setStyleSheet(QString::fromUtf8("border-image: url(:/Sym/Sym/a.png);\n"
"background-color: rgb(85, 85, 0);"));
        err2PushButton = new QPushButton(frame);
        err2PushButton->setObjectName(QString::fromUtf8("err2PushButton"));
        err2PushButton->setGeometry(QRect(90, 350, 111, 41));
        err2PushButton->setStyleSheet(QString::fromUtf8("border-image: url(:/Sym/Sym/a.png);\n"
"background-color: rgb(85, 85, 0);"));
        err3PushButton = new QPushButton(frame);
        err3PushButton->setObjectName(QString::fromUtf8("err3PushButton"));
        err3PushButton->setGeometry(QRect(90, 400, 111, 41));
        err3PushButton->setStyleSheet(QString::fromUtf8("border-image: url(:/Sym/Sym/a.png);\n"
"background-color: rgb(85, 85, 0);"));
        openErrWindowPushButton = new QPushButton(frame);
        openErrWindowPushButton->setObjectName(QString::fromUtf8("openErrWindowPushButton"));
        openErrWindowPushButton->setGeometry(QRect(90, 190, 111, 101));
        openErrWindowPushButton->setStyleSheet(QString::fromUtf8("border-image: url(:/Errors/Errors/errorPix.png);\n"
"background-color: rgb(85, 85, 0,0);"));
        exitPushButton = new QPushButton(frame);
        exitPushButton->setObjectName(QString::fromUtf8("exitPushButton"));
        exitPushButton->setGeometry(QRect(220, 500, 161, 71));
        exitPushButton->setFont(font);
        exitPushButton->setStyleSheet(QString::fromUtf8("border-image: url(:/Sym/Sym/a.png);\n"
"background-color: rgb(85, 85, 0);"));

        retranslateUi(startupWindow);

        QMetaObject::connectSlotsByName(startupWindow);
    } // setupUi

    void retranslateUi(QWidget *startupWindow)
    {
        startupWindow->setWindowTitle(QCoreApplication::translate("startupWindow", "startupWindow", nullptr));
        label->setText(QCoreApplication::translate("startupWindow", "TEST WINDOW", nullptr));
        pushButton->setText(QCoreApplication::translate("startupWindow", "Change Window", nullptr));
        err1PushButton->setText(QCoreApplication::translate("startupWindow", "ERR1", nullptr));
        err2PushButton->setText(QCoreApplication::translate("startupWindow", "ERR2", nullptr));
        err3PushButton->setText(QCoreApplication::translate("startupWindow", "ERR3", nullptr));
        openErrWindowPushButton->setText(QString());
        exitPushButton->setText(QCoreApplication::translate("startupWindow", "EXIT ", nullptr));
    } // retranslateUi

};

namespace Ui {
    class startupWindow: public Ui_startupWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STARTUPWINDOW_H
