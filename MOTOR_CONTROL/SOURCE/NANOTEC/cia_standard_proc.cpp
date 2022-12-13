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
        ctrlw = rxSDO.getVal();
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
        ctrlw = rxSDO.getVal();
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
    static uchar store_attempt;
    uint index;
    static unsigned short chk = 0;

    switch(wStatus){
      case 0:
        deviceInitialized=false;
        nanojUploaded = false;
        wStatus++;
        qDebug() << "DEVICE (" << deviceId << "): UPLOAD NANOJ ..";
        return 1;

      case 1:
        delay = subNanojProgramUpload(false);
        if(delay) return delay;

        if(!nanojUploaded){
           qDebug() << "DEVICE (" << deviceId << "): UPLOAD NANOJ FAILED!";
        }

        if(configVector == nullptr){
            wStatus = 5;
            return 1;
        }else wStatus++;
        return 1;

      case 2:
        qDebug() << "DEVICE (" << deviceId << "): UPLOAD CONFIG VECTOR ..";

        // Calc the checksum of the vector to evaluate the reload in memory
        index = 0;
        chk = 0;
        while(1){
            if(configVector[index++].type == 0) break;
            chk += (ushort) configVector[index].val;
        }

        readSDO(OD_2700_03);
        wStatus++;
        return 1;

    case 3:
        if(!sdoRxTx.sdo_rx_ok){
            qDebug() << QString("DEVICE (%1): FAILED DATA READING, ERROR READING OD %2.%3").arg(deviceId).arg(txSDO.getIndex(),1,16).arg(txSDO.getSubIndex());
            return 0;
        }


        if(rxSDO.getVal() == chk){
            changed = false;
            wStatus = 5;
            return 1;
        }

        changed = true;
        writeSDO(OD_2700_03,chk);
        wStatus++;
        return 100;

    case 4:
        delay = subRoutineUploadVector(configVector, nullptr, &success);
        if(delay) return delay;

        if(!success){
            qDebug() << "DEVICE (" << deviceId << "): FAILED UPLOADING CONFIG VECTOR!";
            return 0;
        }
        wStatus++;
        return 1;

    case 5:
        if(initVector == nullptr){
            wStatus = 8;
            return 1;
        }else   wStatus++;

        return 1;

    case 6:
        qDebug() << "DEVICE (" << deviceId << "): UPLOAD VOLATILE VECTOR ..";
        wStatus++;
        return 1;

    case 7:
        delay = subRoutineUploadVector(initVector, nullptr, &success);
        if(delay) return delay;

        if(!success){
            qDebug() << "DEVICE (" << deviceId << "): FAILED UPLOADING VOLATILE VECTOR!";
            return 0;
        }
        wStatus++;
        return 1;

    case 8:

        if(!changed){
            qDebug() << "DEVICE (" << deviceId << "): INITIALIZATION COMPLETED";
            deviceInitialized=true;
            return 0;
        }

        qDebug() << "STORING";
        store_attempt = 10;
        wStatus++;
        return 1;

     case 9:
        if(store_attempt == 0) {
            qDebug() << QString("DEVICE (%1): FAILED DATA STORING, ERROR READING OD %2.%3").arg(deviceId).arg(txSDO.getIndex(),1,16).arg(txSDO.getSubIndex());
            return 0;
        }
        store_attempt--;

        // Stores the parameters
        writeSDO(OD_1010_01, OD_SAVE_CODE);
        sdoRxTx.sdo_rx_tx_pending = false;
        wStatus++;
        return 1000;

    case 10:
        readSDO(OD_1010_01);
        wStatus++;
        return 5;

    case 11:
        if(!sdoRxTx.sdo_rx_ok){
            qDebug() << QString("DEVICE (%1): FAILED DATA STORING, ERROR READING OD %2.%3").arg(deviceId).arg(txSDO.getIndex(),1,16).arg(txSDO.getSubIndex());
            return 0;
        }


        if(rxSDO.getVal() != 1){
            wStatus = 10;
            return 100;
        }

        wStatus++;
        return 1;

    case 12:
        writeSDO(OD_2700_01, 1);
        wStatus++;
        return 5;

    case 13:
        if(!sdoRxTx.sdo_rx_ok) {
            qDebug() << QString("DEVICE (%1): ERROR SAVING THE CHECKSUM").arg(deviceId);
            return 0;
        }

        wStatus++;
        return 1;

    case 14:
        readSDO(OD_2700_01);
        wStatus++;
        return 5;

    case 15:
        if(!sdoRxTx.sdo_rx_ok) {
            qDebug() << QString("DEVICE (%1): ERROR READING THE USER PARAM CONTROL REGISTER").arg(deviceId);
            return 0;
        }

        if(rxSDO.getVal()!=0){
            wStatus--;
            return 100;
        }
        wStatus++;
        return 5;

    case 16:
         qDebug() << "DEVICE (" << deviceId << "): INITIALIZATION COMPLETED";
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
        uval = rxSDO.getVal();
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
        uval = rxSDO.getVal();
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
        ctrlw = rxSDO.getVal();
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


ushort pd4Nanotec::idleCallback(void){
    if(execCommand == _ZERO_SETTING_COMMAND) return initPd4ZeroSettingCommand();
    else if(execCommand == _POSITIONING_COMMAND) return initPd4PositioningCommand();
    return 0;
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
            qDebug() << QString("DEVICE (%1): ERROR READING OD %2.%3").arg(deviceId).arg(txSDO.getIndex(),1,16).arg(txSDO.getSubIndex());
            return 0;
        }

        wStatus++;
        return 1;

    case 202: // reset the BIT4 of Control Word to start the sequence
        val = rxSDO.getVal();
        val &=~ OD_MASK(OD_6040_00_RESET_OMS);
        val |= OD_VAL(OD_6040_00_RESET_OMS);
        writeSDO(OD_6040_00, val);
        wStatus++;
        return 5;

    case 203:
        if(!sdoRxTx.sdo_rx_ok) {
            qDebug() << QString("DEVICE (%1): ERROR WRITING OD %2.%3").arg(deviceId).arg(txSDO.getIndex(),1,16).arg(txSDO.getSubIndex());
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
        val = rxSDO.getVal();
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
        delay = subReadPositionEncoder();
        if(delay) return delay;

        execCommand = _NO_COMMAND;
        qDebug() << "DEVICE (" << deviceId << "): COMMAND COMPLETED. Current position is:" << _POS_TO_cGRAD(position_encoder_val);
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

