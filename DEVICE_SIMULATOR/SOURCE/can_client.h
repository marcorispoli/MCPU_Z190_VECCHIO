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
    explicit canClient(ushort canId);
    ~canClient();

    int Start(QString remip, int remport);

signals:
    void rxCanFrame(QByteArray );  // Received can frame from the CAN BUS
    void clientConfigured(bool); // Client configured with the canId

public slots:
    void txCanFrameToClient(QByteArray );  // Data to be sent to the CAN driver


private slots:
    void socketRxData(); // Ricezione dati da socket
    void socketError(QAbstractSocket::SocketError error); // Errore dal socket
    void socketConnected(); // Segnale di connessione avvenuta con il server
    void socketDisconnected(); // IL server ha chiiuso la connessione    
    void verifyClientConfiguration(void);

public:
    bool connectionStatus;
    bool connectionAttempt; // E' in corso un tentativo di connessione

private:
    QHostAddress serverip;      // Addrees of the remote server
    quint16      serverport;    // Port of the remote server
    QTcpSocket*  socket;

    void clientConnect();       // Try to connect the remote server

    // Configuration
    ushort canId;
    ushort configuredCanId;
    void configClient(ushort canId);   // Comand to configure the client with the canId

};

#endif // CAN_CLIENT_H
