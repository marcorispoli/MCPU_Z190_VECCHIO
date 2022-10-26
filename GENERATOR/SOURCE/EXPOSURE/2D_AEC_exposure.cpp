#include "application.h"
#include <QTimer>

void statusManager::handle_2D_AEC(void){
    static uchar current_status;

    current_status = R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.GeneratorStatus;

    switch(subStatus){
    case 0:
        qDebug() << "EXPOSURE 2D+AEC STATUS";
        clearPostExposureList();
        aecDataPresent = false;
        break;

    case 1:
        // Validate the exposure data
        if(!validate2DExposurePre()){
            qDebug() << "EXPOSURE 2D+AEC FAILED: VALIDATION FAILED";
            INTERFACE->EventXrayCompleted(0,Interface::_EXPOSURE_ABORT,Interface::_EXP_ERR_PRE_VALIDATION);
            changeStatus(SMS_IDLE,0,SMS_IDLE,0);
            return;
        }

        // Load Data Bank
        wait_command_processed = true;
        COMMUNICATION->set2DDataBank(R2CP::DB_Pre,focus,pre_kV,pre_mAs);
        break;

    case 2:
        if(!command_process_state){
            qDebug() << "EXPOSURE 2D+AEC FAILED: SET DATABANK COMMAND FAILED. " << COMMUNICATION->getCommandProcessedString();
            INTERFACE->EventXrayCompleted(0,Interface::_EXPOSURE_ABORT,Interface::_EXP_ERR_PRE_VALIDATION);
            changeStatus(SMS_IDLE,0,SMS_IDLE,0);
            return;
        }

        // Procedure activation
        wait_command_processed = true;
        COMMUNICATION->activate2DAecProcedurePre();
        break;

    case 3:
        if(!command_process_state){
            qDebug() << "EXPOSURE 2D+AEC FAILED: PROCEDURE ACTIVATION FAILED";
            INTERFACE->EventXrayCompleted(0,Interface::_EXPOSURE_ABORT,Interface::_EXP_ERR_GENERATOR_COMMAND);
            changeStatus(SMS_IDLE,0,SMS_IDLE,0);
            return;

        }


        // Get Status
        wait_command_processed = true;
        COMMUNICATION->getGeneratorStatusV6();
        break;

    case 4:
        if(!command_process_state){
            qDebug() << "EXPOSURE 2D+AEC FAILED: GET STATUS FAILED";
            INTERFACE->EventXrayCompleted(0,Interface::_EXPOSURE_ABORT,Interface::_EXP_ERR_GENERATOR_COMMAND);
            changeStatus(SMS_IDLE,0,SMS_IDLE,0);
            return;
        }

        // Clear the disable Rx message
        wait_command_processed = true;
        COMMUNICATION->setDisableRxSystemMessage(false);
        break;

    case 5:
        if(!command_process_state){
            qDebug() << "EXPOSURE 2D+AEC FAILED: CLEAR DISABLE MESSAGE FAILED";
            INTERFACE->EventXrayCompleted(0,Interface::_EXPOSURE_ABORT,Interface::_EXP_ERR_GENERATOR_COMMAND);
            changeStatus(SMS_IDLE,0,SMS_IDLE,0);
            return;
        }

        // Clear all the System Messages
        if(R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.SystemMessage.Fields.Active == R2CP::Stat_SystemMessageActive_Active){
            changeStatus(SMS_CLEAR_SYS_MESSAGES,0,internalState,subStatus+1);
            return;
        }
        break;

    case 6: // Test Not cleared messages

        if(R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.SystemMessage.Fields.Active == R2CP::Stat_SystemMessageActive_Active){
            qDebug() << "EXPOSURE 2D+AEC FAILED: GENERATOR ERROR MESSAGES";
            INTERFACE->EventXrayCompleted(0,Interface::_EXPOSURE_ABORT,Interface::_EXP_ERR_GENERATOR_ERRORS);
            changeStatus(SMS_IDLE,0,SMS_IDLE,0);
            return;;
        }

        qDebug() << "XRAY-ENA ACTIVATION..";

        if(abortRxRequest){
            qDebug() << "EXPOSURE 2D MANUAL FAILED:ABORT REQUEST";
            INTERFACE->EventXrayCompleted(0,Interface::_EXPOSURE_ABORT,Interface::_EXP_ERR_ABORT_REQUEST);
            changeStatus(SMS_IDLE,0,SMS_IDLE,0);
            return;
        }

        // Ask for the Xray Request signal activation
        INTERFACE->EventSetXrayEna(0,true);
        break;

    case 7:

        // Wait for the Exposure in progress
        switch(current_status){
            case R2CP::Stat_Standby:

                if(abortRxRequest){
                    qDebug() << "EXPOSURE 2D+AEC  FAILED:ABORT REQUEST";
                    INTERFACE->EventXrayCompleted(0,Interface::_EXPOSURE_ABORT,Interface::_EXP_ERR_ABORT_REQUEST);
                    changeStatus(SMS_IDLE,0,SMS_IDLE,0);
                    return;
                }

                if(R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.SystemMessage.Fields.Active == R2CP::Stat_SystemMessageActive_Active){
                    qDebug() << "EXPOSURE 2D+AEC FAILED: GENERATOR ERROR MESSAGES";
                    INTERFACE->EventXrayCompleted(0,Interface::_EXPOSURE_ABORT,Interface::_EXP_ERR_GENERATOR_ERRORS);
                    changeStatus(SMS_IDLE,0,SMS_IDLE,0);
                    return;
                }

                QTimer::singleShot(10, this, SLOT(handleCurrentStatus()));
                return;

            case R2CP::Stat_Error:
            case R2CP::Stat_WaitFootRelease:
            case R2CP::Stat_GoigToShutdown:
            case R2CP::Stat_Service:
            case R2CP::Stat_Initialization:
                qDebug() << "EXPOSURE 2D+AEC FAILED: PREPARATION ERROR";
                INTERFACE->EventXrayCompleted(0,Interface::_EXPOSURE_ABORT,Interface::_EXP_ERR_GENERATOR_ERRORS);
                changeStatus(SMS_IDLE,0,SMS_IDLE,0);
                return;

            case R2CP::Stat_ExpInProgress:
            case R2CP::Stat_Ready:
                break;

        }

        break;

      case 8:

        // Wait for Standby
        switch(current_status){

            case R2CP::Stat_Standby:
                qDebug() << "Pre pulse completed: wait for the AEC data";
                INTERFACE->EventGetPulseData(0);
                break;

            case R2CP::Stat_WaitFootRelease:
            case R2CP::Stat_GoigToShutdown:
            case R2CP::Stat_Service:
            case R2CP::Stat_Initialization:
            case R2CP::Stat_Error:
                qDebug() << "EXPOSURE 2D+AEC FAILED: PRE PULSE ERROR";
                INTERFACE->EventXrayCompleted(0,Interface::_EXPOSURE_ABORT,Interface::_EXP_ERR_GENERATOR_ERRORS);
                changeStatus(SMS_IDLE,0,SMS_IDLE,0);
                return;            

            case R2CP::Stat_Preparation:
            case R2CP::Stat_Ready:
            case R2CP::Stat_ExpInProgress:
                QTimer::singleShot(10, this, SLOT(handleCurrentStatus()));
                return;

            default:
                QTimer::singleShot(10, this, SLOT(handleCurrentStatus()));
                return;
        }

        break;

    case 9:
        if(!aecDataPresent){

            if(abortRxRequest){
                qDebug() << "EXPOSURE 2D+AEC FAILED:ABORT REQUEST";
                INTERFACE->EventXrayCompleted(0,Interface::_EXPOSURE_ABORT,Interface::_EXP_ERR_ABORT_REQUEST);
                changeStatus(SMS_IDLE,0,SMS_IDLE,0);
                return;
            }

            QTimer::singleShot(10, this, SLOT(handleCurrentStatus()));
            return;
        }

        qDebug() << "AEC data received";

        if(R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.SystemMessage.Fields.Active == R2CP::Stat_SystemMessageActive_Active){
            qDebug() << "EXPOSURE 2D+AEC FAILED: GENERATOR ERROR MESSAGES";
            INTERFACE->EventXrayCompleted(0,Interface::_EXPOSURE_ABORT,Interface::_EXP_ERR_GENERATOR_ERRORS);
            changeStatus(SMS_IDLE,0,SMS_IDLE,0);
            return;
        }

        // Validate the exposure data
        if(!validate2DExposurePulse()){
            qDebug() << "EXPOSURE 2D+AEC FAILED: VALIDATION PULSE FAILED";
            INTERFACE->EventXrayCompleted(0,Interface::_EXPOSURE_ABORT,Interface::_EXP_ERR_PULSE_VALIDATION);
            changeStatus(SMS_IDLE,0,SMS_IDLE,0);
            return;
        }

        // Load Data Bank
        wait_command_processed = true;
        COMMUNICATION->set2DDataBank(R2CP::DB_Pulse,focus,pulse_kV,pulse_mAs);
        break;

    case 10:
        if(!command_process_state){
            qDebug() << "EXPOSURE 2D+AEC FAILED: SET DATABANK FOR PULSE COMMAND FAILED. " << COMMUNICATION->getCommandProcessedString();
            INTERFACE->EventXrayCompleted(0,Interface::_EXPOSURE_ABORT,Interface::_EXP_ERR_PULSE_VALIDATION);
            changeStatus(SMS_IDLE,0,SMS_IDLE,0);
            return;
        }

        // Procedure activation
        wait_command_processed = true;
        COMMUNICATION->activate2DAecProcedurePulse();
        break;

    case 11:
        if(!command_process_state){
            qDebug() << "EXPOSURE 2D+AEC FAILED: PROCEDURE PULSE ACTIVATION FAILED";
            INTERFACE->EventXrayCompleted(0,Interface::_EXPOSURE_ABORT,Interface::_EXP_ERR_GENERATOR_COMMAND);
            changeStatus(SMS_IDLE,0,SMS_IDLE,0);
            return;
        }

        // Get Status
        wait_command_processed = true;
        COMMUNICATION->getGeneratorStatusV6();
        break;


    case 12:
        // Wait for the Exposure in progress
        switch(current_status){
            case R2CP::Stat_Standby:

                if(abortRxRequest){
                    qDebug() << "EXPOSURE 2D+AEC FAILED:ABORT REQUEST";
                    INTERFACE->EventXrayCompleted(0,Interface::_EXPOSURE_ABORT,Interface::_EXP_ERR_ABORT_REQUEST);
                    changeStatus(SMS_IDLE,0,SMS_IDLE,0);
                    return;
                }

                if(R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.SystemMessage.Fields.Active == R2CP::Stat_SystemMessageActive_Active){
                    qDebug() << "EXPOSURE 2D+AEC FAILED: GENERATOR ERROR MESSAGES";
                    INTERFACE->EventXrayCompleted(0,Interface::_EXPOSURE_ABORT,Interface::_EXP_ERR_GENERATOR_ERRORS);
                    changeStatus(SMS_IDLE,0,SMS_IDLE,0);
                    return;
                }

                QTimer::singleShot(10, this, SLOT(handleCurrentStatus()));
                return;


            case R2CP::Stat_Error:
            case R2CP::Stat_WaitFootRelease:
            case R2CP::Stat_GoigToShutdown:
            case R2CP::Stat_Service:
            case R2CP::Stat_Initialization:
                qDebug() << "EXPOSURE 2D+AEC FAILED: PREPARATION PULSE ERROR";
                INTERFACE->EventXrayCompleted(0,Interface::_EXPOSURE_ABORT,Interface::_EXP_ERR_GENERATOR_ERRORS);
                changeStatus(SMS_IDLE,0,SMS_IDLE,0);
                return;

            case R2CP::Stat_ExpInProgress:
            case R2CP::Stat_Ready:
                break;
        }

        break;

    case 13:

        // Wait for Standby
        switch(current_status){

            case R2CP::Stat_WaitFootRelease:
            case R2CP::Stat_Standby:
                qDebug() << "XRAY-ENA DEACTIVATION..";
                INTERFACE->EventSetXrayEna(0,false);
                break;


            case R2CP::Stat_Error:
            case R2CP::Stat_GoigToShutdown:
            case R2CP::Stat_Service:
            case R2CP::Stat_Initialization:
                qDebug() << "EXPOSURE 2D+AEC FAILED: ERROR DURING XRAY";
                INTERFACE->EventXrayCompleted(0,Interface::_EXPOSURE_PARTIAL,Interface::_EXP_ERR_GENERATOR_ERRORS);
                changeStatus(SMS_IDLE,0,SMS_IDLE,0);
                return;

            case R2CP::Stat_ExpInProgress:
                if(abortRxRequest){
                    qDebug() << "EXPOSURE 2D+AEC FAILED:ABORT REQUEST";
                    INTERFACE->EventXrayCompleted(0,Interface::_EXPOSURE_PARTIAL,Interface::_EXP_ERR_ABORT_REQUEST);
                    changeStatus(SMS_IDLE,0,SMS_IDLE,0);
                    return;
                }
                QTimer::singleShot(10, this, SLOT(handleCurrentStatus()));
                return;

            case R2CP::Stat_Ready:
            case R2CP::Stat_Preparation:

                if(abortRxRequest){
                    qDebug() << "EXPOSURE 2D+AEC FAILED:ABORT REQUEST";
                    INTERFACE->EventXrayCompleted(0,Interface::_EXPOSURE_ABORT,Interface::_EXP_ERR_ABORT_REQUEST);
                    changeStatus(SMS_IDLE,0,SMS_IDLE,0);
                    return;
                }

                QTimer::singleShot(10, this, SLOT(handleCurrentStatus()));
                return;

            default:
                QTimer::singleShot(10, this, SLOT(handleCurrentStatus()));
                return;
        }

        break;

    case 14:
        // Wait for Standby
        switch(current_status){

            case R2CP::Stat_WaitFootRelease:
                QTimer::singleShot(10, this, SLOT(handleCurrentStatus()));
                return;

            case R2CP::Stat_Standby:
                break;
        }

        break;

    default:
        qDebug() << "EXPOSURE 2D+AEC TERMINATED";
        INTERFACE->EventXrayCompleted(0,Interface::_EXPOSURE_COMPLETED,0);
        changeStatus(SMS_IDLE,0,SMS_IDLE,0);
        return;
    }

    subStatus++;
    QTimer::singleShot(10, this, SLOT(handleCurrentStatus()));

}


