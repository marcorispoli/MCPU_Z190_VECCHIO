#ifndef CANOPENINTERFACE_H
#define CANOPENINTERFACE_H


#include <QObject>
#include <QVariant>
#include <QThread>

#include "canDriverClassInterface.h"


/**
 * @brief This class implements the Object Dictionary register
 *
 * The OD Register is characterized to have:
 * + 16 bit address Idx;
 * + 8 bit sub address subIdx;
 * + 4 byte of data;
 * + a pre defined data type identificator.
 *
 * The class handle the data creation and the
 * data decoding.
 */
class odRegister{
public:

    //! Definition of the register data type
    typedef enum{
        _8BIT_DATA=0,
        _16BIT_DATA=1,
        _24BIT_DATA=2,
        _32BIT_DATA=3
    }_odDataType;

    //! Data type codes of the CANopen frame (data[0])
    typedef enum{
        _8BIT_RD  = 0x4F, //!< 8 Bit data type of a Read access
        _16BIT_RD = 0x4B, //!< 16 Bit data type of a Read access
        _24BIT_RD = 0x47, //!< 24 Bit data type of a Read access
        _32BIT_RD = 0x43, //!< 32 Bit data type of a Read access
        _8BIT_WR  = 0x2F, //!< 8 Bit data type of a Write access
        _16BIT_WR = 0x2B, //!< 16 Bit data type of a Write access
        _24BIT_WR = 0x27, //!< 24 Bit data type of a Write access
        _32BIT_WR = 0x23,  //!< 32 Bit data type of a Write access
        _ERROR_FRAME = 0x80, //!< The received frame is an error frame
        _VOID_FRAME = 0 //!< Data object with no content
    }_odFrameType;

    typedef enum {
        _OD_ACTION_UNDEFINED = 0,   //!< No action is pending
        _OD_ACTION_COMMAND_REQUEST, //!< A Command action request has been initiated
        _OD_ACTION_COMMAND_REQUEST_ERROR, //!< The command request has been rejected or error terminated in fault
        _OD_ACTION_COMMAND_EXECUTING, //!< The command request has been accepted and in process
        _OD_ACTION_COMMAND_COMPLETED, //!< A Command action completed with success.
        _OD_ACTION_COMMAND_COMPLETED_WITH_ERROR, //!< A current Command action terminated with error
        _OD_ACTION_READ_PENDING,    //!< A read action is processing
        _OD_ACTION_READ_COMPLETED,  //!< A read action completed with success. Data is available
        _OD_ACTION_READ_ERROR,      //!< A current read action terminated with error
        _OD_ACTION_WRITE_PENDING,   //!< A write action is processing
        _OD_ACTION_WRITE_COMPLETED, //!< A write action completed with success.
        _OD_ACTION_WRITE_ERROR,      //!< A current write action terminated with error
        _OD_ACTION_TYPE_FORMAT_ERROR, //!< A wrong data type received by device
    }odActionStatus;

    //! Void Object Register constructor
    odRegister(void){
        frameType = _VOID_FRAME;
        dtype = _8BIT_DATA;
        actionStatus = _OD_ACTION_UNDEFINED;
        for (int i=0; i<4; i++) data[i] = 0;
        for (int i=0; i<4; i++) error[i] = 0;
        return;
    };

    //! Object Register constructor with individual assignment
    odRegister(unsigned short node, int idx, int sub, _odDataType dT){

        actionStatus = _OD_ACTION_UNDEFINED;
        nodeId = node;       
        dtype = dT;

        // Assignes the address
        this->idx = idx;
        this->subIdx = sub;
        for (int i=0; i<4; i++) data[i] = 0;
        for (int i=0; i<4; i++) error[i] = 0;

    };

