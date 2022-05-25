#ifndef AWSCOMMUNICATION_H
#define AWSCOMMUNICATION_H

#include <QObject>
#include "awsinterface.h"



class awsCommunication : public awsInterface
{
    Q_OBJECT

public:
    explicit awsCommunication(QString address, int port, QObject *parent = nullptr);
    bool getConnectionStatus(void){return connectionStatus;}
    void bind(QString ip, uint port);

signals:
    void sendAsyncSgn(commandProtocol prot, int code, QList<QString>* params);


public slots:
    void rxFromAws(QByteArray);
    void rxFromGantry(QByteArray);
    void sendAsyncSlot(commandProtocol prot, int code, QList<QString>* params){
        sendAsync(prot, code, params);
    }


    void trxCompletedSlot(void);
    void armCompletedSlot(void);
    void powerdownSlot(void);

public:


    void gantryReqProjection(QString item);
    void gantryReqAbortProjection(void);
    void gantrySetAccessory(QString potter, QString paddle, QString colli);
    void gantrySetTubeTemperature(QString anode, QString housing);
    void gantryXrayPushEvent(void);
    void gantryCompressorData(QString thick, QString force);
    void gantryError(QString error);
    void gantryXraySequenceCompleted(QString result);
    void gantryPulseCompleted(void);



private:

    // Projections
    QList<QString> ProjectionList;
    QList<QString> setProjectionList;

    // Tomo configuration
    QString selectedTomoId;
    QString selectedTomoConfiguration;


    commandProtocol trxProtocol;
    commandProtocol armProtocol;


    bool arm_fault;
    bool arm_enabled;


    QString PatientName;
    bool study;
    bool trx_fault;


    bool readyForExposure;




    // Commands implementation
    void EXEC_OpenStudy(void);
    void EXEC_Close(void);
    void EXEC_TrxPosition(void);
    void EXEC_ArmPosition(void);
    void EXEC_AbortProjections(void);
    void GET_RotationAngles(void);
    void GET_Accessories(void);
    void GET_CompressorData(void);
    void SET_ProjectionList(void);
    void GET_TomoConfigFile(void);
    void GET_TomoConfig(void);
    void EXEC_ResetError(void);
    void EXEC_PowerOff(void);
    void GET_TubeTemperature(void);
    void SET_CompressionRelease(void);
    void SET_XrayPushEnable(void);
    void GET_ReadyForExposure(void);
    void GET_XrayPushStatus(void);
    void EXEC_StartXraySequence(void);
    void SET_PulseData(void);
    void SET_ExposureMode(void);
    void GET_ExposureCompletionData(void);
    void SET_Language(void);

};

#endif // AWSCOMMUNICATION_H
