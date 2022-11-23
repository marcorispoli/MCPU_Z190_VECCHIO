#include "application.h"
#include <QTimer>

void statusManager::handle_3D_AEC(void){
    QList<Interface::tPostExposureData> postExposure;
    float totalPremAs;
    float totalPulsemAs;

    static uchar current_status;
    static bool exposureError = false;
    static unsigned char error_code;
    float pulse_mAs;

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
        tomoConfig.valid = false;
        changeStatus(SMS_IDLE,0,SMS_IDLE,0);
        return;
    }

    current_status = R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.GeneratorStatus;

    switch(subStatus){
    case 0:
        qDebug() << "EXPOSURE 3D AEC START SEQUENCE";
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

        // Verify if theprocedure needs to be rebuilded
        if((!procedureCreated) || (tomoConfig.changed)){
            changeStatus(SMS_SETUP_GENERATOR,0,internalState,subStatus+1);
            return;
        }
        break;

    case 2:
        if(!procedureCreated){
            exposureError = true;
            error_code = Interface::_EXP_ERR_PROCEDURE_SETUP;
            qDebug() << "_EXP_ERR_PROCEDURE_SETUP";
            QTimer::singleShot(0, this, SLOT(handleCurrentStatus()));
            return;
        }

        // Load Data Bank
        wait_command_processed = true;
        COMMUNICATION->set2DDataBank(R2CP::DB_Pre,preExposureData.focus,preExposureData.kV,preExposureData.mAs, 5000);
        break;

    case 3:

        // Procedure activation
        wait_command_processed = true;
        COMMUNICATION->activate3DAecProcedurePre();
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
            qDebug() << "_EXP_ERR_GENERATOR_ERRORS";
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

    case 10:

        // Wait for Standby
        switch(current_status){


            case R2CP::Stat_Standby:
                if(!R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.ExposureSwitches.Fields.ExpsignalStatus){
                    exposureError = true;
                    error_code = Interface::_EXP_ERR_XRAY_ENA_EARLY_RELEASED;
                    qDebug() << "_EXP_ERR_XRAY_ENA_EARLY_RELEASED";
                    QTimer::singleShot(0, this, SLOT(handleCurrentStatus()));
                    return;
                }

                qDebug() << "Pre pulse completed: wait for the AEC data";
                abortTimeoutRequest = false;
                QTimer::singleShot(preExposureData.tmo, this, SLOT(timeoutAec()));
                INTERFACE->EventGetPulseData(0);
                // INTERFACE->EventSetXrayEna(0,false);
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

        if(R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.SystemMessage.Fields.Active == R2CP::Stat_SystemMessageActive_Active){
            exposureError = true;
            error_code = Interface::_EXP_ERR_GENERATOR_ERRORS;
            qDebug() << "_EXP_ERR_GENERATOR_ERRORS";
            QTimer::singleShot(0, this, SLOT(handleCurrentStatus()));
            return;
        }

        if(!pulseExposureData.valid){
            QTimer::singleShot(10, this, SLOT(handleCurrentStatus()));
            return;
        }

        qDebug() << "AEC data received";


        // In order to find the Anodic Current and the eosure time:
        // - 1: set the 2point mode with kV and the integer mAs per pulse;
        // - Using the mS of the renard scale determined by the Generator, calc the new mA to be used for 3 point tech.
        pulse_mAs = pulseExposureData.mAs / tomoConfig.samples;
        if((float)((ushort) pulse_mAs) != pulse_mAs) pulse_mAs = (float) ((ushort) pulse_mAs + 1);
        else pulse_mAs = (float) ((ushort) pulse_mAs);

        wait_command_processed = true;
        COMMUNICATION->set2DDataBank(R2CP::DB_Pulse,pulseExposureData.focus,pulseExposureData.kV,pulse_mAs,5000);
        subStatus = 20;
        break;

    case 21:

        pulseExposureData.mS = R2CP::CaDataDicGen::GetInstance()->radInterface.DbDefinitions[R2CP::DB_Pulse].ms100.value / 100;
        if(pulseExposureData.mS > tomoConfig.EW){
            exposureError = true;
            error_code = Interface::_EXP_ERR_MAS_OUT_OF_RANGE;
            qDebug() << "_EXP_ERR_MAS_OUT_OF_RANGE";
            QTimer::singleShot(0, this, SLOT(handleCurrentStatus()));
            return;
        }

        pulseExposureData.mA = (pulseExposureData.mAs * 1000) / (tomoConfig.samples * pulseExposureData.mS);
        qDebug()<<"mA:" << pulseExposureData.mA <<" mS:" << pulseExposureData.mS;

        wait_command_processed = true;
        COMMUNICATION->set3DDataBank(R2CP::DB_Pulse,pulseExposureData.focus,pulseExposureData.kV,pulseExposureData.mA, pulseExposureData.mS,tomoConfig.EW);
        subStatus = 11;
        break;

        //////////////////////////////////////////////////////////
    case 12:

        // Procedure activation
        wait_command_processed = true;
        COMMUNICATION->activate3DAecProcedurePulse();
        break;

    case 13:
        if(!R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.ExposureSwitches.Fields.ExpsignalStatus){
            exposureError = true;
            error_code = Interface::_EXP_ERR_XRAY_ENA_EARLY_RELEASED;
            qDebug() << " XRAY-ENA EARLY RELEASED";
            QTimer::singleShot(0, this, SLOT(handleCurrentStatus()));
            return;
        }

        if(R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.SystemMessage.Fields.Active == R2CP::Stat_SystemMessageActive_Active){
            exposureError = true;
            error_code = Interface::_EXP_ERR_GENERATOR_ERRORS;
            qDebug() << "_EXP_ERR_GENERATOR_ERRORS";
            QTimer::singleShot(0, this, SLOT(handleCurrentStatus()));
            return;
        }


        // Wait for Standby
        switch(current_status){
            case R2CP::Stat_WaitFootRelease:
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

            case R2CP::Stat_Standby:
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
        qDebug() << "EXPOSURE 3D+AEC TERMINATED. Total PRE mAs:" << totalPremAs << ", Total Pulse mAs:" << totalPulsemAs;
        INTERFACE->EventXrayCompleted(0,Interface::_EXPOSURE_COMPLETED,totalPremAs, totalPulsemAs, Interface::_EXP_ERR_NONE);
        pulseExposureData.valid = false;
        preExposureData.valid = false;
        tomoConfig.valid = false;
        changeStatus(SMS_IDLE,0,SMS_IDLE,0);
        return;
    }

    subStatus++;
    QTimer::singleShot(10, this, SLOT(handleCurrentStatus()));

}

