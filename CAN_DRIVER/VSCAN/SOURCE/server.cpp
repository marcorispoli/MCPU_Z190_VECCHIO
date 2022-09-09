#include "server.h"
#include <QStringConverter>



Server::Server(QString ipaddress, int port):QTcpServer()
{
    socketList.clear();
    localip = QHostAddress(ipaddress);
    localport = port;
    idseq=0;
}

Server::~Server()
{
    if(socketList.size()){
        for(int i = 0; i<socketList.size(); i++ ){
            if(socketList[i]->socket != nullptr) {
                socketList[i]->socket->close();
                socketList[i]->socket->deleteLater();
            }
        }
    }
}

bool Server::Start(void)
{
    qDebug() << "VSCAN DRIVER INTERFACE ADDRESS: IP=" << localip.toString() << ", PORT=" << localport ;
    if (!this->listen(localip,localport)) return false;
    return true;

}


void Server::incomingConnection(qintptr socketDescriptor)
{

    SocketItem* item = new SocketItem();

    item->socket = new QTcpSocket(this); // Create a new socket
    if(!item->socket->setSocketDescriptor(socketDescriptor))
    {
        delete item->socket;
        delete item;
        return;
    }

    item->socket->setSocketOption(QAbstractSocket::LowDelayOption,1);
    socketList.append(item);


    // Interface signal connection
    connect(item->socket,SIGNAL(readyRead()), item, SLOT(socketRxData()),Qt::UniqueConnection);
    connect(item->socket,SIGNAL(disconnected()),item, SLOT(disconnected()),Qt::UniqueConnection);
    connect(item,SIGNAL(sendToClient(QByteArray)),item, SLOT(socketTxData(QByteArray)),Qt::QueuedConnection);

    connect(item,SIGNAL(itemDisconnected(ushort )),this, SLOT(disconnected(ushort )),Qt::UniqueConnection);
    connect(item->socket,SIGNAL(errorOccurred(QAbstractSocket::SocketError)),item,SLOT(socketError(QAbstractSocket::SocketError)),Qt::UniqueConnection);
    connect(item,SIGNAL(sendToCan(ushort, QByteArray )),this, SLOT(sendToCanSlot(ushort, QByteArray )),Qt::UniqueConnection);

    item->id = this->idseq++;
    item->canId = 0;

    return;
 }


void SocketItem::disconnected(void){
    emit itemDisconnected(this->id);
}

void SocketItem::socketError(QAbstractSocket::SocketError error)
{
    emit itemDisconnected(this->id);
    return;
}


void Server::disconnected(ushort id)
{

    for(int i =0; i < socketList.size(); i++ ){
        if(socketList[i]->id == id){

            disconnect(socketList[i]);
            socketList[i]->socket->deleteLater();
            delete socketList[i];
            socketList.remove(i);
            return;
        }
    }

}

void Server::sendToCanSlot(ushort canId, QByteArray dataFrame)
{
    emit sendToCan(canId, dataFrame);
}



void SocketItem::socketRxData()
{
    QByteArray frame;
    QString stringa;

    if(socket->bytesAvailable()==0) return;
    QByteArray data = socket->readAll();


    if(data.size() < 3) return;
    if(data.at(0)!='<') return;
    if(data.at(2)!='>') return;

    if(data.at(1) == 'D'){
        if(this->canId == 0) return;
        frame.clear();
        for(int i=3; i< data.size(); i++) frame.append(data[i]);

        //QString stringa = QString("TO CAN: %1 %2 %3 %4 %5 %6 %7 %8").arg((uchar) frame[0]).arg((uchar) frame[1]).arg((uchar) frame[2]).arg((uchar) frame[3]).arg((uchar) frame[4]).arg((uchar) frame[5]).arg((uchar) frame[6]).arg((uchar) frame[7]);
        //qDebug() << stringa;

        emit sendToCan(this->canId,frame);
        return;
    }

    if(data.at(1) == 'C'){

        if(data.size() < 5) return;
        this->canId = (data.at(3) + 256 * data.at(4)) & 0x7FF;
        emit sendToClient(data);
        return;
    }

    if(data.at(1) == 'Q'){
        exit(0);
        return;
    }

}
void SocketItem::socketTxData(QByteArray data)
{
    this->socket->write(data);
    this->socket->waitForBytesWritten(100);
    return;

}



void Server::receivedCanFrame(ushort canId, QByteArray data){
    QByteArray frame;
    frame.append('<');
    frame.append('D');
    frame.append('>');

    for(int i=0; i< 8;i++){
        if(i >= data.size()) frame.append((uchar) 0);
        else frame.append(data[i]);
    }

    /*
    QString stringa = QString("FROM CAN: %1 %2 %3 %4 %5 %6 %7 %8").arg((uchar) frame[3]).arg((uchar) frame[4]).arg((uchar) frame[5]).arg((uchar) frame[6]).arg((uchar) frame[7]).arg((uchar) frame[8]).arg((uchar) frame[9]).arg((uchar) frame[10]);
    qDebug() << stringa;
    */

    for(int i =0; i< socketList.size(); i++){
        if((socketList[i]->canId == canId) || (socketList[i]->canId == 0)){
            if(socketList[i]->canId == 0){
                QString stringa = QString("CANID:%1, DATA: %2 %3 %4 %5 %6 %7 %8 %9").arg(canId).arg((uchar) frame[3]).arg((uchar) frame[4]).arg((uchar) frame[5]).arg((uchar) frame[6]).arg((uchar) frame[7]).arg((uchar) frame[8]).arg((uchar) frame[9]).arg((uchar) frame[10]);
                socketList[i]->socket->write(stringa.toLatin1());
            }else   socketList[i]->socket->write(frame);
            socketList[i]->socket->waitForBytesWritten(100);
        }
    }

}


