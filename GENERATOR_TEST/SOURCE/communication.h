#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <QObject>
#include "tcpipclient.h"
#include "R2CP_Eth.h"
#include "CaDataDicGen.h"



class Communication : public QObject
{
    Q_OBJECT

public:
    explicit Communication(QObject *parent = nullptr);


    #define IP_ADDRESS  "127.0.0.1"
    #define HUB_PORT    10000



    void start(void);
    void eventConnection(void);

signals:


public slots:

    void clientConnection(bool status);
    void clientRxData(QByteArray data);

public:

    int16_t sendData(byte *pMessage , word datalength);

    void generatorStatusGet(void);

private:
    TcpIpClient client;
    bool connection_status;

    CR2CP_Eth* R2CP_Eth;


};

#endif // COMMUNICATION_H
