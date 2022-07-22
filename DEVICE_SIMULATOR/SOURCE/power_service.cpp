#include "power_service.h"
#include "startupWindow.h"


extern startupWindow*      window;

powerService::powerService( uchar revMaj, uchar revMin, uchar revSub,ushort CanId, QString IP, uint port):deviceSimulator(CanId, IP, port)
{
    statusRegisters[deviceSimulator::_S_REVISION].set(revMaj,revMin,revSub,0);    // Assing the revision

    // Create the Register of the Power Service Device

    // Status register instance
    statusRegisters.append(canRegister(_S_BATTERY));

    // Parameter registers instance
    paramRegisters.append(canRegister(_P_DISABLE_POWER_ON_TIMER));
    paramRegisters.append(canRegister(_P_HARDWARE_POWER_OFF_TIMER));
    paramRegisters.append(canRegister(_P_KEEP_ALIVE_TIMER));
    paramRegisters.append(canRegister(_P_SOFT_POWER_OFF_TIMER));
    paramRegisters.append(canRegister(_P_LOW_BATT_LEVEL));
    paramRegisters.append(canRegister(_P_BODY_ROTATION_LOCK_TIMER));
    paramRegisters.append(canRegister(_P_PEDALBOARD_LOCK_TIMER));
    paramRegisters.append(canRegister(_P_MANUAL_BUTTON_LOCK_TIMER));
    paramRegisters.append(canRegister(_P_XRAY_BUTTON_LOCK_TIMER));

    // Data registers instance
    dataRegisters.append(canRegister(_D_OUTPUT));
    executing = false;


    // Init Device Inputs to false
    for(int i=0; i<I_PWS_NUM; i++) inputs[i] = false;

    // Init Device Outputs to false
    for(int i=0; i<O_PWS_NUM; i++) outputs[i] = false;


    // Init the simulator flags
    softPowerOffReq = false;
    batt1Low = false;
    batt2Low = false;
    safetyMotorStatus = false;
    burninJumperCheck = false;
    motorDcOk = false;

    updatePWSInputs();
    updateSystemStatusRegister();

}

void powerService::deviceStart(void){
    if(executing) return;
    executing = true;
    Start();
    deviceLoop = startTimer(50);

}
void powerService::updateSystemStatusRegister(void){
    uchar data[4];

    data[0] =0;
                                                            // Bit0: error status
    if(inputs[I_PWS_EMERGENCY])     data[0]   |= 0x02;   // Emergency
    if(inputs[I_PWS_POWER_DOWN])    data[0]   |= 0x04;   // Power Down Status
    if(inputs[I_PWS_MOTOR_SAFETY])  data[0]   |= 0x08;   // Motor safety Switch
    if(softPowerOffReq)             data[0]   |= 0x10;   // Soft Power Off request
    if(batt1Low)                    data[0]   |= 0x20;   // Batt1 Low
    if(batt2Low)                    data[0]   |= 0x40;   // Batt2 Low
    if(inputs[I_PWS_BATT_ENA])      data[0]   |= 0x80;   // Battery Enable Button

    data[1] =0;
    if((inputs[I_PWS_CARM_UP]) ||  (inputs[I_PWS_PED_CARM_UP]))      data[1]   |= 0x01;   // C-ARM UP Request
    if((inputs[I_PWS_CARM_DWN]) ||  (inputs[I_PWS_PED_CARM_DWN]))    data[1]   |= 0x02;   // C-ARM DOWN Request
    if(inputs[I_PWS_TILT_UP])       data[1]   |= 0x04;   // TILT UP Button
    if(inputs[I_PWS_TILT_DWN])      data[1]   |= 0x08;   // TILT DOWN Button
    if(inputs[I_PWS_ROT_CW])        data[1]   |= 0x10;   // ROT CW Button
    if(inputs[I_PWS_ROT_CCW])       data[1]   |= 0x20;   // ROT CCW Button
    if(inputs[I_PWS_BODY_CW])       data[1]   |= 0x40;   // BODY ROT CW Button
    if(inputs[I_PWS_BODY_CCW])      data[1]   |= 0x80;   // BODY ROT CCW Button

    data[2] =0;
    if(motorDcOk)                   data[2]   |= 0x01;   // Motor DC-OK status
    if(safetyMotorStatus)           data[2]   |= 0x02;   // Safety Motor Power Activation status
    if(inputs[I_PWS_CMP_ON])        data[2]   |= 0x04;   // Compression ON signal
    if(inputs[I_PWS_XRAY_REQ])      data[2]   |= 0x08;   // X-RAY Push Button status
    if(inputs[I_PWS_CLOSED_DOOR])   data[2]   |= 0x10;   // Closed Door Status
    if(burninJumperCheck)           data[2]   |= 0x20;   // Burning Jumper check

    data[3] =0;
    if(inputs[I_PWS_PED_CARM_UP])   data[3]   |= 0x01;   // Pedal C-ARM UP
    if(inputs[I_PWS_PED_CARM_DWN])  data[3]   |= 0x02;   // Pedal C-ARM DOW
    if(inputs[I_PWS_PED_CMP_UP])    data[3]   |= 0x04;   // Pedal Compression UP
    if(inputs[I_PWS_PED_CMP_DWN])   data[3]   |= 0x08;   // Pedal Compression DWN

    statusRegisters[deviceSimulator::_S_SYSTEM].set(data[0],data[1],data[2],data[3]);

}

void powerService::updatePWSInputs(void){
    inputs[I_PWS_EMERGENCY] =  window->getEmergencyStat();
    inputs[I_PWS_POWER_DOWN] = window->getPowerdownStat();
    inputs[I_PWS_MOTOR_SAFETY] = window->getMotorSafetySwitchStat();
    inputs[I_PWS_BATT_ENA] = window->getBatteryEnableStat();
    inputs[I_PWS_CARM_UP] = window->getCArmUpStat();
    inputs[I_PWS_CARM_DWN] = window->getCArmDwnStat();
    inputs[I_PWS_TILT_UP] = window->getTiltUpStat();
    inputs[I_PWS_TILT_DWN] = window->getTiltDwnStat();
    inputs[I_PWS_ROT_CW] = window->getRotCwStat();
    inputs[I_PWS_ROT_CCW] = window->getRotCCwStat();
    inputs[I_PWS_BODY_CW] = window->getBodyCwStat();
    inputs[I_PWS_BODY_CCW] = window->getBodyCCwStat();

    inputs[I_PWS_CMP_ON] = window->getCompressorOnStat();


    inputs[I_PWS_XRAY_REQ] = window->getXRayReqStat() || (window->getBurninStat() && outputs[O_PSW_BURNING]);

    inputs[I_PWS_CLOSED_DOOR] = window->getCloseDoorStat();

    inputs[I_PWS_PED_CARM_UP] = window->getPedalCarmUpStat();
    inputs[I_PWS_PED_CARM_DWN] = window->getPedalCarmDwnStat();
    inputs[I_PWS_PED_CMP_UP] = window->getPedalCmpUpStat();
    inputs[I_PWS_PED_CMP_DWN] = window->getPedalCmpDwnStat();


}

void powerService::updatePWSOutputs(void){


}

void powerService::timerEvent(QTimerEvent* ev){

    if(ev->timerId() == deviceLoop)
    {
        updatePWSOutputs();
        updatePWSInputs();
        updateSystemStatusRegister();
    }
}

void powerService::canDriverReady(bool stat){
    window->setConnection(stat);
}
