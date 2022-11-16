#include "application.h"
#include <QTimer>

void statusManager::handle_2D_AEC(void){
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
        qDebug() << "EXPOSURE 2D+AEC START SEQUENCE";
        clearPostExposureList();
        aecDataPresent = false;
        exposureError = false;
        error_code = 0;
        break;

    case 1:
        // Validate the exposure data
        if(!validate2DExposurePre()){
            exposureError = true;
            error_code = Interface::_EXP_ERR_PRE_VALIDATION;
            QTimer::singleShot(0, this, SLOT(handleCurrentStatus()));
            return;
        }

        // Load Data Bank
        wait_command_processed = true;
        COMMUNICATION->set2DDataBank(R2CP::DB_Pre,focus,pre_kV,pre_mAs);
        break;

    case 2:       

        // Procedure activation
        wait_command_processed = true;
        COMMUNICATION->activate2DAecProcedurePre();
        break;

    case 3:

        // Get Status
        wait_command_processed = true;
        COMMUNICATION->getGeneratorStatusV6();
        break;

    case 4:        

        // Clear the disable Rx message
        wait_command_processed = true;
        COMMUNICATION->setDisableRxSystemMessage(false);
        break;

    case 5:       

        // Clear all the System Messages
        if(R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.SystemMessage.Fields.Active == R2CP::Stat_SystemMessageActive_Active){
            changeStatus(SMS_CLEAR_SYS_MESSAGES,0,internalState,subStatus+1);
            return;
        }
        break;

    case 6: // Test Not cleared messages

        if(R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.SystemMessage.Fields.Active == R2CP::Stat_SystemMessageActive_Active){
            exposureError = true;
            error_code = Interface::_EXP_ERR_GENERATOR_ERRORS;
            QTimer::singleShot(0, this, SLOT(handleCurrentStatus()));
            return;
        }

        qDebug() << "XRAY-ENA ACTIVATION..";

        // Ask for the Xray Request signal activation
        INTERFACE->EventSetXrayEna(0,true);
        break;

    case 7:
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
                exposureError = true;
                error_code = Interface::_EXP_ERR_GENERATOR_STATUS;
                QTimer::singleShot(0, this, SLOT(handleCurrentStatus()));
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
            QTimer::singleShot(10, this, SLOT(handleCurrentStatus()));
            return;
        }

        qDebug() << "AEC data received";

        if(R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.SystemMessage.Fields.Active == R2CP::Stat_SystemMessageActive_Active){
            exposureError = true;
            error_code = Interface::_EXP_ERR_GENERATOR_ERRORS;
            QTimer::singleShot(0, this, SLOT(handleCurrentStatus()));
            return;
        }

        // Validate the exposure data
        if(!validate2DExposurePulse()){
            exposureError = true;
            error_code = Interface::_EXP_ERR_PULSE_VALIDATION;
            QTimer::singleShot(0, this, SLOT(handleCurrentStatus()));
            return;
        }

        // Load Data Bank
        wait_command_processed = true;
        COMMUNICATION->set2DDataBank(R2CP::DB_Pulse,focus,pulse_kV,pulse_mAs);
        break;

    case 10:

        // Procedure activation
        wait_command_processed = true;
        COMMUNICATION->activate2DAecProcedurePulse();
        break;

    case 11:

        // Get Status
        wait_command_processed = true;
        COMMUNICATION->getGeneratorStatusV6();
        break;


    case 12:
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
