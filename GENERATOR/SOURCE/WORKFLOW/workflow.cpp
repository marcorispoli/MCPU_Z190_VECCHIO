#include "application.h"


/**
 * @brief Constructor of the class
 *
 * The constructor initializes the internal items
 *
 * @param parent QObject pointer
 */
workflowManager::workflowManager( QObject *parent)
{
    // Instance of the Data Base manager with the path and file declared as a constant
    DBSysMessages = new DbManager(Application::SQL_GENERATOR_DATABASE);

    // Socket activation to connect the smart hub
    COMMUNICATION = new Communication();
    COMMUNICATION->start();


    abortRxRequest = false;
    interfaceStatus = workflowManager::_NOT_READY_FOR_EXPOSURE;

    executingWorkflow = SMS_UNDEFINED;
    procedureCreated = false;

    // Initialize with the Smart Hub connection workflow
    changeWorkflow(SMS_SMART_HUB_CONNECTION);
}



/**
 * @brief workflowManager::handleCommandProcessedState
 *
 * This function manage the Command Processed Frame
 * reception from the Generator.
 *
 * The Command Processed Frame is a frame of the R2CP
 * protocol sent by the generator when a given protocol command
 * (Set or Get) has been completelly handled.
 *
 * Not all the Protocol commands require to be synchronized
 * with the Command Processed  frame.
 *
 * In order to activate the mechanism, the Application \n
 * shall to set the workflowManager::wait_command_processed = true\n
 * before to send the command with the COMMUNICATION interface.
 *
 */
void workflowManager::handleCommandProcessedState(void){
    QString errstr;

    // If the R2CP is waiting the command processed code,
    // the application waits for a maximum of 1000 ms before to rise an error condition
    if(!COMMUNICATION->isCommandProcessed() ){
        cp_timeout--;

        if(!cp_timeout){
            cp_timeout = 100;
            wait_command_processed = false;
            command_process_state = false;
            qDebug() << "COMMAND PROCESSED ERROR: TIMEOUT";
            command_process_code = 0;
            scheduleWorkflow(0);
            return;

        }

        scheduleWorkflow(10);
        return;
    }

    // In case the command processed code has been received
    // It is evaluated the correctness
    cp_timeout = 100;
    wait_command_processed = false;

    command_process_code = COMMUNICATION->getCommandProcessed();
    if(command_process_code){

        // An error Command Processed code has been received
        command_process_state = false;

        //qDebug() << "COMMAND PROCESSED ERROR: " << COMMUNICATION->getCommandProcessedString();
        errstr = COMMUNICATION->getCommandProcessedString();

        // The error code is sent to the External Client
        if(errstr != "") INTERFACE->EventMessage(0,errstr);
    } else command_process_state = true;


    scheduleWorkflow(10);
    return;
}

/**
 * @brief This is the main scheduled handler of the worflows routines
 *
 * This Handler is called by the scheduler following the use of the \n
 * 'scheduleWorkflow()' function.
 *
 * The Handler:
 * - Handle the command processed mechanism
 * - Select the current running workflow and call the related workflow's routine;
 *
 * The Handler applies priorities in selecting the Workflow with this order:
 * - If the Smart Hub disconnects the current workflow is the SMS_SMART_HUB_CONNECTION;
 * - If the generator disconnects, then the current selected workflow will be:SMS_GENERATOR_CONNECTION;
 * - The rest of the Workflows are selected with the same priority.
 *
 *
 */
