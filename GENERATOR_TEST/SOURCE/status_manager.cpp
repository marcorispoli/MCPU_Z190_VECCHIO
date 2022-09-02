#include "application.h"
#include <QTimer>

statusManager::statusManager( QObject *parent)
{

    WINDOWS = new startupWindow();
    COMMUNICATION = new Communication();
    COMMUNICATION->start();

    generator_status_changed = false;

    //connect(pComm, SIGNAL(generatorStatusSgn()), window, SLOT(onRecetionGenStatusSlot()), Qt::QueuedConnection);
    //connect(pComm, SIGNAL(systemMessageEventSgn()), window, SLOT(onSystemMessageEventSlot()), Qt::QueuedConnection);
    window->show();
    internalState = SMS_UNDEFINED;
    changeStatus(SMS_SMART_HUB_CONNECTION,0,SMS_SMART_HUB_CONNECTION,0);
}

void statusManager::changeStatus(_statusManagerState new_status, uchar sub, _statusManagerState ret_status, uchar ret_sub){
    returnState = ret_status;
    returnSubStatus = ret_sub;

    if(internalState == new_status) return;
    internalState = new_status;
    requestState = new_status;

    subStatus = sub;
    wait_command_processed = false;
    cp_timeout = 100;
    QTimer::singleShot(0, this, SLOT(handleCurrentStatus()));
}

void statusManager::setErrorCondition(uint8_t subidx, QString errstr, _statusManagerState ret_status, uchar ret_sub){
    error_string = errstr;
    error_index = SMS_ERR_GENERIC;
    error_subindex = subidx;
    changeStatus(SMS_ERROR,0,ret_status,ret_sub);
}

void statusManager::setErrorCondition(uint8_t idx, uint8_t subidx, QString errstr, _statusManagerState ret_status, uchar ret_sub){
    error_string = errstr;
    error_index = idx;
    error_subindex = subidx;
    changeStatus(SMS_ERROR,0,ret_status,ret_sub);
}

void statusManager::handleCommandProcessedState(void){

    if(!COMMUNICATION->isCommandProcessed() ){
        cp_timeout--;

        if(!cp_timeout){
            cp_timeout = 100;
            wait_command_processed = false;
            command_process_state = false;
            command_process_code = 0;
            QTimer::singleShot(0, this, SLOT(handleCurrentStatus()));
            return;

        }

        QTimer::singleShot(10, this, SLOT(handleCurrentStatus()));
        return;
    }

    cp_timeout = 100;
    wait_command_processed = false;

    command_process_code = COMMUNICATION->getCommandProcessed();
    if(command_process_code)  command_process_state = false;
    else command_process_state = true;


    QTimer::singleShot(10, this, SLOT(handleCurrentStatus()));
    return;
}

void statusManager::handleCurrentStatus(void){

    if(wait_command_processed){
        handleCommandProcessedState();
        return;
    }



    if(internalState == SMS_SMART_HUB_CONNECTION){
        handle_SH_CONNECTION();
        return;
    }

    if(internalState == SMS_WAIT_GENERATOR_CONNECTION){
        handle_GENERATOR_CONNECTION();
        return;
    }

    if(!COMMUNICATION->isSHConnected()){
        changeStatus(SMS_SMART_HUB_CONNECTION,0,SMS_SMART_HUB_CONNECTION,0);
        return;
    }

    if(!COMMUNICATION->isGenConnected()){
        changeStatus(SMS_WAIT_GENERATOR_CONNECTION,0,SMS_WAIT_GENERATOR_CONNECTION,0);
        return;
    }

    if(internalState == SMS_ERROR){
        handle_SMS_ERROR();
        return;
    }



    switch(internalState){

    case SMS_MESSAGE_NOTIFY:            handle_SMS_MESSAGE_NOTIFY(); break;
    case SMS_CLEAR_SYS_MESSAGES:        handle_CLEAR_ALL_SMS_MESSAGES(); break;
    case SMS_IDLE:                      handle_SMS_IDLE(); break;
    case SMS_EXECUTING_2D_MANUAL:       handle_2D_MANUAL(); break;
    case SMS_EXECUTING_2D_AEC:          handle_2D_AEC(); break;

    }

}

