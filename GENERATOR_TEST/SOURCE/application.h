#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>
#include "tcpipclient.h"
#include "startupWindow.h"
#include "communication.h"
#include "status_manager.h"

#define COMMUNICATION   pComm
#define WINDOWS         window
#define MANAGER         pStatus

// Global definitions
#ifdef MAIN_CPP
    startupWindow*              window;
    Communication*              pComm;
    statusManager*              pStatus;
#else
    extern startupWindow*       WINDOWS;
    extern Communication*       COMMUNICATION;
    extern statusManager*       pStatus;
#endif

#endif // APPLICATION_H
