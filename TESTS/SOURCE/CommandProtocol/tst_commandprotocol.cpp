#include <QtTest>
#include <QCoreApplication>

// add necessary includes here
#include "commandprotocol.h"

/**
 *  \defgroup COMMAND_PROTOCOL_CLASS_MODULE commandProtocol class Unit Test module
 *
 *  This module describes the tests on the commandProtocol class.
 *
 *
 *  # TEST DESCRIPTION
 *
 *  This Test section verifies the class implementation with the following test list:
 *
 *  + test_commandProtocol::initTestCase();
 *  + test_commandProtocol::test_commandProtocol_1();
 *  + test_commandProtocol::test_commandProtocol_2();
 *  + test_commandProtocol::test_commandProtocol_3();
 *  + test_commandProtocol::test_commandProtocol_4();
 *  + test_commandProtocol::test_commandProtocol_5();
 *  + test_commandProtocol::test_commandProtocol_6();
 *  + test_commandProtocol::test_commandProtocol_7();
 *  + test_commandProtocol::test_commandProtocol_8();
 *  + test_commandProtocol::test_commandProtocol_9();
 *  + test_commandProtocol::test_commandProtocol_10();
 *  + test_commandProtocol::test_commandProtocol_11();
 *  + test_commandProtocol::test_commandProtocol_12();
 *  + test_commandProtocol::test_commandProtocol_13();
 *  + test_commandProtocol::test_commandProtocol_14();
 *  + test_commandProtocol::test_commandProtocol_15();
 *  + test_commandProtocol::cleanupTestCase();
 *
 *
 *  ## INVALID-FRAME type identification test
 *  **Function**: test_commandProtocol::test_commandProtocol_1() \n
 *  **Description**: the test verifies the ability to detect a non valid frame.
 *
 *  **Execution**:
 *  -# Creates an invalid  frame with an invalid frame lenght field;
 *   + Verify that the detected frame is invalid;
 *  -# Creates an invalid  frame missing the frame initiator character;
 *   + Verify that the detected frame is invalid;
 *  -# Creates an invalid  frame with a wrong '\%' character usage;
 *   + Verify that the detected frame is invalid;
 *
 *  ## COMMAND-FRAME type identification test
 *  **Function**: test_commandProtocol::test_commandProtocol_2(). \n
 *  **Description**: The test verifies the ability to detect a valid COMMAND-FRAME.
 *
 *  **Execution**:
 *  -# Creates a valid Command Frame with a list of three parameters;
 *   + Verify that the detected frame is a COMMAND frame;
 *   + Verify the "Command" field is correct;
 *   + Verify the "Parameters" fields are correct
 *
 *  ## COMMAND-FRAME type and parameter identification test
 *  **Function**: test_commandProtocol::test_commandProtocol_3(). \n
 *  **Description**: The test verifies the ability to detect a valid COMMAND-FRAME and its parameters format.
 *
 *  **Execution**:
 *  -# Creates a valid Command Frame with two parametrs: a  \"string\" parameter and a normal prameter;
 *   + Verify that the detected frame is a COMMAND frame;
 *   + Verify the "Command" field is correct;
 *   + Verify the ID field is correct;
 *   + Verify the number of parameter is correct;
 *   + Verify the "Parameters" fields are correct;
 *
 *  ## OK-FRAME type and parameter identification test
 *  **Function**: test_commandProtocol::test_commandProtocol_4(). \n
 *  **Description**: The test veifies the ability to detect a valid OK-FRAME and its parameters.
 *
 *  **Execution**:
 *  -# Creates a valid OK Command Frame type ;
 *   + Verify that the detected frame is a OK frame;
 *   + Verify the ID field is correct;
 *   + Verify the CODE field is correct;
 *   + Verify the number of parameter is correct;
 *   + Verify the "Command" fields are correct;
 *   + Verify the "Parameters" fields are correct;
 *
 *  ## DELAYED-FRAME type and parameter identification test
 *  **Function**: test_commandProtocol::test_commandProtocol_5(). \n
 *  **Description**: The test veifies the ability to detect a valid DELAYED-FRAME and its parameters.
 *
 *  **Execution**:
 *  -# Creates a valid DELAYED  Frame type ;
 *   + Verify that the detected frame is a DELAYED frame;
 *   + Verify the ID field is correct;
 *   + Verify the CODE field is correct;
 *   + Verify the number of parameters is correct;
 *   + Verify the "Command" fields are correct;
 *   + Verify the "Parameters" fields are correct;
 *
 *  ## NOK-FRAME type and parameter identification test
 *  **Function**: test_commandProtocol::test_commandProtocol_6(). \n
 *  **Description**: The test verifies the ability to detect a valid NOK-FRAME and its parameters.
 *
 *  **Execution**:
 *  -# Creates a valid NOK-FRAME  Frame type ;
 *   + Verify that the detected frame is a NOK-FRAME frame;
 *   + Verify the ID field is correct;
 *   + Verify the CODE field is correct;
 *   + Verify the number of parameters is correct;
 *   + Verify the "Command" fields are correct;
 *   + Verify the "Parameters" fields are correct;
 *
 *  ## ACK-FRAME type and parameter identification test
 *  **Function**: test_commandProtocol::test_commandProtocol_7(). \n
 *  **Description**: The test verifies the ability to detect a valid ACK-FRAME and its parameters.
 *
 *  **Execution**:
 *  -# Creates a valid ACK-FRAME  Frame type ;
 *   + Verify that the detected frame is a ACK-FRAME frame;
 *   + Verify the ID field is correct;
 *   + Verify the CODE field is correct;
 *   + Verify the number of parameters is correct;
 *   + Verify the "Command" fields are correct;
 *   + Verify the "Parameters" fields are correct;
 *
 *  ## STATUS-FRAME type and parameter identification test
 *  **Function**: test_commandProtocol::test_commandProtocol_8(). \n
 *  **Description**: The test verifies the ability to detect a valid STATUS-FRAME and its parameters.
 *
 *  **Execution**:
 *  -# Creates a valid STATUS-FRAME  Frame type ;
 *   + Verify that the detected frame is a STATUS-FRAME frame;
 *   + Verify the ID field is correct;
 *   + Verify the CODE field is correct;
 *   + Verify the number of parameters is correct;
 *   + Verify the "Command" fields are correct;
 *   + Verify the "Parameters" fields are correct;
 *
 *  ## COMMAND-TYPE frame format creation test
 *  **Function**: test_commandProtocol::test_commandProtocol_9(). \n
 *  **Description**: The test verifies the ability to format a frame of COMMAND-TYPE.
 *
 *  **Execution**:
 *  -# Creates a Command frame with a list of parameters;
 *  -# Decode the frame with the commandProtocol class constructor;
 *   + Verify that the detected frame is a COMMAND-FRAME ;
 *   + Verify the ID field is correct;
 *   + Verify the number of parameters is correct;
 *   + Verify the "Command" fields are correct;
 *   + Verify the "Parameters" fields are correct;
 *
 *  ## DELAY-COMPLETED frame format creation test
 *  **Function**: test_commandProtocol::test_commandProtocol_10(). \n
 *  **Description**: The test verifies the ability to format a frame of DELAY-COMPLETED.
 *
 *  **Execution**:
 *  -# Creates a DELAY-COMPLETED frame with a list of parameters;
 *  -# Decode the frame with the commandProtocol class constructor;
 *   + Verify that the detected frame is a DELAY-COMPLETED ;
 *   + Verify the ID field is correct;
 *   + Verify the CODE field is correct;
 *   + Verify the number of parameters is correct;
 *   + Verify the "Command" fields are correct;
 *   + Verify the "Parameters" fields are correct;
 *
 *  ## NA frame format creation test
 *  **Function**: test_commandProtocol::test_commandProtocol_11(). \n
 *  **Description**: The test verifies the ability to format a frame of NA.
 *
 *  **Execution**:
 *  -# Creates a NA frame ;
 *  -# Decode the frame with the commandProtocol class constructor;
 *   + Verify that the detected frame is a NA ;
 *
 *  ## NOK frame format creation test
 *  **Function**: test_commandProtocol::test_commandProtocol_12(). \n
 *  **Description**: The test verifies the ability to format a frame of NOK.
 *
 *  **Execution**:
 *  -# Creates a Command frame with a list of parameters;
 *  -# Decode the frame with the commandProtocol class constructor;
 *  -# Creates a NOK frame with a code from the decoded protocol;
 *  -# Decode the NOK frame with the commandProtocol class constructor;
 *   + Verify that the detected frame is a NOK ;
 *   + Verify the CODE field is correct;
 *
 *  ## OK frame format creation test
 *  **Function**: test_commandProtocol::test_commandProtocol_13(). \n
 *  **Description**: The test verifies the ability to format a frame of OK.
 *
 *  **Execution**:
 *  -# Creates a Command frame with a list of parameters;
 *  -# Decode the frame with the commandProtocol class constructor;
 *  -# Creates a OK answer frame with a code from the decoded protocol;
 *  -# Decode the OK frame with the commandProtocol class constructor;
 *   + Verify that the detected frame is a OK ;
 *   + Verify the ID field is correct;
 *   + Verify the CODE field is correct;
 *   + Verify the number of parameters is correct;
 *   + Verify the "Command" fields are correct;
 *   + Verify the "Parameters" fields are correct;
 *
 *  ## DELAYED frame format creation test
 *  **Function**: test_commandProtocol::test_commandProtocol_14(). \n
 *  **Description**: The test verifies the ability to format a frame of DELAYED.
 *
 *  **Execution**:
 *  -# Creates a Command frame with a list of parameters;
 *  -# Decode the frame with the commandProtocol class constructor;
 *  -# Creates a DELAYED (<OK code>) answer frame with a code from the decoded protocol;
 *  -# Decode the DELAYED frame with the commandProtocol class constructor;
 *   + Verify that the detected frame is a DELAYED ;
 *   + Verify the ID field is correct;
 *   + Verify the CODE field is correct;
 *   + Verify the number of parameters is correct;
 *   + Verify the "Command" fields are correct;
 *   + Verify the "Parameters" fields are correct;
 *
 *  ## STATUS frame format creation test
 *  **Function**: test_commandProtocol::test_commandProtocol_15(). \n
 *  **Description**: The test verifies the ability to format a frame of STATUS.
 *
 *  **Execution**:
 *  -# Creates a STATUS frame with a list of parameters;
 *  -# Decode the STATUS frame with the commandProtocol class constructor;
 *   + Verify that the detected frame is a STATUS ;
 *   + Verify the ID field is correct;
 *   + Verify the CODE field is correct;
 *   + Verify the number of parameters is correct;
 *   + Verify the "Command" fields are correct;
 *   + Verify the "Parameters" fields are correct;
 *
 */

