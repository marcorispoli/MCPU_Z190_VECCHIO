#include "application.h"
#include <QTimer>

/**
 * @brief This function handles the 2D Manual exposure sequence.
 *
 * # 2D Exposure Sequence description
 *
 * The 2D exposure sequence is a single pulse exposure.
 *
 * The Exposure Pulse data shall be present and validated in the exposureManager::pulseExposureData:
 * - The exposure data is validated when the data is set with the exposureManager::setPulseData();
 * - The data is invalidated as soon as the exposure termines;
 *
 * The Procedure loads the exposure data into the Generator then \n
 * sends the Server::EventSetXrayEna() event to the Gantry, requesting the XRAY-ENA activation;
 *
 * The Procedure polls the Generator status in order to control the exposure execution;
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
 * In the 2D manual  exposure procedure the Application may set exposure options (see the exposureManager::setExposureOptions() ):
 * - Enable/Disable the Detector synchronization signals;
 * - Enable/Disable the Grid synchronization signals;
 *
 *  > NOTE: In case the Detector Synch option should be enabled, mandatory the Grid Synch shall be enabled as well.
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
void exposureManager::handle_2D_MANUAL(void){


    if((STATUS->subStatus!=0) && ((STATUS->abortRxRequest) || (!STATUS->command_process_state) || (exposureError))){

        totalPulsemAs = 0;
        totalPremAs = 0;
        for (int i=0; i< postExposure.size(); i++) totalPulsemAs += postExposure[i].mAs;

        if(STATUS->abortRxRequest) qDebug() << "Abort Rx Command Executed: total PRE mAs:" << totalPremAs << " toal Pulse mAs:" << totalPulsemAs;
        else if(!STATUS->command_process_state) qDebug() << "Cp Error on Substatus = " << STATUS->subStatus << " total PRE mAs:" << totalPremAs << " toal Pulse mAs:" << totalPulsemAs;
        else qDebug() << "Exposure Error on Substatus = " << STATUS->subStatus << " total PRE mAs:" << totalPremAs << " toal Pulse mAs:" << totalPulsemAs;


        if(totalPulsemAs) INTERFACE->EventXrayCompleted(0,_EXPOSURE_ABORT, totalPremAs, totalPulsemAs, error_code);
        else INTERFACE->EventXrayCompleted(0,_EXPOSURE_PARTIAL,totalPremAs, totalPulsemAs, error_code);

        pulseExposureData.valid = false;
        preExposureData.valid = false;
        STATUS->changeWorkflow(workflowManager::SMS_IDLE);
        return;
    }

    current_status = R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.GeneratorStatus;

    switch(STATUS->subStatus){
    case 0:
        qDebug() << "EXPOSURE 2D START SEQUENCE";
        postExposure.clear();
        exposureError = false;
        error_code = 0;        
        break;

    case 1:
        // Validate the exposure data
        if(!pulseExposureData.valid){
            exposureError = true;
            error_code = _EXP_ERR_PULSE_VALIDATION;
            qDebug() << "_EXP_ERR_PULSE_VALIDATION";
            STATUS->scheduleWorkflow(0);
            return;
        }

        // Load Data Bank
        STATUS->wait_command_processed = true;
        COMMUNICATION->set2DDataBank(R2CP::DB_Pulse,pulseExposureData.focus,pulseExposureData.kV,pulseExposureData.mAs, 5000);
        break;

    case 2:       

        // Procedure activation
        STATUS->wait_command_processed = true;
        COMMUNICATION->activate2DProcedurePulse(exposureOptions.use_detector, exposureOptions.use_grid);
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

    case 9:
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
        for (int i=0; i< postExposure.size(); i++) totalPulsemAs += postExposure[i].mAs;
        qDebug() << "EXPOSURE 2D MANUAL TERMINATED. Total PRE mAs:" << totalPremAs << ", Total Pulse mAs:" << totalPulsemAs;

        INTERFACE->EventXrayCompleted(0,_EXPOSURE_COMPLETED,totalPremAs, totalPulsemAs, _EXP_ERR_NONE);
        pulseExposureData.valid = false;
        preExposureData.valid = false;
        STATUS->changeWorkflow(workflowManager::SMS_IDLE);
        return;
    }

    STATUS->subStatus++;
    STATUS->scheduleWorkflow(10);

}


