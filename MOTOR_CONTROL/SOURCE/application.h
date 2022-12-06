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
 *
 * # DEPENDENCIES AND CONFIGURATION FILES
 *
 *
 * # APPLICATION MODULES
 *
 * The Application implements the following modules:
 * - @ref applicationModule : defines constant along the application
 * - @ref interfaceModule : this is the communication socket to receive command from Gantry;
 * - @ref canModule : this is the communication socket to connect the Can Driver application;
 * - @ref canOpenModule : this is the module implementing the Can Open Protocol;
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
#include "server.h"




#define CANCLIENT       pCan
#define WINDOW          window
#define INTERFACE       pServer
#define DEBUG           window
#define TRX             pTrx

// Global definitions
#ifdef MAIN_CPP
    debugWindow*                window;
    canClient*                  pCan;
    Server*                     pServer;
    trxModule*                  pTrx;
#else
    extern debugWindow*                window;
    extern canClient*                  pCan;
    extern Server*                     pServer;
    extern trxModule*                  pTrx;
#endif

#ifdef TIME_MEASURE
    #define T1START  this->t1 = clock()
    #define T1MEASURE(x) { qDebug() << x << ((double)(clock() - this->t1)/CLOCKS_PER_SEC);}
#else
    #define T1START
    #define T1MEASURE(x)
#endif

#endif // APPLICATION_H
