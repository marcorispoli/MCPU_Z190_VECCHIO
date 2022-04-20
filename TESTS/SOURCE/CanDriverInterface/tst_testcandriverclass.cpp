#include <QtTest>
#include <QCoreApplication>

// add necessary includes here
#include "canDriverClassInterface.h"

/**
 *  \defgroup CAN_DRIVER_INTERFACE_CLASS_MODULE canDriverInterface class Unit Test module
 *
 *  This module describes the tests on the canDriverInterface class.
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
 *  + test_canDriverInterface::test_canDriverInterface_1();
 *  + test_canDriverInterface::test_canDriverInterface_2();
 *  + test_canDriverInterface::test_canDriverInterface_3();
 *  + test_canDriverInterface::test_canDriverInterface_4();
 *  + test_canDriverInterface::test_canDriverInterface_5();
 *  + test_canDriverInterface::test_canDriverInterface_6();
 *  + test_canDriverInterface::test_canDriverInterface_7();
 *  + test_canDriverInterface::test_canDriverInterface_8();
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
 *  ## Driver communication activation (setting run mode)
 *  **Function**: test_canDriverInterface::test_canDriverInterface_1() \n
 *  **Description**: The test verifies the  runDriver() method.
 *
 *  **Execution**:
 *  -# The test calls the runDriver() method of the driver;
 *  -# Sends a data frame with canId = 1;
 *  -# Waits the data reception from the driver§;
 *   + Verify that the receiver canId is correct;
 *   + Verify that HardwareInit flag of the driver is == TRUE (hardware inizialized)
 *
 *  ## Driver reads the Filter setting
 *  **Function**: test_canDriverInterface::test_canDriverInterface_2() \n
 *  **Description**: The Test read the current filter setting from the implemented Driver.
 *
 *  **Execution**:
 *  -# Sends a data frame with canId = 2;
 *  -# Waits the data reception from the driver§;
 *   + Verify that the receiver canId is correct;
 *   + Verify that the received filter field content is correct (0x580);
 *
 *  ## Driver reads the Filter Mask setting
 *  **Function**: test_canDriverInterface::test_canDriverInterface_3() \n
 *  **Description**: The Test read the current filter mask setting from the implemented Driver.
 *
 *  **Execution**:
 *  -# Sends a data frame with canId = 3;
 *  -# Waits the data reception from the driver§;
 *   + Verify that the receiver canId is correct;
 *   + Verify that the received filter mask field content is correct (0x08FF);
 *
 *  ## Driver reads the Extended/Standard mode setting
 *  **Function**: test_canDriverInterface::test_canDriverInterface_4() \n
 *  **Description**: The Test reads the current Extended/Standard mode from the implemented Driver.
 *
 *  **Execution**:
 *  -# Sends a data frame with canId = 4;
 *  -# Waits the data reception from the driver§;
 *   + Verify that the receiver canId is correct;
 *   + Verify that the received identifier mode flag is correct (==0);
 *
 *
 *  ## Driver reads the Run/Pause mode status
 *  **Function**: test_canDriverInterface::test_canDriverInterface_5() \n
 *  **Description**: The Test reads the current Run/Pause mode from the implemented Driver.
 *
 *  **Execution**:
 *  -# Sends a data frame with canId = 5;
 *  -# Waits the data reception from the driver§;
 *   + Verify that the receiver canId is correct;
 *   + Verify that the received Run mode flag is correct (==1);
 *
 *  ## Driver reads the Timeout value
 *  **Function**: test_canDriverInterface::test_canDriverInterface_6() \n
 *  **Description**: The Test reads the current Timeout value from the implemented Driver.
 *
 *  **Execution**:
 *  -# Sends a data frame with canId = 6, Timeout = 0x100;
 *  -# Waits the data reception from the driver§;
 *   + Verify that the receiver canId is correct;
 *   + Verify that the received Timeout is correct (0x100);
 *
 *  ## No Answer test
 *  **Function**: test_canDriverInterface::test_canDriverInterface_7() \n
 *  **Description**: The test verifies the case with no reception from the BUS.
 *
 *  **Execution**:
 *  -# Sends a data frame with canId = 7, Timeout = 0x100;
 *  -# Waits the data reception from the driver;
 *   + Verify that the receiver canId is equal to 0 (no reception event);
 *
 *  ## Driver Stop mode
 *  **Function**: test_canDriverInterface::test_canDriverInterface_8() \n
 *  **Description**: The test verifies the stopDriver() command.
 *
 *  **Execution**:
 *  -# Activate the stopDriver() function of the driver;
 *  -# Sends a data frame with canId = 2, Timeout = 0x100;
 *  -# Waits the data reception from the driver;
 *   + Verify that the receiver canId is equal to 0 (no reception event);
 *   + Verify that the internal HARDWARE_INI flag of the driver remains set to TRUE;
 *  -# Activate the runDriver() function of the driver;
 *  -# Sends a data frame with canId = 2, Timeout = 0x100;
 *  -# Waits the data reception from the driver;
 *   + Verify that the receiver canId is equal to 2 (read of the filter data command);
 *   + Verify that the receiver filter field is equal to 0x580 ;
 *
 *
 */


