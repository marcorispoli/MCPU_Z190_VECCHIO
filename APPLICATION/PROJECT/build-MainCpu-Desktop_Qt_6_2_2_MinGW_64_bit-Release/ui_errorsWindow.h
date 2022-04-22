/********************************************************************************
** Form generated from reading UI file 'errorsWindow.ui'
**
** Created by: Qt User Interface Compiler version 6.2.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ERRORSWINDOW_H
#define UI_ERRORSWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_errorsWindow
{
public:
    QFrame *workingFrame;
    QLabel *errorString;
    QLabel *titleString;
    QPlainTextEdit *descriptionString;
    QPushButton *scrollPushButton;
    QPushButton *exitPushButton;
    QFrame *pixFrame;

    void setupUi(QWidget *errorsWindow)
    {
        if (errorsWindow->objectName().isEmpty())
            errorsWindow->setObjectName(QString::fromUtf8("errorsWindow"));
        errorsWindow->resize(600, 600);
        workingFrame = new QFrame(errorsWindow);
        workingFrame->setObjectName(QString::fromUtf8("workingFrame"));
        workingFrame->setGeometry(QRect(0, 0, 600, 600));
        workingFrame->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 0);\n"
"background-color: rgb(85, 0, 0);"));
        workingFrame->setFrameShape(QFrame::StyledPanel);
        workingFrame->setFrameShadow(QFrame::Raised);
        errorString = new QLabel(workingFrame);
        errorString->setObjectName(QString::fromUtf8("errorString"));
        errorString->setGeometry(QRect(100, 70, 141, 41));
        QFont font;
        font.setPointSize(16);
        font.setBold(true);
        errorString->setFont(font);
        errorString->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        errorString->setAlignment(Qt::AlignCenter);
        titleString = new QLabel(workingFrame);
        titleString->setObjectName(QString::fromUtf8("titleString"));
        titleString->setGeometry(QRect(120, 190, 361, 21));
        QFont font1;
        font1.setPointSize(16);
        titleString->setFont(font1);
        titleString->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        titleString->setAlignment(Qt::AlignCenter);
        descriptionString = new QPlainTextEdit(workingFrame);
        descriptionString->setObjectName(QString::fromUtf8("descriptionString"));
        descriptionString->setGeometry(QRect(120, 220, 361, 221));
        descriptionString->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgb(52, 0, 0);"));
        scrollPushButton = new QPushButton(workingFrame);
        scrollPushButton->setObjectName(QString::fromUtf8("scrollPushButton"));
        scrollPushButton->setGeometry(QRect(360, 450, 121, 41));
        scrollPushButton->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgb(52, 0, 0);"));
        exitPushButton = new QPushButton(workingFrame);
        exitPushButton->setObjectName(QString::fromUtf8("exitPushButton"));
        exitPushButton->setGeometry(QRect(240, 550, 121, 41));
        exitPushButton->setStyleSheet(QString::fromUtf8("border-image: url(:/Sym/Sym/a.png);\n"
"background-color: rgb(0, 85, 255);"));
        pixFrame = new QFrame(workingFrame);
        pixFrame->setObjectName(QString::fromUtf8("pixFrame"));
        pixFrame->setGeometry(QRect(360, 50, 120, 91));
        pixFrame->setStyleSheet(QString::fromUtf8("border-image: url(:/errorPixmap/Errors/errorPix.png);"));
        pixFrame->setFrameShape(QFrame::StyledPanel);
        pixFrame->setFrameShadow(QFrame::Raised);

        retranslateUi(errorsWindow);

        QMetaObject::connectSlotsByName(errorsWindow);
    } // setupUi

    void retranslateUi(QWidget *errorsWindow)
    {
        errorsWindow->setWindowTitle(QCoreApplication::translate("errorsWindow", "Form", nullptr));
        errorString->setText(QCoreApplication::translate("errorsWindow", "TextLabel", nullptr));
        titleString->setText(QCoreApplication::translate("errorsWindow", "TextLabel", nullptr));
        scrollPushButton->setText(QCoreApplication::translate("errorsWindow", "SCROLL ERRORS", nullptr));
        exitPushButton->setText(QCoreApplication::translate("errorsWindow", "EXIT", nullptr));
    } // retranslateUi

};

namespace Ui {
    class errorsWindow: public Ui_errorsWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ERRORSWINDOW_H
