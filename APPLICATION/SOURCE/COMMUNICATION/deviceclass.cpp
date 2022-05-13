#include "deviceclass.h"

deviceClass::deviceClass(){
    paramList.clear();
    statusList.clear();
    dataList.clear();

    statusList.append(deviceDataRegisterClass(false)); // RESERVED
    statusList.append(deviceDataRegisterClass(false)); // SYSTEM
    statusList.append(deviceDataRegisterClass(false)); // ERRORS

    paramList.append(deviceDataRegisterClass(false)); // RESERVED
    dataList.append(deviceDataRegisterClass(false)); // RESERVED

}


void deviceClass::uploadParam(uchar seq, uchar index, deviceDataRegisterClass parameter){
    if(index >= paramList.size()) return;

    paramList[index].status = false; // Invalid the current value until it receives the answer from the slave
    emit sendToDevice(seq, ID, (uchar) _DEVCMD_WRITEPAR, parameter.data[0],parameter.data[1], parameter.data[2], parameter.data[3]);
}

void deviceClass::uploadData(uchar seq, uchar index, deviceDataRegisterClass data){
    if(index >= dataList.size()) return;

    dataList[index].status = false; // Invalid the current value until it receives the answer from the slave
    emit sendToDevice(seq, ID, (uchar) _DEVCMD_WRITEDATA, data.data[0],data.data[1], data.data[2], data.data[3]);

}

void deviceClass::downloadStatus(uchar seq, uchar index){
    if(index >= statusList.size()) return;

    statusList[index].status = false; // Invalid the current value until it receives the answer from the slave
    emit sendToDevice(seq, ID, (uchar) _DEVCMD_READSTAT, 0,0, 0, 0);

}


bool deviceClass::decodeFrame(canDriverInterface::canDataFrame* rxFrame, _RegisterSetOptions options = _REG_DEV_NOP){

    if(rxFrame == nullptr) return false;

    QList<deviceDataRegisterClass>* pList = nullptr;
    uchar* pdata;
    uchar index;
    bool  regchg = false;

    switch((_DEVICE_COMMAND_FIELD) rxFrame->data[_DEVICE_CMD_DATA_FRAME]){
    case  _DEVCMD_WRITEPAR:
        if(pList == nullptr) pList = &paramList;
    case  _DEVCMD_WRITEDATA:
        if(pList == nullptr) pList = &dataList;
    case  _DEVCMD_READSTAT:
        if(pList == nullptr) pList = &statusList;

        pdata = &rxFrame->data[_DEVICE_DATA0_DATA_FRAME];
        index = rxFrame->data[_DEVICE_REGINDEX_DATA_FRAME];
        if(index == 0) return false;
        if(index >= pList->size()) return false;

        // Update registers
        regchg = (*pList)[index].setRegister(pdata);
        if(pList != &statusList) return true;

        // Evaluete the statusChangeSgn signal emission
        if(options & _REG_DEV_NO_ACTION) return true;
        if(options & _REG_DEV_FORCE) emit statusChangedSgn(index);
        else if(regchg) emit statusChangedSgn(index);

        return true;
        break;

    case _DEVCMD_EXEC:
        break;
    case _DEVCMD_STOREPAR:
        break;
    }


    return false;
}
