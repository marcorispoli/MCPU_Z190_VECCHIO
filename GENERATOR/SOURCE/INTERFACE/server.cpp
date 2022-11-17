#include "application.h"
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
    qDebug() << "GENERATOR DRIVER INTERFACE ADDRESS: IP=" << localip.toString() << ", PORT=" << localport ;
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
    connect(item,SIGNAL(receivedCommandSgn(ushort, QByteArray)), this, SLOT(receivedCommandSlot(ushort, QByteArray)),Qt::QueuedConnection);

    connect(item->socket,SIGNAL(disconnected()),item, SLOT(disconnected()),Qt::UniqueConnection);
    connect(this,SIGNAL(txFrame(QByteArray)),item, SLOT(socketTxData(QByteArray)),Qt::QueuedConnection);

    connect(item,SIGNAL(itemDisconnected(ushort )),this, SLOT(disconnected(ushort )),Qt::UniqueConnection);
    connect(item->socket,SIGNAL(errorOccurred(QAbstractSocket::SocketError)),item,SLOT(socketError(QAbstractSocket::SocketError)),Qt::UniqueConnection);
    item->id = this->idseq++;    

    EventStatus(1, true);

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



void SocketItem::socketRxData()
{
    QByteArray frame;
    QString stringa;

    if(socket->bytesAvailable()==0) return;
    QByteArray data = socket->readAll();

    qDebug() << data;

    bool init_find = true;
    QByteArray streaming = "";

    for(int i=0; i<data.size(); i++){
        if(init_find){
            if(data.at(i) == '<'){
                streaming.clear();
                init_find = false;
            }
        }else{
            if(data.at(i) == '>'){
                if(streaming.size()) emit receivedCommandSgn(this->id, streaming);
                streaming.clear();
                init_find = true;
            }else streaming.append(data.at(i));
        }

    }


}


void SocketItem::socketTxData(QByteArray data)
{
    this->socket->write(data);
    this->socket->waitForBytesWritten(100);
    return;

}




QList<QString> Server::getProtocolFrame(QByteArray* data){
    QList<QString> comando;

    bool init_find = true;
    QString stringa = "";

    for(int i=0; i<data->size(); i++){
        if(init_find){
            if(data->at(i) != ' '){
                stringa = data->at(i);
                init_find = false;
            }
        }else{
            if(data->at(i) == ' '){
                comando.append(stringa);
                init_find = true;
                stringa = "";
            }else stringa += data->at(i);
        }
    }
    if(stringa != "") comando.append(stringa);
    return comando;
}




void Server::sendAnswer(ushort id, QList<QString>* data){
    QByteArray buffer;
    buffer.append('<');
    buffer.append('A');
    buffer.append(' ');

    for(int i =0; i< data->size(); i++){
            buffer.append(data->at(i).toLatin1());
            buffer.append(' ');
    }
    buffer.append('>');
    qDebug() << buffer;

    buffer.append('\n');
    buffer.append('\r');

    // Sends only to the socket requesting the command
    for(int i=0; i< socketList.size(); i++){
        if(socketList[i]->id == id){
            socketList[i]->socket->write(buffer);
            socketList[i]->socket->waitForBytesWritten(100);
            return;
        }
    }
}

void Server::sendEvent(QList<QString>* data){
    QByteArray buffer;
    buffer.append('<');
    buffer.append('E');
    buffer.append(' ');

    for(int i =0; i< data->size(); i++){
            buffer.append(data->at(i).toLatin1());
            buffer.append(' ');
    }
    buffer.append('>');
    qDebug() << buffer;

    buffer.append('\n');
    buffer.append('\r');



    // Sends only to the socket requesting the command
    for(int i=0; i< socketList.size(); i++){
        socketList[i]->socket->write(buffer);
        socketList[i]->socket->waitForBytesWritten(100);
    }
}

void Server::receivedCommandSlot(ushort id, QByteArray data){
    QList<QString> command =  getProtocolFrame(&data);
    QList<QString> answer;


    if(command.size() < 3) return;
    if(command.at(0) != "E") return;

    if(command.at(2)        == "SetExposurePre")    answer = SetExposurePre(&command);
    else if(command.at(2)   == "SetExposurePulse")  answer = SetExposurePulse(&command);
    else if(command.at(2)   == "SetTomoConfig")     answer = SetTomoConfig(&command);
    else if(command.at(2)   == "StartExposure")     answer = StartExposure(&command);
    else if(command.at(2)   == "AbortExposure")     answer = AbortExposure(&command);
    else if(command.at(2)   == "GetPostExposure")   answer = GetPostExposure(&command);

    if(!answer.size()) return;
    sendAnswer(id, &answer);
}

