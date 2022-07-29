
#include "startupWindow.h"
#include "communication.h"
#include <QApplication>

startupWindow*      window;
Communication*  pComm;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    window = new startupWindow();

    pComm = new Communication();


    window->show();
    return a.exec();
}
