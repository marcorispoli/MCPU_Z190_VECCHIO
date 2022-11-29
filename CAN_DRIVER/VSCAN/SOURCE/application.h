#ifndef APPLICATION_H
#define APPLICATION_H

/*!
 * \defgroup  applicationModule Application Module.
 */



/*!
 * \mainpage
 *
 * # GENERAL OVERVIEW
 *
 * The Application provides a Server input socket at IP:127.0.0.1@10001
 * to interface with the Gantry application (or other applications implementing the
 * can interface protocol).
 *
 * The Application can run with rthe following options:
 *
 * - -win: allows to run the application with a graphical window to \n
 *      provide manual interaction with the Can Driver. The Debug strings will be
 *      redirect into the Window panel.
 * - -log: the Application redirects the debug messages to a file:\n
 *      C:/OEM/Gantry/candriver.log
 * - -canLoopback: the can Device operates in loopback mode.

 * # DEPENDENCIES AND CONFIGURATION FILES
 *
 *
 * # APPLICATION MODULES
 *
 * The Application implements the following modules:
 * - @ref applicationModule : defines constant along the application
 * - @ref candriverModule : implements the communication interface with the CAN Network
 *
 * - @ref windowModule : this is an optional Windows interface used for Service/Debug;
 *
 * # SOFTWARE LICENCING
 *
 * The Application has been developed with the Qt Libraries
 * with the LGPL licences.
 *
 * The Application uses the following Libraries:
 * - Network (LGPL)
 * - Core (LGPL);
 * - vs_can_api (LGPL)
 *
 * The Application uses only those libraries without change them and they are
 * dinamically linked into the application.
 *
 */


#include <QApplication>
#include <QObject>
#include <QTimer>
#include "tcpipclient.h"


#include "can_driver.h"
#include "server.h"
#include "window.h"


#define WINDOW          pWindow
#define DEBUG           pWindow
#define INTERFACE       pServer
#define CAN             pCanDriver

// Global definitions
#ifdef MAIN_CPP
    Server*   pServer;
    VscanCanDriver*   pCanDriver;
    debugWindow* pWindow;

#else
    extern  Server*   INTERFACE;
    extern  VscanCanDriver*   CAN;
    extern  debugWindow* WINDOW ;

#endif



    /**
    * @brief Application constants definition
    *
    *  This Namespace defines all the MAIN constants that are used along the \n
    *  Application.
    *
    * \ingroup applicationModule
    */
    namespace Application
    {
       static const char*  SERVER_IP = "127.0.01"; //!< Server Interface Ip Address
       static const unsigned short  SERVER_PORT = 10001; //!< Server Interface Port
       static const VscanCanDriver::_CanBR CAN_BAUDRATE = VscanCanDriver::_CAN_1000K;

    }


#endif // APPLICATION_H
