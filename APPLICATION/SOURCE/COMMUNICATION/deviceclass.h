#ifndef DEVICECLASS_H
#define DEVICECLASS_H

#include <QObject>
#include "canDriverClassInterface.h"

class deviceDataRegisterClass
{
public:
    explicit deviceDataRegisterClass(bool curstat = true){
        for(int i=0; i<4; i++) data[i] = 0;
        status = curstat;
    }

    bool setRegister(uchar* pdata, bool stat = true){
        bool changed = false;
        for(int i=0; i<4; i++){
            if(data[i] != pdata[i]){
                data[i] = pdata[i];
                changed = true;
            }
        }
        status = stat;
        return changed;
    }

public:
    bool  status;         //< True = ready, FALSE = pending
    uchar data[4];        //< Register Data content
};

class deviceClass: public QObject
{
    Q_OBJECT

    explicit deviceClass(){
        paramList.clear();
        statusList.clear();
        dataList.clear();

        statusList.append(deviceDataRegisterClass(false)); // RESERVED
        statusList.append(deviceDataRegisterClass(false)); // SYSTEM
        statusList.append(deviceDataRegisterClass(false)); // ERRORS

        paramList.append(deviceDataRegisterClass(false)); // RESERVED
        dataList.append(deviceDataRegisterClass(false)); // RESERVED

    }

    /// Definition of the Command Frame Codes
    typedef enum{
        _DEVCMD_READSTAT,       //< Read Status register
        _DEVCMD_WRITEPAR,       //< Write Parameter register
        _DEVCMD_WRITEDATA,      //< Write Data register
        _DEVCMD_STOREPAR,       //< Store Parameter register
        _DEVCMD_EXEC            //< Execute Command
    }_DEVICE_COMMAND_FIELD;


    #define _DEVICE_SEQ_DATA_FRAME 0
    #define _DEVICE_CMD_DATA_FRAME 1
    #define _DEVICE_REGINDEX_DATA_FRAME 2
    #define _DEVICE_CRC_DATA_FRAME 7

    // Data register content
    #define _DEVICE_DATA0_DATA_FRAME 3
    #define _DEVICE_DATA1_DATA_FRAME 4
    #define _DEVICE_DATA2_DATA_FRAME 5
    #define _DEVICE_DATA3_DATA_FRAME 6

    // SPECIAL STATUS REGISTERS ADDRESS
    #define _DEVICE_RESERVED_REGSITER  0
    #define _DEVICE_SYSTEM_STATUS      1
    #define _DEVICE_ERRORS_STATUS      2


    /** Register setting options
     *  The Priority is:
     *  -   _REG_DEV_NO_ACTION
     *  -   _REG_DEV_FORCE
    */
    typedef enum {
        _REG_DEV_NOP = 0,           //< No options
        _REG_DEV_FORCE = 0x1,       //< Force the signaling in the case the register is not changed
        _REG_DEV_NO_ACTION = 0x2,   //< No signaling in case of data is changed
    }_RegisterSetOptions;


signals:
    void statusChangedSgn(uchar index); //< emitted when a Status register changes
    void sendToDevice(uchar seq, uchar id, uchar command, uchar d0, uchar d1, uchar d2, uchar d3);

public:
    void uploadParam(uchar seq, uchar index, deviceDataRegisterClass parameter){
        if(index >= paramList.size()) return;

        paramList[index].status = false; // Invalid the current value until it receives the answer from the slave
        emit sendToDevice(seq, ID, (uchar) _DEVCMD_WRITEPAR, parameter.data[0],parameter.data[1], parameter.data[2], parameter.data[3]);
    }

    void uploadData(uchar seq, uchar index, deviceDataRegisterClass data){
        if(index >= dataList.size()) return;

        dataList[index].status = false; // Invalid the current value until it receives the answer from the slave
        emit sendToDevice(seq, ID, (uchar) _DEVCMD_WRITEDATA, data.data[0],data.data[1], data.data[2], data.data[3]);

    }

    void downloadStatus(uchar seq, uchar index){
        if(index >= statusList.size()) return;

        statusList[index].status = false; // Invalid the current value until it receives the answer from the slave
        emit sendToDevice(seq, ID, (uchar) _DEVCMD_READSTAT, 0,0, 0, 0);

    }


    bool decodeFrame(canDriverInterface::canDataFrame* rxFrame, _RegisterSetOptions options = _REG_DEV_NOP){

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


    /// Return the Address of the current device
    _inline uchar getId(){return ID;}

private:
    QList<deviceDataRegisterClass> paramList; //< List of the device parameters (read/write)
    QList<deviceDataRegisterClass> dataList; //< List of the device data (read/write)
    QList<deviceDataRegisterClass> statusList; //< List of the status registers (readOnly)

    uchar ID; //< ID address of the device
};

#endif // DEVICECLASS_H

