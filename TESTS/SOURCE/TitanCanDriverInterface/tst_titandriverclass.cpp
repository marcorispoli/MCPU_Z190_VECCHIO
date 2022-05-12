#include <QtTest>
#include <QCoreApplication>

// add necessary includes here
#include "titanCanDriver.h"
#include "canDriverClassInterface.h"

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
    QSignalSpy* spy;

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
    void test_titanDriverInterface_8();

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


void test_titanDriverInterface::cleanupTestCase()
{
    driver_ch0.driverClose();
    driver_ch1.driverClose();

}

void test_titanDriverInterface::test_titanDriverInterface_1(){

    canDriverInterface::_canDriverErrors opstat = driver_ch0.driverOpen("COM8",canDriverInterface::_CAN_1000K,0x100,0x7FF,"NORMAL");
    QCOMPARE(opstat, canDriverInterface::_CAN_DRIVER_OK);

    qInfo(driver_ch0.getVersion());
    qInfo(driver_ch0.getSerialNumber());
    driver_ch0.driverClose();
}

void test_titanDriverInterface::test_titanDriverInterface_2(){
    canDriverInterface::_canDriverErrors opstat = driver_ch1.driverOpen("COM9",canDriverInterface::_CAN_1000K,0x100,0x7FF,"NORMAL");
    QCOMPARE(opstat, canDriverInterface::_CAN_DRIVER_OK);

    qInfo(driver_ch1.getVersion());
    qInfo(driver_ch1.getSerialNumber());
    driver_ch1.driverClose();
}


void test_titanDriverInterface::test_titanDriverInterface_3(){
    QByteArray buffer;
    QString stringa;
    canDriverInterface::canDataFrame tx_canframe;
    canDriverInterface::canDataFrame rx_canframe;

    canDriverInterface::_canDriverErrors opstat = driver_ch0.driverOpen("COM8",canDriverInterface::_CAN_1000K,0x100,0x7FF,"NORMAL");
    QCOMPARE(opstat, canDriverInterface::_CAN_DRIVER_OK);
    opstat = driver_ch1.driverOpen("COM9",canDriverInterface::_CAN_1000K,0x100,0x7FF,"NORMAL");
    QCOMPARE(opstat, canDriverInterface::_CAN_DRIVER_OK);


    int wrong_rx = 0;
    int wrong_format = 0;

    uchar ofs = 0;
    uchar cval;
    int i;

    bool action;
    for(int j=0; j<10; j++){

        tx_canframe.canId = 0x100;

        for(int i=0; i<8; i++){
            cval = (uchar) i+ofs;
            tx_canframe.data[i] = cval;
        }

        driver_ch0.driverWrite(tx_canframe);
        for(i=0; i<10; i++){
            if(driver_ch1.driverRead())  break;
            else QTest::qSleep(1);
        }
        if(i == 10){
            wrong_rx++;
            continue;
        }

        if(driver_ch1.getCanRxFrame().canId != 0x100){
            wrong_format++;
            continue;
        }

        action = true;
        for(int i=0; i<8; i++){
            cval = (uchar) i+ofs;            
            if(driver_ch1.getCanRxFrame().data[i] != cval){
                action = false;
                break;
            }
        }
        if(!action)
        {
            wrong_format++;
            continue;
        }

        ofs++;
    }

    driver_ch0.driverClose();
    driver_ch1.driverClose();

    QCOMPARE(wrong_rx, 0);
    QCOMPARE(wrong_format, 0);

}

void test_titanDriverInterface::test_titanDriverInterface_4(){
    QByteArray buffer;
    QString stringa;
    canDriverInterface::canDataFrame tx_canframe;
    canDriverInterface::canDataFrame rx_canframe;

    canDriverInterface::_canDriverErrors opstat = driver_ch0.driverOpen("COM8",canDriverInterface::_CAN_1000K,0x100,0x7FE,"NORMAL");
    QCOMPARE(opstat, canDriverInterface::_CAN_DRIVER_OK);
    opstat = driver_ch1.driverOpen("COM9",canDriverInterface::_CAN_1000K,0x100,0x7FE,"NORMAL");
    QCOMPARE(opstat, canDriverInterface::_CAN_DRIVER_OK);


    int wrong_rx = 0;
    int wrong_format = 0;

    uchar ofs = 0;
    uchar cval;
    int i;

    bool action;
    for(int j=0; j<100; j++){

        tx_canframe.canId = 0x100;
        driver_ch0.driverWrite(tx_canframe);
        tx_canframe.canId = 0x101;
        driver_ch0.driverWrite(tx_canframe);

        for(i=0; i<10; i++){
            if(driver_ch1.driverRead())  break;
            else QTest::qSleep(1);
        }
        if(i == 10){
            wrong_rx++;
            continue;
        }

        if(driver_ch1.getCanRxFrame().canId != 0x100){
            wrong_format++;
            continue;
        }

        for(i=0; i<10; i++){
            if(driver_ch1.driverRead())  break;
            else QTest::qSleep(1);
        }
        if(i == 10){
            wrong_rx++;
            continue;
        }

        if(driver_ch1.getCanRxFrame().canId != 0x101){
            wrong_format++;
            continue;
        }

    }

    driver_ch0.driverClose();
    driver_ch1.driverClose();

    QCOMPARE(wrong_rx, 0);
    QCOMPARE(wrong_format, 0);

}



