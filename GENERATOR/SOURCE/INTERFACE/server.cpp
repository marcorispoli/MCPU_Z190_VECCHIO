#include "application.h"
#include <QStringConverter>

/**
 * @brief Server::Server
 *
 * @param
 * - ipaddress: IP where server will be bounded;
 * - port: bounding port
 *
 * \ingroup InterfaceModule
 */
Server::Server(QString ipaddress, int port):QTcpServer()
{
    socketList.clear();
    localip = QHostAddress(ipaddress);
    localport = port;
    idseq=0;
}

/**
 * @brief Server::~Server
 *
 * When the server should be destroyed, it shall disconnect all the clients
 * already connected.
 *
 * \ingroup InterfaceModule
 */
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

/**
 * @brief Start
 *
 * Starts the Client connection listening
 *
 * @return
 * - True: listening successfully started;
 * - flase: error in activate the listening;
 *
 * \ingroup InterfaceModule
 */
bool Server::Start(void)
{

    if (!this->listen(localip,localport)){
        qDebug() << "ERROR LISTENING AT ADDRESS: IP=" << localip.toString() << ", PORT=" << localport ;
        return false;
    }else{
        qDebug() << "LISTENING AT ADDRESS: IP=" << localip.toString() << ", PORT=" << localport ;
        return true;
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
 * \ingroup InterfaceModule
 */
void Server::incomingConnection(qintptr socketDescriptor)
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
    EventStatus(1);

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
 * \ingroup InterfaceModule
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



/**
 * @brief getProtocolFrame
 *
 * This function extract a list of String Items contained into
 * the parameter. Those items are parts of the Command frame.
 *
 * Each individual item is separated with a space from the next Item,
 * with the exception of the first and the last item:
 * - the first item can be preceded by the '<' character.
 * - the last item can be followed by the '>' character.
 *
 *
 * @param
 * - data: dataframe containing the list of command items;
 * @return
 *
 * \ingroup InterfaceModule
 */
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



/**
 * @brief sendAck
 *
 * This function sends an Acknowledge frame format to the Client Id
 *
 * The Parameter shall contain only the Acknowledge item,
 * the function will append the proper structure:
 *  > <A SEQ [data] >
 *
 * @param
 * - id: the client identifier that will receive the Ack frame;
 * - seq: sequence number of the received command to be acknowledged;
 * - data: the list of items that the function will include in the protocol frame.
 *
 * \ingroup InterfaceModule
 */
void Server::sendAck(ushort id, ushort seq,  QList<QString>* data){
    QByteArray buffer;

    // Creates the Acknowledge frame
    buffer.append('<');
    buffer.append('A');
    buffer.append(' ');
    buffer.append(QString("%1").arg(seq).toLatin1());
    buffer.append(' ');

    // Append the data content of the frame
    for(int i =0; i< data->size(); i++){
            buffer.append(data->at(i).toLatin1());
            buffer.append(' ');
    }
    buffer.append('>');
    qDebug() << buffer;

    // Append a line feed to help a character client to show the frame
    buffer.append('\n');
    buffer.append('\r');

    // Sends only to the socket Id requesting the command
    for(int i=0; i< socketList.size(); i++){
        if(socketList[i]->id == id){
            socketList[i]->socket->write(buffer);
            socketList[i]->socket->waitForBytesWritten(100);
            return;
        }
    }
}

/**
 * @brief sendEvent
 *
 * This function sends an EVENT frame with the format:
 * - <E SEQ [data]>
 *
 * The frame is sent broadcast to all the connected Clients.
 *
 * @param
 * - seq: sequence number of the Event ;
 * - data: the list of parameter's item of the EVENT.
 *
 * \ingroup InterfaceModule
 */
void Server::sendEvent( ushort seq, QList<QString>* data){
    QByteArray buffer;
    buffer.append('<');
    buffer.append('E');
    buffer.append(' ');
    buffer.append(QString("%1").arg(seq).toLatin1());
    buffer.append(' ');

    // Append the EVENT items
    for(int i =0; i< data->size(); i++){
            buffer.append(data->at(i).toLatin1());
            buffer.append(' ');
    }
    buffer.append('>');
    qDebug() << buffer;

    // Append the line feed to help a character monitor client
    buffer.append('\n');
    buffer.append('\r');

    // Sends broadcast to ALL clients
    for(int i=0; i< socketList.size(); i++){
        socketList[i]->socket->write(buffer);
        socketList[i]->socket->waitForBytesWritten(100);
    }
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
void Server::receivedCommandSlot(ushort id, QByteArray data){

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
}

