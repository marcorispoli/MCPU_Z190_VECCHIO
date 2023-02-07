#ifndef PD4_H
#define PD4_H

#define cGRADsec_TO_ROT_min(x)  ((uint32_t) (((float)((int)x)  * (float) MOTOR_GEAR / 36000) * (float) SPEED_DENOMINATOR))
#define cGRAD_TO_ENCODER(x)        ((int) (((float) ((int)x) * (MOTOR_GEAR * 10) )/180))
#define ENCODER_TO_cGRAD(x)        ((int)((((float) ((int)x)) * 180) / (MOTOR_GEAR*10)))
#define END_ODVECTOR {0,0,(canOpenDictionary::_ODDataType) 0,0}


/**
 *  \defgroup nanotecModule Nanotec Devices Implementation module
 *
 *  This Module implements all the features related to the PD4 Nanotec \n
 *  motor devices.
 *
 *  Those Devices implement the Slave CANOpen protocol.
 *
 *  This module allows to:
 *  - Establish a connection with the Target PD4 Nanotec device through the Can Application Driver;
 *  - Handle the CAN Open communication;
 *  - Handle the CiA Status Machine of the device;
 *  - Provides the main controlling features:
 *    - Activating the Zero setting procedure;
 *    - Activating the positioning with or without the NanoJ program activation;
 *    - Handling the Configuration Uploading and storing of the main Device registers;
 *    - Handling the Error conditions;
 *    - handling the device Digital Input;
 *
 *    This module is based on two sub-moduls:
 *    - @ref pd4Module: used to implement the PD4 functions;
 *    - @ref canOpenDictionaryModule: to implement the Object Dictionary Management.
 */

/*!
 * \defgroup  pd4Module Nanotec PD4 Device implementation Module.
 *
 * This Module implements the communication protocol with the PD4 Nanotec Motor Device.
 *
 * # MODULE OVERVIEW
 *
 * This module connects a PD4 Nanotec based motor device handling all the
 * CANOpen based feature.
 *
 * The module has been developed to be subclassed by a Device specific implementation class.
 *
 *
 * The module allows to:
 * - Configure the whole device internal registers;
 * - Handle the Fault conditions;
 * - Activate a positioning procedure;
 * - Activate the zero setting procedure;
 * - Upload an optional nanoj program;
 * - Activate a nanoj program;
 * - Handle safety inputs;
 *
 * # DEVICE CONFIGURATION
 *
 * The module handles a set of configuration Register Vectors
 * in order to customize the motor behaviour.
 *
 * The Register Vectors are:
 * - pd4Nanotec::registerVectors.configVector: this is a register vector that is uploaded at the startup (if necessary) and stored into the device;
 * - pd4Nanotec::registerVectors.runtimeVector: this is a register vector that is always uploaded at the startup;
 * - pd4Nanotec::registerVectors.zeroSettingVector: this is a register vector that is uploaded at the beginning of a zero setting procedure;
 *
 * The Module provides the tools to configure a NanoJ program to be executed.
 *
 * The Module provides to configure the Digital inputs to act as safety motor interruption.
 *
 * ## REGISTER VECTOR DESCRIPTION
 *
 * The Register Vectors are actually Arrays of items of type pd4Nanotec::_OD_InitVector.\n
 * A valid Register Vector shall mandatory termines with the END_ODVECTOR macro.
 *
 *
 *      NOTE: In case the END_ODVECTOR should be omitted, the program may crash
 *      due to a possible infinite loops.
 *
 *
 * ## CONFIGURATION REGISTER VECTOR DESCRIPTION
 *
 * This Register Vector should be a const vector of Motor registers that\n
 * needs to be uploaded once and stored into the Motor Device.\n
 *
 * The Module Uploads and Stores the registers soon after the start-up\n
 * if the Module detects a different checksum from the Device list and the \n
 * coded list. In case the checksum should match, the register vector will not be\n
 * uploaded, saving startup time and can network occupancy.\n
 *
 * ## RUNTIME REGISTER VECTOR DESCRIPTION
 *
 * This register vector is a list of device register that will be always upload \n
 * after every startup.
 *
 *
 *      NOTE: although this is an optional feature, however there is some internal register\n
 *      that cannot be stored for an unknown reason, not declared by Nanotec. Those registers\n
 *      consequently needs to be refreshed after the system startup.
 *
 *      NOTE: the known register that refuses to be stored are:
 *      - OD_3250_02
 *
 * This Register vector can be declared as a const vector because \n
 * it will not be changed at runtime.\n
 *
 * ## ZERO SETTING REGISTER VECTOR
 *
 * This register vector is a list of register that characterize the \n
 * zero setting behavior. Those register will be uploaded at the beginning \n
 * of the zero setting procedure.\n
 *
 * This Register vector can be declared as a const vector because \n
 * it will not be changed at runtime.\n
 *
 *
 * ## NANOJ PROGRAM
 *
 * The module allows to automatically upload the nanoj program at the startup.
 * The pd4Nanotec::nanojStr.vector shall be assigned to the array of bytes \n
 * of the binary nanojprogram.
 *
 * The module provides the method pd4Nanotec::setNanojVector() in order to\n
 * assigne the binary array to the module pointer.
 *
 * The Module checks the checksum of the program and compares it with the
 * last stored into the flash memory of the Motor device. In case it should \n
 * be different, the module automatically upload the new program.
 *
 * The Application can force to upload and store a new program with the
 * method:
 * - pd4Nanotec::uploadNanojProgram()
 *
 *
 * ## SAFETY CONFIGURATION
 *
 * The module allows to use the Digital Inputs as a software safety Motor Interruption.
 *
 * The PD4 device provides four digital inputs. The Subclassing application
 * can set the mask and the value corresponding with the stop condition:
 * - Stop Condition: (DI & SAFE_MASK) == SAFE_VAL;
 *
 * The Safety Stop condition causes an immediate Motor Stop.
 *
 * \ingroup nanotecModule
 */



