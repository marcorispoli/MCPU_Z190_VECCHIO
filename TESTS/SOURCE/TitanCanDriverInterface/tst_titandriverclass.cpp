#include <QtTest>
#include <QCoreApplication>

// add necessary includes here
#include "titanCanDriver.h"

/**
 *  \defgroup TITAN_DRIVER_INTERFACE_CLASS_MODULE titanDriverInterface class Unit Test module
 *
 *  This module describes the tests on the titanDriverInterface class.
 *
 *  # TEST DESCRIPTION
 *
 *  The test makes use of the followng test classes:
 *  + driverClass: is the implementation of the class under test;
 *  + usingDriver: this is a class using the driverClass for test purposes;
 *
 *  The test are performed with the following test cases:
 *
 *  + test_canDriverInterface::initTestCase();
 *  + test_canDriverInterface::test_titanDriverInterface_1();
 *  + test_canDriverInterface::cleanupTestCase();
 *
 *  ## Driver creation and reception filter aperture test
 *  **Function**: test_canDriverInterface::initTestCase() \n
 *  **Description**: the test verifies the driver creation and opening features.
 *
 *  **Execution**:
 *  -# Create a driver class: this creates the user class and internally a thread with the canDriverInterface objeect to be tested;
 *  -# Open the canDriver reception port with a 0x580 filter ID and 0x8FF filter mask, with standard Identifier;
 *   + Verify that the communication is not running in this status;
 *   + Verify that the driver received frame is a frame with canId == 0;
 *
 *
 */


/**
 * @brief Executing class
 * \see TITAN_DRIVER_INTERFACE_CLASS_MODULE
 */
class test_titanDriverInterface : public QObject
{
    Q_OBJECT

public:
    test_titanDriverInterface();
    ~test_titanDriverInterface();

signals:




public slots:




public:
    titanCanDriver driver_ch0;
    titanCanDriver driver_ch1;

    bool status;

private slots:

    void initTestCase();
    void cleanupTestCase();
    void test_titanDriverInterface_1();
    void test_titanDriverInterface_2();
};

test_titanDriverInterface::test_titanDriverInterface()
{

}

test_titanDriverInterface::~test_titanDriverInterface()
{
}


void test_titanDriverInterface::initTestCase()
{
}
/*
deviceDriver = new usingDriver();
deviceDriver->driver.openDriverPort("COM0", "1000", 0x580, 0x8FF, false);
txframe.canId = 1;
deviceDriver->txData(txframe,0,100);
QSignalSpy spy(deviceDriver, SIGNAL(rxDone()));
while (spy.count() == 0)  QTest::qWait(200);
QCOMPARE(deviceDriver->rxframe.canId, 0);
*/
/*
// Read the hardware init
QSignalSpy* spy = new QSignalSpy(deviceDriver, SIGNAL(rxDone()));
deviceDriver->driver.runDriver();
txframe.canId = 1;
deviceDriver->txData(txframe,0,100);
while (spy->count() == 0)  QTest::qWait(200);
QCOMPARE(deviceDriver->rxframe.canId, 1);
QCOMPARE(deviceDriver->rxframe.data[0], 1); // Hardware Init
delete spy;
*/


void test_titanDriverInterface::cleanupTestCase()
{
    driver_ch0.hardwareShutdown();
    driver_ch1.hardwareShutdown();

}

void test_titanDriverInterface::test_titanDriverInterface_1(){
    status = driver_ch0.hardwareInit("COM8","1000");
    qInfo(driver_ch0.getVersion());
    qInfo(driver_ch0.getSerialNumber());
    qInfo(driver_ch0.getStatus());
    QCOMPARE(status, true);

}

void test_titanDriverInterface::test_titanDriverInterface_2(){
    status = driver_ch1.hardwareInit("COM9","1000");
    qInfo(driver_ch1.getVersion());
    qInfo(driver_ch1.getSerialNumber());
    qInfo(driver_ch1.getStatus());
    QCOMPARE(status, true);

}

void test_titanDriverInterface::test_titanDriverInterface_3(){


}



QTEST_MAIN(test_titanDriverInterface)

#include "tst_titandriverclass.moc"
