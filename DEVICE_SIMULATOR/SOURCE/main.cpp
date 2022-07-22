
#include "startupWindow.h"
#include "power_service.h"
#include <QApplication>

startupWindow*      window;
powerService*       pService;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);




    window = new startupWindow();

    // Open the power service board
    pService = new powerService(1,0,0,0x201,"127.0.0.1", 10010);

    window->initWindow();
    window->show();
    return a.exec();
}
