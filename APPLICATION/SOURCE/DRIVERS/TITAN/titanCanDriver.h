#ifndef TITAN_CAN_DRIVER_H
#define TITAN_CAN_DRIVER_H

#include <QSerialPort>
#include "canDriverClassInterface.h"

class titanCanDriver: public canDriverInterface
{
   Q_OBJECT
   public:

   titanCanDriver();
   ~titanCanDriver(){};


    // Virtual function implementation
    bool hardwareInit(QByteArray COM, QByteArray br);
    bool hardwareShutdown();
    bool setStandardFilter(uint addr, uint msk);
    bool open(void);
    void close(void);
    canDataFrame driverTxRxData(canDataFrame frame, int tmo);

    _inline QByteArray getAddr(void){return ADDR;};
    _inline QByteArray getBr(void){return BR;};
    _inline QByteArray getMask(void){return MASK;};
    _inline QByteArray getVersion(void){return versionCode;};
    _inline QByteArray getSerialNumber(void){return SN;};
    _inline QByteArray getStatus(void){return STATUS;};
    _inline QByteArray getRxFrame(void){return rx_frame;};

    bool driverTx(canDriverInterface::canDataFrame frame);
    canDriverInterface::canDataFrame driverRx(void);
    void driverFlush(void);
    bool open_loopback(void);

private:
    QSerialPort virtualCom;
    QByteArray versionCode;
    QByteArray SN;
    QByteArray STATUS;
    QByteArray COM;
    QByteArray BR;
    QByteArray ADDR;
    QByteArray MASK;

    QByteArray CANID;
    QByteArray BDATA;

    char getHexVal(uchar val);
    QByteArray get11BitHexFormat(uint val);
    QByteArray get8BitHexFormat(uchar val);
    uint from11bitHex(QByteArray data);
    uchar from8bitHex(QByteArray data);
    bool checkHex(char data);

    bool running;
    QByteArray rx_frame;
    int rx_index;

    bool waitForRxData(void);

    canDriverInterface::canDataFrame rx_can_nok;
    canDriverInterface::canDataFrame rx_canframe;
};

#endif // TITAN_CAN_DRIVER_H
