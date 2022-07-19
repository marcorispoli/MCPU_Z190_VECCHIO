
#include "startupWindow.h"
#include "communication.h"
#include <QApplication>

startupWindow*      window;
Communication*      pCommunication;


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    window = new startupWindow();
    pCommunication = new Communication();

    window->show();
    return a.exec();
}
