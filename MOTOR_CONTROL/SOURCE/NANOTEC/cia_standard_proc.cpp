#include "application.h"
#include "pd4_dictionary.h"




/**
 * This status is activated by the Motor after the startup
 * procedure completes.
 *
 * From this status the Application can enter the \n
 * ReadyToSwitchOn status with the SHUTDOWN command sequence.
 *
 *
 * @return
 */
ushort pd4Nanotec::CiA402_SwitchOnDisabledCallback(void){

    uint ctrlw;

    switch(wStatus){

    case 0: // Read the Control Word
        wStatus++;

    case 1:
         readSDO(OD_6040_00);
         wStatus++;
         return 5;

    case 2: // Get the control word
        if(!sdoRxTx.sdo_rx_ok) return 0;
        wStatus++;
        return 1;

    case 3:
        // To the Ready to SwitchOn Status
        ctrlw = sdoRxTx.rxSDO.getVal();
        ctrlw &=~ OD_MASK(OD_6040_00_SHUTDOWN);
        ctrlw |= OD_VAL(OD_6040_00_SHUTDOWN);
        writeSDO(OD_6040_00, ctrlw);
        wStatus++;
        return 5;

    default:

        return 0;

    }

    return 0;
}

/**
 * This is the ReadyToSwitchOn CiA status handler
 *
 * The Application shall attempt to enter the
 * SwitchedOn status
 * @return
 */
ushort pd4Nanotec::CiA402_ReadyToSwitchOnCallback(void){
    uint ctrlw;

    switch(wStatus){

    case 0: // Read the Control Word
        wStatus++;

    case 1:
         readSDO(OD_6040_00);
         wStatus++;
         return 5;

    case 2: // Get the control word
        if(!sdoRxTx.sdo_rx_ok) return 0;
        wStatus++;       
        return 1;

    case 3:
        // To the Ready to SwitchOn Status
        ctrlw = sdoRxTx.rxSDO.getVal();
        ctrlw &=~ OD_MASK(OD_6040_00_SWITCHON);
        ctrlw |= OD_VAL(OD_6040_00_SWITCHON);
        writeSDO(OD_6040_00, ctrlw);
        wStatus++;
        return 5;

    default:

        return 0;

    }

    return 0;

}


/**
 * Procedura di inizializzazione del motore TRX
 * @return
 */
