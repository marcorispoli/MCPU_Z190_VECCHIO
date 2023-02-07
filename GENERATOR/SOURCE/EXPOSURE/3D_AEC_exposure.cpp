#include "application.h"
#include <QTimer>


/**
 * @brief This function handles the 3D+AEC Manual exposure sequence.
 *
 * # 3D+AEC Exposure Sequence description
 *
 * The 3D+AEC exposure sequence is a sequence with a pre pulse to evaluate the breast density\n
 * followed by the variable number of pulses;
 *
 * The Exposure Pre-Pulse data shall be present and validated in the exposureManager::preExposureData:
 * - The exposure data is validated when the data is set with the exposureManager::setPreData();
 * - The data is invalidated as soon as the exposure termines;
 *
 * The number of pulses and other Tomo parameters shall be set in the exposureManager::tomoConfig before to start the workflow;
 *
 * The Procedure loads the pre-pulse exposure data into the Generator then \n
 * sends the Server::EventSetXrayEna() event to the Gantry, requesting the XRAY-ENA activation;
 *
 * When the Generator completes the pre pulse execution, the workflow \n
 * request to the Gantry to have the Pulse Exposure data.
 *
 * The Procedure calculate with the 3Point Technique the correct mA an mS to be used
 * to accomplish at the requested mAs per pulse.
 *
 * The Procedure polls the Generator status in order to control the exposure execution;
 *
 * During the Exposure, the generator sends post message blocks with \n
 * the data of the last pulse executed.
 *
 * The Procedure collects all the post data blocks so that at the exposure completion, they\n
 * will be available for the Gantry (see the Server::GetPostExposure() command).
 *  > NOTE: the list of pulses remains available until a next exposure is requested.
 *
 *
 * In case of an error, the exposure is immediatelly terminated.
 *
 * # Exposure Options
 *
 *  No options are available with this Workflow;
 *
 *
 * # Exposure Terminated Event
 *
 * When the exposure termines, whether with success or error,
 * the procedure:
 *
 * - Requests the Gantry to clear the XRAY-ENA signal. See Server::EventSetXrayEna().
 * - Computes the total mAs executed for the pulse;
 * - Sends to Gantry the Server::EventXrayCompleted() event.
 *
 * # Exposure Errors
 *
 * There are several causes because an Exposure early termines in error condition:
 *
 * - The XRAY-ENA signal drops early;
 * - The Gantry request a software abort  (see Server::AbortExposure() );
 * - The AEC pulse data are not received within a timeout after the Pre Pulse completion;
 * - The Generator internal Status changes to a not expected state;
 * - The Generator emits a System Error Message;
 * - The Procedure starts without the exposure data validation;
 *
 * The error code is reported in the Server::EventXrayCompleted() event.\n
 * See the exposureManager::tExposureErrors enumeration.
 *
 *
 *
 */