/**
 * @brief Executing class
 * \see COMMAND_PROTOCOL_CLASS_MODULE
 */

class test_commandProtocol : public QObject
{
    Q_OBJECT

public:
    test_commandProtocol();
    ~test_commandProtocol();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void test_commandProtocol_1();

    void test_commandProtocol_2();
    void test_commandProtocol_2_1();

    void test_commandProtocol_3();
    void test_commandProtocol_4();
    void test_commandProtocol_5();
    void test_commandProtocol_6();
    void test_commandProtocol_7();
    void test_commandProtocol_8();
    void test_commandProtocol_9();
    void test_commandProtocol_10();
    void test_commandProtocol_11();
    void test_commandProtocol_12();
    void test_commandProtocol_13();
    void test_commandProtocol_14();
    void test_commandProtocol_15();

};



test_commandProtocol::test_commandProtocol()
{

}

test_commandProtocol::~test_commandProtocol()
{

}

void test_commandProtocol::initTestCase()
{


}

void test_commandProtocol::cleanupTestCase()
{

}

/**
*
* @brief INVALID-FRAME type identification test.
*
* The test verifies the ability to detect a non valid frame.
*/
void test_commandProtocol::test_commandProtocol_1()
{
    QByteArray frame = "<010 0036 % Comando Par1 Par2 Par3 %>";
    commandProtocol command(&frame, QStringConverter::Encoding::Utf8);    
    QVERIFY(command.getDecodedType() == commandProtocol::_INVALID_FRAME);

    frame = "010 0037 % Comando Par1 Par2 Par3 %>";
    command = commandProtocol(&frame, QStringConverter::Encoding::Utf8);
    QVERIFY(command.getDecodedType() == commandProtocol::_INVALID_FRAME);

    frame = "<010 0037 % %Comando Par1 Par2 Par3 %>";
    command = commandProtocol(&frame, QStringConverter::Encoding::Utf8);
    QVERIFY(command.getDecodedType() == commandProtocol::_INVALID_FRAME);
}


