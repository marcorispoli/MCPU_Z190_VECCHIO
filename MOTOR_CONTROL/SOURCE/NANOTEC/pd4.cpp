#include "application.h"
#include "pd4_dictionary.h"


pd4Nanotec::pd4Nanotec(uchar ID)
{
    deviceId = ID;
    ready = false;

    // Connect the Can Driver canReady Signal    
    connect(CANCLIENT, SIGNAL(rxFromCan(ushort , QByteArray )), this, SLOT(rxFromCan(ushort , QByteArray )), Qt::QueuedConnection);
    connect(this,SIGNAL(txToCan(ushort , QByteArray )), CANCLIENT,SLOT(txToCanData(ushort , QByteArray )), Qt::QueuedConnection);

    initVector =nullptr;
    zeroSettingVector = nullptr;
    positionSettingVector = nullptr;

    execCommand = _NO_COMMAND;
    CiAcurrentStatus = CiA402_Undefined;
    zero_setting_ok = false;

    tmo_timer = 0;
}
pd4Nanotec::~pd4Nanotec()
{

}

void pd4Nanotec::timerEvent(QTimerEvent* ev)
{
    // Timer for the Send/Receive
    if(ev->timerId() == tmo_timer)
    {
        sdo_timeout = true;
        killTimer((tmo_timer));
        tmo_timer = 0;
        return;
    }


}

/**
 * @brief This function inits the canOpne activities
 */
void pd4Nanotec::run(void){
    check_status = true;
    sdo_received = false;
    sdo_error = false;
    sdo_timeout = true; // Force the initial read

    wStatus = 0;
    wSubStatus = 0;
    workflow = _DEVICE_INIT;

    QTimer::singleShot(0,this, SLOT(statusHandler()));
}


void pd4Nanotec::rxFromCan(ushort devId, QByteArray data){

    if((devId & 0xFF) != deviceId) return; // Invalid ID
    sdo_received = true;
    if(tmo_timer) killTimer(tmo_timer);
    tmo_timer = 0;

    // Update the status handler
    rxSDO.set(&data);
    if(txSDO.getIndex() != rxSDO.getIndex()){
        sdo_error = true;
        return;
    }
    if(txSDO.getSubIndex() != rxSDO.getSubIndex()) {
        sdo_error = true;
        return;
    }

    // The SDO has been successfully received
    if(rxSDO.isError()){
        qDebug() << rxSDO.printError();
        sdo_error = true;
    }

    return;
}

void pd4Nanotec::writeSDO(ushort index, uchar sub, canOpenDictionary::_ODDataType type, ulong val){
    txSDO.setOd(index,sub,type,val);
    rxSDO.clear();

    T1START;
    emit txToCan(0x600 + deviceId, txSDO.get());

    // Timeout after 10ms
    if(tmo_timer) killTimer(tmo_timer);
    tmo_timer = startTimer(10);

    // Activate the timeout handler, in case no answer should be received
    sdo_received = false;
    sdo_timeout = false;
    sdo_error = false;
}

void pd4Nanotec::readSDO(ushort index, uchar sub, uchar type){
    txSDO.setOd(index,sub,canOpenDictionary::RD_COMMAND,0);
    rxSDO.clear();

    T1START;
    emit txToCan(0x600 + deviceId, txSDO.get());

    // Timeout after 10ms
    if(tmo_timer) killTimer(tmo_timer);
    tmo_timer = startTimer(10);

    // Activate the timeout handler, in case no answer should be received
    sdo_received = false;
    sdo_timeout = false;
    sdo_error = false;
}



