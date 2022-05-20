
#include "startupWindow.h"
#include "awscommunication.h"
#include <QApplication>

startupWindow*      window;
awsCommunication*   pAws;


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    window = new startupWindow();
    window->show();


    // Open the Acquisition software communication channel
    pAws = new awsCommunication("127.0.0.1", 10001);


    return a.exec();
}