void test_titanDriverInterface::test_titanDriverInterface_5(){
    QByteArray buffer;
    QString stringa;
    canDriverInterface::canDataFrame tx_canframe;
    canDriverInterface::canDataFrame rx_canframe;
    bool action;


    canDriverInterface::_canDriverErrors opstat = driver_ch0.driverOpen("COM8",canDriverInterface::_CAN_1000K,0x100,0x7FF,"LOOPBACK");
    QCOMPARE(opstat, canDriverInterface::_CAN_DRIVER_OK);


    int wrong_rx = 0;
    int wrong_format = 0;

    uchar ofs = 0;
    uchar cval;
    int i;


    for(int j=0; j<100; j++){

        tx_canframe.canId = 0x100;

        for(int i=0; i<8; i++){
            cval = (uchar) i+ofs;
            tx_canframe.data[i] = cval;
        }


        driver_ch0.driverWrite(tx_canframe);
        for(i=0; i<10; i++){
            if(driver_ch0.driverRead())  break;
            else QTest::qSleep(1);
        }
        if(i == 10){
            wrong_rx++;
            continue;
        }
        rx_canframe = driver_ch0.getCanRxFrame();

        if(driver_ch0.getCanRxFrame().canId != 0x100){
            wrong_format++;
            continue;
        }

        action = true;

        for(int i=0; i<8; i++){
            cval = (uchar) i+ofs;
            if(rx_canframe.data[i] != cval){
                action = false;
                break;
            }
        }
        if(!action)
        {
            wrong_format++;
            continue;
        }


        ofs++;
    }
    driver_ch0.driverClose();

    QCOMPARE(wrong_rx, 0);
    QCOMPARE(wrong_format, 0);



}

void test_titanDriverInterface::test_titanDriverInterface_6(){
    QByteArray buffer;
    QString stringa;
    canDriverInterface::canDataFrame tx_canframe;
    canDriverInterface::canDataFrame rx_canframe;
    bool action;


    canDriverInterface::_canDriverErrors opstat = driver_ch0.driverOpen("COM8",canDriverInterface::_CAN_1000K,0x100,0x7FF,"LOOPBACK");
    QCOMPARE(opstat, canDriverInterface::_CAN_DRIVER_OK);


    int wrong_rx = 0;
    int wrong_format = 0;
    int i;


    for(int j=0; j<100; j++){

        tx_canframe.canId = 0x100;
        for(int i=0; i<8; i++){
            tx_canframe.data[i] = 1;
        }
        driver_ch0.driverWrite(tx_canframe);
        for(int i=0; i<8; i++){
            tx_canframe.data[i] = 2;
        }
        driver_ch0.driverWrite(tx_canframe);


        for(i=0; i<10; i++){
            if(driver_ch0.driverRead())  break;
            else QTest::qSleep(1);
        }
        if(i == 10){
            wrong_rx++;
            continue;
        }
        rx_canframe = driver_ch0.getCanRxFrame();
        if(rx_canframe.canId != 0x100){
            wrong_format++;
            continue;
        }
        action = true;
        for(int i=0; i<8; i++){
            if(rx_canframe.data[i] != 1){
                action = false;
                break;
            }
        }
        if(!action)
        {
            wrong_format++;
            continue;
        }

        for(i=0; i<10; i++){
            if(driver_ch0.driverRead())  break;
            else QTest::qSleep(1);
        }
        if(i == 10){
            wrong_rx++;
            continue;
        }
        rx_canframe = driver_ch0.getCanRxFrame();
        if(rx_canframe.canId != 0x100){
            wrong_format++;
            continue;
        }
        action = true;
        for(int i=0; i<8; i++){
            if(rx_canframe.data[i] != 2){
                action = false;
                break;
            }
        }
        if(!action)
        {
            wrong_format++;
            continue;
        }


    }
    driver_ch0.driverClose();

    QCOMPARE(wrong_rx, 0);
    QCOMPARE(wrong_format, 0);



}

