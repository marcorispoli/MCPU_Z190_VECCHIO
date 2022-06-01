#ifndef DEVICECLASS_H
#define DEVICECLASS_H

#include <QObject>
#include "can_driver_class_interface.h"
#include "device_registers.h"


/**
 * \defgroup GANTRY_DEVICE Gantry Device Implementation
 * \ingroup GANTRY_DEVICE_INTERFACE
 *
 * @brief The deviceClass class
 * \nosubgrouping
 *
 * This class implements the communication protocol for the Gantry Devices.
 *
 * # General Description
 *
 * The class implements the specification of the device protocol
 * communication.
 *
 * In particolar:
 *
 * + The device provides a Parameter list array;
 * + The device provides a Data list array;
 * + The device provides a Status list array;
 * + The device provides APIs to handle the data handling;
 * + The device provides API's to handle the Device Command Execution.
 *
 *  > NOTE: The Device Class is in the same thread of the MAIN application!
 *
 * # Status Registers Handling
 *
 * The Device provides a list of basic Status registers.
 * As for the protocol specification, the first two Status registers shall be reserved for:
 * + Address 0: reserved;
 * + Address 1: SYSTEM REGISTER;
 * + Address 2: ERROR REGISTER;
 * + Address 3: COMMAND REGISTER;
 *
 * The Class subclassing the base class shall use the API ***addStatusRegister()*** in order
 * to add extra registers in the list.
 *
 * The application implementing a Device can read a Status register
 * using the convenient API ***downloadStatus(seq, index)***
 *
 * The function sends the READ STATUS formatted frame to the device
 * with the application seq number. After the function is used
 * the Status register changes its internal status to _DEVREG_PENDING
 * until the Device acknowledges the request with the actual
 * register data content.
 *
 * Excluding the special registers, for the custom device status register the Application:
 * + emit the signal statusChangedSgn(index) if the Status register content changes;
 * + emit the signal sequenceSgn(seq) always the command completes;
 *
 * # Data Registers Handling
 *
 * The Data registers are Device's registers that the Application modifies
 * runtime to control the behavior of the Device.
 *
 * The Data registers are Read Only for the Device and Read/Write for Application.
 *
 * The Application can Write the Data register with the function  ***uploadData()***
 *
 * When the application request to write a Data register, its internal status changes
 * in _DEVREG_PENDING, until the Device acknowledges the request with the actual
 * register data content.
 *
 * When the device acknowledges the request, a signal  ***sequenceSgn()***
 * is emitted.
 *
 * # Parameter Registers Handling
 *
 * The Parameter registers are Device's registers that sets the way the
 * Device behaves. The Parameters differ from the Data registers
 * because they can be stored in the Device non volatile memory and
 * retriven at startup time.
 *
 * The Application should change the Parameters only  during system configuration.
 *
 * The Parameter registers are Read Only for the Device and Read/Write for Application.
 *
 * The Application can Write a Data register with the function  ***uploadParam()***.
 * The Application can Store in non volatile memory a single Parameter register
 * or the whole parameter list with the function  ***storeParam()***.
 *
 *
 * When the application request to write a Parameter register, its internal status changes
 * in _DEVREG_PENDING, until the Device acknowledges the request with the actual
 * register data content.
 *
 * In either cases on which the Application request a Write Parameter or Store Parameter,
 * when the device acknowledges the request, a signal  ***sequenceSgn()***
 * is emitted.
 *
 * # Command Execution
 *
 * The Device protocol provides a method to request the Device
 * to execute a function.
 *
 * The Application, in order to request a command execution, shall
 * call the function  ***execCommand()***.
 *
 * When the Device acknowledges the command, the Application emit the signal ***execCmdSgn()***
 * with the Sequence message, the message code processed and the current execution status:
 *
 * + _EXECCMD_COMPLETED: the command has been istantly executed;
 * + _EXECCMD_PROCESSING: the command is in process;
 * + _EXECCMD_ERROR: the command cannot be executed because an error has been detected;
 *
 * In case the status should be _EXECCMD_PROCESSING, the application shall
 * poll the special *COMMAND*  register (address = 3)  in order to detect the execution progress.
 *
 * In case the *COMMAND* register *status* field should be  _EXECCMD_COMPLETED or _EXECCMD_ERROR
 * the Application will notify the command comletion event signaling the  ***execCmdSgn()***
 *
 *
 *
 */
class deviceClass: public QObject
{
    Q_OBJECT


