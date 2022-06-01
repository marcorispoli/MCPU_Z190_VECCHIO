#include <QtTest>
#include <QCoreApplication>

// add necessary includes here
#include "awsinterface.h"
#include "tcpipclient.h"
#include "commandprotocol.h"
/**
 *  \defgroup AWS_INTERFACE_CLASS_MODULE awsInterface class Unit Test module
 *
 *  This module describes the tests on the awsInterface class.
 *
 *
 *  # TEST DESCRIPTION
 *
 *  This Test section verifies the class implementation with the following test list:
 *
 *  + test_commandProtocol::initTestCase();
 *  + test_commandProtocol::test_awsInterface_1();
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
 *
 */

/**
 * @brief Executing class
 * \see AWS_INTERFACE_CLASS_MODULE
 */

class test_awsInterface : public QObject
{
    Q_OBJECT


public:

    test_awsInterface();
    ~test_awsInterface();

    class awsTestClass: public awsInterface
    {
        public:
        awsTestClass(QString address, int port) : awsInterface(address,port){

        }

    public:
        void _inline Ok(QList<QString>* params = nullptr) {sendOk(params);}
        void _inline DelayedOk(int time, QList<QString>* params = nullptr) {sendOk(time, params);}
        void _inline Nok(int code, QString error = "") {sendNok(code,error);}
        void _inline Status(QString status, QList<QString>* params = nullptr){sendStatus(status, params);}

    };



private slots:
    void initTestCase();
    void cleanupTestCase();
    void test_awsInterface_EXEC_AbortProjections();
    void test_awsInterface_EXEC_ArmPosition();
    void test_awsInterface_EXEC_Close();
    void test_awsInterface_EXEC_OpenStudy();
    void test_awsInterface_EXEC_PowerOff();
    void test_awsInterface_EXEC_ResetError();
    void test_awsInterface_EXEC_StartXraySequence();
    void test_awsInterface_EXEC_TrxPosition();
    void test_awsInterface_GET_Accessories();
    void test_awsInterface_GET_CompressorData();
    void test_awsInterface_GET_ExposureCompletionData();
    void test_awsInterface_GET_ReadyForExposure();
    void test_awsInterface_GET_RotationAngles();
    void test_awsInterface_GET_TomoConfig();
    void test_awsInterface_GET_TomoConfigFile();
    void test_awsInterface_GET_TubeTemperature();
    void test_awsInterface_GET_XrayPushStatus();
    void test_awsInterface_SET_CompressionRelease();
    void test_awsInterface_SET_Language();
    void test_awsInterface_SET_ProjectionList();
    void test_awsInterface_SET_PulseData();
    void test_awsInterface_SET_XrayPushEnable();
    void test_awsInterface_GANTRY_AbortProjection();


    void test_awsInterface_GANTRY_ERROR();
    void test_awsInterface_GANTRY_PulseCompleted();
    void test_awsInterface_GANTRY_SelectProjection();
    void test_awsInterface_GANTRY_SetAccessories();
    void test_awsInterface_GANTRY_SetTubeTemperature();
    void test_awsInterface_GANTRY_XrayPushEvent();
    void test_awsInterface_GANTRY_XraySequenceCompleted();

private:
    awsTestClass* pInterface;
    TcpIpClient* pClient;
    QSignalSpy* spy;
    QList<QVariant> arguments;
};



test_awsInterface::test_awsInterface()
{

}

test_awsInterface::~test_awsInterface()
{

}

void test_awsInterface::initTestCase()
{
    pInterface = new awsTestClass("127.0.0.1",10001);
    pClient = new TcpIpClient();

    spy = new QSignalSpy(pClient, SIGNAL(clientConnection(bool)));
    pClient->Start("127.0.0.1",10001);
    while (spy->count() == 0)  QTest::qWait(200);
    QCOMPARE(spy->count(), 1);

    QList<QVariant> arguments = spy->takeFirst();
    QVERIFY(arguments.at(0).toBool() == true);
    delete spy;
}

void test_awsInterface::cleanupTestCase()
{
    pInterface->deleteLater();
}


