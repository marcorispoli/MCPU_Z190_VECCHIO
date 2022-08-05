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
    if(!COMMUNICATION->isSHConnected()){
        if(internalState !=SMS_SMART_HUB_CONNECTION ){
            internalState = SMS_SMART_HUB_CONNECTION;
            subStatus = 0;
        }

    } else if(!COMMUNICATION->isGenConnected()){
        if(internalState !=SMS_WAIT_GENERATOR_CONNECTION ){
            internalState = SMS_WAIT_GENERATOR_CONNECTION;
            subStatus = 0;
        }

    }
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

    case SMS_WAIT_GENERATOR_CONNECTION: handle_GENERATOR_CONNECTION(); break;
    case SMS_MESSAGE_NOTIFY: handle_SMS_MESSAGE_NOTIFY(); break;
    case SMS_IDLE: handle_SMS_IDLE(); break;

    }

}

/*


        switch()
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
  */
void statusManager::handle_GENERATOR_CONNECTION(void){
    switch(subStatus){
     case 0:
        WINDOWS->setStatus("GENERATOR CONNECTING ..");
        subStatus++;
        QTimer::singleShot(0, this, SLOT(handleCurrentStatus()));
        break;
    case 1:
        COMMUNICATION->setProtocolVersion6();subStatus++;
        QTimer::singleShot(100, this, SLOT(handleCurrentStatus()));
        break;
     case 2:
        if(!COMMUNICATION->isProtocolV6()){
            subStatus--;
            QTimer::singleShot(0, this, SLOT(handleCurrentStatus()));
            break;
        }
        COMMUNICATION->getGeneratorStatusV6();subStatus++;
        QTimer::singleShot(100, this, SLOT(handleCurrentStatus()));
        break;

        break;
     case 3:
        if(!COMMUNICATION->isGenConnected()){
            subStatus--;
            QTimer::singleShot(0, this, SLOT(handleCurrentStatus()));
            break;
        }

        COMMUNICATION->setupProcedureV6(R2CP::ProcId_Standard_Mammography_2D);subStatus++;
        QTimer::singleShot(100, this, SLOT(handleCurrentStatus()));
        break;

    case 4:
       if(!COMMUNICATION->isProcedureInitialized(R2CP::ProcId_Standard_Mammography_2D)){
           subStatus--;
           QTimer::singleShot(0, this, SLOT(handleCurrentStatus()));
           break;
       }

       COMMUNICATION->setupProcedureV6(R2CP::ProcId_Aec_Mammography_2D);subStatus++;
       QTimer::singleShot(100, this, SLOT(handleCurrentStatus()));
       break;

    case 5:
       if(!COMMUNICATION->isProcedureInitialized(R2CP::ProcId_Aec_Mammography_2D)){
           subStatus--;
           QTimer::singleShot(0, this, SLOT(handleCurrentStatus()));
           break;
       }

       COMMUNICATION->setupProcedureV6(R2CP::ProcId_Standard_Mammography_3D);subStatus++;
       QTimer::singleShot(100, this, SLOT(handleCurrentStatus()));
       break;

    case 6:
       if(!COMMUNICATION->isProcedureInitialized(R2CP::ProcId_Standard_Mammography_3D)){
           subStatus--;
           QTimer::singleShot(0, this, SLOT(handleCurrentStatus()));
           break;
       }

       COMMUNICATION->setupProcedureV6(R2CP::ProcId_Aec_Mammography_3D);subStatus++;
       QTimer::singleShot(100, this, SLOT(handleCurrentStatus()));
       break;

    case 7:
       if(!COMMUNICATION->isProcedureInitialized(R2CP::ProcId_Aec_Mammography_3D)){
           subStatus--;
           QTimer::singleShot(0, this, SLOT(handleCurrentStatus()));
           break;
       }


       COMMUNICATION->setDbInitialize(R2CP::DB_AecSmall);subStatus++;
       QTimer::singleShot(100, this, SLOT(handleCurrentStatus()));
       break;

    case 8:
       if(!COMMUNICATION->isDbInitialized(R2CP::DB_AecSmall)){
           subStatus--;
           QTimer::singleShot(0, this, SLOT(handleCurrentStatus()));
           break;
       }


       COMMUNICATION->setDbInitialize(R2CP::DB_PulseSmall);subStatus++;
       QTimer::singleShot(100, this, SLOT(handleCurrentStatus()));
       break;

    case 9:
       if(!COMMUNICATION->isDbInitialized(R2CP::DB_PulseSmall)){
           subStatus--;
           QTimer::singleShot(0, this, SLOT(handleCurrentStatus()));
           break;
       }


       COMMUNICATION->setDbInitialize(R2CP::DB_AecLarge);subStatus++;
       QTimer::singleShot(100, this, SLOT(handleCurrentStatus()));
       break;

    case 10:
       if(!COMMUNICATION->isDbInitialized(R2CP::DB_AecLarge)){
           subStatus--;
           QTimer::singleShot(0, this, SLOT(handleCurrentStatus()));
           break;
       }


       COMMUNICATION->setDbInitialize(R2CP::DB_PulseLarge);subStatus++;
       QTimer::singleShot(100, this, SLOT(handleCurrentStatus()));
       break;

    case 11:
       if(!COMMUNICATION->isDbInitialized(R2CP::DB_PulseLarge)){
           subStatus--;
           QTimer::singleShot(0, this, SLOT(handleCurrentStatus()));
           break;
       }


       COMMUNICATION->setDbInitialize(R2CP::DB_AecTomo);subStatus++;
       QTimer::singleShot(100, this, SLOT(handleCurrentStatus()));
       break;

    case 12:
       if(!COMMUNICATION->isDbInitialized(R2CP::DB_AecTomo)){
           subStatus--;
           QTimer::singleShot(0, this, SLOT(handleCurrentStatus()));
           break;
       }


       COMMUNICATION->setDbInitialize(R2CP::DB_PulseTomo);subStatus++;
       QTimer::singleShot(100, this, SLOT(handleCurrentStatus()));
       break;


    case 13:
       if(!COMMUNICATION->isDbInitialized(R2CP::DB_PulseTomo)){
           subStatus--;
           QTimer::singleShot(0, this, SLOT(handleCurrentStatus()));
           break;
       }

        internalState = SMS_IDLE;
        subStatus = 0;
        QTimer::singleShot(10, this, SLOT(handleCurrentStatus()));
        return;
     default:
            subStatus = 0;
            QTimer::singleShot(0, this, SLOT(handleCurrentStatus()));
        break;
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
        COMMUNICATION->getGeneratorStatusV6();subStatus++;
        QTimer::singleShot(100, this, SLOT(handleCurrentStatus()));
        break;
     case 4:
        if(R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.SystemMessage.Fields.Active == R2CP::Stat_SystemMessageActive_NotActive){
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

    if(R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.SystemMessage.Fields.Active == R2CP::Stat_SystemMessageActive_Active){
        internalState = SMS_MESSAGE_NOTIFY;
        QTimer::singleShot(10, this, SLOT(handleCurrentStatus()));
        subStatus = 0;
        return;
    }

    WINDOWS->onRecetionGenStatusSlot();

    switch(subStatus){
    case 0:WINDOWS->setStatus("STANDBY STATUS");    break;
    case 10:
        COMMUNICATION->getGeneratorStatusV6();
        subStatus = 0;
        QTimer::singleShot(100, this, SLOT(handleCurrentStatus()));
        return;
    }

    subStatus++;
    QTimer::singleShot(100, this, SLOT(handleCurrentStatus()));

}
