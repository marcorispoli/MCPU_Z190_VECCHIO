#include "device.h"
#include <QApplication>



Device::Device()
{
    can = new canProtocol(CANID);
    deviceProcessSeq = 0;
    deviceProcessTimer = 0;
    assignedCanId = 0;
    dataSequence = 0;
    currentCommandExecution = _CMD_NONE;
    commandAck = false;
}

void  Device::timerEvent(QTimerEvent* ev)
{
    if(ev->timerId() == deviceProcessTimer)
    {        
        killTimer(deviceProcessTimer);
        deviceProcessTimer = 0;

        // Check if the action as been executed: select the next sequence
        if(commandAck) deviceProcessSeq++;
        commandAck = false;

        switch(deviceExecutionProcess){
        case _DEV_INIT:deviceProcessInitialization();break;
        case _DEV_IDLE:deviceProcessIdle();break;
        }
    }

}

void  Device::deviceProcessInitialization(void){
    QByteArray frame;
    uchar b0,b1,b2,b3;

    switch(deviceProcessSeq){
    case 0:
        sendConfig(); // Setup CanId to the Can Driver        
        break;
    case 1:
        readStatus(canProtocol::_S_REVISION);
        break;
    case 2:
        can->statusRegisters[canProtocol::_S_REVISION].get(&b0, &b1, &b2, &b3);
        qDebug() << "REVISION: " << b0 << b1 << b2 << b3;
        commandAck = true;
        break;
    default:
        deviceExecutionProcess = _DEV_IDLE;
        deviceProcessSeq = 0;
        commandAck = false;
        break;
    }

    deviceProcessTimer = startTimer(20);
}

void  Device::deviceProcessIdle(void){


    switch(deviceProcessSeq){
    case 0:
        commandAck = true;
        break;
    case 1:
        commandAck = true;
        break;
    case 2:
        commandAck = true;
        break;
    case 3:
        commandAck = true;
        break;
    case 4:
        commandAck = true;
        break;
    case 5:
        commandAck = true;
        break;

    default:
        deviceProcessSeq = 0;
        commandAck = false;
        break;
    }
    deviceProcessTimer = startTimer(20);
}

void Device::canConnection(bool stat){

    if(stat){
        qDebug() << "CONNECTED";
        deviceExecutionProcess = _DEV_INIT;
        deviceProcessSeq = 0;
        commandAck = false;
        if(deviceProcessTimer) killTimer(deviceProcessTimer);
        deviceProcessTimer = startTimer(20);
    }else{

        // Signals the disconnection to the Client
    }
}


void Device::rxFromCanDriver(QByteArray data){

    // No command answer is waiting
    if(currentCommandExecution == _CMD_NONE ) return;

    // Check the data protocol
    if(data.size() < 3) return;
    if(data.at(0)!='<') return;
    if(data.at(2)!='>') return;

    // Heartbeat answer ..
    if(data.at(1) == 'H'){
        return;
    }

    // Configuration
    if(data.at(1) == 'C'){
        if(currentCommandExecution != _CMD_CONFIG ) return;

        if(data.size() < 5) return;
        assignedCanId = (data.at(3) + 256 * data.at(4)) & 0x7FF;
        if(assignedCanId == CANID){
            commandAck = true;
            qDebug() << "REGISTERED";
        }

        if(deviceProcessTimer) killTimer(deviceProcessTimer);
        deviceProcessTimer = startTimer(0);

        return;
    }

    // Can Data
    if(data.at(1) == 'D'){
        if(data.size() < 11) return;
        QByteArray frame = data.right(8);

        switch(can->decodeFrame(dataSequence, requestedRegAddress, frame)){
        case canProtocol::_PROTO_READ_STATUS:
            if(currentCommandExecution != _CMD_READ_STATUS_REGISTER ) return;
            if(can->evaluateReadStatusFrame(frame)) commandAck = true;
            break;
        case canProtocol::_PROTO_WRITE_PARAM:
            if(currentCommandExecution != _CMD_WRITE_PARAM_REGISTER ) return;
            if(can->evaluateWriteParamFrame(frame)) commandAck = true;
            break;
        case canProtocol::_PROTO_WRITE_DATA:
            if(currentCommandExecution != _CMD_WRITE_DATA_REGISTER ) return;
            if(can->evaluateWriteDataFrame(frame)) commandAck = true;
            break;
        case canProtocol::_PROTO_STORE_PARAMS:
            if(currentCommandExecution != _CMD_STORE_PARAMS_REGISTER ) return;
            if(can->evaluateStoreFrame(frame)) commandAck = true;
            break;
        case canProtocol::_PROTO_COMMAND_EXEC:
            if(currentCommandExecution != _CMD_COMMAND_EXEC ) return;
            if(can->evaluateCommandFrame(frame)) commandAck = true;
            break;
        case canProtocol::_PROTO_NOT_DEFINED:
            return;
            break;

        }

        if(deviceProcessTimer) killTimer(deviceProcessTimer);
        deviceProcessTimer = startTimer(0);
        return;
    }
}

void Device::sendConfig(void){
    QByteArray frame;
    frame.append('<');
    frame.append('C');
    frame.append('>');
    frame.append((uchar) CANID);
    frame.append((uchar) (CANID >> 8));

    currentCommandExecution = _CMD_CONFIG;
    commandAck = false;
    emit txToCanDriver(frame);
    return;
}

void Device::readStatus(uchar address){
    QByteArray frame;
    frame.append('<');
    frame.append('D');
    frame.append('>');

    dataSequence++;
    requestedRegAddress = address;
    frame.append(canProtocol::formatReadStatus(dataSequence, address));
    currentCommandExecution = _CMD_READ_STATUS_REGISTER;
    commandAck = false;
    emit txToCanDriver(frame);
}

void Device::writeParam(uchar address, QByteArray val){

}

void Device::writeData(uchar address, QByteArray val){

}
void Device::sendCommand(uchar address, QByteArray val){

}
