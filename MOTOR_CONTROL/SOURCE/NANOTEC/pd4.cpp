#include "application.h"
#include "pd4_dictionary.h"


pd4Nanotec::pd4Nanotec(uchar ID)
{
    deviceId = ID;
    ready = false;

    // Connect the Can Driver canReady Signal    
    connect(CANCLIENT, SIGNAL(rxFromCan(ushort , QByteArray )), this, SLOT(rxFromCan(ushort , QByteArray )), Qt::QueuedConnection);
    connect(this,SIGNAL(txToCan(ushort , QByteArray )), CANCLIENT,SLOT(txToCanData(ushort , QByteArray )), Qt::QueuedConnection);

    // invalidate the status of the digital input
    digital_input_valid = false;

    // Safety Input Mask closed
    setSafetyDigitalInput(0,0);

    configVector = nullptr;
    initVector =nullptr;
    zeroSettingVector = nullptr;
    positionSettingVector = nullptr;

    sdoRxTx.sdo_rx_tx_pending = false;
    sdoRxTx.sdo_rx_ok = true;
    sdoRxTx.sdo_rxtx_completed = true;

    execCommand = _NO_COMMAND;
    CiAcurrentStatus = CiA402_Undefined;
    zero_setting_ok = false;

}
pd4Nanotec::~pd4Nanotec()
{

}



/**
 * @brief This function inits the canOpne activities
 */
void pd4Nanotec::run(void){


    sdoRxTx.sdo_rx_tx_pending = false;
    sdoRxTx.sdo_rx_ok = true;
    sdoRxTx.sdo_rxtx_completed = true;

    wStatus = 0;
    workflow = _GET_CIA_STATUS;

    QTimer::singleShot(0,this, SLOT(statusHandler()));
}


void pd4Nanotec::rxFromCan(ushort devId, QByteArray data){

    if((devId & 0xFF) != deviceId) return; // Invalid ID
    sdoRxTx.sdo_rxtx_completed = true;

    if(nanojStr.rxblock){
          if( data.at(0) != ((nanojStr.txBlock.at(0)& 0xF0) | 0x20) ){
              //qDebug() << QString("errore:%1 atteso:%2").arg((uchar) data.at(0),1,16).arg((uchar)(nanojStr.txBlock.at(0) | 0x20),1,16);
              return;
          }
          sdoRxTx.sdo_rx_ok = true;
          return;
    }


    // Update the status handler
    rxSDO.set(&data);
    if(txSDO.getIndex() != rxSDO.getIndex()){
        return;
    }
    if(txSDO.getSubIndex() != rxSDO.getSubIndex()) {
        return;
    }

    // The SDO has been successfully received
    if(rxSDO.isError()){
        qDebug() << rxSDO.printError();
        return;
    }

    sdoRxTx.sdo_rx_ok = true;
    return;
}

void pd4Nanotec::writeSDO(ushort index, uchar sub, canOpenDictionary::_ODDataType type, uint val){
    txSDO.setOd(index,sub,type,val);
    rxSDO.clear();

    T1START;
    emit txToCan(0x600 + deviceId, txSDO.get());

    // Activate the timeout handler, in case no answer should be received
    sdoRxTx.sdo_rx_ok = false;
    sdoRxTx.sdo_rxtx_completed = false;
    sdoRxTx.sdo_rx_tx_pending = true;
}

void pd4Nanotec::readSDO(ushort index, uchar sub, uchar type){
    txSDO.setOd(index,sub,canOpenDictionary::RD_COMMAND,0);
    rxSDO.clear();

    T1START;
    emit txToCan(0x600 + deviceId, txSDO.get());

    // Activate the timeout handler, in case no answer should be received
    sdoRxTx.sdo_rx_ok = false;
    sdoRxTx.sdo_rxtx_completed = false;
    sdoRxTx.sdo_rx_tx_pending = true;

}

void pd4Nanotec::resetNode(void){
    rxSDO.clear();

    QByteArray frame;
    frame.append(0x81);
    frame.append(deviceId);
    T1START;
    emit txToCan(0, frame);

    // Activate the timeout handler, in case no answer should be received
    sdoRxTx.sdo_rx_ok = true;
    sdoRxTx.sdo_rxtx_completed = true;
    sdoRxTx.sdo_rx_tx_pending = false;

}


void pd4Nanotec::sendAgainSDO(void){
    rxSDO.clear();

    T1START;
    emit txToCan(0x600 + deviceId, txSDO.get());

    // Activate the timeout handler, in case no answer should be received
    sdoRxTx.sdo_rx_ok = false;
    sdoRxTx.sdo_rxtx_completed = false;
    sdoRxTx.sdo_rx_tx_pending = true;

}

