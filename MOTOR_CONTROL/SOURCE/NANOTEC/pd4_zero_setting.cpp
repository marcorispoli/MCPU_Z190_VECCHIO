#include "application.h"
#include "pd4_dictionary.h"


bool pd4Nanotec::activateZeroSetting(void){
    if( execCommand != _NO_COMMAND){
        qDebug() << "DEVICE (" << deviceId << "): INVALID ZERO SETTING ACTIVATION. COMMAND EXECUTING !";
        return false;
    }
    if(CiAcurrentStatus != CiA402_SwitchedOn){
        qDebug() << "DEVICE (" << deviceId << "): INVALID ZERO SETTING ACTIVATION. WRONG CiA STATUS !";
        return false;
    }

    safety.immediate_stop_command = false;
    execCommand = _ZERO_SETTING_COMMAND;
    return true;

}



ushort pd4Nanotec::initPd4ZeroSettingCommand(void){
    ushort delay;
    static bool success;
    static uint ctrlw;

    switch(wStatus){
    case 0:
        // Upload Zero Setting registers
        wStatus++;
        zero_setting_ok = false;
        return 1;

    case 1:
      // Upload Zero setting register loop
      delay = subRoutineUploadVector(registerVectors.zeroSettingVector, nullptr, &success);
      if(delay) return delay;

      if(!success){
          qDebug() << "DEVICE (" << deviceId << "): ZERO SETTING FAILED!";
          wStatus = 200;
          return 1;
      }

       wStatus++;
       return 1;

    case 2:
        // Upload Operating mode
        writeSDO(OD_6060_00,OD_6060_00_PROFILE_HOMING);
        wStatus++;
        return 5;

    case 3:
        if(!sdoRxTx.sdo_rx_ok) {
            qDebug() << QString("DEVICE (%1): ERROR WRITING OD %2.%3").arg(deviceId).arg(sdoRxTx.txSDO.getIndex(),1,16).arg(sdoRxTx.txSDO.getSubIndex());
            wStatus = 200;
            return 1;
        }

        wStatus++;
        return  1;

    case 4: // Read the Control Word
        readSDO(OD_6040_00);        
        wStatus++;
        return 5;

    case 5: // Get the control word
        if(!sdoRxTx.sdo_rx_ok) {
            qDebug() << QString("DEVICE (%1): ERROR READING OD %2.%3").arg(deviceId).arg(sdoRxTx.txSDO.getIndex(),1,16).arg(sdoRxTx.txSDO.getSubIndex());
            wStatus = 200;
            return 1;
        }

        // To the SwitchOn Status
        ctrlw = sdoRxTx.rxSDO.getVal();
        ctrlw &=~ OD_MASK(ZERO_SETTING_CTRL_INIT);
        ctrlw |= OD_VAL(ZERO_SETTING_CTRL_INIT);

        wStatus++;
        return 1;

    case 6:
        writeSDO(OD_6040_00, ctrlw);
        wStatus++;
        return 5;

    case 7:
        if(!sdoRxTx.sdo_rx_ok) {
            qDebug() << QString("DEVICE (%1): ERROR WRITING OD %2.%3").arg(deviceId).arg(sdoRxTx.txSDO.getIndex(),1,16).arg(sdoRxTx.txSDO.getSubIndex());
            wStatus = 200;
            return 1;
        }


        // Attivazione delle operazioni
        // Setta i soli bit per il cambio stato
        ctrlw &=~ OD_MASK(OD_6040_00_ENABLEOP);
        ctrlw |= OD_VAL(OD_6040_00_ENABLEOP);

        ctrlw &=~ OD_MASK(ZERO_SETTING_CTRL_INIT);
        ctrlw |= OD_VAL(ZERO_SETTING_CTRL_INIT);
        wStatus++;
        return 1;

   case 8:    
        writeSDO(OD_6040_00, ctrlw);
        wStatus++;
        return 5;

    case 9:
        if(!sdoRxTx.sdo_rx_ok) {
            qDebug() << QString("DEVICE (%1): ERROR WRITING OD %2.%3").arg(deviceId).arg(sdoRxTx.txSDO.getIndex(),1,16).arg(sdoRxTx.txSDO.getSubIndex());
            wStatus = 200;
            return 1;
        }


        return 0;

   case 200: // Error condition handling
        execCommand = _NO_COMMAND;
        return 0;
    }

    return 0;
}

