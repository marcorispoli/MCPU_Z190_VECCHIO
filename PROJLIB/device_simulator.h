#ifndef DEVICE_H
#define DEVICE_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QHostAddress>
#include <QNetworkInterface>
#include <QAbstractSocket>
#include <QMutex>
#include <QWaitCondition>

class canRegister{

public:

    explicit canRegister(uchar address){
        data[0] =0;
        data[1] =0;
        data[2] =0;
        data[3] =0;
        this->address = address;
        updated = false;
    }
    ~canRegister(){};

    bool set(uchar b0, uchar b1, uchar b2, uchar b3){
        if((data[0] == b0) && (data[1] == b1) && (data[2] == b2) && (data[3] == b3)) return updated;

        data[0] = b0;
        data[1] = b1;
        data[2] = b2;
        data[3] = b3;
        updated = true;
        return true;
    }

    bool get(uchar* b0, uchar* b1, uchar* b2, uchar* b3, bool reset = true){
        *b0 = data[0];
        *b1 = data[1];
        *b2 = data[2];
        *b3 = data[3];
        bool stat = updated;
        if(reset) updated = false;
        return stat;
    }

    uchar data[4];
    uchar address;
    bool  updated;

};


class deviceSimulator: public QObject
{
    Q_OBJECT

public:
    explicit deviceSimulator(ushort CanId, QString IP, uint port);
    ~deviceSimulator();

    // protocol Implementation
    typedef enum{
        _PROTO_NOT_DEFINED  = 0,
        _PROTO_READ_STATUS  = 1,
        _PROTO_WRITE_PARAM  = 2,
        _PROTO_WRITE_DATA   = 3,
        _PROTO_STORE_PARAMS = 4,
        _PROTO_COMMAND_EXEC = 5,
    }_CanProtocolFrameCode;

    typedef enum{
       _S_RESERVED = 0,
       _S_REVISION,
       _S_SYSTEM,
       _S_ERROR,
       _S_COMMAND,
       _S_LAST
    }_StatusRegisters;

    typedef enum{
       _P_RESERVED = 0,
       _P_LAST
    }_ParamRegisters;

    typedef enum{
       _D_RESERVED = 0,
       _D_LAST
    }_DataRegisters;


signals:


public slots:
    virtual void canDriverReady(bool ){};

public:
    _inline canRegister getStatusRegister(uchar address) { return statusRegisters[address];}
    _inline canRegister getParamRegisters(uchar address) { return paramRegisters[address];}
    _inline canRegister getDataRegisters(uchar address) { return dataRegisters[address];}

protected:
    int Start(void);

    ushort canId;
    QList<canRegister> statusRegisters;
    QList<canRegister> paramRegisters;
    QList<canRegister> dataRegisters;


private slots:
    void socketRxData(); // Ricezione dati da socket
    void socketError(QAbstractSocket::SocketError error); // Errore dal socket
    void socketConnected(); // Segnale di connessione avvenuta con il server
    void socketDisconnected(); // IL server ha chiiuso la connessione
    void verifyClientConfiguration(void);

private:

    // Connection with the Can Server Socket
    QHostAddress serverip;      // Addrees of the remote server
    quint16      serverport;    // Port of the remote server
    QTcpSocket*  socket;        // Socket for  the ethernet client connection to the can Driver
    void clientConnect();       // Try to connect the canDriver
    bool connectionStatus;
    void txCanData(void);


    // Driver Configuration
    ushort configuredCanId;
    void configClient(ushort canId);   // Comand to configure the client with the canId

    // Can Protocol Handling
    QByteArray rxcanframe;
    QByteArray txcanframe;
    QByteArray formatReadStatus(uchar seq, uchar address);

    _CanProtocolFrameCode decodeFrame(void);
    void evaluateReadStatusFrame(void);
    void evaluateWriteParamFrame(void);
    void evaluateWriteDataFrame(void);
    void evaluateStoreFrame(void);
    void evaluateCommandFrame(void);
    void txCanData(QByteArray);

};

#endif // DEVICE_H
