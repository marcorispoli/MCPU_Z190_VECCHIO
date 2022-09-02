#define MAIN_CPP

#include "application.h"




int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    STATUS = new statusManager();
    return a.exec();
}
