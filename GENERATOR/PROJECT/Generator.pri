
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

#Include supporto per Qt Network (moduli TCP/IP)
QT += network
QT += sql




FORMS += \
    $${TARGET_SOURCE}/WINDOW/startupWindow.ui \

RESOURCES += \
    $${TARGET_RESOURCE}/assets.qrc


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


DISTFILES +=


SOURCES += \
    $${PROJLIB}/APPLICATION_INTERFACE/applicationInterface.cpp \
    $${TARGET_SOURCE}/main.cpp \
    $${TARGET_SOURCE}/WORKFLOW/workflow.cpp \
    $${TARGET_SOURCE}/EXPOSURE/2D_exposure.cpp \
    $${TARGET_SOURCE}/EXPOSURE/2D_AEC_exposure.cpp \
    $${TARGET_SOURCE}/EXPOSURE/3D_exposure.cpp \
    $${TARGET_SOURCE}/EXPOSURE/3D_AEC_exposure.cpp \
    $${TARGET_SOURCE}/INTERFACE/interface.cpp \
    $${TARGET_SOURCE}/INTERFACE/events.cpp \
    $${TARGET_SOURCE}/INTERFACE/commands.cpp \
    $${TARGET_SOURCE}/COMMUNICATION/communication.cpp \
    $${TARGET_SOURCE}/COMMUNICATION/tcpipclient.cpp \
    $${TARGET_SOURCE}/LIB_SEDECAL/R2CP_Eth.cpp    \
    $${TARGET_SOURCE}/LIB_SEDECAL/CaDataDic.cpp    \
    $${TARGET_SOURCE}/LIB_SEDECAL/GenDataDic/CaDataDicGen.cpp    \
    $${TARGET_SOURCE}/LIB_SEDECAL/GenDataDic/CaDataDicGen_GenIndex.cpp    \
    $${TARGET_SOURCE}/LIB_SEDECAL/GenDataDic/CaDataDicGen_PatientIndex.cpp    \
    $${TARGET_SOURCE}/LIB_SEDECAL/GenDataDic/CaDataDicGen_Network.cpp    \
    $${TARGET_SOURCE}/LIB_SEDECAL/GenDataDic/CaDataDicGen_Service.cpp    \
    $${TARGET_SOURCE}/LIB_SEDECAL/GenDataDic/CaDataDicGen_SystemIndex.cpp    \
    $${TARGET_SOURCE}/WINDOW/startupWindow.cpp \



HEADERS += \
    $${PROJLIB}/APPLICATION_INTERFACE/applicationInterface.h \
    $${TARGET_SOURCE}/application.h \
    $${TARGET_SOURCE}/Typedef.h \
    $${TARGET_SOURCE}/WORKFLOW/workflow.h \
    $${TARGET_SOURCE}/INTERFACE/interface.h \
    $${TARGET_SOURCE}/EXPOSURE/exposure.h \
    $${TARGET_SOURCE}/COMMUNICATION/communication.h \
    $${TARGET_SOURCE}/COMMUNICATION/tcpipclient.h \
    $${TARGET_SOURCE}/LIB_SEDECAL/R2CP_Eth.h    \
    $${TARGET_SOURCE}/LIB_SEDECAL/CaDataDic.h    \
    $${TARGET_SOURCE}/LIB_SEDECAL/GenDataDic/CaDataDicGen.h    \
    $${TARGET_SOURCE}/LIB_SEDECAL/GenDataDic/R2CP_Index.h    \
    $${TARGET_SOURCE}/LIB_SEDECAL/GenDataDic/Interfaces/aDataDicRadInterface.h    \
    $${TARGET_SOURCE}/WINDOW/startupWindow.h \



# Aggiunge tutti i path di progetto
INCLUDEPATH += \
    $${PROJLIB}/APPLICATION_INTERFACE \
    $${TARGET_SOURCE} \
    $${TARGET_SOURCE}/INTERFACE/ \
    $${TARGET_SOURCE}/EXPOSURE/ \
    $${TARGET_SOURCE}/WORKFLOW/ \
    $${TARGET_SOURCE}/COMMUNICATION/ \
    $${TARGET_SOURCE}/WINDOW/ \
    $${TARGET_SOURCE}/LIB_SEDECAL \
    $${TARGET_SOURCE}/LIB_SEDECAL/GenDataDic \