void workflowManager::handleCurrentStatus(void){

    // When a command processed return code is requested, the state machine is suspendend
    if(wait_command_processed){
        handleCommandProcessedState();
        return;
    }


    if(executingWorkflow == SMS_SMART_HUB_CONNECTION){
        interfaceStatus = workflowManager::_NOT_READY_FOR_EXPOSURE;
        handle_SH_CONNECTION();
        return;
    }

    if(executingWorkflow == SMS_GENERATOR_CONNECTION){
        interfaceStatus = workflowManager::_NOT_READY_FOR_EXPOSURE;
        handle_GENERATOR_CONNECTION();
        return;
    }

    if(!COMMUNICATION->isSHConnected()){
        changeWorkflow(SMS_SMART_HUB_CONNECTION);
        return;
    }

    if(!COMMUNICATION->isGenConnected()){
        changeWorkflow(SMS_GENERATOR_CONNECTION);
        return;
    }

    if(executingWorkflow == SMS_ERROR){
        interfaceStatus = workflowManager::_STATUS_ERROR;
        handle_SMS_ERROR();
        return;
    }



    switch(executingWorkflow){

    case SMS_CLEAR_SYS_MESSAGES:        handle_CLEAR_ALL_SMS_MESSAGES(); break;
    case SMS_IDLE:                      handle_SMS_IDLE(); break;
    case SMS_SETUP_GENERATOR:           handle_SETUP_GENERATOR();break;
    case SMS_EXECUTING_2D_MANUAL:       EXPOSURE->handle_2D_MANUAL(); break;
    case SMS_EXECUTING_2D_AEC:          EXPOSURE->handle_2D_AEC(); break;
    case SMS_EXECUTING_3D_MANUAL:       EXPOSURE->handle_3D_MANUAL(); break;
    case SMS_EXECUTING_3D_AEC:          EXPOSURE->handle_3D_AEC(); break;

    }

}

/// This is the Timeout callback to handle the AEC timeout event
void workflowManager::timeoutAec(void){
    EXPOSURE->abortAecTimeoutRequest = true;
}

/**
 * @brief This is the Workflow routine handling the connection with the Smart Hub
 *
 * The Workflow loops every 100ms on the COMMUNICATION Socket with the Smart Hub
 *
 * When the connection is established, the workflow changes in SMS_GENERATOR_CONNECTION.
 *
 */
void workflowManager::handle_SH_CONNECTION(void){

    switch(subStatus){
     case 0:
        qDebug() << "SH CONNECTION STATUS";
        subStatus++;
        break;
     case 1:
        if(COMMUNICATION->isSHConnected()){
            changeWorkflow(SMS_GENERATOR_CONNECTION);
            return;
        }
        subStatus++;
        break;
    case 2:
        COMMUNICATION->setSHConnection();
        subStatus = 1;
        break;
    }

    scheduleWorkflow(100);
}

/**
 * @brief This is the Workflow routine handling the connection with the Generator
 *
 * When the connection has been established,
 * the Workflow:
 * - Request the generator protocol version;
 * - If the protocol is not the 6.0.A send the proper protocol command to the generator;
 * - Gets the Generator status and remains in loop until the generator termines the initialization;
 * - Calls the SubWorkflow SMS_SETUP_GENERATOR in order to setup the data structures of the generator;
 * - Calls the SubWorkflow SMS_CLEAR_SYS_MESSAGES in order to clear any active error message into the generator;
 * - Set a DisableRxMessage into the Generator in order to prevent any unwanted exposure sequence;
 * - Exit to the SMS_IDLE workflow.
 */
