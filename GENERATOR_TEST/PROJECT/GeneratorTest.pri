
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
    $${TARGET_SOURCE}/LIB_SEDECAL/R2CP_Eth.cpp    \
    $${TARGET_SOURCE}/LIB_SEDECAL/CaDataDic.cpp    \
    $${TARGET_SOURCE}/LIB_SEDECAL/GenDataDic/CaDataDicGen.cpp    \
    $${TARGET_SOURCE}/LIB_SEDECAL/GenDataDic/CaDataDicGen_GenIndex.cpp    \
    $${TARGET_SOURCE}/LIB_SEDECAL/GenDataDic/CaDataDicGen_PatientIndex.cpp    \
    $${TARGET_SOURCE}/LIB_SEDECAL/GenDataDic/CaDataDicGen_Network.cpp    \
    $${TARGET_SOURCE}/LIB_SEDECAL/GenDataDic/CaDataDicGen_Service.cpp    \
    $${TARGET_SOURCE}/LIB_SEDECAL/GenDataDic/CaDataDicGen_SystemIndex.cpp    \
    #$${TARGET_SOURCE}/LIB_SEDECAL/GenDataDic/CaDataDicGen_TestIndex.cpp    \
    $${TARGET_SOURCE}/communication.cpp \
    #$${MAIN_CPU}/LIB/tcpipserver.cpp \
    $${MAIN_CPU}/LIB/tcpipclient.cpp \


HEADERS += \
    $${TARGET_SOURCE}/startupWindow.h \
    $${TARGET_SOURCE}/Typedef.h \
    $${TARGET_SOURCE}/LIB_SEDECAL/R2CP_Eth.h    \
    $${TARGET_SOURCE}/LIB_SEDECAL/CaDataDic.h    \
    $${TARGET_SOURCE}/LIB_SEDECAL/GenDataDic/CaDataDicGen.h    \
    $${TARGET_SOURCE}/LIB_SEDECAL/GenDataDic/R2CP_Index.h    \
    $${TARGET_SOURCE}/LIB_SEDECAL/GenDataDic/Interfaces/aDataDicRadInterface.h    \
    $${TARGET_SOURCE}/LIB_SEDECAL/GenDataDic/metInterfaces/metDataDicRadInterface.h    \
    $${TARGET_SOURCE}/communication.h \
    $${MAIN_CPU}/LIB/tcpipclient.h \
    


# Aggiunge tutti i path di progetto
INCLUDEPATH += \
    $${MAIN_CPU}/LIB \
    $${TARGET_SOURCE} \
    $${TARGET_SOURCE}/LIB_SEDECAL \
    $${TARGET_SOURCE}/LIB_SEDECAL/GenDataDic \
    $${TARGET_SOURCE}/LIB_SEDECAL/GenDataDic/Interfaces/ \
    $${TARGET_SOURCE}/LIB_SEDECAL/GenDataDic/metInterfaces/ \




