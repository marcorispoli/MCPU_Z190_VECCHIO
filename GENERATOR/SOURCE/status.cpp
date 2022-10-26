#include "application.h"


statusManager::statusManager( QObject *parent)
{

    COMMUNICATION = new Communication();
    COMMUNICATION->start();

    generator_status_changed = false;
    abortRxRequest = false;
    interfaceStatus = Interface::_STATUS_NOT_READY;

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
    command_process_state = true;
    cp_timeout = 100;
    QTimer::singleShot(0, this, SLOT(handleCurrentStatus()));
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
    static uchar interfaceStatus_back = Interface::_STATUS_NOT_READY;

    // When a command processed return code is requested, the state machine is suspendend
    if(wait_command_processed){
        handleCommandProcessedState();
        return;
    }

    // Update Interface Status to Interface actors
    if(interfaceStatus_back != interfaceStatus){
        interfaceStatus_back = interfaceStatus;
        INTERFACE->EventStatus(0, interfaceStatus);
    }


    if(internalState == SMS_SMART_HUB_CONNECTION){
        interfaceStatus = Interface::_STATUS_NOT_READY;
        handle_SH_CONNECTION();
        return;
    }

    if(internalState == SMS_WAIT_GENERATOR_CONNECTION){
        interfaceStatus = Interface::_STATUS_NOT_READY;
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
        interfaceStatus = Interface::_STATUS_ERROR;
        handle_SMS_ERROR();
        return;
    }



    switch(internalState){

    case SMS_CLEAR_SYS_MESSAGES:        handle_CLEAR_ALL_SMS_MESSAGES(); break;
    case SMS_IDLE:                      handle_SMS_IDLE(); break;
    case SMS_EXECUTING_2D_MANUAL:       handle_2D_MANUAL(); break;
    case SMS_EXECUTING_2D_AEC:          handle_2D_AEC(); break;
    case SMS_SETUP_GENERATOR:           handle_SETUP_GENERATOR();break;
    }

}

void statusManager::handle_SH_CONNECTION(void){

    switch(subStatus){
     case 0:
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

        changeStatus(SMS_SETUP_GENERATOR,0,SMS_SETUP_GENERATOR,0);
        return;
    }

    QTimer::singleShot(10, this, SLOT(handleCurrentStatus()));

}

void statusManager::handle_SETUP_GENERATOR(void){
    static bool error_condition = false;

    // In case of an error condition detected, the processn stands in this status
    if(error_condition){
      interfaceStatus = Interface::_STATUS_ERROR;
      QTimer::singleShot(100, this, SLOT(handleCurrentStatus()));

      return;
    }

    if(!command_process_state){
        INTERFACE->EventSwError(0,Interface::_SW_ERR_GENERATOR_SETUP);
        error_condition = true;
        qDebug() << "Generator Setup Error, subStatus=" << subStatus;
        QTimer::singleShot(100, this, SLOT(handleCurrentStatus()));
        return;
    }

    switch(subStatus){

    case 0:
        qDebug() << "GENERATOR SETUP STATUS";
        interfaceStatus = Interface::_STATUS_NOT_READY;
        error_condition = false;

        // Clear All messages
        changeStatus(SMS_CLEAR_SYS_MESSAGES,0,internalState,subStatus+1);
        return;

    case 1:
        wait_command_processed = true;
        COMMUNICATION->clearAllProcedures();
        subStatus++;
        break;

    case 2:
        wait_command_processed = true;
        COMMUNICATION->setupProcedureV6(R2CP::ProcId_Standard_Test);subStatus++;
        break;

    case 3:
       wait_command_processed = true;
       COMMUNICATION->setupProcedureV6(R2CP::ProcId_Standard_Test_with_grid);subStatus++;
       break;

    case 4:
       wait_command_processed = true;
       COMMUNICATION->setupProcedureV6(R2CP::ProcId_Standard_Mammography_2D);subStatus++;
       break;

    case 5:
       wait_command_processed = true;
       COMMUNICATION->setupProcedureV6(R2CP::ProcId_Aec_Mammography_2D);subStatus++;
       break;

    case 6:

       wait_command_processed = true;
       COMMUNICATION->setupProcedureV6(R2CP::ProcId_Aec_Mammography_2D);subStatus++;
       break;

    case 7:

       wait_command_processed = true;
       COMMUNICATION->setupProcedureV6(R2CP::ProcId_Standard_Mammography_3D);subStatus++;
       break;

    case 8:

       wait_command_processed = true;
       COMMUNICATION->setupProcedureV6(R2CP::ProcId_Aec_Mammography_3D);subStatus++;
       break;

    case 9:
        wait_command_processed = true;
        COMMUNICATION->set2DDataBank(R2CP::DB_Pre,1,20,10); subStatus++;
        break;

    case 10:
        wait_command_processed = true;
        COMMUNICATION->set2DDataBank(R2CP::DB_Pulse,1,20,10); subStatus++;
        break;

    case 11:
        wait_command_processed = true;
        COMMUNICATION->assignDbToProc(R2CP::DB_Pulse,R2CP::ProcId_Standard_Test,1);subStatus++;
        break;

    case 12:
        wait_command_processed = true;
        COMMUNICATION->assignDbToProc(R2CP::DB_Pulse,R2CP::ProcId_Standard_Test_with_grid,1);subStatus++;
        break;
    case 13:
        wait_command_processed = true;
        COMMUNICATION->assignDbToProc(R2CP::DB_Pulse,R2CP::ProcId_Standard_Mammography_2D,1);subStatus++;
        break;

    case 14:
        wait_command_processed = true;
        COMMUNICATION->assignDbToProc(R2CP::DB_Pre,R2CP::ProcId_Aec_Mammography_2D,1);subStatus++;
        break;

    case 15:
        wait_command_processed = true;
        COMMUNICATION->assignDbToProc(R2CP::DB_Pulse,R2CP::ProcId_Aec_Mammography_2D,2);subStatus++;
        break;

    case 16:
        // Set the disable Rx message
        wait_command_processed = true;
        COMMUNICATION->setDisableRxSystemMessage(true);subStatus++;
        break;

    case 17:
        // Clear all the current System messages
        changeStatus(SMS_CLEAR_SYS_MESSAGES, 0, SMS_IDLE, 0);
        return;

    default:
        changeStatus(SMS_IDLE,0,SMS_IDLE,0);
        return;
    }

    QTimer::singleShot(10, this, SLOT(handleCurrentStatus()));

}




