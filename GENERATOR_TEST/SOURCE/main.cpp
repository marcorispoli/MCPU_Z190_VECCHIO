
#include "startupWindow.h"
#include <QApplication>

startupWindow*      window;


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    window = new startupWindow();

    window->show();
    return a.exec();
}