#include <QtCore>
#include "dictionary.h"

/*
 *
  - connection status: (String) [CONNECTED/NOT_CONNECTED];
*  - operating status:  (String) [READY/NOT_ZERO_SET/NOT_INITIALIZED/ERROR]
*  - current activity:  (String) [INITIALIZING/ZERO_SETTING/POSITIONING/NO_ACTIVITY]
*  - encoder position:  (int)    hundredths of degree or millimeters
*/

class pd4Nanotec: public QObject
{
    Q_OBJECT

public:
    explicit pd4Nanotec(uchar ID);
    ~pd4Nanotec();

    typedef struct{
      ushort index;
      uchar subidx;
      canOpenDictionary::_ODDataType type;
      uint val;
    }_OD_InitVector;

    void run(void);
    void stop(void);
    _inline bool isStopMode(void){return (workflow == _DEVICE_STOP_MODE);}
    _inline bool isConfigMode(void){return (workflow == _DEVICE_INIT);}
    _inline bool isConfigCompleted(void){return (deviceInitialized);}
    _inline bool isOperating(void){return (CiAcurrentStatus == CiA402_SwitchedOn);}
    _inline bool isFault(void){return (CiAcurrentStatus == CiA402_Fault);}
    _inline bool isCommandExecuting(void){return (execCommand != _NO_COMMAND);}

    void setNanojVector(uchar* p, uint size);
    void setConfigVector(_OD_InitVector* p);
    void setRuntimeVector(_OD_InitVector* p);
    void setZerosettingVector(_OD_InitVector* p);
    void setMotorConfig(float gearratio, float speed_denominator);

    uint  convert_cGRADsec_TO_ROT_min(float x);
    int   convert_cGRAD_TO_EncoderUnit(float x);
    int   convert_EncoderUnit_TO_cGRAD(float x);

    void enableConfiguration(void);
    bool activateZeroSetting(void);
    bool activatePositioning(int cAngle, uint cAcc, uint cDec, uint cSpeed, bool nanoj_start);

    void immediateStop(void);
    void quickStop(void);
    void uploadNanojProgram(void);

    uchar getDigitalInputs(void);
    void setSafetyDigitalInput(uchar mask, uchar val);
    bool isSafetyDigitalInputOn(void);



signals:
    void txToCan(ushort canId, QByteArray data); //!< Sends Can data frame to the canDriver


protected:

    void writeSDO(ushort index, uchar sub, canOpenDictionary::_ODDataType type, uint val);
    void writeSDO(canOpenDictionary od);
    void nanojWritedata(void);
    void readSDO(ushort index, uchar sub, uchar type);
    void readSDO(canOpenDictionary od);
    void resetNode(void);

private:
    uchar deviceId; //!< Target Can device ID
    clock_t  t1; //!> Time performance measurement variable

    ushort workflowStatus; //!> Workflow status index
    ushort wStatus; //!> Callback status index
    ushort wSubStatus; //!> Callback Sub Status index

    bool stopMode; //!< The device is in stop Mode
    bool ready; //!< The Can Driver is ready to receive data
    bool zero_setting_ok;//!< Flag of the zero setting completed
    bool enableDeviceInitialization; //!< Enable the initialization process
    bool deviceInitialized; //!< Flag of a correected initialization process

    bool nanojUploaded;
    bool uploadNanojRequest;

    struct{
        _OD_InitVector* configVector;
        _OD_InitVector* runtimeVector;
        _OD_InitVector* zeroSettingVector;
    }registerVectors;

