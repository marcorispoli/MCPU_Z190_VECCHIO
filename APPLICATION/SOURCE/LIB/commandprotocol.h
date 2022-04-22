#ifndef COMMANDPROTOCOL_H
#define COMMANDPROTOCOL_H

#include <QtCore>
#include <QStringConverter>

/**
 * @brief This is the implementation of the Command Protocol interface
 *
 *
 * This class implements the features of the Command protocol interface.
 * See \ref PG_TD_AWS_PROTO
 *
 *
 * The protocol is character based, supporting several encoding standards: Latin1, Utf8, Utf16\n
 * See the Qt QStringConverter::Encoding data type for more details.
 *
 * # PROTOCOL DESCRIPTION
 *
 * The protocol disciplines two kind of data exchange transactions:
 *
 * [1] Command and answer transaction:
 * + Client Command Frame -> Server
 *   + Server answer frame -> Client
 *
 * [2] Server status change transaction:
 * + Server Status Change Frame -> Client
 * + No answer from Client is needed!
 *
 * In the transaction [1] the Client sends commands to the Server;\n
 * the Server shall process the command and notifies the resultsto the Client;
 *
 * In the transaction [2] the Server sends Status frames used to notify the Client\n
 * about its the internal status. Those frames shall not be knowledged by the Client program.
 *
 * The Command frame of [1] contains an ID code that is reported back in the Server Answer frame:\n
 * this code can be either a progressive code or a unique code. \n
 *
 * In the Command/Answer scheme of [1], Server can sends back several possible answer frame types:
 * + <Immediate OK> frame: the Server successfully processed the command;
 * + <Immediate NOK> frame: the Server failed to process the command;
 * + <Delayed Command> frame: the Server accepts the command but the command takes time to complete;
 *    + <Delay Completed> Frame: the Server shall send back this frame to notify the command completion to the Client;
 * + Command not implemented (or NA) frame: the requested command is not supported or not implemented;
 *
 * # DATA FRAME FORMAT
 *
 * ## DATA TYPE AND ENCODING FORMAT
 *
 * The frames are composed by characters formatted with a given character encoding standard;\n
 * All the parameter fields shall be converted from its string representation\n
 * to the target type, based on the specific command implementation.\n
 *
 * ## STRUCTURE OF DATA FRAME
 *
 * The data frame formats are of four possible forms:
  \verbatim
   [1]  <ID FRAME_LEN % CMD PAR1 .. PAR-N %>            Client command frame;
   [2]  <ID FRAME_LEN % "OK/NOK" CODE OPT1 .. OPT-N %>  Server answer frame;
   [3]  <'A' FRAME_LEN % ID CODE OPT1 .. OPT-N %>       Server delayed completed command answer;
   [4]  <'S' FRAME_LEN % CMD PAR1 .. PAR-N  %>          Server Status command to Client

    Where:
    '<' and '>  : delimitators of the data frame
    '%'         : delimitators of the frame content;
    ID          : command idntification code
    'A'         : key character identifying the Gantry <Delay Completed Frame> answer frame format;
    'S'         : key character identifying the Gantry <Status> command frame format;
    FRAME_LEN   : integer number in string format, equals to the total number of characters composing the frame;
    PAR1 - PAR-N: list of command parameters, in string format, separated by spaces;
    OPT1 - OPT-N: list of optionnal parameters, in string format, separated by spaces;

   \endverbatim
 *
 *
 * ## <COMMAND> DATA FRAME FORMAT [1]
 *
 * The Command data frame frame format is based on the form [1] as following described:
 * \verbatim
    <ID FRAME_LEN % COMMAND PARAM-1 PARAM-2 .... PARAM-N %>
    Where:
    COMMAND: is a String defining a command code;
    PARAM-1, .. , PARAM-N: are a list of parameters, in string format, of variant type. The actual type depends by the command implementation.
   \endverbatim
 *
 * ## <IMMEDIATE OK> DATA FRAME FORMAT [2]
 *  The <immediate ok> data frame format is based on the form [2];\n
 *  It shall be sent back by the Server if the requested command has been successfully processed.
 *
 *  \verbatim
    <ID FRAME_LEN % OK 0 OPT1 ... OPT-N %>
    Where:
    ID: is the ID of the processed Command Frame
    OPT1 .. OPT-N: are optionals data sent back to the Sender
    The " OK 0 " part of the frame identifies the frame type.
   \endverbatim
 *
 *
 * ## <DELAYED OK> DATA FRAME FORMAT [2]
 *  The <delayed ok> data frame format is based on the form [2];\n
 *  It shall be sent back by the Server if the command takes time to completes.
 *
 *  \verbatim
    <ID FRAME_LEN % OK TIME OPT1 ... OPT-N %>
    Where:
    ID: is the ID of the processed Command Frame;
    TIME: this is a number defining approximatelly a waiting time.
    OPT1 .. OPT-N: are optional data sent back in string format.
   \endverbatim
 *
 * As soon as the command terminates, the Server shall send back to the Client\n
 * the <Delay Completed Frame> data format [3].\n
 *
 *  \verbatim
    <A FRAME_LEN % ID REASULT OPT1 .. OPT-N %>
    Where:
    ID: is the ID of the processed Command Frame
    REASULT:
        - 0: if the command is successfully terminated;
        - CODE: error code;
    OPT1 .. OPT-N: are optional data sent back in string format.
   \endverbatim
 *
 * ## <IMMEDIATE NOK> DATA FRAME FORMAT [2]
 *  The <immediate NOK> data frame format is based on the form [2];\n
 *  It shall be sent back by the Server in the case the command fails or\n
 *  cannot be executed.
 *
 * The format is the following:
 *
 *  \verbatim
    <ID FRAME_LEN % NOK ERRCODE OPT1 ... OPT-N %>
    Where:
    ID: is the ID of the processed Command Frame;
    ERRCODE: is the failure code;
    OPT1 .. OPT-N: are optional data sent back in string format.
   \endverbatim
 *
 * ## <NA> DATA FRAME FORMAT [2]
 *  The <NA> data frame format is based on the form [2];\n
 *  It shall be sent back by the Server in the case \n
 *  the requested command should not be implemented or not supported:
 *
 *  \verbatim
    <ID FRAME_LEN % NA %>
   \endverbatim
 *
 * ## <STATUS> DATA FRAME FORMAT [4]
 *  The <STATUS> data frame format is based on the form [4];\n
 *  It shall be sent by the Server in order to notify the Client
 *  of a change of internal status.
 *
 *  The frame format is the following:
 *
\verbatim
    <'S' FRAME_LEN % CMD OPT1 .. OPT-N  %>
    Where:
    'S': is the status frame identifier character;
    CMD: is the notify command;
    OPT1 .. OPT-N: is a list of optional variable status.
   \endverbatim
 *
 * @attention
 * The Client shall never acknowledge a status frame.
 *
 * # PARAMETER FORMAT SPECIAL CONSIDERATIONS
 *
 * The protocol exchanges Commands and data in string format;
 * this mean that an Integer, float, String data can be exchanged
 * converting it in a string.
 *
 * The data frame can be formatted with one of possible
 * data encoding: UTF8, UTF16, ...
 *
 * In order to successfully communicate,
 * Client and Server shall share the same data encoding (example: UTF-16)
 *
 * Because of data and command items are separated by spaces (the number of spaces doesn't matter)\n
 * if the Sender should send a composed string as a unique parameter, he shall
 * delimit it into the " " characters.
 *
 * Example:
 * \verbatim
    In this case
    <ID FRAME_LEN % OpenStudy "Pico della Mirandola" %>
    Decoded items:
        command = OpenStudy;
        param-1 = "Pico della Mirandola"

    In this case
    <ID FRAME_LEN % OpenStudy Pico della Mirandola %>
    Decoded items:
        command = OpenStudy;
        param-1 = "Pico";
        param-2 = "della";
        param-3 = "Mirandola"
  \endverbatim
 *
 * # USAGE
 * ## CLIENT IMPLEMENTATION
 *
 * + Client composes a command frame using the static method:
 *  + commandProtocol::formatCommand(ENCODING, ID, COMMAND, (opt.) paramlist). See commandProtocol::formatCommand();
 * + Client shall handle a reception stream of QByteArray:

   \verbatim
   QByteArray rxframe = is the received answer frame;
   Here, a an example, the encoding method used is Utf8;

   commandProtocol proto  = commandProtocol(&rxframe,QStringConverter::Encoding::Utf8);
   switch(proto.getDecodedType()){
        case commandProtocol::_OK_FRAME:
            QList<QString>* params = proto.getParams(); is the pointer to the parameter list,
            ....
        break;
        case commandProtocol::_DELAYED_FRAME:
            The command is in execution ...
        break;
        case commandProtocol::_NOK_FRAME:
            The command has been rejected
            proto.getCode() return the cause of the reject;
            QList<QString>* params = proto.getParams(); is the pointer to the parameter list
        break;
        case commandProtocol::_ACK_FRAME:
            The command delayed now is completed.
            proto.getCode() return the result  of the command execution;
            QList<QString>* params = proto.getParams(); is the pointer to the parameter list
        break;
        case commandProtocol::_NA_FRAME:
            The command is not implemented or not supported
        break;
        case commandProtocol::_INVALID_FRAME:
            The received frame is of wrong format
        break;
    }

   \endverbatim
 *
 * ## SERVER IMPLEMENTATION
 *
 * + Server shall handle a reception stream of QByteArray:
   \verbatim
   QByteArray rxframe = is the received command frame;
   Here, as an example, the encoding method used is Utf8;

   commandProtocol proto  = commandProtocol(&rxframe,QStringConverter::Encoding::Utf8);
   switch(proto.getDecodedType()){
        case commandProtocol::_CMD_FRAME:
            proto.getCmd(): is the received command
            QList<QString>* params = proto.getParams(); is the pointer to the parameter list
        break;
   }
   \endverbatim
 *
 * + Based on the command implementation the Server answer:
 *  + proto.formatOkAnswer(0, ..): for an immediate OK. See commandProtocol::formatOkAnswer().
 *  + proto.formatOkAnswer(Time, ..): for a delay command execution. See commandProtocol::formatOkAnswer().
 *      + When the command completes: commandProtocol::formatDelayCompletedCommand();
 *  + proto.formatNokAnswer(CODE,..): for an immediate NOK with reason. See commandProtocol::formatNokAnswer().
 *  + proto.formatNaAnswer(): for NA. See commandProtocol::formatNaAnswer().
 *
 * + Server shall send Status Commands to the Client using the commandProtocol::formatStatusCommand();
 *
 */
