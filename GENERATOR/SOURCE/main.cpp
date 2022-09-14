#define MAIN_CPP

#include "application.h"




int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    window = new startupWindow();
    window->show();

    STATUS = new statusManager();
    pServer = new Server("127.0.0.1", 10002);
    pServer->Start();
    return a.exec();
}
