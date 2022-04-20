
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

#Include supporto per Qt Network (moduli TCP/IP)
QT += network
QT += serialport



# Dichiarazione dei files di traduzione generati da QtLanguage Update tool
TRANSLATIONS += $${TARGET_RESOURCE}/Languages/traduzione_ita.ts \
                $${TARGET_RESOURCE}/Languages/traduzione_spa.ts \
                $${TARGET_RESOURCE}/Languages/traduzione_eng.ts \
                $${TARGET_RESOURCE}/Languages/traduzione_ger.ts \
                $${TARGET_RESOURCE}/Languages/traduzione_rus.ts \
                $${TARGET_RESOURCE}/Languages/traduzione_tur.ts \
                $${TARGET_RESOURCE}/Languages/traduzione_por.ts \
                $${TARGET_RESOURCE}/Languages/traduzione_pol.ts \
                $${TARGET_RESOURCE}/Languages/traduzione_chn.ts \
                $${TARGET_RESOURCE}/Languages/traduzione_fra.ts \
                $${TARGET_RESOURCE}/Languages/traduzione_ltu.ts \

FORMS += \
    $${TARGET_SOURCE}/WINDOWS/errorsWindow.ui \
    $${TARGET_SOURCE}/WINDOWS/startupWindow.ui \

RESOURCES += \
    $${TARGET_RESOURCE}/assets.qrc


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


DISTFILES += \
    $${SYSTEM_DOC}/DOCUMENTATION/SYSTEM/gantry_system_communication_interfaces.h\
    $${SYSTEM_DOC}/DOCUMENTATION/SYSTEM/gantry_system_architecture.h\
    $${SYSTEM_DOC}/DOCUMENTATION/SYSTEM/gantry_system_configuration.h\
    $${SYSTEM_DOC}/DOCUMENTATION/SYSTEM/gantry_system_gui.h\
    $${SYSTEM_DOC}/DOCUMENTATION/SYSTEM/gantry_system_main_page.h\
    $${SYSTEM_DOC}/DOCUMENTATION/DETAILED/MAIN_CPU/maincpu_software_detailed_documentation.h\
    $${SYSTEM_DOC}/DOCUMENTATION/DETAILED/INTERFACES/CANOPEN_DEVICE_INTERFACE/arm_rotation_protocol_interface.h\
    $${SYSTEM_DOC}/DOCUMENTATION/DETAILED/INTERFACES/CANOPEN_DEVICE_INTERFACE/arm_tilt_protocol_interface.h\
    $${SYSTEM_DOC}/DOCUMENTATION/DETAILED/INTERFACES/CANOPEN_DEVICE_INTERFACE/arm_updown_protocol_interface.h\
    $${SYSTEM_DOC}/DOCUMENTATION/DETAILED/INTERFACES/CANOPEN_DEVICE_INTERFACE/collimator_interface.h\
    $${SYSTEM_DOC}/DOCUMENTATION/DETAILED/INTERFACES/CANOPEN_DEVICE_INTERFACE/compressor_interface.h\
    $${SYSTEM_DOC}/DOCUMENTATION/DETAILED/INTERFACES/CANOPEN_DEVICE_INTERFACE/device_canopen_protocol_interface.h\
    $${SYSTEM_DOC}/DOCUMENTATION/DETAILED/INTERFACES/CANOPEN_DEVICE_INTERFACE/powerservice_interface.h\
    $${SYSTEM_DOC}/DOCUMENTATION/DETAILED/INTERFACES/SERVICE_INTERFACE/gantry_service_communication_interface.h\
    $${SYSTEM_DOC}/DOCUMENTATION/DETAILED/INTERFACES/IRS_INTERFACE/irs_communication_interface.h\
    $${SYSTEM_DOC}/DOCUMENTATION/DETAILED/INTERFACES/AWS_INTERFACE/aws_interface_main_page.h\
    $${SYSTEM_DOC}/DOCUMENTATION/DETAILED/INTERFACES/AWS_INTERFACE/aws_interface_general_description.h\
    $${SYSTEM_DOC}/DOCUMENTATION/DETAILED/INTERFACES/AWS_INTERFACE/aws_interface_commands.h\
    $${SYSTEM_DOC}/DOCUMENTATION/DETAILED/INTERFACES/AWS_INTERFACE/aws_interface_status.h\


SOURCES += \
    $${TARGET_SOURCE}/SIMULATORS/candriversimulator.cpp \
    $${TARGET_SOURCE}/main.cpp \
    $${TARGET_SOURCE}/systemconfig.cpp \
    $${TARGET_SOURCE}/WINDOWS/errorsWindow.cpp \
    $${TARGET_SOURCE}/WINDOWS/startupWindow.cpp \
    $${TARGET_SOURCE}/LIB/awsinterface.cpp \
    $${TARGET_SOURCE}/LIB/commandprotocol.cpp \
    $${TARGET_SOURCE}/LIB/configfile.cpp \
    $${TARGET_SOURCE}/LIB/tcpipserver.cpp \
    $${TARGET_SOURCE}/LIB/tcpipclient.cpp \
    $${TARGET_SOURCE}/LIB/translator.cpp \
    $${TARGET_SOURCE}/LIB/windows.cpp \
    $${TARGET_SOURCE}/LIB/canopeninterface.cpp \
    $${TARGET_SOURCE}/LIB/CanDriverClassInterface.cpp \
    $${TARGET_SOURCE}/COMMUNICATION/awscommunication.cpp \
    $${TARGET_SOURCE}/DRIVERS/TITAN/titanCanDriver.cpp\


HEADERS += \
    $${TARGET_SOURCE}/SIMULATORS/candriversimulator.h \
    $${TARGET_SOURCE}/SIMULATORS/compressorDeviceSimulator.h \
    $${TARGET_SOURCE}/systemconfig.h \
    $${TARGET_SOURCE}/LIB/awsinterface.h \
    $${TARGET_SOURCE}/LIB/errorTable.h \
    $${TARGET_SOURCE}/WINDOWS/errorsWindow.h \
    $${TARGET_SOURCE}/WINDOWS/startupWindow.h \
    $${TARGET_SOURCE}/LIB/commandprotocol.h \
    $${TARGET_SOURCE}/LIB/configfile.h \
    $${TARGET_SOURCE}/LIB/tcpipserver.h \
    $${TARGET_SOURCE}/LIB/tcpipclient.h \
    $${TARGET_SOURCE}/LIB/translator.h \
    $${TARGET_SOURCE}/LIB/windows.h \
    $${TARGET_SOURCE}/LIB/canDriverClassInterface.h \
    $${TARGET_SOURCE}/LIB/canopeninterface.h \
    $${TARGET_SOURCE}/COMMUNICATION/awscommunication.h \
    $${TARGET_SOURCE}/DRIVERS/TITAN/titanCanDriver.h\


# Aggiunge tutti i path di progetto
INCLUDEPATH += \
    $${TARGET_SOURCE}/LIB \
    $${TARGET_SOURCE} \
    $${TARGET_SOURCE}/SERVICE \
    $${TARGET_SOURCE}/DEVICES \
    $${TARGET_SOURCE}/WINDOWS \
    $${TARGET_SOURCE}/DRIVERS/TITAN \
    $${TARGET_SOURCE}/COMMUNICATION \
    $${TARGET_SOURCE}/SIMULATORS \




