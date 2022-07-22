#include "device_simulator.h"
#include <QTimer>

deviceSimulator::deviceSimulator(ushort canAddr, QString IP, uint port)
{
    connectionStatus=false;
    serverip = QHostAddress(IP);
    serverport = port;

    socket=0;
    this->canId = canAddr;
    configuredCanId = 0;

    // Status register instance
    statusRegisters.append(canRegister(_S_RESERVED));
    statusRegisters.append(canRegister(_S_REVISION));
    statusRegisters.append(canRegister(_S_SYSTEM));
    statusRegisters.append(canRegister(_S_ERROR));
    statusRegisters.append(canRegister(_S_COMMAND));


    // Parameter registers instance
    paramRegisters.append(canRegister(_P_RESERVED));

    // Data registers instance
    dataRegisters.append(canRegister(_D_RESERVED));

}
deviceSimulator::~deviceSimulator()
{
    if(socket)
    {
        socket->close();
        socket->deleteLater();
        socket=0;
        connectionStatus=false;
    }
}


int deviceSimulator::Start(void)
{
    if(connectionStatus){
        connectionStatus = false;
        socket->abort();
        socket->close();
        socket->connectToHost(serverip, serverport);
        return 0;
    }


    // Crea il socket
    socket = new QTcpSocket(this);
    connect(socket,SIGNAL(connected()),this,SLOT(socketConnected()),Qt::UniqueConnection);
    connect(socket,SIGNAL(errorOccurred(QAbstractSocket::SocketError)),this,SLOT(socketError(QAbstractSocket::SocketError)),Qt::UniqueConnection);
    connect(socket,SIGNAL(readyRead()), this,SLOT(socketRxData()),Qt::UniqueConnection);
    connect(socket,SIGNAL(disconnected()),this,SLOT(socketDisconnected()),Qt::UniqueConnection);
    socket->connectToHost(serverip, serverport);
    return 0;
}

void deviceSimulator::socketConnected()
{
    // Connessione avvenuta
    connectionStatus=true;
    socket->setSocketOption(QAbstractSocket::LowDelayOption,1);

    // Send the configuration command
    configClient(canId);
    QTimer::singleShot(20,this,SLOT(verifyClientConfiguration()));
}

void deviceSimulator::verifyClientConfiguration(void){
    if(configuredCanId == canId){
        canDriverReady(true);
        qDebug() << "CLIENT CONFIGURED:" << canId;
    }
    else {
        configClient(canId);
        QTimer::singleShot(20,this,SLOT(verifyClientConfiguration()));
    }
}

void deviceSimulator::socketDisconnected()
{
    connectionStatus=false;
    canDriverReady(false);
    socket->connectToHost(serverip, serverport);
}


void deviceSimulator::socketError(QAbstractSocket::SocketError error)
{
    // Invia la comunicazione tempestiva che la comunicazione è interrotta
    if(connectionStatus==true)
    {
        connectionStatus=false;
        canDriverReady(false);
    }

    if(error == QAbstractSocket::RemoteHostClosedError)
    {
        // Quest'errore viene generato PRIMA della chiusura
        // del socket (slot socketDisconnected())
        return ;
    }


    socket->abort();
    socket->close();
    socket->connectToHost(serverip, serverport);
    return;
}

void deviceSimulator::socketRxData()
{
    if(connectionStatus ==false) return;
    if(socket->bytesAvailable()==0)
    {
        return;
    }

    rxcanframe = socket->readAll();
    if(rxcanframe.size()==0)
    {
        return;
    }

    // Check the data protocol
    if(rxcanframe.size() < 3) return;
    if(rxcanframe.at(0)!='<') return;
    if(rxcanframe.at(2)!='>') return;

    // Heartbeat answer ..
    if(rxcanframe.at(1) == 'H'){
        return;
    }

    // Configuration
    if(rxcanframe.at(1) == 'C'){

        if(rxcanframe.size() < 5) return;
        configuredCanId = (rxcanframe.at(3) + 256 * rxcanframe.at(4)) & 0x7FF;
        return;
    }


    switch(decodeFrame()){
    case _PROTO_READ_STATUS:
        evaluateReadStatusFrame();
        break;
    case _PROTO_WRITE_PARAM:
        evaluateWriteParamFrame();
        break;
    case _PROTO_WRITE_DATA:
        evaluateWriteDataFrame();
        break;
    case _PROTO_STORE_PARAMS:
        evaluateStoreFrame();
        break;
    case _PROTO_COMMAND_EXEC:
        evaluateCommandFrame();
        break;
    case _PROTO_NOT_DEFINED:
        return;
        break;

    }
    return;

}

