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
    if(txSDO.index != rxSDO.index){
        sdo_error = true;
        return;
    }
    if(txSDO.subindex != rxSDO.subindex) {
        sdo_error = true;
        return;
    }

    // The SDO has been successfully received
    if(rxSDO.isError()){
        rxSDO.printError();
        sdo_error = true;
    }

    return;
}

void pd4Nanotec::writeSDO(ushort index, uchar sub, canOpenDictionary::_ODDataType type, ulong val){
    txSDO.index = index;
    txSDO.subindex = sub;
    txSDO.odType = type;
    txSDO.b[0] = (uchar) val; val = val >>8;
    txSDO.b[1] = (uchar) val; val = val >>8;
    txSDO.b[2] = (uchar) val; val = val >>8;
    txSDO.b[3] = (uchar) val;
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
    txSDO.index = index;
    txSDO.subindex = sub;
    txSDO.odType = canOpenDictionary::RD_COMMAND;
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
    static  _CiA402Status ciastat = CiA402_Undefined;

    // Wait for the CANCLIENT READY CONDITION
    if(!CANCLIENT->isCanReady()){
        wStatus = 0;
        wSubStatus = 0;
        workflow = _DEVICE_INIT;
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
        if(! sdo_received){
            readSDO(OD_6041_00); // Read the status
            QTimer::singleShot(5,this, SLOT(statusHandler()));
            return;
        }

        ODstatus = this->rxSDO;
        if( getCiAStatus(&ODstatus) != ciastat) {
            ciastat = getCiAStatus(&ODstatus);
            qDebug() << "DEVICE ID(" << this->deviceId << ") STATUS CHANGE IN:" << getCiAStatusString(ciastat);

            // Initialize the status workflow for the given CiA status.
            wStatus = 0;
            wSubStatus = 0;
        }

        workflow = _HANDLE_DEVICE_STATUS;

        // Not break here!!

    case _HANDLE_DEVICE_STATUS:

        switch(getCiAStatus(&ODstatus)){

        case CiA402_QuickStopActive:
        case CiA402_SwitchOnDisabled:
            delay = CiA402_SwitchOnDisabledCallback();
            break;
        case CiA402_ReadyToSwitchOn:
            delay = CiA402_ReadyToSwitchOnCallback();
            break;
        case CiA402_SwitchedOn: delay = idleCallback(); break;
        case CiA402_OperationEnabled: delay = runCallback(); break;
        case CiA402_Fault:delay = faultCallback(); break;

        default:
            delay = 0;
        }

        if(delay==0) {
            workflow = _GET_CIA_STATUS;
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
    ushort val;

    switch(wStatus){

    case 0: // Read the Control Word
        readSDO(OD_6040_00);
        wStatus++;
        return 5;

    case 1: // Get the control word
        if((! sdo_received) ||(sdo_error)){
            wStatus--;
            return 100;
        }       
        wStatus++;
        return 1;

    case 2:
         // To the Ready to SwitchOn Status
        val = rxSDO.b[0] + 256 * rxSDO.b[1];
        val &=~ OD_MASK(OD_6040_00_SHUTDOWN);
        val |= OD_VAL(OD_6040_00_SHUTDOWN);

        writeSDO(OD_6040_00, (ulong) val);
        wStatus++;
        return 5;

    case 3:
        if((! sdo_received) ||(sdo_error)){
            wStatus--;
            return 100;
        }

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
    ushort val;

    switch(wStatus){

    case 0: // Read the Control Word
        readSDO(OD_6040_00);
        wStatus++;
        return 5;

    case 1: // Get the control word
        if((! sdo_received) ||(sdo_error)){
            wStatus--;
            return 100;
        }
        wStatus++;
        return 1;

    case 2:
        // To the SwitchOn Status
        val = rxSDO.b[0] + 256 * rxSDO.b[1];
        val &=~ OD_MASK(OD_6040_00_SWITCHON);
        val |= OD_VAL(OD_6040_00_SWITCHON);

        writeSDO(OD_6040_00, (ulong) val);
        wStatus++;
        return 5;

    case 3:
        if((! sdo_received) ||(sdo_error)){
            wStatus--;
            return 100;
        }

        return 0;

    }

    return 0;
}

/**
 * Procedura di inizializzazione del motore TRX
 * @return
 */
ushort pd4Nanotec::initCallback(void){
    static ushort i;
    static bool changed = false;

    switch(wStatus){
        case 0:
            if(initVector == nullptr){
                qDebug() << "INIT DEVICE (" << deviceId << "): COMPLETED";
                deviceInitialized=true;
                return 0;
            }

            i=0;
            changed = false;
            wStatus = 100;
            return 1;
        break;

    case 100:
        readSDO(initVector[i].index, initVector[i].subidx, initVector[i].type);
        wStatus++;
        return 5;

    case 101:
        if(rxSDO.getVal() == rxSDO.formatVal(initVector[i].val)){
            wStatus = 104;
            return 1;
        }
        qDebug() << QString("%1 %2 %3").arg(rxSDO.index,1,16).arg(rxSDO.getVal(),1,16).arg(rxSDO.formatVal(initVector[i].val),1,16);
        changed = true;
        wStatus++;
        return 1;

    case 102:
        writeSDO(initVector[i].index, initVector[i].subidx, initVector[i].type, initVector[i].val);
        wStatus++;
        return 5;

    case 103:
        if((!sdo_received) ||(sdo_error)){
            wStatus--;
            return 100;
        }
        wStatus++;
        return 1;

    case 104:
        i++;
        if(initVector[i].type == 0){
            wStatus++;
            return 1;
        }

        wStatus = 100;
        return 1;

    case 105:
        if(!changed){
            qDebug() << "INIT DEVICE (" << deviceId << "): COMPLETED";
            deviceInitialized=true;
            return 0;
        }else qDebug() << "INIT DEVICE (" << deviceId << "): STORING DATA";


        wStatus++;
        return 1;

    case 106:
        // Stores the parameters
        writeSDO(OD_1010_01, OD_SAVE_CODE);
        wStatus++;
        return 1000;

    case 107:
        readSDO(OD_1010_01);
        wStatus++;
        return 5;

    case 108:
        if((!sdo_received) ||(sdo_error)){
            wStatus--;
            return 100;
        }

        if(rxSDO.getVal() != 1){
            wStatus--;
            return 100;
        }

        wStatus++;
        return 1;

    case 109:
         qDebug() << "INIT DEVICE (" << deviceId << "): STORE DATA COMPLETED";
         deviceInitialized=true;
         return 0;
    }

    return 0;
}

ushort trxModule::faultCallback(void){
    static ulong err_class = 0;
    static ulong err_code = 0;
    static ulong ctrlw;
    ulong uval;

    switch(wStatus){
    case 0:
        // Get the error class
        readSDO(OD_1001_00);
        wStatus++;
        return 5;

    case 1:
        if((!sdo_received) ||(sdo_error)){
            wStatus--;
            return 100;
        }

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
    case 2:
        // Get the error code
        readSDO(OD_1003_01);
        wStatus++;
        return 5;

    case 3:
        if((!sdo_received) ||(sdo_error)){
            wStatus--;
            return 100;
        }

        uval = rxSDO.getVal();
        if(uval != err_code) qDebug() << "DEVICE (" << deviceId << ") ERROR CODE:" << getErrorCode1003(uval);
        err_code = uval;
        wStatus++;
        return 1;

    case 4:
        wStatus = 0;
        return 500;

   case 100:
        readSDO(OD_6040_00);
        wStatus++;
        return 5;
   case 101:
        if((!sdo_received) ||(sdo_error)){
            wStatus--;
            return 100;
        }
        wStatus++;
        return 1;

   case 102:
        rxSDO.b[0] |= 0x80;
        ctrlw = rxSDO.getVal();
        writeSDO(OD_6040_00,ctrlw);
        wStatus++;
        return 5;

    case 103:
        if((!sdo_received) ||(sdo_error)){
            wStatus=100;
            return 100;
        }
        wStatus++;
        return 1;
    case 104:
        ctrlw &=~ 0x80;
        writeSDO(OD_6040_00,ctrlw);
        wStatus++;
        return 5;

    case 105:
        if((!sdo_received) ||(sdo_error)){
            wStatus=100;
            return 100;
        }

        return 0;

    }
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
