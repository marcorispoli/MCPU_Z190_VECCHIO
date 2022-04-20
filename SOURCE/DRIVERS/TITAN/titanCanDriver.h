#ifndef TITAN_CAN_DRIVER_H
#define TITAN_CAN_DRIVER_H

#include <QSerialPort>
#include "canDriverClassInterface.h"

class titanCanDriver: public canDriverInterface
{
   Q_OBJECT
   public:

   titanCanDriver(){};
   ~titanCanDriver(){};


    // Virtual function implementation
    bool hardwareInit(QString COM, QString br);
    bool hardwareShutdown();
    bool setStandardFilter(uint addr, uint msk);
    void open(void);
    void close(void);
    canDataFrame driverTxRxData(canDataFrame frame, int tmo);

    _inline QString getAddr(void){return ADDR;};
    _inline QString getBr(void){return BR;};
    _inline QString getMask(void){return MASK;};
    _inline QByteArray getVersion(void){return versionCode;};
    _inline QByteArray getSerialNumber(void){return SN;};
    _inline QByteArray getStatus(void){return STATUS;};

private:
    QSerialPort virtualCom;
    QByteArray versionCode;
    QByteArray SN;
    QByteArray STATUS;


    QString COM;
    QString BR;
    QString ADDR;
    QString MASK;

    bool running;

};

#endif // TITAN_CAN_DRIVER_H