/**
 * \ingroup IMPLEMENT_CAN_DRIVER_INTERFACE_CLASS_MODULE
 * @brief This class implements the DUT class canDriverInterface
 *
 * This is the driver class implementation test.
 * The class inherit the canDriverInterface that is the
 * class under test.
 *
 * The class implements the hardware interface methods
 * that a can driver should implement:
 * +  hardwareInit(): shall be implemented in order to initi the specific hardware inetrface;
 * +  hardwareShutdown(): shall be implemented in order to terminates the hardware activities;
 * +  openPort(): this shall open a given reception filter;
 * +  closePorts(): closes all the reception filters;
 * +  run(): activates the bus arbitration;
 * +  pause(): stops the bus arbitration;
 * +  driverTxRxData(): implements the simulated tx/rx data exchange
 *
 * The TxRx simulated received frame (rxframe) implementation are:
 *
 *
 * + txframe.canId == 1 -> rxframe.data[0] = hardware initialization status (1 or 0);
 * + txframe.canId == 2 -> rxframe.data[0][1] = FILTER_ID (LSB first);
 * + txframe.canId == 3 -> rxframe.data[0][1] = FILTER_MASK (LSB first);
 * + txframe.canId == 4 -> rxframe.data[0] = Extended mode status;
 * + txframe.canId == 5 -> rxframe.data[0] = Running mode status;
 * + txframe.canId == 6 -> rxframe.data[0][1] = TIMEOUT (LSB first);
 * + txframe.canId == 7 -> Simulate no answer: rxframe.canId = 0;
 */
class driverClass: public canDriverInterface
{
    Q_OBJECT

public:
    driverClass(){
        HARDWARE_INI = false;
        FILTER_ID = 0;
        FILTER_MASK = 0;
        EXTENDED_MODE = false;
        RUNNING = false;
        TIMEOUT = 0;
    }

    /**
     * @name  Can Driver Api implementation section
     * @{
     */

    //! Implements the Hardware initialization operations
    bool hardwareInit(){ HARDWARE_INI = true; return true;}

    //! Implements the Hardware Shutdown Operations
    bool hardwareShutdown(){ HARDWARE_INI = false; return true;}

    //! Implements the reception filter aperture
    bool openPort(QString COM, QString BR, uint addr, uint msk, bool md){ FILTER_ID = addr; FILTER_MASK = msk; EXTENDED_MODE = md; return true;}

    //! Closes all the reception filters
    bool closePorts(void){FILTER_ID = 0; FILTER_MASK=0; RUNNING = false; return true;};

    //! Run Bus arbitration
    void run(void){RUNNING = true;}

    //! Pause Bus arbitration
    void pause(void){RUNNING = false;}

