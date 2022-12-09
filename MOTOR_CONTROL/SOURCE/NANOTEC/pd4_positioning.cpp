#include "application.h"
#include "pd4_dictionary.h"


bool pd4Nanotec::activatePositioning(int cAngle){
    if( execCommand != _NO_COMMAND) return false;
    if(CiAcurrentStatus != CiA402_SwitchedOn) return false;
    //if(!zero_setting_ok ) return false;
    if(positionSettingVector == nullptr) return false;

    int i=0;
    while(positionSettingVector[i].type != 0){
        if(positionSettingVector[i].index == OD_IDX(OD_607A_00)){

            // Sets the target
            positionSettingVector[i].val = _cGRAD_TO_POS(cAngle);
            qDebug() << "TARGET:" << cAngle << " CONVERTED" << _cGRAD_TO_POS(cAngle);
            execCommand = _POSITIONING_COMMAND;
            return true;
        }
        i++;
    }
    return false;

}


ushort pd4Nanotec::initPd4PositioningCommand(void){
    ushort delay;
    static bool success;
    static ulong ctrlw;

    switch(wStatus){
    case 0:
        // Upload Zero Setting registers
        wSubStatus = 0;
        wStatus++;
        positioning_ok = false;
        return 1;

    case 1:
      // Upload Positioning setting register loop
      delay = subRoutineUploadVector(positionSettingVector, nullptr, &success);
      if(delay) return delay;

      if(!success){
          qDebug() << "DEVICE (" << deviceId << "): POSITIONING SETTING FAILED!";
          wStatus = 200;
          return 1;
      }

      wStatus++;
      return 1;

    case 2:
        // Upload Operating mode
        writeSDO(OD_6060_00,OD_6060_00_PROFILE_POSITIONING);
        wStatus++;
        return 5;

    case 3:
        if(!sdo_rx_ok) {
            qDebug() << QString("DEVICE (%1): ERROR WRITING OD %2.%3").arg(deviceId).arg(txSDO.getIndex(),1,16).arg(txSDO.getSubIndex());
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
        if(!sdo_rx_ok) {
            qDebug() << QString("DEVICE (%1): ERROR READING OD %2.%3").arg(deviceId).arg(txSDO.getIndex(),1,16).arg(txSDO.getSubIndex());
            wStatus = 200;
            return 1;
        }

        // To the SwitchOn Status
        ctrlw = rxSDO.getVal();
        ctrlw &=~ OD_MASK(POSITION_SETTING_CTRL_INIT);
        ctrlw |= OD_VAL(POSITION_SETTING_CTRL_INIT);
        wStatus++;
        return 1;

    case 6:
        writeSDO(OD_6040_00, ctrlw);
        wStatus++;
        return 5;

    case 7:
        if(!sdo_rx_ok) {
            qDebug() << QString("DEVICE (%1): ERROR WRITING OD %2.%3").arg(deviceId).arg(txSDO.getIndex(),1,16).arg(txSDO.getSubIndex());
            wStatus = 200;
            return 1;
        }

        // Setta i soli bit per il cambio stato
        ctrlw &=~ OD_MASK(OD_6040_00_ENABLEOP);
        ctrlw |= OD_VAL(OD_6040_00_ENABLEOP);

        ctrlw &=~ OD_MASK(POSITION_SETTING_CTRL_INIT);
        ctrlw |= OD_VAL(POSITION_SETTING_CTRL_INIT);

        wStatus++;
        return 1;

   case 8:
        // Attivazione delle operazioni   
        writeSDO(OD_6040_00, ctrlw);
        wStatus++;
        return 5;

    case 9:
        if(!sdo_rx_ok) {
            qDebug() << QString("DEVICE (%1): ERROR WRITING OD %2.%3").arg(deviceId).arg(txSDO.getIndex(),1,16).arg(txSDO.getSubIndex());
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

ushort pd4Nanotec::pd4PositioningLoop(){
    static ulong ctrlw;
    ulong val;
    static int encoder;

    switch(wStatus){
        case 0:
            qDebug() << QString("DEVICE (%1): POSITIONING SETTING STARTED").arg(deviceId);
            wStatus++;
            return 1;

        case 1:// Read the Control Word
            readSDO(OD_6040_00);
            wStatus++;
            return 5;

        case 2: // Get the control word
            if(!sdo_rx_ok) {
                qDebug() << QString("DEVICE (%1): ERROR READING OD %2.%3").arg(deviceId).arg(txSDO.getIndex(),1,16).arg(txSDO.getSubIndex());
                return 0;
            }

            ctrlw = rxSDO.getVal();
            ctrlw &=~ OD_MASK(POSITION_SETTING_START);
            ctrlw |= OD_VAL(POSITION_SETTING_START);
            wStatus++;
            return 1;

        case 3: // Set the BIT4 of Control Word to start the sequence
            writeSDO(OD_6040_00, ctrlw);
            wStatus++;
            return 5;

        case 4:
            if(!sdo_rx_ok) {
                qDebug() << QString("DEVICE (%1): ERROR WRITING OD %2.%3").arg(deviceId).arg(txSDO.getIndex(),1,16).arg(txSDO.getSubIndex());
                return 0;
            }

            wStatus++;
            return 1;

        case 5: // Read the Encoder position
            readSDO(OD_6064_00);
            wStatus++;
            return 5;

        case 6:
            if(!sdo_rx_ok) {
                qDebug() << QString("DEVICE (%1): ERROR READING OD %2.%3").arg(deviceId).arg(txSDO.getIndex(),1,16).arg(txSDO.getSubIndex());
                return 0;
            }

            qDebug() << (long) rxSDO.getVal() << _POS_TO_cGRAD((int) rxSDO.getVal());
            encoder = _POS_TO_cGRAD((int) rxSDO.getVal());

            wStatus++;
            return 1;

        case 7:// Read the Status Word to detect the command completion
            readSDO(OD_6041_00);
            wStatus++;
            return 5;

        case 8:
            if(!sdo_rx_ok) {
                qDebug() << QString("DEVICE (%1): ERROR READING OD %2.%3").arg(deviceId).arg(txSDO.getIndex(),1,16).arg(txSDO.getSubIndex());
                return 0;
            }

            // Check the status register content
            val = rxSDO.getVal();
            CiAcurrentStatus = getCiAStatus(&rxSDO);

            if(CiAcurrentStatus != CiA402_OperationEnabled){
                qDebug() << QString("DEVICE (%1): ERROR POSITIONING STATUS").arg(deviceId);
                return 0;
            }

            if((val &0x1400) == 0x1400){
                positioning_ok = true;
                qDebug() << QString("DEVICE (%1): POSITIONING SUCCESSFULLY COMPLETED").arg(deviceId);
                return 0;
            }

            // Repeats the status read until the following conditions are met.
            wStatus=5;
            return 1000;


    }
    return 0;
}

