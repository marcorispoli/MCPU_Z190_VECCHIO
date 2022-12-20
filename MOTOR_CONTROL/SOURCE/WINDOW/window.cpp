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
    connect(ui->zeroSettingButton, SIGNAL(pressed()), this, SLOT(onZeroSettingButton()), Qt::UniqueConnection);
    connect(ui->positionA, SIGNAL(pressed()), this, SLOT(onPositionA()), Qt::UniqueConnection);
    connect(ui->positionB, SIGNAL(pressed()), this, SLOT(onPositionB()), Qt::UniqueConnection);
    connect(ui->stopCommand, SIGNAL(pressed()), this, SLOT(onStopCommand()), Qt::UniqueConnection);
    connect(ui->immediateCommand, SIGNAL(pressed()), this, SLOT(onImmediateStopCommand()), Qt::UniqueConnection);
    connect(ui->uploadnanoj, SIGNAL(pressed()), this, SLOT(onUploadnanoj()), Qt::UniqueConnection);
    connect(ui->enableConfig, SIGNAL(pressed()), this, SLOT(onEnableConfig()), Qt::UniqueConnection);

    connect(CANCLIENT,SIGNAL(canReady(bool)), this, SLOT(onCanReady(bool)));
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
    //QString stringa = QString("%1> %2").arg(generalDebugIndex++).arg(data);
    QString stringa = QString("%1> %2").arg(((double) clock())/CLOCKS_PER_SEC).arg(data);

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


void debugWindow::onCanReady(bool stat){
    if(stat) qDebug() << "CAN READY TO OPERATE";
    else qDebug() << "CAN NOT READY TO OPERATE";
}

void debugWindow::onZeroSettingButton(void){
  if(!TRX->activateZeroSetting()) qDebug() << "WINDOW: ZERO SETTING NOT ACTIVATED!";

}

void debugWindow::onPositionA(void){


  if(!TRX->activatePositioning(2500, 100, 100, 400,true)) qDebug() << "WINDOW: POSITIONING NOT ACTIVATED!";



}
void debugWindow::onPositionB(void){

  if(!TRX->activatePositioning(-2500, 100, 100, 400,true)) qDebug() << "WINDOW: POSITIONING NOT ACTIVATED!";

}
void debugWindow::onStopCommand(void){
  TRX->quickStop();
}

void debugWindow::onImmediateStopCommand(void){
  TRX->immediateStop();
}

void debugWindow::onUploadnanoj(void){
  TRX->uploadNanojProgram();
}


void debugWindow::onEnableConfig(void){
  //TRX->enableConfiguration();

    if(INTERFACE) INTERFACE->configReceived();
}