/**
*
* @brief COMMAND-FRAME type identification test.
*
* The test verifies the ability to detect a valid COMMAND-FRAME
*/
void test_commandProtocol::test_commandProtocol_2()
{
    QByteArray frame = "<010 0037 % Comando Par1 Par2 Par3 %>";
    commandProtocol command(&frame, QStringConverter::Encoding::Utf8);
    QVERIFY(command.getDecodedType() == commandProtocol::_CMD_FRAME);

    QVERIFY(command.getCmd() == QString("Comando"));
    QCOMPARE(command.getId(),10);
    QCOMPARE(command.getParams()->count(),3);
    QVERIFY((*command.getParams())[0] == QString("Par1"));
    QVERIFY((*command.getParams())[1] == QString("Par2"));
    QVERIFY((*command.getParams())[2] == QString("Par3"));

}

void test_commandProtocol::test_commandProtocol_2_1()
{
    QString stringa = "<010 0037 % Comando Par1 Par2 Par3 %>";
    auto fromUtf = QStringEncoder(QStringConverter::Encoding::Utf16);
    QByteArray frame = fromUtf(stringa);

    commandProtocol command(&frame, QStringConverter::Encoding::Utf16);
    QVERIFY(command.getDecodedType() == commandProtocol::_CMD_FRAME);

    QVERIFY(command.getCmd() == QString("Comando"));
    QCOMPARE(command.getId(),10);
    QCOMPARE(command.getParams()->count(),3);
    QVERIFY((*command.getParams())[0] == QString("Par1"));
    QVERIFY((*command.getParams())[1] == QString("Par2"));
    QVERIFY((*command.getParams())[2] == QString("Par3"));

}

