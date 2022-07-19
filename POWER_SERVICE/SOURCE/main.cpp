
#include "server.h"
#include "can_client.h"
#include "device.h"
#include <QApplication>


Server*   pServer;
canClient*   pCan;
Device*   pDevice;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QString info;



    pCan = new canClient();
    pCan->Start("127.0.0.1", 10001);

    pServer = new Server("127.0.0.1", 10002);
    pServer->Start();


    pDevice = new Device();
    QObject::connect(pDevice, SIGNAL(txToCanDriver(QByteArray)), pCan, SLOT(txData(QByteArray )), Qt::UniqueConnection);
    QObject::connect(pCan, SIGNAL(rxData(QByteArray )), pDevice, SLOT(rxFromCanDriver(QByteArray)), Qt::UniqueConnection);
    QObject::connect(pCan, SIGNAL(clientConnection(bool)), pDevice, SLOT(canConnection(bool)), Qt::UniqueConnection);

    return a.exec();
}
