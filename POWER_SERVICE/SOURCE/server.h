#ifndef SERVER_H
#define SERVER_H

#include <QObject>

#include <QTcpServer>
#include <QTcpSocket>
#include <QHostAddress>
#include <QNetworkInterface>


class SocketItem: public QObject
{
     Q_OBJECT

public:

    explicit SocketItem(){};
    ~SocketItem(){};

signals:
    void itemDisconnected(ushort id);

public slots:

    void disconnected();
    void socketError(QAbstractSocket::SocketError error);
    void socketRxData();

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

    bool Start(void);


signals:

public slots:
    void disconnected(ushort id);


protected:
    void incomingConnection(qintptr socketDescriptor) override; //!< Incoming connection slot


private:

    QList<SocketItem*>   socketList;    //!< List of Sockets
    QHostAddress        localip;       //!< Address of the local server
    quint16             localport;     //!< Port of the local server
    ushort              idseq;
};


#endif // SERVER_H