/**
*
* @brief COMMAND-FRAME type and parameter identification test.
*
* The test veifies the ability to detect a valid COMMAND-FRAME
* and its parameters.
*/
void test_commandProtocol::test_commandProtocol_3()
{
    QByteArray frame = "<010 0039 % Comando \"Par1 Par2\" Par3 %>";
    commandProtocol command(&frame, QStringConverter::Encoding::Utf8);
    QVERIFY(command.getDecodedType() == commandProtocol::_CMD_FRAME);

    QVERIFY(command.getCmd() == QString("Comando"));
    QCOMPARE(command.getId(),10);
    QCOMPARE(command.getParams()->count(),2);
    QVERIFY((*command.getParams())[0] == QString("Par1 Par2"));
    QVERIFY((*command.getParams())[1] == QString("Par3"));
}

/**
*
* @brief OK-FRAME type and parameter identification test.
*
* The test veifies the ability to detect a valid OK-FRAME
* and its parameters.
*/
void test_commandProtocol::test_commandProtocol_4()
{
    QByteArray frame = "<010 0029 % OK 0 Par2 Par3 %>";
    commandProtocol command(&frame, QStringConverter::Encoding::Utf8);
    QCOMPARE(command.getDecodedType(), commandProtocol::_OK_FRAME);
    QCOMPARE(command.getId(),10);
    QCOMPARE(command.getCode(),0);
    QCOMPARE(command.getParams()->count(),3);
    QVERIFY(command.getCmd() == QString("OK"));
    QVERIFY((*command.getParams())[0] == QString("0"));
    QVERIFY((*command.getParams())[1] == QString("Par2"));
    QVERIFY((*command.getParams())[2] == QString("Par3"));
}

/**
*
* @brief DELAYED-FRAME type and parameter identification test.
*
* The test veifies the ability to detect a valid DELAYED-FRAME
* and its parameters.
*/
void test_commandProtocol::test_commandProtocol_5()
{
    QByteArray frame = "<010 0029 % OK 5 Par2 Par3 %>";
    commandProtocol command(&frame, QStringConverter::Encoding::Utf8);
    QCOMPARE(command.getDecodedType(), commandProtocol::_DELAYED_FRAME);
    QCOMPARE(command.getId(),10);
    QCOMPARE(command.getCode(),5);
    QCOMPARE(command.getParams()->count(),3);
    QVERIFY(command.getCmd() == QString("OK"));
    QVERIFY((*command.getParams())[0] == QString("5"));
    QVERIFY((*command.getParams())[1] == QString("Par2"));
    QVERIFY((*command.getParams())[2] == QString("Par3"));
}

