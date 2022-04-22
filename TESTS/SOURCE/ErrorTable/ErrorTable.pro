QT += widgets
QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

TARGET_SOURCE = $${PWD}/../../../APPLICATION/SOURCE

SOURCES += \
tst_errortable.cpp\

HEADERS += \
$${TARGET_SOURCE}/LIB/errorTable.h


INCLUDEPATH += \
$${TARGET_SOURCE}/LIB/
