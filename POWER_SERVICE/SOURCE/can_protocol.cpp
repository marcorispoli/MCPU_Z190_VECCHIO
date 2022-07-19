#include "can_protocol.h"
#include <QApplication>


canProtocol::canProtocol(ushort canId)
{
    this->canId = canId;

    // Status register instance
    statusRegisters.append(canRegister(_S_RESERVED));
    statusRegisters.append(canRegister(_S_REVISION));
    statusRegisters.append(canRegister(_S_SYSTEM));
    statusRegisters.append(canRegister(_S_ERROR));
    statusRegisters.append(canRegister(_S_COMMAND));
    statusRegisters.append(canRegister(_S_BATTERY));

    // Parameter registers instance
    paramRegisters.append(canRegister(_P_RESERVED));
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
    dataRegisters.append(canRegister(_D_RESERVED));
    dataRegisters.append(canRegister(_D_OUTPUT));

};

canProtocol::_CanProtocolFrameCode canProtocol::decodeFrame(uchar seq, uchar address, QByteArray data){
    uchar rxseq = (uchar) data.at(0);
    uchar rxaddr = (uchar) data.at(2);
    if(seq != rxseq){
        return _PROTO_NOT_DEFINED;     // Wrong Sequence
    }
    if(address != rxaddr) {
        return _PROTO_NOT_DEFINED;     // Wrong Sequence
    }

    // CRC frame check
    uchar crc = 0;
    for(int i=0; i<8; i++) crc ^= (uchar) data.at(i);
    if(crc) {
        return _PROTO_NOT_DEFINED;     // Wrong Sequence
    }

    return (_CanProtocolFrameCode) data.at(1);
}

bool canProtocol::evaluateReadStatusFrame(QByteArray data){
    if(data.at(2) >= _S_LAST) return false;

    statusRegisters[data.at(2)].set(data[3], data[4], data[5], data[6]);
    return true;
}
bool canProtocol::evaluateWriteParamFrame( QByteArray data){


    return true;
}
bool canProtocol::evaluateWriteDataFrame(QByteArray data){


    return true;
}
bool canProtocol::evaluateStoreFrame(QByteArray data){


    return true;
}

bool canProtocol::evaluateCommandFrame(QByteArray data){


    return true;
}
