#include <QtTest>
#include <QCoreApplication>

// add necessary includes here
#include "canopeninterface.h"

/**
 *  \defgroup CANOPEN_INTERFACE_CLASS_MODULE canOpenInterface class Unit Test module
 *
 *  This module describes the tests on the canOpenInterface class.
 *
 *
 *  # TEST DESCRIPTION
 *
 *  This Test section verifies the class implementation with the following test list:
 *
 *  + test_canOpenInterface::initTestCase();
 *  + test_canOpenInterface::cleanupTestCase();
 *  + test_canOpenInterface::test_odRegister_1();
 *  + test_canOpenInterface::test_odRegister_2();
 *
 *  ## odRegister class, Object Register data access test
 *  **Function**: test_canOpenInterface::test_odRegister_1() \n
 *  **Description**: This is a multi test sequence, testing the data access features
 *
 *  **Execution**:
 *  -# Create an Object Register with known data content and 8 bit data register
 *   + Verify that getNodeId() returns the correct node Id;
 *   + Verify that getIdx() returns the correct Idx field;
 *   + Verify that getSubIdx() returns the correct SubIdx field;
 *   + Verify that getError() returns the correct Error field;
 *   + Verify that getData<int>() returns the correct Data field;
 *
 *  -# Create an Object Register with known data content and 8 bit data register
 *  -# Set a 16 bit data content;
 *   + Verify that getData<int>() returns a 8 bit value;
 *   + Verify that getError() returns the correct Error field;
 *   + Verify that getNodeId() returns the correct node Id;
 *   + Verify that getIdx() returns the correct Idx field;
 *   + Verify that getSubIdx() returns the correct SubIdx field;

 *  -# Create an Object Register with known data content and 16 bit data register
 *  -# Set a 24 bit data content;
 *   + Verify that getData<int>() returns a 16 bit value;
 *   + Verify that getError() returns the correct Error field;
 *   + Verify that getNodeId() returns the correct node Id;
 *   + Verify that getIdx() returns the correct Idx field;
 *   + Verify that getSubIdx() returns the correct SubIdx field;
 *
 *  -# Create an Object Register with known data content and 24 bit data register
 *  -# Set a 24 bit data content;
 *   + Verify that getData<int>() returns the correct Data field;
 *   + Verify that getError() returns the correct Error field;
 *   + Verify that getNodeId() returns the correct node Id;
 *   + Verify that getIdx() returns the correct Idx field;
 *   + Verify that getSubIdx() returns the correct SubIdx field;
 *
 *  -# Create an Object Register with known data content and 24 bit data register
 *  -# Set a 32 bit data content;
 *   + Verify that getData<int>() returns a 24 bit value;
 *   + Verify that getError() returns the correct Error field;
 *   + Verify that getNodeId() returns the correct node Id;
 *   + Verify that getIdx() returns the correct Idx field;
 *   + Verify that getSubIdx() returns the correct SubIdx field;
 *
 *  -# Create an Object Register with known data content and 32 bit data register
 *  -# Set a 32 bit data content;
 *   + Verify that getData<int>() returns a 32 bit value;
 *   + Verify that getError() returns the correct Error field;
 *   + Verify that getNodeId() returns the correct node Id;
 *   + Verify that getIdx() returns the correct Idx field;
 *   + Verify that getSubIdx() returns the correct SubIdx field;
 *
 *
 *  ## odRegister class, canDataFrame conversion features test
 *  **Function**: test_canOpenInterface::test_odRegister_2() \n
 *  **Description**: This is a multi test sequence, testing the canDataFrame format conversion features.
 *
 *  **Execution**:
 *
 *  -# Create an Object Register with known data content and 8 bit data register;
 *  -# Get the canDataFrame frame from the oRegister::toReadCanFrame() method;
 *   + Verify that frame::canId is correct;
 *   + Verify that frame.data[0] .. to frame.data[7] are correct;
 *
 *  -# Create an Object Register with known data content and 8 bit data register;
 *  -# Get the canDataFrame frame from the oRegister::toWriteCanFrame() method;
 *   + Verify that frame::canId is correct;
 *   + Verify that frame.data[0] .. to frame.data[7] are correct;
 *
 *  -# Create an Object Register with known data content and 32 bit data register;
 *  -# set a 32 bit data content;
 *  -# Get the canDataFrame frame from the oRegister::toReadCanFrame() method;
 *   + Verify that frame::canId is correct;
 *   + Verify that frame.data[0] .. to frame.data[7] are correct;
 *
 *  -# Create an Object Register with known data content and 32 bit data register;
 *  -# set a 32 bit data content;
 *  -# Get the canDataFrame frame from the oRegister::toWriteCanFrame() method;
 *   + Verify that frame::canId is correct;
 *   + Verify that frame.data[0] .. to frame.data[7] are correct;
 *
 *
 *  -# Create an Object Register with known data content and 32 bit data register;
 *  -# set a 32 bit data content;
 *  -# Get the canDataFrame frame from the oRegister::toReadCanFrame() method;
 *  -# Set the frame.data[0] to be a 32 bit data read answer frame
 *   + Verify that odRegister::isReadFrame() is true;
 *   + Verify that odRegister::isWriteFrame() is false;
 *   + Verify that odRegister::isErrorFrame() is false;
 *   + Verify that odRegister::getCanId(frame) is correct;
 *   + Verify that odRegister::getIdx(frame) is correct;
 *   + Verify that odRegister::getSubIdx(frame) is correct;
 *
 *
 *  -# Create an Object Register with known data content and 32 bit data register;
 *  -# set a 32 bit data content;
 *  -# Get the canDataFrame frame from the oRegister::toWriteCanFrame() method;
 *  -# Set the frame.data[0] to be write request answer frame;
 *   + Verify that odRegister::isReadFrame() is false;
 *   + Verify that odRegister::isWriteFrame() is true;
 *   + Verify that odRegister::isErrorFrame() is false;
 *   + Verify that odRegister::getCanId(frame) is correct;
 *   + Verify that odRegister::getIdx(frame) is correct;
 *   + Verify that odRegister::getSubIdx(frame) is correct;
 *
 *  -# Create an Object Register with known data content and 32 bit data register;
 *  -# set a 32 bit data content;
 *  -# Get the canDataFrame frame from the oRegister::toWriteCanFrame() method;
 *  -# Set the frame.data[0] to be a received error frame;
 *   + Verify that odRegister::isReadFrame() is false;
 *   + Verify that odRegister::isWriteFrame() is false;
 *   + Verify that odRegister::isErrorFrame() is true;
 *   + Verify that odRegister::getCanId(frame) is correct;
 *   + Verify that odRegister::getIdx(frame) is correct;
 *   + Verify that odRegister::getSubIdx(frame) is correct;
 *
 *
 */

