#include "can_driver.h"
#include <QApplication>

VscanCanDriver::VscanCanDriver(){
    canTimer=0;
    for(int i=0; i<8; i++)    rxCanData.append((uchar) 0);
    loopback = false;
}


bool VscanCanDriver::driverOpen(_CanBR BR){
    VSCAN_STATUS status;
    char string[33];


    loopback = false;

    // status = VSCAN_Ioctl(NULL, VSCAN_IOCTL_SET_DEBUG, VSCAN_DEBUG_LOW);

    // Open the device
    handle = VSCAN_Open("COM10", VSCAN_MODE_NORMAL);
    if(handle <= 0){        
        VSCAN_GetErrorString((VSCAN_STATUS) handle, string, 32);
        qDebug() << "Open Command: " << string;
        return false;
    }

    qDebug() << "Can Driver Opened";

    // Get the APi release code
    status = VSCAN_Ioctl(handle, VSCAN_IOCTL_GET_API_VERSION, &version);
    if(status != VSCAN_ERR_OK){
        VSCAN_GetErrorString(status, string, 32);
        qDebug() << "Get Api Version Command: " << string;
        return false;
    }

    qDebug() << "VSCAN API: " << version.Major << "." << version.Minor << "." << version.SubMinor;

    // Get Hardware release code
    status = VSCAN_Ioctl(handle, VSCAN_IOCTL_GET_HWPARAM, &hwparam);
    if(status != VSCAN_ERR_OK){
        VSCAN_GetErrorString(status, string, 32);
        qDebug() << "Get Hw Param Command: " << string;
        return false;
    }

    uchar hwmaj = hwparam.HwVersion >> 4;
    uchar hwmin = hwparam.HwVersion & 0x0F;
    uchar swmaj = hwparam.SwVersion >> 4;
    uchar swmin = hwparam.SwVersion & 0x0F;

    qDebug() << "VSCAN HARDWARE:  SN=" << hwparam.SerialNr << " HwREV=" << hwmaj <<"."<<hwmin << " SwREV=" << swmaj<<"."<<swmin << " TYPE=" << hwparam.HwType;

    // Set Baudrate
    void* br = VSCAN_SPEED_1M;
    QString brstring = " 1Mbs";

    switch(BR){
    case _CAN_1000K: br = VSCAN_SPEED_1M; brstring = " 1Mbs"; break;
    case _CAN_800K: br = VSCAN_SPEED_800K; brstring = " 800Kbs"; break;
    case _CAN_500K: br = VSCAN_SPEED_500K; brstring = " 500Kbs"; break;
    case _CAN_250K: br = VSCAN_SPEED_250K; brstring = " 250Kbs"; break;
    case _CAN_125K: br = VSCAN_SPEED_125K; brstring = " 125Kbs"; break;
    case _CAN_100K: br = VSCAN_SPEED_100K; brstring = " 100Kbs"; break;
    case _CAN_50K: br = VSCAN_SPEED_50K; brstring = " 50Kbs"; break;
    case _CAN_20K: br = VSCAN_SPEED_20K; brstring = " 20Kbs"; break;
    }

    // Set Baudrate
    status = VSCAN_Ioctl(handle, VSCAN_IOCTL_SET_SPEED, br);
    if(status != VSCAN_ERR_OK){
        VSCAN_GetErrorString(status, string, 32);
        qDebug() << "Set Baudrate Command: " << string;
        return false;
    }

    qDebug() << "SET BAUDRATE TO " << brstring;


    // Set the Filter to accept all frames
    status = VSCAN_Ioctl(handle, VSCAN_IOCTL_SET_FILTER_MODE, VSCAN_FILTER_MODE_DUAL);
    if(status != VSCAN_ERR_OK){
        VSCAN_GetErrorString(status, string, 32);
        qDebug() << "VSCAN_IOCTL_SET_FILTER_MODE Command: " << string;
        return false;
    }
    // receive all frames on the CAN bus (default)

    VSCAN_CODE_MASK codeMask;
    codeMask.Code = VSCAN_IOCTL_ACC_CODE_ALL;
    codeMask.Mask = VSCAN_IOCTL_ACC_MASK_ALL;
    status = VSCAN_Ioctl(handle, VSCAN_IOCTL_SET_ACC_CODE_MASK, &codeMask);

    if(status != VSCAN_ERR_OK){
        VSCAN_GetErrorString(status, string, 32);
        qDebug() << "Set Code And Mask Command: " << string;
        return false;
    }

    // clear all filters
    VSCAN_FILTER filter;
    filter.Size = 0;
    status = VSCAN_Ioctl(handle, VSCAN_IOCTL_SET_FILTER, &filter);
    if(status != VSCAN_ERR_OK){
        VSCAN_GetErrorString(status, string, 32);
        qDebug() << "VSCAN_IOCTL_SET_FILTER Command: " << string;
        return false;
    }

    qDebug() << "VSCAN DRIVER READY";

    if(canTimer) killTimer(canTimer);
    canTimer = startTimer(1);
    return true;

}

void VscanCanDriver::driverClose(void){

    if(canTimer) killTimer(canTimer);
    canTimer = 0;

    VSCAN_STATUS status = VSCAN_Close(handle);
    if(status != VSCAN_ERR_OK){
        char string[33];
        VSCAN_GetErrorString(status, string, 32);
        qDebug() << string;
        return ;
    }

    return;
}







