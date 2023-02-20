#ifndef CAN_DEVICE_PROTOCOL_H
#define CAN_DEVICE_PROTOCOL_H



/*!
 * \defgroup  canDeviceModule CAN Device Protocol Module Interface.
 * \ingroup libraryModules
 *
 * This library module implements the CAN Device Protocol interface
 * as described in the document:
 *
 * + Gantry Software Detailed Documentation/Device Protocol Specification
 *
 * # DEPENDENCES
 *
 * This module requires the use of the:
 *
 * + canclient.cpp
 * + canclient.h
 *
 */



#include <QtCore>

class canDeviceProtocolFrame
{
public:

    static const uchar CAN_ABORT_COMMAND = 0; //!< Defines the special command code for abort procedure

   /**
    *  This enumeration defines the Frame Command Codes
    */
    typedef enum{
        READ_REVISION = 1,      //!< Request to read the Revision register
        READ_ERRORS,            //!< Request to read the Errors register
        READ_COMMAND,           //!< Request to read the Command register
        READ_STATUS,            //!< Request to read a Status register
        READ_DATA,              //!< Request to read a Data register
        READ_PARAM,             //!< Request to read a Parameter register
        WRITE_DATA,             //!< Request to Write a Data register
        WRITE_PARAM,            //!< Request to Write a Parameter register
        STORE_PARAMS,           //!< Request to Store in non volatile memory the parameters
        COMMAND_EXEC,           //!< Command Execution	Request
    }CAN_FRAME_COMMANDS_t;

    typedef enum{
        CAN_COMMAND_EXECUTING=1,         //!< The Command is processing
        CAN_COMMAND_EXECUTED,            //!< The Command has been processed successfully
        CAN_COMMAND_ERROR,               //!< The Command has been terminated with error condition
        CAN_COMMAND_STATUS_UNASSIGNED,   //!< The Application forgot to assign a correct return code

    }CAN_CommandExecStatus_t;

    /**
     *  This is the Command Execution code error enumeraiton list
     */
    typedef enum{
        CAN_COMMAND_NO_ERROR = 0,       //!< No Error
        CAN_COMMAND_BUSY = 1,           //!< A command is executing
        CAN_COMMAND_INVALID_DATA = 2,   //!< The Command data are invalid
        CAN_COMMAND_NOT_ENABLED = 3,    //!< The Command cannot be momentary executed
        CAN_COMMAND_NOT_AVAILABLE = 4,  //!< The Command cannot be executed in this revision
        CAN_COMMAND_WRONG_RETURN_CODE = 5,  //!< The Command returned a non valid status code
        CAN_COMMAND_ABORT_CODE = 5,     //!< The Command has been aborted
        CAN_COMMAND_APPLICATION_ERRORS  //!< Starting code for applicaiton specific errors
    }CAN_CommandErrors_t;

    typedef struct{
        uchar seq;
        uchar frame_type;
        uchar idx;
        uchar d[4];
    }CAN_FRAME_CONTENT_t;

    typedef struct{
        uchar d[4];
        bool valid;
    }CAN_REGISTER_t;

    typedef struct{
        struct{
            uchar status;
            uchar ris0;
            uchar ris1;
            uchar error;
        }d;
        bool valid;
    }CAN_COMMAND_t;


    static CAN_FRAME_CONTENT_t toContent(QByteArray* data){
        CAN_FRAME_CONTENT_t content;
        content.seq = 0;

        uchar crc = 0;
        if(data->size() != 8) return content;

        for(int i=0; i<7; i++) crc ^= (uchar) data->at(i);
        if(crc != (uchar) data->at(7)) return content;

        content.seq = data->at(0);
        content.frame_type = (CAN_FRAME_COMMANDS_t) data->at(1);
        content.idx = data->at(2);
        for(int i=0; i< 4; i++) content.d[i] = data->at(3+i);
        return content;

    }

    static QByteArray toCanData(CAN_FRAME_CONTENT_t* content){
        QByteArray data;
        data.append(content->seq);
        data.append(content->frame_type);
        data.append(content->idx);
        data.append(content->d[0]);
        data.append(content->d[1]);
        data.append(content->d[2]);
        data.append(content->d[3]);
        uchar crc = 0;
        for(int i=0; i<7; i++) crc ^= data.at(i);
        data.append(crc);
        return data;
    }

};

class canDeviceProtocol: public QObject
{
    Q_OBJECT

public:
    explicit canDeviceProtocol(uchar devid, const char* ip_driver, const ushort port_driver);
    ~canDeviceProtocol();

     static const unsigned short CAN_ID_DEVICE_BASE_ADDRESS = 0x200;


signals:
    void txToCan(ushort canId, QByteArray data); //!< Sends Can data frame to the canDriver

protected:
    bool  accessRegister(canDeviceProtocolFrame::CAN_FRAME_COMMANDS_t regtype, uchar idx=0, uchar d0=0, uchar d1=0, uchar d2=0, uchar d3=0);
    bool  inline abortCommand(void){return accessRegister(canDeviceProtocolFrame::COMMAND_EXEC, canDeviceProtocolFrame::CAN_ABORT_COMMAND);};
    QString getFrameErrorStr(void);

    canDeviceProtocolFrame::CAN_REGISTER_t           revisionRegister;
    canDeviceProtocolFrame::CAN_REGISTER_t           errorsRegister;
    canDeviceProtocolFrame::CAN_COMMAND_t            commandRegister;
    QList<canDeviceProtocolFrame::CAN_REGISTER_t>    statusRegisters;
    QList<canDeviceProtocolFrame::CAN_REGISTER_t>    dataRegisters;
    QList<canDeviceProtocolFrame::CAN_REGISTER_t>    paramRegisters;

    bool inline isCommunicationPending(void) {return busy;}
    bool inline isCommunicationOk(void) {return rxOk;}

private slots:
   void rxFromCan(ushort canId, QByteArray data);//!< Receive Can data frame from the canDriver  
   void timerEvent(QTimerEvent *event);
protected slots:





private:
    ushort deviceID; //!< This is the target device ID

    void receptionEvent(canDeviceProtocolFrame::CAN_FRAME_CONTENT_t* pContent);
    uchar frame_sequence;   //!< Frame sequence iterator
    bool  busy;             //!< Busy flag waiting for the Device answer
    bool  rxOk;             //!< The Frame has been correctly received

    struct{
        uchar tmo:1;
        uchar seq:1;
        uchar crc:1;
        uchar id:1;
        uchar idx:1;
        uchar frame_code:1;
        uchar spare:2;
    }frameError;            //!< In case of error frame, this is the error cause
    int  tmoTimer;



};




#endif // CAN_DEVICE_PROTOCOL_H
