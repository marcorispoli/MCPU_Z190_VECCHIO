#ifndef DEVICES_INTERFACE_H
#define DEVICES_INTERFACE_H

#include <QObject>
#include "can_driver_class_interface.h"
#include "device_class.h"



/**
 * \defgroup GANTRY_DEVICE_INTERFACE Gantry Device Communication modules
 *
 * # General description
 *
 * This module implements the basic elements of the communication protocol,
 * providing APIs to interact with the target Device registers and commands.
 *
 * The module is composed by three different classes:
 * + deviceDataRegisterClass: this class implement the device register features;
 * + deviceClass: this class implements the API's to handle the protocol characteristics;
 * + devicesInterface: this class is a container of several devices; it handle the intercation
 *   from the device class and the can communication driver.
 *
 * # Communication architecture
 *
 * The communication with a device requires to implement a specific protocol:
 * (See: Gantry Software Detailed Documentation: Device CAN BUS Description)
 *
 * In order to implement the device stack, the application:
 * + shall instance a Can Driver based o the canDriverInterface class;
 * + shall instance a devicesInterface class, assigning the can driver as the sender;
 * + For every device, the application shall subclass the deviceClass:
 *  +  the subclass implements the specific behavior of the target device;
 *  + the subclass shall be registered to the devicesInterface in order to properly communicate through the CAN.
 *
 * # Usage
 *
 * A tipical stack initialization is the following:
 *
 *  > // Declare the Can Driver interface:<br>
 *  > canDrive* driver = new canDriver();<br>
 *  > //Assignes the particolar CAN driver implemenation to be used;<br>
 *  > driver->initialize<canDriverSubclass>();<br>
 *  >
 *  > // Declare the devicesInterface class assigning the can driver it shall use for communication:<br>
 *  > devicesInterface* devicesHandler = new devicesInterface(driver);
 *
 *  > // Declare a deviceClass based class:<br>
 *  > deviceSubClass* device = new deviceSubClass(...);<br>
 *  > //register the class to the devicesHandler:<br>
 *  > devicesHandler->register((deviceClass*) device);<br>
 *
 *
 *
 */


/**
 * \defgroup GANTRY_DEVICES_INTERFACE Gantry Device Container
 * \ingroup GANTRY_DEVICE_INTERFACE
 *
 * @brief The devicesInterface class implementation
 * \nosubgrouping
 *
 * This class implements the container class that allows
 * the devices to communicate on the CAN BUS through the
 * application CAN driver.
 *
 *
 * # Public Methods
 *
 * + devicesInterface(canDriver* driver): class declaration API;
 * + registerDevice(deviceClass* device): device registration API
 *
 * # General description
 *
 * This class connects a given registered device to the can driver interface.
 *
 * When a device sends data, the signal emitted by the device is automaticalli connected
 * internally into the can driver sender slot;
 *
 * When the can driver receives a data frame, the content is inspected and validate
 * and the target device receiving callback is called.
 *
 *
 */
class devicesInterface : public QObject
{
    Q_OBJECT
public:

    /**
     * @brief devicesInterface
     *
     * Class contructor
     *
     * @param driver: this is the canDriver* pointer to the current can communication driver interface instance.
     */
    explicit devicesInterface(canDriver* driver){

        // Connect the reception handler of the driver
        this->driver = driver;
        connect(driver, &canDriver::received200Sgn, this, &devicesInterface::deviceReceptionHandler, Qt::UniqueConnection);

        // Initialize the device list
        pDeviceList.clear();

        // Initialize the reception error counter
        dbg_rxerrors = 0;

    }

    /// Working mode definition
    typedef enum{
        DEVICE_INIT=0,          //< Initialization
        DEVICE_CONFIGURATION,   //< Device configuration
        DEVICE_IDLE             //< Idle mode
    }_WORKING_MODE;


signals:

private slots:

    /**
     * @brief sendToDevice
     *
     * Sends a command frame to the target device based on the protocol specifications.
     *
     * @param seq: this is the command seqeunce number, reported in the acknowledge frame by the device
     *
     * @param id: this is the target device's address
     *
     * @param command: this is the command code
     *
     * @param d0: D0 of the command;
     * @param d1: D1 of the command;
     * @param d2: D2 of the command;
     * @param d3: D3 of the command;
     */
    void sendToDevice(uchar seq, uchar id, uchar command, uchar d0, uchar d1, uchar d2, uchar d3, uchar d4){
        if(driver == nullptr) return;

        canDriverInterface::canDataFrame txFrame;
        txFrame.canId = 0x200 + id;
        txFrame.data[_DEVICE_SEQ_DATA_FRAME] = seq;
        txFrame.data[_DEVICE_CMD_DATA_FRAME] = command;
        txFrame.data[_DEVICE_DATA0_DATA_FRAME] = d0;
        txFrame.data[_DEVICE_DATA1_DATA_FRAME] = d1;
        txFrame.data[_DEVICE_DATA2_DATA_FRAME] = d2;
        txFrame.data[_DEVICE_DATA3_DATA_FRAME] = d3;
        txFrame.data[_DEVICE_DATA4_DATA_FRAME] = d4;

        uchar crc = 0;
        for(int i=0; i<7; i++) crc ^= txFrame.data[i];
        txFrame.data[7] = crc;

        driver->send(txFrame);
    }

    void deviceReceptionHandler(uchar id, canDriverInterface::canDataFrame frame); //< CAN message reception handler

public:

    /**
     * @brief registerDevice
     * This function register a given device in the class
     *
     * @param device: this is the deviceClass* pointer to the device implementing class.
     */
    _inline void registerDevice(deviceClass* device){
        pDeviceList.append(device);
        connect(device, &deviceClass::sendToDevice, this, &devicesInterface::sendToDevice, Qt::UniqueConnection);
    }

    /**
     * @brief This function activate the device polling
     *
     * The function activate the device polling, controlled
     * by the  devicesInterface class that synchronizes the
     * devices preventing unwanted overlapping of the can
     * communication frames.
     *
     * The polling routin launches the device's polling function
     * every timeslot milliseconds.
     *
     * @param timeslot
     */
    void startPolling(uint timeslot);

private slots:
    void pollingSlot(void);

private:
    QList<deviceClass*> pDeviceList; //< Registered device list

    canDriver* driver;  //< Pointer to the CAN driver interface
    _WORKING_MODE workingMode; //< current working mode

    uchar Seq; //< Message sequence in process
    ulong dbg_rxerrors; //< Number of wrong received can frame


    bool pollingRunning;//< the polling has been activated

};

#endif // DEVICES_INTERFACE_H

