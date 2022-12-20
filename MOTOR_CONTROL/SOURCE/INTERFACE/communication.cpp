#include "application.h"


/**
 * @brief Start
 *
 * Starts the Client connection listening
 *
 * @return
 * - True: listening successfully started;
 * - flase: error in activate the listening;
 *
 */
bool Interface::Start(void)
{

    if (!this->listen(localip,localport)){
        qDebug() << "ERROR LISTENING AT ADDRESS: IP=" << localip.toString() << ", PORT=" << localport ;
        return false;
    }else{
        qDebug() << "LISTENING AT ADDRESS: IP=" << localip.toString() << ", PORT=" << localport ;
        return true;
    }

}


void Interface::disconnected(ushort id)
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

/**
 * @brief Server::incomingConnection
 * @param socketDescriptor
 *
 * This Slot is connected to the QTcpServer class connection request.
 * Every Client connection is assigned to a proper thread
 * handling the connection.
 *
 * When a client is successfully connected, it will receive the 'EventStatus()'
 * as a Welcome frame.
 *
 */
void Interface::incomingConnection(qintptr socketDescriptor)
{

    // Create a new SocketItem and its internal socket
    // This will be added to the Socket list of the server.
    SocketItem* item = new SocketItem();

    item->socket = new QTcpSocket(this); // Create a new socket
    if(!item->socket->setSocketDescriptor(socketDescriptor))
    {
        delete item->socket;
        delete item;
        return;
    }

    // Add the Client socket to the list of the Connected socket
    item->socket->setSocketOption(QAbstractSocket::LowDelayOption,1);
    socketList.append(item);


    // Interface signal connection
    connect(item->socket,SIGNAL(readyRead()), item, SLOT(socketRxData()),Qt::UniqueConnection);
    connect(item,SIGNAL(receivedCommandSgn(ushort, QByteArray)), this, SLOT(receivedCommandSlot(ushort, QByteArray)),Qt::QueuedConnection);

    connect(item->socket,SIGNAL(disconnected()),item, SLOT(disconnected()),Qt::UniqueConnection);
    connect(this,SIGNAL(txFrame(QByteArray)),item, SLOT(socketTxData(QByteArray)),Qt::QueuedConnection);

    connect(item,SIGNAL(itemDisconnected(ushort )),this, SLOT(disconnected(ushort )),Qt::UniqueConnection);
    connect(item->socket,SIGNAL(errorOccurred(QAbstractSocket::SocketError)),item,SLOT(socketError(QAbstractSocket::SocketError)),Qt::UniqueConnection);

    // The client is assigned to an unique ID identifier
    item->id = this->idseq++;

    // The Welcome Frame is sent to the client with the current generator status.


    return;
 }




/**
 * @brief receivedCommandSlot
 *
 * This function decodes the data frame providing a list
 * of decoded items that make part of a received EVENT.
 *
 * The list of the Item is further handled by the
 * specific EVENT callback in order to be correctly executed.
 *
 * @param
 * - id: the client identifier that sent the EVENT;
 * - data: command data stream.
 *
 * \ingroup InterfaceModule
 */
void Interface::receivedCommandSlot(ushort id, QByteArray data){

    /*
    // Extracts the protocol items list
    QList<QString> command =  getProtocolFrame(&data);
    if(command.size() < 3) return;
    if(command.at(0) != "E") return;
    ushort seq = command.at(1).toUShort();

    QList<QString> answer;

    // Command discrimination and execution
    if(command.at(2)        == "SetExposurePre")    answer  += SetExposurePre(&command);
    else if(command.at(2)   == "SetExposurePulse")  answer  += SetExposurePulse(&command);
    else if(command.at(2)   == "SetTomoConfig")     answer  += SetTomoConfig(&command);
    else if(command.at(2)   == "StartExposure")     answer  += StartExposure(&command);
    else if(command.at(2)   == "AbortExposure")     answer  += AbortExposure(&command);
    else if(command.at(2)   == "GetPostExposure")   answer  += GetPostExposure(&command);

    // The Acknowledge frame is sent back to the client
    sendAck(id, seq, &answer);
    */
}


void SocketItem::disconnected(void){
    emit itemDisconnected(this->id);
}

void SocketItem::socketError(QAbstractSocket::SocketError error)
{
    emit itemDisconnected(this->id);
    return;
}



/**
 * @brief SocketItem::socketRxData
 *
 * This is the handler of the data reception of a given socket.
 *
 * When a data frame is received, it is decoded and the internal command string is \n
 * forwarded to the Command function handler through the signal 'receivedCommandSgn()'.
 *
 * The function is able to handle queued frames contained in a single data streaming.
 *
 * A valid frame shall be a set of string within the < > delimiters.
 *
 */
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



