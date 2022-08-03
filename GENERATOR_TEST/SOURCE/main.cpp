#define MAIN_CPP

#include "application.h"




int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    MANAGER = new statusManager();
    return a.exec();
}