/*
void statusManager::handle_2D_AEC(void){
    static uchar oldstat;
    bool  chgstat = false;

    if(R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.GeneratorStatus != oldstat){
        chgstat = true;
        oldstat = R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.GeneratorStatus;
    }

    WINDOWS->onRecetionGenStatusSlot();

    switch(subStatus){
    case 0:
        WINDOWS->setStatus("EXPOSURE 2D AEC");
        qDebug() << "EXPOSURE 2D AEC STATUS";
        aecDataPresent = false;
        break;

    case 1: // Get status to check the generator before to proceed
        wait_command_processed = true;
        COMMUNICATION->getGeneratorStatusV6();
        break;

    case 2:
        if(!command_process_state){
            setErrorCondition(SMS_ERR_EXPOSURE, SMS_EXP_ERR_PREPARATION, "GET STATUS",SMS_IDLE,0);
            return;
        }
        break;

    case 3:  // Clear all the System Messages
        if(R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.SystemMessage.Fields.Active == R2CP::Stat_SystemMessageActive_Active){
            changeStatus(SMS_CLEAR_SYS_MESSAGES,0,internalState,subStatus+1);
            return;
        }
        break;    

    case 4: // Test Not cleared messages

        if(R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.SystemMessage.Fields.Active == R2CP::Stat_SystemMessageActive_Active){
            setErrorCondition(SMS_ERR_EXPOSURE, SMS_EXP_ERR_SYSMSG, "SYSTEM MESSAGES PRESENTS",SMS_IDLE,0);
            return;
        }

        // Validate the exposure data
        if(!validate2DExposurePre()){
            setErrorCondition(SMS_ERR_EXPOSURE, SMS_EXP_ERR_PULSE_VALIDATION, "EXPOSURE PRE DATA VALIDATION",SMS_IDLE,0);
            return;
        }

        // Load Data Bank
        wait_command_processed = true;
        COMMUNICATION->set2DDataBank(R2CP::DB_Pre,focus,pre_kV,pre_mAs,mA,mS);
        break;

    case 5:
        if(!command_process_state){
            setErrorCondition(SMS_ERR_EXPOSURE, SMS_EXP_ERR_PREPARATION, "SET PRE DATABANK",SMS_IDLE,0);
            return;
        }

        // Procedure activation
        wait_command_processed = true;
        COMMUNICATION->activate2DAecProcedurePre();
        break;

    case 6:
        if(!command_process_state){
            setErrorCondition(SMS_ERR_EXPOSURE, SMS_EXP_ERR_PREPARATION, "PROCEDURE ACTIVATION",SMS_IDLE,0);
            return;

        }
        oldstat = R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.GeneratorStatus;

        subStatus = 7;
        //wait_command_processed = true;
        //COMMUNICATION->startExposure();
        break;

    case 7:
        if(!command_process_state){
            setErrorCondition(SMS_ERR_EXPOSURE, SMS_EXP_ERR_PREPARATION, "START EXPOSURE COMMAND",SMS_IDLE,0);
            return;
        }

        break;

    case 8:
        if(R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.SystemMessage.Fields.Active == R2CP::Stat_SystemMessageActive_Active){
            setErrorCondition(SMS_ERR_EXPOSURE, SMS_EXP_ERR_SYSMSG, "SYSTEM MESSAGES PRESENTS",SMS_IDLE,0);
            return;
        }

        if(oldstat == R2CP::Stat_Standby){
            qDebug() << "WAITING PUSH BUTTON PRESS..";
        }
        break;

    case 9:
        // Wait for the Exposure in progress
        switch(oldstat){
            case R2CP::Stat_Standby:                

                if(R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.SystemMessage.Fields.Active == R2CP::Stat_SystemMessageActive_Active){
                    setErrorCondition(SMS_ERR_EXPOSURE, SMS_EXP_ERR_SYSMSG, "SYSTEM MESSAGES PRESENTS",SMS_IDLE,0);
                    return;
                }

                QTimer::singleShot(10, this, SLOT(handleCurrentStatus()));
                return;

            case R2CP::Stat_Preparation:
                if(chgstat) qDebug() << "EXPOSURE PREPARATION ..";
                break;
            case R2CP::Stat_Ready:
                if(chgstat) qDebug() << "EXPOSURE READY ..";
                break;
            case R2CP::Stat_ExpInProgress:
                if(chgstat) qDebug() << "EXPOSURE IN PROGRESS ..";
                break;

            case R2CP::Stat_Error:
                setErrorCondition(SMS_ERR_EXPOSURE, SMS_EXP_ERR_PREPARATION, "GENERATOR ERROR CONDITION",SMS_IDLE,0);
                return;

            case R2CP::Stat_WaitFootRelease:
            case R2CP::Stat_GoigToShutdown:            
            case R2CP::Stat_Service:
            case R2CP::Stat_Initialization:
                setErrorCondition(SMS_ERR_EXPOSURE, SMS_EXP_ERR_PREPARATION, "INVALID GENERATOR STATUS",SMS_IDLE,0);
                return;
        }

        break;

    case 10:
        // Wait for Standby
        switch(oldstat){

            case R2CP::Stat_Standby:
                qDebug() << "WAIT FOR AEC DATA";
                break;


            case R2CP::Stat_Error:
                setErrorCondition(SMS_ERR_EXPOSURE, SMS_EXP_ERR_SEQUENCE, "GENERATOR ERROR CONDITION",SMS_IDLE,0);
                return;

            case R2CP::Stat_GoigToShutdown:
            case R2CP::Stat_Service:
            case R2CP::Stat_Initialization:
                setErrorCondition(SMS_ERR_EXPOSURE, SMS_EXP_ERR_SEQUENCE, "GENERATOR INVALID STATUS",SMS_IDLE,0);
                return;

            case R2CP::Stat_Preparation:
                if(chgstat) qDebug() << "EXPOSURE PREPARATION ..";
                QTimer::singleShot(10, this, SLOT(handleCurrentStatus()));
                return;
            case R2CP::Stat_Ready:
                if(chgstat) qDebug() << "EXPOSURE READY ..";
                QTimer::singleShot(10, this, SLOT(handleCurrentStatus()));
                return;
            case R2CP::Stat_ExpInProgress:
                if(chgstat) qDebug() << "EXPOSURE IN PROGRESS ..";
                QTimer::singleShot(10, this, SLOT(handleCurrentStatus()));
                return;

            case R2CP::Stat_WaitFootRelease:
                if(chgstat) qDebug() << "EXPOSURE WAIT PUSH RELEASE ..";
                QTimer::singleShot(10, this, SLOT(handleCurrentStatus()));
                return;

            default:
                QTimer::singleShot(10, this, SLOT(handleCurrentStatus()));
                return;
        }

        break;

    case 11:
        if(!aecDataPresent){
            QTimer::singleShot(10, this, SLOT(handleCurrentStatus()));
            return;
        }

        qDebug() << "AEC acquisito";

        if(R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.SystemMessage.Fields.Active == R2CP::Stat_SystemMessageActive_Active){
            setErrorCondition(SMS_ERR_EXPOSURE, SMS_EXP_ERR_SYSMSG, "SYSTEM MESSAGES PRESENTS",SMS_IDLE,0);
            return;
        }

        // Validate the exposure data
        if(!validate2DExposurePulse()){
            setErrorCondition(SMS_ERR_EXPOSURE, SMS_EXP_ERR_PULSE_VALIDATION, "EXPOSURE PULSE DATA VALIDATION",SMS_IDLE,0);
            return;
        }

        // Load Data Bank
        wait_command_processed = true;
        COMMUNICATION->set2DDataBank(R2CP::DB_Pulse,focus,pulse_kV,pulse_mAs,mA,mS);
        break;

    case 12:
        if(!command_process_state){
            setErrorCondition(SMS_ERR_EXPOSURE, SMS_EXP_ERR_PREPARATION, "SET PULSE DATABANK",SMS_IDLE,0);
            return;
        }

        // Procedure activation
        wait_command_processed = true;
        COMMUNICATION->activate2DAecProcedurePulse();
        break;

    case 13:
        if(!command_process_state){
            setErrorCondition(SMS_ERR_EXPOSURE, SMS_EXP_ERR_PREPARATION, "PROCEDURE AEC PULSE ACTIVATION",SMS_IDLE,0);
            return;
        }

        oldstat = R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.GeneratorStatus;
        qDebug() << "attesa esecuzione";
        break;

    case 14:
        // Wait for the Exposure in progress
        switch(oldstat){
            case R2CP::Stat_Standby:

                if(R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.SystemMessage.Fields.Active == R2CP::Stat_SystemMessageActive_Active){
                    setErrorCondition(SMS_ERR_EXPOSURE, SMS_EXP_ERR_SYSMSG, "SYSTEM MESSAGES PRESENTS",SMS_IDLE,0);
                    return;
                }

                QTimer::singleShot(10, this, SLOT(handleCurrentStatus()));
                return;

            case R2CP::Stat_Preparation:
                if(chgstat) qDebug() << "EXPOSURE PREPARATION ..";
                break;
            case R2CP::Stat_Ready:
                if(chgstat) qDebug() << "EXPOSURE READY ..";
                break;
            case R2CP::Stat_ExpInProgress:
                if(chgstat) qDebug() << "EXPOSURE IN PROGRESS ..";
                break;

            case R2CP::Stat_Error:
                setErrorCondition(SMS_ERR_EXPOSURE, SMS_EXP_ERR_PREPARATION, "GENERATOR ERROR CONDITION",SMS_IDLE,0);
                return;

            case R2CP::Stat_WaitFootRelease:
            case R2CP::Stat_GoigToShutdown:
            case R2CP::Stat_Service:
            case R2CP::Stat_Initialization:
                setErrorCondition(SMS_ERR_EXPOSURE, SMS_EXP_ERR_PREPARATION, "INVALID GENERATOR STATUS",SMS_IDLE,0);
                return;
        }

        break;

    case 15:
        // Wait for Standby
        switch(oldstat){

            case R2CP::Stat_Standby:
                qDebug() << "EXPOSURE AEC COMPLETED";
                break;


            case R2CP::Stat_Error:
                setErrorCondition(SMS_ERR_EXPOSURE, SMS_EXP_ERR_SEQUENCE, "GENERATOR ERROR CONDITION",SMS_IDLE,0);
                return;

            case R2CP::Stat_GoigToShutdown:
            case R2CP::Stat_Service:
            case R2CP::Stat_Initialization:
                setErrorCondition(SMS_ERR_EXPOSURE, SMS_EXP_ERR_SEQUENCE, "GENERATOR INVALID STATUS",SMS_IDLE,0);
                return;

            case R2CP::Stat_Preparation:
                if(chgstat) qDebug() << "EXPOSURE PREPARATION ..";
                QTimer::singleShot(10, this, SLOT(handleCurrentStatus()));
                return;
            case R2CP::Stat_Ready:
                if(chgstat) qDebug() << "EXPOSURE READY ..";
                QTimer::singleShot(10, this, SLOT(handleCurrentStatus()));
                return;
            case R2CP::Stat_ExpInProgress:
                if(chgstat) qDebug() << "EXPOSURE IN PROGRESS ..";
                QTimer::singleShot(10, this, SLOT(handleCurrentStatus()));
                return;

            case R2CP::Stat_WaitFootRelease:
                if(chgstat) qDebug() << "EXPOSURE WAIT PUSH RELEASE ..";
                QTimer::singleShot(10, this, SLOT(handleCurrentStatus()));
                return;

            default:
                QTimer::singleShot(10, this, SLOT(handleCurrentStatus()));
                return;
        }

        break;
    default:
        changeStatus(SMS_IDLE,0,SMS_IDLE,0);
        return;
    }

    subStatus++;
    QTimer::singleShot(10, this, SLOT(handleCurrentStatus()));

}

*/
