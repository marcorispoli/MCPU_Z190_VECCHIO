#ifndef POWER_SERVICE_H
#define POWER_SERVICE_H

#include <QWidget>
#include <QTimerEvent>

#include "device_simulator.h"

// Board IO definition

#define I_PWS_PWR_ON_REQ        0
#define I_PWS_POWER_DOWN        1
#define I_PWS_PED_CARM_UP       2
#define I_PWS_PED_CARM_DWN      3
#define I_PWS_PED_CMP_UP        4
#define I_PWS_PED_CMP_DWN       5
#define I_PWS_BODY_CW           6
#define I_PWS_BODY_CCW          7
#define I_PWS_TILT_UP           8
#define I_PWS_TILT_DWN          9
#define I_PWS_CARM_UP           10
#define I_PWS_CARM_DWN          11
#define I_PWS_ROT_CW            12
#define I_PWS_ROT_CCW           13
#define I_PWS_BATT_ENA          14
#define I_PWS_HW_HVON           15
#define I_PWS_HW_PREP           16
#define I_PWS_CMP_ON            17
#define I_PWS_EMERGENCY         18
#define I_PWS_MOTOR_SAFETY      19
#define I_PWS_XRAY_REQ          20
#define I_PWS_CLOSED_DOOR       21  // *****
#define I_PWS_NUM               22


#define O_PSW_DEVICE_ON         0
#define O_PSW_XRAY_LED          1
#define O_PSW_XRAY_LMP1         2
#define O_PSW_XRAY_LMP2         3
#define O_PSW_XRAY_ENA          4
#define O_PSW_BURNING           5
#define O_PSW_GEMMA_ON          6
#define O_PSW_MOT_SAFE_ENA      7
#define O_PSW_MOT_48V_DISABLE   8
#define O_PSW_COMP_ENA          9
#define O_PSW_DISABLE_CHARGE    10
#define O_PSW_BUZZ_ON           11
#define O_PSW_PROGRAMMING       12
#define O_PWS_NUM               13

#define ANL_PSW_BATT1           0
#define ANL_PSW_BATT2           1
#define ANL_PSW_NUM             2


// Protocol Status register definition


class powerService: public deviceSimulator
{
    Q_OBJECT

public:
    explicit powerService( uchar revMaj, uchar revMin, uchar revSub,ushort CanId, QString IP, uint port);

    typedef enum{
       _S_BATTERY = deviceSimulator::_S_LAST,
       _S_LAST
    }_StatusRegisters;

    typedef enum{
       _P_RESERVED = deviceSimulator::_P_LAST,
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
       _D_RESERVED = deviceSimulator::_D_LAST,
       _D_OUTPUT = 1,
       _D_LAST
    }_DataRegisters;

    typedef enum{
       _C_MAIN_CPU_SOFT_POWER_OFF = 1,
       _C_MAIN_CPU_SOFT_POWER_OFF_ABORT,

    }_CommandRegisters;


public slots:
    void timerEvent(QTimerEvent* ev);

    void canDriverReady(bool );
    void deviceStart(void);

private:
    bool executing;
    int  deviceLoop;


    void updateSystemStatusRegister(void); //< Updates the System Status register
    void updatePWSInputs(void); //< Updates the microcontroller inputs
    void updatePWSOutputs(void); //< Updates the microcontroller outputs


    bool    inputs[I_PWS_NUM];

    uchar   analog[ANL_PSW_NUM];

    bool softPowerOffReq;
    bool batt1Low;
    bool batt2Low;
    bool safetyMotorStatus;
    bool burninJumperCheck;
    bool motorDcOk;

public:
     bool    outputs[O_PWS_NUM];

};

#endif // POWER_SERVICE_H