ushort pd4Nanotec::workflowInitCallback(void){
    static bool changed = false;
    static bool success = false;
    ushort delay;
    uint index;
    static unsigned short chk = 0;

    switch(wStatus){
      case 0:
        deviceInitialized=false;
        nanojUploaded = false;
        wStatus++;
        qDebug() << "DEVICE (" << deviceId << "): DEVICE INITIALIZATION PROCESS STARTED";
        return 1;

      case 1: // Upload the Nanoj Vector ----------------------------------------------
        delay = subNanojProgramUpload(false);
        if(delay) return delay;

        if(!nanojUploaded){
           qDebug() << "DEVICE (" << deviceId << "): UPLOAD NANOJ FAILED!";
           return 0;
        }

        wStatus++;
        return 1;

      case 2: // Upload the Config Vector ----------------------------------------------
        if(registerVectors.configVector == nullptr){
            wStatus = 5;
            return 1;
        }

        qDebug() << "DEVICE (" << deviceId << "): UPLOAD CONFIG VECTOR ..";

        // Calc the checksum of the vector to evaluate the reload in memory
        index = 0;
        chk = 0;
        while(1){
            if(registerVectors.configVector[index++].type == 0) break;
            chk += (ushort) registerVectors.configVector[index].val;
        }

        readSDO(CONFIG_USER_PARAM);
        wStatus++;
        return 1;

    case 3:
        if(!sdoRxTx.sdo_rx_ok){
            qDebug() << QString("DEVICE (%1): FAILED DATA READING, ERROR READING OD %2.%3").arg(deviceId).arg(sdoRxTx.txSDO.getIndex(),1,16).arg(sdoRxTx.txSDO.getSubIndex());
            return 0;
        }

        if(sdoRxTx.rxSDO.getVal() == chk){
            changed = false;
            wStatus = 5;
            return 1;
        }

        changed = true;
        writeSDO(CONFIG_USER_PARAM,chk);
        wStatus++;
        return 100;

    case 4:
        delay = subRoutineUploadVector(registerVectors.configVector, nullptr, &success);
        if(delay) return delay;

        if(!success){
            qDebug() << "DEVICE (" << deviceId << "): FAILED UPLOADING CONFIG VECTOR!";
            return 0;
        }
        wStatus++;
        return 1;

    case 5: // Upload the Runtime Vector ----------------------------------------------
        if(registerVectors.runtimeVector == nullptr){
            wStatus = 8;
            return 1;
        }else   wStatus++;

        return 1;

    case 6:
        qDebug() << "DEVICE (" << deviceId << "): UPLOAD RUNTIME VECTOR ..";
        wStatus++;
        return 1;

    case 7:
        delay = subRoutineUploadVector(registerVectors.runtimeVector, nullptr, &success);
        if(delay) return delay;

        if(!success){
            qDebug() << "DEVICE (" << deviceId << "): FAILED UPLOADING VOLATILE VECTOR!";
            return 0;
        }
        wStatus++;
        return 1;

    case 8: // Verify if the parameter shall be stored into memory
        if(!changed){
            wStatus = 17;
            return 1;
        }

        qDebug() << "STORING";
        wStatus++;
        return 1;

     case 9:
        writeSDO(RESET_USER_PARAM, 0); // Reset the user Param Reset Flag
        wStatus++;
        return 5;

    case 10:
        if(!sdoRxTx.sdo_rx_ok){
            qDebug() << QString("DEVICE (%1): FAILED WRITING OD %2.%3").arg(deviceId).arg(sdoRxTx.txSDO.getIndex(),1,16).arg(sdoRxTx.txSDO.getSubIndex());
            return 0;
        }

        // Stores the Object Dictionary parameters
        writeSDO(OD_1010_01, OD_SAVE_CODE);
        wStatus++;
        return 1000;

    case 11:
        readSDO(OD_1010_01);
        wStatus++;
        return 5;

    case 12:
        if(!sdoRxTx.sdo_rx_ok){
            qDebug() << QString("DEVICE (%1): FAILED DATA STORING, ERROR READING OD %2.%3").arg(deviceId).arg(sdoRxTx.txSDO.getIndex(),1,16).arg(sdoRxTx.txSDO.getSubIndex());
            return 0;
        }

        if(sdoRxTx.rxSDO.getVal() != 1){
            wStatus = 11;
            return 100;
        }

        wStatus++;
        return 1;

    case 13: // Stores the user parameters
        writeSDO(SAVE_USER_PARAM, 1);
        wStatus++;
        return 5;

    case 14:
        if(!sdoRxTx.sdo_rx_ok) {
            qDebug() << QString("DEVICE (%1): ERROR STORING THE USER PARAMETERS").arg(deviceId);
            return 0;
        }

        wStatus++;
        return 1;

    case 15:
        readSDO(SAVE_USER_PARAM);
        wStatus++;
        return 5;

    case 16:
        if(!sdoRxTx.sdo_rx_ok) {
            qDebug() << QString("DEVICE (%1): ERROR READING THE USER PARAM CONTROL REGISTER").arg(deviceId);
            return 0;
        }

        if(sdoRxTx.rxSDO.getVal()!=0){
            wStatus--;
            return 100;
        }
        wStatus++;
        return 5;

    case 17: // End of the Initialization process
        writeSDO(RESET_USER_PARAM, 1); // Set to 1 the Reset Flag
        wStatus++;
        return 5;

    case 18:
        if(!sdoRxTx.sdo_rx_ok) {
            qDebug() << QString("DEVICE (%1): ERROR READING THE USER PARAM CONTROL REGISTER").arg(deviceId);
            return 0;
        }

        qDebug() << "DEVICE (" << deviceId << "): INITIALIZATION COMPLETED " << sdoRxTx.rxSDO.getVal();
        deviceInitialized=true;
        return 0;
    }

    return 0;
}

ushort pd4Nanotec::CiA402_FaultCallback(void){
    static uint err_class = 0;
    static uint err_code = 0;
    static uint ctrlw;
    uint uval;

    switch(wStatus){
    case 0:
        wStatus++;

    case 1:
        // Get the error class
        readSDO(OD_1001_00);
        wStatus++;
        return 5;

    case 2:
        if(!sdoRxTx.sdo_rx_ok) return 0;
        uval = sdoRxTx.rxSDO.getVal();
        if(uval == 0){
            err_class = 0;
            err_code = 0;

            // Try to exit from the Fault status
            wStatus = 100;
            return 1;
        }

        if(uval != err_class) qDebug() << "DEVICE (" << deviceId << ") ERROR CLASS:" <<  getErrorClass1001(uval);
        err_class = uval;
        wStatus++;
        return 1;

    case 3:
        // Get the error code
        readSDO(OD_1003_01);
        wStatus++;
        return 5;

    case 4:
        if(!sdoRxTx.sdo_rx_ok) return 0;
        uval = sdoRxTx.rxSDO.getVal();
        if(uval != err_code) qDebug() << "DEVICE (" << deviceId << ") ERROR CODE:" << getErrorCode1003(uval);
        err_code = uval;
        wStatus++;
        return 1;

    case 5:
        wStatus = 1;
        return 500;

   case 100:
        readSDO(OD_6040_00);
        wStatus++;
        return 5;

   case 101:
        if(!sdoRxTx.sdo_rx_ok) return 0;
        wStatus++;
        ctrlw = sdoRxTx.rxSDO.getVal();
        ctrlw |= 0x80;
        return 1;

   case 102:
        writeSDO(OD_6040_00,ctrlw);
        wStatus++;
        return 5;

    case 103:
        if(!sdoRxTx.sdo_rx_ok) return 0;
        wStatus++;
        ctrlw &=~ 0x80;
        return 1;

    case 104:
        writeSDO(OD_6040_00,ctrlw);
        wStatus++;
        return 5;

    case 105:
        return 0;

    }

    return 0;
}


