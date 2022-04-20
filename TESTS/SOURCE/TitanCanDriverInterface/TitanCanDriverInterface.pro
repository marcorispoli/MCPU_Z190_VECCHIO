QT += testlib
QT -= gui
QT += serialport

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app


TARGET_SOURCE = $${PWD}/../../../../MAIN_CPU/SOURCE

SOURCES +=  tst_titandriverclass.cpp\
    $${TARGET_SOURCE}/LIB/CanDriverClassInterface.cpp\
    $${TARGET_SOURCE}/DRIVERS/TITAN/titanCanDriver.cpp\

HEADERS += $${TARGET_SOURCE}/LIB/CanDriverClassInterface.h \
        $${TARGET_SOURCE}/DRIVERS/TITAN/titanCanDriver.h\


INCLUDEPATH += $${TARGET_SOURCE}/LIB/ \
                $${TARGET_SOURCE}/DRIVERS/TITAN \