void statusManager::handle_SH_CONNECTION(void){

    switch(subStatus){
     case 0:
        WINDOWS->setStatus("SH CONNECTING ..");
        qDebug() << "SH CONNECTION STATUS";
        subStatus++;
        break;
     case 1:
        if(COMMUNICATION->isSHConnected()){
            changeStatus(SMS_WAIT_GENERATOR_CONNECTION,0,SMS_WAIT_GENERATOR_CONNECTION,0);
            return;
        }
        subStatus++;
        break;
    case 2:
        COMMUNICATION->setSHConnection();
        subStatus = 1;
        break;
    }

    QTimer::singleShot(100, this, SLOT(handleCurrentStatus()));
}

void statusManager::handle_GENERATOR_CONNECTION(void){
    static bool init = false;

    if(!COMMUNICATION->isGenConnected()){
        if(!init){
            WINDOWS->setStatus("GENERATOR CONNECTING ..");
            qDebug() << "GENERATOR CONNECTION STATUS";
            init = true;

        }
        subStatus = 0;
        QTimer::singleShot(100, this, SLOT(handleCurrentStatus()));
        return;
    }

    switch(subStatus){
     case 0:
        init = false;
        wait_command_processed = true;
        COMMUNICATION->setProtocolVersion6();subStatus++;
        break;

     case 1:
        if(!command_process_state){
            subStatus = 0;
            QTimer::singleShot(100, this, SLOT(handleCurrentStatus()));
            return;
        }

        if(!COMMUNICATION->isProtocolV6()){
            subStatus = 0;
            QTimer::singleShot(100, this, SLOT(handleCurrentStatus()));
            return;
        }

        subStatus++;
        qDebug() << "Wait Generator Initialization ..";
        break;

    case 2:

        wait_command_processed = true;
        COMMUNICATION->getGeneratorStatusV6();subStatus++;
        break;

    case 3:
        if(!command_process_state){
            subStatus = 2;
            QTimer::singleShot(100, this, SLOT(handleCurrentStatus()));
            return;
        }

        if(R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.GeneratorStatus == R2CP::Stat_Initialization ){
            subStatus = 2;
            QTimer::singleShot(100, this, SLOT(handleCurrentStatus()));
            return;
        }
        qDebug() << "Generator in Standby";

        wait_command_processed = true;
        COMMUNICATION->clearAllProcedures();subStatus++;                
        break;

    case 4:
        if(!command_process_state){
            subStatus = 0;
            QTimer::singleShot(100, this, SLOT(handleCurrentStatus()));
            return;
        }


        wait_command_processed = true;
        COMMUNICATION->setupProcedureV6(R2CP::ProcId_Standard_Mammography_2D);subStatus++;
        break;

    case 5:
        if(!command_process_state){            
            subStatus = 0;
            QTimer::singleShot(100, this, SLOT(handleCurrentStatus()));
            return;
        }


       wait_command_processed = true;
       COMMUNICATION->setupProcedureV6(R2CP::ProcId_Aec_Mammography_2D);subStatus++;
       break;

    case 6:
        if(!command_process_state){
            subStatus = 0;
            QTimer::singleShot(100, this, SLOT(handleCurrentStatus()));
            return;
        }

       wait_command_processed = true;
       COMMUNICATION->setupProcedureV6(R2CP::ProcId_Standard_Mammography_3D);subStatus++;
       break;

    case 7:
        if(!command_process_state){
            subStatus = 0;
            QTimer::singleShot(100, this, SLOT(handleCurrentStatus()));
            return;
        }

       wait_command_processed = true;
       COMMUNICATION->setupProcedureV6(R2CP::ProcId_Aec_Mammography_3D);subStatus++;       
       break;

    case 8:
        if(!command_process_state){
            subStatus = 0;
            QTimer::singleShot(100, this, SLOT(handleCurrentStatus()));
            return;
        }

        wait_command_processed = true;
        COMMUNICATION->set2DDataBank(R2CP::DB_Pre,1,20,10,10,100); subStatus++;
        break;

    case 9:
        if(!command_process_state){
            subStatus = 0;
            QTimer::singleShot(100, this, SLOT(handleCurrentStatus()));
            return;
        }

        wait_command_processed = true;
        COMMUNICATION->set2DDataBank(R2CP::DB_Pulse,1,20,10,10,100); subStatus++;
        break;

    case 10:
        if(!command_process_state){
            subStatus = 0;
            QTimer::singleShot(100, this, SLOT(handleCurrentStatus()));
            return;
        }

        wait_command_processed = true;
        COMMUNICATION->assignDbToProc(R2CP::DB_Pulse,R2CP::ProcId_Standard_Mammography_2D,1);subStatus++;
        break;

    case 11:
        if(!command_process_state){
            subStatus = 0;
            QTimer::singleShot(100, this, SLOT(handleCurrentStatus()));
            return;
        }

        wait_command_processed = true;
        COMMUNICATION->assignDbToProc(R2CP::DB_Pre,R2CP::ProcId_Aec_Mammography_2D,1);subStatus++;
        break;

    case 12:
        if(!command_process_state){
            subStatus = 0;
            QTimer::singleShot(100, this, SLOT(handleCurrentStatus()));
            return;
        }

        wait_command_processed = true;
        COMMUNICATION->assignDbToProc(R2CP::DB_Pulse,R2CP::ProcId_Aec_Mammography_2D,2);subStatus++;
        break;

    case 13:
        if(!command_process_state){
            subStatus = 0;
            QTimer::singleShot(100, this, SLOT(handleCurrentStatus()));
            return;
        }

        subStatus++;
        break;

    case 14: // Clear all the current System messages
        changeStatus(SMS_CLEAR_SYS_MESSAGES, 0, internalState, subStatus+1);
        return;

    default:
        changeStatus(SMS_IDLE,0,SMS_IDLE,0);
        return;
    }

    QTimer::singleShot(10, this, SLOT(handleCurrentStatus()));

}


