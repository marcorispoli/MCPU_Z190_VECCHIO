#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <QObject>
#include "tcpipclient.h"



class Communication : public QObject
{
    Q_OBJECT

public:
    explicit Communication(QObject *parent = nullptr);


    #define IP_ADDRESS  "127.0.0.1"
    #define HUB_PORT    10000

    #define BROADCAST_ID 0
    #define SMART_HUB_ID 1
    #define GENERATOR_ID 2
    #define GANTRY_ID    3

    #define NETWORK_IDX 0xA0

    #define _EVENT_FRAME 3
    #define _GET_FRAME   2

    void start(void);


    void eventConnection(void);

signals:


public slots:

    void clientConnection(bool status);
    void clientRxData(QByteArray data);

public:




private:
    TcpIpClient client;
    bool connection_status;

    uchar sequence;
    QByteArray buildFrame(uchar prio, uchar seq, uchar dest, uchar index, uchar subidx, uchar function , ushort len, QByteArray data){
        QByteArray frame;
        uchar val = (prio << 6) | (seq &0x3F);
        frame.append(val);
        frame.append(dest);
        frame.append((uchar) GANTRY_ID);
        frame.append(index);
        frame.append(subidx);
        frame.append(function);
        frame.append(len >> 8);
        frame.append(len & 0xFF);

        frame += data;
        return frame;
    }

    void handleNetworkFrame(QByteArray* data);
    void handleGeneratorFrame(QByteArray* data);
};

#endif // COMMUNICATION_H