void pd4Nanotec::statusHandler(void){
    ushort delay;

    // Wait for the CANCLIENT READY CONDITION
    if(!CANCLIENT->isCanReady()){
        wStatus = 0;
        wSubStatus = 0;
        digital_input_valid = false;
        sdoRxTx.sdo_rx_tx_pending = false;
        nanojStr.rxblock = false;
        workflow = _GET_CIA_STATUS;
        deviceInitialized = false;
        CiAcurrentStatus = CiA402_Undefined;
        QTimer::singleShot(100,this, SLOT(statusHandler()));
        return;
    }

    // Wait any transaction completion
    if(sdoRxTx.sdo_rx_tx_pending){
        if(!sdoRxTx.sdo_rxtx_completed){
            if(sdoRxTx.tmo_attempt){
                sdoRxTx.tmo_attempt--;
                QTimer::singleShot(1,this, SLOT(statusHandler()));
                return;
            }
            sdoRxTx.sdo_rx_ok = false;
            sdoRxTx.sdo_rxtx_completed = true;
        }
        sdoRxTx.tmo_attempt = 100; // ms of waiting the reception

        // In case of unsuccess try again for a maximum time
        if((!sdoRxTx.sdo_rx_ok) && (sdoRxTx.sdo_attempt) && (!nanojStr.rxblock)){
             sdoRxTx.sdo_attempt--;
             sendAgainSDO();
             QTimer::singleShot(100,this, SLOT(statusHandler()));
             return;
        }

    }else{
        sdoRxTx.sdo_rx_ok = true;
        sdoRxTx.sdo_rxtx_completed = true;
        nanojStr.rxblock = false;
    }

    sdoRxTx.sdo_rx_tx_pending = false;
    nanojStr.rxblock = false;
    sdoRxTx.sdo_attempt = 10;
    sdoRxTx.tmo_attempt = 100;


    switch(workflow){
    case _DEVICE_INIT: // Device Initialization procedures
        if(wStatus == 0){
            qDebug() << "DEVICE ID(" << this->deviceId << ") INITIALIZATION";
            deviceInitialized = false;
            digital_input_valid = false;
        }
        delay = workflowInitCallback();
        if(delay==0) {
            deviceInitialized = true;
            workflow = _GET_CIA_STATUS;
            wStatus = 0;
            wSubStatus = 0;
            delay=100;
        }

        QTimer::singleShot(delay,this, SLOT(statusHandler()));
        return;

    case _UPLOAD_NANOJ:
        if(wStatus == 0){
            qDebug() << "DEVICE ID(" << this->deviceId << ") UPLOAD NANOJ PROGRAM";
        }
        delay = workflowUploadNanoj();
        if(delay==0) {
            workflow = _GET_CIA_STATUS;
            wStatus = 0;
            wSubStatus = 0;
            delay=100;
        }
        QTimer::singleShot(delay,this, SLOT(statusHandler()));
        return;

    case _GET_CIA_STATUS:
        switch(wStatus){
        case 0:
            readSDO(OD_6041_00); // Read the status
            wStatus++;
            QTimer::singleShot(5,this, SLOT(statusHandler()));
            return;
        case 1:
            if(!sdoRxTx.sdo_rx_ok){
                wStatus--;
                qDebug() << "DEVICE (" << deviceId << ") NOT RESPONDING";
                deviceInitialized = false;
                digital_input_valid = false;
                QTimer::singleShot(2000,this, SLOT(statusHandler()));
                return ;
            }

            if( getCiAStatus(&rxSDO) != CiAcurrentStatus ) {
                CiAcurrentStatus = getCiAStatus(&rxSDO);
                qDebug() << "DEVICE ID(" << this->deviceId << ") STATUS CHANGE IN:" << getCiAStatusString(CiAcurrentStatus);
            }

            workflow = _HANDLE_DEVICE_STATUS;

            // Initialize the status workflow for the given CiA status.
            wStatus = 0;
            wSubStatus = 0;
        }

        QTimer::singleShot(0,this, SLOT(statusHandler()));
        return;

    case _HANDLE_DEVICE_STATUS:


        switch(CiAcurrentStatus){

        case CiA402_NotReadyToSwitchOn:
            // This status is activated after the device reset
            // When the device resets the initialization shall be repeated
            deviceInitialized = false;
            delay =0;
            break;

        case CiA402_QuickStopActive:
        case CiA402_SwitchOnDisabled:
            delay = CiA402_SwitchOnDisabledCallback();
            break;
        case CiA402_ReadyToSwitchOn:
            delay = CiA402_ReadyToSwitchOnCallback();
            break;
        case CiA402_SwitchedOn:
            // In case the initialization has not present, it is activated
            if(!deviceInitialized){
                uploadNanojRequest = false;
                nanojUploaded = false;
                workflow = _DEVICE_INIT;
                wStatus = 0;
                wSubStatus = 0;
                QTimer::singleShot(1,this, SLOT(statusHandler()));
                return;
            }

            if(uploadNanojRequest){
                uploadNanojRequest = false;
                nanojUploaded = false;
                workflow = _UPLOAD_NANOJ;
                wStatus = 0;
                wSubStatus = 0;
                QTimer::singleShot(1,this, SLOT(statusHandler()));
                return;
            }

            delay = idleCallback();
            break;
        case CiA402_OperationEnabled: delay = CiA402_OperationEnabledCallback(); break;
        case CiA402_Fault:delay = CiA402_FaultCallback(); break;

        default:
            delay = 0;
        }

        if(delay==0) {
            workflow = _GET_CIA_STATUS;
            wStatus = 0;
            wSubStatus = 0;
            delay=100;
        }

        QTimer::singleShot(delay,this, SLOT(statusHandler()));
        return;


    }

    QTimer::singleShot(1,this, SLOT(statusHandler()));
}