void statusManager::handle_SMS_MESSAGE_NOTIFY(void){
    /*
    switch(subStatus){
     case 0:
        WINDOWS->setStatus("SYSTEM MESSAGES PRESENT!");
        qDebug() << "SYSTEM MESSAGES STATUS";
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
            internalState = returnState;
            subStatus = returnSubStatus;
        }else{
            subStatus = 1;
        }

        // Persistent errors
        WINDOWS->updateSystemMessages();

        QTimer::singleShot(100, this, SLOT(handleCurrentStatus()));
        break;
     default:
            subStatus = 0;
            QTimer::singleShot(0, this, SLOT(handleCurrentStatus()));
        break;
    }*/

}



void statusManager::handle_CLEAR_ALL_SMS_MESSAGES(void){
    static QList<uint> id;

    switch(subStatus){
     case 0:
        WINDOWS->setStatus("CLEAR SYSTEM MESSAGES!");
        qDebug() << "CLEAR SYSTEM MESSAGES";
        subStatus++;
        id.clear();
        break;

    case 1:
        wait_command_processed = true;
        COMMUNICATION->getAllSystemMessages();subStatus++;
        break;

    case 2:
        if(!command_process_state){
            subStatus = 5;
            QTimer::singleShot(0, this, SLOT(handleCurrentStatus()));
            return;
        }

        if(R2CP::CaDataDicGen::GetInstance()->systemInterface.messageList.size()==0) {
            subStatus = 5;
            QTimer::singleShot(0, this, SLOT(handleCurrentStatus()));
            return;
        }

        for(int i =0; i < R2CP::CaDataDicGen::GetInstance()->systemInterface.messageList.size(); i++) id.append(R2CP::CaDataDicGen::GetInstance()->systemInterface.messageList[i].Id);
        subStatus++;
        break;

    case 3:

        wait_command_processed = true;
        COMMUNICATION->clearSystemMessage(id[0]);subStatus++;
        break;

    case 4:
        if(!command_process_state){
            subStatus = 5;
            QTimer::singleShot(0, this, SLOT(handleCurrentStatus()));
            return;
        }

        id.remove(0);
        if(id.size()==0) {
            subStatus = 5;
            QTimer::singleShot(0, this, SLOT(handleCurrentStatus()));
            return;
        }

        subStatus = 3;
        break;

    case 5: // Update the status as the final step
        wait_command_processed = true;
        COMMUNICATION->getGeneratorStatusV6();subStatus++;
        break;

    case 6: // Procedure completed
        changeStatus(returnState,returnSubStatus,returnState,returnSubStatus);
        break;
    }

    QTimer::singleShot(10, this, SLOT(handleCurrentStatus()));

}

void statusManager::handle_SMS_IDLE(void){



    WINDOWS->onRecetionGenStatusSlot();

    if(requestState != SMS_IDLE){
        changeStatus(requestState,0,SMS_IDLE,0);
        return;
    }

    switch(subStatus){
    case 0:
        WINDOWS->setStatus("STANDBY STATUS");
        qDebug() << "IDLE STATUS";
        WINDOWS->updateSystemMessages("");
        break;

    case 10:
        COMMUNICATION->getGeneratorStatusV6();
        subStatus = 1;
        QTimer::singleShot(100, this, SLOT(handleCurrentStatus()));
        return;
    }

    subStatus++;
    QTimer::singleShot(100, this, SLOT(handleCurrentStatus()));

}


