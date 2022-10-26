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
    void receivedCommandSgn(ushort id, QByteArray data);


public slots:

    void disconnected();
    void socketError(QAbstractSocket::SocketError error);
    void socketRxData();
    void socketTxData(QByteArray);

public:
    QTcpSocket* socket;
    ushort id;
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
    void txFrame(QByteArray data);

public slots:
    void receivedCommandSlot(ushort id, QByteArray data);
    void disconnected(ushort id);


protected:
    void incomingConnection(qintptr socketDescriptor) override; //!< Incoming connection slot

public:
    void EventStatus(ushort seq,uchar stat);
    void EventSysMessages(ushort seq,QList<ulong>* list);
    void EventExposureError(ushort seq, uchar code);
    void EventSetXrayEna(ushort seq, bool state);
    void EventXrayCompleted(ushort seq, uchar code, uchar error);
    void EventSwError(ushort seq, uchar error);
    void EventGetPulseData(ushort seq);

private:

    QList<SocketItem*>   socketList;    //!< List of Sockets
    QHostAddress        localip;       //!< Address of the local server
    quint16             localport;     //!< Port of the local server
    ushort              idseq;    

    void sendAnswer(ushort id, QList<QString>*  data);
    void sendEvent(QList<QString>*  data);

    QList<QString>      getProtocolFrame(QByteArray* data);


    QList<QString> SetExposurePre(QList<QString>* command);
    QList<QString> SetExposurePulse(QList<QString>* command);
    QList<QString> StartExposure(QList<QString>* command);
    QList<QString> AbortExposure(QList<QString>* command);
    QList<QString> GetPostExposure(QList<QString>* command);

};


#endif // SERVER_H
