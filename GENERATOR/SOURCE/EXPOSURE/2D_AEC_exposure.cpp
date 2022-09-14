#include "application.h"
#include <QTimer>
/*
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


        // Get Status
        wait_command_processed = true;
        COMMUNICATION->getGeneratorStatusV6();
        break;

    case 4:
        if(!command_process_state){
            setErrorCondition(SMS_ERR_EXPOSURE, SMS_EXP_ERR_PREPARATION, "GET STATUS",SMS_IDLE,0);
            return;
        }

        // Clear all the System Messages
        if(R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.SystemMessage.Fields.Active == R2CP::Stat_SystemMessageActive_Active){
            changeStatus(SMS_CLEAR_SYS_MESSAGES,0,internalState,subStatus+1);
            return;
        }
        break;

    case 5: // Test Not cleared messages

        if(R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.SystemMessage.Fields.Active == R2CP::Stat_SystemMessageActive_Active){
            setErrorCondition(SMS_ERR_EXPOSURE, SMS_EXP_ERR_SYSMSG, "SYSTEM MESSAGES PRESENTS",SMS_IDLE,0);
            return;
        }

        qDebug() << "WAITING PUSH BUTTON PRESS..";

        wait_command_processed = true;
        COMMUNICATION->getGeneratorStatusV6();
        break;

    case 6:
        if(!command_process_state){
            setErrorCondition(SMS_ERR_EXPOSURE, SMS_EXP_ERR_PREPARATION, "GET STATUS",SMS_IDLE,0);
            return;
        }

        // Wait for the Exposure in progress
        switch(oldstat){
            case R2CP::Stat_Standby:

                if(R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.SystemMessage.Fields.Active == R2CP::Stat_SystemMessageActive_Active){
                    setErrorCondition(SMS_ERR_EXPOSURE, SMS_EXP_ERR_SYSMSG, "SYSTEM MESSAGES PRESENTS",SMS_IDLE,0);
                    return;
                }

                QTimer::singleShot(10, this, SLOT(handleCurrentStatus()));
                return;

            case R2CP::Stat_ExpInProgress:
            case R2CP::Stat_Ready:
                qDebug() << "START EXPOSURE";
                break;


            case R2CP::Stat_Error:
                setErrorCondition(SMS_ERR_EXPOSURE, SMS_EXP_ERR_PREPARATION, "GENERATOR ERROR CONDITION",SMS_IDLE,0);
                return;

            case R2CP::Stat_WaitFootRelease:
            case R2CP::Stat_GoigToShutdown:
            case R2CP::Stat_Service:
            case R2CP::Stat_Initialization:
                setErrorCondition(SMS_ERR_EXPOSURE, SMS_EXP_ERR_PREPARATION, "INVALID GENERATOR STATUS",SMS_IDLE,0);
                return;
        }

        break;

    case 7:

        wait_command_processed = true;
        COMMUNICATION->getGeneratorStatusV6();
        break;
        break;

    case 8:
        if(!command_process_state){
            setErrorCondition(SMS_ERR_EXPOSURE, SMS_EXP_ERR_PREPARATION, "GET STATUS",SMS_IDLE,0);
            return;
        }

        // Wait for Standby
        switch(oldstat){
*/