    /**
     * @brief driverTxRxData
     * This function implements a device simulation.
     * The simulated device gets the frame then answer with a test purpose
     * protocol in order to test the driver functionnalities.
     * @param frame
     * This is the frame sent by the userClass.
     * @param tmo
     * This is the Timeout that the driver sets for the
     * data reception.
     * @return
     * The txframe is filled with a simulated answer.
     * + txframe.canId == 1 -> rxframe.data[0] = hardware initialization status (1 or 0);
     * + txframe.canId == 2 -> rxframe.data[0][1] = FILTER_ID (LSB first);
     * + txframe.canId == 3 -> rxframe.data[0][1] = FILTER_MASK (LSB first);
     * + txframe.canId == 4 -> rxframe.data[0] = Extended mode status;
     * + txframe.canId == 5 -> rxframe.data[0] = Running mode status;
     * + txframe.canId == 6 -> rxframe.data[0][1] = TIMEOUT (LSB first);
     * + txframe.canId == 7 -> Simulate no answer: rxframe.canId = 0;
     */
    canDataFrame driverTxRxData(canDataFrame frame, int tmo) {
        canDataFrame txframe = frame;
        TIMEOUT = tmo;
        if(!RUNNING) {
            txframe.canId = 0;
            return txframe;
        }
        if(!HARDWARE_INI) {
            txframe.canId = 0;
            return txframe;
        }

        if(frame.canId == 1){
            if(HARDWARE_INI) txframe.data[0] = 1;
            else txframe.data[0] = 0;
            return txframe;
        }
        if(frame.canId == 2){
            txframe.data[0] = (uchar) FILTER_ID;
            txframe.data[1] = (uchar) (FILTER_ID>>8);
            return txframe;
        }
        if(frame.canId == 3){
            txframe.data[0] = (uchar) FILTER_MASK;
            txframe.data[1] = (uchar) (FILTER_MASK>>8);
            return txframe;
        }
        if(frame.canId == 4){
            if(EXTENDED_MODE) txframe.data[0] = 1;
            else txframe.data[0] = 0;
            return txframe;
        }
        if(frame.canId == 5){
            if(RUNNING) txframe.data[0] = 1;
            else txframe.data[0] = 0;
            return txframe;
        }
        if(frame.canId == 6){
            txframe.data[0] = (uchar) TIMEOUT;
            txframe.data[1] = (uchar) (TIMEOUT>>8);
            return txframe;
        }

        // Other condition return canId == 0
        txframe.canId = 0;
        return txframe;
    }

    ///@}

public:
    bool HARDWARE_INI; //!< The Hardware has been initialized
    uint FILTER_ID;    //!< Curent filter active
    uint FILTER_MASK;  //!< Current filter mask
    uint TIMEOUT;      //!< Timeout waiting data from the bus
    bool EXTENDED_MODE; //!<Extended/Standard mode
    bool RUNNING;       //!< Arbitration running
};

/**
 * @brief The usingDriver class
 *
 * This class implements a test class using the driver under test.
 *
 * The class implements the necessary interface methods:
 * +  signal txToDriver(): this slot is connected with the Driver reception slot;
 * +  public txData(): method to be used to send data: internally it triggers the txToDriver() signal;
 * +  public slot deviceRxSlot(): required to receive the reception data from the driver
 *
 * In order to let the test function to wait for Queued data exchange
 * the class implements an extra signal:
 * + signal rxDone(): this signal is emitted as soon as the deviceRxSlot() is called.
 *
 * In the class datata the received frame and odIndex is stored into the
 * rxframe and rxIndex variables.
 */
class usingDriver: public QObject
{
    Q_OBJECT
public:

    /**
     * @brief usingDriver
     * Constructor of the class: the constructor connects the class signals and slots necessary to
     * interact with the handled driver.
     *
     */
    usingDriver(void){
        connect(this, SIGNAL(txToDriver(canDriverInterface::canDataFrame,int,uint,QObject*)),
               &driver, SLOT(driverTxSlot(canDriverInterface::canDataFrame,int,uint,QObject*)), Qt::QueuedConnection);
    };

    ~usingDriver(void){

        // Necessary to clean the memory and processes
        driver.driverDistructor();
    };

signals:
    /**
     * @brief txToDriver
     * This signal functon is required in order to interact with the driver.
     * The signal is connected with the Out of thread driver slot, handling the TxRx data
     * activity.
     *
     * @param frame
     * This is the can frame sent to the driver
     * @param odIndex
     * This is the register index target of the transaction.
     * @param device
     * This is the pointer to the userClass that finally will receive the can received data frame.
     * \attention the userClass is the class that shall implement the slot deviceRxSlot()!
     */
    void txToDriver(canDriverInterface::canDataFrame frame, int odIndex, uint tmo, QObject* device);

    /**
     * @brief rxDone
     *
     * This is a test purpose signal used to trigger the test function when
     * data are received from the Out of Thread driver.
     * The test function shall use the QSignalSpy() looking ath this signal generation
     */
    void rxDone(void);

public slots:
    /**
     * @brief deviceRxSlot
     *
     * This is the **mandatory** function that every driver user class shall implement
     * in order to receive the driver can data frame.
     *
     *
     * @param frame
     * This is the can received data frame.
     *
     * @param odIndex
     * This is the Object Register index the driver userClass linked to the data exchange.
     *
     * The test implementation of this function slot stores the received data into public
     * variables that can easily be inspected by the test function.
     *
     * Finally the function triggers the rxDone() signal to trigger the test that the
     * data has been received from the driver.
     *
     */
    void deviceRxSlot(canDriverInterface::canDataFrame frame, int odIndex){
        rxframe = frame;
        rxIndex = odIndex;
        emit rxDone();
    }

public:

