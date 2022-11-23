#include "application.h"
#include <QTimer>

void statusManager::handle_2D_MANUAL(void){
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
        for (int i=0; i< postExposure.size(); i++) totalPulsemAs += postExposure[i].mAs;

        if(abortRxRequest) qDebug() << "Abort Rx Command Executed: total PRE mAs:" << totalPremAs << " toal Pulse mAs:" << totalPulsemAs;
        else if(!command_process_state) qDebug() << "Cp Error on Substatus = " << subStatus << " total PRE mAs:" << totalPremAs << " toal Pulse mAs:" << totalPulsemAs;
        else qDebug() << "Exposure Error on Substatus = " << subStatus << " total PRE mAs:" << totalPremAs << " toal Pulse mAs:" << totalPulsemAs;


        if(totalPulsemAs) INTERFACE->EventXrayCompleted(0,Interface::_EXPOSURE_ABORT, totalPremAs, totalPulsemAs, error_code);
        else INTERFACE->EventXrayCompleted(0,Interface::_EXPOSURE_PARTIAL,totalPremAs, totalPulsemAs, error_code);

        pulseExposureData.valid = false;
        preExposureData.valid = false;
        changeStatus(SMS_IDLE,0,SMS_IDLE,0);
        return;
    }

    current_status = R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.GeneratorStatus;

    switch(subStatus){
    case 0:
        qDebug() << "EXPOSURE 2D START SEQUENCE";
        clearPostExposureList();        
        exposureError = false;
        error_code = 0;        
        break;

    case 1:
        // Validate the exposure data
        if(!pulseExposureData.valid){
            exposureError = true;
            error_code = Interface::_EXP_ERR_PULSE_VALIDATION;
            qDebug() << "_EXP_ERR_PULSE_VALIDATION";
            QTimer::singleShot(0, this, SLOT(handleCurrentStatus()));
            return;
        }

        // Load Data Bank
        wait_command_processed = true;
        COMMUNICATION->set2DDataBank(R2CP::DB_Pulse,pulseExposureData.focus,pulseExposureData.kV,pulseExposureData.mAs, 5000);
        break;

    case 2:       

        // Procedure activation
        wait_command_processed = true;
        COMMUNICATION->activate2DProcedurePulse(exposureOptions.use_detector, exposureOptions.use_grid);
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

        postExposure = STATUS->getPostExposureList();
        totalPulsemAs = 0;
        totalPremAs = 0;
        for (int i=0; i< postExposure.size(); i++) totalPulsemAs += postExposure[i].mAs;
        qDebug() << "EXPOSURE 2D MANUAL TERMINATED. Total PRE mAs:" << totalPremAs << ", Total Pulse mAs:" << totalPulsemAs;

        INTERFACE->EventXrayCompleted(0,Interface::_EXPOSURE_COMPLETED,totalPremAs, totalPulsemAs, Interface::_EXP_ERR_NONE);
        pulseExposureData.valid = false;
        preExposureData.valid = false;
        changeStatus(SMS_IDLE,0,SMS_IDLE,0);
        return;
    }

    subStatus++;
    QTimer::singleShot(10, this, SLOT(handleCurrentStatus()));

}