void test_awsInterface::test_awsInterface_EXEC_AbortProjections()
{
    QByteArray data;
    QString stringa;
    auto fromUtf = QStringEncoder(QStringConverter::Encoding::Utf16LE);

    stringa = "<01 0036 % EXEC_AbortProjections % >";
    spy = new QSignalSpy(pClient, SIGNAL(rxData(QByteArray)));
    pClient->txData(fromUtf(stringa));
    while (spy->count() == 0)  QTest::qWait(10);
    QCOMPARE(spy->count(), 1);

    arguments = spy->takeFirst();
    data = arguments.at(0).toByteArray();
    commandProtocol proto(&data, QStringConverter::Encoding::Utf16LE);
    QCOMPARE(proto.getDecodedType() , commandProtocol::_NA_FRAME);
    QCOMPARE(proto.getParams()->count() , 1);
    QCOMPARE((proto.getParams())->at(0) , "EXEC_AbortProjections");
    delete spy;

}

void test_awsInterface::test_awsInterface_EXEC_ArmPosition()
{
    QByteArray data;
    QString stringa;
    auto fromUtf = QStringEncoder(QStringConverter::Encoding::Utf16LE);

  //stringa = "<01 0036 % EXEC_AbortProjections % >";
    stringa = "<01 0036 % EXEC_ArmPosition      % >";
    spy = new QSignalSpy(pClient, SIGNAL(rxData(QByteArray)));
    pClient->txData(fromUtf(stringa));
    while (spy->count() == 0)  QTest::qWait(10);
    QCOMPARE(spy->count(), 1);

    arguments = spy->takeFirst();
    data = arguments.at(0).toByteArray();
    commandProtocol proto(&data, QStringConverter::Encoding::Utf16LE);
    QCOMPARE(proto.getDecodedType() , commandProtocol::_NA_FRAME);
    QCOMPARE(proto.getParams()->count() , 1);
    QCOMPARE((proto.getParams())->at(0) , "EXEC_ArmPosition");
    delete spy;
}

void test_awsInterface::test_awsInterface_EXEC_Close()
{
    QByteArray data;
    QString stringa;
    auto fromUtf = QStringEncoder(QStringConverter::Encoding::Utf16LE);

  //stringa = "<01 0036 % EXEC_AbortProjections % >";
    stringa = "<01 0036 % EXEC_Close            % >";
    spy = new QSignalSpy(pClient, SIGNAL(rxData(QByteArray)));
    pClient->txData(fromUtf(stringa));
    while (spy->count() == 0)  QTest::qWait(10);
    QCOMPARE(spy->count(), 1);

    arguments = spy->takeFirst();
    data = arguments.at(0).toByteArray();
    commandProtocol proto(&data, QStringConverter::Encoding::Utf16LE);
    QCOMPARE(proto.getDecodedType() , commandProtocol::_NA_FRAME);
    QCOMPARE(proto.getParams()->count() , 1);
    QCOMPARE((proto.getParams())->at(0) , "EXEC_Close");
    delete spy;
}

void test_awsInterface::test_awsInterface_EXEC_OpenStudy()
{
    QByteArray data;
    QString stringa;
    auto fromUtf = QStringEncoder(QStringConverter::Encoding::Utf16LE);

  //stringa = "<01 0036 % EXEC_AbortProjections % >";
    stringa = "<01 0036 % EXEC_OpenStudy        % >";
    spy = new QSignalSpy(pClient, SIGNAL(rxData(QByteArray)));
    pClient->txData(fromUtf(stringa));
    while (spy->count() == 0)  QTest::qWait(10);
    QCOMPARE(spy->count(), 1);

    arguments = spy->takeFirst();
    data = arguments.at(0).toByteArray();
    commandProtocol proto(&data, QStringConverter::Encoding::Utf16LE);
    QCOMPARE(proto.getDecodedType() , commandProtocol::_NA_FRAME);
    QCOMPARE(proto.getParams()->count() , 1);
    QCOMPARE((proto.getParams())->at(0) , "EXEC_OpenStudy");
    delete spy;
}

