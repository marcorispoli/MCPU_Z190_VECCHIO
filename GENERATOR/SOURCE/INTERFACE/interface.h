#ifndef INTERFACE_H
#define INTERFACE_H

#include "Typedef.h"
#include "applicationInterface.h"

/*!
 * \defgroup  InterfaceModule Gantry Interface Module.
 *
 * This Module implements the communication protocol with the Gantry.
 *
 * The communication protocol is Based on the applicationInterface library module.
 *
 * # CONNECTION DETAILS
 *
 * The Server IP address is: 127.0.0.1@10002.
 *
 *
 * # COMMANDS DESCRIPTION
 *
 *
 * Follows the list of the Implemented Commands:
 *
 * - Interface::SetExposurePre(): Gantry sets the next Pre Exposure pulse data;
 * - Interface::SetExposurePulse(): Gantry sets the next Pulse Exposure data;
 * - Interface::SetTomoConfig(): Gantry sets the data for the next Tomo exposure;
 * - Interface::StartExposure(): Gantry starts the execution of the Exposure sequence;
 * - Interface::AbortExposure(): Gantry aborts the current executing exposure sequence;
 * - Interface::GetPostExposure(): Gantry requests the list of the pulses of the last exposure;
 *
 *
 * # EVENTS DESCRIPTION
 *
 *
 * Follows a list of the implemented Events:
 *
 * - Interface::EventMessage(): this is an "error" message generated by the Sedecal Generator;
 * - Interface::EventSetXrayEna(): this event notify the Gantry to activate/Deactivate the hardware signal XRAY-ENA;
 * - Interface::EventGetPulseData(): this event requests the Pulse data to the Gantry (AEC mode);
 * - Interface::EventXrayCompleted(): this event notifies Gantry about the Exposure complete event;
 * - Interface::EventStatus(): this event updates Gantry with the Sedecal Generator internal status;
 *
 *
 */



/**
 * @brief This class implement the Command and Event interface
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

public:

    // Lists of the event from Gantry
    void EventStatus(void);
    void EventMessage(QString msg);
    void EventSetXrayEna(bool state);
    void EventXrayCompleted(uchar code, float masPre, float masPulse, uchar error);
    void EventGetPulseData(void);

    // Lists of the event to Gantry
    uint SetExposurePre(QList<QString>* command, QList<QString>* answer);
    uint SetExposurePulse(QList<QString>* command, QList<QString>* answer);
    uint SetExposureTestPulse(QList<QString>* command, QList<QString>* answer);
    uint SetTomoConfig(QList<QString>* command, QList<QString>* answer);
    uint StartExposure(QList<QString>* command, QList<QString>* answer);
    uint AbortExposure(QList<QString>* command, QList<QString>* answer);
    uint GetPostExposure(QList<QString>* command, QList<QString>* answer);

};


#endif // SERVER_H