    explicit deviceClass(uchar id); //< Class constructor

    /// Definition of the Command Frame Codes
    typedef enum{
        _DEVCMD_READSTAT,       //< Read Status register
        _DEVCMD_WRITEPAR,       //< Write Parameter register
        _DEVCMD_WRITEDATA,      //< Write Data register
        _DEVCMD_STOREPAR,       //< Store Parameter register
        _DEVCMD_EXEC            //< Execute Command
    }_DEVICE_COMMAND_FIELD;

    /// Execution Command Status
    typedef enum{
        _EXECCMD_COMPLETED = 0,     //< The command has been executed
        _EXECCMD_PROCESSING,        //< The command has been activated
        _EXECCMD_ERROR,             //< Error condition
    }_DEVICE_EXEC_STATUS;

    #define _DEVICE_SEQ_DATA_FRAME   0
    #define _DEVICE_CMD_DATA_FRAME   1
    #define _DEVICE_DATA0_DATA_FRAME 2
    #define _DEVICE_DATA1_DATA_FRAME 3
    #define _DEVICE_DATA2_DATA_FRAME 4
    #define _DEVICE_DATA3_DATA_FRAME 5
    #define _DEVICE_DATA4_DATA_FRAME 6
    #define _DEVICE_CRC_DATA_FRAME   7

    #define _DEVICE_REGINDEX_DATA_FRAME (_DEVICE_DATA0_DATA_FRAME)

    // Data position for Command Execution frames
    #define _DEVICE_CMDCODE_DATA_FRAME (_DEVICE_DATA0_DATA_FRAME)
    #define _DEVICE_CMDSTAT_DATA_FRAME (_DEVICE_DATA1_DATA_FRAME)
    #define _DEVICE_CMDD0_DATA_FRAME (_DEVICE_DATA2_DATA_FRAME)
    #define _DEVICE_CMDD1_DATA_FRAME (_DEVICE_DATA3_DATA_FRAME)
    #define _DEVICE_CMDD2_DATA_FRAME (_DEVICE_DATA4_DATA_FRAME)
    #define _DEVICE_CMDERR_DATA_FRAME (_DEVICE_DATA4_DATA_FRAME)

    // SPECIAL STATUS REGISTERS ADDRESS
    #define _DEVICE_RESERVED_REGSITER  0

    #define _DEVICE_SYSTEM_STATREG      1
    #define _DEVICE_ERRORS_STATREG      2
    #define _DEVICE_COMMAND_STATREG     3

    // COMMAND REGISTER DATA DEFINITION
    #define _DEVICE_COMMAND_CODE    0
    #define _DEVICE_COMMAND_SEQ     1
    #define _DEVICE_COMMAND_STAT    2
    #define _DEVICE_COMMAND_ERR     3


signals:
    /**
     * @brief This signal is generated when a Status register changes its value.
     *
     * When a decoded can frame relates to a READ-STATUS command, the STATUS register
     * content received from the device is compared with the current value. If the value should
     * change, the signal can be generated.
     *
     * @param index: this is the index of the changed Status register
     */
    void statusChangedSgn(uchar index);

    /**
     * @brief sequenceSgn
     *
     * This signal is generated always a transaction completes.
     * NOTE: in case of the Status register read, this signal is emitted AFTER
     * the statusChangedSgn();
     *
     * @param seq: this is the sequence of the transaction
     */
    void sequenceSgn(uchar seq);

    /**
     * @brief This signal is emitted when a command request is acknowledge
     *
     * When the Device acknowledges an Applicaion command request execCommand(),
     * the Application emits this signal with the Command status returned by the Device.
     *
     * @param seq: this is the command sequence number
     * @param cmdcode: this is the Execution command code
     * @param stat: this is the status of the Execution status (see deviceClass::_DEVICE_EXEC_STATUS)
     * @param err: execution command error condition
     * @param d0: D0 of the command completion;
     * @param d1: D1 of the command completion;
     */
    void execCmdSgn(uchar seq, uchar cmdcode, uchar stat, uchar err, uchar d0, uchar d1);