void statusManager::handle_CLEAR_ALL_SMS_MESSAGES(void){
    static QList<ulong> id;

    switch(subStatus){
     case 0:
        qDebug() << "CLEAR SYSTEM MESSAGES";
        subStatus++;

        break;

    case 1:
        wait_command_processed = true;
        COMMUNICATION->getAllSystemMessages();subStatus++;
        break;

    case 2:
        if(!command_process_state){
            // If the procedure fails, exit ..
            subStatus = 5;
            qDebug() << "Failed to get the message list";
            QTimer::singleShot(0, this, SLOT(handleCurrentStatus()));
            return;
        }

        if(COMMUNICATION->getErrorList()->size()==0) {
            // No messages are present
            subStatus = 5;
            qDebug() << "No messages are present";
            QTimer::singleShot(0, this, SLOT(handleCurrentStatus()));
            return;
        }

        id.clear();
        for(int i =0; i < COMMUNICATION->getErrorList()->size(); i++) id.append(COMMUNICATION->getErrorList()->at(i));
        subStatus++;
        break;

    case 3:

        wait_command_processed = true;
        COMMUNICATION->clearSystemMessage(id[0]);subStatus++;
        break;

    case 4:
        if(!command_process_state){
            subStatus = 5;
            qDebug() << "Failed to clear a message";
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
        COMMUNICATION->getGeneratorStatusV6();subStatus++;
        break;

    case 6: // Procedure completed
        changeStatus(returnState,returnSubStatus,returnState,returnSubStatus);
        break;
    }

    QTimer::singleShot(10, this, SLOT(handleCurrentStatus()));

}

void statusManager::handle_SMS_IDLE(void){
    QList<ulong> errlst;


    if(requestState != SMS_IDLE){
        changeStatus(requestState,0,SMS_IDLE,0);
        return;
    }

    // Verify the system messages presents
    if(COMMUNICATION->getErrorList()->size()){
        changeStatus(SMS_ERROR,0,SMS_IDLE,0);
        return;
    }


    switch(subStatus){
    case 0:
        qDebug() << "IDLE STATUS";
        abortRxRequest = false;
        INTERFACE->EventSetXrayEna(0,false);
        interfaceStatus = Interface::_STATUS_READY;

        break;

    case 1:
        // Set the disable Rx message
        wait_command_processed = true;
        COMMUNICATION->setDisableRxSystemMessage(true);
        break;

    case 2:
        if(!command_process_state){
            // Repeat the disable Rx message command
            subStatus--;
            QTimer::singleShot(100, this, SLOT(handleCurrentStatus()));
            return;
        }
        break;

    case 3:

        break;

    case 10:
        COMMUNICATION->getGeneratorStatusV6();
        subStatus = 2;
        QTimer::singleShot(100, this, SLOT(handleCurrentStatus()));
        return;
    }

    subStatus++;
    QTimer::singleShot(100, this, SLOT(handleCurrentStatus()));

}


void statusManager::handle_SMS_ERROR(void){

    switch(subStatus){
      case 0:            
            INTERFACE->EventSysMessages(0,COMMUNICATION->getErrorList());
            interfaceStatus = Interface::_STATUS_ERROR;
            subStatus++;
            break;

      case 1:
            // Clear All messages
            changeStatus(SMS_CLEAR_SYS_MESSAGES,0,internalState,subStatus+1);
            return;

      case 2:
           // get all system messages
           wait_command_processed = true;
           COMMUNICATION->getAllSystemMessages();
           subStatus++;
           break;

       case 3:
            if(!command_process_state){
                subStatus--;
                QTimer::singleShot(100, this, SLOT(handleCurrentStatus()));
                return;
            }

           if(COMMUNICATION->getErrorList()->size()){
               // Repeat Until the error disappears
               subStatus = 1;
               QTimer::singleShot(500, this, SLOT(handleCurrentStatus()));
               return;
           }

           subStatus++;
           break;

       default:
            changeStatus(SMS_IDLE,0,SMS_IDLE,0);
            return;
        }

    QTimer::singleShot(10, this, SLOT(handleCurrentStatus()));
}

bool statusManager::validate2DExposurePulse(void){
    qDebug() << "Validated Pulse: kV=" << pulse_kV << ", mAs=" << pulse_mAs << "focus=" << focus;
    return true;
}
bool statusManager::validate2DExposurePre(void){

    qDebug() << "Validated Pre: kV=" << pre_kV << ", mAs=" << pre_mAs << "focus=" << focus;
    return true;
}

