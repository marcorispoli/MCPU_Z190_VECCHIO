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

    static ulong ctrlw;

    switch(wStatus){

    case 0: // Read the Control Word
        wStatus++;

    case 1:
         readSDO(OD_6040_00);
         wStatus++;
         return 5;

    case 2: // Get the control word
        if(!sdo_rx_ok) return 0;
        wStatus++;

        // To the Ready to SwitchOn Status
        ctrlw = rxSDO.getVal();
        ctrlw &=~ OD_MASK(OD_6040_00_SHUTDOWN);
        ctrlw |= OD_VAL(OD_6040_00_SHUTDOWN);
        return 1;

    case 3:
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
    static ulong ctrlw;

    switch(wStatus){

    case 0: // Read the Control Word
        wStatus++;

    case 1:
         readSDO(OD_6040_00);
         wStatus++;
         return 5;

    case 2: // Get the control word
        if(!sdo_rx_ok) return 0;
        wStatus++;

        // To the Ready to SwitchOn Status
        ctrlw = rxSDO.getVal();
        ctrlw &=~ OD_MASK(OD_6040_00_SWITCHON);
        ctrlw |= OD_VAL(OD_6040_00_SWITCHON);
        return 1;

    case 3:
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
ushort pd4Nanotec::initCallback(void){
    static bool changed = false;
    static bool success = false;
    ushort delay;
    static uchar store_attempt;

    switch(wStatus){
      case 0:
        wSubStatus = 0;
        deviceInitialized=false;
        wStatus++;
        return 1;

      case 1:

        if(initVector == nullptr){
            qDebug() << "INIT DEVICE (" << deviceId << "): COMPLETED";
            deviceInitialized=true;
            return 0;
        }

        delay = subRoutineUploadVector(initVector, &changed, &success);
        if(delay) return delay;

        if(!success){
            qDebug() << "INIT DEVICE (" << deviceId << "): FAILED!";
            return 0;
        }

        if(!changed){
            qDebug() << "INIT DEVICE (" << deviceId << "): COMPLETED";
            deviceInitialized=true;
            return 0;
        }

        store_attempt = 10;
        wStatus++;
        return 1;

     case 2:
        if(store_attempt == 0) {
            qDebug() << QString("DEVICE (%1): FAILED DATA STORING, ERROR READING OD %2.%3").arg(deviceId).arg(txSDO.getIndex(),1,16).arg(txSDO.getSubIndex());
            return 0;
        }
        store_attempt--;

        // Stores the parameters
        writeSDO(OD_1010_01, OD_SAVE_CODE);
        sdo_rx_tx_pending = false;
        wStatus++;
        return 1000;

    case 3:
        readSDO(OD_1010_01);
        wStatus++;
        return 5;

    case 4:
        if(!sdo_rx_ok){
            qDebug() << QString("DEVICE (%1): FAILED DATA STORING, ERROR READING OD %2.%3").arg(deviceId).arg(txSDO.getIndex(),1,16).arg(txSDO.getSubIndex());
            return 0;
        }


        if(rxSDO.getVal() != 1){
            wStatus = 2;
            return 100;
        }

        wStatus++;
        return 1;

    case 5:
         qDebug() << "INIT DEVICE (" << deviceId << "): STORE DATA COMPLETED";
         deviceInitialized=true;
         return 0;
    }

    return 0;
}

ushort pd4Nanotec::CiA402_FaultCallback(void){
    static ulong err_class = 0;
    static ulong err_code = 0;
    static ulong ctrlw;
    ulong uval;

    switch(wStatus){
    case 0:
        wStatus++;

    case 1:
        // Get the error class
        readSDO(OD_1001_00);
        wStatus++;
        return 5;

    case 2:
        if(!sdo_rx_ok) return 0;
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
        if(!sdo_rx_ok) return 0;
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
        if(!sdo_rx_ok) return 0;
        wStatus++;
        ctrlw = rxSDO.getVal();
        ctrlw |= 0x80;
        return 1;

   case 102:
        writeSDO(OD_6040_00,ctrlw);
        wStatus++;
        return 5;

    case 103:
        if(!sdo_rx_ok) return 0;
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
    ulong val;
    ushort delay;

    switch(wStatus){


    case 250: // Read the Control Word 
        wStatus++;

    case 251:
        readSDO(OD_6040_00);
        wStatus++;
        return 5;

    case 252: // Get the control word
        if(!sdo_rx_ok) {
            execCommand = _NO_COMMAND;
            qDebug() << "DEVICE (" << deviceId << "): COMMAND FAILED";
            return 0;
        }
        wStatus++;

        // To the SwitchOn Status
        val = rxSDO.getVal();
        val &=~ OD_MASK(OD_6040_00_DISABLEOP);
        val |= OD_VAL(OD_6040_00_DISABLEOP);
        return 1;

    case 253:
        writeSDO(OD_6040_00, val);
        wStatus++;
        return 5;

    case 254:
        if(!sdo_rx_ok) {
            execCommand = _NO_COMMAND;
            qDebug() << "DEVICE (" << deviceId << "): COMMAND FAILED";
            return 0;
        }

        execCommand = _NO_COMMAND;
        qDebug() << "DEVICE (" << deviceId << "): COMMAND COMPLETED";
        return 0;

    default:

        if(execCommand == _ZERO_SETTING_COMMAND) delay = pd4ZeroSettingLoop();
        else if(execCommand == _POSITIONING_COMMAND) delay = pd4PositioningLoop();
        else delay = 0; // invalid command or no command

        // Command termination
        if(delay == 0){
            execCommand = _NO_COMMAND;
            wStatus = 250;
            return 1;
        }
        return delay;
    }

}




