
#include "startupWindow.h"
#include "translator.h"
#include "windows.h"
#include "errorsWindow.h"
#include "systemconfig.h"
#include "awscommunication.h"

#include <QApplication>
startupWindow* w1;
startupWindow* w2;
errorsWindow*  pErrorsWindow;   // Pointer to the error window page
systemConfig*  pSysConfig;
awsCommunication*  pAws;

//#define HOME
#define DESKTOP
//#define WORK

#ifdef HOME
    #define MONITOR_PANEL   1366,768
    #define MONITOR_WS      1366,768
#endif
#ifdef DESKTOP
    #define MONITOR_PANEL   1440,900
    #define MONITOR_WS      1920,1080
#endif
#ifdef WORK
    #define MONITOR_PANEL   1440,900
    #define MONITOR_WS      1920,1080
#endif


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    // Crea la classe di traduzione assegnando per default la lingua italiana
    translatorClass translation(translatorClass::_LNG_ENG);

    // Configuration management class instance
    pSysConfig = new systemConfig();

    // Error window instance
    pErrorsWindow = new errorsWindow(MONITOR_PANEL);

    w1 = new startupWindow(MONITOR_PANEL);
    w2 = new startupWindow(MONITOR_WS);
    w1->openWindow();
    w2->openWindow();

    pAws = new awsCommunication(pSysConfig->sysini.getParam<QString>(AWS_ADDRESS_PARAM,0), pSysConfig->sysini.getParam<int>(AWS_ADDRESS_PARAM,1));


    return a.exec();
}
