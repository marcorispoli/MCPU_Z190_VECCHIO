#include "application.h"
#include <QStringConverter>

/**
 * Interface class constructor
 *
 * @param
 * - ipaddress: IP where server will be bounded;
 * - port: bounding port
 *
 * \ingroup InterfaceModule
 */
Interface::Interface(QString ipaddress, int port):applicationInterface(ipaddress,port)
{

}

/**
 * This is the Base Class method  Command handler.
 *
 * The Command handler shall be reimplemented by the Subclass
 * in order to implement the Command interface.
 *
 * The Handler shall return an error code different from zero,
 * in case of an error condition.
 *
 * The Handler shall get the Command Name from frame->at(2)
 * and the optionals parameters from frame->at(3) ... to frame->at(n);
 *
 * The Handler shall populate the Answer list with the optional acknowledge params.
 *
 * @param frame
 *  - frame[2]: is the command string;
 *  - frame[3] to [n]: if presents represents the optional parameters of the command;
 *
 * @param answer
 * - is a list of optonal parameters that the application can send to Client acknowledging a command.
 *
 * @return
 * - is an uint error code:
 *  - 0: no error;
 *  - <>0: error condition;
 */
uint Interface::handleReceivedCommand(QList<QString>* frame, QList<QString>* answer){

    if(frame->at(2) == "SetExposurePre")    return SetExposurePre(frame, answer);
    if(frame->at(2) == "SetExposurePulse")  return SetExposurePulse(frame, answer);
    if(frame->at(2) == "SetTomoConfig")     return SetTomoConfig(frame, answer);
    if(frame->at(2) == "StartExposure")     return StartExposure(frame, answer);
    if(frame->at(2) == "AbortExposure")     return AbortExposure(frame, answer);
    if(frame->at(2) == "GetPostExposure")   return GetPostExposure(frame, answer);

    return false;
}

