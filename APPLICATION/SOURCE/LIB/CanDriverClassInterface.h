#ifndef CANDRIVERCLASSINTERFACE_H
#define CANDRIVERCLASSINTERFACE_H

#include <QObject>
#include <QThread>

/**
 * \defgroup CAN_INTERFACE_MODULE Can Interface Module
 * \ingroup LIB_MODULES
 */

/**
 * \addtogroup CAN_HARDWARE_DRIVER_INTERFACE Can Hardware Driver Implementation
 * \ingroup CAN_INTERFACE_MODULE
 * @brief The canDriverInterface class
 * \nosubgrouping
 *
 * Thos class implements the API to control a Hardware Can
 * communication board.
 *
 * The class creates a working thread on witch the communication takes place.
 * The Application shall implement a user Class living in the Main Thread
 * in order to interact with the driver.
 *
 * # API Functions
 * The class implements the following API interface functions:
 * + openDriverPort(): this function opens a reception filter on the can Interface board.
 * As for the CAN protocol definition, a node is defined with a CANID identifier that can be at
 * 11 bit (standard mode) or 18 bit (Extended mode).
 *
 * + closeDriver(): this function close the can board activities.
 * The worker thread remains however running, ready to a new open command execution.
 *
 * + runDriver(): this command activate the CAN transactions on the BUS.
 * The communication now can take place.
 *
 * + stopDriver(): the communication is paused.
 *
 * + driverDistructor(): the class driver is deleted.
 * \attention this method shall be called by the application main thread
 * in order to delete the class and its inherent thread.
 *
 * # USAGE
 * The Driver Class implementing the API for a given interface board
 * shall inherit this class in order to implement a correct interface for the
 * Application.
 *
 * The class shall implement the following virtual methods with proper
 * board commands:
 *
 * + hardwareInit(): executes all the necessary actions to initialize the hardware board;
 * + hardwareShutdown(): executes all the actions to shutdown the board;
 * + openPort(): open a given reception filter with identifier and acceptance mask;
 * + closePorts(): closes all the receiving filters;
 * + run(): activates the communication on the BUS;
 * + pause(): pauses the communication on the BUS;
 * + driverTxRxData(): a blocking method transmitting and waiting a received frame to/from a node.
 *
 *  EXAMPLE:
 *
 \verbatim
    class canDriver: public canDriverInterface
    {
       Q_OBJECT
       public:
       canDriver(){};
       ~canDriver(){};

       // Implements the virtual functions here..

        bool hardwareInit(){
            // Hardware initialization ..
            return true;
        }
        bool hardwareShutdown(){
            // Hardware shutdown ...
            return true;
        }
        bool openPort(QString com, QString BR, uint addr, uint msk, bool md){
            // Open acceptance filter here ..
            return true;
        }
        bool closePorts(void){
            // Close filters here ...
            return true;
        };
        void run(void){
            // Activates the communication ..
        }
        void pause(void){
            // Pause the communication ..
        }

        canDataFrame driverTxRxData(canDataFrame frame, int tmo) {
            // Send the data frame...

            // Wait the reception from the target node ...

            // Return the received frame
            return frame;
        }


    }
 \endverbatim
 */
class canDriverInterface : public QObject
{
    Q_OBJECT

public:

    canDriverInterface(void);
    ~canDriverInterface(void);

    /// This is the dataframe format
    typedef struct{
        unsigned short canId; //!< CAN-ID frame identifier
        unsigned char data[8]; //!< 8 bytes data load;
    } canDataFrame;

    ///@name API Interface
    /// @{

    /**
     * @brief openDriverPort
     * This function executes several tasks:
     * + Initialize the workerthread at the first call;
     * + Initialize the hardware if not initialized
     * + Opens a receiver filter
     *
     * The communication however is not initiated until a runDriver()
     * command is called
     *
     * @param com
     * QString type: Virtual com used to control the device (COM1, COM2 ..)
     * @param BR
     * QString type: Boud rate of the communication (K units)
     * @param address
     * Filter code
     * @param mask
     * Filter mask
     * @param extended
     * Extended/Standard mode
     * @return
     * true if success
     */
    bool openDriverPort(QByteArray com, QByteArray BR, uint address, uint mask);

    /**
     * @brief closeDriver
     * This function close the communication
     * and shutdown the driver. The Thread however remain
     * active.
     */
    void closeDriver(void){
        if(!initDriver) return;
        initDriver = false;
        hardwareShutdown();
        // emit workFinished();
    }

    /**
     * @brief runDriver
     * The communication can start.
     * This command is valid only if the hardware has been
     * initialized and a filter  has been open.
     */
    void runDriver(void);

    /**
     * @brief stopDriver
     * The communication is paused
     */
    void stopDriver(void);

    /**
     * @brief driverDistructor
     *
     * The application shall call this metod in order to destry the driver.
     */
    void driverDistructor(void){
        emit workFinished();
    }

    /// @}
signals:

