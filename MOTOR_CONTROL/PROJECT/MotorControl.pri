
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
    $${TARGET_SOURCE}/INTERFACE/commands.cpp \
    $${TARGET_SOURCE}/INTERFACE/events.cpp \
    $${TARGET_SOURCE}/NANOTEC/dictionary.cpp \
    $${TARGET_SOURCE}/NANOTEC/pd4.cpp \
    $${TARGET_SOURCE}/NANOTEC/nanoj.cpp \
    $${TARGET_SOURCE}/NANOTEC/cia_standard_proc.cpp \
    $${TARGET_SOURCE}/NANOTEC/pd4_zero_setting.cpp \
    $${TARGET_SOURCE}/NANOTEC/pd4_positioning.cpp \
    $${TARGET_SOURCE}/TRX/trx.cpp \
    $${TARGET_SOURCE}/WINDOW/window.cpp \


HEADERS += \
    $${TARGET_SOURCE}/application.h \
    $${PROJLIB}/APPLICATION_INTERFACE/applicationInterface.h \
    $${PROJLIB}/CAN_CLIENT/canclient.h \
    $${PROJLIB}/APPLOG/applog.h \
    $${TARGET_SOURCE}/INTERFACE/interface.h \
    $${TARGET_SOURCE}/NANOTEC/dictionary.h \
    $${TARGET_SOURCE}/NANOTEC/pd4.h \
    $${TARGET_SOURCE}/NANOTEC/pd4_dictionary.h \
    $${TARGET_SOURCE}/TRX/trx.h \
    $${TARGET_SOURCE}/TRX/nanoj_trx.h \
    $${TARGET_SOURCE}/WINDOW/window.h \


# Aggiunge tutti i path di progetto
INCLUDEPATH += \
    $${PROJLIB}/APPLICATION_INTERFACE \
    $${PROJLIB}/CAN_CLIENT \
    $${PROJLIB}/APPLOG \
    $${TARGET_SOURCE} \
    $${TARGET_SOURCE}/INTERFACE/ \
    $${TARGET_SOURCE}/CANCLIENT/ \
    $${TARGET_SOURCE}/WINDOW/ \
    $${TARGET_SOURCE}/NANOTEC/ \
    $${TARGET_SOURCE}/LIFT/ \
    $${TARGET_SOURCE}/BODY/ \
    $${TARGET_SOURCE}/ARM/ \
    $${TARGET_SOURCE}/TRX/ \
    $${TARGET_SOURCE}/SLIDE/ \




