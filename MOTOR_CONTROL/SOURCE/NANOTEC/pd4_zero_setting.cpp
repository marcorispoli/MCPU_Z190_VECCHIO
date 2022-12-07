#include "application.h"
#include "pd4_dictionary.h"


bool pd4Nanotec::activateZeroSetting(void){
    if( execCommand != _NO_COMMAND) return false;
    if(CiAcurrentStatus != CiA402_SwitchedOn) return false;

    execCommand = _ZERO_SETTING_COMMAND;
    return true;

}


/*
 * // reset di un eventuale richiesta inevasa di stop
    driver_stat.quickstop = false;

    // Caricamento registri di movimento
    if(trxUploadActivationContext(CONTEXT_TRX_ZEROSETTING,0)==false) return false;

    // Caricamento modo operativo
    _canopen_ObjectDictionary_t od={OD_6060_00,PD4_PROFILE_HOMING};
    if(canopenWriteSDO(&od, CANOPEN_TRX_CONTEXT)==false) return false;

    // Reset BIT 4 of the control word
    if(Pd4CiA402SetControlOD(ZERO_SETTING_CTRL_INIT,CANOPEN_TRX_CONTEXT,pStat)==false) return false;

    // Switch CiA status in Operation Enable to start the zero setting
    if(CiA402_SwitchedOn_To_OperationEnabled(ZERO_SETTING_CTRL_INIT,CANOPEN_TRX_CONTEXT,pStat)==false) return false;

    driver_stat.event_type = TRX_RUN;
    driver_stat.event_code = 0;
    driver_stat.event_data = 0;
    _EVSET(_EV0_TRX_EVENT);

 */
ushort pd4Nanotec::initPd4ZeroSettingCommand(void){
    ushort delay;
    static bool success;
    static uchar attempt;
    static ulong ctrlw;

    switch(wStatus){
    case 0:
        // Upload Zero Setting registers
        wSubStatus = 0;
        wStatus++;
        zero_setting_ok = false;
        return 1;

    case 1:
      // Upload Zero setting register loop
      delay = subRoutineUploadVector(zeroSettingVector, nullptr, &success);
      if(delay) return delay;

      if(!success){
          qDebug() << "DEVICE (" << deviceId << "): ZERO SETTING FAILED!";
          wStatus = 200;
          return 1;
      }

       attempt = 10;
       wStatus++;
       return 1;

    case 2:
        // Upload Operating mode
        writeSDO(OD_6060_00,OD_6060_00_PROFILE_HOMING);
        wStatus++;
        return 5;

    case 3:
        if((!sdo_received) ||(sdo_error)){
            if(attempt == 0) {
                qDebug() << QString("DEVICE (%1): ERROR WRITING OD %2.%3").arg(deviceId).arg(txSDO.getIndex(),1,16).arg(txSDO.getSubIndex());
                wStatus = 200;
                return 1;
            }
            attempt--;
            wStatus--;
            return 100;
        }
        wStatus++;
        attempt = 10;
        return  1;

    case 4: // Read the Control Word
        readSDO(OD_6040_00);        
        wStatus++;
        return 5;

    case 5: // Get the control word
        if((! sdo_received) ||(sdo_error)){
            if(attempt == 0) {
                qDebug() << QString("DEVICE (%1): ERROR READING OD %2.%3").arg(deviceId).arg(txSDO.getIndex(),1,16).arg(txSDO.getSubIndex());
                wStatus = 200;
                return 1;
            }
            attempt--;
            wStatus--;
            return 100;
        }
        // To the SwitchOn Status
        ctrlw = rxSDO.getVal();
        ctrlw &=~ OD_MASK(ZERO_SETTING_CTRL_INIT);
        ctrlw |= OD_VAL(ZERO_SETTING_CTRL_INIT);

        wStatus++;
        attempt = 10;
        return 1;

    case 6:
        writeSDO(OD_6040_00, ctrlw);
        wStatus++;
        return 5;

    case 7:
        if((! sdo_received) ||(sdo_error)){
            if(attempt == 0) {
                qDebug() << QString("DEVICE (%1): ERROR WRITING OD %2.%3").arg(deviceId).arg(txSDO.getIndex(),1,16).arg(txSDO.getSubIndex());
                wStatus = 200;
                return 1;
            }
            attempt--;
            wStatus--;
            return 100;
        }

        // Attivazione delle operazioni
        // Setta i soli bit per il cambio stato
        ctrlw &=~ OD_MASK(OD_6040_00_ENABLEOP);
        ctrlw |= OD_VAL(OD_6040_00_ENABLEOP);

        ctrlw &=~ OD_MASK(ZERO_SETTING_CTRL_INIT);
        ctrlw |= OD_VAL(ZERO_SETTING_CTRL_INIT);
        wStatus++;
        attempt = 10;
        return 1;

   case 8:    
        writeSDO(OD_6040_00, ctrlw);
        wStatus++;
        return 5;

    case 9:
        if((! sdo_received) ||(sdo_error)){
            if(attempt == 0) {
                qDebug() << QString("DEVICE (%1): ERROR WRITING OD %2.%3").arg(deviceId).arg(txSDO.getIndex(),1,16).arg(txSDO.getSubIndex());
                wStatus = 200;
                return 1;
            }
            attempt--;
            wStatus--;
            return 100;
        }

        return 0;

   case 200: // Error condition handling
        execCommand = _NO_COMMAND;
        return 0;
    }

    return 0;
}