/**
 * @brief Executing class
 * \see CANOPEN_INTERFACE_CLASS_MODULE
 */
class test_canOpenInterface : public QObject
{
    Q_OBJECT

public:
    test_canOpenInterface();
    ~test_canOpenInterface();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void test_odRegister_1();
    void test_odRegister_2();
    //void test_canopen_readRegister();

};

test_canOpenInterface::test_canOpenInterface()
{

}

test_canOpenInterface::~test_canOpenInterface()
{

}

void test_canOpenInterface::initTestCase()
{

}

void test_canOpenInterface::cleanupTestCase()
{

}

/**
 * @brief odRegister class, Object Register data access
 *
 * This is a multi test sequence, testing
 * the data access feature:
 * + Data type formatting (8 to 32 bit dtaa types);
 * + class Data content access;
 */
void test_canOpenInterface::test_odRegister_1()
{
    odRegister od(40, 0x1000, 0x20, odRegister::_odDataType::_8BIT_DATA);
    QCOMPARE(od.getNodeId(), 40);
    QCOMPARE(od.getIdx(), 0x1000);
    QCOMPARE(od.getSubIdx(), 0x20);
    QCOMPARE(od.getError(), (unsigned char) 0);
    QCOMPARE(od.getData<int>(), 0);

    od = odRegister(40, 0x1000, 0x20, odRegister::_odDataType::_8BIT_DATA);
    od.setData<int>(1000);
    QCOMPARE(od.getData<int>(), (int) 232);
    QCOMPARE(od.getError(), (unsigned char)0);
    QCOMPARE(od.getNodeId(), 40);
    QCOMPARE(od.getIdx(), 0x1000);
    QCOMPARE(od.getSubIdx(), 0x20);

    od = odRegister(40, 0x1000, 0x20, odRegister::_odDataType::_16BIT_DATA);
    od.setData<int>(100000);

    QCOMPARE(od.getData<int>(), (int) 0x86A0);
    QCOMPARE(od.getError(), (unsigned char)0);
    QCOMPARE(od.getNodeId(), 40);
    QCOMPARE(od.getIdx(), 0x1000);
    QCOMPARE(od.getSubIdx(), 0x20);

    od = odRegister(40, 0x1000, 0x20, odRegister::_odDataType::_24BIT_DATA);
    od.setData<int>(100000);
    QCOMPARE(od.getData<int>(), (int) 0x0186A0);
    QCOMPARE(od.getError(), (unsigned char)0);
    QCOMPARE(od.getNodeId(), 40);
    QCOMPARE(od.getIdx(), 0x1000);
    QCOMPARE(od.getSubIdx(), 0x20);

    od = odRegister(40, 0x1000, 0x20, odRegister::_odDataType::_24BIT_DATA);
    od.setData<int>(0x01010101);
    QCOMPARE(od.getData<int>(), (int) 0x010101);
    QCOMPARE(od.getError(), (unsigned char)0);
    QCOMPARE(od.getNodeId(), 40);
    QCOMPARE(od.getIdx(), 0x1000);
    QCOMPARE(od.getSubIdx(), 0x20);

    od = odRegister(40, 0x1000, 0x20, odRegister::_odDataType::_32BIT_DATA);
    od.setData<int>(0x01010101);
    QCOMPARE(od.getData<int>(), (int) 0x01010101);
    QCOMPARE(od.getError(), (unsigned char)0);
    QCOMPARE(od.getNodeId(), 40);
    QCOMPARE(od.getIdx(), 0x1000);
    QCOMPARE(od.getSubIdx(), 0x20);


}