/**
*
* @brief NOK-FRAME type and parameter identification test.
*
* The test veifies the ability to detect a valid NOK-FRAME
* and its parameters.
*/
void test_commandProtocol::test_commandProtocol_6()
{
    QByteArray frame = "<010 0031 % NOK 40 Par2 Par3 %>";
    commandProtocol command(&frame, QStringConverter::Encoding::Utf8);
    QCOMPARE(command.getDecodedType(), commandProtocol::_NOK_FRAME);
    QCOMPARE(command.getId(),10);
    QCOMPARE(command.getCode(),40);
    QCOMPARE(command.getParams()->count(),3);
    QVERIFY(command.getCmd() == QString("NOK"));
    QVERIFY((*command.getParams())[0] == QString("40"));
    QVERIFY((*command.getParams())[1] == QString("Par2"));
    QVERIFY((*command.getParams())[2] == QString("Par3"));
}

/**
*
* @brief ACK-FRAME type and parameter identification test.
*
* The test veifies the ability to detect a valid ACK-FRAME
* and its parameters.
*/
void test_commandProtocol::test_commandProtocol_7()
{
    QByteArray frame = "<A 0027 % 10 1 Par2 Par3 %>";
    commandProtocol command(&frame, QStringConverter::Encoding::Utf8);
    QCOMPARE(command.getDecodedType(), commandProtocol::_ACK_FRAME);
    QCOMPARE(command.getId(),10);
    QCOMPARE(command.getCode(),1);
    QCOMPARE(command.getParams()->count(),3);
    QVERIFY(command.getCmd() == QString("10"));
    QVERIFY((*command.getParams())[0] == QString("1"));
    QVERIFY((*command.getParams())[1] == QString("Par2"));
    QVERIFY((*command.getParams())[2] == QString("Par3"));
}

/**
*
* @brief STATUS-FRAME type and parameter identification test.
*
* The test veifies the ability to detect a valid STATUS-FRAME
* and its parameters.
*/
void test_commandProtocol::test_commandProtocol_8()
{
    QByteArray frame = "<S 0035 % Comando Par1 Par2 Par3 %>";
    commandProtocol command(&frame, QStringConverter::Encoding::Utf8);
    QCOMPARE(command.getDecodedType(), commandProtocol::_STATUS_FRAME);
    QCOMPARE(command.getId(),0);
    QCOMPARE(command.getCode(),0);
    QCOMPARE(command.getParams()->count(),3);
    QVERIFY(command.getCmd() == QString("Comando"));
    QVERIFY((*command.getParams())[0] == QString("Par1"));
    QVERIFY((*command.getParams())[1] == QString("Par2"));
    QVERIFY((*command.getParams())[2] == QString("Par3"));
}

/**
*
* @brief COMMAND-TYPE frame format creation.
*
* The test verifies the ability to format a frame of COMMAND-TYPE.
*/
void test_commandProtocol::test_commandProtocol_9()
{
    QList<QString> params;
    params.append("Par1");
    params.append("Par2");
    params.append("Par3");

    QByteArray frame = commandProtocol::formatCommand(QStringConverter::Encoding::Utf8, 10, QString("Comando"), &params);

    commandProtocol command(&frame, QStringConverter::Encoding::Utf8);
    QCOMPARE(command.getDecodedType(), commandProtocol::_CMD_FRAME);
    QCOMPARE(command.getId(),10);
    QCOMPARE(command.getParams()->count(),3);
    QCOMPARE(command.getCmd() , QString("Comando"));
    QCOMPARE((*command.getParams())[0] , QString("Par1"));
    QCOMPARE((*command.getParams())[1] , QString("Par2"));
    QCOMPARE((*command.getParams())[2] , QString("Par3"));
}

/**
*
* @brief DELAY-COMPLETED frame format creation.
*
* The test veifies the ability to format a frame of DELAY-COMPLETED type.
*/
void test_commandProtocol::test_commandProtocol_10()
{
    QList<QString> params;
    params.append("Par1");
    params.append("Par2");
    params.append("Par3");

    QByteArray frame = commandProtocol::formatDelayCompletedCommand(QStringConverter::Encoding::Utf8, 10, 1, &params);
    commandProtocol command(&frame, QStringConverter::Encoding::Utf8);
    QCOMPARE(command.getDecodedType(), commandProtocol::_ACK_FRAME);
    QCOMPARE(command.getId(),10);
    QCOMPARE(command.getCode(),1);
    QCOMPARE(command.getParams()->count(),4);
    QVERIFY(command.getCmd() == QString("10"));
    QVERIFY((*command.getParams())[0] == QString("1"));
    QVERIFY((*command.getParams())[1] == QString("Par1"));
    QVERIFY((*command.getParams())[2] == QString("Par2"));
    QVERIFY((*command.getParams())[3] == QString("Par3"));
}

