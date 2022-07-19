#ifndef CAN_PROTOCOL_H
#define CAN_PROTOCOL_H

#include <QObject>

class canRegister{

public:

    explicit canRegister(uchar address){
        data[0] =0;
        data[1] =0;
        data[2] =0;
        data[3] =0;
        this->address = address;
        updated = false;
    }
    ~canRegister(){};

    bool set(uchar b0, uchar b1, uchar b2, uchar b3){
        if((data[0] == b0) && (data[1] == b1) && (data[2] == b2) && (data[3] == b3)) return updated;

        data[0] = b0;
        data[1] = b1;
        data[2] = b2;
        data[3] = b3;
        updated = true;
        return true;
    }

    bool get(uchar* b0, uchar* b1, uchar* b2, uchar* b3, bool reset = true){
        *b0 = data[0];
        *b1 = data[1];
        *b2 = data[2];
        *b3 = data[3];
        bool stat = updated;
        if(reset) updated = false;
        return stat;
    }

    uchar data[4];
    uchar address;
    bool  updated;

};

class canProtocol: public QObject
{
     Q_OBJECT

public:

    explicit canProtocol(ushort canId);
    ~canProtocol(){};

    // protocol Implementation
    typedef enum{
        _PROTO_NOT_DEFINED  = 0,
        _PROTO_READ_STATUS  = 1,
        _PROTO_WRITE_PARAM  = 2,
        _PROTO_WRITE_DATA   = 3,
        _PROTO_STORE_PARAMS = 4,
        _PROTO_COMMAND_EXEC = 5,
    }_CanProtocolFrameCode;

    typedef enum{
       _S_RESERVED = 0,
       _S_REVISION,
       _S_SYSTEM,
       _S_ERROR,
       _S_COMMAND,
       _S_LAST
    }_StatusRegisters;

    typedef enum{
       _P_RESERVED = 0,
       _P_LAST
    }_ParamRegisters;

    typedef enum{
       _D_RESERVED = 0,
       _D_LAST
    }_DataRegisters;



signals:


public slots:

public:
    // Device Target CanId Address
    ushort canId;

    QList<canRegister> statusRegisters;
    QList<canRegister> paramRegisters;
    QList<canRegister> dataRegisters;

    static QByteArray formatReadStatus(uchar seq, uchar address){
        QByteArray frame;
        frame.append(seq);
        frame.append((uchar) _PROTO_READ_STATUS);
        frame.append((uchar) address);
        frame.append((uchar) 0);
        frame.append((uchar) 0);
        frame.append((uchar) 0);
        frame.append((uchar) 0);
        frame.append((uchar)seq ^ (uchar) _PROTO_READ_STATUS ^ (uchar) address);
        return frame;
    }

    _CanProtocolFrameCode decodeFrame(QByteArray data);
    QByteArray evaluateReadStatusFrame(QByteArray data);
    QByteArray evaluateWriteParamFrame(QByteArray data);
    QByteArray evaluateWriteDataFrame(QByteArray data);
    QByteArray evaluateStoreFrame(QByteArray data);
    QByteArray evaluateCommandFrame(QByteArray data);

};




#endif // CAN_PROTOCOL_H
