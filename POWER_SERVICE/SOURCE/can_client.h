#ifndef CAN_CLIENT_H
#define CAN_CLIENT_H


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
    explicit canClient();
    ~canClient();

    int Start(QString remip, int remport);

signals:
    void clientConnection(bool status);
    void rxData(QByteArray );

public slots:
    void txData(QByteArray );


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

#endif // CAN_CLIENT_H