ushort pd4Nanotec::pd4ZeroSettingLoop(){
    static uint ctrlw;
    static ushort memCtrl=0xFFFF;
    uint val;
    bool error;
    bool completed;
    ushort delay;

    // This activates the immediate stop motor
    if(safety.immediate_stop_command){
        safety.immediate_stop_command = false;
        qDebug() << QString("DEVICE (%1): IMMEDIATE STOP COMMAND RECEIVED").arg(deviceId);
        return 0;
    }

    // This causes a quick stop deceleration (set in the device vector)
    if(safety.quick_stop_command){
        delay = subActivateQuickStopCommand();
        if(delay) return delay;
        safety.quick_stop_command = false;
        qDebug() << QString("DEVICE (%1): QUICK STOP COMMAND RECEIVED").arg(deviceId);
        return 0;
    }

    switch(wStatus){
        case 0:
            qDebug() << QString("DEVICE (%1): ZERO SETTING STARTED").arg(deviceId);
            memCtrl=0xFFFF;
            wStatus++;
            return 1;

        case 1:// Read the Control Word
            readSDO(OD_6040_00);
            wStatus++;
            return 5;

        case 2: // Get the control word
            if(!sdoRxTx.sdo_rx_ok) {
                qDebug() << QString("DEVICE (%1): ERROR READING OD %2.%3").arg(deviceId).arg(sdoRxTx.txSDO.getIndex(),1,16).arg(sdoRxTx.txSDO.getSubIndex());
                return 0;
            }

            ctrlw = sdoRxTx.rxSDO.getVal();
            ctrlw &=~ OD_MASK(ZERO_SETTING_START);
            ctrlw |= OD_VAL(ZERO_SETTING_START);
            wStatus++;
            return 1;

        case 3: // Set the BIT4 of Control Word to start the sequence
            writeSDO(OD_6040_00, ctrlw);
            wStatus++;
            return 5;

        case 4:
            if(!sdoRxTx.sdo_rx_ok) {
                qDebug() << QString("DEVICE (%1): ERROR WRITING OD %2.%3").arg(deviceId).arg(sdoRxTx.txSDO.getIndex(),1,16).arg(sdoRxTx.txSDO.getSubIndex());
                return 0;
            }
            wStatus++;
            return 1;

        case 5: // Zero Setting Cycle loop ------------------------------------------------------------------

            // Read the Status Word to detect the command completion
            readSDO(OD_6041_00);
            wStatus++;
            return 5;

        case 6:
            if(!sdoRxTx.sdo_rx_ok) {
                qDebug() << QString("DEVICE (%1): ERROR READING OD %2.%3").arg(deviceId).arg(sdoRxTx.txSDO.getIndex(),1,16).arg(sdoRxTx.txSDO.getSubIndex());
                return 0;
            }


            // Repeats the status read until the following conditions are met.
            wStatus = 5;

            // Check the status register content
            val = sdoRxTx.rxSDO.getVal();
            val &= OD_MASK(ZERO_STAT_COMPLETED);
            if(val == memCtrl) return 100; // No status variation
            memCtrl = val;
            error = false;
            completed = false;

            switch(val){
            case OD_VAL(ZERO_STAT_PERFORMED):

                break;
            case OD_VAL(ZERO_STAT_INTERRUPTED):
                qDebug() << QString("DEVICE (%1): ERROR ZERO SETTING INTERRUPTED").arg(deviceId);
                error=true;
                break;
            case OD_VAL(ZERO_STAT_CONFIRMED):
                qDebug() << QString("DEVICE (%1): ZERO SETTING INITIATED").arg(deviceId);
                break;
            case OD_VAL(ZERO_STAT_COMPLETED):
                qDebug() << QString("DEVICE (%1): ZERO SETTING COMPLETED").arg(deviceId);
                completed = true;
                break;
            case OD_VAL(ZERO_STAT_ERROR_RUN):
                qDebug() << QString("DEVICE (%1): ERROR ZERO SETTING BUSY").arg(deviceId);
                error=true;
                break;
            case OD_VAL(ZERO_STAT_ERROR_IDL):
                qDebug() << QString("DEVICE (%1): ERROR ZERO SETTING MOTOR STOP").arg(deviceId);
                error=true;
                break;

            }

            if(error) {
                return 0;
            }

            if(completed) {
                zero_setting_ok = true;
                qDebug() << QString("DEVICE (%1): ZERO SETTING SUCCESSFULLY COMPLETED").arg(deviceId);
                return 0;
            }

            return 100;


    }
    return 0;
}

