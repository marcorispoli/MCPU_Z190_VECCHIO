#ifndef APPLICATION_H
#define APPLICATION_H

/*!
 * \defgroup  applicationModule Application Module.
 */

/*!
 * \defgroup  libraryModules Library Modules Used in the Application.
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
 * - @ref applicationModule : defines constant along the application
 * - @ref libraryModules : this is the set of the libraries used in the current application;
 * - @ref r2cpCommunicationModule : implements the communication protocol stack with the R2CP protocol;
 * - @ref exposureModule : Implements the exposure sequence workflows;
 * - @ref InterfaceModule : this is the communication socket to receive command from Gantry;
 * - @ref R2CP : this is the module implementing the communication with the Sedecal Generator Device;
 * - @ref workflowModule : this is the module implementing the application Workflows;
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


/**
* @brief Application constants definition
*
*  This Namespace defines all the MAIN constants that are used along the \n
*  Application in order to interface the Generator and the Gantry.
*
* \ingroup applicationModule
*/
namespace Application
{
   static const char*           SQL_GENERATOR_DATABASE = "C:/OEM/AppData/system_messages.sqlite"; //!< SQLLite database name and path for Generator System Messages
   static const char*           SH_IP_ADDRESS = "192.6.1.201"; //!< Smart Hub Ip address
   static const unsigned short  SH_PORT = 10000; //!< Smart Hub Port Number
   static const unsigned short  SH_NODE_ID = 1; //!< Node ID of the Smart Hub
   static const unsigned short  GENERATOR_NODE_ID = 6; //!< Generator Node ID
   static const unsigned short  APPLICATION_NODE_ID = 17; //!< Node ID of the Application
   static const unsigned short  DIRECT_WORKSTATION_ID = 1; //!< Workstation ID for Test Exposures without synch
   static const unsigned short  DETECTOR_WORKSTATION_ID = 5; //!< Workstation ID for Detector synchronization signals

}


#include <QApplication>
#include <QObject>
#include <QTimer>
#include "tcpipclient.h"
#include "Typedef.h"
#include "startupWindow.h"
#include "communication.h"
#include "exposure.h"
#include "workflow.h"
#include "interface.h"




#define COMMUNICATION   pComm
#define WINDOW          window
#define INTERFACE       pInterface
#define DEBUG           window
#define STATUS          pStatus
#define EXPOSURE        pExposure

// Global definitions
#ifdef MAIN_CPP
    startupWindow*              window;
    Communication*              pComm;
    workflowManager*            pStatus;
    exposureManager*            pExposure;
    Interface*                  pInterface;
#else
    extern startupWindow*       window;
    extern Communication*       pComm;
    extern workflowManager*     pStatus;
    extern exposureManager*     pExposure;
    extern Interface*           pInterface;
#endif






#endif // APPLICATION_H
