QT += testlib
QT -= gui

QT += network

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

TARGET_SOURCE = $${PWD}/../../../APPLICATION/SOURCE

SOURCES += \
tst_awsinterface.cpp\
$${TARGET_SOURCE}/LIB/commandprotocol.cpp\
$${TARGET_SOURCE}/LIB/tcpipserver.cpp\
$${TARGET_SOURCE}/LIB/tcpipclient.cpp\
$${TARGET_SOURCE}/COMMUNICATION/AWS/awsinterface.cpp\

HEADERS += \
$${TARGET_SOURCE}/LIB/commandprotocol.h\
$${TARGET_SOURCE}/LIB/tcpipserver.h\
$${TARGET_SOURCE}/LIB/tcpipclient.h\
$${TARGET_SOURCE}/COMMUNICATION/AWS/awsinterface.h\


INCLUDEPATH += \
$${TARGET_SOURCE}/LIB/
$${TARGET_SOURCE}/COMMUNICATION/AWS/
