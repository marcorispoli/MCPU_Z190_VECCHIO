#ifndef GANTRY_DEVICES_H
#define GANTRY_DEVICES_H

#include <QObject>
#include "canDriverClassInterface.h"
#include "deviceclass.h"


class gantryDevices : public QObject
{
    Q_OBJECT
public:
    explicit gantryDevices(canDriver* driver){

        // Connect the reception handler of the driver
        this->driver = driver;
        connect(driver, &canDriver::received200Sgn, this, &gantryDevices::deviceReceptionHandler, Qt::UniqueConnection);

        // Initialize the device list
        pDeviceList.clear();

        // Initialize the reception error counter
        dbg_rxerrors = 0;

    }

    // Working mode definition
    typedef enum{
        DEVICE_INIT=0,          //< Initialization
        DEVICE_CONFIGURATION,   //< Device configuration
        DEVICE_IDLE             //< Idle mode
    }_WORKING_MODE;


signals:

private slots:
    void sendToDevice(uchar seq, uchar id, uchar command, uchar d0, uchar d1, uchar d2, uchar d3){
        if(driver == nullptr) return;

        canDriverInterface::canDataFrame txFrame;
        txFrame.canId = 0x200 + id;
        txFrame.data[_DEVICE_SEQ_DATA_FRAME] = seq;
        txFrame.data[_DEVICE_CMD_DATA_FRAME] = command;
        txFrame.data[_DEVICE_DATA0_DATA_FRAME] = d0;
        txFrame.data[_DEVICE_DATA1_DATA_FRAME] = d1;
        txFrame.data[_DEVICE_DATA2_DATA_FRAME] = d2;
        txFrame.data[_DEVICE_DATA3_DATA_FRAME] = d3;

        uchar crc = 0;
        for(int i=0; i<7; i++) crc ^= txFrame.data[i];
        txFrame.data[7] = crc;

        driver->send(txFrame);
    }

    void deviceReceptionHandler(uchar id, canDriverInterface::canDataFrame frame); //< CAN message reception handler

public:

    /// This function register a given device in the class
    _inline void registerDevice(deviceClass* device){
        pDeviceList.append(device);
        connect(device, &deviceClass::sendToDevice, this, &gantryDevices::sendToDevice, Qt::UniqueConnection);
    }

private:
    QList<deviceClass*> pDeviceList; //< Registered device list

    canDriver* driver;  //< Pointer to the CAN driver interface
    _WORKING_MODE workingMode; //< current working mode

    uchar Seq; //< Message sequence in process
    ulong dbg_rxerrors; //< Number of wrong received can frame

};

#endif // GANTRY_DEVICES_H

