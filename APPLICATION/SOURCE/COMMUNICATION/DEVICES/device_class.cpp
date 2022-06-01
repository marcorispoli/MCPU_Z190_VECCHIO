#include "device_class.h"

/**
 * @brief deviceClass::deviceClass
 *
 * This is the class constructor.
 *
 * The constructor creates the basic data structures and assigned the target
 * device identifier.
 *
 * @param id: device identifier address (1:255)
 */
deviceClass::deviceClass(uchar id){
    paramList.clear();
    statusList.clear();
    dataList.clear();

    statusList.append(deviceDataRegisterClass()); // RESERVED
    statusList.append(deviceDataRegisterClass()); // SYSTEM
    statusList.append(deviceDataRegisterClass()); // ERRORS
    statusList.append(deviceDataRegisterClass()); // COMMAND

    paramList.append(deviceDataRegisterClass()); // RESERVED
    dataList.append(deviceDataRegisterClass()); // RESERVED

    ID = id;

}


/**
 * @brief deviceClass::addStatusRegister
 *
 * The class that should subclass the deviceClass,
 * shall use this function to add extra status register to the base
 * reserved list.
 *
 *
 * @param stat: this is the status of the initial register value
 * @return the index of the register in the list
 */
uint deviceClass::addStatusRegister(deviceDataRegisterClass::_devreg_status stat){
    statusList.append(deviceDataRegisterClass(stat));
    return statusList.size() - 1;
}

void deviceClass::uploadParam(uchar seq, uchar index, uchar d0, uchar d1, uchar d2, uchar d3){
    if(index >= paramList.size()) return;

    paramList[index].status = deviceDataRegisterClass::_DEVREG_PENDING;; // Invalid the current value until it receives the answer from the slave
    emit sendToDevice(seq, ID, (uchar) _DEVCMD_WRITEPAR, index, d0,d1, d2, d3);
}

void deviceClass::storeParam(uchar seq, uchar index){
    if(index >= paramList.size()) return;
    emit sendToDevice(seq, ID, (uchar) _DEVCMD_STOREPAR,index,0,0,0,0);
}



void deviceClass::uploadData(uchar seq, uchar index, uchar d0, uchar d1, uchar d2, uchar d3){
    if(index >= dataList.size()) return;

    dataList[index].status = deviceDataRegisterClass::_DEVREG_PENDING; // Invalid the current value until it receives the answer from the slave
    emit sendToDevice(seq, ID, (uchar) _DEVCMD_WRITEDATA,  index, d0,d1, d2, d3);

}

void deviceClass::downloadStatus(uchar seq, uchar index){
    if(index >= statusList.size()) return;

    statusList[index].status = deviceDataRegisterClass::_DEVREG_PENDING;; // Invalid the current value until it receives the answer from the slave
    emit sendToDevice(seq, ID, (uchar) _DEVCMD_READSTAT, index, 0,0, 0, 0);

}

void deviceClass::execCommand(uchar seq, uchar cmdcode, uchar d0, uchar d1, uchar d2, uchar d3){
     emit sendToDevice(seq, ID, (uchar) _DEVCMD_EXEC, cmdcode, d0,d1, d2, d3);
}


bool deviceClass::decodeFrame(canDriverInterface::canDataFrame* rxFrame){

    if(rxFrame == nullptr) return false;

    QList<deviceDataRegisterClass>* pList = nullptr;
    uchar* pdata;
    uchar index, d0,d1,d2;
    uchar seq;

    bool  regchg = false;

    pdata = &rxFrame->data[_DEVICE_DATA0_DATA_FRAME];
    index = rxFrame->data[_DEVICE_REGINDEX_DATA_FRAME];

    seq =   rxFrame->data[_DEVICE_SEQ_DATA_FRAME];

    switch((_DEVICE_COMMAND_FIELD) rxFrame->data[_DEVICE_CMD_DATA_FRAME]){
    case _DEVCMD_STOREPAR:

        if(index >= paramList.size()) return false;
        emit sequenceSgn(seq);
        return true;
        break;

    case  _DEVCMD_WRITEPAR:
        if(pList == nullptr) pList = &paramList;
    case  _DEVCMD_WRITEDATA:
        if(pList == nullptr) pList = &dataList;
    case  _DEVCMD_READSTAT:
        if(pList == nullptr) pList = &statusList;

        if(index == 0) return false;
        if(index >= pList->size()) return false;

        // Update registers
        regchg = (*pList)[index].setRegister(pdata);

        // In case it shouldn't be a Status register, sends the sequenceSgn()
        if(pList != &statusList){
            emit sequenceSgn(seq);
            return true;
        }

        // In case of COMMAND status register, verifies the command execution
        if(index == _DEVICE_COMMAND_STATREG){
            if(executingCommandStatus != _EXECCMD_PROCESSING) return true; // No command is executing

            executingCommandStatus = getCommandStatus(); // Update the status
            if(executingCommandStatus != _EXECCMD_PROCESSING) return true; // The command is still in execution

            d0 = rxFrame->data[_DEVICE_CMDERR_DATA_FRAME]; // Error code field
            emit execCmdSgn(executingCommandSeq, executingCommandCode, executingCommandStatus,d0,0,0);
            return true;
        }

        // Evaluete the statusChangeSgn signal emission
        if(regchg) emit statusChangedSgn(index);

        emit sequenceSgn(seq);

        return true;
        break;

    case _DEVCMD_EXEC:
        executingCommandCode = rxFrame->data[_DEVICE_CMDCODE_DATA_FRAME];
        executingCommandSeq = seq;
        executingCommandStatus = rxFrame->data[_DEVICE_CMDSTAT_DATA_FRAME];

        // The Device is processing the command
        if(executingCommandStatus == _EXECCMD_PROCESSING) return true;

        // The Device istantly completes the command
        d0 = rxFrame->data[_DEVICE_CMDD0_DATA_FRAME];
        d1 = rxFrame->data[_DEVICE_CMDD1_DATA_FRAME];
        d2 = rxFrame->data[_DEVICE_CMDD2_DATA_FRAME];
        emit execCmdSgn(seq, executingCommandCode, executingCommandStatus,d0,d1,d2);

        break;

    }


    return false;
}
