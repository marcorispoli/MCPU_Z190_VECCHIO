QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app


TARGET_SOURCE = $${PWD}/../../../../MAIN_CPU/SOURCE

SOURCES +=  tst_testcandriverclass.cpp\
    $${TARGET_SOURCE}/LIB/CanDriverClassInterface.cpp\

HEADERS += $${TARGET_SOURCE}/LIB/CanDriverClassInterface.h \

INCLUDEPATH += $${TARGET_SOURCE}/LIB/
