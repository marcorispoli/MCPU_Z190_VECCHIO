#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include "tcpipclient.h"
#include "Typedef.h"
#include <QtEndian>
#include "R2CP_Eth.h"
#include "CaDataDicGen.h"



class Communication : public QObject
{
    Q_OBJECT

public:
    explicit Communication(QObject *parent = nullptr);

    void start(void);
    void eventConnection(void);

signals:
    void rxDataEventSgn(QByteArray);

public slots:    
    void clientConnection(bool status);
    void clientRxData(QByteArray data);

public:

    int16_t sendData(byte *pMessage , word datalength);


    void clearRxList(){rxList.clear();}
    QList<QByteArray> getList(){return rxList;}

    // _______________________________________________________________________________________
    void smartHubConnectionEvent(void);
    void generatorConnectionEvent(bool);

    void generatorReceivedStatusEvent(void);



    _inline bool isSHConnected(void) {return smartHubConnected;}
    _inline bool isGenConnected(void) {return generatorConnected;}
    _inline void getGeneratorStatus() {R2CP::CaDataDicGen::GetInstance()->Generator_Get_Status();}
    _inline void setSHConnection() {R2CP::CaDataDicGen::GetInstance()->Network_ConnectionRequest_Event(SH_NODE_ID, APPLICATION_NODE_ID);}
    _inline void clearSystemMessages(void){R2CP::CaDataDicGen::GetInstance()->SystemMessages_Clear_AllMessages();}
    _inline void getAllSystemMessages(void){R2CP::CaDataDicGen::GetInstance()->SystemMessages_Get_AllMessages();}

    _inline void setupProcedure(uchar num){R2CP::CaDataDicGen::GetInstance()->Patient_SetupProcedure(num);}

private:
    TcpIpClient client;
    bool connection_status;
    CR2CP_Eth* R2CP_Eth;

    bool smartHubConnected;
    bool generatorConnected;

    QList<QByteArray> rxList;

};

#endif // COMMUNICATION_H
