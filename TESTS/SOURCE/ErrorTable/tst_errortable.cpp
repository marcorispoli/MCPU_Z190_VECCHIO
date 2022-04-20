#include <QtTest>
#include <QCoreApplication>

// add necessary includes here
#include "errorTable.h"

/**
 *  \defgroup ERROR_TABLE_CLASS errorTable class Unit Test module
 *
 *  This module describes the tests on the errorTable class.
 *
 *
 *  # TEST DESCRIPTION
 *
 *  This Test section verifies the class implementation with the following test list:
 *
 *  + ...
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
 */

/**
 * @brief Executing class
 * \see ERROR_TABLE_CLASS
 */

class test_errorTable : public QObject
{
    Q_OBJECT

public:
    test_errorTable();
    ~test_errorTable();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void test_errorTable_1();
    void test_errorTable_2();
    void test_errorTable_3();
    void test_errorTable_4();
    void test_errorTable_5();
private:

};



test_errorTable::test_errorTable()
{

}

test_errorTable::~test_errorTable()
{

}

void test_errorTable::initTestCase()
{


}

void test_errorTable::cleanupTestCase()
{

}

/**
*
* @brief INVALID-FRAME type identification test.
*
* The test verifies the ability to detect a non valid frame.
*/
void test_errorTable::test_errorTable_1()
{
    errorTable table;

    QCOMPARE(table.errorExist("E00101"), true);
    QCOMPARE(table.errorExist("E00102"), true);
    QCOMPARE(table.errorExist("E00103"), true);
    QCOMPARE(table.errorExist("E00104"), true);
    QCOMPARE(table.errorExist("E00105"), true);

    QSignalSpy* spy = new QSignalSpy(&table, SIGNAL(activateErrorSgn(QString)));
    table.activateError("E00101");
    QCOMPARE(spy->count(), 1);
    QCOMPARE(table.isErrorActive("E00101"), true);
    delete spy;


    spy = new QSignalSpy(&table, SIGNAL(clearErrorSgn()));
    table.clearError("E00101");
    QCOMPARE(spy->count(), 1);
    QCOMPARE(table.isErrorActive("E00101"), false);
    delete spy;



}

void test_errorTable::test_errorTable_2()
{
    errorTable table;

    table.activateError("E00101");
    table.activateError("E00102");
    table.activateError("E00103");

    QCOMPARE(table.isErrorActive("E00101"), true);
    QCOMPARE(table.isErrorActive("E00102"), true);
    QCOMPARE(table.isErrorActive("E00103"), true);
    QCOMPARE(table.isAnyErrorActive(), true);

    table.clearError("E00101");
    QCOMPARE(table.isErrorActive("E00101"), false);
    QCOMPARE(table.isErrorActive("E00102"), true);
    QCOMPARE(table.isErrorActive("E00103"), true);
    QCOMPARE(table.isAnyErrorActive(), true);

    table.clearError("E00102");
    QCOMPARE(table.isErrorActive("E00101"), false);
    QCOMPARE(table.isErrorActive("E00102"), false);
    QCOMPARE(table.isErrorActive("E00103"), true);
    QCOMPARE(table.isAnyErrorActive(), true);

    table.clearError("E00103");
    QCOMPARE(table.isErrorActive("E00101"), false);
    QCOMPARE(table.isErrorActive("E00102"), false);
    QCOMPARE(table.isErrorActive("E00103"), false);
    QCOMPARE(table.isAnyErrorActive(), false);


}

