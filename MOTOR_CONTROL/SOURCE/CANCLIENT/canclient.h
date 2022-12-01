#ifndef CANCLIENT_H
#define CANCLIENT_H

/*!
 * \defgroup  canModule Can Driver Communication Module.
 *
 * This Module implements the communication protocol with the Gantry.
 *
 *
 */


#include <QTcpServer>
#include <QTcpSocket>
#include <QHostAddress>
#include <QNetworkInterface>
#include <QAbstractSocket>
#include <QMutex>
#include <QWaitCondition>

class canClient: public QTcpServer
{
    Q_OBJECT

public:
    explicit canClient(void);
    ~canClient();


    int Start(QString remip, int remport);

    QHostAddress peerAddress() {return serverip;}
    int peerPort(){return serverport;}

signals:
    void clientConnection(bool status);
    void rxData(QByteArray data);

public slots:
    void txData(QByteArray data, long timeout = 5000);


private slots:
    void socketRxData(); // Ricezione dati da socket
    void socketError(QAbstractSocket::SocketError error); // Errore dal socket
    void socketConnected(); // Segnale di connessione avvenuta con il server
    void socketDisconnected(); // IL server ha chiiuso la connessione

public:
    bool connectionStatus;
    bool connectionAttempt; // E' in corso un tentativo di connessione

private:
    QHostAddress serverip;      // Addrees of the remote server
    quint16      serverport;    // Port of the remote server
    QTcpSocket*  socket;

    void clientConnect();       // Try to connect the remote server

};

/*
Q_DECLARE_METATYPE(QAbstractSocket::SocketState)
Q_DECLARE_METATYPE(QAbstractSocket::SocketError)
*/

#endif // TCPIPCLIENT_H
