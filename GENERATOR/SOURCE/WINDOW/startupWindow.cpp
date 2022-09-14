#include "application.h"
#include "ui_startupWindow.h"

startupWindow::startupWindow(QWidget *parent)
    : ui(new Ui::startupWindow)
{
    ui->setupUi(this);


    // Set the View to handle the rotation



    connect(ui->logUpdateButtonButton, SIGNAL(pressed()), this, SLOT(onLogUpdateButton()), Qt::UniqueConnection);
    connect(ui->logClearButton, SIGNAL(pressed()), this, SLOT(onLogClearButton()), Qt::UniqueConnection);
    connect(ui->logEnableCheck, SIGNAL(stateChanged(int)), this, SLOT(on_logEnableCheck_stateChanged(int)));



    pollingTimer  = 0;

}

startupWindow::~startupWindow()
{
    delete ui;
}

void startupWindow::initWindow(void)
{


}

void startupWindow::exitWindow(void)
{

    if(pollingTimer)    killTimer(pollingTimer);

}


void startupWindow::timerEvent(QTimerEvent* ev)
{
    if(ev->timerId() == pollingTimer)
    {
        switch(polling){


        default:
            polling = 0;
        }


        return;
    }


}

void startupWindow::onLogUpdateButton(void){


}
void startupWindow::onLogClearButton(void){
    ui->debugText->appendPlainText("");
    ui->debugText->clear();
}

void startupWindow::onLogRxSlot(QByteArray data){
    QString stringa = "";

    for(int j=0; j< data.size(); j++){
        stringa += QString("%1 ").arg((unsigned char) data[j]);
    }
    ui->debugText->appendPlainText(stringa);
}




void startupWindow::on_logEnableCheck_stateChanged(int arg1)
{
    static bool connected = false;
    if(arg1){
        if(!connected) connect(pComm, SIGNAL(rxDataEventSgn(QByteArray)), this, SLOT(onLogRxSlot(QByteArray)), Qt::QueuedConnection);
        connected = true;
    }else{
        disconnect(pComm, SIGNAL(rxDataEventSgn(QByteArray)), this, SLOT(onLogRxSlot(QByteArray)));
        connected = false;

    }
}
