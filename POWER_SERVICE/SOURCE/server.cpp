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
    connect(item,SIGNAL(itemDisconnected(ushort )),this, SLOT(disconnected(ushort )),Qt::UniqueConnection);
    connect(item->socket,SIGNAL(errorOccurred(QAbstractSocket::SocketError)),item,SLOT(socketError(QAbstractSocket::SocketError)),Qt::UniqueConnection);

    item->id = this->idseq++;
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






