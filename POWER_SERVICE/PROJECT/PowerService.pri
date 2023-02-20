
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

#Include supporto per Qt Network (moduli TCP/IP)
QT += network

FORMS += \
    $${TARGET_SOURCE}/WINDOW/window.ui \

RESOURCES += \
    $${TARGET_RESOURCE}/assets.qrc


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=

SOURCES += \
    $${TARGET_SOURCE}/main.cpp \
    $${PROJLIB}/APPLICATION_INTERFACE/applicationInterface.cpp \
    $${PROJLIB}/CAN_CLIENT/canclient.cpp \
    $${PROJLIB}/APPLOG/applog.cpp \
    $${TARGET_SOURCE}/INTERFACE/interface.cpp \
    $${TARGET_SOURCE}/PROTOCOL/can_device_protocol.cpp \
    $${TARGET_SOURCE}/PROTOCOL/device_protocol.cpp \
    $${TARGET_SOURCE}/WINDOW/window.cpp \


HEADERS += \
    $${TARGET_SOURCE}/application.h \
    $${PROJLIB}/APPLICATION_INTERFACE/applicationInterface.h \
    $${PROJLIB}/CAN_CLIENT/canclient.h \
    $${PROJLIB}/APPLOG/applog.h \
    $${TARGET_SOURCE}/INTERFACE/interface.h \
    $${TARGET_SOURCE}/PROTOCOL/can_device_protocol.h \
    $${TARGET_SOURCE}/PROTOCOL/device_protocol.h \
    $${TARGET_SOURCE}/WINDOW/window.h \



# Aggiunge tutti i path di progetto
INCLUDEPATH += \
    $${PROJLIB}/APPLICATION_INTERFACE \
    $${PROJLIB}/CAN_CLIENT \
    $${PROJLIB}/APPLOG \
    $${TARGET_SOURCE} \
    $${TARGET_SOURCE}/INTERFACE/ \
    $${TARGET_SOURCE}/PROTOCOL/ \
    $${TARGET_SOURCE}/WINDOW/ \