QByteArray deviceSimulator::formatReadStatus(uchar seq, uchar address){
    QByteArray frame;
    frame.append(seq);
    frame.append((uchar) _PROTO_READ_STATUS);
    frame.append((uchar) address);
    frame.append((uchar) 0);
    frame.append((uchar) 0);
    frame.append((uchar) 0);
    frame.append((uchar) 0);
    frame.append((uchar)seq ^ (uchar) _PROTO_READ_STATUS ^ (uchar) address);
    return frame;
}

deviceSimulator::_CanProtocolFrameCode deviceSimulator::decodeFrame(){
    if(rxcanframe.size() < 11) return _PROTO_NOT_DEFINED;
    if(rxcanframe.at(1) != 'D') return _PROTO_NOT_DEFINED;

    uchar crc = 0;
    for(int i=3; i<11; i++) crc ^= (uchar) rxcanframe.at(i);
    if(crc) return _PROTO_NOT_DEFINED;     // Wrong CRC

    return (_CanProtocolFrameCode) rxcanframe.at(4);
}

void deviceSimulator::evaluateReadStatusFrame(){

    if(rxcanframe.at(5) >= statusRegisters.size()) return ;

    uchar s0,s1,s2,s3;
    statusRegisters[rxcanframe.at(5)].get(&s0, &s1, &s2, &s3);

    QByteArray data = rxcanframe;
    data[6] = s0;
    data[7] = s1;
    data[8] = s2;
    data[9] = s3;

    uchar crc = 0;
    for(int i=3; i<10; i++) crc ^= (uchar) data.at(i);
    data[10] = crc;

    socket->write(data);
    socket->waitForBytesWritten(100);

}

void deviceSimulator::evaluateWriteParamFrame( ){
    if(rxcanframe.at(5) >= paramRegisters.size()) return ;

    paramRegisters[rxcanframe.at(5)].set(rxcanframe[6], rxcanframe[7], rxcanframe[8], rxcanframe[9]);

    QByteArray data = rxcanframe;

    uchar crc = 0;
    for(int i=3; i<10; i++) crc ^= (uchar) data.at(i);
    data[10] = crc;

    socket->write(data);
    socket->waitForBytesWritten(100);
}

void deviceSimulator::evaluateWriteDataFrame(){
    if(rxcanframe.at(5) >= dataRegisters.size()) return ;

    dataRegisters[rxcanframe.at(5)].set(rxcanframe[6], rxcanframe[7], rxcanframe[8], rxcanframe[9]);

    QByteArray data = rxcanframe;

    uchar crc = 0;
    for(int i=3; i<10; i++) crc ^= (uchar) data.at(i);
    data[10] = crc;

    socket->write(data);
    socket->waitForBytesWritten(100);
}

void deviceSimulator::evaluateStoreFrame(){

}

void deviceSimulator::evaluateCommandFrame(){

}



void deviceSimulator::txCanData(QByteArray frame)
{
    // Invia i dati ed attende di ricevere la risposta
    if(!socket) return;
    if(!connectionStatus) return;

    QByteArray data;
    data.append('<');
    data.append('D');
    data.append('>');
    data.append(frame);

    socket->write(data);
    socket->waitForBytesWritten(100);

}

void deviceSimulator::configClient(ushort canId)
{
    // Invia i dati ed attende di ricevere la risposta
    if(!socket) return;
    if(!connectionStatus) return;

    QByteArray data;
    data.append('<');
    data.append('C');
    data.append('>');
    data.append((uchar) canId);
    data.append((uchar) (canId>>8));


    socket->write(data);
    socket->waitForBytesWritten(100);

}


