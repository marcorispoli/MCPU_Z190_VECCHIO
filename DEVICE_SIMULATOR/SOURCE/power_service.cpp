#include "power_service.h"
#include "startupWindow.h"


extern startupWindow*      window;

powerService::powerService(uint id, uchar revMaj, uchar revMin, uchar revSub, QObject *parent ):deviceClass(id, revMaj, revMin, revSub, parent)
{
    statusRegisterList.append(dataregT()); // IO register

}

/**
 * @brief updateStatusIO
 *
 * This function build the IO status register
 * based on the Power Service communication protocol
 *
 */
void powerService::updateStatusIO(void){
    dataregT IO;

    if(inputs[I_PWS_PWR_ON_REQ]) IO.data[0]     |= 1;
    if(inputs[I_PWS_POWER_DOWN]) IO.data[0]     |= 2;
    if(inputs[I_PWS_PED_CARM_UP]) IO.data[0]    |= 4;
    if(inputs[I_PWS_PED_CARM_DWN]) IO.data[0]   |= 8;

    if(inputs[I_PWS_PED_CMP_UP]) IO.data[0]     |= 0x10;
    if(inputs[I_PWS_PED_CMP_DWN]) IO.data[0]    |= 0x20;
    if(inputs[I_PWS_BODY_CW]) IO.data[0]        |= 0x40;
    if(inputs[I_PWS_BODY_CCW]) IO.data[0]       |= 0x80;

    if(inputs[I_PWS_TILT_UP]) IO.data[1]        |= 1;
    if(inputs[I_PWS_TILT_DWN]) IO.data[1]       |= 2;
    if(inputs[I_PWS_CARM_UP]) IO.data[1]        |= 4;
    if(inputs[I_PWS_CARM_DWN]) IO.data[1]       |= 8;

    if(inputs[I_PWS_ROT_CW]) IO.data[1]         |= 0x10;
    if(inputs[I_PWS_ROT_CCW]) IO.data[1]        |= 0x20;
    if(inputs[I_PWS_BATT_ENA]) IO.data[1]       |= 0x40;
    if(inputs[I_PWS_HW_HVON]) IO.data[1]        |= 0x80;

    if(inputs[I_PWS_HW_PREP]) IO.data[2]        |= 1;
    if(inputs[I_PWS_CMP_ON]) IO.data[2]         |= 2;
    if(inputs[I_PWS_EMERGENCY]) IO.data[2]      |= 4;
    if(inputs[I_PWS_MOTOR_SAFETY]) IO.data[2]   |= 8;

    if(inputs[I_PWS_XRAY_REQ]) IO.data[2]       |= 0x10;
    if(inputs[I_PWS_CLOSED_DOOR]) IO.data[2]    |= 0x20;

    IO.data[3] = (analog[ANL_PSW_BATT1] & 0x0F) + (analog[ANL_PSW_BATT1] << 4);

    statusRegisterList[STATUS_PWS_IO] = IO;

}

bool powerService::readStatus(uchar index, dataregT* data){
    updateStatusIO();
    return powerService::readStatus(index, data);
}

bool powerService::execCommand(uchar cmd, dataregT* data){
    return false;
}