void test_awsInterface::test_awsInterface_EXEC_PowerOff()
{
    QByteArray data;
    QString stringa;
    auto fromUtf = QStringEncoder(QStringConverter::Encoding::Utf16LE);

  //stringa = "<01 0036 % EXEC_AbortProjections % >";
    stringa = "<01 0036 % EXEC_PowerOff         % >";
    spy = new QSignalSpy(pClient, SIGNAL(rxData(QByteArray)));
    pClient->txData(fromUtf(stringa));
    while (spy->count() == 0)  QTest::qWait(10);
    QCOMPARE(spy->count(), 1);

    arguments = spy->takeFirst();
    data = arguments.at(0).toByteArray();
    commandProtocol proto(&data, QStringConverter::Encoding::Utf16LE);
    QCOMPARE(proto.getDecodedType() , commandProtocol::_NA_FRAME);
    QCOMPARE(proto.getParams()->count() , 1);
    QCOMPARE((proto.getParams())->at(0) , "EXEC_PowerOff");
    delete spy;
}

void test_awsInterface::test_awsInterface_EXEC_ResetError()
{
    QByteArray data;
    QString stringa;
    auto fromUtf = QStringEncoder(QStringConverter::Encoding::Utf16LE);

  //stringa = "<01 0036 % EXEC_AbortProjections % >";
    stringa = "<01 0036 % EXEC_ResetError       % >";
    spy = new QSignalSpy(pClient, SIGNAL(rxData(QByteArray)));
    pClient->txData(fromUtf(stringa));
    while (spy->count() == 0)  QTest::qWait(10);
    QCOMPARE(spy->count(), 1);

    arguments = spy->takeFirst();
    data = arguments.at(0).toByteArray();
    commandProtocol proto(&data, QStringConverter::Encoding::Utf16LE);
    QCOMPARE(proto.getDecodedType() , commandProtocol::_NA_FRAME);
    QCOMPARE(proto.getParams()->count() , 1);
    QCOMPARE((proto.getParams())->at(0) , "EXEC_ResetError");
    delete spy;
}

void test_awsInterface::test_awsInterface_EXEC_StartXraySequence()
{
    QByteArray data;
    QString stringa;
    auto fromUtf = QStringEncoder(QStringConverter::Encoding::Utf16LE);

  //stringa = "<01 0036 % EXEC_AbortProjections % >";
    stringa = "<01 0036 % EXEC_StartXraySequence% >";
    spy = new QSignalSpy(pClient, SIGNAL(rxData(QByteArray)));
    pClient->txData(fromUtf(stringa));
    while (spy->count() == 0)  QTest::qWait(10);
    QCOMPARE(spy->count(), 1);

    arguments = spy->takeFirst();
    data = arguments.at(0).toByteArray();
    commandProtocol proto(&data, QStringConverter::Encoding::Utf16LE);
    QCOMPARE(proto.getDecodedType() , commandProtocol::_NA_FRAME);
    QCOMPARE(proto.getParams()->count() , 1);
    QCOMPARE((proto.getParams())->at(0) , "EXEC_StartXraySequence");
    delete spy;
}

void test_awsInterface::test_awsInterface_EXEC_TrxPosition()
{
    QByteArray data;
    QString stringa;
    auto fromUtf = QStringEncoder(QStringConverter::Encoding::Utf16LE);

  //stringa = "<01 0036 % EXEC_AbortProjections % >";
    stringa = "<01 0036 % EXEC_TrxPosition      % >";
    spy = new QSignalSpy(pClient, SIGNAL(rxData(QByteArray)));
    pClient->txData(fromUtf(stringa));
    while (spy->count() == 0)  QTest::qWait(10);
    QCOMPARE(spy->count(), 1);

    arguments = spy->takeFirst();
    data = arguments.at(0).toByteArray();
    commandProtocol proto(&data, QStringConverter::Encoding::Utf16LE);
    QCOMPARE(proto.getDecodedType() , commandProtocol::_NA_FRAME);
    QCOMPARE(proto.getParams()->count() , 1);
    QCOMPARE((proto.getParams())->at(0) , "EXEC_TrxPosition");
    delete spy;
}

