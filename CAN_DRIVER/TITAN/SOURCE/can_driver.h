#ifndef TITAN_CAN_DRIVER_H
#define TITAN_CAN_DRIVER_H

#include <QSerialPort>
#include <QTimer>
#include <QTimerEvent>

class titanCanDriver: public QObject
{
   Q_OBJECT
   public:

   titanCanDriver();
   ~titanCanDriver(){};


   enum{
    _CAN_1000K = 0,
    _CAN_500K,
    _CAN_250K,
    _CAN_100K
    }_titanCanBR;

    void driverClose(void);
    bool driverOpen(QByteArray COM, uchar BR, uint address, uint mask, QByteArray mode);
    bool driverRead(void);


    // For test purpose
    _inline QByteArray getAddr(void){return ADDR;};
    _inline QByteArray getMask(void){return MASK;};
    _inline QByteArray getVersion(void){return versionCode;};
    _inline QByteArray getSerialNumber(void){return SN;};
    _inline QByteArray getStatus(void){return STATUS;};
    _inline QByteArray getRxFrame(void){return rx_frame;};


signals:
    void receivedCanFrame(ushort id, QByteArray data);

public slots:
    void sendOnCanSlot(ushort canId, QByteArray data);

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
    bool startCanCommunication(void);
    void close(void);
    void clearBuffer(uint i);

    ushort rxCanId;
    QByteArray rxCanData;
    void timerEvent(QTimerEvent* ev);
    int canRxTimer;
};

#endif // TITAN_CAN_DRIVER_H
