#ifndef STATUS_MANAGER_H
    #define STATUS_MANAGER_H

#include <QObject>
#include "Typedef.h"


class statusManager : public QObject
{
    Q_OBJECT

public:
    explicit statusManager(QObject *parent = nullptr);


    void aecPulsData(uchar foc, float kv, ushort mAs){
        focus = foc;
        pulse_kV = kv;
        pulse_mAs = mAs;
        aecDataPresent = true;
    }

    void setPreData(uchar foc, float kv, ushort mAs){
        focus = foc;
        pre_kV = kv;
        pre_mAs = mAs;
    }

    void setPulseData(uchar foc, float kv, ushort mAs){
        focus = foc;
        pulse_kV = kv;
        pulse_mAs = mAs;
    }




    _inline void setGeneratorStatusChanged(void ) {generator_status_changed = true;}
    _inline void start2DExposure(void){ abortRxRequest = false; requestState = SMS_EXECUTING_2D_MANUAL;  }
    _inline void start2DAecExposure(void){ abortRxRequest = false; requestState = SMS_EXECUTING_2D_AEC;  }
    _inline bool isIdle(void){ return  (internalState == SMS_IDLE ); }
    _inline bool requestAbortProcedure(void){ abortRxRequest = true; }
    _inline uchar getStatus(void){ return  interfaceStatus;}

    _inline void clearPostExposureList(void){ postExposureList.clear();}
    _inline QList<Interface::tPostExposureData> getPostExposureList(void){ return postExposureList;}
    void setPostExposureData(uchar pulse_seq, uchar foc, float kV, ushort mAs, ushort mA, ushort ms, uchar result){
        Interface::tPostExposureData data;

        data.pulse_seq = pulse_seq;
        data.foc = foc;
        data.kV = kV;
        data.mAs = mAs;
        data.mA = mA;
        data.ms = ms;
        data.result = result;
        postExposureList.append(data);
    }

signals:


public slots:


public:

    typedef enum{
      SMS_UNDEFINED = 0,
      SMS_SMART_HUB_CONNECTION,
      SMS_WAIT_GENERATOR_CONNECTION,
      SMS_SERVICE,
      SMS_IDLE,
      SMS_MESSAGE_NOTIFY,
      SMS_CLEAR_SYS_MESSAGES,
      SMS_EXECUTING_2D_MANUAL,
      SMS_EXECUTING_2D_AEC,
      SMS_EXECUTING_3D_MANUAL,
      SMS_EXECUTING_3D_AEC,
      SMS_EXECUTING_AE_MANUAL,
      SMS_EXECUTING_AE_AEC,
      SMS_ERROR,
    }_statusManagerState;

    typedef enum{
      SMS_ERR_NONE = 0,
      SMS_ERR_EXPOSURE,
      SMS_ERR_SYSMSG,
      SMS_ERR_GENERIC,
    }_statusErrors;

    typedef enum{
      SMS_GENERIC_ERR_NONE = 0,
      SMS_ERR_GENERATOR_CONNECTION,
    }_genericErrors;

    typedef enum{
      SMS_EXP_ERR_NONE = 0,
      SMS_EXP_ERR_SYSMSG,
      SMS_EXP_ERR_PULSE_VALIDATION,
      SMS_EXP_ERR_PRE_VALIDATION,
      SMS_EXP_ERR_PREPARATION,
      SMS_EXP_ERR_SEQUENCE,
    }_exposurErrors;

private slots:
    void handleCurrentStatus(void);

private:
    uchar interfaceStatus;
    bool generator_status_changed;

    bool wait_command_processed;
    uint8_t command_process_code;
    bool command_process_state;
    uint cp_timeout;

    bool    abortRxRequest;

    _statusManagerState requestState;
    _statusManagerState internalState;
    uchar subStatus;

    void  changeStatus(_statusManagerState new_status, uchar sub, _statusManagerState ret_status, uchar ret_sub);
    _statusManagerState returnState;
    uchar               returnSubStatus;



    void handleCommandProcessedState(void);
    void handle_SH_CONNECTION(void);
    void handle_GENERATOR_CONNECTION(void);
    void handle_SMS_MESSAGE_NOTIFY(void);
    void handle_CLEAR_ALL_SMS_MESSAGES(void);

    void handle_SMS_IDLE(void);
    void handle_SMS_ERROR(void);

    void handle_2D_MANUAL(void);
    void handle_2D_AEC(void);

    // Exposure parameters    
    QList<Interface::tPostExposureData> postExposureList;

    float       pre_kV;
    ushort      pre_mAs;


    float       pulse_kV;
    ushort      pulse_mAs;


    uchar       focus;
    uchar       fps;
    uchar       skeep_fps;
    ushort      mA;
    ushort      mS;
    bool        aecDataPresent;

    bool validate2DExposurePulse(void);
    bool validate2DExposurePre(void);

};

/*
 *
 */
#endif // STATUS_MANAGER_H
