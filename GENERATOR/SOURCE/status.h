#ifndef STATUS_MANAGER_H
    #define STATUS_MANAGER_H

#include <QObject>
#include "Typedef.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>

class DbManager
{
public:
    DbManager(const QString& path);

    QString getErrorString(ulong errcode);

private:
    QSqlDatabase m_db;
};


/**
 * @brief This is the statusManager implementation class
 *  \nosubgrouping
 *
 * This class implements a Status Machine to control
 * the Generator workflow.
 *
 * The basic status that the class handle are:
 *     + SMS_SMART_HUB_CONNECTION: this is the status controlling the connection with the smart Hub
 *     + SMS_WAIT_GENERATOR_CONNECTION,
 *     SMS_SETUP_GENERATOR,
 *     SMS_SERVICE,
 *     SMS_IDLE,
 *     SMS_CLEAR_SYS_MESSAGES,
 *
 *
 * # Template Hardware Communication Class
 * This module makes use of an arbitrary communication API.
 * The class implementing the Hardware Communication API shall
 * implement the following interface methods:
 * be passed as a Template class
 *
 *
 */
class statusManager : public QObject
{
    Q_OBJECT

public:
    explicit statusManager(QObject *parent = nullptr);

    /// This is the enumeration for the focal spot selection
    typedef enum{
        _FOCUS_SMALL = 0, //!< [0] Small focus
        _FOCUS_LARGE = 1  //!< [1] Large Focus
    }tExposureFocus;


    /// This structure provide the varible of an exposure
    typedef struct{
        ushort tmo;     //!< Exposure timeout when applicable;
        float kV;       //!< kV to be used;
        float mAs;      //!< mAs
        float mS;       //!< milliseconds for exposure
        float mA;       //!< Anodic current in mA
        tExposureFocus focus;    //!< focus to be used
        bool valid;     //!< data are valid for he next exposure
    }_tExposureData;

    /// This is the structure defineing the Tomo configuration scan
    typedef struct{
        uchar samples;  //!< Number of valid samles in the scan
        uchar skip;     //!< Number of initial pulses to be skip
        ushort EW;      //!< Maximum integration time
        bool valid;     //!< Data are valid for the next exposure
        bool changed;   //!< The configuration has been changed
    }_tTomoConfig;

    /// This is the structure defining the exposure options
    typedef struct{
        bool use_detector;  //!< The exposure will use the detector synch signals
        bool use_grid;      //!< The exposure will use the grid synch signals
    }_tExposurOpt;

    /**
     * @brief
     * This function set the options to be applied in the next exposure sequence.
     *
     *
     * @param
     * - use_detector: this option enable the synch with the detector
     * - use_grid: this option activate the sync with the grif device
     *  > NOTE: use_detector and use_grid options applies only to 2D Manual exposure.
     *
     *  \ingroup statusModule
     */
    void setExposureOptions(bool use_detector , bool use_grid ){

       exposureOptions.use_detector = use_detector;
       exposureOptions.use_grid = use_grid;

    }

    /**
     * @brief
     * This function set the exposure data for the Pre Pulse in a AEC sequence.
     *
     * @param
     * - focus: is the current focus. See tExposureFocus;
     * - kv: is the kV value in decimal format;
     * - mAs: mAs in integer format;
     *
     *  \ingroup statusModule
     */
    void setPreData(uchar focus, float kv, float mAs, ushort tmo){
        preExposureData.tmo = tmo;
        preExposureData.focus = (tExposureFocus) focus;
        preExposureData.kV = kv;
        preExposureData.mAs =  mAs;
        preExposureData.valid = true;
    }

    /**
     * @brief
     * This function set the exposure data for the Pulse.
     *
     * @param
     * - focus: is the current focus. See tExposureFocus;
     * - kv: is the kV value in decimal format;
     * - mAs: mAs in integer format;
     *
     *  \ingroup statusModule
     */
    void setPulseData(uchar foc, float kv, float mAs){

        pulseExposureData.focus = (tExposureFocus) foc;
        pulseExposureData.kV = kv;
        pulseExposureData.mAs = mAs;
        pulseExposureData.valid = true;
    }

