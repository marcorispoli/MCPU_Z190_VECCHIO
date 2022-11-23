#ifndef APPLICATION_H
#define APPLICATION_H

/*!
 * \defgroup  MainModule APPLICATION.
 */



/*!
 * \mainpage
 *
 * # GENERAL OVERVIEW
 *
 * This Executable allows to interface the Sedecal Generator
 * with the Gantry Application.
 *
 * The Application connects as a Client the SmartHub at the IP:192.6.1.201@10000.
 *
 * The Application implements the R2CP Protocol revision 6.0.A.
 *
 * The Application provides a Server input socket at IP:127.0.0.1@10002
 * to interface with the Gantry application (or other applications implementing the
 * application cprotocol command).
 *
 * The Application can run without debug messages or alternatively
 * can be launched with -log or -win option:
 * - -win: allows to run the application with a graphical window to
 * provide manual interaction with the Sedecal Generator;
 * - -log: the Application redirects the debug messages to a file
 * in C:/OEM/Logs/SERVICELOG/gantrylog.log
 *
 * # DEPENDENCIES AND CONFIGURATION FILES
 *
 * The Application makes use of the SQLLite driver in order
 * to read the C:/OEM/AppData/system_messages.sqlite
 * to get the current error messages description of the
 * Sedecal Generator system message.
 *
 * # APPLICATION MODULES
 *
 * The Application implements the following modules:
 * - Gantry Interface Module: this is the communication socket to receive command from Gantry;
 * - R2CP Module: this is the module implementing the communication with the Sedecal Generator Device;
 * - Status Module: this is the module implementing the Application Workflow and Exposure sequences;
 * - Window Module: this is an optional Windows interface used for Service/Debug
 *
 * # SOFTWARE LICENCING
 *
 * The Application has been developed with the Qt Libraries
 * with the LGPL licences.
 *
 * The Application uses the following Libraries:
 * - Network (LGPL)
 * - Core (LGPL);
 * - SQL (FREE);
 *
 * The Application uses only those libraries without change them and they are
 * dinamically linked into the application.
 *
 *
 *
 *
 *
 */

#include <QApplication>
#include <QObject>
#include <QTimer>
#include "tcpipclient.h"
#include "Typedef.h"
#include "startupWindow.h"
#include "communication.h"
#include "status.h"
#include "server.h"

#define _WINDOW_ON

#define COMMUNICATION   pComm
#define WINDOW          window
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