void workflowManager::handle_GENERATOR_CONNECTION(void){
    static bool init = false;

    // If the generator connection is not present, the workflow remains here
    if(!COMMUNICATION->isGenConnected()){
        if(!init){
            qDebug() << "GENERATOR CONNECTION STATUS";
            init = true;

        }
        subStatus = 0;
        scheduleWorkflow(100);
        return;
    }

    switch(subStatus){
     case 0:                
        init = false;
        wait_command_processed = true;
        COMMUNICATION->getProtocolVersion();subStatus++;
        scheduleWorkflow(300);
        return;

     case 1:

        if(!COMMUNICATION->isVersionUpdated()){
            subStatus = 0;
            scheduleWorkflow(10);
            return;
        }

        if(!COMMUNICATION->isProtocolV6()){
            wait_command_processed = true;
            COMMUNICATION->setProtocolVersion6();
            subStatus=0;
            scheduleWorkflow(100);
            return;
        }


        subStatus++;
        qDebug() << "Wait Generator Initialization ..";
        break;

    case 2:

        wait_command_processed = true;
        COMMUNICATION->getGeneratorStatusV6();subStatus++;
        break;

    case 3:
        if(!command_process_state){
            subStatus = 2;
            scheduleWorkflow(100);
            return;
        }

        if(R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.GeneratorStatus == R2CP::Stat_Initialization ){
            subStatus = 2;
            scheduleWorkflow(100);
            return;
        }
        qDebug() << "Generator in Standby";

        changeSubWorkflow(SMS_SETUP_GENERATOR, subStatus+1);
        return;

    case 4:
        changeSubWorkflow(SMS_CLEAR_SYS_MESSAGES, subStatus+1);
        return;

    case 5:

        // Set the disable Rx message
        wait_command_processed = true;
        COMMUNICATION->setDisableRxSystemMessage(true);subStatus++;
        break;

    case 6:

        // Clear all the current System messages
        changeWorkflow(SMS_IDLE);
        return;

    }

    scheduleWorkflow(10);

}

/**
 * @brief This is the SubWorkflow routine handling the generator data setup
 *
 *
 * The SubWorkflow:
 * - Creates all the Generator Procedures;
 * - Set the Databank used in the procedures
 * - Assignes the Databank to the procedures;
 * - Returns to the calling Workflow
 *
 */
