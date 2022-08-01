#include "startupWindow.h"
#include "communication.h"
#include "ui_startupWindow.h"
#include "QStringConverter"
#include "QDateTime"

extern startupWindow* w1;
extern Communication*  pComm;

startupWindow::startupWindow(QWidget *parent)
    : ui(new Ui::startupWindow)
{
    ui->setupUi(this);


    timeEv = 0;
    // Set the View to handle the rotation


    ui->connectedLabel->hide();
    connect(ui->connectionButton, SIGNAL(pressed()), this, SLOT(onConnectionButton()), Qt::UniqueConnection);
    connect(ui->getStatusButton, SIGNAL(pressed()), this, SLOT(onGetStatusButton()), Qt::UniqueConnection);

    pollingTimer = 0;



}

startupWindow::~startupWindow()
{
    delete ui;
}

void startupWindow::initWindow(void)
{

    timeEv = startTimer(1000);
}

void startupWindow::exitWindow(void)
{

    killTimer(timeEv);
    timeEv=0;
}


void startupWindow::timerEvent(QTimerEvent* ev)
{
    if(ev->timerId() == timeEv)
    {

    }

}

void startupWindow::connectionStat(bool stat){
    if(stat) ui->connectedLabel->show();
    else ui->connectedLabel->hide();
    return;
}

void startupWindow::onConnectionButton(void){
    pComm->start();

}
void startupWindow::onGetStatusButton(void){
    pComm->generatorStatusGet();
}

