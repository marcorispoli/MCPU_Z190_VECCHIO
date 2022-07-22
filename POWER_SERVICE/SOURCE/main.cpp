
#include "power_service.h"
#include <QApplication>


powerService*   pDevice;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QString info;


    pDevice = new powerService(0x201, "127.0.0.1", 10001, 10002);
    return a.exec();
}
