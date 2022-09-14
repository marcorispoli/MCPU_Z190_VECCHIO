#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>
#include <QObject>
#include <QTimer>
#include "tcpipclient.h"
#include "Typedef.h"
#include "startupWindow.h"
#include "communication.h"
#include "status.h"
#include "server.h"


#define COMMUNICATION   pComm
//#define WINDOWS         window
#define INTERFACE       pServer
#define DEBUG           window
#define STATUS          pStatus

// Global definitions
#ifdef MAIN_CPP
    startupWindow*              window;
    Communication*              pComm;
    statusManager*              pStatus;
    Server*                     pServer;
#else
    extern startupWindow*       window;
    extern Communication*       pComm;
    extern statusManager*       pStatus;
    extern Server*              pServer;
#endif





#endif // APPLICATION_H
