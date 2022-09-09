#ifndef SERVER_H
#define SERVER_H

#include <QObject>

#include <QTcpServer>
#include <QTcpSocket>
#include <QHostAddress>
#include <QNetworkInterface>

#define _DATA_CONFIG    0
#define _DATA_CAN       1

class SocketItem: public QObject
{
     Q_OBJECT

public:

    explicit SocketItem(){};
    ~SocketItem(){};

signals:
    void itemDisconnected(ushort id);
    void sendToCan(ushort canId, QByteArray dataFrame);
    void sendToClient(QByteArray data);

public slots:

    void disconnected();
    void socketError(QAbstractSocket::SocketError error);
    void socketRxData();
    void socketTxData(QByteArray);

public:
    QTcpSocket* socket;
    ushort id;
    ushort canId;
};

class Server : public QTcpServer
{
    Q_OBJECT

public:

    explicit Server(QString ipaddress, int port);
    ~Server();

    static const long _DEFAULT_TX_TIMEOUT = 5000;    //!< Default timeout in ms for tx data
    bool Start(void);

    int _inline localPort(){return localport;} //!< Returns the port binded
    QHostAddress _inline localAddress(){return localip;} //!< returns the IP Address binded

signals:
    void sendToCan(ushort canId, QByteArray dataFrame);

public slots:
    void disconnected(ushort id);
    void receivedCanFrame(ushort canId, QByteArray data);
    void sendToCanSlot(ushort canId, QByteArray dataFrame);


protected:
    void incomingConnection(qintptr socketDescriptor) override; //!< Incoming connection slot


private:

    QList<SocketItem*>   socketList;    //!< List of Sockets
    QHostAddress        localip;       //!< Address of the local server
    quint16             localport;     //!< Port of the local server
    ushort              idseq;
};


#endif // SERVER_H
