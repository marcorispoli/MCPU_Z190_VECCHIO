#include "power_service.h"
#include "startupWindow.h"


extern startupWindow*      window;

powerService::powerService(uint id, uchar revMaj, uchar revMin, uchar revSub, QObject *parent ):deviceClass(id, revMaj, revMin, revSub, parent)
{
    executing = false;

    // Set the list of Custom Simulated Status Registers
    statusRegisterList.append(dataregT());

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

void powerService::start(void){
    if(executing) return;
    executing = true;
    deviceLoop = startTimer(50);

}
void powerService::updateSystemStatusRegister(void){
    dataregT IO;

    IO.data[0] =0;
                                                            // Bit0: error status
    if(inputs[I_PWS_EMERGENCY])     IO.data[0]   |= 0x02;   // Emergency
    if(inputs[I_PWS_POWER_DOWN])    IO.data[0]   |= 0x04;   // Power Down Status
    if(inputs[I_PWS_MOTOR_SAFETY])  IO.data[0]   |= 0x08;   // Motor safety Switch
    if(softPowerOffReq)             IO.data[0]   |= 0x10;   // Soft Power Off request
    if(batt1Low)                    IO.data[0]   |= 0x20;   // Batt1 Low
    if(batt2Low)                    IO.data[0]   |= 0x40;   // Batt2 Low
    if(inputs[I_PWS_BATT_ENA])      IO.data[0]   |= 0x80;   // Battery Enable Button

    IO.data[1] =0;
    if((inputs[I_PWS_CARM_UP]) ||  (inputs[I_PWS_PED_CARM_UP]))      IO.data[1]   |= 0x01;   // C-ARM UP Request
    if((inputs[I_PWS_CARM_DWN]) ||  (inputs[I_PWS_PED_CARM_DWN]))      IO.data[1]   |= 0x02;   // C-ARM DOWN Request
    if(inputs[I_PWS_TILT_UP])       IO.data[1]   |= 0x04;   // TILT UP Button
    if(inputs[I_PWS_TILT_DWN])      IO.data[1]   |= 0x08;   // TILT DOWN Button
    if(inputs[I_PWS_ROT_CW])        IO.data[1]   |= 0x10;   // ROT CW Button
    if(inputs[I_PWS_ROT_CCW])       IO.data[1]   |= 0x20;   // ROT CCW Button
    if(inputs[I_PWS_BODY_CW])       IO.data[1]   |= 0x40;   // BODY ROT CW Button
    if(inputs[I_PWS_BODY_CCW])      IO.data[1]   |= 0x80;   // BODY ROT CCW Button

    IO.data[2] =0;
    if(motorDcOk)                   IO.data[2]   |= 0x01;   // Motor DC-OK status
    if(safetyMotorStatus)           IO.data[2]   |= 0x02;   // Safety Motor Power Activation status
    if(inputs[I_PWS_CMP_ON])        IO.data[2]   |= 0x04;   // Compression ON signal
    if(inputs[I_PWS_XRAY_REQ])      IO.data[2]   |= 0x08;   // X-RAY Push Button status
    if(inputs[I_PWS_CLOSED_DOOR])   IO.data[2]   |= 0x10;   // Closed Door Status
    if(burninJumperCheck)           IO.data[2]   |= 0x20;   // Burning Jumper check

    IO.data[3] =0;
    if(inputs[I_PWS_PED_CARM_UP])   IO.data[3]   |= 0x01;   // Pedal C-ARM UP
    if(inputs[I_PWS_PED_CARM_DWN])  IO.data[3]   |= 0x02;   // Pedal C-ARM DOW
    if(inputs[I_PWS_PED_CMP_UP])    IO.data[3]   |= 0x04;   // Pedal Compression UP
    if(inputs[I_PWS_PED_CMP_DWN])   IO.data[3]   |= 0x08;   // Pedal Compression DWN

    systemreg = IO;

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

    if()
}

void powerService::timerEvent(QTimerEvent* ev){

    if(ev->timerId() == deviceLoop)
    {
        updatePWSOutputs();
        updatePWSInputs();
        updateSystemStatusRegister();
    }
}

bool powerService::readStatus(uchar index, dataregT* data){    
    return deviceClass::readStatus(index, data);
}

bool powerService::execCommand(uchar cmd, dataregT* data){
    return false;
}
