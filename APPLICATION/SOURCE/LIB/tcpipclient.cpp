#include "tcpipclient.h"

TcpIpClient::TcpIpClient():QTcpServer()
{
    connectionStatus=false;
    socket=0;
}
TcpIpClient::~TcpIpClient()
{
    if(socket)
    {
        socket->close();
        socket->deleteLater();
        socket=0;
        connectionStatus=false;
    }
}


int TcpIpClient::Start(QString remip, int remport)
{
    // Impostazione server remoto
    serverip = QHostAddress(remip);
    serverport = remport;

    // Crea il socket
    socket = new QTcpSocket(this);
    connect(socket,SIGNAL(connected()),this,SLOT(socketConnected()),Qt::UniqueConnection);
    connect(socket,SIGNAL(errorOccurred(QAbstractSocket::SocketError)),this,SLOT(socketError(QAbstractSocket::SocketError)),Qt::UniqueConnection);
    connect(socket,SIGNAL(readyRead()), this,SLOT(socketRxData()),Qt::UniqueConnection);
    connect(socket,SIGNAL(disconnected()),this,SLOT(socketDisconnected()),Qt::UniqueConnection);

    socket->connectToHost(serverip, serverport);
    return 0;
}


void TcpIpClient::socketConnected()
{
    // Connessione avvenuta
    connectionStatus=true;
    emit clientConnection(true);
    socket->setSocketOption(QAbstractSocket::LowDelayOption,1);


}


void TcpIpClient::socketDisconnected()
{
    socket->connectToHost(serverip, serverport);
}


void TcpIpClient::socketError(QAbstractSocket::SocketError error)
{
    // Invia la comunicazione tempestiva che la comunicazione è interrotta
    if(connectionStatus==true)
    {
        connectionStatus=false;
        emit clientConnection(false);
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


void TcpIpClient::socketRxData()
{
    if(connectionStatus ==false) return;
    if(socket->bytesAvailable()==0)
    {
        return;
    }
    QByteArray data = socket->readAll();
    if(data.size()==0)
    {
        return;
    }

    // Legge tutto il buffer ricevuto e lo passa a chi deve gestirlo
    emit rxData(data);//(socket->bytesAvailable()));

}

void TcpIpClient::txData(QByteArray data, long timeout)
{
    // Invia i dati ed attende di ricevere la risposta
    if(!socket) return;
    if(!connectionStatus) return;

    socket->write(data);
    socket->waitForBytesWritten(timeout);

}

