#include "can_driver.h"
#include <QApplication>

titanCanDriver::titanCanDriver(){


    virtualCom = nullptr;

    CANID.append((uchar) 0);
    CANID.append((uchar) 0);
    CANID.append((uchar) 0);
    BDATA.append((uchar) 0);
    BDATA.append((uchar) 0);
    rx_frame.clear();
    rx_index = 0;
    canRxTimer=0;
}

bool titanCanDriver::driverOpen(QByteArray COM, uchar BR, uint address, uint mask, QByteArray mode){

    if(virtualCom == nullptr)  virtualCom = new QSerialPort();

    QString Baud;
    switch(BR){
    case _CAN_1000K:
        Baud  = "1Mb/s";
        break;
    case _CAN_500K:
        Baud  = "500Kb/s";
        break;
    case _CAN_250K:
        Baud  = "250Kb/s";
        break;
    case _CAN_100K:
        Baud  = "100Kb/s";
        break;
    }

    QString stringa = QString("TITAN CAN DRIVER SETUP: COM PORT=%1, BAUD:%2, MODE:%3 ").arg(COM).arg(Baud).arg(mode);
    qDebug() << stringa;

    // Hardware initialization ..
    this->openMode = mode;

    // Open the Virtual COM
    virtualCom->setPortName(COM);

    //virtualCom->setBaudRate(460800);
    virtualCom->setFlowControl(QSerialPort::HardwareControl);
    if(!virtualCom->open(QIODeviceBase::ReadWrite)) return false;

    // Send the software request version code
    virtualCom->write("V\r");
    virtualCom->waitForBytesWritten(100);
    if(!virtualCom->waitForReadyRead(1000)){
        virtualCom->close();
        return false;
    }

    // Get the firmware revision
    versionCode.clear();
    versionCode = virtualCom->readAll();
    while (virtualCom->waitForReadyRead(10)) versionCode += virtualCom->readAll();

    // Reset the device for convenience
    virtualCom->write("RST\r");
    virtualCom->waitForBytesWritten(100);

    // Gets the serial number
    int tmo = 50;
    SN.clear();
    while(tmo){
        virtualCom->write("N\r");
        virtualCom->waitForBytesWritten(100);
        if(!virtualCom->waitForReadyRead(100)){
            tmo--;
            continue;
        }
        SN = virtualCom->readAll();
        break;
    }

    if(!tmo){
        virtualCom->close();
        return false;
    }

    // Gets the error status
    STATUS.clear();
    virtualCom->write("F\r");
    virtualCom->waitForBytesWritten(10);
    if(!virtualCom->waitForReadyRead(10)){
        virtualCom->close();
        return false;
    }
    STATUS = virtualCom->readAll();

    // Set the CAN BAUD RATE
    QByteArray command;
    switch(BR){
    case _CAN_1000K:
        command = "S8\r";
        break;
    case _CAN_500K:
        command = "S6\r";
        break;
    case _CAN_250K:
        command = "S5\r";
        break;
    case _CAN_100K:
        command = "S3\r";
        break;
    }

    virtualCom->write(command.data());
    virtualCom->waitForBytesWritten(10);
    if(!virtualCom->waitForReadyRead(10)){
        virtualCom->close();
        return false;
    }

    rx_frame = virtualCom->readAll();
    if(!rx_frame.contains('\r')){
        virtualCom->close();
        return false;
    }

    // Set the acceptance filter
    if(!setStandardFilter(address,mask)){
        virtualCom->close();
        return false;
    }

    // Open the CAN communication
    if(!startCanCommunication()){
        virtualCom->close();
        return false;
    }

    // canRxTimer = startTimer(2);
    return true;

}

void titanCanDriver::driverClose(void){

    // CAN Communication Close
    QByteArray command = "C\r";
    virtualCom->write(command.data());
    virtualCom->waitForBytesWritten(10);
    virtualCom->waitForReadyRead(10);

    // Virtual Com Close
    virtualCom->close();
    delete virtualCom;
    virtualCom = nullptr;
    if(canRxTimer) killTimer(canRxTimer);
    canRxTimer = 0;
    return;
}



char titanCanDriver::getHexVal(uchar val){
    val &= 0x0F;
    if(val<=9) return '0' + val;
    else return 'A' + (val-10);
}

QByteArray titanCanDriver::get11BitHexFormat(uint val) {
    val &= 0x07FF;
    QByteArray result ;
    result.append('0');
    result.append('0');
    result.append('0');

    uchar uc = val & 0x000F;
    if(uc) result[2] = getHexVal(uc);
    val = val >> 4;
    uc = val & 0x000F;
    if(uc) result[1] = getHexVal(uc);
    val = val >> 4;
    uc = val & 0x000F;
    if(uc) result[0] = getHexVal(uc);

    return result;
}

QByteArray titanCanDriver::get8BitHexFormat(uchar val) {

    QByteArray result ;
    result.append('0');
    result.append('0');
    if(val & 0xF) result[1] = getHexVal(val);
    val = val >> 4;
    if(val) result[0] = getHexVal(val);

    return result;
}

uint titanCanDriver::from11bitHex(QByteArray data){
    uchar c;
    uint result = 0;

    for(int i = 0; i<3; i++){
        if(data.at(i) >= 'a') c =  10 + data.at(i) - 'a';
        else if(data.at(i) >= 'A') c =  10 + data.at(i) - 'A';
        else c =  data.at(i) - '0';
        result += (c << (8 - 4 * i));
    }

    return result;
}