QString pd4Nanotec::getErrorClass1001(uint val){
    uchar cval = (uchar) val;
    QString errstr = "";
    if(cval & OD_1001_00_GENERAL_ERROR) errstr += "[GENERAL]";
    if(cval & OD_1001_00_I_ERROR) errstr += "[I]";
    if(cval & OD_1001_00_VOLTAGE_ERROR) errstr += "[VOLTAGE]";
    if(cval & OD_1001_00_TEMP_ERROR) errstr += "[TEMP]";
    if(cval & OD_1001_00_COM_ERROR) errstr += "[COM]";
    if(cval & OD_1001_00_PROFILE_ERROR) errstr += "[PROFILE]";
    if(cval & OD_1001_00_RESERVED_ERROR) errstr += "[RESERVED]";
    if(cval & OD_1001_00_MANUFACT_ERROR) errstr += "[MANUFACTURER]";
    return errstr;
}

QString pd4Nanotec::getErrorClass1003(uint val){
    return getErrorClass1001 (val >> 16);
}

QString pd4Nanotec::getErrorCode1003(uint val){
    ushort sval = (ushort) (val & 0xFFFF);

    switch(sval){
        case 0x1000: return "General error";
        case 0x2300: return "Current at the controller output too large";
        case 0x3100: return "Overvoltage/undervoltage at controller input";
        case 0x4200: return "Temperature error within the controller";
        case 0x6010: return "Software reset (watchdog)";
        case 0x6100: return "Internal software error, generic";
        case 0x6320: return "Rated current must be set (203Bh:01h)";
        case 0x7121: return "Motor blocked";
        case 0x7305: return "Incremental encoder or Hall sensor faulty";
        case 0x7600: return "Nonvolatile memory full or corrupt; restart the controller for cleanup work";
        case 0x8000: return "Error during fieldbus monitoring";
        case 0x8130: return "CANopen only: Life Guard error or Heartbeat error";
        case 0x8200: return "CANopen only: Slave took too long to send PDO messages.";
        case 0x8210: return "CANopen only: PDO was not processed due to a length error";
        case 0x8220: return "CANopen only: PDO length exceeded";
        case 0x8611: return "Position monitoring error: Following error too large";
        case 0x8612: return "Position monitoring error: Limit switch and tolerance zone exceeded";
        case 0x9000: return "EtherCAT: Motor running while EtherCAT changes from OP -> SafeOp, PreOP, etc.";
    }
}