void workflowManager::handle_SETUP_GENERATOR(void){

    if(subStatus != 0){
        if(!command_process_state){
            qDebug() << "Generator Setup Error, subStatus=" << subStatus;
            returSubWorkflow();
            return;
        }
    }

    switch(subStatus){

    case 0:
        qDebug() << "GENERATOR SETUP STATUS";
        interfaceStatus = workflowManager::_NOT_READY_FOR_EXPOSURE;
        procedureCreated = false;
        subStatus++;
        break;

    case 1:
        //wait_command_processed = true;
        //COMMUNICATION->clearAllProcedures();
        subStatus++;
        break;

    case 2:
        wait_command_processed = true;
        COMMUNICATION->setupProcedureV6(R2CP::ProcId_Standard_Test,0);subStatus++;
        break;

    case 3:
       wait_command_processed = true;
       COMMUNICATION->setupProcedureV6(R2CP::ProcId_Standard_Test_with_grid,0);subStatus++;
       break;

    case 4:
       wait_command_processed = true;
       COMMUNICATION->setupProcedureV6(R2CP::ProcId_Standard_Mammography_2D,0);subStatus++;
       break;

    case 5:
       wait_command_processed = true;
       COMMUNICATION->setupProcedureV6(R2CP::ProcId_Aec_Mammography_2D,0);subStatus++;
       break;

    case 6:

       wait_command_processed = true;
       COMMUNICATION->setupProcedureV6(R2CP::ProcId_Aec_Mammography_2D,0);subStatus++;
       break;

    case 7:

       wait_command_processed = true;
       COMMUNICATION->setupProcedureV6(R2CP::ProcId_Standard_Mammography_3D,EXPOSURE->tomoConfig.samples);subStatus++;
       break;

    case 8:

       wait_command_processed = true;
       COMMUNICATION->setupProcedureV6(R2CP::ProcId_Aec_Mammography_3D, EXPOSURE->tomoConfig.samples);subStatus++;
       break;

    case 9:
        wait_command_processed = true;
        COMMUNICATION->set2DDataBank(R2CP::DB_Pre,1,20,10,5000); subStatus++;
        break;

    case 10:
        wait_command_processed = true;
        COMMUNICATION->set2DDataBank(R2CP::DB_Pulse,1,20,10,5000); subStatus++;
        break;

    case 11:
        wait_command_processed = true;
        COMMUNICATION->setGenerator_SkipPulse_Databank(R2CP::DB_SkipPulse,EXPOSURE->tomoConfig.skip); subStatus++;
        break;


    case 12:
        wait_command_processed = true;
        COMMUNICATION->assignDbToProc(R2CP::DB_Pulse,R2CP::ProcId_Standard_Test,1);subStatus++;
        break;

    case 13:
        wait_command_processed = true;
        COMMUNICATION->assignDbToProc(R2CP::DB_Pulse,R2CP::ProcId_Standard_Test_with_grid,1);subStatus++;
        break;
    case 14:
        wait_command_processed = true;
        COMMUNICATION->assignDbToProc(R2CP::DB_Pulse,R2CP::ProcId_Standard_Mammography_2D,1);subStatus++;
        break;

    case 15:
        wait_command_processed = true;
        COMMUNICATION->assignDbToProc(R2CP::DB_Pre,R2CP::ProcId_Aec_Mammography_2D,1);subStatus++;
        break;

    case 16:
        wait_command_processed = true;
        COMMUNICATION->assignDbToProc(R2CP::DB_Pulse,R2CP::ProcId_Aec_Mammography_2D,2);subStatus++;
        break;

    case 17:
        wait_command_processed = true;
        COMMUNICATION->assignDbToProc(R2CP::DB_Pulse,R2CP::ProcId_Standard_Mammography_3D,1);subStatus++;
        break;

    case 18:
        wait_command_processed = true;
        COMMUNICATION->setGenerator_Assign_SkipPulse_Databank(R2CP::DB_SkipPulse,R2CP::ProcId_Standard_Mammography_3D);subStatus++;
        break;

    case 19:
        wait_command_processed = true;
        COMMUNICATION->assignDbToProc(R2CP::DB_Pre,R2CP::ProcId_Aec_Mammography_3D,1);subStatus++;
        break;

    case 20:
        wait_command_processed = true;
        COMMUNICATION->assignDbToProc(R2CP::DB_Pulse,R2CP::ProcId_Aec_Mammography_3D,2);subStatus++;
        break;

    case 21:
        wait_command_processed = true;
        COMMUNICATION->setGenerator_Assign_SkipPulse_Databank(R2CP::DB_SkipPulse,R2CP::ProcId_Aec_Mammography_3D);subStatus++;
        break;

    default:
        procedureCreated = true;
        EXPOSURE->tomoConfig.changed = false;
        qDebug() << "PROCEDURE CONFIGURED";
        returSubWorkflow();
        return;
    }

    scheduleWorkflow(10);

}



/**
 * @brief This is the SubWorkflow routine to clear the Generator System Messages
 *
 *
 * The SubWorkflow:
 * - Get the list of the active messages from the generator;
 * - Clear every active message in the list;
 * - Update the Generator status;
 * - Returns to the calling Workflow
 *
 */
void workflowManager::handle_CLEAR_ALL_SMS_MESSAGES(void){
    static QList<ulong> id;

    switch(subStatus){
     case 0:
        qDebug() << "CLEAR SYSTEM MESSAGES";
        subStatus++;

        break;

    case 1:
        wait_command_processed = true;
        COMMUNICATION->getAllSystemMessages();subStatus++;
        break;

    case 2:
        if(!command_process_state){
            // If the procedure fails, exit ..
            subStatus = 5;
            qDebug() << "Failed to get the message list";
            scheduleWorkflow(0);
            return;
        }

        if(COMMUNICATION->getErrorList()->size()==0) {
            // No messages are present
            subStatus = 5;
            qDebug() << "No messages are present";
            scheduleWorkflow(0);
            return;
        }

        id.clear();
        for(int i =0; i < COMMUNICATION->getErrorList()->size(); i++) id.append(COMMUNICATION->getErrorList()->at(i));
        subStatus++;
        break;

    case 3:

        wait_command_processed = true;
        COMMUNICATION->clearSystemMessage(id[0]);subStatus++;
        break;

    case 4:
        if(!command_process_state){
            subStatus = 5;
            qDebug() << "Failed to clear a message";
            scheduleWorkflow(0);
            return;
        }

        id.remove(0);
        if(id.size()==0) {
            subStatus = 5;
            scheduleWorkflow(0);
            return;
        }

        subStatus = 3;
        break;

    case 5: // Update the status as the final step
        COMMUNICATION->getGeneratorStatusV6();subStatus++;
        break;

    case 6: // Procedure completed
        returSubWorkflow();
        break;
    }

    scheduleWorkflow(10);

}