void pd4Nanotec::statusHandler(void){
    ushort delay;
    static uchar attempt;

    // Wait for the CANCLIENT READY CONDITION
    if(!CANCLIENT->isCanReady()){
        wStatus = 0;
        wSubStatus = 0;
        workflow = _DEVICE_INIT;
        CiAcurrentStatus = CiA402_Undefined;
        QTimer::singleShot(100,this, SLOT(statusHandler()));
        return;
    }

    // Waiting of the tx/rx completion process
    if((!sdo_received) && (!sdo_timeout)){
        QTimer::singleShot(1,this, SLOT(statusHandler()));
        return;
    }

    switch(workflow){
    case _DEVICE_INIT: // Device Initialization procedures
        if(wStatus == 0){
            qDebug() << "DEVICE ID(" << this->deviceId << ") INITIALIZATION";
            deviceInitialized = false;
        }
        delay = initCallback();
        if(delay==0) {
            deviceInitialized = true;
            workflow = _GET_CIA_STATUS;
            wStatus = 0;
            wSubStatus = 0;
            sdo_received = false;
            sdo_error = false;
            sdo_timeout = true;
            if(tmo_timer) killTimer(tmo_timer);
            tmo_timer = 0;
            delay=100;
        }

        QTimer::singleShot(delay,this, SLOT(statusHandler()));
        return;

    case _GET_CIA_STATUS:
        switch(wStatus){
        case 0:
            attempt = 10;
            wStatus++;
        case 1:
            readSDO(OD_6041_00); // Read the status
            wStatus++;
            QTimer::singleShot(5,this, SLOT(statusHandler()));
            return;
        case 2:
            if((!sdo_received) ||(sdo_error)){
                wStatus--;
                if(attempt == 0) {
                    attempt = 10;
                    qDebug() << "DEVICE (" << deviceId << ") NOT RESPONDING";
                    deviceInitialized = false;
                    QTimer::singleShot(2000,this, SLOT(statusHandler()));
                    return ;
                }
                attempt--;
                QTimer::singleShot(100,this, SLOT(statusHandler()));
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
        // In case the initialization has not present, it is activated
        if(!deviceInitialized){
            workflow = _DEVICE_INIT;
            QTimer::singleShot(1,this, SLOT(statusHandler()));
            return;
        }

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
        case CiA402_SwitchedOn: delay = idleCallback(); break;
        case CiA402_OperationEnabled: delay = CiA402_OperationEnabledCallback(); break;
        case CiA402_Fault:delay = faultCallback(); break;

        default:
            delay = 0;
        }

        if(delay==0) {
            workflow = _GET_CIA_STATUS;
            wStatus = 0;
            wSubStatus = 0;
            sdo_received = false;
            sdo_error = false;
            sdo_timeout = true;
            if(tmo_timer) killTimer(tmo_timer);
            tmo_timer = 0;
            delay=100;
        }

        QTimer::singleShot(delay,this, SLOT(statusHandler()));
        return;


    }

    QTimer::singleShot(1,this, SLOT(statusHandler()));
}



QString pd4Nanotec::getErrorClass1001(ulong val){
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

QString pd4Nanotec::getErrorClass1003(ulong val){
    return getErrorClass1001 (val >> 16);
}

QString pd4Nanotec::getErrorCode1003(ulong val){
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
    static uchar attempt;

    switch(wSubStatus){
    case 0:
        if(pVector == nullptr) {
            if(changed) *changed = false;
            if(uploadOk) *uploadOk = true;
            return 0;
        }

        i=0;
        wSubStatus++;
        attempt = 10;
        return 1;

    case 1:
        readSDO(pVector[i].index, pVector[i].subidx, pVector[i].type);
        wSubStatus++;
        return 5;

    case 2:
        if((!sdo_received) ||(sdo_error)){
            if(attempt == 0) {
                if(uploadOk) *uploadOk = false;
                qDebug() << QString("DEVICE (%1): ERROR READING OD %2.%3").arg(deviceId).arg(pVector[i].index,1,16).arg(pVector[i].subidx);
                return 0;
            }
            attempt--;
            wSubStatus--;
            return 100;
        }

        if(rxSDO.getVal() == rxSDO.formatVal(initVector[i].val)){
            wSubStatus = 5;
            return 1;
        }

        if(changed) *changed = true;
        wSubStatus++;
        attempt = 10;
        return 1;

    case 3:
        writeSDO(pVector[i].index, pVector[i].subidx, pVector[i].type, pVector[i].val);
        wSubStatus++;
        return 5;

    case 4:
        if((!sdo_received) ||(sdo_error)){
            if(attempt == 0) {
                if(uploadOk) *uploadOk = false;
                qDebug() << QString("DEVICE (%1): ERROR WRITING OD %2.%3").arg(deviceId).arg(pVector[i].index,1,16).arg(pVector[i].subidx);
                return 0;
            }
            attempt--;
            wSubStatus--;
            return 100;
        }

        wSubStatus++;
        return 1;

    case 5:
        i++;
        if(pVector[i].type == 0){
            if(uploadOk) *uploadOk = true;
            return 0;
        }
        attempt = 10;
        wSubStatus = 1;
        return 1;

    default:
        if(uploadOk) *uploadOk = false;
        qDebug() << QString("DEVICE (%1): INVALID wSubStatus = %2").arg(deviceId).arg(wSubStatus);
        return 0;

    }
}