ushort pd4Nanotec::subRoutineUploadVector(_OD_InitVector* pVector, bool* changed, bool* uploadOk){
    static ushort i;

    switch(wSubStatus){
    case 0:
        if(pVector == nullptr) {
            if(changed) *changed = false;
            if(uploadOk) *uploadOk = true;
            wSubStatus = 0;
            return 0;
        }

        i=0;
        wSubStatus++;
        return 1;

    case 1:
        readSDO(pVector[i].index, pVector[i].subidx, pVector[i].type);
        wSubStatus++;
        return 5;

    case 2:
        if(!sdoRxTx.sdo_rx_ok){
            if(uploadOk) *uploadOk = false;
            qDebug() << QString("DEVICE (%1): ERROR READING OD %2.%3").arg(deviceId).arg(pVector[i].index,1,16).arg(pVector[i].subidx);
            wSubStatus = 0;
            return 0;
        }


        if(rxSDO.getVal() == rxSDO.formatVal(pVector[i].val)){
            wSubStatus = 5;
            return 1;
        }

       // qDebug() << QString("cambiato: %1.%2=%3 contro %4").arg(rxSDO.getIndex(),1,16).arg(rxSDO.getSubIndex()).arg(rxSDO.getVal(),1,16).arg(rxSDO.formatVal(pVector[i].val),1,16);

        if(changed) *changed = true;
        wSubStatus++;
        return 1;

    case 3:
        writeSDO(pVector[i].index, pVector[i].subidx, pVector[i].type, pVector[i].val);
       // qDebug() << QString("SCRIVE:0x%1.%2=%3").arg(pVector[i].index,1,16).arg(pVector[i].subidx).arg( pVector[i].val,1,16);
        wSubStatus++;
        return 5;

    case 4:
        if(!sdoRxTx.sdo_rx_ok){
            if(uploadOk) *uploadOk = false;
            qDebug() << QString("DEVICE (%1): ERROR WRITING OD %2.%3").arg(deviceId).arg(pVector[i].index,1,16).arg(pVector[i].subidx);
            wSubStatus = 0;
            return 0;
        }

        wSubStatus++;
        return 1;

    case 5:
        i++;
        if(pVector[i].type == 0){
            if(uploadOk) *uploadOk = true;
            wSubStatus = 0;
            return 0;
        }

        wSubStatus = 1;
        return 1;

    default:
        if(uploadOk) *uploadOk = false;
        qDebug() << QString("DEVICE (%1): INVALID wSubStatus = %2").arg(deviceId).arg(wSubStatus);
        wSubStatus = 0;
        return 0;

    }
}

ushort pd4Nanotec::subReadDigitalInputs(void){

    switch(wSubStatus){
    case 0:
        readSDO(OD_3240_05); // Reads the digital inputs
        wSubStatus++;
        return 5;

    case 1:
        if(!sdoRxTx.sdo_rx_ok) {
            qDebug() << QString("DEVICE (%1): ERROR READING OD %2.%3").arg(deviceId).arg(txSDO.getIndex(),1,16).arg(txSDO.getSubIndex());
            digital_input_valid = false;
            wSubStatus = 0;
            return 0;
        }

        digital_input_valid = true;
        digital_input_val =  rxSDO.getVal();
        wSubStatus = 0;
        return 0;
    }
}

ushort pd4Nanotec::subReadPositionEncoder(void){

    switch(wSubStatus){
    case 0:
        readSDO(OD_6064_00); // Reads the digital inputs
        wSubStatus++;
        return 5;

    case 1:
        if(!sdoRxTx.sdo_rx_ok) {
            qDebug() << QString("DEVICE (%1): ERROR READING OD %2.%3").arg(deviceId).arg(txSDO.getIndex(),1,16).arg(txSDO.getSubIndex());
            wSubStatus = 0;
            return 0;
        }

        position_encoder_val =  rxSDO.getVal();
        wSubStatus = 0;
        return 0;
    }
}

ushort pd4Nanotec::subActivateQuickStopCommand(void){
    ushort ctrlw;

    switch(wSubStatus){
    case 0:
        readSDO(OD_6040_00); // Reads the control word
        wSubStatus++;
        return 5;

    case 1:
        if(!sdoRxTx.sdo_rx_ok) {
            qDebug() << QString("DEVICE (%1): ERROR READING OD %2.%3").arg(deviceId).arg(txSDO.getIndex(),1,16).arg(txSDO.getSubIndex());
            wSubStatus = 0;
            return 0;
        }

        wSubStatus++;
        return 1;

    case 2:

        ctrlw = rxSDO.getVal();
        ctrlw &=~ OD_MASK(OD_6040_00_QUICKSTOP);
        ctrlw |= OD_VAL(OD_6040_00_QUICKSTOP);
        writeSDO(OD_6040_00, ctrlw);
        wSubStatus++;
        return 5;

    case 3:
        if(!sdoRxTx.sdo_rx_ok) {
            qDebug() << QString("DEVICE (%1): ERROR WRITING OD %2.%3").arg(deviceId).arg(txSDO.getIndex(),1,16).arg(txSDO.getSubIndex());
        }

        wSubStatus = 0;
        return 0;
    }
}
