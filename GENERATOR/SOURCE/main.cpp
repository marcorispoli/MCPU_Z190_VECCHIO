#define MAIN_CPP

#include "application.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    WINDOW = nullptr;

    if(argc>1) {
        for(int i=0; i< argc; i++){
            QString dato = QString(argv[i]);
            if(dato == "-dbg"){
                WINDOW = new startupWindow();
                WINDOW->show();
            }
        }

    }



    STATUS = new statusManager();
    pServer = new Server("127.0.0.1", 10002);
    pServer->Start();

    return a.exec();
}