class commandProtocol
{
public:
    commandProtocol(QByteArray* dt, QStringConverter::Encoding format );
    commandProtocol(void);

    //! Definition of the received frame formats
    typedef enum{
        _CMD_FRAME = 0, //!< The decode frame is a command frame
        _OK_FRAME,      //!< The decode frame is an immediate OK frame
        _NOK_FRAME,     //!< The decode frame is an immediate NOK frame
        _DELAYED_FRAME, //!< The decode frame is a delayed frame
        _ACK_FRAME,     //!< The decode frame is a delayed completed frame
        _STATUS_FRAME,  //!< The decode frame is a status frame
        _NA_FRAME,      //!< The decode frame is a not implemented command frame
        _INVALID_FRAME  //!< Undetected format
    }_FrameType;

    /**
     * @brief This static method shall be used to format a command frame.
     *
     *
     * @param format
     * QStringConverter::Encoding type possible encoding formats
     * @param id
     * integer value defining the ID code of the command. It can be either an incrementing number or a unique
     * number.
     * @param cmd
     * String formatted command
     * @param params
     * QList<QString>* pointer to a list of strings representing Variant parameter type
     * @return
     * QByteArray formatted data byte stream.
     */
    static QByteArray formatCommand(QStringConverter::Encoding format,int id, QString cmd, QList<QString>* params = nullptr);




