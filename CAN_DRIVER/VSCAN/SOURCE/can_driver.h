#ifndef VSCAN_CAN_DRIVER_H
#define VSCAN_CAN_DRIVER_H

#include <QTimer>
#include <QTimerEvent>

typedef void VOID;
typedef char CHAR;
typedef unsigned long DWORD;
typedef unsigned char UCHAR;
typedef unsigned short USHORT;
typedef unsigned long ULONG;
#include "vs_can_api.h"

class VscanCanDriver: public QObject
{
   Q_OBJECT
   public:

   VscanCanDriver();
   ~VscanCanDriver(){};


   #define VSCAN_NUM_MESSAGES 10

   typedef enum{
        _CAN_1000K = 0,
        _CAN_800K,
        _CAN_500K,
        _CAN_250K,
        _CAN_125K,
        _CAN_100K,
        _CAN_50K,
        _CAN_20K
    }_CanBR;

    void driverClose(void);
    bool driverOpen(_CanBR BR);
    bool driverOpenLoopback(_CanBR BR);



signals:
    void receivedCanFrame(ushort id, QByteArray data);

public slots:
    void sendOnCanSlot(ushort canId, QByteArray data);

private:
    VSCAN_API_VERSION   version;
    VSCAN_HWPARAM       hwparam;
    VSCAN_HANDLE        handle;
    VSCAN_MSG           rxmsgs[VSCAN_NUM_MESSAGES];
    DWORD               rxmsg;

    ushort rxCanId;
    QByteArray rxCanData;
    void timerEvent(QTimerEvent* ev);
    int canTimer;
    bool loopback;
};

#endif // VSCAN_CAN_DRIVER_H
