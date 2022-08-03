#include "application.h"
#include <QTimer>

statusManager::statusManager( QObject *parent)
{

    WINDOWS = new startupWindow();
    COMMUNICATION = new Communication();
    COMMUNICATION->start();

    //connect(pComm, SIGNAL(generatorStatusSgn()), window, SLOT(onRecetionGenStatusSlot()), Qt::QueuedConnection);
    //connect(pComm, SIGNAL(systemMessageEventSgn()), window, SLOT(onSystemMessageEventSlot()), Qt::QueuedConnection);
    window->show();

    internalState = SMS_SMART_HUB_CONNECTION;
    subStatus = 0;
    QTimer::singleShot(100, this, SLOT(handleCurrentStatus()));
}



void statusManager::handleCurrentStatus(void){
    // General cases
    if(!COMMUNICATION->isSHConnected()) internalState = SMS_SMART_HUB_CONNECTION;
    else if(!COMMUNICATION->isGenConnected()) internalState = SMS_WAIT_GENERATOR_CONNECTION;
    //else if(R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatus.SystemMessage.Fields.Active == R2CP::Stat_SystemMessageActive_Active) internalState = SMS_MESSAGE_NOTIFY;

    switch(internalState){

    case SMS_SMART_HUB_CONNECTION:
        WINDOWS->setStatus("SH CONNECTING ..");

        if(COMMUNICATION->isSHConnected()){
            internalState = SMS_WAIT_GENERATOR_CONNECTION;
            subStatus = 0;
            QTimer::singleShot(100, this, SLOT(handleCurrentStatus()));
            return;
        }else{
            COMMUNICATION->setSHConnection();
            QTimer::singleShot(100, this, SLOT(handleCurrentStatus()));
            return;
        }
        break;

    case SMS_WAIT_GENERATOR_CONNECTION:
        WINDOWS->setStatus("GENERATOR CONNECTING ..");

        if(COMMUNICATION->isGenConnected()){
            COMMUNICATION->getGeneratorStatus();
            internalState = SMS_IDLE;
            subStatus = 0;
            QTimer::singleShot(10, this, SLOT(handleCurrentStatus()));
            return;
        }else{
            COMMUNICATION->getGeneratorStatus();
            QTimer::singleShot(100, this, SLOT(handleCurrentStatus()));
            return;
        }
        break;

    case SMS_MESSAGE_NOTIFY: handle_SMS_MESSAGE_NOTIFY(); break;
    case SMS_IDLE: handle_SMS_IDLE(); break;

    }

}

void statusManager::handle_SMS_MESSAGE_NOTIFY(void){
    switch(subStatus){
     case 0:
        WINDOWS->setStatus("SYSTEM MESSAGES PRESENT!");
        subStatus++;
        QTimer::singleShot(0, this, SLOT(handleCurrentStatus()));
        break;
    case 1:
        COMMUNICATION->getAllSystemMessages();subStatus++;
        QTimer::singleShot(500, this, SLOT(handleCurrentStatus()));
        break;
     case 2:
        COMMUNICATION->clearSystemMessages();subStatus++;
        QTimer::singleShot(500, this, SLOT(handleCurrentStatus()));
        break;
     case 3:
        COMMUNICATION->getGeneratorStatus();subStatus++;
        QTimer::singleShot(100, this, SLOT(handleCurrentStatus()));
        break;
     case 4:
        if(R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatus.SystemMessage.Fields.Active == R2CP::Stat_SystemMessageActive_NotActive){
            internalState = SMS_IDLE;
            subStatus = 0;
        }else{
            subStatus = 0;
        }
        // Persistent errors
        WINDOWS->updateSystemMessages();

        QTimer::singleShot(100, this, SLOT(handleCurrentStatus()));
        break;
     default:
            subStatus = 0;
            QTimer::singleShot(0, this, SLOT(handleCurrentStatus()));
        break;
    }

}

void statusManager::handle_SMS_IDLE(void){

    if(R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatus.SystemMessage.Fields.Active == R2CP::Stat_SystemMessageActive_Active){
        internalState = SMS_MESSAGE_NOTIFY;
        QTimer::singleShot(10, this, SLOT(handleCurrentStatus()));
        subStatus = 0;
        return;
    }

    WINDOWS->onRecetionGenStatusSlot();

    switch(subStatus){
    case 0:WINDOWS->setStatus("STANDBY STATUS");    break;
    case 10:
        COMMUNICATION->getGeneratorStatus();
        subStatus = 0;
        QTimer::singleShot(100, this, SLOT(handleCurrentStatus()));
        return;
    }

    subStatus++;
    QTimer::singleShot(100, this, SLOT(handleCurrentStatus()));

}
