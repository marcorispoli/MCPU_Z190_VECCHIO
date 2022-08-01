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
    connect(ui->connectionButton, SIGNAL(pressed()), this, SLOT(connectionButtonSlot()), Qt::UniqueConnection);
    connect(ui->genGetStatus, SIGNAL(pressed()), this, SLOT(onGenGetStatusButtonSlot()), Qt::UniqueConnection);

    // Other connections


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

void startupWindow::connectionButtonSlot(void){
    pComm->start();

}
void startupWindow::onGenGetStatusButtonSlot(void){
    R2CP::CaDataDicGen::GetInstance()->Generator_Get_Status();
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



