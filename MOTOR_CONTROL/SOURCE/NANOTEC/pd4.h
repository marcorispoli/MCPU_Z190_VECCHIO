#ifndef PD4_H
#define PD4_H

#define cGRADsec_TO_ROT_min(x)  ((uint32_t) (((float)((int)x)  * (float) RIDUZIONE / 36000) * (float) SPEED_DENOMINATOR))
#define cGRAD_TO_POS(x)        ((int) (((float) ((int)x) * (RIDUZIONE * 10) )/180))
#define POS_TO_cGRAD(x)        ((int)((((float) ((int)x)) * 180) / (RIDUZIONE*10)))


/*!
 * \defgroup  nanotecModule Nanotec PD4 Device Communication Module.
 *
 * This Module implements the communication protocol with the PD4 Nanotec Motor Device.
 *
 * \ingroup canOpenModule
 */

#include <QtCore>
#include "dictionary.h"

class pd4Nanotec: public QObject
{
    Q_OBJECT

public:
    explicit pd4Nanotec(uchar ID);
    ~pd4Nanotec();

    void run(void);

    typedef struct{
      ushort index;
      uchar subidx;
      canOpenDictionary::_ODDataType type;
      ulong val;
    }_OD_InitVector;


    typedef enum{
        CiA402_NotReadyToSwitchOn = 0,
        CiA402_SwitchOnDisabled,
        CiA402_ReadyToSwitchOn,
        CiA402_SwitchedOn,
        CiA402_OperationEnabled,
        CiA402_QuickStopActive,
        CiA402_FaultReactionActive,
        CiA402_Fault,
        CiA402_Undefined,
    }_CiA402Status;

    _CiA402Status getCiAStatus(canOpenDictionary* od){
        uchar val = (uchar) (od->getVal() & 0xFF);
        if((val & 0x4F) == 0 ) return  CiA402_NotReadyToSwitchOn;
        else if((val & 0x4F) == 0x40 ) return  CiA402_SwitchOnDisabled;
        else if((val & 0x6F) == 0x21 ) return  CiA402_ReadyToSwitchOn;
        else if((val & 0x6F) == 0x23 ) return  CiA402_SwitchedOn;
        else if((val & 0x6F) == 0x27 ) return  CiA402_OperationEnabled;
        else if((val & 0x6F) == 0x7 ) return  CiA402_QuickStopActive;
        else if((val & 0x4F) == 0xF ) return  CiA402_FaultReactionActive;
        else if((val & 0x4F) == 0x8 ) return  CiA402_Fault;
        return CiA402_NotReadyToSwitchOn;
    }

    QString getCiAStatusString(_CiA402Status status){
        switch(status){
            case CiA402_NotReadyToSwitchOn: return QString("CiA402_NotReadyToSwitchOn"); break;
            case CiA402_SwitchOnDisabled: return QString("CiA402_SwitchOnDisabled"); break;
            case CiA402_ReadyToSwitchOn: return QString("CiA402_ReadyToSwitchOn"); break;
            case CiA402_SwitchedOn: return QString("CiA402_SwitchedOn"); break;
            case CiA402_OperationEnabled: return QString("CiA402_OperationEnabled"); break;
            case CiA402_QuickStopActive: return QString("CiA402_QuickStopActive"); break;
            case CiA402_FaultReactionActive: return QString("CiA402_FaultReactionActive"); break;
            case CiA402_Fault: return QString("CiA402_Fault"); break;
        }
    }

    typedef enum{
        _DEVICE_INIT = 0,
        _GET_CIA_STATUS,
        _HANDLE_DEVICE_STATUS,
    } _workflowType;

    typedef enum{

        _NO_COMMAND=0,
        _ZERO_SETTING_COMMAND,
        _POSITIONING_COMMAND,

    } _executionCommands;

    QString getErrorCode1003(ulong val);
    QString getErrorClass1003(ulong val);
    QString getErrorClass1001(ulong val);
    void setSafetyDigitalInput(uchar mask, uchar val){
        safety.digital_input_mask = mask;
        safety.digital_input_val = val;
    };

    bool isSafetyDigitalInputOn(){
        if(!digital_input_valid ) return false;
        if(!safety.digital_input_mask) return false;
        return ( (digital_input_val & safety.digital_input_mask) == (safety.digital_input_val & safety.digital_input_mask));
    }

