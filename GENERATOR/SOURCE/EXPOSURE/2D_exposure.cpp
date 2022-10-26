#include "application.h"
#include <QTimer>

void statusManager::handle_2D_MANUAL(void){
    static uchar current_status;

    current_status = R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.GeneratorStatus;

    switch(subStatus){
    case 0:
        qDebug() << "EXPOSURE 2D STATUS";
        clearPostExposureList();
        break;

    case 1:
        // Validate the exposure data
        if(!validate2DExposurePulse()){
            qDebug() << "EXPOSURE 2D MANUAL FAILED: VALIDATION FAILED";
            INTERFACE->EventXrayCompleted(0,Interface::_EXPOSURE_ABORT,Interface::_EXP_ERR_PULSE_VALIDATION);
            changeStatus(SMS_IDLE,0,SMS_IDLE,0);
            return;
        }

        // Load Data Bank
        wait_command_processed = true;
        COMMUNICATION->set2DDataBank(R2CP::DB_Pulse,focus,pulse_kV,pulse_mAs);
        break;

    case 2:
        if(!command_process_state){
            qDebug() << "EXPOSURE 2D MANUAL FAILED: SET DATABANK COMMAND FAILED. " << COMMUNICATION->getCommandProcessedString();
            INTERFACE->EventXrayCompleted(0,Interface::_EXPOSURE_ABORT,Interface::_EXP_ERR_PULSE_VALIDATION);
            changeStatus(SMS_IDLE,0,SMS_IDLE,0);
            return;
        }

        // Procedure activation
        wait_command_processed = true;
        COMMUNICATION->activate2DProcedurePulse(use_detector, use_grid);
        break;

    case 3:
        if(!command_process_state){
            qDebug() << "EXPOSURE 2D MANUAL FAILED: PROCEDURE ACTIVATION FAILED";
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
            qDebug() << "EXPOSURE 2D MANUAL FAILED: GET STATUS FAILED";
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
            qDebug() << "EXPOSURE 2D MANUAL FAILED: CLEAR DISABLE MESSAGE FAILED";
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
            qDebug() << "EXPOSURE 2D MANUAL FAILED: GENERATOR ERROR MESSAGES";
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

        INTERFACE->EventSetXrayEna(0,true);
        break;

    case 7:

        // Wait for the Exposure in progress
        switch(current_status){
            case R2CP::Stat_Standby:

                if(abortRxRequest){
                    qDebug() << "EXPOSURE 2D MANUAL FAILED:ABORT REQUEST";
                    INTERFACE->EventXrayCompleted(0,Interface::_EXPOSURE_ABORT,Interface::_EXP_ERR_ABORT_REQUEST);
                    changeStatus(SMS_IDLE,0,SMS_IDLE,0);
                    return;
                }

                if(R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.SystemMessage.Fields.Active == R2CP::Stat_SystemMessageActive_Active){
                    qDebug() << "EXPOSURE 2D MANUAL FAILED: GENERATOR ERROR MESSAGES";
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
                qDebug() << "EXPOSURE 2D MANUAL FAILED: PREPARATION ERROR";
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

            case R2CP::Stat_WaitFootRelease:
            case R2CP::Stat_Standby:
                qDebug() << "XRAY-ENA DEACTIVATION..";
                INTERFACE->EventSetXrayEna(0,false);
                break;


            case R2CP::Stat_Error:               
            case R2CP::Stat_GoigToShutdown:
            case R2CP::Stat_Service:
            case R2CP::Stat_Initialization:
                qDebug() << "EXPOSURE 2D MANUAL FAILED: ERROR DURING XRAY";
                INTERFACE->EventXrayCompleted(0,Interface::_EXPOSURE_PARTIAL,Interface::_EXP_ERR_GENERATOR_ERRORS);
                changeStatus(SMS_IDLE,0,SMS_IDLE,0);
                return;

            case R2CP::Stat_ExpInProgress:
                if(abortRxRequest){
                    qDebug() << "EXPOSURE 2D MANUAL FAILED:ABORT REQUEST";
                    INTERFACE->EventXrayCompleted(0,Interface::_EXPOSURE_PARTIAL,Interface::_EXP_ERR_ABORT_REQUEST);
                    changeStatus(SMS_IDLE,0,SMS_IDLE,0);
                    return;
                }
                QTimer::singleShot(10, this, SLOT(handleCurrentStatus()));
                return;

            case R2CP::Stat_Ready:
            case R2CP::Stat_Preparation:

                if(abortRxRequest){
                    qDebug() << "EXPOSURE 2D MANUAL FAILED:ABORT REQUEST";
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

    case 9:
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
        qDebug() << "EXPOSURE 2D MANUAL TERMINATED";
        INTERFACE->EventXrayCompleted(0,Interface::_EXPOSURE_COMPLETED,0);
        changeStatus(SMS_IDLE,0,SMS_IDLE,0);
        return;
    }

    subStatus++;
    QTimer::singleShot(10, this, SLOT(handleCurrentStatus()));

}



/*
void statusManager::handle_2D_MANUAL(void){
    static uchar oldstat;
    bool  chgstat = false;

    if(R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.GeneratorStatus != oldstat){
        chgstat = true;
        oldstat = R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.GeneratorStatus;
    }

    WINDOWS->onRecetionGenStatusSlot();

    switch(subStatus){
    case 0:
        WINDOWS->setStatus("EXPOSURE 2D");
        qDebug() << "EXPOSURE 2D STATUS";
        break;

    case 1:
        // Validate the exposure data
        if(!validate2DExposurePulse()){
            setErrorCondition(SMS_ERR_EXPOSURE, SMS_EXP_ERR_PULSE_VALIDATION, "EXPOSURE DATA VALIDATION",SMS_IDLE,0);
            return;
        }

        // Load Data Bank
        wait_command_processed = true;
        COMMUNICATION->set2DDataBank(R2CP::DB_Pulse,focus,pulse_kV,pulse_mAs,mA,mS);
        break;

    case 2:
        if(!command_process_state){
            setErrorCondition(SMS_ERR_EXPOSURE, SMS_EXP_ERR_PREPARATION, "SET DATABANK",SMS_IDLE,0);
            return;
        }

        // Procedure activation
        wait_command_processed = true;
        COMMUNICATION->activate2DProcedurePulse();
        break;

    case 3:
        if(!command_process_state){
            setErrorCondition(SMS_ERR_EXPOSURE, SMS_EXP_ERR_PREPARATION, "PROCEDURE ACTIVATION",SMS_IDLE,0);
            return;

        }
        break;

    case 4: // Get status to check the generator before to proceed
        wait_command_processed = true;
        COMMUNICATION->getGeneratorStatusV6();
        break;

    case 5:
        if(!command_process_state){
            setErrorCondition(SMS_ERR_EXPOSURE, SMS_EXP_ERR_PREPARATION, "GET STATUS",SMS_IDLE,0);
            return;
        }
        break;

    case 6:  // Clear all the System Messages
        if(R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.SystemMessage.Fields.Active == R2CP::Stat_SystemMessageActive_Active){
            changeStatus(SMS_CLEAR_SYS_MESSAGES,0,internalState,subStatus+1);
            return;
        }
        break;    

    case 7: // Test Not cleared messages

        if(R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.SystemMessage.Fields.Active == R2CP::Stat_SystemMessageActive_Active){
            setErrorCondition(SMS_ERR_EXPOSURE, SMS_EXP_ERR_SYSMSG, "SYSTEM MESSAGES PRESENTS",SMS_IDLE,0);
            return;
        }

        qDebug() << "WAITING PUSH BUTTON PRESS..";
        wait_command_processed = true;
        COMMUNICATION->getGeneratorStatusV6();
        break;

    case 8:
        if(!command_process_state){
            setErrorCondition(SMS_ERR_EXPOSURE, SMS_EXP_ERR_PREPARATION, "GET STATUS",SMS_IDLE,0);
            return;
        }

        // Wait for the Exposure in progress
        switch(oldstat){
            case R2CP::Stat_Preparation:
            case R2CP::Stat_Standby:

                if(R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.SystemMessage.Fields.Active == R2CP::Stat_SystemMessageActive_Active){
                    setErrorCondition(SMS_ERR_EXPOSURE, SMS_EXP_ERR_SYSMSG, "SYSTEM MESSAGES PRESENTS",SMS_IDLE,0);
                    return;
                }

                QTimer::singleShot(10, this, SLOT(handleCurrentStatus()));
                return;

            case R2CP::Stat_Ready:
                if(chgstat) qDebug() << "EXPOSURE READY ..";
                break;


            case R2CP::Stat_Error:
                setErrorCondition(SMS_ERR_EXPOSURE, SMS_EXP_ERR_PREPARATION, "GENERATOR ERROR CONDITION",SMS_IDLE,0);
                return;

            case R2CP::Stat_ExpInProgress:
            case R2CP::Stat_WaitFootRelease:
            case R2CP::Stat_GoigToShutdown:            
            case R2CP::Stat_Service:
            case R2CP::Stat_Initialization:
                setErrorCondition(SMS_ERR_EXPOSURE, SMS_EXP_ERR_PREPARATION, "INVALID GENERATOR STATUS",SMS_IDLE,0);
                return;
        }

        break;

    case 9:
        qDebug() << "START EXPOSURE";

        wait_command_processed = true;
        COMMUNICATION->startExposure();
        break;

    case 10:
        if(!command_process_state){
            setErrorCondition(SMS_ERR_EXPOSURE, SMS_EXP_ERR_PREPARATION, "START EXPOSURE COMMAND",SMS_IDLE,0);
            return;
        }

        // Wait for Standby
        switch(oldstat){

            case R2CP::Stat_Standby: break;


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
        qDebug() << "EXPOSURE COMPLETED";
        break;

    default:
        changeStatus(SMS_IDLE,0,SMS_IDLE,0);
        return;
    }

    subStatus++;
    QTimer::singleShot(10, this, SLOT(handleCurrentStatus()));

}

*/
