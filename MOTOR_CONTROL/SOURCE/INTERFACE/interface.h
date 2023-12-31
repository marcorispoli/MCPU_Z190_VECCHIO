#ifndef INTERFACE_H
#define INTERFACE_H

#include "QObject"
#include "applicationInterface.h"

/*!
 * \defgroup  interfaceModule Application Command Interface Module.
 *
 * This Module implements the communication protocol with the Gantry.
 *
 * # CONNECTION DETAILS
 *
 * The Server IP address is: 127.0.0.1@10003.
 *
 * # EVENTS FROM GANTRY DESCRIPTION
 *
 * The Events from Gantry are considered *Commands* to be executed;
 *
 * Follows the list of the Implemented Commands:
 *
 * - SetExposurePre(): Gantry sets the next Pre Exposure pulse data;
 *
 *  > The Application will acknowledge the command ONLY to the client that sent the command;
 *
 *
 * # EVENTS TO GANTRY DESCRIPTION
 *
 * The Events to Gantry are considered *Events* to be notified to the Gantry;
 *
 * Follows a list of the implemented Events:
 *
 * - EventMessage(): this is an "error" message generated by the Sedecal Generator;
 *
 *  > The Application will forward the EVENTS to all the connected clients.
 *
 */


/**
 * @brief This class resides into the Main Thread and implements the
 * communication protocol with the Gantry.
 *
 * The class starts listening an incoming Client connection and creates\n
 * for any individual connection a given socket handler (SocketItem class). \n
 * Every Socket is assigned to a unique ID so that the Server can redirect the
 * answer frame with the sender client.
 *
 * The Reception and Transmission are implemented into the  QTcpServer library
 * that runs in a separate thread and exchanges data only through the SIgnal/Slot
 * mechanism.
 *
 *
 * \ingroup InterfaceModule
 */
class Interface : public applicationInterface
{
    Q_OBJECT

public:

    explicit Interface(QString ipaddress, int port);
    ~Interface(){};

    uint handleReceivedCommand(QList<QString>* frame, QList<QString>* answer); //!< Reimplement the handler for the received Commmands
    void configReceived(void){configuration.configuration_ok = true;}

private slots:
    void handleInterfaceStatus(void);

private:
    ushort wStatus;
    enum{
        _SYSTEM_CONFIGURATION = 0,
        _SYSTEM_RUNNING,
        _SYSTEM_DEVICE_RESET,
        _SYSTEM_FAULT
    }interfaceStatus;

    typedef enum{
        ACTIVATE_NONE = 0,
        ACTIVATE_ARM_CW,
        ACTIVATE_ARM_CCW,
        ACTIVATE_ARM_MCW,
        ACTIVATE_ARM_MCCW,
        ACTIVATE_ARM_ISOCW,
        ACTIVATE_ARM_ISOCCW,
        ACTIVATE_TRX_CW,
        ACTIVATE_TRX_CCW,
        ACTIVATE_TRX_TCCW,
        ACTIVATE_LIFT_UP,
        ACTIVATE_LIFT_DOWN,
        ACTIVATE_BODY_CW,
        ACTIVATE_BODY_CCW,
        ACTIVATE_SLIDE_UP,
        ACTIVATE_SLIDE_DOWN,
    }activationCommands;


    struct{
        bool configuration_ok;
    }configuration;


    ushort handleInterfaceConfigurationStatus(void);
    ushort handleInterfaceRunningStatus(void);
    ushort handleInterfaceResetDeviceStatus(void);
    ushort handleInterfaceFaultStatus(void);

    // Protocol Interface commands
    uint GetStatus(QList<QString>* command, QList<QString>* answer); //!< Gantry requests the current device status

};

#endif // SERVER_H
