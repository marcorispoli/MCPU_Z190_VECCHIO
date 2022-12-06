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
    explicit canClient(ushort can_mask, ushort can_address, QString IP, int PORT);
    ~canClient();


    int ConnectToCanServer(void);

    _inline bool isCanReady(void) {return rx_filter_open;}

signals:
    void rxFromCan(ushort canId, QByteArray data);

public slots:
    void txToCanData(ushort canId, QByteArray data);


private slots:
    void socketRxData(); // Ricezione dati da socket
    void socketError(QAbstractSocket::SocketError error); // Errore dal socket
    void socketConnected(); // Segnale di connessione avvenuta con il server
    void socketDisconnected(); // IL server ha chiiuso la connessione
    void setAcceptanceFilter();

public:
    bool connectionStatus;
    bool connectionAttempt; // E' in corso un tentativo di connessione

private:
    QHostAddress serverip;      // Addrees of the remote server
    quint16      serverport;    // Port of the remote server
    QTcpSocket*  socket;

    ushort filter_mask;         //!< The CAN Acceptance Filter Mask
    ushort filter_address;      //!< The CAN Acceptance Filter Address
    bool rx_filter_open;        //!< The Acceptance filter has been set

    void clientConnect();       // Try to connect the remote server    

    void handleSocketFrame(QByteArray* data);
    ushort getItem(int* index, QByteArray* data, bool* data_ok);
};



#endif // TCPIPCLIENT_H