/**
 * @brief odRegister class, canDataFrame conversion features
 *
 * This is a multi test sequence, testing the class to properly convert
 * its content in a correct canDataFrame format.
 */
void test_canOpenInterface::test_odRegister_2()
{
    odRegister od(40, 0x1000, 0x20, odRegister::_odDataType::_8BIT_DATA);
    canDriverInterface::canDataFrame frame = od.toReadCanFrame();

    QCOMPARE(frame.canId, 40 + 0x600);
    QCOMPARE(frame.data[0], 0x40);
    QCOMPARE(frame.data[1], 0x00);
    QCOMPARE(frame.data[2], 0x10);
    QCOMPARE(frame.data[3], 0x20);
    QCOMPARE(frame.data[4], (unsigned char) 0);
    QCOMPARE(frame.data[5], (unsigned char) 0);
    QCOMPARE(frame.data[6], (unsigned char) 0);
    QCOMPARE(frame.data[7], (unsigned char) 0);

    od = odRegister(40, 0x1000, 0x20, odRegister::_odDataType::_8BIT_DATA);
    frame = od.toWriteCanFrame();

    QCOMPARE(frame.canId, 40 + 0x600);
    QCOMPARE(frame.data[0], odRegister::_odFrameType::_8BIT_WR);
    QCOMPARE(frame.data[1], 0x00);
    QCOMPARE(frame.data[2], 0x10);
    QCOMPARE(frame.data[3], 0x20);
    QCOMPARE(frame.data[4], (unsigned char) 0);
    QCOMPARE(frame.data[5], (unsigned char) 0);
    QCOMPARE(frame.data[6], (unsigned char) 0);
    QCOMPARE(frame.data[7], (unsigned char) 0);


    od = odRegister(40, 0x1000, 0x20, odRegister::_odDataType::_32BIT_DATA);
    od.setData<int>(0x04030201);
    frame = od.toReadCanFrame();

    QCOMPARE(frame.canId, 40 + 0x600);
    QCOMPARE(frame.data[0], 0x40);
    QCOMPARE(frame.data[1], 0x00);
    QCOMPARE(frame.data[2], 0x10);
    QCOMPARE(frame.data[3], 0x20);
    QCOMPARE(frame.data[4], (unsigned char) 0x01);
    QCOMPARE(frame.data[5], (unsigned char) 0x02);
    QCOMPARE(frame.data[6], (unsigned char) 0x03);
    QCOMPARE(frame.data[7], (unsigned char) 0x04);

    od = odRegister(40, 0x1000, 0x20, odRegister::_odDataType::_32BIT_DATA);
    od.setData<int>(0x04030201);
    frame = od.toWriteCanFrame();

    QCOMPARE(frame.canId, 40 + 0x600);
    QCOMPARE(frame.data[0], odRegister::_odFrameType::_32BIT_WR);
    QCOMPARE(frame.data[1], 0x00);
    QCOMPARE(frame.data[2], 0x10);
    QCOMPARE(frame.data[3], 0x20);
    QCOMPARE(frame.data[4], (unsigned char) 0x01);
    QCOMPARE(frame.data[5], (unsigned char) 0x02);
    QCOMPARE(frame.data[6], (unsigned char) 0x03);
    QCOMPARE(frame.data[7], (unsigned char) 0x04);



    od = odRegister(40, 0x1000, 0x20, odRegister::_odDataType::_32BIT_DATA);
    od.setData<int>(0x04030201);
    frame = od.toReadCanFrame();
    frame.data[0] = odRegister::_odFrameType::_32BIT_RD;
    QCOMPARE(odRegister::isReadFrame(frame), true);
    QCOMPARE(odRegister::isWriteFrame(frame), false);
    QCOMPARE(odRegister::isErrorFrame(frame), false);
    QCOMPARE(odRegister::getCanId(frame), 0x600 + 40);
    QCOMPARE(odRegister::getIdx(frame), 0x1000);
    QCOMPARE(odRegister::getSubIdx(frame), 0x20);

    od = odRegister(40, 0x1000, 0x20, odRegister::_odDataType::_32BIT_DATA);
    od.setData<int>(0x04030201);
    frame = od.toWriteCanFrame();
    frame.data[0] = 0x60;
    QCOMPARE(odRegister::isReadFrame(frame), false);
    QCOMPARE(odRegister::isWriteFrame(frame), true);
    QCOMPARE(odRegister::isErrorFrame(frame), false);
    QCOMPARE(odRegister::getCanId(frame), 0x600 + 40);
    QCOMPARE(odRegister::getIdx(frame), 0x1000);
    QCOMPARE(odRegister::getSubIdx(frame), 0x20);

    od = odRegister(40, 0x1000, 0x20, odRegister::_odDataType::_32BIT_DATA);
    od.setData<int>(0x04030201);
    frame = od.toWriteCanFrame();
    frame.data[0] = odRegister::_ERROR_FRAME;
    QCOMPARE(odRegister::isReadFrame(frame), false);
    QCOMPARE(odRegister::isWriteFrame(frame), false);
    QCOMPARE(odRegister::isErrorFrame(frame), true);
    QCOMPARE(odRegister::getCanId(frame), 0x600 + 40);
    QCOMPARE(odRegister::getIdx(frame), 0x1000);
    QCOMPARE(odRegister::getSubIdx(frame), 0x20);

}


QTEST_MAIN(test_canOpenInterface)

#include "tst_testcanopeninterface.moc"