    /**
     * @brief txData
     *
     * This public methid is used to let the test function
     * sending data frames to the can driver.
     *
     * @param frame
     * This is the frame to be sent.
     * @param odIndex
     * This is the register index that will be received back
     * from the driver.
     *
     * The function emits the txToDriver() with the userClass target
     * set to this, because the user Class in this case is **this** class.
     */
    void txData(canDriverInterface::canDataFrame frame, int odIndex, uint tmo){
        emit txToDriver(frame, odIndex, tmo, this);
    }


    driverClass driver; //!< instance of the driverClass to be tested
    canDriverInterface::canDataFrame rxframe; //!< handles the received frame;
    int rxIndex; //!< handles the received register index

};

/**
 * @brief Executing class
 * \see CAN_DRIVER_INTERFACE_CLASS_MODULE
 */
class test_canDriverInterface : public QObject
{
    Q_OBJECT

public:
    test_canDriverInterface();
    ~test_canDriverInterface();

signals:




public slots:




public:
    usingDriver* deviceDriver;
    canDriverInterface::canDataFrame txframe;
private slots:

    void initTestCase();
    void cleanupTestCase();

    void test_canDriverInterface_1();
    void test_canDriverInterface_2();
    void test_canDriverInterface_3();
    void test_canDriverInterface_4();
    void test_canDriverInterface_5();
    void test_canDriverInterface_6();
    void test_canDriverInterface_7();
    void test_canDriverInterface_8();
};

test_canDriverInterface::test_canDriverInterface()
{

}

test_canDriverInterface::~test_canDriverInterface()
{

}

/**
 * @brief Driver creation and reception filter aperture
 *
 * This test creates the driver and open a reception filter.
 * The test shall verify that the Test should not communicate
 * beacuse it should not be in run mode now.
 */
void test_canDriverInterface::initTestCase()
{
    deviceDriver = new usingDriver();
    deviceDriver->driver.openDriverPort("COM0", "1000", 0x580, 0x8FF, false);
    txframe.canId = 1;
    deviceDriver->txData(txframe,0,100);
    QSignalSpy spy(deviceDriver, SIGNAL(rxDone()));
    while (spy.count() == 0)  QTest::qWait(200);
    QCOMPARE(deviceDriver->rxframe.canId, 0);
}

/**
 * @brief Driver deleting
 *
 * The user class is deleted and the driver should Shutdown.
 * The worker and the thread of the driver shall be deleted
 * successfully.
 *
 */
void test_canDriverInterface::cleanupTestCase()
{
    deviceDriver->deleteLater();

}



/**
*
* @brief The driver is set to Run Mode.
*
* The test verify the  runDriver() method:
* After the runDriver() command is used the test
* reads the Hardware Init flag from the implemented Driver.
*/
void test_canDriverInterface::test_canDriverInterface_1(){
    // Read the hardware init
    QSignalSpy* spy = new QSignalSpy(deviceDriver, SIGNAL(rxDone()));
    deviceDriver->driver.runDriver();
    txframe.canId = 1;
    deviceDriver->txData(txframe,0,100);
    while (spy->count() == 0)  QTest::qWait(200);
    QCOMPARE(deviceDriver->rxframe.canId, 1);
    QCOMPARE(deviceDriver->rxframe.data[0], 1); // Hardware Init
    delete spy;
}

/**
*
* @brief Read the Filter setting.
*
* The Test read the current filter setting from the implemented Driver:
* the current expected filter setting should be the one
* provided in the TestInit with the openPort command.
*/
void test_canDriverInterface::test_canDriverInterface_2(){
    // Read the filter id
    QSignalSpy* spy = new QSignalSpy(deviceDriver, SIGNAL(rxDone()));
    txframe.canId = 2;
    deviceDriver->txData(txframe,0,100);
    while (spy->count() == 0)  QTest::qWait(200);
    QCOMPARE(deviceDriver->rxframe.canId, 2);
    QCOMPARE(deviceDriver->rxframe.data[0], 0x80);
    QCOMPARE(deviceDriver->rxframe.data[1], 0x05);
    delete spy;
}


/**
*
* @brief Read the Filter Mask.
*
* The Test reads the current filter Mask from the implemented Driver:
* the current expected filter setting should be the one
* provided in the TestInit with the openPort command.
*/
void test_canDriverInterface::test_canDriverInterface_3(){
    // Read the filter mask
    QSignalSpy* spy = new QSignalSpy(deviceDriver, SIGNAL(rxDone()));
    txframe.canId = 3;
    deviceDriver->txData(txframe,0,100);
    while (spy->count() == 0)  QTest::qWait(200);
    QCOMPARE(deviceDriver->rxframe.canId, 3);
    QCOMPARE(deviceDriver->rxframe.data[0], 0xFF);
    QCOMPARE(deviceDriver->rxframe.data[1], 0x08);
    delete spy;
}

