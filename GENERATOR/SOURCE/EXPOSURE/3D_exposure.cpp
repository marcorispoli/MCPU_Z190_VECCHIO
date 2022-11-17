#include "application.h"
#include <QTimer>

void statusManager::handle_3D_MANUAL(void){
    static uchar current_status;
    static bool exposureError = false;
    static unsigned char error_code;


    if((subStatus!=0) && ((abortRxRequest) || (!command_process_state) || (exposureError))){
        if(abortRxRequest) qDebug() << "Abort Rx Command Executed";
        else if(!command_process_state) qDebug() << "Cp Error on Substatus = " << subStatus;
        else qDebug() << "Exposure Error on Substatus = " << subStatus;

        QList<Interface::tPostExposureData> list = STATUS->getPostExposureList();

        if(list.size() == 0) INTERFACE->EventXrayCompleted(0,Interface::_EXPOSURE_ABORT,error_code);
        else INTERFACE->EventXrayCompleted(0,Interface::_EXPOSURE_PARTIAL,error_code);
        changeStatus(SMS_IDLE,0,SMS_IDLE,0);
        return;
    }

    current_status = R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.GeneratorStatus;

    switch(subStatus){
    case 0:
        qDebug() << "EXPOSURE 3D START SEQUENCE";
        clearPostExposureList();
        aecDataPresent = false;
        exposureError = false;
        error_code = 0;
        break;

    case 1:
        // Verify if theprocedure needs to be rebuilded
        if((!procedureCreated) || (tomo_n_samples != fps) || (tomo_n_skip != skip)){
            qDebug() << "Tomo Procedure Setup";
            tomo_n_samples = fps;
            tomo_n_skip = skip;
            changeStatus(SMS_SETUP_GENERATOR,0,internalState,subStatus+1);
            return;
        }
        break;

    case 2:
        if(!procedureCreated){
            exposureError = true;
            error_code = Interface::_EXP_ERR_PROCEDURE_SETUP;
            QTimer::singleShot(0, this, SLOT(handleCurrentStatus()));
            return;
        }

        // Validate the exposure data
        if(!validate3DExposurePulse()){
            exposureError = true;
            error_code = Interface::_EXP_ERR_PRE_VALIDATION;
            QTimer::singleShot(0, this, SLOT(handleCurrentStatus()));
            return;
        }

        // Load Data Bank
        wait_command_processed = true;
        COMMUNICATION->set2DDataBank(R2CP::DB_Pulse,focus,pulse_kV,pulse_mAs);
        break;

    case 3:

        // Procedure activation
        wait_command_processed = true;
        COMMUNICATION->activate3DProcedurePulse();
        break;

    case 4:

        // Get Status
        wait_command_processed = true;
        COMMUNICATION->getGeneratorStatusV6();
        break;

    case 5:

        // Clear the disable Rx message
        wait_command_processed = true;
        COMMUNICATION->setDisableRxSystemMessage(false);
        break;

    case 6:

        // Clear all the System Messages
        if(R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.SystemMessage.Fields.Active == R2CP::Stat_SystemMessageActive_Active){
            changeStatus(SMS_CLEAR_SYS_MESSAGES,0,internalState,subStatus+1);
            return;
        }
        break;

    case 7: // Test Not cleared messages

        if(R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.SystemMessage.Fields.Active == R2CP::Stat_SystemMessageActive_Active){
            exposureError = true;
            error_code = Interface::_EXP_ERR_GENERATOR_ERRORS;
            QTimer::singleShot(0, this, SLOT(handleCurrentStatus()));
            return;
        }

        qDebug() << "XRAY-ENA ACTIVATION..";

        INTERFACE->EventSetXrayEna(0,true);
        break;

    case 8:

        // Wait for the PRX and XRAY ENA
        if(R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.ExposureSwitches.Fields.ExpsignalStatus) break;
        QTimer::singleShot(0, this, SLOT(handleCurrentStatus()));
        return;

    case 9:
        if(!R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.ExposureSwitches.Fields.ExpsignalStatus){
            exposureError = true;
            error_code = Interface::_EXP_ERR_XRAY_ENA_EARLY_RELEASED;
            qDebug() << " XRAY-ENA EARLY RELEASED";
            QTimer::singleShot(0, this, SLOT(handleCurrentStatus()));
            return;
        }

        // Wait for the Exposure in progress
        switch(current_status){
            case R2CP::Stat_Standby:

                if(R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.SystemMessage.Fields.Active == R2CP::Stat_SystemMessageActive_Active){
                    exposureError = true;
                    error_code = Interface::_EXP_ERR_GENERATOR_ERRORS;
                    QTimer::singleShot(0, this, SLOT(handleCurrentStatus()));
                    return;
                }

                QTimer::singleShot(10, this, SLOT(handleCurrentStatus()));
                return;

            case R2CP::Stat_Error:
            case R2CP::Stat_WaitFootRelease:
            case R2CP::Stat_GoigToShutdown:
            case R2CP::Stat_Service:
            case R2CP::Stat_Initialization:
                exposureError = true;
                error_code = Interface::_EXP_ERR_GENERATOR_STATUS;
                QTimer::singleShot(0, this, SLOT(handleCurrentStatus()));
                return;

            case R2CP::Stat_ExpInProgress:
            case R2CP::Stat_Ready:
                break;

        }

        break;

    case 10:

        // Wait for Standby
        switch(current_status){

            case R2CP::Stat_WaitFootRelease:
            case R2CP::Stat_Standby:
                if(!R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.ExposureSwitches.Fields.ExpsignalStatus){
                    exposureError = true;
                    error_code = Interface::_EXP_ERR_XRAY_ENA_EARLY_RELEASED;
                    qDebug() << " XRAY-ENA EARLY RELEASED";
                    QTimer::singleShot(0, this, SLOT(handleCurrentStatus()));
                    return;
                }

                INTERFACE->EventSetXrayEna(0,false);
            break;


            case R2CP::Stat_Error:
                qDebug() << "Stat Error";
            case R2CP::Stat_GoigToShutdown:
            case R2CP::Stat_Service:
            case R2CP::Stat_Initialization:
                exposureError = true;
                error_code = Interface::_EXP_ERR_GENERATOR_STATUS;
                QTimer::singleShot(0, this, SLOT(handleCurrentStatus()));
                return;

            case R2CP::Stat_ExpInProgress:               
            case R2CP::Stat_Ready:
            case R2CP::Stat_Preparation:         
                QTimer::singleShot(10, this, SLOT(handleCurrentStatus()));
                return;

            default:
                QTimer::singleShot(10, this, SLOT(handleCurrentStatus()));
                return;
        }

        break;

    case 11:
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
        qDebug() << "EXPOSURE 3D MANUAL TERMINATED";
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