void test_errorTable::test_errorTable_3()
{
    errorTable table;


    // __________ ACTIVATION __________________________________________________
    QSignalSpy* spy = new QSignalSpy(&table, SIGNAL(activateErrorSgn(QString)));
    table.activateError("E00101");
    QCOMPARE(spy->count(), 1);
    QCOMPARE(table.isErrorActive("E00101"), true);
    delete spy;

    spy = new QSignalSpy(&table, SIGNAL(activateErrorSgn(QString)));
    table.activateError("E00101");
    QCOMPARE(spy->count(), 0);
    QCOMPARE(table.isErrorActive("E00101"), true);
    delete spy;


    spy = new QSignalSpy(&table, SIGNAL(activateErrorSgn(QString)));
    table.activateError("E00101", errorTable::_ERR_FORCE );
    QCOMPARE(spy->count(), 1);
    QCOMPARE(table.isErrorActive("E00101"), true);
    delete spy;


    // __________ CLEAR __________________________________________________
    spy = new QSignalSpy(&table, SIGNAL(clearErrorSgn()));
    table.clearError("E00101");
    QCOMPARE(spy->count(), 1);
    QCOMPARE(table.isErrorActive("E00101"), false);
    delete spy;

    spy = new QSignalSpy(&table, SIGNAL(clearErrorSgn()));
    table.clearError("E00101");
    QCOMPARE(spy->count(), 0);
    QCOMPARE(table.isErrorActive("E00101"), false);
    delete spy;

    spy = new QSignalSpy(&table, SIGNAL(clearErrorSgn()));
    table.clearError("E00101", errorTable::_ERR_FORCE );
    QCOMPARE(spy->count(), 1);
    QCOMPARE(table.isErrorActive("E00101"), false);
    delete spy;

    // __________ ACTIVATION NO SIGNAL ____________________________________
    spy = new QSignalSpy(&table, SIGNAL(activateErrorSgn(QString)));
    table.activateError("E00101", errorTable::_ERR_NO_SGN);
    QCOMPARE(spy->count(), 0);
    QCOMPARE(table.isErrorActive("E00101"), true);
    delete spy;

    // __________ CLEAR NO SIGNAL__________________________________________
    spy = new QSignalSpy(&table, SIGNAL(clearErrorSgn()));
    table.clearError("E00101", errorTable::_ERR_NO_SGN );
    QCOMPARE(spy->count(), 0);
    QCOMPARE(table.isErrorActive("E00101"), false);
    delete spy;

}

void test_errorTable::test_errorTable_4()
{
    errorTable table;

    table.activateError("E00101");
    table.activateError("E00102");
    table.activateError("E00103", errorTable::_ERR_AUTOEXPIRE);

    QCOMPARE(table.isErrorActive("E00101"), true);
    QCOMPARE(table.isErrorActive("E00102"), true);
    QCOMPARE(table.isErrorActive("E00103"), true);
    QCOMPARE(table.isAnyErrorActive(), true);

    table.clearAutoExpireErrors();
    QCOMPARE(table.isErrorActive("E00101"), true);
    QCOMPARE(table.isErrorActive("E00102"), true);
    QCOMPARE(table.isErrorActive("E00103"), false);
    QCOMPARE(table.isAnyErrorActive(), true);

    table.activateError("E00103");
    QCOMPARE(table.isErrorActive("E00101"), true);
    QCOMPARE(table.isErrorActive("E00102"), true);
    QCOMPARE(table.isErrorActive("E00103"), true);
    QCOMPARE(table.isAnyErrorActive(), true);

}

void test_errorTable::test_errorTable_5()
{
    errorTable table;

    QCOMPARE(table.errorExist("TEST01"), true);
    QCOMPARE(table.errorExist("TEST01"), true);
    QCOMPARE(table.errorExist("TEST03"), true);
    QCOMPARE(table.errorExist("TEST04"), true);


    errorTable::errorIterator i = table.getErrorIterator("TEST01");
    QCOMPARE(table.isValidIterator(i), true);
    QCOMPARE(table.getErrorCode(i), QString("TEST01"));
    QCOMPARE(i.value().getTitle(), QString("TITLE01"));
    QCOMPARE(i.value().getDescription(), QString(" TEST01 DESCRIPTION"));
    QCOMPARE(i.value().getPixmapUID(), QString("//Errors/Errors/errorPix.png"));

    i = table.getErrorIterator("TEST0");
    QCOMPARE(table.isValidIterator(i), false);

    i = table.getErrorIterator("TEST02");
    QCOMPARE(table.isValidIterator(i), true);
    QCOMPARE(table.getErrorCode(i), QString("TEST02"));
    QCOMPARE(i.value().getTitle(), QString("TITLE02"));
    QCOMPARE(i.value().getDescription(), QString(" TEST02 DESCRIPTION \n CONTINUE"));
    QCOMPARE(i.value().getPixmapUID(), QString("//Errors/Errors/errorPix.png"));

    i = table.getErrorIterator("TEST03");
    QCOMPARE(table.isValidIterator(i), true);
    QCOMPARE(table.getErrorCode(i), QString("TEST03"));
    QCOMPARE(i.value().getTitle(), QString("TITLE03"));
    QCOMPARE(i.value().getDescription(), QString(""));
    QCOMPARE(i.value().getPixmapUID(), QString("//Errors/Errors/errorPix.png"));

    i = table.getErrorIterator("TEST04");
    QCOMPARE(table.isValidIterator(i), true);
    QCOMPARE(table.getErrorCode(i), QString("TEST04"));
    QCOMPARE(i.value().getTitle(), QString("TITLE04"));
    QCOMPARE(i.value().getDescription(), QString(""));
    QCOMPARE(i.value().getPixmapUID(), QString("//Errors/Errors/errorPix.png"));

}


QTEST_MAIN(test_errorTable)

#include "tst_errortable.moc"