ushort pd4Nanotec::CiA402_SwitchedOnCallback(void){
    ushort delay;
    static uchar locStat = 0;

    // In case the nanoj has been started and not stopped.
    if(nanojStr.nanoj_program_started){
        delay = subDisableNanojProgram();
        if(delay) return delay;
        locStat = 0;
        return 0;
    }

    // handles the available commands
    if(execCommand == _ZERO_SETTING_COMMAND) return initPd4ZeroSettingCommand();
    else if(execCommand == _POSITIONING_COMMAND) return initPd4PositioningCommand();

    // Delay the Cia Status register calling to limit the CAN Band occupancy
    locStat++;
    if(locStat < 10) return 100;
    else {
        locStat = 0;
        return 0;
    }
}

ushort pd4Nanotec::CiA402_OperationEnabledCallback(void){
    uint val;
    ushort delay;

    switch(wStatus){

    case 200:// Read the Control Word
        readSDO(OD_6040_00);
        wStatus++;
        return 5;

    case 201: // Get the control word
        if(!sdoRxTx.sdo_rx_ok) {
            qDebug() << QString("DEVICE (%1): ERROR READING OD %2.%3").arg(deviceId).arg(sdoRxTx.txSDO.getIndex(),1,16).arg(sdoRxTx.txSDO.getSubIndex());
            return 0;
        }

        wStatus++;
        return 1;

    case 202: // reset the BIT4 of Control Word to start the sequence
        val = sdoRxTx.rxSDO.getVal();
        val &=~ OD_MASK(OD_6040_00_RESET_OMS);
        val |= OD_VAL(OD_6040_00_RESET_OMS);
        writeSDO(OD_6040_00, val);
        wStatus++;
        return 5;

    case 203:
        if(!sdoRxTx.sdo_rx_ok) {
            qDebug() << QString("DEVICE (%1): ERROR WRITING OD %2.%3").arg(deviceId).arg(sdoRxTx.txSDO.getIndex(),1,16).arg(sdoRxTx.txSDO.getSubIndex());
            return 0;
        }

        wStatus++;
        return 1;

    case 204: // Read the Control Word
        wStatus++;

    case 205:
        readSDO(OD_6040_00);
        wStatus++;
        return 5;

    case 206: // Get the control word
        if(!sdoRxTx.sdo_rx_ok) {
            execCommand = _NO_COMMAND;
            qDebug() << "DEVICE (" << deviceId << "): COMMAND FAILED";
            return 0;
        }
        wStatus++;
        return 1;

    case 207:
        // To the SwitchOn Status
        val = sdoRxTx.rxSDO.getVal();
        val &=~ OD_MASK(OD_6040_00_DISABLEOP);
        val |= OD_VAL(OD_6040_00_DISABLEOP);
        writeSDO(OD_6040_00, val);
        wStatus++;
        return 5;

    case 208:
        if(!sdoRxTx.sdo_rx_ok) {
            execCommand = _NO_COMMAND;
            qDebug() << "DEVICE (" << deviceId << "): COMMAND FAILED";
            return 0;
        }

        wStatus++;
        return 100;

    case 209:
        delay = subDisableNanojProgram();
        if(delay) return delay;

        wStatus++;
        return 1;

    case 210:
        delay = subReadPositionEncoder();
        if(delay) return delay;

        execCommand = _NO_COMMAND;
        qDebug() << "DEVICE (" << deviceId << "): COMMAND COMPLETED. Current position is:" << convert_EncoderUnit_TO_cGRAD(position_encoder_val);
        return 0;

    default:

        if(execCommand == _ZERO_SETTING_COMMAND) delay = pd4ZeroSettingLoop();
        else if(execCommand == _POSITIONING_COMMAND) delay = pd4PositioningLoop();
        else delay = 0; // invalid command or no command

        // Command termination
        if(delay == 0){
            execCommand = _NO_COMMAND;
            wStatus = 200;
            return 1;
        }
        return delay;
    }

}


ushort pd4Nanotec::workflowUploadNanoj(void){
    ushort delay;

    switch(wStatus){
      case 0:
        wStatus++;
        return 1;

      case 1:
        delay = subNanojProgramUpload(true);
        if(delay) return delay;

        return 0;

    }
}

