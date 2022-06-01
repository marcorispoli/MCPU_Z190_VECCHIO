#ifndef TITAN_CAN_DRIVER_H
#define TITAN_CAN_DRIVER_H

#include <QSerialPort>
#include "can_driver_class_interface.h"

class titanCanDriver: public canDriverInterface
{
   Q_OBJECT
   public:

   titanCanDriver();
   ~titanCanDriver(){};


    _canDriverErrors driverOpen(QByteArray COM, _canBaudRate BR, uint address, uint mask, QByteArray mode);
    void driverClose(void);
    bool driverRead(void);
    void driverWrite(canDriverInterface::canDataFrame frame);

    // For test purpose
    _inline QByteArray getAddr(void){return ADDR;};
    _inline QByteArray getMask(void){return MASK;};
    _inline QByteArray getVersion(void){return versionCode;};
    _inline QByteArray getSerialNumber(void){return SN;};
    _inline QByteArray getStatus(void){return STATUS;};
    _inline QByteArray getRxFrame(void){return rx_frame;};

    //canDriverInterface::canDataFrame driverRx(uint tmo);
    //void driverFlush(void);


private:
    QSerialPort* virtualCom;
    QByteArray versionCode;
    QByteArray SN;
    QByteArray STATUS;
    QByteArray ADDR;
    QByteArray MASK;
    QByteArray openMode;

    QByteArray CANID;
    QByteArray BDATA;

    char getHexVal(uchar val);
    QByteArray get11BitHexFormat(uint val);
    QByteArray get8BitHexFormat(uchar val);
    uint from11bitHex(QByteArray data);
    uchar from8bitHex(QByteArray data);
    bool checkHex(char data);

    QByteArray rx_frame;
    int rx_index;




    bool setStandardFilter(uint addr, uint msk);
    bool open(void);
    void close(void);
    void clearBuffer(uint i);
    bool isOpen;
};

#endif // TITAN_CAN_DRIVER_H
