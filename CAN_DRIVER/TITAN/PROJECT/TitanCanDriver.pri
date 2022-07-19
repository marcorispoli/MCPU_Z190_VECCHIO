
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

#Include supporto per Qt Network (moduli TCP/IP)
QT += network
QT += serialport

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


DISTFILES +=


SOURCES += \
    $${TARGET_SOURCE}/main.cpp \    
    $${TARGET_SOURCE}/server.cpp \
    $${TARGET_SOURCE}/can_driver.cpp \
    $${MAIN_CPU}/LIB/tcpipserver.cpp \
    $${MAIN_CPU}/LIB/configfile.cpp \


HEADERS += \
    $${TARGET_SOURCE}/configuration.h \
    $${TARGET_SOURCE}/server.h \
    $${TARGET_SOURCE}/can_driver.h \
    $${MAIN_CPU}/LIB/tcpipserver.h \
    $${MAIN_CPU}/LIB/configfile.h \


# Aggiunge tutti i path di progetto
INCLUDEPATH += \
    $${MAIN_CPU}/LIB \
    $${TARGET_SOURCE} \




