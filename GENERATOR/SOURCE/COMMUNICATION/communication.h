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
    void PostExposureEvent(uchar db_number, uchar foc, float kV, ushort mAs, ushort mA, ushort ms, uchar result_str);


    void handleCommandProcess(uchar seq, uchar val){};

    _inline bool isCommandProcessed(void){return R2CP_Eth->isCommandProcessed();}
    _inline uint8_t getCommandProcessed(void){return R2CP_Eth->getCommandProcessedResult();}
    _inline uint8_t getAssignedSequence(void){return R2CP_Eth->getAssignedSequence();}
    _inline QString getCommandProcessedString(void){return R2CP_Eth->getCommandProcessedString();}


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
    _inline void set2DDataBank(uchar i, uchar focus, float kV, uint mAs) {R2CP::CaDataDicGen::GetInstance()->Generator_Set_2D_Databank(i,focus,kV,mAs);}
    _inline bool isDbInitialized(uchar i){return R2CP::CaDataDicGen::GetInstance()->radInterface.isDbInitialized(i);}


    _inline void setSHConnection() {R2CP::CaDataDicGen::GetInstance()->Network_ConnectionRequest_Event(SH_NODE_ID, APPLICATION_NODE_ID);}

    _inline void clearSystemMessage(uint id){R2CP::CaDataDicGen::GetInstance()->SystemMessages_Clear_Message(id);}
    _inline void getAllSystemMessages(void){R2CP::CaDataDicGen::GetInstance()->SystemMessages_Get_AllMessages();}
    _inline void setDisableRxSystemMessage(bool state){R2CP::CaDataDicGen::GetInstance()->SystemMessages_SetDisableRx(state);}
    _inline  QList<ulong>* getErrorList(void){return &R2CP::CaDataDicGen::GetInstance()->systemInterface.messageList;}

    _inline void setupProcedureV5(uchar num){R2CP::CaDataDicGen::GetInstance()->Patient_SetupProcedureV5(num);}
    _inline void setupProcedureV6(uchar num){R2CP::CaDataDicGen::GetInstance()->Patient_SetupProcedureV6(num);}
    _inline void clearAllProcedures(void){R2CP::CaDataDicGen::GetInstance()->Patient_ClearAllProcedures();}


    _inline void assignDbToProc(uint8_t db, uint8_t proc, uint8_t index){R2CP::CaDataDicGen::GetInstance()->Generator_AssignDbToProc(db, proc, index);}
    _inline void verifyDbToProc(uint8_t proc, uint8_t index){R2CP::CaDataDicGen::GetInstance()->Generator_verifyDbToProc(proc, index);}

    _inline void activate2DProcedurePulse(bool detector, bool grid){R2CP::CaDataDicGen::GetInstance()->Patient_Activate2DProcedurePulse(detector, grid);}

    _inline void activate2DAecProcedurePulse(void){R2CP::CaDataDicGen::GetInstance()->Patient_Activate2DAecProcedurePulse();}
    _inline void activate2DAecProcedurePre(void){R2CP::CaDataDicGen::GetInstance()->Patient_Activate2DAecProcedurePre();}

    _inline void startExposure(void){R2CP::CaDataDicGen::GetInstance()->Generator_startExposure();}
    _inline void stopExposure(void){R2CP::CaDataDicGen::GetInstance()->Generator_stopExposure();}

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
