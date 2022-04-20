
#include "titanCanDriver.h"

bool titanCanDriver::hardwareInit(QString COM, QString BR){
    // Hardware initialization ..
    this->COM = COM;
    this->BR = BR;
    running = false;

    virtualCom.setPortName(COM);
    virtualCom.setBaudRate(460800);
    virtualCom.setFlowControl(QSerialPort::HardwareControl);

    if(!virtualCom.open(QIODeviceBase::ReadWrite)) return false;

    // Send the software request version code
    virtualCom.write("V\n");
    virtualCom.waitForBytesWritten(100);
    if(!virtualCom.waitForReadyRead(1000)){
        virtualCom.close();
        return false;
    }

    // Get the firmware revision
    versionCode.clear();
    versionCode = virtualCom.readAll();
    while (virtualCom.waitForReadyRead(10)) versionCode += virtualCom.readAll();

    // Reset the device for convenience
    virtualCom.write("RST\n");
    virtualCom.waitForBytesWritten(100);

    int tmo = 50;
    SN.clear();
    while(tmo){
        // Gets the serial number
        virtualCom.write("N\n");
        virtualCom.waitForBytesWritten(100);

        if(!virtualCom.waitForReadyRead(100)){
            tmo--;
            continue;
        }
        SN = virtualCom.readAll();
        break;
    }

    if(!tmo) return false;

    // Gets the error status
    STATUS.clear();
    virtualCom.write("F\n");
    virtualCom.waitForBytesWritten(10);
    if(!virtualCom.waitForReadyRead(10)) return false;
    STATUS = virtualCom.readAll();


    return true;
}
bool titanCanDriver::hardwareShutdown(){
    // Hardware shutdown ...
    virtualCom.close();
    return true;
}

bool titanCanDriver::setStandardFilter(uint addr, uint msk){



    return true;
}



void titanCanDriver::open(void){
    // Activates the communication ..
}
void titanCanDriver::close(void){
    // Pause the communication ..
}

canDriverInterface::canDataFrame titanCanDriver::driverTxRxData(canDataFrame frame, int tmo) {
 /*
    CAN_MSG msg;
    msg.Flags = CAN_FLAGS_STANDARD;
    msg.Id = frame.canId;
    msg.Size = 8;
    for(int i=0; i<8; i++) msg.Data[i] = frame.data[i];

    can_stat = CAN_Write(can_handle , &msg);

    // Send the data frame...

    // Wait the reception from the target node ...

    // Return the received frame
    */
    return frame;
}