ushort pd4Nanotec::pd4ZeroSettingLoop(){
    static uchar attempt;
    static ulong ctrlw;
    static ushort memCtrl=0xFFFF;
    ulong val;
    bool error;
    bool completed;

    switch(wStatus){
        case 0:
            qDebug() << QString("DEVICE (%1): ZERO SETTING STARTED").arg(deviceId);
            memCtrl=0xFFFF;
            wStatus++;
            attempt = 10;
            return 1;

        case 1:// Read the Control Word
            readSDO(OD_6040_00);
            wStatus++;
            return 5;

        case 2: // Get the control word
            if((! sdo_received) ||(sdo_error)){
                if(attempt == 0) {
                    qDebug() << QString("DEVICE (%1): ERROR READING OD %2.%3").arg(deviceId).arg(txSDO.getIndex(),1,16).arg(txSDO.getSubIndex());
                    return 0;
                }
                attempt--;
                wStatus--;
                return 100;
            }

            ctrlw = rxSDO.getVal();
            ctrlw &=~ OD_MASK(ZERO_SETTING_START);
            ctrlw |= OD_VAL(ZERO_SETTING_START);
            wStatus++;
            attempt = 10;
            return 1;

        case 3: // Set the BIT4 of Control Word to start the sequence
            writeSDO(OD_6040_00, ctrlw);
            wStatus++;
            return 5;

        case 4:
            if((! sdo_received) ||(sdo_error)){
                if(attempt == 0) {
                    qDebug() << QString("DEVICE (%1): ERROR WRITING OD %2.%3").arg(deviceId).arg(txSDO.getIndex(),1,16).arg(txSDO.getSubIndex());
                    return 0;
                }
                attempt--;
                wStatus--;
                return 100;
            }

            wStatus++;
            attempt = 10;
            return 1;

        case 5:// Read the Status Word to detect the command completion
            readSDO(OD_6041_00);
            wStatus++;
            return 5;

        case 6:
            if((! sdo_received) ||(sdo_error)){
                if(attempt == 0) {
                    qDebug() << QString("DEVICE (%1): ERROR READING OD %2.%3").arg(deviceId).arg(txSDO.getIndex(),1,16).arg(txSDO.getSubIndex());
                    return 0;
                }
                attempt--;
                wStatus--;
                return 100;
            }

            // Repeats the status read until the following conditions are met.
            wStatus--;
            attempt = 10;

            // Check the status register content
            val = rxSDO.getVal();
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

