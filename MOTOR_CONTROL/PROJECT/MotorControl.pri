
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
    $${TARGET_SOURCE}/INTERFACE/server.cpp \
    $${TARGET_SOURCE}/CANCLIENT/canclient.cpp \    
    $${TARGET_SOURCE}/CANOPEN/dictionary.cpp \
    $${TARGET_SOURCE}/NANOTEC/pd4.cpp \
    $${TARGET_SOURCE}/TRX/trx.cpp \
    $${TARGET_SOURCE}/WINDOW/window.cpp \



HEADERS += \
    $${TARGET_SOURCE}/application.h \
    $${TARGET_SOURCE}/INTERFACE/server.h \
    $${TARGET_SOURCE}/CANCLIENT/canclient.h \
    $${TARGET_SOURCE}/CANOPEN/dictionary.h \
    $${TARGET_SOURCE}/NANOTEC/pd4.h \
    $${TARGET_SOURCE}/NANOTEC/pd4_dictionary.h \
    $${TARGET_SOURCE}/TRX/trx.h \
    $${TARGET_SOURCE}/WINDOW/window.h \



# Aggiunge tutti i path di progetto
INCLUDEPATH += \
    #$${MAIN_CPU}/LIB \
    $${TARGET_SOURCE} \
    $${TARGET_SOURCE}/INTERFACE/ \
    $${TARGET_SOURCE}/CANCLIENT/ \
    $${TARGET_SOURCE}/WINDOW/ \
    $${TARGET_SOURCE}/CANOPEN/ \
    $${TARGET_SOURCE}/NANOTEC/ \
    $${TARGET_SOURCE}/LIFT/ \
    $${TARGET_SOURCE}/BODY/ \
    $${TARGET_SOURCE}/ARM/ \
    $${TARGET_SOURCE}/TRX/ \
    $${TARGET_SOURCE}/SLIDE/ \