    /**
     * @brief This function returns the data of the od, formatted with the assigned type.
     *
     * During the OD instancing the data type is pre-determined, following
     * the CANOpen protocol specification.
     *
     * There are four possibles types: 8/16/24/32 bit.
     *
     * The function returns a T type converter of an integer value
     * calculated on the odType assigned.
     *
     * @return
     * + (T) d[0] if data type is 8 bit;
     * + (T) d[0] + 256 * d[1] if data type is 16 bit;
     * + (T) d[0] + 256 * d[1] + 0x10000 * d[2] if data type is 24 bit;
     * + (T) d[0] + 256 * d[1] + 0x10000 * d[2] + 0x1000000 * d[3] if data type is 32 bit;
     */
    template <typename T> T getData(void){
        if(dtype == _8BIT_DATA) return (T) (data[0]);
        else if(dtype == _16BIT_DATA) return (T) (data[0] + ((T)data[1] << 8));
        else if(dtype == _24BIT_DATA) return (T) (data[0] + ((T)data[1] << 8) + ((T)data[2] << 16));
        else return (T) (data[0] + ((T)data[1] << 8) + ((T)data[2] << 16)+ ((T)data[3] << 24));
    }

    canDriverInterface::canDataFrame toReadCanFrame(void){
        canDriverInterface::canDataFrame frame;
        frame.canId = 0x600 + nodeId;
        frame.data[0] = 0x40;
        frame.data[1] = (unsigned char) (idx);
        frame.data[2] = (unsigned char) (idx >> 8);
        frame.data[3] = (unsigned char) (subIdx);
        for (int i=0; i<4; i++) frame.data[4+i] = data[i];
        return frame;
    }

    canDriverInterface::canDataFrame toWriteCanFrame(void){
        canDriverInterface::canDataFrame frame;
        frame.canId = 0x600 + nodeId;

        if(dtype == _8BIT_DATA) frame.data[0] = _8BIT_WR;
        else if(dtype == _16BIT_DATA) frame.data[0] = _16BIT_WR;
        else if(dtype == _24BIT_DATA) frame.data[0] = _24BIT_WR;
        else frame.data[0] = _32BIT_WR;

        frame.data[1] = (unsigned char) (idx);
        frame.data[2] = (unsigned char) (idx >> 8);
        frame.data[3] = (unsigned char) (subIdx);
        for (int i=0; i<4; i++) frame.data[4+i] = data[i];
        return frame;
    }



     _inline unsigned short getNodeId(void){
        return nodeId;
     }
     _inline unsigned short getIdx(void){
        return idx;
     }
     _inline unsigned char getSubIdx(void){
        return subIdx;
     }

     _inline unsigned int getError(void){
        return (error[0] + (unsigned int) (error[1] << 8) + (unsigned int) (error[2] << 16) + (unsigned int) (error[3] << 24));
     }



    /**
     * @brief This function returns the List of raw four byte.
     *
     * @return (unsigned char*) register data pointer;
     */
    _inline unsigned char* getData(void){
        return data;
    }

    template <typename T> void  setData(T d){
        data[0] = (unsigned char) d;
        data[1] = (unsigned char) (d>>8);
        data[2] = (unsigned char) (d>>16);
        data[3] = (unsigned char) (d>>24);
    }

    void setData(const canDriverInterface::canDataFrame& frame){
        for (int i=0; i<4; i++) data[i] = frame.data[4+i];
    };
    void setError(const  canDriverInterface::canDataFrame& frame){
        for (int i=0; i<4; i++) error[i] = frame.data[4+i];
    };
    void setError(uint val){
        error[0] = (uchar) val;
        error[1] = (uchar) (val >> 8);
        error[2] = (uchar) (val >> 16);
        error[3] = (uchar) (val >> 24);
    };

    _inline _odDataType getDataType(void){
        return dtype;
    }
    _inline void setOdStat(odActionStatus stat){
        actionStatus = stat;
    }
    _inline odActionStatus getOdStat(){
        return actionStatus;
    }

    static _inline bool isReadFrame(const canDriverInterface::canDataFrame& frame){
        if((frame.data[0] == _8BIT_RD ) || (frame.data[0] == _16BIT_RD )|| (frame.data[0] == _24BIT_RD )|| (frame.data[0] == _32BIT_RD )) return true;
        return false;
    }

    static _inline bool isWriteFrame(const canDriverInterface::canDataFrame& frame){
        if(frame.data[0] == 0x60 ) return true;
        return false;    
    }

    static _inline bool isErrorFrame(const canDriverInterface::canDataFrame& frame){
        if(frame.data[0] == _ERROR_FRAME )  return true;
        return false;
    }

