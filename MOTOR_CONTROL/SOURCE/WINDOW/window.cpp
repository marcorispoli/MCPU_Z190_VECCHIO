#include "application.h"
#include "ui_window.h"



debugWindow::debugWindow(QWidget *parent)
    : ui(new Ui::debugWindow)
{
    ui->setupUi(this);
    generalDebugIndex = 0;

    // Set the View to handle the rotation


    connect(ui->logClearButton, SIGNAL(pressed()), this, SLOT(onLogClearButton()), Qt::UniqueConnection);
    connect(ui->debugClearButton, SIGNAL(pressed()), this, SLOT(onDebugClearButton()), Qt::UniqueConnection);
    connect(ui->logEnableCheck, SIGNAL(stateChanged(int)), this, SLOT(on_logEnableCheck_stateChanged(int)));



    pollingTimer  = startTimer(500);

}

debugWindow::~debugWindow()
{
    delete ui;
}

void debugWindow::initWindow(void)
{


}

void debugWindow::exitWindow(void)
{

    if(pollingTimer)    killTimer(pollingTimer);

}


void debugWindow::timerEvent(QTimerEvent* ev)
{
    if(ev->timerId() == pollingTimer)
    {


        return;
    }


}


void debugWindow::onLogClearButton(void){
    ui->r2cpText->appendPlainText("");
    ui->r2cpText->clear();
}
void debugWindow::onDebugClearButton(void){
    ui->debugText->appendPlainText("");
    ui->debugText->clear();
}



void debugWindow::onDebug(QByteArray data){
    QString stringa = QString("%1> %2").arg(generalDebugIndex++).arg(data);
    if(ui->debugEnable->isChecked())   ui->debugText->appendPlainText(stringa);
}


void debugWindow::on_logEnableCheck_stateChanged(int arg1)
{
    /*
    static bool connected = false;
    if(arg1){
        if(!connected) connect(COMMUNICATION, SIGNAL(rxDataEventSgn(QByteArray)), this, SLOT(onLogRxSlot(QByteArray)), Qt::QueuedConnection);
        connected = true;
    }else{
        disconnect(COMMUNICATION, SIGNAL(rxDataEventSgn(QByteArray)), this, SLOT(onLogRxSlot(QByteArray)));
        connected = false;

    }
    */
}



