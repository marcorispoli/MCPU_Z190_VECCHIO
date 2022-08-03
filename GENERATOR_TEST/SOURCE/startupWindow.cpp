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


    connect(ui->genGetStatus, SIGNAL(pressed()), this, SLOT(onGenGetStatusButtonSlot()), Qt::UniqueConnection);
    connect(ui->getSystemMessagesButton, SIGNAL(pressed()), this, SLOT(onGetSystemMessagesButtonSlot()), Qt::UniqueConnection);
    connect(ui->clearSystemMessagesButton, SIGNAL(pressed()), this, SLOT(onClearSystemMessagesButtonSlot()), Qt::UniqueConnection);



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

void  startupWindow::setStatus(QString stringa){
    ui->statusLabel->setText(stringa);
}


void startupWindow::connectionButtonSlot(void){
    pComm->start();

}
void startupWindow::onGenGetStatusButtonSlot(void){
    R2CP::CaDataDicGen::GetInstance()->Generator_Get_Status();
}

void startupWindow::onGetSystemMessagesButtonSlot(void){
    R2CP::CaDataDicGen::GetInstance()->SystemMessages_Get_AllMessages();
}

void startupWindow::onClearSystemMessagesButtonSlot(void){
    R2CP::CaDataDicGen::GetInstance()->SystemMessages_Clear_AllMessages();
}



void startupWindow::onRecetionGenStatusSlot(void){

   QString stringa = "STATUS: ";
   switch(R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatus.GeneratorStatus){
        case R2CP::Stat_Initialization: stringa += "INIT";break;
        case R2CP::Stat_Standby: stringa += "STANDBY";break;
        case R2CP::Stat_Preparation: stringa += "PREPARATION";break;
        case R2CP::Stat_Ready: stringa += "READY";break;
        case R2CP::Stat_ExpReq: stringa += "EXPOSURE-REQ";break;
        case R2CP::Stat_ExpInProgress: stringa += "EXPOSURE-PROGRESS";break;
        case R2CP::Stat_WaitFootRelease: stringa += "WAIT RELEASE";break;
        case R2CP::Stat_GoigToShutdown: stringa += "SHUTDOWN";break;
        case R2CP::Stat_Error: stringa += "ERROR";break;
        case R2CP::Stat_Service: stringa += "SERVICE";break;
   }

    ui->statusStat->setText(stringa);

    stringa = "PROC-ID: " + QString("%1").arg(R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatus.ProcedureId);
    ui->statusProcId->setText(stringa);


    stringa = "PROC-STAT: ";
    switch(R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatus.ProcedureStatus){
         case R2CP::Stat_ProcedureStat_NotActive: stringa += "NOT ACTIVE";break;
         case R2CP::Stat_ProcedureStat_Active: stringa += "ACTIVE";break;
         case R2CP::Stat_ProcedureStat_Paused: stringa += "PAUSED";break;
         case R2CP::Stat_ProcedureStat_Finished: stringa += "FINISHED";break;
    }
    ui->statusProcStat->setText(stringa);

    stringa = "SYSMSG: ";
    switch(R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatus.SystemMessage.Fields.Active){
         case R2CP::Stat_SystemMessageActive_NotActive: stringa += "NOT ACTIVE";break;
         case R2CP::Stat_SystemMessageActive_Active: stringa += "ACTIVE";break;
    }
    ui->statusSysMsg->setText(stringa);

    stringa = "INHIBIT: ";
    switch(R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatus.SystemMessage.Fields.Inhibit){
         case R2CP::Stat_SystemMessageInhibit_NotActive: stringa += "NOT ACTIVE";break;
         case R2CP::Stat_SystemMessageInhibit_Active: stringa += "ACTIVE";break;
    }
    ui->statusSysMsgInhibit->setText(stringa);

}

void startupWindow::updateSystemMessages(void){
    ui->messageList->clear();
    QString stringa = "";
    for (int i=0; i< R2CP::CaDataDicGen::GetInstance()->systemInterface.messageList.size(); i++){
        stringa += QString("%1 \n\r").arg(R2CP::CaDataDicGen::GetInstance()->systemInterface.messageList[i].Id);
    }
    ui->messageList->setPlainText(stringa);

}



void startupWindow::on_logEnableCheck_stateChanged(int arg1)
{
    if(arg1){
        connect(pComm, SIGNAL(rxDataEventSgn(QByteArray)), this, SLOT(onLogRxSlot(QByteArray)), Qt::QueuedConnection);
    }else{
        disconnect(pComm, SIGNAL(rxDataEventSgn(QByteArray)), this, SLOT(onLogRxSlot(QByteArray)));
    }
}