void test_awsInterface::test_awsInterface_GET_Accessories()
{
    QByteArray data;
    QString stringa;
    auto fromUtf = QStringEncoder(QStringConverter::Encoding::Utf16LE);

  //stringa = "<01 0036 % EXEC_AbortProjections % >";
    stringa = "<01 0036 % GET_Accessories       % >";
    spy = new QSignalSpy(pClient, SIGNAL(rxData(QByteArray)));
    pClient->txData(fromUtf(stringa));
    while (spy->count() == 0)  QTest::qWait(10);
    QCOMPARE(spy->count(), 1);

    arguments = spy->takeFirst();
    data = arguments.at(0).toByteArray();
    commandProtocol proto(&data, QStringConverter::Encoding::Utf16LE);
    QCOMPARE(proto.getDecodedType() , commandProtocol::_NA_FRAME);
    QCOMPARE(proto.getParams()->count() , 1);
    QCOMPARE((proto.getParams())->at(0) , "GET_Accessories");
    delete spy;
}

void test_awsInterface::test_awsInterface_GET_CompressorData()
{
    QByteArray data;
    QString stringa;
    auto fromUtf = QStringEncoder(QStringConverter::Encoding::Utf16LE);

  //stringa = "<01 0036 % EXEC_AbortProjections % >";
    stringa = "<01 0036 % GET_CompressorData    % >";
    spy = new QSignalSpy(pClient, SIGNAL(rxData(QByteArray)));
    pClient->txData(fromUtf(stringa));
    while (spy->count() == 0)  QTest::qWait(10);
    QCOMPARE(spy->count(), 1);

    arguments = spy->takeFirst();
    data = arguments.at(0).toByteArray();
    commandProtocol proto(&data, QStringConverter::Encoding::Utf16LE);
    QCOMPARE(proto.getDecodedType() , commandProtocol::_NA_FRAME);
    QCOMPARE(proto.getParams()->count() , 1);
    QCOMPARE((proto.getParams())->at(0) , "GET_CompressorData");
    delete spy;
}

void test_awsInterface::test_awsInterface_GET_ExposureCompletionData()
{
    QByteArray data;
    QString stringa;
    auto fromUtf = QStringEncoder(QStringConverter::Encoding::Utf16LE);

  //stringa = "<01 0041 % EXEC_AbortProjections      % >";
    stringa = "<01 0041 % GET_ExposureCompletionData % >";
    spy = new QSignalSpy(pClient, SIGNAL(rxData(QByteArray)));
    pClient->txData(fromUtf(stringa));
    while (spy->count() == 0)  QTest::qWait(10);
    QCOMPARE(spy->count(), 1);

    arguments = spy->takeFirst();
    data = arguments.at(0).toByteArray();
    commandProtocol proto(&data, QStringConverter::Encoding::Utf16LE);
    QCOMPARE(proto.getDecodedType() , commandProtocol::_NA_FRAME);
    QCOMPARE(proto.getParams()->count() , 1);
    QCOMPARE((proto.getParams())->at(0) , "GET_ExposureCompletionData");
    delete spy;
}

void test_awsInterface::test_awsInterface_GET_ReadyForExposure()
{
    QByteArray data;
    QString stringa;
    auto fromUtf = QStringEncoder(QStringConverter::Encoding::Utf16LE);

  //stringa = "<01 0041 % EXEC_AbortProjections      % >";
    stringa = "<01 0041 % GET_ReadyForExposure       % >";
    spy = new QSignalSpy(pClient, SIGNAL(rxData(QByteArray)));
    pClient->txData(fromUtf(stringa));
    while (spy->count() == 0)  QTest::qWait(10);
    QCOMPARE(spy->count(), 1);

    arguments = spy->takeFirst();
    data = arguments.at(0).toByteArray();
    commandProtocol proto(&data, QStringConverter::Encoding::Utf16LE);
    QCOMPARE(proto.getDecodedType() , commandProtocol::_NA_FRAME);
    QCOMPARE(proto.getParams()->count() , 1);
    QCOMPARE((proto.getParams())->at(0) , "GET_ReadyForExposure");
    delete spy;
}

