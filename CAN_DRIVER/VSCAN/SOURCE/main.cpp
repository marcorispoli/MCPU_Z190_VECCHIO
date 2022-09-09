
#include "server.h"
#include "configuration.h"
#include "can_driver.h"
#include <QApplication>

titanCanDriverConfiguration*   pConfig;
Server*   pServer;
VscanCanDriver*   pCanDriver;

//#define LOOPBACK

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QString info;


#ifdef LOOPBACK
    pCanDriver = new VscanCanDriver();
    pCanDriver->driverOpenLoopback(VscanCanDriver::_CAN_1000K);
#else
    // Open the Acquisition software communication channel
    pConfig = new titanCanDriverConfiguration();

    pServer = new Server(pConfig->getParam<QString>(INTERFACE_ADDRESS,0), pConfig->getParam<ushort>(INTERFACE_ADDRESS,1));
    pCanDriver = new VscanCanDriver();
    pCanDriver->driverOpen(VscanCanDriver::_CAN_1000K);

    QObject::connect(pCanDriver,SIGNAL(receivedCanFrame(ushort , QByteArray )), pServer, SLOT(receivedCanFrame(ushort , QByteArray)),Qt::QueuedConnection);
    QObject::connect(pServer,SIGNAL(sendToCan(ushort , QByteArray )), pCanDriver, SLOT(sendOnCanSlot(ushort , QByteArray)),Qt::QueuedConnection);


    pServer->Start();
#endif


    return a.exec();
}
