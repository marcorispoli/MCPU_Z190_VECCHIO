#ifndef POWER_SERVICE_H
#define POWER_SERVICE_H

#include <QObject>
#include <QTimer>
#include <QTimerEvent>
#include "device.h"
#include "device_command_interface.h"
#include "command_interface.h"

class powerService: public Device
{
     Q_OBJECT

public:

    explicit powerService(ushort CanId, QString IP, uint port, uint commandPort);
    ~powerService(){};


    // Device Custom Register definition
    typedef enum{
       _S_BATTERY = Device::_S_LAST,
       _S_LAST
    }_StatusRegisters;

    typedef enum{
       _P_RESERVED = Device::_P_LAST,
       _P_DISABLE_POWER_ON_TIMER ,
       _P_HARDWARE_POWER_OFF_TIMER,
       _P_KEEP_ALIVE_TIMER,
       _P_SOFT_POWER_OFF_TIMER,
       _P_LOW_BATT_LEVEL,
       _P_BODY_ROTATION_LOCK_TIMER,
       _P_PEDALBOARD_LOCK_TIMER,
       _P_MANUAL_BUTTON_LOCK_TIMER,
       _P_XRAY_BUTTON_LOCK_TIMER,
       _P_LAST
    }_ParamRegisters;

    typedef enum{
       _D_RESERVED = Device::_D_LAST,
       _D_OUTPUT = 1,
       _D_LAST
    }_DataRegisters;

    typedef enum{
       _C_MAIN_CPU_SOFT_POWER_OFF = 1,
       _C_MAIN_CPU_SOFT_POWER_OFF_ABORT,

    }_CommandRegisters;



public slots:
    void executeProcess(void); // Internal slot

private slots:
    void canTxRxCompleted(uchar seq, Device::_CanTxErrors error);   // Override of the Device method
    void canDriverReady(bool );                                     // Override of the Device method
    void targetDeviceReady(bool );                                  // Override of the Device method

private:
    deviceCommandInterface* commandInterface; ///< Creates the command protocol
    void initCommandInterface(void);

    // Status machines
    typedef enum{
        _DEVICE_EXEC_GET_STATUS = 0, ///< Gets all the Status registers from the target device
        _DEVICE_EXEC_IDLE = 1,       ///< Idle activities
    }_DeviceExecutionT;

    // Status machine variables
    uchar deviceProcessSeq; ///< sequence messages
    _DeviceExecutionT deviceExecutionProcess; ///< status process variable

    void deviceProcessDownloadStatusRegisters(void); ///< Download all the Status registers from the target device
    void deviceProcessIdle(void);                    ///< Idle Status


    void readStatus(uchar address);

};




#endif // POWER_SERVICE_H