void test_awsInterface::test_awsInterface_GET_RotationAngles()
{
    QByteArray data;
    QString stringa;
    auto fromUtf = QStringEncoder(QStringConverter::Encoding::Utf16LE);

  //stringa = "<01 0041 % EXEC_AbortProjections      % >";
    stringa = "<01 0041 % GET_RotationAngles         % >";
    spy = new QSignalSpy(pClient, SIGNAL(rxData(QByteArray)));
    pClient->txData(fromUtf(stringa));
    while (spy->count() == 0)  QTest::qWait(10);
    QCOMPARE(spy->count(), 1);

    arguments = spy->takeFirst();
    data = arguments.at(0).toByteArray();
    commandProtocol proto(&data, QStringConverter::Encoding::Utf16LE);
    QCOMPARE(proto.getDecodedType() , commandProtocol::_NA_FRAME);
    QCOMPARE(proto.getParams()->count() , 1);
    QCOMPARE((proto.getParams())->at(0) , "GET_RotationAngles");
    delete spy;
}


void test_awsInterface::test_awsInterface_GET_TomoConfig()
{
    QByteArray data;
    QString stringa;
    auto fromUtf = QStringEncoder(QStringConverter::Encoding::Utf16LE);

  //stringa = "<01 0041 % EXEC_AbortProjections      % >";
    stringa = "<01 0041 % GET_TomoConfig             % >";
    spy = new QSignalSpy(pClient, SIGNAL(rxData(QByteArray)));
    pClient->txData(fromUtf(stringa));
    while (spy->count() == 0)  QTest::qWait(10);
    QCOMPARE(spy->count(), 1);

    arguments = spy->takeFirst();
    data = arguments.at(0).toByteArray();
    commandProtocol proto(&data, QStringConverter::Encoding::Utf16LE);
    QCOMPARE(proto.getDecodedType() , commandProtocol::_NA_FRAME);
    QCOMPARE(proto.getParams()->count() , 1);
    QCOMPARE((proto.getParams())->at(0) , "GET_TomoConfig");
    delete spy;
}

void test_awsInterface::test_awsInterface_GET_TomoConfigFile()
{
    QByteArray data;
    QString stringa;
    auto fromUtf = QStringEncoder(QStringConverter::Encoding::Utf16LE);

  //stringa = "<01 0041 % EXEC_AbortProjections      % >";
    stringa = "<01 0041 % GET_TomoConfigFile         % >";
    spy = new QSignalSpy(pClient, SIGNAL(rxData(QByteArray)));
    pClient->txData(fromUtf(stringa));
    while (spy->count() == 0)  QTest::qWait(10);
    QCOMPARE(spy->count(), 1);

    arguments = spy->takeFirst();
    data = arguments.at(0).toByteArray();
    commandProtocol proto(&data, QStringConverter::Encoding::Utf16LE);
    QCOMPARE(proto.getDecodedType() , commandProtocol::_NA_FRAME);
    QCOMPARE(proto.getParams()->count() , 1);
    QCOMPARE((proto.getParams())->at(0) , "GET_TomoConfigFile");
    delete spy;
}

void test_awsInterface::test_awsInterface_GET_TubeTemperature()
{
    QByteArray data;
    QString stringa;
    auto fromUtf = QStringEncoder(QStringConverter::Encoding::Utf16LE);

  //stringa = "<01 0041 % EXEC_AbortProjections      % >";
    stringa = "<01 0041 % GET_TubeTemperature        % >";
    spy = new QSignalSpy(pClient, SIGNAL(rxData(QByteArray)));
    pClient->txData(fromUtf(stringa));
    while (spy->count() == 0)  QTest::qWait(10);
    QCOMPARE(spy->count(), 1);

    arguments = spy->takeFirst();
    data = arguments.at(0).toByteArray();
    commandProtocol proto(&data, QStringConverter::Encoding::Utf16LE);
    QCOMPARE(proto.getDecodedType() , commandProtocol::_NA_FRAME);
    QCOMPARE(proto.getParams()->count() , 1);
    QCOMPARE((proto.getParams())->at(0) , "GET_TubeTemperature");
    delete spy;
}

