
#include "server.h"
#include "configuration.h"
#include "can_driver.h"
#include <QApplication>

titanCanDriverConfiguration*   pConfig;
Server*   pServer;
titanCanDriver*   pCanDriver;


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QString info;


    // Open the Acquisition software communication channel
    pConfig = new titanCanDriverConfiguration();

    pServer = new Server(pConfig->getParam<QString>(INTERFACE_ADDRESS,0), pConfig->getParam<ushort>(INTERFACE_ADDRESS,1));
    pCanDriver = new titanCanDriver();

    if(!pCanDriver->driverOpen(pConfig->getParam<QByteArray>(VIRTUAL_COM,0),titanCanDriver::_CAN_1000K,0x100,0,pConfig->getParam<QByteArray>(CAN_SETUP,1))){
        qDebug("CAN OPEN FAILED!\n");
    }else{
        info = QString("CAN DRIVER SN:%1").arg(pCanDriver->getSerialNumber());
        qDebug() << info;
        info = QString("CAN DRIVER REVISION:%1").arg(pCanDriver->getVersion());
        qDebug() << info;
    }

    QObject::connect(pCanDriver,SIGNAL(receivedCanFrame(ushort , QByteArray )), pServer, SLOT(receivedCanFrame(ushort , QByteArray)),Qt::QueuedConnection);
    QObject::connect(pServer,SIGNAL(sendToCan(ushort , QByteArray )), pCanDriver, SLOT(sendOnCanSlot(ushort , QByteArray)),Qt::QueuedConnection);


    pServer->Start();

    return a.exec();
}
