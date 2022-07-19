#include "device.h"


deviceClass::deviceClass(ushort CanId,  QObject *parent)
{
    this->canId = CanId;
    protocol = new canProtocol(CanId);

    // Open the connection to the Can Driver for simulators
    can = new canClient(CanId);
    connect(can,SIGNAL(rxCanFrame(QByteArray)), this, SLOT(rxFromCanDriver(QByteArray)), Qt::QueuedConnection);
    connect(this,SIGNAL(txToCanDriver(QByteArray)), can, SLOT(txCanFrameToClient(QByteArray)), Qt::QueuedConnection);
    connect(can,SIGNAL(clientConfigured(bool)), this, SLOT(clientConfigured(bool)), Qt::QueuedConnection);
}

void deviceClass::StartClient(void){
    can->Start("127.0.0.1", 10010);
}

void deviceClass::rxFromCanDriver(QByteArray frame){

        switch(protocol->decodeFrame(frame)){
        case canProtocol::_PROTO_READ_STATUS:
            emit txToCanDriver(protocol->evaluateReadStatusFrame(frame));
            break;
        case canProtocol::_PROTO_WRITE_PARAM:
            emit txToCanDriver(protocol->evaluateWriteParamFrame(frame));
            break;
        case canProtocol::_PROTO_WRITE_DATA:
            emit txToCanDriver(protocol->evaluateWriteDataFrame(frame));
            break;
        case canProtocol::_PROTO_STORE_PARAMS:
            emit txToCanDriver(protocol->evaluateStoreFrame(frame));
            break;
        case canProtocol::_PROTO_COMMAND_EXEC:
            emit txToCanDriver(protocol->evaluateCommandFrame(frame));
            break;
        case canProtocol::_PROTO_NOT_DEFINED:
            return;
            break;

        }
        return;

}