    /**
     * @brief sendToDevice
     * This signal is emitted internally whenever a public send function needs to send data to a driver.
     *
     * This signal is internally connected to a slot of the devicesInterface class.
     *
     *
     * @param seq: this is the command seqeunce number, reported in the acknowledge frame by the device
     *
     * @param id: this is the target device's address
     *
     * @param command: this is the command code
     *
     * @param d0: D0 of the command;
     * @param d1: D1 of the command;
     * @param d2: D2 of the command;
     * @param d3: D3 of the command;
     * @param d4: D4 of the command;
     */
    void sendToDevice(uchar seq, uchar id, uchar command, uchar d0, uchar d1, uchar d2, uchar d3, uchar d4); //< Signal to be connected to the Can Driver send

public:
    /**
     * @brief This function upload a Parameter register content into the target device
     * @param seq: this is the seqeunce message number
     * @param index; this is the index address of the register (1:255)
     * @param d0: register data 0;
     * @param d1: register data 1;
     * @param d2: register data 2;
     * @param d3: register data 3;
     */
    void uploadParam(uchar seq, uchar index, uchar d0, uchar d1, uchar d2, uchar d3);

    /**
     * @brief This function request to stores a single or the whole Device parameters set
     *
     * @param seq: this is the seqeunce number of the message
     * @param index: this is the register index.
     * If index == 0 then the Device shall store All the Parameters.
     */
    void storeParam(uchar seq, uchar index);

    /**
     * @brief This function upload a Data register content into the target device
     * @param seq: this is the sequence message number
     * @param index; this is the index address of the register (1:255)
     * @param d0: register data 0;
     * @param d1: register data 1;
     * @param d2: register data 2;
     * @param d3: register data 3;
     */
    void uploadData(uchar seq, uchar index, uchar d0, uchar d1, uchar d2, uchar d3);

    /**
     * @brief This function retrive the content of a given Status register of the target device
     * @param seq: this is the message seqeunce number
     * @param index: this is the requested index Status register
     */
    void downloadStatus(uchar seq, uchar index);

    /**
     * @brief This function request the Device to execute a given command
     * @param seq: this is the message seqeunce number
     * @param code: this is the requested command code (1:255)
     * @param d0: command param 0;
     * @param d1: command param 1;
     * @param d2: command param 2;
     * @param d3: command param 3;
     */
    void execCommand(uchar seq, uchar code, uchar d0, uchar d1, uchar d2, uchar d3);


    /**
     * @brief This function decodes the incoming CAN frame based on the Device Can Protocol Interface
     * @param rxFrame: this is the incoming Can frame    
     * @return
     * + TRUE: the frame has been correctly decoded;
     * + FALSE: the frame didn't match with the protocol criteria.
     */
    bool decodeFrame(canDriverInterface::canDataFrame* rxFrame);

    /**
     * @brief This virtual function handles the polling callbacks
     *
     * Subclass this function to implement a polling routine for the device.
     *
     * @return
     * + true if the polling is consumed;
     * + false if the polling isn't consumed;
     */
    virtual bool pollingHandler(void){return true;}

    /// Return the Address of the current device
    _inline uchar getId(){return ID;}

protected:
    uint addStatusRegister(deviceDataRegisterClass::_devreg_status stat = deviceDataRegisterClass::_DEVREG_PENDING); //< The subclass uses this convenient function to add extra Status Registers


    /// Get the command Status of the COMMAND register
    _inline uchar getCommandStatus(void){
        return statusList[_DEVICE_COMMAND_STATREG].data[_DEVICE_COMMAND_STAT];
    }

    /// Get the command Code of the COMMAND register
    _inline uchar getCommandCode(void){
        return statusList[_DEVICE_COMMAND_STATREG].data[_DEVICE_COMMAND_CODE];
    }

    /// Get the command Sequence of the COMMAND register
    _inline uchar getCommandSeq(void){
        return statusList[_DEVICE_COMMAND_STATREG].data[_DEVICE_COMMAND_SEQ];
    }

    /// Get the command Error of the COMMAND register
    _inline uchar getCommandErr(void){
        return statusList[_DEVICE_COMMAND_STATREG].data[_DEVICE_COMMAND_ERR];
    }

private:
    QList<deviceDataRegisterClass> paramList; //< List of the device parameters (read/write)
    QList<deviceDataRegisterClass> dataList; //< List of the device data (read/write)
    QList<deviceDataRegisterClass> statusList; //< List of the status registers (readOnly)

    uchar ID; //< ID address of the device
    uchar executingCommandCode; //< current execution command code
    uchar executingCommandSeq;  //< Sequence number of the current execution command code
    uchar executingCommandStatus; //< Current execution command status


};

#endif // DEVICECLASS_H