    bool activateZeroSetting(void);
    bool activatePositioning(int cAngle);
    void immediateStop(void){
        safety.stop_command = true;
    }

    ulong   _cGRADsec_TO_ROT_min(float x){
        return (ulong) (x  * gearratio * speed_denominator / 36000);
    }

    int   _cGRAD_TO_POS(float x){
        return (int) (x  * gearratio * 10 / 180);
    }

    int   _POS_TO_cGRAD(float x){
        return (int) (x  * 180 / (gearratio * 10));
    }


signals:
    void txToCan(ushort canId, QByteArray data); //!< Sends Can data frame to the canDriver


public slots:

    virtual ushort idleCallback(void); //!< Idle status (CiA_SwitchedOn status)
    virtual ushort runCallback(void){return 0;}  //!< Motor activation control callback (CiA_OperationEnabled)


protected:
    float gearratio;
    float speed_denominator;

    uchar deviceId; //!< Target Can device ID
    clock_t  t1; //!> Time performance measurement variable
    ushort wStatus; //!> Callback status index
    ushort wSubStatus; //!> SubRoutine status index


    void writeSDO(ushort index, uchar sub, canOpenDictionary::_ODDataType type, ulong val);
    void readSDO(ushort index, uchar sub, uchar type);


    canOpenDictionary rxSDO; //!< Received SDO
    canOpenDictionary txSDO; //!< SDO to be transmetted


    bool deviceInitialized;
    _OD_InitVector* initVector;
    _OD_InitVector* zeroSettingVector;
    _OD_InitVector* positionSettingVector;


    _executionCommands execCommand;
private:

    _workflowType workflow; //!< Main workflow routine control word
    _CiA402Status CiAcurrentStatus; //!< CiA current detected status

    /// This structure handle the variables to control the Send/Receive process
    typedef struct {
        bool sdo_rx_tx_pending;     //! The Workflow is waiting for a rxtx transaction completion
        bool sdo_rxtx_completed;    //! The SDO transaction  has been completed (successfuffly or with error)
        bool sdo_rx_ok;             //! The SDO has been received in a correct format
        uchar sdo_attempt;          //!< Current Rx/Tx attempt number
        uchar tmo_attempt;          //!< Number of attempts without reception
    }_sdoRxTxControl;

    _sdoRxTxControl sdoRxTx; //!< Data structure controlling the Send and Reception process

    bool ready; //!< The Can Driver is ready to receive data
    bool zero_setting_ok;//!< Flag of the zero setting completed
    bool positioning_ok; //!< Flag of correct positioning


    ushort subRoutineUploadVector(_OD_InitVector* pVector, bool* changed, bool* uploadOk);
    void sendAgainSDO(void);

    bool digital_input_valid; //!< Sets the validity of the current digital input value
    uchar digital_input_val; //!< status of the motor digital inputs

    typedef struct{
        uchar digital_input_mask;    //!< Input mask to use digital inputs as safety
        uchar digital_input_val;     //!< Input statusof digital inputs to activate the safety
        bool stop_command;          //!< Activate with the immediateStop() interface function to quicly stop the motor
    }_SafetyStructure;

    // Safety Handling
    _SafetyStructure safety; //!< Handles the safety of the activation


private slots:

    ushort CiA402_SwitchOnDisabledCallback(void);
    ushort CiA402_ReadyToSwitchOnCallback(void);
    ushort CiA402_OperationEnabledCallback(void);
    ushort CiA402_FaultCallback(void); //!< Motor Fault callback



    ushort initPd4ZeroSettingCommand(void);//!< Motor Zero Setting Initialization Procedure
    ushort pd4ZeroSettingLoop();//!< Motor Zero Setting Procedure

    ushort initPd4PositioningCommand(void);//!< Motor Positioning Initialization Procedure
    ushort pd4PositioningLoop();//!< Motor Positioning Procedure

    void statusHandler(void); //!< Main callback handling the cia status
    void rxFromCan(ushort canId, QByteArray data);//!< Receive Can data frame from the canDriver
    ushort initCallback(void); //!< Initialization procedures after the reset


};



#endif // PD4_H