/**
*
* @brief "NA" frame format creation.
*
* The test veifies the ability to format a frame of "NA" type.
*/
void test_commandProtocol::test_commandProtocol_11()
{

    QByteArray frame = "<010 0037 % Comando Par1 Par2 Par3 %>";
    commandProtocol rxcom(&frame, QStringConverter::Encoding::Utf8);
    frame = rxcom.formatNaAnswer();

    commandProtocol command(&frame, QStringConverter::Encoding::Utf8);
    QCOMPARE(command.getDecodedType(), commandProtocol::_NA_FRAME);

}

/**
*
* @brief "NOK" frame format creation.
*
* The test veifies the ability to format a frame of "NOK" type.
*/
void test_commandProtocol::test_commandProtocol_12()
{

    QByteArray frame = "<010 0037 % Comando Par1 Par2 Par3 %>";
    commandProtocol rxcom(&frame, QStringConverter::Encoding::Utf8);
    frame = rxcom.formatNokAnswer(2, "");

    commandProtocol command(&frame, QStringConverter::Encoding::Utf8);
    QCOMPARE(command.getDecodedType(), commandProtocol::_NOK_FRAME);
    QCOMPARE(command.getCode(),2);
}


/**
*
* @brief "OK" frame format creation.
*
* The test veifies the ability to format a frame of "OK" type.
*/
void test_commandProtocol::test_commandProtocol_13()
{
    QByteArray frame = "<010 0037 % Comando Par1 Par2 Par3 %>";
    commandProtocol rxcom(&frame, QStringConverter::Encoding::Utf8);

    QList<QString> params;
    params.append(QString("%1").arg((int) 0));
    frame = rxcom.formatOk(&params);

    commandProtocol command(&frame, QStringConverter::Encoding::Utf8);
    QCOMPARE(command.getDecodedType(), commandProtocol::_OK_FRAME);
    QCOMPARE(command.getId(),10);
    QCOMPARE(command.getCode(),0);
    QCOMPARE(command.getParams()->count(),1);
    QVERIFY(command.getCmd() == QString("OK"));
    QVERIFY((*command.getParams())[0] == QString("0"));


}

/**
*
* @brief "DELAYED" frame format creation.
*
* The test veifies the ability to format a frame of "DELAYED" type.
*/
void test_commandProtocol::test_commandProtocol_14()
{
    QByteArray frame = "<010 0037 % Comando Par1 Par2 Par3 %>";
    commandProtocol rxcom(&frame, QStringConverter::Encoding::Utf8);

    QList<QString> params;
    params.append(QString("%1").arg((int) 15));
    frame = rxcom.formatOk(&params);

    commandProtocol command(&frame, QStringConverter::Encoding::Utf8);
    QCOMPARE(command.getDecodedType(), commandProtocol::_DELAYED_FRAME);
    QCOMPARE(command.getId(),10);
    QCOMPARE(command.getCode(),15);
    QCOMPARE(command.getParams()->count(),1);
    QVERIFY(command.getCmd() == QString("OK"));
    QVERIFY((*command.getParams())[0] == QString("15"));

}


/**
*
* @brief "STATUS" frame format creation.
*
* The test veifies the ability to format a frame of "STATUS" type.
*/
void test_commandProtocol::test_commandProtocol_15()
{

    QList<QString> params;
    params.append("Par1");
    params.append("Par2");
    params.append("Par3");

    QByteArray frame = commandProtocol::formatStatusCommand(QStringConverter::Encoding::Utf8, QString("Comando"), &params);
    commandProtocol command(&frame, QStringConverter::Encoding::Utf8);
    QCOMPARE(command.getDecodedType(), commandProtocol::_STATUS_FRAME);
    QCOMPARE(command.getId(),0);
    QCOMPARE(command.getCode(),0);
    QCOMPARE(command.getParams()->count(),3);
    QVERIFY(command.getCmd() == QString("Comando"));
    QVERIFY((*command.getParams())[0] == QString("Par1"));
    QVERIFY((*command.getParams())[1] == QString("Par2"));
    QVERIFY((*command.getParams())[2] == QString("Par3"));

}

QTEST_MAIN(test_commandProtocol)

#include "tst_commandprotocol.moc"
