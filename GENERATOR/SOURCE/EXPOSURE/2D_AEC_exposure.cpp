#include "application.h"
#include <QTimer>

/**
 * @brief This function handles the 2D+AEC exposure sequence.
 *
 * # 2D+AEC Exposure Sequence description
 *
 * The 2D+AEC exposure sequence is a dual pulse exposure:
 * - The Pre pulse is first generated to evaluate the Breast density;
 * - The Pulse then is generated based on the breast density calculated.
 *
 * The Exposure Pre-Pulse data shall be present and validated in the exposureManager::preExposureData:
 * - The exposure data is validated when the data is set with the exposureManager::setPreData();
 * - The data is invalidated as soon as the exposure termines;
 *
 * The Procedure loads the exposure data into the Generator then \n
 * sends the Server::EventSetXrayEna() event to the Gantry, requesting the XRAY-ENA activation;
 *
 * The Procedure polls the Generator status in order to control the exposure execution;
 *
 * When the Pre Pulse completes, the Workflow request the Gantry to obtain the Exposure Pulse Data.
 *
 * When the Pulse Data is received then the Workflow proceeds activating the Generator \n
 * to execute the Exposure with the Pulse data.
 *
 * During the Exposure, the generator sends post message blocks with \n
 * the data of the last pulse executed.
 *  > NOTE: a single Exposure can be splitted into more sub pulses due to the Grid interruption.
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
 * No Options are available with this exposure workflow.
 *
 * # Exposure Terminated Event
 *
 * When the exposure termines, whether with success or error,
 * the procedure:
 *
 * - Requests the Gantry to clear the XRAY-ENA signal. See Server::EventSetXrayEna().
 * - Computes the total mAs executed for the pre-pulse and for the pulse;
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
void exposureManager::handle_2D_AEC(void){


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

        if(totalPulsemAs+totalPremAs) INTERFACE->EventXrayCompleted(0,_EXPOSURE_ABORT, totalPremAs, totalPulsemAs, error_code);
        else INTERFACE->EventXrayCompleted(0,_EXPOSURE_PARTIAL,totalPremAs, totalPulsemAs, error_code);

        pulseExposureData.valid = false;
        preExposureData.valid = false;
        STATUS->changeWorkflow(workflowManager::SMS_IDLE);
        return;
    }

    current_status = R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.GeneratorStatus;


    switch(STATUS->subStatus){
    case 0:
        qDebug() << "EXPOSURE 2D+AEC START SEQUENCE";
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

        // Load Data Bank
        STATUS->wait_command_processed = true;
        COMMUNICATION->set2DDataBank(R2CP::DB_Pre,preExposureData.focus,preExposureData.kV,preExposureData.mAs, 5000);
        break;

    case 2:       

        // Procedure activation
        STATUS->wait_command_processed = true;
        COMMUNICATION->activate2DAecProcedurePre();
        break;

    case 3:

        // Get Status
        STATUS->wait_command_processed = true;
        COMMUNICATION->getGeneratorStatusV6();
        break;

    case 4:        

        // Clear the disable Rx message
        STATUS->wait_command_processed = true;
        COMMUNICATION->setDisableRxSystemMessage(false);
        break;

    case 5:       

        // Clear all the System Messages
        if(R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.SystemMessage.Fields.Active == R2CP::Stat_SystemMessageActive_Active){
            STATUS->changeSubWorkflow(workflowManager::SMS_CLEAR_SYS_MESSAGES,STATUS->subStatus+1);
            return;
        }
        break;

    case 6: // Test Not cleared messages

        if(R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.SystemMessage.Fields.Active == R2CP::Stat_SystemMessageActive_Active){
            exposureError = true;
            error_code = _EXP_ERR_GENERATOR_ERRORS;
            qDebug() << "_EXP_ERR_GENERATOR_ERRORS";
            STATUS->scheduleWorkflow(0);
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

      case 8:
        if(!R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.ExposureSwitches.Fields.ExpsignalStatus){
            exposureError = true;
            error_code = _EXP_ERR_XRAY_ENA_EARLY_RELEASED;
            qDebug() << "_EXP_ERR_XRAY_ENA_EARLY_RELEASED";
            STATUS->scheduleWorkflow(0);
            return;
        }

        // Wait for Standby
        switch(current_status){

            case R2CP::Stat_Standby:
                qDebug() << "Pre pulse completed: wait for the AEC data";
                INTERFACE->EventGetPulseData(0);
                abortAecTimeoutRequest = false;
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

            case R2CP::Stat_Preparation:
            case R2CP::Stat_Ready:
            case R2CP::Stat_ExpInProgress:
                STATUS->scheduleWorkflow(10);
                return;

            default:
                STATUS->scheduleWorkflow(10);
                return;
        }

        break;

    case 9:
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

        if(!pulseExposureData.valid){
            STATUS->scheduleWorkflow(10);
            return;
        }

        qDebug() << "Pulse received from the AEC";

        if(R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.SystemMessage.Fields.Active == R2CP::Stat_SystemMessageActive_Active){
            exposureError = true;
            error_code = _EXP_ERR_GENERATOR_ERRORS;
            qDebug() << "_EXP_ERR_GENERATOR_ERRORS";
            STATUS->scheduleWorkflow(0);
            return;
        }

        // Load Data Bank
        STATUS->wait_command_processed = true;
        COMMUNICATION->set2DDataBank(R2CP::DB_Pulse,pulseExposureData.focus,pulseExposureData.kV,pulseExposureData.mAs, 5000);
        break;

    case 10:

        // Procedure activation
        STATUS->wait_command_processed = true;
        COMMUNICATION->activate2DAecProcedurePulse();
        break;

    case 11:

        // Get Status
        STATUS->wait_command_processed = true;
        COMMUNICATION->getGeneratorStatusV6();
        break;


    case 12:

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
        qDebug() << "EXPOSURE 2D+AEC TERMINATED. Total PRE mAs:" << totalPremAs << ", Total Pulse mAs:" << totalPulsemAs;


        INTERFACE->EventXrayCompleted(0,_EXPOSURE_COMPLETED,totalPremAs, totalPulsemAs, _EXP_ERR_NONE);
        pulseExposureData.valid = false;
        preExposureData.valid = false;
        STATUS->changeWorkflow(workflowManager::SMS_IDLE);
        return;
    }

    STATUS->subStatus++;
    STATUS->scheduleWorkflow(10);

}
