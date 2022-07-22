#include "power_service.h"
#include <QApplication>



powerService::powerService(ushort CanId, QString IP, uint port, uint commandPort):Device(CanId, IP, port)
{    
    // Activates the server for the command interface
    commandInterface = new deviceCommandInterface(IP,commandPort);
    initCommandInterface();
    commandInterface->CommandStart();


    deviceProcessSeq = 0;

    // Status register instance
    statusRegisters.append(canRegister(_S_BATTERY));

    // Parameter registers instance
    paramRegisters.append(canRegister(_P_DISABLE_POWER_ON_TIMER));
    paramRegisters.append(canRegister(_P_HARDWARE_POWER_OFF_TIMER));
    paramRegisters.append(canRegister(_P_KEEP_ALIVE_TIMER));
    paramRegisters.append(canRegister(_P_SOFT_POWER_OFF_TIMER));
    paramRegisters.append(canRegister(_P_LOW_BATT_LEVEL));
    paramRegisters.append(canRegister(_P_BODY_ROTATION_LOCK_TIMER));
    paramRegisters.append(canRegister(_P_PEDALBOARD_LOCK_TIMER));
    paramRegisters.append(canRegister(_P_MANUAL_BUTTON_LOCK_TIMER));
    paramRegisters.append(canRegister(_P_XRAY_BUTTON_LOCK_TIMER));

    // Data registers instance
    dataRegisters.append(canRegister(_D_OUTPUT));
}




void powerService::readStatus(uchar address){
    QByteArray frame;

    frame.append(formatReadStatus(deviceProcessSeq, address));
    if(!txCanData(frame)){
        QTimer::singleShot(10, this ,SLOT(executeProcess()));
    }
}


void powerService::canDriverReady(bool stat){
    if(stat == true){
        deviceProcessSeq = 0;
        deviceExecutionProcess = _DEVICE_EXEC_GET_STATUS;
        qDebug() << "INIT THE STATUS REGISTERS DOWNLOAD";
        QTimer::singleShot(0,this,SLOT(executeProcess()));
    }else{
        QList<QByteArray> answer;
        answer.clear();
        answer.append(EVENT_DISCONNECTION);
        commandInterface->sendToClient(0, answer);
    }
}

void  powerService::deviceProcessDownloadStatusRegisters(void){

    // Status register acquisition
    if(deviceProcessSeq >= statusRegisters.size()){

        qDebug() << "DOWNLOAD STATUS REGISTERS COMPLETED";


        // Changes to Idle management
        deviceExecutionProcess = _DEVICE_EXEC_IDLE;
        deviceProcessSeq = 0;
        QTimer::singleShot(0,this,SLOT(executeProcess()));
        return ;
    }

    readStatus(deviceProcessSeq);
    return;
}

void  powerService::deviceProcessIdle(void){

    switch(deviceExecutionProcess){
        default:
            deviceExecutionProcess = _DEVICE_EXEC_IDLE;
            deviceProcessSeq = 0;
            QTimer::singleShot(100,this,SLOT(executeProcess()));
            break;
    }
}

void  powerService::executeProcess(void){
    switch(deviceExecutionProcess){
    case _DEVICE_EXEC_GET_STATUS: deviceProcessDownloadStatusRegisters(); break;
    case _DEVICE_EXEC_IDLE: deviceProcessIdle(); break;
    default:
        qDebug() << "INVALID SEQUENCE ACTIVATED";
    }
}



void powerService::canTxRxCompleted(uchar seq, _CanTxErrors error){

    // deviceProcessSeq = seq;
    if(error == _CAN_NO_ERROR) deviceProcessSeq = seq + 1;
    executeProcess();
}

void powerService::targetDeviceReady(bool stat){

    if(stat){
        QList<QByteArray> answer;
        answer.clear();
        answer.append(EVENT_CONNECTION);
        answer.append(QString("%1").arg(maj).toLatin1());
        answer.append(QString("%1").arg(min).toLatin1());
        answer.append(QString("%1").arg(sub).toLatin1());
        commandInterface->sendToClient(0, answer);
    }else{
        QList<QByteArray> answer;
        answer.clear();
        answer.append(EVENT_DISCONNECTION);
        commandInterface->sendToClient(0, answer);
    }
}
