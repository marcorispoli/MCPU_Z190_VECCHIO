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


    // Parameter registers instance
    paramRegisters.append(canRegister(_P_RESERVED));

    // Data registers instance
    dataRegisters.append(canRegister(_D_RESERVED));

};

canProtocol::_CanProtocolFrameCode canProtocol::decodeFrame( QByteArray data){

    // CRC frame check
    uchar crc = 0;
    for(int i=0; i<8; i++) crc ^= (uchar) data.at(i);
    if(crc) {
        return _PROTO_NOT_DEFINED;     // Wrong Sequence
    }

    return (_CanProtocolFrameCode) data.at(1);
}

QByteArray canProtocol::evaluateReadStatusFrame(QByteArray data){

    QByteArray answer = data;

    if(data.at(2) >= statusRegisters.size()){
        for(int i=0; i<answer.size(); i++) answer[i] = 0;
        return answer;
    }

    uchar s0,s1,s2,s3;
    statusRegisters[data.at(2)].get(&s0, &s1, &s2, &s3);
    answer[3] = s0;
    answer[4] = s1;
    answer[5] = s2;
    answer[6] = s3;

    uchar crc = 0;
    for(int i=0; i<7; i++) crc ^= (uchar) answer.at(i);
    answer[7] = crc;

    return answer;
}
QByteArray canProtocol::evaluateWriteParamFrame( QByteArray data){

    QByteArray answer = data;

    if(data.at(2) >= paramRegisters.size()){
        for(int i=0; i<answer.size(); i++) answer[i] = 0;
        return answer;
    }

    paramRegisters[data.at(2)].set(answer[3], answer[4], answer[5], answer[6]);

    uchar crc = 0;
    for(int i=0; i<7; i++) crc ^= (uchar) answer.at(i);
    answer[7] = crc;

    return answer;
}

QByteArray canProtocol::evaluateWriteDataFrame(QByteArray data){
    QByteArray answer = data;

    if(data.at(2) >= dataRegisters.size()){
        for(int i=0; i<answer.size(); i++) answer[i] = 0;
        return answer;
    }

    dataRegisters[data.at(2)].set(answer[3], answer[4], answer[5], answer[6]);

    uchar crc = 0;
    for(int i=0; i<7; i++) crc ^= (uchar) answer.at(i);
    answer[7] = crc;

    return answer;
}

QByteArray canProtocol::evaluateStoreFrame(QByteArray data){
    QByteArray answer = data;

    for(int i=0; i<answer.size(); i++) answer[i] = 0;

    uchar crc = 0;
    for(int i=0; i<7; i++) crc ^= (uchar) answer.at(i);
    answer[7] = crc;

    return answer;
}

QByteArray canProtocol::evaluateCommandFrame(QByteArray data){
    QByteArray answer = data;

    for(int i=0; i<answer.size(); i++) answer[i] = 0;

    uchar crc = 0;
    for(int i=0; i<7; i++) crc ^= (uchar) answer.at(i);
    answer[7] = crc;

    return answer;
}