    struct{
        float gearratio;
        float speed_denominator;
    }motorConfig;


    typedef enum{
        _NO_COMMAND=0,
        _ZERO_SETTING_COMMAND,
        _POSITIONING_COMMAND,
    } _executionCommands;
    _executionCommands execCommand;

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

    _CiA402Status getCiAStatus(canOpenDictionary* od);
    QString getCiAStatusString(_CiA402Status status);

    typedef enum{
        _DEVICE_NOT_CONNECTED = 0,
        _DEVICE_STOP_MODE,
        _DEVICE_INIT,
        _UPLOAD_NANOJ,
        _HANDLE_DEVICE_STATUS,
    } _workflowType;


    _workflowType workflow; //!< Main workflow routine control word
    _CiA402Status CiAcurrentStatus; //!< CiA current detected status

    /// This structure handle the variables to control the Send/Receive process
    struct {
        bool sdo_rx_tx_pending;     //! The Workflow is waiting for a rxtx transaction completion
        bool sdo_rxtx_completed;    //! The SDO transaction  has been completed (successfuffly or with error)
        bool sdo_rx_ok;             //! The SDO has been received in a correct format
        uchar sdo_attempt;          //!< Current Rx/Tx attempt number
        uchar tmo_attempt;          //!< Number of attempts without reception
        canOpenDictionary rxSDO;    //!< Received SDO
        canOpenDictionary txSDO;    //!< SDO to be transmetted
    }sdoRxTx; //!< Data structure controlling the Send and Reception process


    /// Memory Block control
    struct{
        bool    rxblock;               //! The next reception is a block type
        QByteArray txBlock;            //! < Tx data block sent
        QByteArray rxBlock;            //! < Rx data block sent
        uint    vector_index;          //! Index of the current data to be written in the nanoj memory
        uint    num_block_items;       //! Counter of the total item currently in the block (max 1024)
        uchar*   vector;
        uint    sizeofNanoj;
        bool    nanoj_program_started; //!< The Nanoj program has been activated
    } nanojStr;

    typedef struct{
        uchar digital_input_mask;    //!< Input mask to use digital inputs as safety
        uchar digital_input_val;     //!< Input statusof digital inputs to activate the safety
        bool immediate_stop_command; //!< Activate with the immediateStop() interface function to istantly stop the motor
        bool quick_stop_command;     //!< Stop with higher deceleration rate
    }_SafetyStructure;

    ushort  subRoutineUploadVector(_OD_InitVector* pVector, bool* changed, bool* uploadOk);
    ushort  subRoutineWriteRegisterList(QList<canOpenDictionary>* podList,  bool* uploadOk);
    ushort  subDisableNanojProgram(void);

    ushort  subReadDigitalInputs(void);
    ushort  subReadPositionEncoder(void);
    ushort  subActivateQuickStopCommand(void);
    void    sendAgainSDO(void);
    ushort  subNanojProgramUpload(bool force);
    QString getErrorCode1003(uint val);
    QString getErrorClass1003(uint val);
    QString getErrorClass1001(uint val);

    struct{
        QList<canOpenDictionary> registers; //!< List of the registers to be uploaded in the Positioning
        int target;
        uint cAcc;
        uint cDec;
        uint cSpeed;
        bool target_ok; //!< Flag of correct positioning
        bool nanoj_start;   //!< Nanoj Activation request
    }positioningStr;


    bool digital_input_valid; //!< Sets the validity of the current digital input value
    uint digital_input_val; //!< status of the motor digital inputs
    int position_encoder_val; //!< last read position encoder

    // Safety Handling
    _SafetyStructure safety; //!< Handles the safety of the activation


private slots:

    ushort CiA402_SwitchOnDisabledCallback(void);
    ushort CiA402_ReadyToSwitchOnCallback(void);
    ushort CiA402_OperationEnabledCallback(void);
    ushort CiA402_FaultCallback(void); //!< Motor Fault callback
    ushort CiA402_SwitchedOnCallback(void);


    ushort initPd4ZeroSettingCommand(void);//!< Motor Zero Setting Initialization Procedure
    ushort pd4ZeroSettingLoop();//!< Motor Zero Setting Procedure

    ushort initPd4PositioningCommand(void);//!< Motor Positioning Initialization Procedure
    ushort pd4PositioningLoop();//!< Motor Positioning Procedure

    void statusHandler(void); //!< Main callback handling the cia status
    void rxFromCan(ushort canId, QByteArray data);//!< Receive Can data frame from the canDriver
    ushort workflowInitCallback(void); //!< Workflow of Initialization procedures after the reset
    ushort workflowUploadNanoj(void); //!< Workflow of Upload Nanoj-program

};



#endif // PD4_H