void test_awsInterface::test_awsInterface_GET_XrayPushStatus()
{
    QByteArray data;
    QString stringa;
    auto fromUtf = QStringEncoder(QStringConverter::Encoding::Utf16LE);

  //stringa = "<01 0041 % EXEC_AbortProjections      % >";
    stringa = "<01 0041 % GET_XrayPushStatus         % >";
    spy = new QSignalSpy(pClient, SIGNAL(rxData(QByteArray)));
    pClient->txData(fromUtf(stringa));
    while (spy->count() == 0)  QTest::qWait(10);
    QCOMPARE(spy->count(), 1);

    arguments = spy->takeFirst();
    data = arguments.at(0).toByteArray();
    commandProtocol proto(&data, QStringConverter::Encoding::Utf16LE);
    QCOMPARE(proto.getDecodedType() , commandProtocol::_NA_FRAME);
    QCOMPARE(proto.getParams()->count() , 1);
    QCOMPARE((proto.getParams())->at(0) , "GET_XrayPushStatus");
    delete spy;
}

void test_awsInterface::test_awsInterface_SET_CompressionRelease()
{
    QByteArray data;
    QString stringa;
    auto fromUtf = QStringEncoder(QStringConverter::Encoding::Utf16LE);

  //stringa = "<01 0041 % EXEC_AbortProjections      % >";
    stringa = "<01 0041 % SET_CompressionRelease     % >";
    spy = new QSignalSpy(pClient, SIGNAL(rxData(QByteArray)));
    pClient->txData(fromUtf(stringa));
    while (spy->count() == 0)  QTest::qWait(10);
    QCOMPARE(spy->count(), 1);

    arguments = spy->takeFirst();
    data = arguments.at(0).toByteArray();
    commandProtocol proto(&data, QStringConverter::Encoding::Utf16LE);
    QCOMPARE(proto.getDecodedType() , commandProtocol::_NA_FRAME);
    QCOMPARE(proto.getParams()->count() , 1);
    QCOMPARE((proto.getParams())->at(0) , "SET_CompressionRelease");
    delete spy;
}

void test_awsInterface::test_awsInterface_SET_Language()
{
    QByteArray data;
    QString stringa;
    auto fromUtf = QStringEncoder(QStringConverter::Encoding::Utf16LE);

  //stringa = "<01 0041 % EXEC_AbortProjections      % >";
    stringa = "<01 0041 % SET_Language               % >";
    spy = new QSignalSpy(pClient, SIGNAL(rxData(QByteArray)));
    pClient->txData(fromUtf(stringa));
    while (spy->count() == 0)  QTest::qWait(10);
    QCOMPARE(spy->count(), 1);

    arguments = spy->takeFirst();
    data = arguments.at(0).toByteArray();
    commandProtocol proto(&data, QStringConverter::Encoding::Utf16LE);
    QCOMPARE(proto.getDecodedType() , commandProtocol::_NA_FRAME);
    QCOMPARE(proto.getParams()->count() , 1);
    QCOMPARE((proto.getParams())->at(0) , "SET_Language");
    delete spy;
}

void test_awsInterface::test_awsInterface_SET_ProjectionList()
{
    QByteArray data;
    QString stringa;
    auto fromUtf = QStringEncoder(QStringConverter::Encoding::Utf16LE);

  //stringa = "<01 0041 % EXEC_AbortProjections      % >";
    stringa = "<01 0041 % SET_ProjectionList         % >";
    spy = new QSignalSpy(pClient, SIGNAL(rxData(QByteArray)));
    pClient->txData(fromUtf(stringa));
    while (spy->count() == 0)  QTest::qWait(10);
    QCOMPARE(spy->count(), 1);

    arguments = spy->takeFirst();
    data = arguments.at(0).toByteArray();
    commandProtocol proto(&data, QStringConverter::Encoding::Utf16LE);
    QCOMPARE(proto.getDecodedType() , commandProtocol::_NA_FRAME);
    QCOMPARE(proto.getParams()->count() , 1);
    QCOMPARE((proto.getParams())->at(0) , "SET_ProjectionList");
    delete spy;
}

