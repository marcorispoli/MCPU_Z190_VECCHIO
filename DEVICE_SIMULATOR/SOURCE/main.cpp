
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
    pService = new powerService(0x201,1,0,0);

    window->initWindow();
    window->show();
    return a.exec();
}