uchar titanCanDriver::from8bitHex(QByteArray data){
    uchar c;
    uint result = 0;

    for(int i = 0; i<2; i++){
        if(data.at(i) >= 'a') c =  10 + data.at(i) - 'a';
        else if(data.at(i) >= 'A') c =  10 + data.at(i) - 'A';
        else c =  data.at(i) - '0';
        result += (c << (4 - 4 * i));
    }

    return result;
}

bool titanCanDriver::checkHex(char data){

    if( (data >= '0') && (data <= '9') ) return true;
    if( (data >= 'A') && (data <= 'F') ) return true;
    if( (data >= 'a') && (data <= 'f') ) return true;

    return false;

}

bool titanCanDriver::setStandardFilter(uint addr, uint msk){

    rx_frame.clear();
    ADDR = get11BitHexFormat(addr);
    MASK = get11BitHexFormat(msk);


    QByteArray command = "m" + MASK + '\r';
    virtualCom->write(command.data());
    virtualCom->waitForBytesWritten(10);
    if(!virtualCom->waitForReadyRead(10)) return false;
    rx_frame = virtualCom->readAll();
    if(!rx_frame.contains('\r')) return false;

    command = "M" + ADDR + '\r';
    virtualCom->write(command.data());
    virtualCom->waitForBytesWritten(10);
    if(!virtualCom->waitForReadyRead(10)) return false;
    rx_frame = virtualCom->readAll();
    if(!rx_frame.contains('\r')) return false;

    return true;
}



bool titanCanDriver::startCanCommunication(void){
    QByteArray command;

    // Activates the communication ..
    if(openMode == "LOOPBACK"){
        command = "Y\r";
    }else command = "O\r";

    virtualCom->write(command.data());
    virtualCom->waitForBytesWritten(10);
    if(!virtualCom->waitForReadyRead(10)) return false;
    rx_frame = virtualCom->readAll();
    if(!rx_frame.contains('\r')) return false;

    rx_frame.clear();
    return  true;
}


void titanCanDriver::sendOnCanSlot(ushort canId, QByteArray data){
    CANID = get11BitHexFormat(canId);
    QByteArray DATA;
    uchar val;
    for(int i=0; i<8; i++){
        if(i < data.size()) val = data[i];
        else val = 0;
        DATA.append(get8BitHexFormat(val));

    }

    QByteArray command = "t" + CANID + "8" + DATA + "\r";
    virtualCom->write(command.data());
    virtualCom->waitForBytesWritten(1);

    // Wait to receive the answer to prevent a burst of transmission
    for(int i = 0; i <10; i++){
        if(driverRead()){
            emit receivedCanFrame(rxCanId, rxCanData);
            return;
        }
    }

    rxCanData.clear();
    emit receivedCanFrame(rxCanId, rxCanData);
    return;
}

void titanCanDriver::clearBuffer(uint i){
    if(rx_frame.size() == 0) return;


    for(;i < rx_frame.size(); i++){
        if(rx_frame[i] == 't') break;
    }
    if( i >=  rx_frame.size()) {
        rx_frame.clear();
        return;
    }

    QByteArray buffer;
    for(; i < rx_frame.size(); i++)   buffer.append(rx_frame[i]);
    rx_frame = buffer;
    return;

}

bool titanCanDriver::driverRead(void){

    // Reads all the incoming data buffer
    while(virtualCom->waitForReadyRead(1)) rx_frame.append(virtualCom->readAll());

    // Find 't' character
    rx_index = 0;
    while(rx_index < rx_frame.size()){
        if(rx_frame[rx_index]=='t') break;
        rx_index++;
    }

    if(rx_index == rx_frame.size()){
        rx_frame.clear();
        return false;
    }


    // Check the minimum size
    if(rx_frame.size() - rx_index < 4) return false;
    rx_index++;

    CANID[0] = rx_frame.at(rx_index++);
    CANID[1] = rx_frame.at(rx_index++);
    CANID[2] = rx_frame.at(rx_index++);

    if( (!checkHex(CANID[0])) || (!checkHex(CANID[1])) || (!checkHex(CANID[2]))){
         clearBuffer(rx_index);
         return false;
    }

    rxCanId = from11bitHex(CANID);
    if(!checkHex(rx_frame.at(rx_index))){
        clearBuffer(rx_index);
        return false;
   }


    int len = rx_frame.at(rx_index++) - '0';
    if(len > 8 ){
        clearBuffer(rx_index);
        return false;
    }


    // Check the minimum size
    if(rx_frame.size() - rx_index < 2 * len) return false;
    rxCanData.clear();

    for(int j=0; j < len; j++){
        BDATA[0] = rx_frame.at(rx_index++);
        BDATA[1] = rx_frame.at(rx_index++);
        if( (!checkHex(BDATA[0])) || (!checkHex(BDATA[1])) ){
            clearBuffer(rx_index);
            return false;
        }
        rxCanData.append(from8bitHex(BDATA));
    }

    clearBuffer(rx_index);
    return true;
}

void titanCanDriver::timerEvent(QTimerEvent* ev)
{
    if(ev->timerId() == canRxTimer)
    {

        if(driverRead()) emit receivedCanFrame(rxCanId, rxCanData);

    }

}
