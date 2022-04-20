#include "startupWindow.h"
#include "ui_startupWindow.h"
#include "errorsWindow.h"
#include "commandprotocol.h"
#include "systemconfig.h"

extern startupWindow* w1;
extern startupWindow* w2;

startupWindow::startupWindow(int W, int H, QWidget *parent)
    : windowsClass(W,H,parent), ui(new Ui::startupWindow)
{
    ui->setupUi(this);


    timeEv = 0;
    // Set the View to handle the rotation


}

startupWindow::~startupWindow()
{
    delete ui;
}

void startupWindow::initWindow(void)
{
    if(this == w1)  ui->label->setText(QApplication::translate("STARTUP-WINDOW","HELLO WORLD") + QString(" FINESTRA 1"));
    else ui->label->setText(QApplication::translate("STARTUP-WINDOW","HELLO WORLD") + QString(" FINESTRA 2"));
    connect(ui->pushButton,SIGNAL(released()),this,SLOT(onPushButton()),Qt::UniqueConnection);
    connect(ui->err1PushButton,SIGNAL(released()),this,SLOT(onErr1PushButton()),Qt::UniqueConnection);
    connect(ui->err2PushButton,SIGNAL(released()),this,SLOT(onErr2PushButton()),Qt::UniqueConnection);
    connect(ui->err3PushButton,SIGNAL(released()),this,SLOT(onErr3PushButton()),Qt::UniqueConnection);
    connect(ui->openErrWindowPushButton,SIGNAL(released()),this,SLOT(onOpenErrWindowPushButton()),Qt::UniqueConnection);
    connect(ui->exitPushButton,SIGNAL(released()),this,SLOT(onExitPushButton()),Qt::QueuedConnection);
    errorsWindow::enableErrorWindow(true);



    angolo = 0;
    setAbsoluteRotation(angolo);
    timeEv = startTimer(1000);
}

void startupWindow::exitWindow(void)
{
    disconnect(ui->pushButton);
    disconnect(ui->err1PushButton);
    disconnect(ui->err2PushButton);
    disconnect(ui->err3PushButton);
    disconnect(ui->openErrWindowPushButton);
    disconnect(ui->exitPushButton);

    killTimer(timeEv);
    timeEv=0;
    errorsWindow::enableErrorWindow(false);
}

void startupWindow::onPushButton(void){
    if(w1->isOpen())  w2->openWindow();
    else w1->openWindow();
}
void startupWindow::onErr1PushButton(void){

}
void startupWindow::onErr2PushButton(void){

}
void startupWindow::onErr3PushButton(void){

}
void startupWindow::onOpenErrWindowPushButton(void){
    errorsWindow::openErrorWindow();
}
void startupWindow::onExitPushButton(void){
    QApplication::quit();
}


void startupWindow::timerEvent(QTimerEvent* ev)
{
    if(ev->timerId() == timeEv)
    {
        // angolo -= 10;
        setAbsoluteRotation(angolo);


    }

}