     //! Emit this signal to cause the auto delete and thread delete
    void workFinished(void);

protected:
    /// Implement the hardware initialization functions
    virtual bool hardwareInit(QByteArray com, QByteArray br){ return true;}

    /// Implements the hardware shutdown functions
    virtual bool hardwareShutdown(){ return true;}

    /**
     * @brief openPort
     * This function shall open a reception filter on the can board
     * @param com
     * This is the virtual com to be open
     * @param BR
     * This is the baud rate to be used
     * @param addr
     * This is the standard/extended identifier address code
     * @param msk
     * This is the standard/extended identifier mask
     * @param md
     * this is the standard(false) or Extended (true) activation mode flag
     * @return
     */
    virtual bool setStandardFilter(uint addr, uint msk){  address = addr; mask = msk; return true;}

    /// Open the Bus communication
    virtual bool open(void){return true;};

    /// Close Bus communication
    virtual void close(void){}



    /**
     * @brief driverTxRxData
     * This function implements the data Transmission/Reception on the BUS.
     * The function shall be implemented as a blockant with timeout.
     * In case no data should be received, the function shall return
     * a all 0 frame.
     * @param frame
     * This is the frame to be transmitted;
     * @param tmo
     * This is the max time in ms the driver shall wait for a data reception.
     * @return
     * + The received frame in case of valid reception.
     * + All 0's frame in case of no data reception.
     */
    virtual canDataFrame driverTxRxData(canDataFrame frame, int tmo) {timeout = tmo; return frame;}

public slots:
    ///@name API Interface
    /// @{

    /**
     * @brief driverTxSlot
     * The driver implementing class shall connect this slot in order to send
     * a data frame through the can hardwar board.
     *
     * @param frame
     * This is the frame to be sent
     * @param odIndex
     * This is the frame index that is rported back to the received frame by the driver.
     * @param tmo
     * This should be the timeout in ms the driver shall wait for a reception frame.
     * @param device
     * This is the pointer of the calling implementing class
     */
    void driverTxSlot(canDriverInterface::canDataFrame frame, int odIndex, uint tmo, QObject* device);
    /// @}

private:
    bool initDriver;
    bool initThread;
    QThread workerThread;
    uint address;
    uint mask;
    QByteArray COM;
    QByteArray BR;

    bool mode;
    int timeout;
};

/**
 * \addtogroup CAN_PROTOCOL_INTERFACE Can Protocol Implementation
 * \ingroup CAN_INTERFACE_MODULE
 * @brief The canDriverUserClass class
 * \nosubgrouping
 *
 * This class is a recipient class to be used in order to
 * implementing a protocol based on the Can Communication.
 *
 * A can implementing class should inherit this class in order
 * to use the canDriver API
 *
 * # USAGE
 *
 * A user class subclassing the canDriverUserClass
 * shall pass a pointer to the implemented driver (canDriverInterface)
 * that will be used for the data TxRx activities.
 *
 * The class then can use the methods:
 * + canTxFrame(): send frames to CAN BUS;
 * + deviceRxSlot(): handles the received frames
 *
 * The class shall implements its own deviceRxSlot() function
 * in order to handle the received data.
 *
 *
 */
class canDriverUserClass: public QObject
{
    Q_OBJECT
public:

    /**
     * @brief canDriverUserClass
     * This is the class constructor: the implementing calss shall
     * pass the driver shall be used a s a pointer.
     *
     * @param drv
     * Pointer to the class implementing the Can driver interface
     */
    canDriverUserClass(const canDriverInterface& drv);
    ~canDriverUserClass(void);


signals:
    /**
     * @brief txToDriver
     * This is the internal signal emitted when the canTxFrame is used by the application.
     * @param frame
     * This is the frame to be sent
     * @param odIndex
     * This is a proper index of the frame (up to the implementation)
     * @param tmo
     * This should be the timeout in ms the driver should wait for a data reception.
     * @param device
     * THis is the pointer to the implementing class
     */
    void txToDriver(canDriverInterface::canDataFrame frame, int odIndex, uint tmo, QObject* device);

public slots:
    ///@name API Interface
    /// @{
    /**
     * @brief deviceRxSlot
     * The class shall implement this method in order to handle the can data frame received.
     * @param frame
     * This is the frame received
     * @param odIndex
     * This is the frame identifier integer code.
     */
    virtual void deviceRxSlot(canDriverInterface::canDataFrame frame, int odIndex)=0;

    /**
     * @brief canTxFrame
     * This is the transmission function.
     * @param frame
     * This is the frame to be sent
     * @param odIndex
     * This is the frame index
     * @param tmo
     * This is the reception timeout
     */
    void canTxFrame(canDriverInterface::canDataFrame frame, int odIndex, uint tmo){
        emit txToDriver(frame, odIndex, tmo, this);
    }

    ///@}
public:

    const canDriverInterface* driver; //!< pointer of the driver

};



#endif // CANDRIVERCLASSINTERFACE_H