void statusManager::handle_2D_AEC(void){
    static uchar oldstat;
    bool  chgstat = false;

    /*
    if(R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.GeneratorStatus != oldstat){
        chgstat = true;
        oldstat = R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.GeneratorStatus;
    }

    WINDOWS->onRecetionGenStatusSlot();

    switch(subStatus){
    case 0:
        WINDOWS->setStatus("EXPOSURE 2D AEC");
        qDebug() << "EXPOSURE 2D AEC STATUS";
        aecDataPresent = false;
        break;

    case 1:
        // Validate the exposure data
        if(!validate2DExposurePre()){
            setErrorCondition(SMS_ERR_EXPOSURE, SMS_EXP_ERR_PULSE_VALIDATION, "EXPOSURE DATA VALIDATION",SMS_IDLE,0);
            return;
        }

        // Load Data Bank
        wait_command_processed = true;
        COMMUNICATION->set2DDataBank(R2CP::DB_Pre,focus,pre_kV,pre_mAs,mA,mS);
        break;

    case 2:
        if(!command_process_state){
            setErrorCondition(SMS_ERR_EXPOSURE, SMS_EXP_ERR_PREPARATION, "SET DATABANK",SMS_IDLE,0);
            return;
        }

        // Procedure activation
        wait_command_processed = true;
        COMMUNICATION->activate2DAecProcedurePre();
        break;

    case 3:
        if(!command_process_state){
            setErrorCondition(SMS_ERR_EXPOSURE, SMS_EXP_ERR_PREPARATION, "PROCEDURE ACTIVATION",SMS_IDLE,0);
            return;

        }

        // Get Status
        wait_command_processed = true;
        COMMUNICATION->getGeneratorStatusV6();
        break;

    case 4:
        if(!command_process_state){
            setErrorCondition(SMS_ERR_EXPOSURE, SMS_EXP_ERR_PREPARATION, "GET STATUS",SMS_IDLE,0);
            return;
        }

        // Clear all the System Messages
        if(R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.SystemMessage.Fields.Active == R2CP::Stat_SystemMessageActive_Active){
            changeStatus(SMS_CLEAR_SYS_MESSAGES,0,internalState,subStatus+1);
            return;
        }
        break;

    case 5: // Test Not cleared messages

        if(R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.SystemMessage.Fields.Active == R2CP::Stat_SystemMessageActive_Active){
            setErrorCondition(SMS_ERR_EXPOSURE, SMS_EXP_ERR_SYSMSG, "SYSTEM MESSAGES PRESENTS",SMS_IDLE,0);
            return;
        }

        qDebug() << "WAITING PUSH BUTTON PRESS..";

        wait_command_processed = true;
        COMMUNICATION->getGeneratorStatusV6();
        break;

    case 6:
        if(!command_process_state){
            setErrorCondition(SMS_ERR_EXPOSURE, SMS_EXP_ERR_PREPARATION, "GET STATUS",SMS_IDLE,0);
            return;
        }

        // Wait for the Exposure in progress
        switch(oldstat){
            case R2CP::Stat_Standby:

                if(R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.SystemMessage.Fields.Active == R2CP::Stat_SystemMessageActive_Active){
                    setErrorCondition(SMS_ERR_EXPOSURE, SMS_EXP_ERR_SYSMSG, "SYSTEM MESSAGES PRESENTS",SMS_IDLE,0);
                    return;
                }

                QTimer::singleShot(10, this, SLOT(handleCurrentStatus()));
                return;

            case R2CP::Stat_ExpInProgress:
            case R2CP::Stat_Ready:
                qDebug() << "START EXPOSURE";
                break;


            case R2CP::Stat_Error:
                setErrorCondition(SMS_ERR_EXPOSURE, SMS_EXP_ERR_PREPARATION, "GENERATOR ERROR CONDITION",SMS_IDLE,0);
                return;

            case R2CP::Stat_WaitFootRelease:
            case R2CP::Stat_GoigToShutdown:
            case R2CP::Stat_Service:
            case R2CP::Stat_Initialization:
                setErrorCondition(SMS_ERR_EXPOSURE, SMS_EXP_ERR_PREPARATION, "INVALID GENERATOR STATUS",SMS_IDLE,0);
                return;
        }

        break;

    case 7:

        wait_command_processed = true;
        COMMUNICATION->getGeneratorStatusV6();
        break;
        break;

    case 8:
        if(!command_process_state){
            setErrorCondition(SMS_ERR_EXPOSURE, SMS_EXP_ERR_PREPARATION, "GET STATUS",SMS_IDLE,0);
            return;
        }

        // Wait for Standby
        switch(oldstat){

            case R2CP::Stat_Standby:
                qDebug() << "WAIT FOR AEC DATA";
                break;


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

    case 9:
        if(!aecDataPresent){
            QTimer::singleShot(10, this, SLOT(handleCurrentStatus()));
            return;
        }

        qDebug() << "AEC acquisito";

        if(R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.SystemMessage.Fields.Active == R2CP::Stat_SystemMessageActive_Active){
            setErrorCondition(SMS_ERR_EXPOSURE, SMS_EXP_ERR_SYSMSG, "SYSTEM MESSAGES PRESENTS",SMS_IDLE,0);
            return;
        }

        // Validate the exposure data
        if(!validate2DExposurePulse()){
            setErrorCondition(SMS_ERR_EXPOSURE, SMS_EXP_ERR_PULSE_VALIDATION, "EXPOSURE PULSE DATA VALIDATION",SMS_IDLE,0);
            return;
        }

        // Load Data Bank
        wait_command_processed = true;
        COMMUNICATION->set2DDataBank(R2CP::DB_Pulse,focus,pulse_kV,pulse_mAs,mA,mS);
        break;

    case 10:
        if(!command_process_state){
            setErrorCondition(SMS_ERR_EXPOSURE, SMS_EXP_ERR_PREPARATION, "SET PULSE DATABANK",SMS_IDLE,0);
            return;
        }

        // Procedure activation
        wait_command_processed = true;
        COMMUNICATION->activate2DAecProcedurePulse();
        break;

    case 11:
        if(!command_process_state){
            setErrorCondition(SMS_ERR_EXPOSURE, SMS_EXP_ERR_PREPARATION, "PROCEDURE AEC PULSE ACTIVATION",SMS_IDLE,0);
            return;
        }

        oldstat = R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.GeneratorStatus;
        qDebug() << "attesa esecuzione";
        break;

    case 12:
        // Wait for the Exposure in progress
        switch(oldstat){
            case R2CP::Stat_Standby:

                if(R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.SystemMessage.Fields.Active == R2CP::Stat_SystemMessageActive_Active){
                    setErrorCondition(SMS_ERR_EXPOSURE, SMS_EXP_ERR_SYSMSG, "SYSTEM MESSAGES PRESENTS",SMS_IDLE,0);
                    return;
                }

                QTimer::singleShot(10, this, SLOT(handleCurrentStatus()));
                return;

            case R2CP::Stat_Preparation:
                if(chgstat) qDebug() << "EXPOSURE PREPARATION ..";
                break;
            case R2CP::Stat_Ready:
                if(chgstat) qDebug() << "EXPOSURE READY ..";
                break;
            case R2CP::Stat_ExpInProgress:
                if(chgstat) qDebug() << "EXPOSURE IN PROGRESS ..";
                break;

            case R2CP::Stat_Error:
                setErrorCondition(SMS_ERR_EXPOSURE, SMS_EXP_ERR_PREPARATION, "GENERATOR ERROR CONDITION",SMS_IDLE,0);
                return;

            case R2CP::Stat_WaitFootRelease:
            case R2CP::Stat_GoigToShutdown:
            case R2CP::Stat_Service:
            case R2CP::Stat_Initialization:
                setErrorCondition(SMS_ERR_EXPOSURE, SMS_EXP_ERR_PREPARATION, "INVALID GENERATOR STATUS",SMS_IDLE,0);
                return;
        }

        break;

    case 13:
        // Wait for Standby
        switch(oldstat){

            case R2CP::Stat_Standby:
                qDebug() << "EXPOSURE AEC COMPLETED";
                break;


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
    default:
        changeStatus(SMS_IDLE,0,SMS_IDLE,0);
        return;
    }

    subStatus++;
    QTimer::singleShot(10, this, SLOT(handleCurrentStatus()));
*/
}


