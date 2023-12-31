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
 * The Application can run without debug messages or alternatively
 * can be launched with -file, -win or -console option:
 * - -win: allows to run the application with a graphical window to
 * provide manual interaction with the Sedecal Generator;
 * - -file: the Application redirects the debug messages to a file
 * in C:/OEM/Logs/GANTRY/Motors.log
 * - -console: redirects the debug messages to the standard console
 *
 * # DEPENDENCIES AND CONFIGURATION FILES
 *
 *
 * # APPLICATION MODULES
 *
 * The Application implements the following modules:
 * - @ref applicationModule : defines constant along the application
 * - @ref libraryModules : this is the set of the libraries used in the current application;
 * - @ref interfaceModule : this is the communication socket to receive command from Gantry;
 * - @ref nanotecModule : this is the module implementing the Can Open Protocol of the Nanotec PD4 Motors;
 * - @ref trxModule : this is the module implementing the TRX motor control;
 * - @ref armModule : this is the module implementing the ARM motor control;
 * - @ref liftModule : this is the module implementing the LIFT motor control;
 * - @ref slideModule : this is the module implementing the SLIDE motor control;
 * - @ref bodyModule : this is the module implementing the BODY motor control;
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
 *
 * The Application uses only those libraries without change them and they are
 * dinamically linked into the application.
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
   static const char*           IP_CAN_ADDRESS = "127.0.0.1"; //!< Can Client Server IP address
   static const unsigned short  CAN_PORT = 10001; //!< Can Client Server Port
   static const char*           IP_INTERFACE_ADDRESS = "127.0.0.1"; //!< Motor Interface IP address
   static const unsigned short  INTERFACE_PORT = 10003; //!< Motor Interface Port

   static const unsigned char  TRX_ID = 3; //!< Can Open Address of the TRX driver
   static const unsigned char  ARM_ID = 4; //!< Can Open Address of the ARM driver
   static const unsigned char  LIFT_ID = 5; //!< Can Open Address of the LIFT driver
   static const unsigned char  SLIDE_ID = 6; //!< Can Open Address of the SLIDE driver
   static const unsigned char  BODY_ID = 7; //!< Can Open Address of the BODY driver

}


#include <QApplication>
#include <QObject>
#include <QTimer>
#include "time.h"
#include "canclient.h"
#include "pd4.h"
#include "trx.h"
#include "window.h"
#include "interface.h"


#define WINDOW          window
#define INTERFACE       pInterface
#define DEBUG           window
#define TRX             pTrx
#define ARM             pArm
#define LIFT            pLift
#define BODY            pBody
#define SLIDE           pSlide


// Global definitions
#ifdef MAIN_CPP
     debugWindow*                WINDOW;
     Interface*                  INTERFACE;
     trxModule*                  TRX;
     pd4Nanotec*                 ARM;
     pd4Nanotec*                 BODY;
     pd4Nanotec*                 LIFT;
     pd4Nanotec*                 SLIDE;
#else
    extern debugWindow*                WINDOW;
    extern Interface*                  INTERFACE;
    extern trxModule*                  TRX;
    extern pd4Nanotec*                 ARM;
    extern pd4Nanotec*                 BODY;
    extern pd4Nanotec*                 LIFT;
    extern pd4Nanotec*                 SLIDE;

#endif

#ifdef TIME_MEASURE
    #define T1START  this->t1 = clock()
    #define T1MEASURE(x) { qDebug() << x << ((double)(clock() - this->t1)/CLOCKS_PER_SEC);}
#else
    #define T1START
    #define T1MEASURE(x)
#endif

#endif // APPLICATION_H