/**
*
* @brief Read the Extended/Standard mode setting.
*
* The Test reads the current Extended/Standard mode from the implemented Driver:
* the current expected  Extended/Standard mode setting should be the one
* provided in the TestInit with the openPort command.
*/
void test_canDriverInterface::test_canDriverInterface_4(){
    // Read the extended mode status
    QSignalSpy* spy = new QSignalSpy(deviceDriver, SIGNAL(rxDone()));
    txframe.canId = 4;
    deviceDriver->txData(txframe,0,100);
    while (spy->count() == 0)  QTest::qWait(200);
    QCOMPARE(deviceDriver->rxframe.canId, txframe.canId);
    QCOMPARE(deviceDriver->rxframe.data[0], 0);
    delete spy;
}


/**
*
* @brief Read the Run/Pause mode status.
*
* The Test reads the current Run/Pause mode from the implemented Driver:
* the expected  value shall be Run Mode.
*/
void test_canDriverInterface::test_canDriverInterface_5(){
    // Read the running mode status
    QSignalSpy* spy = new QSignalSpy(deviceDriver, SIGNAL(rxDone()));
    txframe.canId = 5;
    deviceDriver->txData(txframe,0,100);
    while (spy->count() == 0)  QTest::qWait(200);
    QCOMPARE(deviceDriver->rxframe.canId, txframe.canId);
    QCOMPARE(deviceDriver->rxframe.data[0], 1);
    delete spy;
}


/**
*
* @brief Read the Timeout value.
*
* The Test reads the current Timeout value from the implemented Driver:
* the expected  value is the one passed with the txData() function in the test.
*/
void test_canDriverInterface::test_canDriverInterface_6(){
    // Read the timeout
    QSignalSpy* spy = new QSignalSpy(deviceDriver, SIGNAL(rxDone()));
    txframe.canId = 6;
    deviceDriver->txData(txframe,0,0x100);
    while (spy->count() == 0)  QTest::qWait(200);
    QCOMPARE(deviceDriver->rxframe.canId, txframe.canId);
    QCOMPARE(deviceDriver->rxframe.data[0], 0x00);
    QCOMPARE(deviceDriver->rxframe.data[1], 0x01);
    delete spy;
}


/**
*
* @brief No Answer test .
*
* The test verifies the case with no reception from the BUS
*
* In this case the driver shall
* returns a frame with canId == 0.
*/
void test_canDriverInterface::test_canDriverInterface_7(){
    // Read the timeout
    QSignalSpy* spy = new QSignalSpy(deviceDriver, SIGNAL(rxDone()));
    txframe.canId = 7;
    deviceDriver->txData(txframe,0,0x100);
    while (spy->count() == 0)  QTest::qWait(200);
    QCOMPARE(deviceDriver->rxframe.canId, 0);
    delete spy;
}

/**
*
* @brief Driver Stop mode.
*
* The test verifies the stopDriver() command.
* This function shall keep the driver alive but in a non communicating status.
*
* The test verify that the driver doesn't communicate after the
* command is used and the hardware inti status flag is still true;
*
* The test then calls the runDriver() method to restore the communication.
*
*/
void test_canDriverInterface::test_canDriverInterface_8(){

    deviceDriver->driver.stopDriver();
    txframe.canId = 2;
    deviceDriver->txData(txframe,0,0x100);
    QSignalSpy* spy = new QSignalSpy(deviceDriver, SIGNAL(rxDone()));
    while (spy->count() == 0)  QTest::qWait(200);
    QCOMPARE(deviceDriver->rxframe.canId, 0);
    delete spy;

    QCOMPARE(deviceDriver->driver.HARDWARE_INI, true);

    deviceDriver->driver.runDriver();
    txframe.canId = 2;
    deviceDriver->txData(txframe,0,0x100);
    spy = new QSignalSpy(deviceDriver, SIGNAL(rxDone()));
    while (spy->count() == 0)  QTest::qWait(200);
    QCOMPARE(deviceDriver->rxframe.canId, 2);
    QCOMPARE(deviceDriver->rxframe.data[0], 0x80);
    QCOMPARE(deviceDriver->rxframe.data[1], 0x05);
    delete spy;
}



QTEST_MAIN(test_canDriverInterface)

#include "tst_testcandriverclass.moc"