    /**
     * @brief This static method shall be used to format an Acknowledge command completion answer frame.
     *
     *
     * @param format
     * QStringConverter::Encoding type possible encoding formats
     * @param id
     * integer value defining the ID code of the processed command.
     * @param code
     * integer resulting code
     * @param params
     * Optional QList<QString>* pointer to a list of strings representing Variant parameter type
     * @return
     * QByteArray formatted data byte stream.
     */
    static QByteArray formatDelayCompletedCommand(QStringConverter::Encoding format, int id, int code, QList<QString>* params = nullptr);

    /**
     * @brief This static method shall be used to format a status command  frame.
     *
     * @param format
     * QStringConverter::Encoding type possible encoding formats
     * @param cmd
     * String formatted command
     * @param params
     * QList<QString>* pointer to a list of strings representing Variant parameter type
     * @return
     * QByteArray formatted data byte stream.
     */
    static QByteArray formatStatusCommand(QStringConverter::Encoding format, QString cmd, QList<QString>* params = nullptr);

    /**
     * @brief This is the Immediate or Delayed  OK answer method
     *
     * @param code
     * + code == 0: the command has been successfully executed;
     * + code == T: the command has been started and it takes approximatelly T time to completes.
     * The T interpretation is up the implementation.
     * @param params
     * ptional QList<QString>* pointer to a list of strings representing Variant parameter type
     * @return
     * QByteArray formatted data byte stream.
     */
    _inline QByteArray  formatOkAnswer(int code, QList<QString>* params = nullptr){ return formatCommand(format, id, "OK " + QString("%1").arg(code), params); }

    /**
     * @brief This is the Immediate NOK answer method
     *
     * @param code
     * Reason of the failure
     * @return
     * QByteArray formatted data byte stream.
     */
    _inline QByteArray  formatNokAnswer(int code, QString error){
        return formatCommand(format, id, QString("NOK %1 ").arg(code) + "\"" + error + "\"");
    }

    //! Not implemented command answer
    _inline QByteArray  formatNaAnswer(QString error = ""){ return formatCommand(format, id, "NA " + error); }

    //! Returns the decoded frame type of the commandProtocol::_FrameType
    _inline _FrameType      getDecodedType(void) {return frameType;}

    //! Returns the decoded command
    _inline QString         getCmd(void) {return cmd;}

    //! Returns the decoded code (OK, NOK, ACK frames)
    _inline int             getCode(void) {return code;}

    //! Returns the decoded ID (Commands or ACK frames)
    _inline int             getId(void) {return id;}

    //! Returns the decoded pointer to the params list
    _inline QList<QString>* getParams(void){return &params;}


private:
    QStringConverter::Encoding format;  //!< Encoding data format
    QList<QString> params;              //!< decoded parmeters
    QString cmd;                        //!< decoded command
    int code;                           //! integer code for OK/NOK and ACK format
    int id;                             //!< ID of the current protocol message or ID of the ack frame
    _FrameType frameType;               //!< type of decoded frame

};

#endif // COMMANDPROTOCOL_H
