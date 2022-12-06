#include "application.h"
#include <QTimer>

canClient::canClient(ushort can_mask, ushort can_address, QString IP, int PORT):QTcpServer()
{
    serverip = QHostAddress(IP);
    serverport = PORT;
    filter_mask = can_mask;
    filter_address = can_address;
    rx_filter_open = false;

    connectionStatus=false;
    socket=0;

}
canClient::~canClient()
{
    if(socket)
    {
        socket->close();
        socket->deleteLater();
        socket=0;
        connectionStatus=false;
    }
}


int canClient::ConnectToCanServer(void)
{
    if(connectionStatus){
        qDebug() << "ConnectToCanServer() command failed: The Can Server is already connected!";
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


void canClient::socketConnected()
{
    // Connessione avvenuta
    connectionStatus=true;
    rx_filter_open = false;
    //emit clientConnection(true);
    socket->setSocketOption(QAbstractSocket::LowDelayOption,1);

    // Open the Acceptance filter to the Can Server application
    setAcceptanceFilter();

}


void canClient::socketDisconnected()
{
    rx_filter_open = false;
    socket->connectToHost(serverip, serverport);

}


void canClient::socketError(QAbstractSocket::SocketError error)
{
    // Invia la comunicazione tempestiva che la comunicazione è interrotta
    if(connectionStatus==true)
    {
        connectionStatus=false;
        rx_filter_open = false;
    }

    if(error == QAbstractSocket::RemoteHostClosedError)
    {
        // Quest'errore viene generato PRIMA della chiusura
        // del socket (slot socketDisconnected())
        rx_filter_open = false;
        return ;
    }


    socket->abort();
    socket->close();
    socket->connectToHost(serverip, serverport);
    return;
}

ushort canClient::getItem(int* index, QByteArray* data, bool* data_ok){
    *data_ok = false;
    bool is_hex = false;
    QString val;

    for(; *index< data->size(); (*index)++) if(data->at(*index) != ' ') break; // Removes the spaces

    for(; *index< data->size(); (*index)++){
        if(data->at(*index) == ' '){
            if(!val.size()) return 0;
            *data_ok = true;
            if(is_hex) return val.toUShort(data_ok, 16);
            else return val.toUShort();
        }
        if((data->at(*index) == 'x') |(data->at(*index) == 'X')) is_hex = true;
        val.append(data->at(*index));
    }

    // Non è terminato con uno spazio: errore
    return 0;
}

void canClient::handleSocketFrame(QByteArray* data){

    QByteArray frame;
    bool is_register;
    bool is_valid;
    int i;
    bool data_ok;


    is_valid = false;
    for(i=0; i< data->size(); i++){
        if(data->at(i)== ' ') continue;
        if(data->at(i)== 'F') {
            is_register = true;
            is_valid = true;
            i++;
            break;
        }

        if(data->at(i)== 'D') {
            is_register = false;
            is_valid = true;
            i++;
            break;
        }
    }
    if(!is_valid) return;

    ushort mask, address;
    if(is_register){// Can Registering Frame: set the reception mask and address
        mask = getItem(&i, data, &data_ok);
        if(!data_ok) return;
        address = getItem(&i, data, &data_ok);
        if(!data_ok) return;
        if(mask != filter_mask) return;
        if(address != filter_address) return;
        qDebug() << QString("ACCEPTANCE FILTER OPEN TO: MASK=0x%1, ADDR=0x%2").arg(filter_mask,1,16).arg(filter_address,1,16);
        rx_filter_open = true;
        return;

    }else{


        frame.clear();
        ushort canid = getItem(&i, data, &data_ok);        
        if(!data_ok) return;

        ushort val;
        for(; (i< data->size()) && (frame.size() <= 8) ; i++){
            val = getItem(&i, data, &data_ok)&0xFF;
            if(data_ok) {
                frame.append((unsigned char) val);
            } else break;
        }

        // If a valid set of data has been identified they will be sent to the driver
        ushort devId = (canid & (~filter_mask) );
        if(frame.size()) {
            emit rxFromCan(devId,frame);
        }
    }
    return;
}

void canClient::socketRxData()
{
    if(connectionStatus ==false) return;
    if(socket->bytesAvailable()==0) return;
    QByteArray data = socket->readAll();
    QByteArray frame;

    // Identifies all the possible frames in the received stream
    for(int i=0; i<data.size(); i++){
        if(data.at(i) == '<') {
            frame.clear();
        }else if(data.at(i) == '>'){
            if(frame.size() > 4) {
                frame.append(' ');
                handleSocketFrame(&frame);
                frame.clear();
            }
        }else{
            frame.append(data.at(i));
        }
    }

}

void canClient::txToCanData(ushort canId, QByteArray data)
{
    // Invia i dati ed attende di ricevere la risposta
    if(!socket) return;
    if(!connectionStatus) return;


    QString frame = QString("<D %1 ").arg(canId);
    for(int i=0; i<data.size(); i++) frame.append(QString("%1 ").arg((unsigned char) data.at(i)));
    frame.append(">");


    socket->write(frame.toLatin1());
    socket->waitForBytesWritten(5000);


}

void canClient::setAcceptanceFilter()
{
    // Invia i dati ed attende di ricevere la risposta
    if(!socket) return;
    if(!connectionStatus) return;
    if(rx_filter_open) return;

    QString frame = QString("<F %1 %2>").arg(filter_mask).arg(filter_address);
    socket->write(frame.toLatin1());
    socket->waitForBytesWritten(5000);

    QTimer::singleShot(50,this, SLOT(setAcceptanceFilter()));
}
