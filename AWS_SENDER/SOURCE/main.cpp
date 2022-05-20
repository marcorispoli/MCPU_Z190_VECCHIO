
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
    return a.exec();
}
