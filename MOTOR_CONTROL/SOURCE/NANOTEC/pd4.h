#ifndef PD4_H
#define PD4_H

/*!
 * \defgroup  canOpenModule CanOpen Protocol implementation.
 *
 * This Module implements the CanOpen protocol.
 *
 *
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
        if((od->b[0] & 0x4F) == 0 ) return  CiA402_NotReadyToSwitchOn;
        else if((od->b[0] & 0x4F) == 0x40 ) return  CiA402_SwitchOnDisabled;
        else if((od->b[0] & 0x6F) == 0x21 ) return  CiA402_ReadyToSwitchOn;
        else if((od->b[0] & 0x6F) == 0x23 ) return  CiA402_SwitchedOn;
        else if((od->b[0] & 0x6F) == 0x27 ) return  CiA402_OperationEnabled;
        else if((od->b[0] & 0x6F) == 0x7 ) return  CiA402_QuickStopActive;
        else if((od->b[0] & 0x4F) == 0xF ) return  CiA402_FaultReactionActive;
        else if((od->b[0] & 0x4F) == 0x8 ) return  CiA402_Fault;
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

    QString getErrorCode1003(ulong val);
    QString getErrorClass1003(ulong val);
    QString getErrorClass1001(ulong val);



signals:
    void txToCan(ushort canId, QByteArray data); //!< Sends Can data frame to the canDriver

public slots:
    ushort initCallback(void); //!< Initialization procedures after the reset
    virtual ushort idleCallback(void){return 0;} //!< Idle status (CiA_SwitchedOn status)
    virtual ushort runCallback(void){return 0;}  //!< Motor activation control callback (CiA_OperationEnabled)
    virtual ushort faultCallback(void){return 0;}//!< Motor Fault callback


    void timerEvent(QTimerEvent* ev);

private slots:

    void statusHandler(void); //!< Main callback handling the cia status
    void rxFromCan(ushort canId, QByteArray data);//!< Receive Can data frame from the canDriver

public:

protected:
    uchar deviceId; //!< Target Can device ID
    clock_t  t1; //!> Time performance measurement variable
    ushort wStatus;
    ushort wSubStatus;

    bool sdo_received;  //! The SDO has been received
    bool sdo_error;     //! The SDO has been received in a wrong format or error code
    bool sdo_timeout;   //!< Flag activated in case of timeout

    void writeSDO(ushort index, uchar sub, canOpenDictionary::_ODDataType type, ulong val);
    void readSDO(ushort index, uchar sub, uchar type);


    canOpenDictionary rxSDO; //!< Received SDO
    canOpenDictionary txSDO; //!< SDO to be transmetted
    canOpenDictionary ODstatus; //!< Received status

    _OD_InitVector* initVector;
    bool deviceInitialized;

private:
    bool ready; //!< The Can Driver is ready to receive data


    int tmo_timer;
    bool check_status;
    _workflowType workflow;

private slots:


    ushort CiA402_SwitchOnDisabledCallback(void);
    ushort CiA402_ReadyToSwitchOnCallback(void);


};



#endif // PD4_H