/**
 * @brief This is the Workflow routine handling the IDLE status
 *
 * The Idle status is the Status ready to activate an exposure.
 *
 * In Idle Status the generator is connected and initialized, \n
 * and no errors are present.
 *
 * During the Idle status a special System Message is kept active in the \n
 * Generator, preventing any unwanted exposure seqeunce activation.
 *
 *
 * The Workflow:
 * - Check and activate an external request to change the Workflow (Exposure activation);
 * - If a System error is detected, the workflow calls the SubWorkflow SMS_ERROR;
 *
 */
void workflowManager::handle_SMS_IDLE(void){
    QList<ulong> errlst;


    if(requestState != SMS_IDLE){
        changeWorkflow(requestState);
        return;
    }

    // Verify the system messages presents
    if(COMMUNICATION->getErrorList()->size()){
        changeWorkflow(SMS_ERROR);
        return;
    }


    switch(subStatus){
    case 0:
        qDebug() << "IDLE STATUS";
        abortRxRequest = false;
        INTERFACE->EventSetXrayEna(0,false);
        interfaceStatus = workflowManager::_READY_FOR_EXPOSURE;

        break;

    case 1:
        // Set the disable Rx message
        wait_command_processed = true;
        COMMUNICATION->setDisableRxSystemMessage(true);
        break;

    case 2:
        if(!command_process_state){
            // Repeat the disable Rx message command
            subStatus--;
            scheduleWorkflow(100);
            return;
        }
        break;

    case 3:

        break;

    case 10:
        //COMMUNICATION->getGeneratorStatusV6();
        subStatus = 2;
        scheduleWorkflow(100);
        return;
    }

    subStatus++;
    scheduleWorkflow(100);

}

/**
 * @brief This is the Workflow routine to handle the System Error Messages
 *
 * This Workflow is activated only in IDLE status when Generator System Messages are present.
 *
 * The Workflow sends every active message notification to the Gantry \n
 * through the EventMessage() notification.
 *
 * When all the messages has been notified, the workflow clears
 * the generator messages then return to the IDLE STatus.
 *
 *
 */
void workflowManager::handle_SMS_ERROR(void){
    QString messg;

    switch(subStatus){
      case 0:            

            interfaceStatus = workflowManager::_STATUS_ERROR;
            subStatus++;

            for(int i=0; i<COMMUNICATION->getErrorList()->size(); i++){
                messg = DBSysMessages->getErrorString(COMMUNICATION->getErrorList()->at(i));
                INTERFACE->EventMessage(0,messg);
                qDebug() << messg;
            }
            break;

      case 1:
            // Clear All messages
            changeSubWorkflow(SMS_CLEAR_SYS_MESSAGES,subStatus+1);
            return;

      case 2:
           // get all system messages
           wait_command_processed = true;
           COMMUNICATION->getAllSystemMessages();
           subStatus++;
           break;

       case 3:
            if(!command_process_state){
                subStatus--;
                scheduleWorkflow(100);
                return;
            }

           if(COMMUNICATION->getErrorList()->size()){
               // Repeat Until the error disappears
               subStatus = 1;
               scheduleWorkflow(500);
               return;
           }

           subStatus++;
           break;

       default:
            changeWorkflow(SMS_IDLE);
            return;
        }

    scheduleWorkflow(10);
}