void exposureManager::handle_3D_AEC(void){
    float pulse_mAs;
    float mS;

    if((STATUS->subStatus!=0) && ((STATUS->abortRxRequest) || (!STATUS->command_process_state) || (exposureError))){

        totalPulsemAs = 0;
        totalPremAs = 0;
        for (int i=0; i< postExposure.size(); i++){
            if(postExposure[i].DB == R2CP::DB_Pre) totalPremAs += postExposure[i].mAs;
            else if(postExposure[i].DB == R2CP::DB_Pulse) totalPulsemAs += postExposure[i].mAs;
        }

        if(STATUS->abortRxRequest) qDebug() << "Abort Rx Command Executed: total PRE mAs:" << totalPremAs << " toal Pulse mAs:" << totalPulsemAs;
        else if(!STATUS->command_process_state) qDebug() << "Cp Error on Substatus = " << STATUS->subStatus << " total PRE mAs:" << totalPremAs << " toal Pulse mAs:" << totalPulsemAs;
        else qDebug() << "Exposure Error on Substatus = " << STATUS->subStatus << " total PRE mAs:" << totalPremAs << " toal Pulse mAs:" << totalPulsemAs;

        if(totalPulsemAs+totalPremAs) INTERFACE->EventXrayCompleted(_EXPOSURE_ABORT, totalPremAs, totalPulsemAs, error_code);
        else INTERFACE->EventXrayCompleted(_EXPOSURE_PARTIAL,totalPremAs, totalPulsemAs, error_code);


        pulseExposureData.valid = false;
        preExposureData.valid = false;
        tomoConfig.valid = false;
        STATUS->changeWorkflow(workflowManager::SMS_IDLE);
        return;
    }

    current_status = R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.GeneratorStatus;

    switch(STATUS->subStatus){
    case 0:
        qDebug() << "EXPOSURE 3D AEC START SEQUENCE";
        postExposure.clear();
        pulseExposureData.valid = false;
        exposureError = false;
        error_code = 0;
        break;

    case 1:
        // Validate the exposure data
        if(!preExposureData.valid){
            exposureError = true;
            error_code = _EXP_ERR_PRE_VALIDATION;
            qDebug() << "_EXP_ERR_PRE_VALIDATION";
            STATUS->scheduleWorkflow(0);
            return;
        }

        // Verify if theprocedure needs to be rebuilded
        if((!STATUS->procedureCreated) || (tomoConfig.changed)){
            STATUS->changeSubWorkflow(workflowManager::SMS_SETUP_GENERATOR,STATUS->subStatus+1);
            return;
        }
        break;

    case 2:
        if(!STATUS->procedureCreated){
            exposureError = true;
            error_code = _EXP_ERR_PROCEDURE_SETUP;
            qDebug() << "_EXP_ERR_PROCEDURE_SETUP";
            STATUS->scheduleWorkflow(0);
            return;
        }

        // Load Data Bank
        STATUS->wait_command_processed = true;
        COMMUNICATION->set2DDataBank(R2CP::DB_Pre,preExposureData.focus,preExposureData.kV,preExposureData.mAs, 5000);
        break;

    case 3:

        // Procedure activation
        STATUS->wait_command_processed = true;
        COMMUNICATION->activate3DAecProcedurePre();
        break;

    case 4:

        // Get Status
        STATUS->wait_command_processed = true;
        COMMUNICATION->getGeneratorStatusV6();
        break;

    case 5:

        // Clear the disable Rx message
        STATUS->wait_command_processed = true;
        COMMUNICATION->setDisableRxSystemMessage(false);
        break;

    case 6:

        // Clear all the System Messages
        if(R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.SystemMessage.Fields.Active == R2CP::Stat_SystemMessageActive_Active){
            STATUS->changeSubWorkflow(workflowManager::SMS_CLEAR_SYS_MESSAGES,STATUS->subStatus+1);
            return;
        }
        break;

    case 7: // Test Not cleared messages

        if(R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.SystemMessage.Fields.Active == R2CP::Stat_SystemMessageActive_Active){
            exposureError = true;
            error_code = _EXP_ERR_GENERATOR_ERRORS;
            qDebug() << "_EXP_ERR_GENERATOR_ERRORS";
            STATUS->scheduleWorkflow(0);
            return;
        }

        qDebug() << "XRAY-ENA ACTIVATION..";

        INTERFACE->EventSetXrayEna(true);
        break;

    case 8:

        // Wait for the PRX and XRAY ENA
        if(R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.ExposureSwitches.Fields.ExpsignalStatus) break;
        STATUS->scheduleWorkflow(0);
        return;

    case 9:
        if(!R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.ExposureSwitches.Fields.ExpsignalStatus){
            exposureError = true;
            error_code = _EXP_ERR_XRAY_ENA_EARLY_RELEASED;
            qDebug() << "_EXP_ERR_XRAY_ENA_EARLY_RELEASED";
            STATUS->scheduleWorkflow(0);
            return;
        }

        // Wait for the Exposure in progress
        switch(current_status){
            case R2CP::Stat_Standby:

                if(R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.SystemMessage.Fields.Active == R2CP::Stat_SystemMessageActive_Active){
                    exposureError = true;
                    error_code = _EXP_ERR_GENERATOR_ERRORS;
                    qDebug() << "_EXP_ERR_GENERATOR_ERRORS";
                    STATUS->scheduleWorkflow(0);
                    return;
                }

                STATUS->scheduleWorkflow(10);
                return;

            case R2CP::Stat_Error:
                exposureError = true;
                error_code = _EXP_ERR_GENERATOR_ERRORS;
                qDebug() << "_EXP_ERR_GENERATOR_ERRORS";
                STATUS->scheduleWorkflow(0);
                return;

            case R2CP::Stat_WaitFootRelease:
            case R2CP::Stat_GoigToShutdown:
            case R2CP::Stat_Service:
            case R2CP::Stat_Initialization:
                exposureError = true;
                error_code = _EXP_ERR_GENERATOR_STATUS;
                qDebug() << "_EXP_ERR_GENERATOR_STATUS";
                STATUS->scheduleWorkflow(0);
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
                    error_code = _EXP_ERR_XRAY_ENA_EARLY_RELEASED;
                    qDebug() << "_EXP_ERR_XRAY_ENA_EARLY_RELEASED";
                    STATUS->scheduleWorkflow(0);
                    return;
                }

                qDebug() << "Pre pulse completed: wait for the AEC data";
                abortAecTimeoutRequest = false;
                INTERFACE->EventGetPulseData();
                STATUS->scheduleAecTmo(preExposureData.tmo);
            break;


            case R2CP::Stat_Error:
                exposureError = true;
                error_code = _EXP_ERR_GENERATOR_ERRORS;
                qDebug() << "_EXP_ERR_GENERATOR_ERRORS";
                STATUS->scheduleWorkflow(0);
                return;

            case R2CP::Stat_WaitFootRelease:
            case R2CP::Stat_GoigToShutdown:
            case R2CP::Stat_Service:
            case R2CP::Stat_Initialization:
                exposureError = true;
                error_code = _EXP_ERR_GENERATOR_STATUS;
                qDebug() << "_EXP_ERR_GENERATOR_STATUS";
                STATUS->scheduleWorkflow(0);
                return;

            case R2CP::Stat_ExpInProgress:               
            case R2CP::Stat_Ready:
            case R2CP::Stat_Preparation:         
                STATUS->scheduleWorkflow(10);
                return;

            default:
                STATUS->scheduleWorkflow(10);
                return;
        }

        break;

    case 11:
        if(abortAecTimeoutRequest){
            exposureError = true;
            error_code = _EXP_ERR_AEC_TMO;
            qDebug() << "_EXP_ERR_AEC_TMO";
            STATUS->scheduleWorkflow(0);
            return;
        }

        if(!R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.ExposureSwitches.Fields.ExpsignalStatus){
            exposureError = true;
            error_code = _EXP_ERR_XRAY_ENA_EARLY_RELEASED;
            qDebug() << "_EXP_ERR_XRAY_ENA_EARLY_RELEASED";
            STATUS->scheduleWorkflow(0);
            return;
        }

        if(R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.SystemMessage.Fields.Active == R2CP::Stat_SystemMessageActive_Active){
            exposureError = true;
            error_code = _EXP_ERR_GENERATOR_ERRORS;
            qDebug() << "_EXP_ERR_GENERATOR_ERRORS";
            STATUS->scheduleWorkflow(0);
            return;
        }

        if(!pulseExposureData.valid){
            STATUS->scheduleWorkflow(10);
            return;
        }

        qDebug() << "AEC data received";


        // In order to find the Anodic Current and the eosure time:
        // - 1: set the 2point mode with kV and the integer mAs per pulse;
        // - Using the mS of the renard scale determined by the Generator, calc the new mA to be used for 3 point tech.
        pulse_mAs = pulseExposureData.mAs / tomoConfig.samples;
        if((float)((ushort) pulse_mAs) != pulse_mAs) pulse_mAs = (float) ((ushort) pulse_mAs + 1);
        else pulse_mAs = (float) ((ushort) pulse_mAs);

        STATUS->wait_command_processed = true;
        COMMUNICATION->set2DDataBank(R2CP::DB_Pulse,pulseExposureData.focus,pulseExposureData.kV,pulse_mAs,5000);
        STATUS->subStatus = 20;
        break;

    case 21:


        // The procedure verifies if the Generator proposed Time should be lower than the minimum.
        // In case the the time lower than the minimum, it is used the next R10 value, but only
        // if this should be lower of the Maximum Integration time EW
        pulseExposureData.mS = R2CP::CaDataDicGen::GetInstance()->radInterface.DbDefinitions[R2CP::DB_Pulse].ms100.value / 100;
        mS = getR10Time(MIN_TOMO_EXPOSURE_TIME, true);
        if((pulseExposureData.mS < MIN_TOMO_EXPOSURE_TIME) && (mS < tomoConfig.EW) ) pulseExposureData.mS = mS;

        if(pulseExposureData.mS > tomoConfig.EW){
            exposureError = true;
            error_code = _EXP_ERR_MAS_OUT_OF_RANGE;
            qDebug() << "_EXP_ERR_MAS_OUT_OF_RANGE";
            STATUS->scheduleWorkflow(0);
            return;
        }



        pulseExposureData.mA = (pulseExposureData.mAs * 1000) / (tomoConfig.samples * pulseExposureData.mS);
        qDebug()<<"mA:" << pulseExposureData.mA <<" mS:" << pulseExposureData.mS;

        STATUS->wait_command_processed = true;
        COMMUNICATION->set3DDataBank(R2CP::DB_Pulse,pulseExposureData.focus,pulseExposureData.kV,pulseExposureData.mA, pulseExposureData.mS,tomoConfig.EW);
        STATUS->subStatus = 11;
        break;

        //////////////////////////////////////////////////////////
    case 12:

        // Procedure activation
        STATUS->wait_command_processed = true;
        COMMUNICATION->activate3DAecProcedurePulse();
        break;

    case 13:
        if(!R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.ExposureSwitches.Fields.ExpsignalStatus){
            exposureError = true;
            error_code = _EXP_ERR_XRAY_ENA_EARLY_RELEASED;
            qDebug() << " XRAY-ENA EARLY RELEASED";
            STATUS->scheduleWorkflow(0);
            return;
        }

        if(R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.SystemMessage.Fields.Active == R2CP::Stat_SystemMessageActive_Active){
            exposureError = true;
            error_code = _EXP_ERR_GENERATOR_ERRORS;
            qDebug() << "_EXP_ERR_GENERATOR_ERRORS";
            STATUS->scheduleWorkflow(0);
            return;
        }


        // Wait for Standby
        switch(current_status){
            case R2CP::Stat_WaitFootRelease:
                 INTERFACE->EventSetXrayEna(false);
            break;


            case R2CP::Stat_Error:
                exposureError = true;
                error_code = _EXP_ERR_GENERATOR_ERRORS;
                qDebug() << "_EXP_ERR_GENERATOR_ERRORS";
                STATUS->scheduleWorkflow(0);
                return;

            case R2CP::Stat_GoigToShutdown:
            case R2CP::Stat_Service:
            case R2CP::Stat_Initialization:
                exposureError = true;
                error_code = _EXP_ERR_GENERATOR_STATUS;
                qDebug() << "_EXP_ERR_GENERATOR_STATUS";
                STATUS->scheduleWorkflow(0);
                return;

            case R2CP::Stat_Standby:
            case R2CP::Stat_ExpInProgress:
            case R2CP::Stat_Ready:
            case R2CP::Stat_Preparation:
                STATUS->scheduleWorkflow(10);
                return;

            default:
                STATUS->scheduleWorkflow(10);
                return;
        }

        break;



    case 14:
        // Wait for Standby
        switch(current_status){

            case R2CP::Stat_WaitFootRelease:
                STATUS->scheduleWorkflow(10);
                return;

            case R2CP::Stat_Standby:
                break;
        }

        break;

    default:

        totalPulsemAs = 0;
        totalPremAs = 0;
        for (int i=0; i< postExposure.size(); i++){
            if(postExposure[i].DB == R2CP::DB_Pre) totalPremAs += postExposure[i].mAs;
            else if(postExposure[i].DB == R2CP::DB_Pulse) totalPulsemAs += postExposure[i].mAs;
        }
        qDebug() << "EXPOSURE 3D+AEC TERMINATED. Total PRE mAs:" << totalPremAs << ", Total Pulse mAs:" << totalPulsemAs;
        INTERFACE->EventXrayCompleted(_EXPOSURE_COMPLETED,totalPremAs, totalPulsemAs, _EXP_ERR_NONE);
        pulseExposureData.valid = false;
        preExposureData.valid = false;
        tomoConfig.valid = false;
        STATUS->changeWorkflow(workflowManager::SMS_IDLE);
        return;
    }

    STATUS->subStatus++;
    STATUS->scheduleWorkflow(10);

}