void test_awsInterface::test_awsInterface_SET_PulseData()
{
    QByteArray data;
    QString stringa;
    auto fromUtf = QStringEncoder(QStringConverter::Encoding::Utf16LE);

  //stringa = "<01 0041 % EXEC_AbortProjections      % >";
    stringa = "<01 0041 % SET_PulseData              % >";
    spy = new QSignalSpy(pClient, SIGNAL(rxData(QByteArray)));
    pClient->txData(fromUtf(stringa));
    while (spy->count() == 0)  QTest::qWait(10);
    QCOMPARE(spy->count(), 1);

    arguments = spy->takeFirst();
    data = arguments.at(0).toByteArray();
    commandProtocol proto(&data, QStringConverter::Encoding::Utf16LE);
    QCOMPARE(proto.getDecodedType() , commandProtocol::_NA_FRAME);
    QCOMPARE(proto.getParams()->count() , 1);
    QCOMPARE((proto.getParams())->at(0) , "SET_PulseData");
    delete spy;
}

void test_awsInterface::test_awsInterface_SET_XrayPushEnable()
{
    QByteArray data;
    QString stringa;
    auto fromUtf = QStringEncoder(QStringConverter::Encoding::Utf16LE);

  //stringa = "<01 0041 % EXEC_AbortProjections      % >";
    stringa = "<01 0041 % SET_XrayPushEnable         % >";
    spy = new QSignalSpy(pClient, SIGNAL(rxData(QByteArray)));
    pClient->txData(fromUtf(stringa));
    while (spy->count() == 0)  QTest::qWait(10);
    QCOMPARE(spy->count(), 1);

    arguments = spy->takeFirst();
    data = arguments.at(0).toByteArray();
    commandProtocol proto(&data, QStringConverter::Encoding::Utf16LE);
    QCOMPARE(proto.getDecodedType() , commandProtocol::_NA_FRAME);
    QCOMPARE(proto.getParams()->count() , 1);
    QCOMPARE((proto.getParams())->at(0) , "SET_XrayPushEnable");
    delete spy;
}

void test_awsInterface::test_awsInterface_GANTRY_AbortProjection()
{

    spy = new QSignalSpy(pClient, SIGNAL(rxData(QByteArray)));
    pInterface->GANTRY_AbortProjection();
    while (spy->count() == 0)  QTest::qWait(10);
    QCOMPARE(spy->count(), 1);

    arguments = spy->takeFirst();
    QByteArray data = arguments.at(0).toByteArray();

    commandProtocol proto(&data, QStringConverter::Encoding::Utf16LE);
    QCOMPARE(proto.getDecodedType() , commandProtocol::_STATUS_FRAME);
    QCOMPARE(proto.getParams()->count() , 0);
    QCOMPARE(proto.getCmd() , "GANTRY_AbortProjection");

    delete spy;
}
void test_awsInterface::test_awsInterface_GANTRY_ERROR()
{

    spy = new QSignalSpy(pClient, SIGNAL(rxData(QByteArray)));
    pInterface->GANTRY_ERROR();
    while (spy->count() == 0)  QTest::qWait(10);
    QCOMPARE(spy->count(), 1);

    arguments = spy->takeFirst();
    QByteArray data = arguments.at(0).toByteArray();

    commandProtocol proto(&data, QStringConverter::Encoding::Utf16LE);
    QCOMPARE(proto.getDecodedType() , commandProtocol::_STATUS_FRAME);
    QCOMPARE(proto.getParams()->count() , 0);
    QCOMPARE(proto.getCmd() , "GANTRY_ERROR");

    delete spy;
}

void test_awsInterface::test_awsInterface_GANTRY_PulseCompleted()
{

    spy = new QSignalSpy(pClient, SIGNAL(rxData(QByteArray)));
    pInterface->GANTRY_PulseCompleted();
    while (spy->count() == 0)  QTest::qWait(10);
    QCOMPARE(spy->count(), 1);

    arguments = spy->takeFirst();
    QByteArray data = arguments.at(0).toByteArray();

    commandProtocol proto(&data, QStringConverter::Encoding::Utf16LE);
    QCOMPARE(proto.getDecodedType() , commandProtocol::_STATUS_FRAME);
    QCOMPARE(proto.getParams()->count() , 0);
    QCOMPARE(proto.getCmd() , "GANTRY_PulseCompleted");

    delete spy;
}

