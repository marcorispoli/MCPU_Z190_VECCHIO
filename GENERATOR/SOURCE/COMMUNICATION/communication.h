#ifndef COMMUNICATION_H
#define COMMUNICATION_H


/**
* \defgroup  r2cpCommunicationModule Communication Interface With R2CP Module.
*
* This Module implements the Communication interface with the R2CP Sedecal Library protocol.
*
* # GENERAL OVERVIEW
*/


#include "tcpipclient.h"
#include "Typedef.h"
#include <QtEndian>
#include "R2CP_Eth.h"
#include "CaDataDicGen.h"


/**
 * @brief The Communication class
 *
 * \ingroup r2cpCommunicationModule
 */
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
    void PostExposureEvent(uchar db_number, uchar foc, float kV, float mAs, float mA, float ms, uchar result_str);


    void handleCommandProcess(uchar seq, uchar val){};

    _inline bool isCommandProcessed(void){return R2CP_Eth->isCommandProcessed();}
    _inline uint8_t getCommandProcessed(void){return R2CP_Eth->getCommandProcessedResult();}
    _inline uint8_t getAssignedSequence(void){return R2CP_Eth->getAssignedSequence();}
    _inline QString getCommandProcessedString(void){return R2CP_Eth->getCommandProcessedString();}


    _inline bool isSHConnected(void) {return smartHubConnected;}
    _inline bool isGenConnected(void) {return generatorConnected;}
    _inline bool isProtocolV6(void){return R2CP::CaDataDicGen::GetInstance()->isProtoV6();}
    _inline bool isVersionUpdated(void){return R2CP::CaDataDicGen::GetInstance()->isVersionUpdated();}



    _inline bool isProtocolV5(void){return R2CP::CaDataDicGen::GetInstance()->isProtoV5();}
    _inline bool isProcedureInitialized(uchar i){return R2CP::CaDataDicGen::GetInstance()->radInterface.isProcInitialized(i);}


    _inline void getProtocolVersion() {R2CP::CaDataDicGen::GetInstance()->Protocol_Get_Version();}
    _inline void setProtocolVersion5() {R2CP::CaDataDicGen::GetInstance()->Protocol_Set_Version5();}
    _inline void setProtocolVersion6() {R2CP::CaDataDicGen::GetInstance()->Protocol_Set_Version6();}

    _inline void getGeneratorStatusV5() {R2CP::CaDataDicGen::GetInstance()->Generator_Get_StatusV5();}
    _inline void getGeneratorStatusV6() {R2CP::CaDataDicGen::GetInstance()->Generator_Get_StatusV6();}


    _inline void getDataBank(uchar i) {R2CP::CaDataDicGen::GetInstance()->Generator_Get_Databank(i);}
    _inline void set2DDataBank(uchar i, uchar focus, float kV, float mAs, ulong tmo) {R2CP::CaDataDicGen::GetInstance()->Generator_Set_2D_Databank(i,focus,kV,mAs, tmo);}
    _inline bool isDbInitialized(uchar i){return R2CP::CaDataDicGen::GetInstance()->radInterface.isDbInitialized(i);}

    _inline void set3DDataBank(uchar dbId, uchar focus, float KV, float MA, float MS, float MT) {R2CP::CaDataDicGen::GetInstance()->Generator_Set_3D_Databank(dbId,focus,KV,MA,MS,MT);}
    _inline void setMsDatabank(uchar dbId, float MS) {R2CP::CaDataDicGen::GetInstance()->Generator_Set_Ms_Databank(dbId, MS);}

    _inline void setGenerator_SkipPulse_Databank(uchar dbId, uchar nskip){R2CP::CaDataDicGen::GetInstance()->Generator_Set_SkipPulse_Databank(dbId, nskip);}
    _inline void setGenerator_Assign_SkipPulse_Databank(uchar dbId, uchar procedureId){R2CP::CaDataDicGen::GetInstance()->Generator_Assign_SkipPulse_Databank(procedureId, dbId);}

    _inline void setSHConnection() {R2CP::CaDataDicGen::GetInstance()->Network_ConnectionRequest_Event();}

    _inline void clearSystemMessage(uint id){R2CP::CaDataDicGen::GetInstance()->SystemMessages_Clear_Message(id);}
    _inline void getAllSystemMessages(void){R2CP::CaDataDicGen::GetInstance()->SystemMessages_Get_AllMessages();}
    _inline void setDisableRxSystemMessage(bool state){R2CP::CaDataDicGen::GetInstance()->SystemMessages_SetDisableRx(state);}
    _inline  QList<ulong>* getErrorList(void){return &R2CP::CaDataDicGen::GetInstance()->systemInterface.messageList;}

    _inline void setupProcedureV5(uchar num){R2CP::CaDataDicGen::GetInstance()->Patient_SetupProcedureV5(num);}
    _inline void setupProcedureV6(uchar procId, uchar param){R2CP::CaDataDicGen::GetInstance()->Patient_SetupProcedureV6(procId, param);}

    _inline void clearAllProcedures(void){R2CP::CaDataDicGen::GetInstance()->Patient_ClearAllProcedures();}


    _inline void assignDbToProc(uint8_t db, uint8_t proc, uint8_t index){R2CP::CaDataDicGen::GetInstance()->Generator_AssignDbToProc(db, proc, index);}
    _inline void verifyDbToProc(uint8_t proc, uint8_t index){R2CP::CaDataDicGen::GetInstance()->Generator_verifyDbToProc(proc, index);}

    _inline void activate2DProcedurePulse(bool detector, bool grid){R2CP::CaDataDicGen::GetInstance()->Patient_Activate2DProcedurePulse(detector, grid);}

    _inline void activate2DAecProcedurePulse(void){R2CP::CaDataDicGen::GetInstance()->Patient_Activate2DAecProcedurePulse();}
    _inline void activate2DAecProcedurePre(void){R2CP::CaDataDicGen::GetInstance()->Patient_Activate2DAecProcedurePre();}

    _inline void activate3DProcedurePulse(void){R2CP::CaDataDicGen::GetInstance()->Patient_Activate3DProcedurePulse();}

    _inline void activate3DAecProcedurePre(void){R2CP::CaDataDicGen::GetInstance()->Patient_Activate3DAecProcedurePre();}
    _inline void activate3DAecProcedurePulse(void){R2CP::CaDataDicGen::GetInstance()->Patient_Activate3DAecProcedurePulse();}

    _inline void startExposure(void){R2CP::CaDataDicGen::GetInstance()->Generator_startExposure();}
    _inline void stopExposure(void){R2CP::CaDataDicGen::GetInstance()->Generator_stopExposure();}



private:
    TcpIpClient client;
    bool connection_status;
    CR2CP_Eth* R2CP_Eth;

    bool smartHubConnected;
    bool generatorConnected;

    QList<QByteArray> rxList;


};

#endif // COMMUNICATION_H