void test_titanDriverInterface::test_titanDriverInterface_7(){
    canDriverInterface::canDataFrame tx_canframe;
    canDriverInterface::canDataFrame rx_canframe;

    canDriver driver ;
    driver.driverInitialize<titanCanDriver>();

    spy = new QSignalSpy(&driver, &canDriver::errorsSgn);
    driver.open("COM9",canDriverInterface::_CAN_1000K,0,0,"LOOPBACK");
    while (spy->count() == 0)  QTest::qWait(20);
    QCOMPARE(driver.getError(), canDriverInterface::_CAN_DRIVER_OK);
    delete spy;


    tx_canframe.canId = 0x581;
    spy = new QSignalSpy(&driver, &canDriver::received500Sgn);
    driver.send(tx_canframe);
    while (spy->count() == 0)  QTest::qWait(20);
    rx_canframe = driver.getRx500Frame();
    QCOMPARE(rx_canframe.canId, 0x581);
    delete spy;

    tx_canframe.canId = 0x481;
    spy = new QSignalSpy(&driver, &canDriver::received400Sgn);
    driver.send(tx_canframe);
    while (spy->count() == 0)  QTest::qWait(20);
    rx_canframe = driver.getRx400Frame();
    QCOMPARE(rx_canframe.canId, 0x481);
    delete spy;

    tx_canframe.canId = 0x381;
    spy = new QSignalSpy(&driver, &canDriver::received300Sgn);
    driver.send(tx_canframe);
    while (spy->count() == 0)  QTest::qWait(20);
    rx_canframe = driver.getRx300Frame();
    QCOMPARE(rx_canframe.canId, 0x381);
    delete spy;

    tx_canframe.canId = 0x281;
    spy = new QSignalSpy(&driver, &canDriver::received200Sgn);
    driver.send(tx_canframe);
    while (spy->count() == 0)  QTest::qWait(20);
    rx_canframe = driver.getRx200Frame();
    QCOMPARE(rx_canframe.canId, 0x281);
    delete spy;

    tx_canframe.canId = 0x181;
    spy = new QSignalSpy(&driver, &canDriver::received100Sgn);
    driver.send(tx_canframe);
    while (spy->count() == 0)  QTest::qWait(20);
    rx_canframe = driver.getRx100Frame();
    QCOMPARE(rx_canframe.canId, 0x181);
    delete spy;

    tx_canframe.canId = 0x81;
    spy = new QSignalSpy(&driver, &canDriver::received000Sgn);
    driver.send(tx_canframe);
    while (spy->count() == 0)  QTest::qWait(20);
    rx_canframe = driver.getRx000Frame();
    QCOMPARE(rx_canframe.canId, 0x081);
    delete spy;


    driver.close();


}

void test_titanDriverInterface::test_titanDriverInterface_8(){
    canDriverInterface::canDataFrame tx_canframe;
    canDriverInterface::canDataFrame rx_canframe;

    canDriver driver ;
    driver.driverInitialize<titanCanDriver>();

    spy = new QSignalSpy(&driver, &canDriver::errorsSgn);
    driver.open("COM9",canDriverInterface::_CAN_1000K,0x580,0x7E0,"LOOPBACK");
    while (spy->count() == 0)  QTest::qWait(20);
    QCOMPARE(driver.getError(), canDriverInterface::_CAN_DRIVER_OK);
    delete spy;

    uint no_answer = 0;
    uint no_address = 0;
    uint no_format = 0;
    uint attempt = 10;

    for(uint i=0; i<10; i++){
        tx_canframe.canId = 0x580;
        for(int j=0; j<8; j++)     tx_canframe.data[j] = (uchar) 1;

        spy = new QSignalSpy(&driver, &canDriver::received500Sgn);
        driver.send(tx_canframe);

        attempt = 10;
        while (attempt){
            if(spy->count()) break;
            QTest::qWait(1);
            attempt--;
        }
        if(!attempt){
            no_answer++;
            delete spy;
            continue;
        }

        rx_canframe = driver.getRx500Frame();
        if(rx_canframe.canId != 0x580){
            no_address++;
            delete spy;
            continue;
        }

        bool action = false;
        for(int j=0; j<8; j++){
            if(tx_canframe.data[j] != rx_canframe.data[j]){
                action = true;
                break;
            }
        }
        if(action){
            no_format++;
            delete spy;
            continue;
        }

        delete spy;

    }
    QCOMPARE(no_answer, 0);
    QCOMPARE(no_address, 0);
    QCOMPARE(no_format, 0);


    driver.close();


}

/*
void test_titanDriverInterface::test_titanDriverInterface_8(){



}



    deviceDriver = new usingDriver();
    deviceDriver->driver.openDriverPort("COM0", "1000", 0x580, 0x8FF, false);
    txframe.canId = 1;
    deviceDriver->txData(txframe,0,100);
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

QTEST_MAIN(test_titanDriverInterface)

#include "tst_titandriverclass.moc"