void test_awsInterface::test_awsInterface_GANTRY_SelectProjection()
{

    spy = new QSignalSpy(pClient, SIGNAL(rxData(QByteArray)));
    pInterface->GANTRY_SelectProjection("RCC");
    while (spy->count() == 0)  QTest::qWait(10);
    QCOMPARE(spy->count(), 1);

    arguments = spy->takeFirst();
    QByteArray data = arguments.at(0).toByteArray();

    commandProtocol proto(&data, QStringConverter::Encoding::Utf16LE);
    QCOMPARE(proto.getDecodedType() , commandProtocol::_STATUS_FRAME);
    QCOMPARE(proto.getParams()->count() , 1);
    QCOMPARE(proto.getCmd() , "GANTRY_SelectProjection");

    delete spy;
}

void test_awsInterface::test_awsInterface_GANTRY_SetAccessories()
{

    spy = new QSignalSpy(pClient, SIGNAL(rxData(QByteArray)));
    pInterface->GANTRY_SetAccessories("POTTER", "PADDLE", "COLLI");
    while (spy->count() == 0)  QTest::qWait(10);
    QCOMPARE(spy->count(), 1);

    arguments = spy->takeFirst();
    QByteArray data = arguments.at(0).toByteArray();

    commandProtocol proto(&data, QStringConverter::Encoding::Utf16LE);
    QCOMPARE(proto.getDecodedType() , commandProtocol::_STATUS_FRAME);
    QCOMPARE(proto.getParams()->count() , 3);
    QCOMPARE(proto.getCmd() , "GANTRY_SetAccessories");

    delete spy;
}

void test_awsInterface::test_awsInterface_GANTRY_SetTubeTemperature()
{

    spy = new QSignalSpy(pClient, SIGNAL(rxData(QByteArray)));
    pInterface->GANTRY_SetTubeTemperature("ANODE","HOUSING");
    while (spy->count() == 0)  QTest::qWait(10);
    QCOMPARE(spy->count(), 1);

    arguments = spy->takeFirst();
    QByteArray data = arguments.at(0).toByteArray();

    commandProtocol proto(&data, QStringConverter::Encoding::Utf16LE);
    QCOMPARE(proto.getDecodedType() , commandProtocol::_STATUS_FRAME);
    QCOMPARE(proto.getParams()->count() , 2);
    QCOMPARE(proto.getCmd() , "GANTRY_SetTubeTemperature");

    delete spy;
}

void test_awsInterface::test_awsInterface_GANTRY_XrayPushEvent()
{

    spy = new QSignalSpy(pClient, SIGNAL(rxData(QByteArray)));
    pInterface->GANTRY_XrayPushEvent();
    while (spy->count() == 0)  QTest::qWait(10);
    QCOMPARE(spy->count(), 1);

    arguments = spy->takeFirst();
    QByteArray data = arguments.at(0).toByteArray();

    commandProtocol proto(&data, QStringConverter::Encoding::Utf16LE);
    QCOMPARE(proto.getDecodedType() , commandProtocol::_STATUS_FRAME);
    QCOMPARE(proto.getParams()->count() , 0);
    QCOMPARE(proto.getCmd() , "GANTRY_XrayPushEvent");

    delete spy;
}

void test_awsInterface::test_awsInterface_GANTRY_XraySequenceCompleted()
{

    spy = new QSignalSpy(pClient, SIGNAL(rxData(QByteArray)));
    pInterface->GANTRY_XraySequenceCompleted("OK");
    while (spy->count() == 0)  QTest::qWait(10);
    QCOMPARE(spy->count(), 1);

    arguments = spy->takeFirst();
    QByteArray data = arguments.at(0).toByteArray();

    commandProtocol proto(&data, QStringConverter::Encoding::Utf16LE);
    QCOMPARE(proto.getDecodedType() , commandProtocol::_STATUS_FRAME);
    QCOMPARE(proto.getParams()->count() , 1);
    QCOMPARE(proto.getCmd() , "GANTRY_XraySequenceCompleted");

    delete spy;
}


QTEST_MAIN(test_awsInterface)

#include "tst_awsinterface.moc"
