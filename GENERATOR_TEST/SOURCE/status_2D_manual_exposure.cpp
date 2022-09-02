#include "application.h"
#include <QTimer>




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
        if(!validate2DExposurePulse()){
            setErrorCondition(SMS_ERR_EXPOSURE, SMS_EXP_ERR_PULSE_VALIDATION, "EXPOSURE DATA VALIDATION",SMS_IDLE,0);
            return;
        }

        // Load Data Bank
        wait_command_processed = true;
        COMMUNICATION->set2DDataBank(R2CP::DB_Pulse,focus,pulse_kV,pulse_mAs,mA,mS);
        break;

    case 5:
        if(!command_process_state){
            setErrorCondition(SMS_ERR_EXPOSURE, SMS_EXP_ERR_PREPARATION, "SET DATABANK",SMS_IDLE,0);
            return;
        }

        // Procedure activation
        wait_command_processed = true;
        COMMUNICATION->activate2DProcedurePulse();
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

