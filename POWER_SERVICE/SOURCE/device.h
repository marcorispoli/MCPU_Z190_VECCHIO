#ifndef DEVICE_H
#define DEVICE_H

#include <QObject>
#include <QTimer>
#include <QTimerEvent>
#include "can_protocol.h"

class Device: public QObject
{
     Q_OBJECT

public:

    explicit Device();
    ~Device(){};

signals:
    void txToCanDriver(QByteArray);

public slots:
    void rxFromCanDriver(QByteArray);
    void canConnection(bool);

public:
    void  timerEvent(QTimerEvent* ev);

private:
    canProtocol* can;

    // Device Target CanId Address
    const ushort CANID = (0x200 + 1);
    ushort assignedCanId;

    typedef enum{
        _DEV_INIT = 0,
        _DEV_IDLE
    }_DeviceExecutionProcess;

    typedef enum{
        _CMD_NONE = 0,
        _CMD_CONFIG,
        _CMD_READ_STATUS_REGISTER,
        _CMD_WRITE_PARAM_REGISTER,
        _CMD_WRITE_DATA_REGISTER,
        _CMD_STORE_PARAMS_REGISTER,
        _CMD_COMMAND_EXEC
    }_CommandExecution;

    int deviceProcessTimer;
    ushort deviceProcessSeq;
    _DeviceExecutionProcess  deviceExecutionProcess;
    _CommandExecution        currentCommandExecution;
    bool    commandAck;

    uchar   dataSequence;
    uchar   requestedRegAddress;

    void deviceProcessInitialization(void);
    void deviceProcessIdle(void);


    void sendConfig(void);
    void readStatus(uchar address);
    void writeParam(uchar address, QByteArray val);
    void writeData(uchar address, QByteArray val);
    void sendCommand(uchar address, QByteArray val);

};




#endif // DEVICE_H
