#include "application.h"
#include <QTimer>

void statusManager::handle_2D_AEC(void){
    QList<Interface::tPostExposureData> postExposure;
    float totalPremAs;
    float totalPulsemAs;
    static uchar current_status;
    static bool exposureError = false;
    static unsigned char error_code;

    if((subStatus!=0) && ((abortRxRequest) || (!command_process_state) || (exposureError))){
        postExposure = STATUS->getPostExposureList();
        totalPulsemAs = 0;
        totalPremAs = 0;
        for (int i=0; i< postExposure.size(); i++){
            if(postExposure[i].pulse_seq == R2CP::DB_Pre) totalPremAs += postExposure[i].mAs;
            else if(postExposure[i].pulse_seq == R2CP::DB_Pulse) totalPulsemAs += postExposure[i].mAs;
        }

        if(abortRxRequest) qDebug() << "Abort Rx Command Executed: total PRE mAs:" << totalPremAs << " toal Pulse mAs:" << totalPulsemAs;
        else if(!command_process_state) qDebug() << "Cp Error on Substatus = " << subStatus << " total PRE mAs:" << totalPremAs << " toal Pulse mAs:" << totalPulsemAs;
        else qDebug() << "Exposure Error on Substatus = " << subStatus << " total PRE mAs:" << totalPremAs << " toal Pulse mAs:" << totalPulsemAs;

        if(totalPulsemAs+totalPremAs) INTERFACE->EventXrayCompleted(0,Interface::_EXPOSURE_ABORT, totalPremAs, totalPulsemAs, error_code);
        else INTERFACE->EventXrayCompleted(0,Interface::_EXPOSURE_PARTIAL,totalPremAs, totalPulsemAs, error_code);

        pulseExposureData.valid = false;
        preExposureData.valid = false;
        changeStatus(SMS_IDLE,0,SMS_IDLE,0);
        return;
    }

    current_status = R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.GeneratorStatus;


    switch(subStatus){
    case 0:
        qDebug() << "EXPOSURE 2D+AEC START SEQUENCE";
        clearPostExposureList();        
        pulseExposureData.valid = false;

        exposureError = false;
        error_code = 0;
        break;

    case 1:
        // Validate the exposure data
        if(!preExposureData.valid){
            exposureError = true;
            error_code = Interface::_EXP_ERR_PRE_VALIDATION;
            qDebug() << "_EXP_ERR_PRE_VALIDATION";
            QTimer::singleShot(0, this, SLOT(handleCurrentStatus()));
            return;
        }

        // Load Data Bank
        wait_command_processed = true;
        COMMUNICATION->set2DDataBank(R2CP::DB_Pre,preExposureData.focus,preExposureData.kV,preExposureData.mAs, 5000);
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
            qDebug() << "_EXP_ERR_GENERATOR_ERRORS";
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
                    qDebug() << "_EXP_ERR_GENERATOR_ERRORS";
                    QTimer::singleShot(0, this, SLOT(handleCurrentStatus()));
                    return;
                }

                QTimer::singleShot(10, this, SLOT(handleCurrentStatus()));
                return;

            case R2CP::Stat_Error:
                exposureError = true;
                error_code = Interface::_EXP_ERR_GENERATOR_ERRORS;
                qDebug() << "_EXP_ERR_GENERATOR_ERRORS";
                QTimer::singleShot(0, this, SLOT(handleCurrentStatus()));
                return;

            case R2CP::Stat_WaitFootRelease:
            case R2CP::Stat_GoigToShutdown:
            case R2CP::Stat_Service:
            case R2CP::Stat_Initialization:
                exposureError = true;
                error_code = Interface::_EXP_ERR_GENERATOR_STATUS;
                qDebug() << "_EXP_ERR_GENERATOR_STATUS";
                QTimer::singleShot(0, this, SLOT(handleCurrentStatus()));
                return;


            case R2CP::Stat_ExpInProgress:
            case R2CP::Stat_Ready:
                break;

        }

        break;

      case 8:
        if(!R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.ExposureSwitches.Fields.ExpsignalStatus){
            exposureError = true;
            error_code = Interface::_EXP_ERR_XRAY_ENA_EARLY_RELEASED;
            qDebug() << "_EXP_ERR_XRAY_ENA_EARLY_RELEASED";
            QTimer::singleShot(0, this, SLOT(handleCurrentStatus()));
            return;
        }

        // Wait for Standby
        switch(current_status){

            case R2CP::Stat_Standby:
                qDebug() << "Pre pulse completed: wait for the AEC data";
                INTERFACE->EventGetPulseData(0);
                abortTimeoutRequest = false;
                QTimer::singleShot(preExposureData.tmo, this, SLOT(timeoutAec()));
                break;

            case R2CP::Stat_Error:
                exposureError = true;
                error_code = Interface::_EXP_ERR_GENERATOR_ERRORS;
                qDebug() << "_EXP_ERR_GENERATOR_ERRORS";
                QTimer::singleShot(0, this, SLOT(handleCurrentStatus()));
                return;

            case R2CP::Stat_WaitFootRelease:
            case R2CP::Stat_GoigToShutdown:
            case R2CP::Stat_Service:
            case R2CP::Stat_Initialization:

                exposureError = true;
                error_code = Interface::_EXP_ERR_GENERATOR_STATUS;
                qDebug() << "_EXP_ERR_GENERATOR_STATUS";
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
        if(abortTimeoutRequest){
            exposureError = true;
            error_code = Interface::_EXP_ERR_AEC_TMO;
            qDebug() << "_EXP_ERR_AEC_TMO";
            QTimer::singleShot(0, this, SLOT(handleCurrentStatus()));
            return;
        }

        if(!R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.ExposureSwitches.Fields.ExpsignalStatus){
            exposureError = true;
            error_code = Interface::_EXP_ERR_XRAY_ENA_EARLY_RELEASED;
            qDebug() << "_EXP_ERR_XRAY_ENA_EARLY_RELEASED";
            QTimer::singleShot(0, this, SLOT(handleCurrentStatus()));
            return;
        }

        if(!pulseExposureData.valid){
            QTimer::singleShot(10, this, SLOT(handleCurrentStatus()));
            return;
        }

        qDebug() << "Pulse received from the AEC";

        if(R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.SystemMessage.Fields.Active == R2CP::Stat_SystemMessageActive_Active){
            exposureError = true;
            error_code = Interface::_EXP_ERR_GENERATOR_ERRORS;
            qDebug() << "_EXP_ERR_GENERATOR_ERRORS";
            QTimer::singleShot(0, this, SLOT(handleCurrentStatus()));
            return;
        }

        // Load Data Bank
        wait_command_processed = true;
        COMMUNICATION->set2DDataBank(R2CP::DB_Pulse,pulseExposureData.focus,pulseExposureData.kV,pulseExposureData.mAs, 5000);
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

        if(!R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.ExposureSwitches.Fields.ExpsignalStatus){
            exposureError = true;
            error_code = Interface::_EXP_ERR_XRAY_ENA_EARLY_RELEASED;
            qDebug() << "_EXP_ERR_XRAY_ENA_EARLY_RELEASED";
            QTimer::singleShot(0, this, SLOT(handleCurrentStatus()));
            return;
        }

        // Wait for the Exposure in progress
        switch(current_status){
            case R2CP::Stat_Standby:

                if(R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.SystemMessage.Fields.Active == R2CP::Stat_SystemMessageActive_Active){
                    exposureError = true;
                    error_code = Interface::_EXP_ERR_GENERATOR_ERRORS;
                    qDebug() << "_EXP_ERR_GENERATOR_ERRORS";
                    QTimer::singleShot(0, this, SLOT(handleCurrentStatus()));
                    return;
                }

                QTimer::singleShot(10, this, SLOT(handleCurrentStatus()));
                return;


            case R2CP::Stat_Error:
                exposureError = true;
                error_code = Interface::_EXP_ERR_GENERATOR_ERRORS;
                qDebug() << "_EXP_ERR_GENERATOR_ERRORS";
                QTimer::singleShot(0, this, SLOT(handleCurrentStatus()));
                return;

            case R2CP::Stat_WaitFootRelease:
            case R2CP::Stat_GoigToShutdown:
            case R2CP::Stat_Service:
            case R2CP::Stat_Initialization:
                exposureError = true;
                error_code = Interface::_EXP_ERR_GENERATOR_STATUS;
                qDebug() << "_EXP_ERR_GENERATOR_STATUS";
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
                exposureError = true;
                error_code = Interface::_EXP_ERR_GENERATOR_ERRORS;
                qDebug() << "_EXP_ERR_GENERATOR_ERRORS";
                QTimer::singleShot(0, this, SLOT(handleCurrentStatus()));
                return;

            case R2CP::Stat_GoigToShutdown:
            case R2CP::Stat_Service:
            case R2CP::Stat_Initialization:
                exposureError = true;
                error_code = Interface::_EXP_ERR_GENERATOR_STATUS;
                qDebug() << "_EXP_ERR_GENERATOR_STATUS";
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

        postExposure = STATUS->getPostExposureList();
        totalPulsemAs = 0;
        totalPremAs = 0;
        for (int i=0; i< postExposure.size(); i++){
            if(postExposure[i].pulse_seq == R2CP::DB_Pre) totalPremAs += postExposure[i].mAs;
            else if(postExposure[i].pulse_seq == R2CP::DB_Pulse) totalPulsemAs += postExposure[i].mAs;
        }
        qDebug() << "EXPOSURE 2D+AEC TERMINATED. Total PRE mAs:" << totalPremAs << ", Total Pulse mAs:" << totalPulsemAs;


        INTERFACE->EventXrayCompleted(0,Interface::_EXPOSURE_COMPLETED,totalPremAs, totalPulsemAs, Interface::_EXP_ERR_NONE);
        pulseExposureData.valid = false;
        preExposureData.valid = false;
        changeStatus(SMS_IDLE,0,SMS_IDLE,0);
        return;
    }

    subStatus++;
    QTimer::singleShot(10, this, SLOT(handleCurrentStatus()));

}
