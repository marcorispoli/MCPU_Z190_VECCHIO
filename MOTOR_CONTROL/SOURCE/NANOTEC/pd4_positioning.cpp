#include "application.h"
#include "pd4_dictionary.h"


bool pd4Nanotec::activatePositioning(int cAngle, uint cAcc, uint cDec, uint cSpeed, bool nanoj_start){
    canOpenDictionary od;

    if( execCommand != _NO_COMMAND){
        qDebug() << "DEVICE (" << deviceId << "): INVALID POSITIONING ACTIVATION. COMMAND EXECUTING !";
        return false;
    }
    if(CiAcurrentStatus != CiA402_SwitchedOn){
        qDebug() << "DEVICE (" << deviceId << "): INVALID POSITIONING ACTIVATION. WRONG CiA STATUS !";
        return false;
    }

    if(!zero_setting_ok ){
        qDebug() << "DEVICE (" << deviceId << "): INVALID POSITIONING ACTIVATION. MISSING ZERO SETTING !";
        return false;
    }


    positioningStr.target_ok = false;
    positioningStr.cAcc = cAcc;
    positioningStr.cDec = cDec;
    positioningStr.cSpeed = cSpeed;
    positioningStr.nanoj_start = nanoj_start;
    positioningStr.registers.clear();

    safety.immediate_stop_command = false;

    od.setOd(OD_6083_00, convert_cGRADsec_TO_ROT_min(cAcc)); // Set the Acceleration ratio
    positioningStr.registers.append(od);

    od.setOd(OD_6084_00, convert_cGRADsec_TO_ROT_min(cDec)); // Set the deceleration ratio
    positioningStr.registers.append(od);

    od.setOd(OD_6081_00, convert_cGRADsec_TO_ROT_min(cSpeed)); // Set the Speed ratio
    positioningStr.registers.append(od);

    od.setOd(OD_607A_00, convert_cGRAD_TO_EncoderUnit(cAngle)); // Set the target position
    positioningStr.registers.append(od);

    if(nanoj_start){
        od.setOd(OD_2300_00, 1); // Nanoj program activation
        positioningStr.registers.append(od);
    }

    execCommand = _POSITIONING_COMMAND;
    return true;
}


ushort pd4Nanotec::initPd4PositioningCommand(void){
    ushort delay;
    static bool success;
    static uint ctrlw;

    switch(wStatus){
    case 0:
        wStatus++;
        return 1;

    case 1:
      // Upload Positioning setting register loop
      delay = subRoutineWriteRegisterList(&positioningStr.registers, &success);
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


        wStatus++;
        return 1;

    case 6:
        // To the SwitchOn Status
        ctrlw = sdoRxTx.rxSDO.getVal();
        ctrlw &=~ OD_MASK(POSITION_SETTING_CTRL_INIT);
        ctrlw |= OD_VAL(POSITION_SETTING_CTRL_INIT);
        writeSDO(OD_6040_00, ctrlw);
        wStatus++;
        return 5;

    case 7:
        if(!sdoRxTx.sdo_rx_ok) {
            qDebug() << QString("DEVICE (%1): ERROR WRITING OD %2.%3").arg(deviceId).arg(sdoRxTx.txSDO.getIndex(),1,16).arg(sdoRxTx.txSDO.getSubIndex());
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
        if(!sdoRxTx.sdo_rx_ok) {
            qDebug() << QString("DEVICE (%1): ERROR WRITING OD %2.%3").arg(deviceId).arg(sdoRxTx.txSDO.getIndex(),1,16).arg(sdoRxTx.txSDO.getSubIndex());
            wStatus = 200;
            return 1;
        }

        // Nanoj activation if required
        if(!positioningStr.nanoj_start) return 0;
        wStatus++;
        return 1;

    case 10:
        readSDO(OD_2301_00); // Read the Nanoj Status register
        wStatus++;
        return 5;

    case 11:
        if(!sdoRxTx.sdo_rx_ok) {
            qDebug() << QString("DEVICE (%1): ERROR READING OD %2.%3").arg(deviceId).arg(sdoRxTx.txSDO.getIndex(),1,16).arg(sdoRxTx.txSDO.getSubIndex());
            wStatus = 200;
            return 1;
        }

        if(sdoRxTx.rxSDO.getVal() & 0x4 ){
             qDebug() << QString("DEVICE (%1): ERROR RUNNING THE NANOJ  PROGRAM").arg(deviceId);
             wStatus = 200;
             return 1;
        }

        qDebug() << QString("DEVICE (%1): NANOJ PROGRAM STARTED").arg(deviceId);
        nanojStr.nanoj_program_started = true;
        return 0;

   case 200: // Error condition handling
        execCommand = _NO_COMMAND;
        return 0;
    }

    return 0;
}

ushort pd4Nanotec::pd4PositioningLoop(){
    uint ctrlw;
    uint val;
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
            if(positioningStr.nanoj_start){
                qDebug() << QString("DEVICE (%1): POSITIONING SETTING WITH NANOJ STARTED").arg(deviceId);
                wStatus =5;
            }else{
                qDebug() << QString("DEVICE (%1): POSITIONING SETTING STARTED").arg(deviceId);
                wStatus++;
            }
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
            wStatus++;
            return 1;

        case 3: // Set the BIT4 of Control Word to start the sequence
            ctrlw = sdoRxTx.rxSDO.getVal();
            ctrlw &=~ OD_MASK(POSITION_SETTING_START);
            ctrlw |= OD_VAL(POSITION_SETTING_START);
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

        case 5: // Positioning Cycle loop ------------------------------------------------------------------

            delay = subReadPositionEncoder();// Read the Encoder position
            if(delay) return delay;
            wStatus++;
            return 5;

        case 6:// Read the Status Word to detect the command completion
            readSDO(OD_6041_00);
            wStatus++;
            return 5;

        case 7:
            if(!sdoRxTx.sdo_rx_ok) {
                qDebug() << QString("DEVICE (%1): ERROR READING OD %2.%3").arg(deviceId).arg(sdoRxTx.txSDO.getIndex(),1,16).arg(sdoRxTx.txSDO.getSubIndex());
                return 0;
            }

            // Check the status register content
            val = sdoRxTx.rxSDO.getVal();
            CiAcurrentStatus = getCiAStatus(&sdoRxTx.rxSDO);

            if(CiAcurrentStatus != CiA402_OperationEnabled){
                qDebug() << QString("DEVICE (%1): ERROR POSITIONING STATUS").arg(deviceId);
                return 0;
            }

            if((val &0x1400) == 0x1400){
                positioningStr.target_ok = true;
                qDebug() << QString("DEVICE (%1): POSITIONING SUCCESSFULLY COMPLETED").arg(deviceId);
                return 0;
            }

            // Repeats the status read until the following conditions are met.
            wStatus=5;
            return 100;


    }
    return 0;
}