void statusManager::handle_SMS_ERROR(void){
    static _statusManagerState locRetStatus;
    static uchar locRetSubStatus;
    static QString errorString ;

    switch(subStatus){
      case 0:
            WINDOWS->setStatus("ERROR STATUS");
            qDebug() << "ERROR STATUS";
            locRetStatus = returnState;
            locRetSubStatus = returnSubStatus;

            requestClearError = false;
            if(error_index == SMS_ERR_EXPOSURE){
                errorString = "EXPOSURE ERROR CONTEST:\n";
                switch(error_subindex){
                     case SMS_EXP_ERR_SYSMSG: errorString += "ERROR TYPE: SMS_EXP_ERR_SYSMSG \n"; break;
                     case SMS_EXP_ERR_PULSE_VALIDATION: errorString += "ERROR TYPE: SMS_EXP_ERR_PULSE_VALIDATION \n"; break;
                     case SMS_EXP_ERR_PRE_VALIDATION: errorString += "ERROR TYPE: SMS_EXP_ERR_PRE_VALIDATION \n"; break;
                     case SMS_EXP_ERR_PREPARATION: errorString += "ERROR TYPE: SMS_EXP_ERR_PREPARATION \n"; break;
                     case SMS_EXP_ERR_SEQUENCE: errorString += "ERROR TYPE: SMS_EXP_ERR_SEQUENCE \n"; break;
                }
            }else{
                errorString = "GENERIC ERROR CONTEST:\n";
                switch(error_subindex){
                    case SMS_ERR_GENERATOR_CONNECTION: errorString += "ERROR TYPE: SMS_ERR_GENERATOR_CONNECTION \n"; break;
                }
            }
            errorString += "ERR INFO: " + error_string + "\n";


            if(command_process_code) errorString += "CP: " + COMMUNICATION->getCommandProcessedString();
            subStatus++;
        break;

     case 1: // get all system messages
            wait_command_processed = true;
            COMMUNICATION->getAllSystemMessages();subStatus++;
            break;

     case 2:

            if(R2CP::CaDataDicGen::GetInstance()->systemInterface.messageList.size()){
                errorString += "GENERATOR SYS MESSAGES:\n";
                for(int i =0; i < R2CP::CaDataDicGen::GetInstance()->systemInterface.messageList.size(); i++)
                    errorString += QString("%1\n").arg(R2CP::CaDataDicGen::GetInstance()->systemInterface.messageList[i].Id);
            }

            subStatus++;
            WINDOWS->updateSystemMessages(errorString);
            break;

      case 3:
        if(requestClearError){
            requestClearError = false;
            errorString = "";
            WINDOWS->updateSystemMessages("");
            subStatus++;
        }
        break;

      case 4: // Clear All messages
        errorString = "";
        changeStatus(SMS_CLEAR_SYS_MESSAGES,0,internalState,subStatus+1);
        return;

      case 5: // get all system messages
           wait_command_processed = true;
           COMMUNICATION->getAllSystemMessages();subStatus++;
           break;

       case 6:
           if(R2CP::CaDataDicGen::GetInstance()->systemInterface.messageList.size()){
               errorString += "GENERATOR SYS MESSAGES:\n";
               for(int i =0; i < R2CP::CaDataDicGen::GetInstance()->systemInterface.messageList.size(); i++)
                   errorString += QString("%1\n").arg(R2CP::CaDataDicGen::GetInstance()->systemInterface.messageList[i].Id);

               subStatus = 4;
               WINDOWS->updateSystemMessages(errorString);
               QTimer::singleShot(500, this, SLOT(handleCurrentStatus()));
               return;
           }

           subStatus++;
           WINDOWS->updateSystemMessages(errorString);
           break;

       default:
            changeStatus(locRetStatus,locRetSubStatus,locRetStatus,locRetSubStatus);
            return;
        }

    QTimer::singleShot(10, this, SLOT(handleCurrentStatus()));
}

bool statusManager::validate2DExposurePulse(void){
    mA = 150;
    mS = (pulse_mAs * 1000)/ mA;
    return true;
}
bool statusManager::validate2DExposurePre(void){
    mA = 150;
    mS = (pre_mAs * 1000)/ mA;
    return true;
}

/*

 */
