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


    connect(ui->getProtocolButton, SIGNAL(pressed()), this, SLOT(onGetProtocolButtonSlot()), Qt::UniqueConnection);
    connect(ui->setVersion5Button, SIGNAL(pressed()), this, SLOT(onSetProtocolVersion5ButtonSlot()), Qt::UniqueConnection);
    connect(ui->setVersion6Button, SIGNAL(pressed()), this, SLOT(onSetProtocolVersion6ButtonSlot()), Qt::UniqueConnection);

    connect(ui->selectManualButton, SIGNAL(pressed()), this, SLOT(onSelectManualButtonSlot()), Qt::UniqueConnection);
    connect(ui->selectAecButton, SIGNAL(pressed()), this, SLOT(onSelectAecButtonSlot()), Qt::UniqueConnection);
    connect(ui->selectTomoButton, SIGNAL(pressed()), this, SLOT(onSelectTomoButtonSlot()), Qt::UniqueConnection);
    connect(ui->selectAecTomoButton, SIGNAL(pressed()), this, SLOT(onSelectAecTomoButtonSlot()), Qt::UniqueConnection);



    connect(ui->getSystemMessagesButton, SIGNAL(pressed()), this, SLOT(onGetSystemMessagesButtonSlot()), Qt::UniqueConnection);
    connect(ui->clearSystemMessagesButton, SIGNAL(pressed()), this, SLOT(onClearSystemMessagesButtonSlot()), Qt::UniqueConnection);

    connect(ui->AECButton, SIGNAL(pressed()), this, SLOT(onAECButtonSlot()), Qt::UniqueConnection);


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

void startupWindow::onGetSystemMessagesButtonSlot(void){
   //R2CP::CaDataDicGen::GetInstance()->SystemMessages_Get_AllMessages();
}

void startupWindow::onClearSystemMessagesButtonSlot(void){
    STATUS->clearErrorsRequest();
    //R2CP::CaDataDicGen::GetInstance()->SystemMessages_Clear_AllMessages();
}


void startupWindow::onGetProtocolButtonSlot(void){
    COMMUNICATION->getProtocolVersion();
}

void startupWindow::onSetProtocolVersion5ButtonSlot(void){
    COMMUNICATION->setProtocolVersion5();
}
void startupWindow::onSetProtocolVersion6ButtonSlot(void){
    COMMUNICATION->setProtocolVersion6();
}

void startupWindow::onSelectManualButtonSlot(void){
    STATUS->start2DExposurePulse(1,ui->kVPulse->text().toFloat(), ui->mAsPulse->text().toUInt());
}

void startupWindow::onSelectAecButtonSlot(void){
    STATUS->start2DAecExposure(1,ui->kVPre->text().toFloat(), ui->mAsPre->text().toUInt());
}
void startupWindow::onSelectTomoButtonSlot(void){

}
void startupWindow::onSelectAecTomoButtonSlot(void){

}


void startupWindow::onAECButtonSlot(void){
    qDebug() << "passato";
    STATUS->aecPulsData(1,ui->kVPulse->text().toFloat(), ui->mAsPulse->text().toUInt());
}


void startupWindow::onRecetionGenStatusSlot(void){

   QString stringa = "STATUS: ";
   switch(R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.GeneratorStatus){
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

    stringa = "PROC-ID: " + QString("%1").arg(R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.ProcedureId);
    ui->statusProcId->setText(stringa);


    stringa = "EXP-TYPE: ";
    switch(R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.ExposureType){
         case R2CP::tGenStatus_ExposureType_None: stringa += "NONE";break;
         case R2CP::tGenStatus_ExposureType_SingleShot: stringa += "SINGLE SHOT";break;
         case R2CP::tGenStatus_ExposureType_SerialRad: stringa += "SERIAL RAD";break;
         case R2CP::tGenStatus_ExposureType_ContFl: stringa += "CONT FL";break;
         case R2CP::tGenStatus_ExposureType_ContFl_HLC: stringa += "CONT FL + HLC";break;
         case R2CP::tGenStatus_ExposureType_PulsedFl: stringa += "PULSED FL";break;
         case R2CP::tGenStatus_ExposureType_PulsedFl_HLC: stringa += "PULSED FL+HLC";break;
    }
    ui->statusProcStat->setText(stringa);

    stringa = "SYSMSG: ";
    switch(R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.SystemMessage.Fields.Active){
         case R2CP::Stat_SystemMessageActive_NotActive: stringa += "NOT ACTIVE";break;
         case R2CP::Stat_SystemMessageActive_Active: stringa += "ACTIVE";break;
    }
    ui->statusSysMsg->setText(stringa);

    stringa = "INHIBIT: ";
    switch(R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.SystemMessage.Fields.Inhibit){
         case R2CP::Stat_SystemMessageInhibit_NotActive: stringa += "NOT ACTIVE";break;
         case R2CP::Stat_SystemMessageInhibit_Active: stringa += "ACTIVE";break;
    }
    ui->statusSysMsgInhibit->setText(stringa);

}

void startupWindow::updateSystemMessages(QString stringa){
    ui->messageList->clear();
    ui->messageList->setPlainText(stringa);

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