    /**
     * @brief
     * This function set the configuration of the next tomo sequence.
     *
     * @param
     * - nsamples: number of valid samples;
     * - nskip: number of initial pulses to be skip;
     * - ew: maximum integration time;
     *
     *  \ingroup statusModule
     */
    void setTomoConfig(uchar nsamples, uchar nskip, ushort ew){

        // Determines if the sequence shall rebuild the procedure
        if( (nsamples != tomoConfig.samples) || (nskip != tomoConfig.skip)) tomoConfig.changed = true;
        else tomoConfig.changed = false;

        tomoConfig.samples = nsamples;
        tomoConfig.skip = nskip;
        tomoConfig.EW = ew;
        tomoConfig.valid = true;
        return;
    }

    _inline void setGeneratorStatusChanged(void ) {generator_status_changed = true;}
    _inline void start2DExposure(void){ abortRxRequest = false; requestState = SMS_EXECUTING_2D_MANUAL;  }
    _inline void start2DAecExposure(void){ abortRxRequest = false; requestState = SMS_EXECUTING_2D_AEC;  }
    _inline void start3DExposure(void){ abortRxRequest = false; requestState = SMS_EXECUTING_3D_MANUAL;  }
    _inline void start3DAecExposure(void){ abortRxRequest = false; requestState = SMS_EXECUTING_3D_AEC;  }


    _inline bool isIdle(void){ return  (internalState == SMS_IDLE ); }
    _inline bool requestAbortProcedure(void){ abortRxRequest = true; }
    _inline uchar getStatus(void){ return  interfaceStatus;}

    _inline void clearPostExposureList(void){ postExposureList.clear();}
    _inline QList<Interface::tPostExposureData> getPostExposureList(void){ return postExposureList;}
    _inline uint getPostExposureListSize(void){ return postExposureList.size();}


    void setPostExposureData(uchar pulse_seq, uchar foc, float kV, float mAs, float mA, float ms, uchar result){
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
      SMS_SETUP_GENERATOR,
      SMS_SETUP_PROCEDURE,
      SMS_SERVICE,
      SMS_IDLE,
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
      SMS_EXP_ERR_NONE = 0,
      SMS_EXP_ERR_SYSMSG,
      SMS_EXP_ERR_PULSE_VALIDATION,
      SMS_EXP_ERR_PRE_VALIDATION,
      SMS_EXP_ERR_PREPARATION,
      SMS_EXP_ERR_SEQUENCE,
    }_exposurErrors;

private slots:
    void handleCurrentStatus(void);

    void timeoutAec(void){
        abortTimeoutRequest = true;
    }
private:
    DbManager* DBSysMessages;

    uchar interfaceStatus;
    bool generator_status_changed;

    bool wait_command_processed;
    uint8_t command_process_code;
    bool command_process_state;
    uint cp_timeout;

    bool    abortRxRequest;
    bool    abortTimeoutRequest;

    _statusManagerState requestState;
    _statusManagerState internalState;
    uchar subStatus;

    void  changeStatus(_statusManagerState new_status, uchar sub, _statusManagerState ret_status, uchar ret_sub);
    _statusManagerState returnState;
    uchar               returnSubStatus;



    void handleCommandProcessedState(void);
    void handle_SH_CONNECTION(void);
    void handle_GENERATOR_CONNECTION(void);
    void handle_SETUP_GENERATOR(void);


    void handle_CLEAR_ALL_SMS_MESSAGES(void);

    void handle_SMS_IDLE(void);
    void handle_SMS_ERROR(void);

    void handle_2D_MANUAL(void);
    void handle_3D_MANUAL(void);
    void handle_2D_AEC(void);
    void handle_3D_AEC(void);


    // Exposure parameters    
    QList<Interface::tPostExposureData> postExposureList;

    _tExposureData preExposureData;
    _tExposureData pulseExposureData;
    _tExposurOpt   exposureOptions;
    _tTomoConfig   tomoConfig;
    bool procedureCreated;


};

/*
 *
 */
#endif // STATUS_MANAGER_H
