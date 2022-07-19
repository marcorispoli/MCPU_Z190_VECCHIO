#include "can_client.h"
#include <QApplication>
#include <QTimer>


canClient::canClient(ushort canId):QTcpServer()
{
    connectionStatus=false;
    socket=0;
    this->canId = canId;
    configuredCanId = 0;
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


int canClient::Start(QString remip, int remport)
{
    if(connectionStatus){
        connectionStatus = false;
        serverip = QHostAddress(remip);
        serverport = remport;
        socket->abort();
        socket->close();
        socket->connectToHost(serverip, serverport);
        return 0;
    }

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


void canClient::socketConnected()
{
    // Connessione avvenuta
    connectionStatus=true;
    socket->setSocketOption(QAbstractSocket::LowDelayOption,1);

    // Send the configuration command
    configClient(canId);
    QTimer::singleShot(20,this,SLOT(verifyClientConfiguration()));
}

void canClient::verifyClientConfiguration(void){
    if(configuredCanId == canId){
        emit clientConfigured(true);
        qDebug() << "CLIENT CONFIGURED:" << canId;
    }
    else {
        configClient(canId);
        QTimer::singleShot(20,this,SLOT(verifyClientConfiguration()));
    }
}

void canClient::socketDisconnected()
{
    connectionStatus=false;
    emit clientConfigured(false);
    socket->connectToHost(serverip, serverport);
}


void canClient::socketError(QAbstractSocket::SocketError error)
{
    // Invia la comunicazione tempestiva che la comunicazione è interrotta
    if(connectionStatus==true)
    {
        connectionStatus=false;
        emit clientConfigured(false);
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


void canClient::socketRxData()
{
    if(connectionStatus ==false) return;
    if(socket->bytesAvailable()==0)
    {
        return;
    }
    QByteArray frame = socket->readAll();
    if(frame.size()==0)
    {
        return;
    }
    // Check the data protocol
    if(frame.size() < 3) return;
    if(frame.at(0)!='<') return;
    if(frame.at(2)!='>') return;

    // Heartbeat answer ..
    if(frame.at(1) == 'H'){
        return;
    }

    // Configuration
    if(frame.at(1) == 'C'){

        if(frame.size() < 5) return;
        configuredCanId = (frame.at(3) + 256 * frame.at(4)) & 0x7FF;        
        return;
    }

    // Can Data
    if(frame.at(1) != 'D') return;
    if(frame.size() < 11) return;
    QByteArray data = frame.right(8);


    emit rxCanFrame(data);

}

void canClient::txCanFrameToClient(QByteArray frame)
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

void canClient::configClient(ushort canId)
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