/*
void statusManager::handle_2D_AEC(void){
    static uchar oldstat;
    bool  chgstat = false;

    if(R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.GeneratorStatus != oldstat){
        chgstat = true;
        oldstat = R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.GeneratorStatus;
    }

    WINDOWS->onRecetionGenStatusSlot();

    switch(subStatus){
    case 0:
        WINDOWS->setStatus("EXPOSURE 2D AEC");
        qDebug() << "EXPOSURE 2D AEC STATUS";
        aecDataPresent = false;
        break;

    case 1: // Get status to check the generator before to proceed
        wait_command_processed = true;
        COMMUNICATION->getGeneratorStatusV6();
        break;

    case 2:
        if(!command_process_state){
            setErrorCondition(SMS_ERR_EXPOSURE, SMS_EXP_ERR_PREPARATION, "GET STATUS",SMS_IDLE,0);
            return;
        }
        break;

    case 3:  // Clear all the System Messages
        if(R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.SystemMessage.Fields.Active == R2CP::Stat_SystemMessageActive_Active){
            changeStatus(SMS_CLEAR_SYS_MESSAGES,0,internalState,subStatus+1);
            return;
        }
        break;    

    case 4: // Test Not cleared messages

        if(R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.SystemMessage.Fields.Active == R2CP::Stat_SystemMessageActive_Active){
            setErrorCondition(SMS_ERR_EXPOSURE, SMS_EXP_ERR_SYSMSG, "SYSTEM MESSAGES PRESENTS",SMS_IDLE,0);
            return;
        }

        // Validate the exposure data
        if(!validate2DExposurePre()){
            setErrorCondition(SMS_ERR_EXPOSURE, SMS_EXP_ERR_PULSE_VALIDATION, "EXPOSURE PRE DATA VALIDATION",SMS_IDLE,0);
            return;
        }

        // Load Data Bank
        wait_command_processed = true;
        COMMUNICATION->set2DDataBank(R2CP::DB_Pre,focus,pre_kV,pre_mAs,mA,mS);
        break;

    case 5:
        if(!command_process_state){
            setErrorCondition(SMS_ERR_EXPOSURE, SMS_EXP_ERR_PREPARATION, "SET PRE DATABANK",SMS_IDLE,0);
            return;
        }

        // Procedure activation
        wait_command_processed = true;
        COMMUNICATION->activate2DAecProcedurePre();
        break;

    case 6:
        if(!command_process_state){
            setErrorCondition(SMS_ERR_EXPOSURE, SMS_EXP_ERR_PREPARATION, "PROCEDURE ACTIVATION",SMS_IDLE,0);
            return;

        }
        oldstat = R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.GeneratorStatus;

        subStatus = 7;
        //wait_command_processed = true;
        //COMMUNICATION->startExposure();
        break;

    case 7:
        if(!command_process_state){
            setErrorCondition(SMS_ERR_EXPOSURE, SMS_EXP_ERR_PREPARATION, "START EXPOSURE COMMAND",SMS_IDLE,0);
            return;
        }

        break;

    case 8:
        if(R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.SystemMessage.Fields.Active == R2CP::Stat_SystemMessageActive_Active){
            setErrorCondition(SMS_ERR_EXPOSURE, SMS_EXP_ERR_SYSMSG, "SYSTEM MESSAGES PRESENTS",SMS_IDLE,0);
            return;
        }

        if(oldstat == R2CP::Stat_Standby){
            qDebug() << "WAITING PUSH BUTTON PRESS..";
        }
        break;

    case 9:
        // Wait for the Exposure in progress
        switch(oldstat){
            case R2CP::Stat_Standby:                

                if(R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.SystemMessage.Fields.Active == R2CP::Stat_SystemMessageActive_Active){
                    setErrorCondition(SMS_ERR_EXPOSURE, SMS_EXP_ERR_SYSMSG, "SYSTEM MESSAGES PRESENTS",SMS_IDLE,0);
                    return;
                }

                QTimer::singleShot(10, this, SLOT(handleCurrentStatus()));
                return;

            case R2CP::Stat_Preparation:
                if(chgstat) qDebug() << "EXPOSURE PREPARATION ..";
                break;
            case R2CP::Stat_Ready:
                if(chgstat) qDebug() << "EXPOSURE READY ..";
                break;
            case R2CP::Stat_ExpInProgress:
                if(chgstat) qDebug() << "EXPOSURE IN PROGRESS ..";
                break;

            case R2CP::Stat_Error:
                setErrorCondition(SMS_ERR_EXPOSURE, SMS_EXP_ERR_PREPARATION, "GENERATOR ERROR CONDITION",SMS_IDLE,0);
                return;

            case R2CP::Stat_WaitFootRelease:
            case R2CP::Stat_GoigToShutdown:            
            case R2CP::Stat_Service:
            case R2CP::Stat_Initialization:
                setErrorCondition(SMS_ERR_EXPOSURE, SMS_EXP_ERR_PREPARATION, "INVALID GENERATOR STATUS",SMS_IDLE,0);
                return;
        }

        break;

    case 10:
        // Wait for Standby
        switch(oldstat){

            case R2CP::Stat_Standby:
                qDebug() << "WAIT FOR AEC DATA";
                break;


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
        if(!aecDataPresent){
            QTimer::singleShot(10, this, SLOT(handleCurrentStatus()));
            return;
        }

        qDebug() << "AEC acquisito";

        if(R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.SystemMessage.Fields.Active == R2CP::Stat_SystemMessageActive_Active){
            setErrorCondition(SMS_ERR_EXPOSURE, SMS_EXP_ERR_SYSMSG, "SYSTEM MESSAGES PRESENTS",SMS_IDLE,0);
            return;
        }

        // Validate the exposure data
        if(!validate2DExposurePulse()){
            setErrorCondition(SMS_ERR_EXPOSURE, SMS_EXP_ERR_PULSE_VALIDATION, "EXPOSURE PULSE DATA VALIDATION",SMS_IDLE,0);
            return;
        }

        // Load Data Bank
        wait_command_processed = true;
        COMMUNICATION->set2DDataBank(R2CP::DB_Pulse,focus,pulse_kV,pulse_mAs,mA,mS);
        break;

    case 12:
        if(!command_process_state){
            setErrorCondition(SMS_ERR_EXPOSURE, SMS_EXP_ERR_PREPARATION, "SET PULSE DATABANK",SMS_IDLE,0);
            return;
        }

        // Procedure activation
        wait_command_processed = true;
        COMMUNICATION->activate2DAecProcedurePulse();
        break;

    case 13:
        if(!command_process_state){
            setErrorCondition(SMS_ERR_EXPOSURE, SMS_EXP_ERR_PREPARATION, "PROCEDURE AEC PULSE ACTIVATION",SMS_IDLE,0);
            return;
        }

        oldstat = R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.GeneratorStatus;
        qDebug() << "attesa esecuzione";
        break;

    case 14:
        // Wait for the Exposure in progress
        switch(oldstat){
            case R2CP::Stat_Standby:

                if(R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.SystemMessage.Fields.Active == R2CP::Stat_SystemMessageActive_Active){
                    setErrorCondition(SMS_ERR_EXPOSURE, SMS_EXP_ERR_SYSMSG, "SYSTEM MESSAGES PRESENTS",SMS_IDLE,0);
                    return;
                }

                QTimer::singleShot(10, this, SLOT(handleCurrentStatus()));
                return;

            case R2CP::Stat_Preparation:
                if(chgstat) qDebug() << "EXPOSURE PREPARATION ..";
                break;
            case R2CP::Stat_Ready:
                if(chgstat) qDebug() << "EXPOSURE READY ..";
                break;
            case R2CP::Stat_ExpInProgress:
                if(chgstat) qDebug() << "EXPOSURE IN PROGRESS ..";
                break;

            case R2CP::Stat_Error:
                setErrorCondition(SMS_ERR_EXPOSURE, SMS_EXP_ERR_PREPARATION, "GENERATOR ERROR CONDITION",SMS_IDLE,0);
                return;

            case R2CP::Stat_WaitFootRelease:
            case R2CP::Stat_GoigToShutdown:
            case R2CP::Stat_Service:
            case R2CP::Stat_Initialization:
                setErrorCondition(SMS_ERR_EXPOSURE, SMS_EXP_ERR_PREPARATION, "INVALID GENERATOR STATUS",SMS_IDLE,0);
                return;
        }

        break;

    case 15:
        // Wait for Standby
        switch(oldstat){

            case R2CP::Stat_Standby:
                qDebug() << "EXPOSURE AEC COMPLETED";
                break;


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
    default:
        changeStatus(SMS_IDLE,0,SMS_IDLE,0);
        return;
    }

    subStatus++;
    QTimer::singleShot(10, this, SLOT(handleCurrentStatus()));

}

*/