void VscanCanDriver::sendOnCanSlot(ushort canId, QByteArray data){

    VSCAN_MSG msg;
    DWORD written;
    uchar len = data.size();
    if(len > 8) len =8;

    msg.Flags = VSCAN_FLAGS_STANDARD;
    msg.Id = canId;
    msg.Size = len;
    for(uchar i=0; i<len; i++) msg.Data[i] = data[i];
    if(VSCAN_Write(handle, &msg, 1, &written) != VSCAN_ERR_OK) return;
    VSCAN_Flush(handle);
    return;
}



void VscanCanDriver::timerEvent(QTimerEvent* ev)
{
    if(ev->timerId() ==  canTimer)
    {

        rxmsg = 0;
        VSCAN_Read(handle, rxmsgs, VSCAN_NUM_MESSAGES, &rxmsg);
        if(rxmsg){
            for(uint i=0; i < (uint) rxmsg; i++){
                rxCanId = rxmsgs[i].Id;
                for(int j=0; j < 8; j++) rxCanData[j] = rxmsgs[i].Data[j];
                if(loopback) sendOnCanSlot(rxCanId,rxCanData);
                else emit receivedCanFrame(rxCanId, rxCanData);
            }
        }

    }

}


bool VscanCanDriver::driverOpenLoopback(_CanBR BR){

    VSCAN_STATUS status;
    char string[33];




    // Open the device
    handle = VSCAN_Open("COM20", VSCAN_MODE_NORMAL);
    if(handle <= 0){
        VSCAN_GetErrorString((VSCAN_STATUS) handle, string, 32);
        qDebug() << "Open Command: " << string;
        return false;
    }

    qDebug() << "Can Driver Loopback Opened";

    // Get the APi release code
    status = VSCAN_Ioctl(handle, VSCAN_IOCTL_GET_API_VERSION, &version);
    if(status != VSCAN_ERR_OK){
        VSCAN_GetErrorString(status, string, 32);
        qDebug() << "Get Api Version Command: " << string;
        return false;
    }

    qDebug() << "VSCAN API: " << version.Major << "." << version.Minor << "." << version.SubMinor;

    // Get Hardware release code
    status = VSCAN_Ioctl(handle, VSCAN_IOCTL_GET_HWPARAM, &hwparam);
    if(status != VSCAN_ERR_OK){
        VSCAN_GetErrorString(status, string, 32);
        qDebug() << "Get Hw Param Command: " << string;
        return false;
    }

    uchar hwmaj = hwparam.HwVersion >> 4;
    uchar hwmin = hwparam.HwVersion & 0x0F;
    uchar swmaj = hwparam.SwVersion >> 4;
    uchar swmin = hwparam.SwVersion & 0x0F;

    qDebug() << "VSCAN HARDWARE:  SN=" << hwparam.SerialNr << " HwREV=" << hwmaj <<"."<<hwmin << " SwREV=" << swmaj<<"."<<swmin << " TYPE=" << hwparam.HwType;

    // Set Baudrate
    void* br = VSCAN_SPEED_1M;
    QString brstring = " 1Mbs";

    switch(BR){
    case _CAN_1000K: br = VSCAN_SPEED_1M; brstring = " 1Mbs"; break;
    case _CAN_800K: br = VSCAN_SPEED_800K; brstring = " 800Kbs"; break;
    case _CAN_500K: br = VSCAN_SPEED_500K; brstring = " 500Kbs"; break;
    case _CAN_250K: br = VSCAN_SPEED_250K; brstring = " 250Kbs"; break;
    case _CAN_125K: br = VSCAN_SPEED_125K; brstring = " 125Kbs"; break;
    case _CAN_100K: br = VSCAN_SPEED_100K; brstring = " 100Kbs"; break;
    case _CAN_50K: br = VSCAN_SPEED_50K; brstring = " 50Kbs"; break;
    case _CAN_20K: br = VSCAN_SPEED_20K; brstring = " 20Kbs"; break;
    }

    // Set Baudrate
    status = VSCAN_Ioctl(handle, VSCAN_IOCTL_SET_SPEED, br);
    if(status != VSCAN_ERR_OK){
        VSCAN_GetErrorString(status, string, 32);
        qDebug() << "Set Baudrate Command: " << string;
        return false;
    }

    qDebug() << "SET BAUDRATE TO " << brstring;


    // Set the Filter to accept all frames
    status = VSCAN_Ioctl(handle, VSCAN_IOCTL_SET_FILTER_MODE, VSCAN_FILTER_MODE_DUAL);
    if(status != VSCAN_ERR_OK){
        VSCAN_GetErrorString(status, string, 32);
        qDebug() << "VSCAN_IOCTL_SET_FILTER_MODE Command: " << string;
        return false;
    }
    // receive all frames on the CAN bus (default)

    VSCAN_CODE_MASK codeMask;
    codeMask.Code = VSCAN_IOCTL_ACC_CODE_ALL;
    codeMask.Mask = VSCAN_IOCTL_ACC_MASK_ALL;
    status = VSCAN_Ioctl(handle, VSCAN_IOCTL_SET_ACC_CODE_MASK, &codeMask);

    if(status != VSCAN_ERR_OK){
        VSCAN_GetErrorString(status, string, 32);
        qDebug() << "Set Code And Mask Command: " << string;
        return false;
    }

    // clear all filters
    VSCAN_FILTER filter;
    filter.Size = 0;
    status = VSCAN_Ioctl(handle, VSCAN_IOCTL_SET_FILTER, &filter);
    if(status != VSCAN_ERR_OK){
        VSCAN_GetErrorString(status, string, 32);
        qDebug() << "VSCAN_IOCTL_SET_FILTER Command: " << string;
        return false;
    }

    qDebug() << "VSCAN DRIVER READY";

    if(canTimer) killTimer(canTimer);
    canTimer = startTimer(1);
    loopback = true;
    return true;

}
