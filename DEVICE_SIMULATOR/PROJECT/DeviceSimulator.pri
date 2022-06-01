
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

#Include supporto per Qt Network (moduli TCP/IP)
QT += network




FORMS += \
    $${TARGET_SOURCE}/startupWindow.ui \

RESOURCES += \
    $${TARGET_RESOURCE}/assets.qrc


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


DISTFILES +=


SOURCES += \
    $${TARGET_SOURCE}/main.cpp \
    $${TARGET_SOURCE}/startupWindow.cpp \
    $${TARGET_SOURCE}/device.cpp \
    $${TARGET_SOURCE}/power_service.cpp \
    $${MAIN_CPU}/LIB/tcpipserver.cpp \
    $${MAIN_CPU}/LIB/tcpipclient.cpp \


HEADERS += \
    $${TARGET_SOURCE}/startupWindow.h \
    $${TARGET_SOURCE}/device.h \
    $${TARGET_SOURCE}/power_service.h \
    $${MAIN_CPU}/LIB/tcpipserver.h \
    $${MAIN_CPU}/LIB/tcpipclient.h \
    


# Aggiunge tutti i path di progetto
INCLUDEPATH += \
    $${MAIN_CPU}/LIB \
    $${TARGET_SOURCE} \




