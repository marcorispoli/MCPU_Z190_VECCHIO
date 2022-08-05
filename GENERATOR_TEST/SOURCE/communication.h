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

    // Event coming from the R2CP interface
    void generatorReceivedStatusEvent(void);
    void generatorReceivedProcedureDefinitionEvent(byte id);



    void handleCommandProcess(uchar seq, uchar val){};


    _inline bool isSHConnected(void) {return smartHubConnected;}
    _inline bool isGenConnected(void) {return generatorConnected;}
    _inline bool isProtocolV6(void){return R2CP::CaDataDicGen::GetInstance()->isProtoV6();}
    _inline bool isProtocolV5(void){return R2CP::CaDataDicGen::GetInstance()->isProtoV5();}
    _inline bool isProcedureInitialized(uchar i){return R2CP::CaDataDicGen::GetInstance()->radInterface.isProcInitialized(i);}


    _inline void getProtocolVersion() {R2CP::CaDataDicGen::GetInstance()->Protocol_Get_Version();}
    _inline void setProtocolVersion5() {R2CP::CaDataDicGen::GetInstance()->Protocol_Set_Version5();}
    _inline void setProtocolVersion6() {R2CP::CaDataDicGen::GetInstance()->Protocol_Set_Version6();}

    _inline void getGeneratorStatusV5() {R2CP::CaDataDicGen::GetInstance()->Generator_Get_StatusV5();}
    _inline void getGeneratorStatusV6() {R2CP::CaDataDicGen::GetInstance()->Generator_Get_StatusV6();}
    _inline void setDbInitialize(uchar i) {R2CP::CaDataDicGen::GetInstance()->Generator_Set_Databank(i);}
    _inline bool isDbInitialized(uchar i){return R2CP::CaDataDicGen::GetInstance()->radInterface.isDbInitialized(i);}


    _inline void setSHConnection() {R2CP::CaDataDicGen::GetInstance()->Network_ConnectionRequest_Event(SH_NODE_ID, APPLICATION_NODE_ID);}
    _inline void clearSystemMessages(void){R2CP::CaDataDicGen::GetInstance()->SystemMessages_Clear_AllMessages();}
    _inline void getAllSystemMessages(void){R2CP::CaDataDicGen::GetInstance()->SystemMessages_Get_AllMessages();}
    _inline void setupProcedureV5(uchar num){R2CP::CaDataDicGen::GetInstance()->Patient_SetupProcedureV5(num);}
    _inline void setupProcedureV6(uchar num){R2CP::CaDataDicGen::GetInstance()->Patient_SetupProcedureV6(num);}

    _inline void setProcedureManual(void){}
    _inline void setProcedureAec(void){}
    _inline void setProcedureTomo(void){}
    _inline void setProcedureTomoAec(void){}

private:
    TcpIpClient client;
    bool connection_status;
    CR2CP_Eth* R2CP_Eth;

    bool smartHubConnected;
    bool generatorConnected;

    QList<QByteArray> rxList;


};

#endif // COMMUNICATION_H
