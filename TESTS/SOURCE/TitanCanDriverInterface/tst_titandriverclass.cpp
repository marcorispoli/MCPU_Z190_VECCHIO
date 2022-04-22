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
    void test_titanDriverInterface_3();
    void test_titanDriverInterface_4();
    void test_titanDriverInterface_5();
    void test_titanDriverInterface_6();
    void test_titanDriverInterface_7();
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
    status = driver_ch0.hardwareInit("COM8","500");
    qInfo(driver_ch0.getVersion());
    qInfo(driver_ch0.getSerialNumber());
    QCOMPARE(status, true);

}

void test_titanDriverInterface::test_titanDriverInterface_2(){
    status = driver_ch1.hardwareInit("COM9","500");
    qInfo(driver_ch1.getVersion());
    qInfo(driver_ch1.getSerialNumber());
    QCOMPARE(status, true);

}

void test_titanDriverInterface::test_titanDriverInterface_3(){

    status = driver_ch1.setStandardFilter(0x100, 0x7FF);
    qInfo(driver_ch1.getAddr());
    qInfo(driver_ch1.getMask());
    QCOMPARE(status, true);
    status = driver_ch0.setStandardFilter(0x100, 0x7FF);
    qInfo(driver_ch0.getAddr());
    qInfo(driver_ch0.getMask());
    QCOMPARE(status, true);
}

void test_titanDriverInterface::test_titanDriverInterface_4(){

    status = driver_ch0.open();
    QCOMPARE(status, true);
    status = driver_ch1.open();
    QCOMPARE(status, true);
}

void test_titanDriverInterface::test_titanDriverInterface_5(){
    QByteArray buffer;
    QString stringa;
    canDriverInterface::canDataFrame tx_canframe;
    canDriverInterface::canDataFrame rx_canframe;


    int wrong_send = 0;
    int wrong_rx = 0;
    int wrong_format = 0;

    uchar ofs = 0;
    uchar cval;

    bool action;
    for(int j=0; j<1000; j++){

        tx_canframe.canId = 0x100;

        for(int i=0; i<8; i++){
            cval = (uchar) i+ofs;
            tx_canframe.data[i] = cval;
        }

        driver_ch1.driverFlush();
        driver_ch0.driverFlush();
        status = driver_ch0.driverTx(tx_canframe);

        if(!status){
            wrong_send++;
            continue;
        }

        rx_canframe = driver_ch1.driverRx();
        if(rx_canframe.canId != 0x100){
            wrong_rx++;
            continue;
        }

        action = true;
        QString buf_data="D: ";
        for(int i=0; i<8; i++){
            cval = (uchar) i+ofs;
            buf_data += QString("%1 ").arg(cval);
            if(rx_canframe.data[i] != cval){
                action = false;
            }
        }
        if(!action)
        {
            buffer = driver_ch1.getRxFrame();
            stringa = buf_data;
            wrong_format++;
            continue;
        }

        ofs++;
    }


    if(wrong_format){
        qInfo(stringa.toLatin1());
        qInfo(buffer);
    }

    stringa = QString("SEND:%1, RX:%2, FORMAT:%3").arg(wrong_send).arg(wrong_rx).arg(wrong_format);
    qInfo(stringa.toLatin1());
    QCOMPARE(wrong_send, 0);
    QCOMPARE(wrong_rx, 0);
    QCOMPARE(wrong_format, 0);



}

void test_titanDriverInterface::test_titanDriverInterface_6(){
    QByteArray buffer;
    QString stringa;
    canDriverInterface::canDataFrame tx_canframe;
    canDriverInterface::canDataFrame rx_canframe;


    driver_ch0.close();
    driver_ch1.close();
    driver_ch0.open_loopback();

    int wrong_send = 0;
    int wrong_rx = 0;
    int wrong_format = 0;

    uchar ofs = 0;
    uchar cval;

    bool action;
    for(int j=0; j<1000; j++){

        tx_canframe.canId = 0x100;

        for(int i=0; i<8; i++){
            cval = (uchar) i+ofs;
            tx_canframe.data[i] = cval;
        }


        status = driver_ch0.driverTx(tx_canframe);

        if(!status){
            wrong_send++;
            continue;
        }

        rx_canframe = driver_ch0.driverRx();
        if(rx_canframe.canId != 0x100){
            wrong_rx++;
            continue;
        }

        action = true;
        QString buf_data="D: ";
        for(int i=0; i<8; i++){
            cval = (uchar) i+ofs;
            buf_data += QString("%1 ").arg(cval);
            if(rx_canframe.data[i] != cval){
                action = false;
            }
        }
        if(!action)
        {
            buffer = driver_ch0.getRxFrame();
            stringa = buf_data;
            wrong_format++;
            continue;
        }

        ofs++;
    }


    if(wrong_format){
        qInfo(stringa.toLatin1());
        qInfo(buffer);
    }

    stringa = QString("SEND:%1, RX:%2, FORMAT:%3").arg(wrong_send).arg(wrong_rx).arg(wrong_format);
    qInfo(stringa.toLatin1());
    QCOMPARE(wrong_send, 0);
    QCOMPARE(wrong_rx, 0);
    QCOMPARE(wrong_format, 0);



}

void test_titanDriverInterface::test_titanDriverInterface_7(){
    QByteArray buffer;
    QString stringa;
    canDriverInterface::canDataFrame tx_canframe;
    canDriverInterface::canDataFrame rx_canframe;

    driver_ch0.close();
    driver_ch1.close();
    driver_ch0.open_loopback();

    uchar ofs = 0;
    uchar cval;

    tx_canframe.canId = 0x100;
    for(int i=0; i<8; i++){
        cval = (uchar) i+ofs;
        tx_canframe.data[i] = cval;
    }

    QBENCHMARK{

        driver_ch0.driverFlush();
        status = driver_ch0.driverTx(tx_canframe);
        QCOMPARE(status,true);

        rx_canframe = driver_ch0.driverRx();
        QCOMPARE(rx_canframe.canId,0x100);


    }

}


QTEST_MAIN(test_titanDriverInterface)

#include "tst_titandriverclass.moc"