    static _inline unsigned short getCanId(const canDriverInterface::canDataFrame& frame){
        return frame.canId;
    }
    static _inline unsigned short getNodeId(const canDriverInterface::canDataFrame& frame){
        return (frame.canId &0x7F);
    }
    static _inline unsigned short getIdx(const canDriverInterface::canDataFrame& frame){
        return frame.data[1] + (unsigned short) (frame.data[2] << 8) ;
    }
    static _inline unsigned char getSubIdx(const canDriverInterface::canDataFrame& frame){
        return frame.data[3] ;
    }




private:
    unsigned short nodeId; //!< This is the node id of the target device
    unsigned short idx; //!< Object Register Idx field
    unsigned char  subIdx; //!< Object register sub index
    _odDataType    dtype; //!< Is the data type content (8/16/24/32 bit)
    _odFrameType   frameType; //!< Defines the frame packet code (Read/Write/Error frame)
    unsigned char  data[4]; //!< Od register data content
    unsigned char  error[4]; //!< Error code in case of error
    odActionStatus actionStatus; //!< Current action status
};


/**
 * \addtogroup CANOPEN_PROTOCOL_INTERFACE CANopen Protocol Implementation
 * \ingroup CAN_INTERFACE_MODULE
 *
 * @brief This is the CANOpen protocol implementation class
 *  \nosubgrouping
 *
 * This class implements the CANopen protocol described  in \ref PG_TD_CAN_OPEN.
 *
 * A class that should interface a Device communicating in the CAN Bus
 * compliance with the standard described in \ref PG_TD_CAN_OPEN shall subclass this class.
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
class canOpenInterface : public canDriverUserClass
{
    Q_OBJECT
public:

    canOpenInterface(int node, const canDriverInterface& driver ): canDriverUserClass(driver)
    {
        nodeId = node;
    }

    ~canOpenInterface() {

    }


    //! This is the enum type of the Device operating mode register
    typedef enum{
      OP_IDLE = 1,  //!< Device is in IDLE operating Mode
      OP_RUNNING,   //!< Device is in RUNNING operating Mode
      OP_LOADER     //!< Device Loader is running
    }_OperMode;

    //! This is the enum type of the Device Command Errors
    typedef enum{
      CMD_READY = 1,                //!< Command completed. Ready for a new command execution.
      CMD_EXECUTING,                //!< Command accepted and executing.
      CMD_ERROR_BUSY,               //!< The Command is running or another command is running.
      CMD_ERROR_DEVICE,             //!< The command failed. See the Error Code field of the Command register.
      CMD_ERROR_INVALID_DATA,       //!< The data/parameters are invalid or no more accessible.
      CMD_ERROR_INVALID_ACCESS,     //!< The requested command cannot be executed in this Device current status.
      CMD_ERROR_NOT_IMPLEMENTED     //!< The requested command is not yet implemented.
    }_CommandCodes;


    //! Sets the list of device registers assigned to the protocol
    void _inline setRegisterList(const QList<odRegister>& reglist ){
        deviceRegisters = reglist;
    }

signals:

    //! This signal is emitted when a frame has been received and classified
    void registerActionCompleteSgn(int index);


protected slots:

    /**
    * @brief canOpenInterface::deviceRxSlot
    *
    * This function handles  the can received frames.
    *
    * Based on the protocol, there could be yhree possibles reception frames:
    * + reception of a read request frame;
    * + reception of a write request frame;
    * + reception of a execution command request.
    *
    * In the case of read request, the result can be the content of the register or an error condition;\n
    * In the case of a write request, the result can be a write confirm,ation or error condition;
    * In case of a execution command request:
    * + if this is the write frame answer it means that it is the command activation (or completion) confirmation;
    * + if this is a read frame answer, it is the command completion confirmation;
    * + if it is a error frame, it is a coomand still executing or terminate with error condition.
    *
    *
    * @param frame
    * This is the received frame;
    * @param odIndex
    * This is the OD index of the register target of the frame.
    */
    void deviceRxSlot(canDriverInterface::canDataFrame frame, int odIndex);

protected:
    QList<odRegister> deviceRegisters; //!< List of odRegisters

public:
    bool readOdRegister(int odIndex,uint tmo); //!< Execute of the Read register action
    bool writeOdRegister(int odIndex,uint tmo);//!< Execute of the Write register action
    bool executeCommand(int odIndex,uint tmo); //!< Execute a Command

private:
    int nodeId;

};

#endif // CANOPENINTERFACE_H
