#ifndef CANDRIVERCLASSINTERFACE_H
#define CANDRIVERCLASSINTERFACE_H

#include <QObject>
#include <QThread>
#include <QMutex>

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

    canDriverInterface(void){
        isOpen = false;
    }

    ~canDriverInterface(void){

    }

    /// This is the dataframe format
    typedef struct{
        unsigned short canId; //!< CAN-ID frame identifier
        unsigned char data[8]; //!< 8 bytes data load;
    } canDataFrame;

    typedef enum{
      _CAN_DRIVER_NA =0,
      _CAN_DRIVER_OK,
      _CAN_DRIVER_INVALID_COM,
      _CAN_DRIVER_ALREADY_OPEN,
      _CAN_DRIVER_INVALID_REVCODE,
      _CAN_DRIVER_INVALID_SN,
      _CAN_DRIVER_INVALID_ERRSTAT,
      _CAN_DRIVER_INVALID_BAUDRATE,
      _CAN_DRIVER_INVALID_FILTER,
      _CAN_DRIVER_INVALID_CAN_OPENING,

    } _canDriverErrors;

    typedef enum{
      _CAN_100K =0,
      _CAN_250K,
      _CAN_500K,
      _CAN_1000K
    } _canBaudRate;

protected:

    /// Open the Bus communication
    virtual _canDriverErrors driverOpen(QByteArray COM, _canBaudRate BR, uint address, uint mask, QByteArray mode){return _CAN_DRIVER_OK;}

    /// Close Bus communication
    virtual void driverClose(void){return;}

    /// Sends and receive data from a remote CAN Node
    virtual canDataFrame driverTxRxData(canDataFrame frame, uint tmo) { return frame;}

    virtual canDataFrame driverRead(void){;}
    virtual void driverSend(canDataFrame frame){;}

public slots:

    void openPort(QByteArray COM, _canBaudRate BR, uint address, uint mask, QByteArray mode){
        _canDriverErrors res = _CAN_DRIVER_OK;

        if(isOpen) {res = _CAN_DRIVER_ALREADY_OPEN;}
        else {res = driverOpen(COM, BR, address, mask, mode);}

        emit driverErrorSgn((uchar) 0, res);
        return;
    }

    void closePort(){        
        if(!isOpen) driverClose();
        isOpen = false;
    }


    void driverSendData(uchar msgid, canDriverInterface::canDataFrame frame, uint tmo){
         emit driverSendCompletedSgn(msgid, driverTxRxData(frame, tmo));
    }

    void receiveThread(void){
        canDataFrame data = driverRead();
        ushort address = data.canId & 0xF80;
        uchar  id = data.canId & 0x07F;

        if(address == 0x400) emit driverRx400(id);
        else if(address == 0x480) emit driverRx480(id);
        else if(address == 0x500) emit driverRx500(id);
        else if(address == 0x580) emit driverRx580(id);
    }

signals:
    void driverRx400(uchar id);
    void driverRx480(uchar id);
    void driverRx500(uchar id);
    void driverRx580(uchar id);

    void driverSendCompletedSgn(uchar msgid, canDriverInterface::canDataFrame frame);
    void driverErrorSgn(uchar msgid, uint errmsg);

private:
    uint address;
    uint mask;
    QByteArray mode;
    bool isOpen;

    canDataFrame rxFrame;
    bool isRx;

    canDataFrame txFrame;
    bool isTx;
};


class canDriver: public QObject{
    Q_OBJECT

public:

    canDriver(void){

    }

    ~canDriver(void){
        emit quit();
    }


    template <class T> void driverInitialize(void){
        worker = new T();
        QThread* thread = new QThread();
        worker->moveToThread(thread);
        connect( worker, &canDriverInterface::driverErrorSgn, this, &canDriver::driverErrorSlot, Qt::QueuedConnection);
        connect( worker, &canDriverInterface::driverSendCompletedSgn, this, &canDriver::driverSendCompletedSlot, Qt::QueuedConnection);

        connect( this, &canDriver::sendSgn, worker, &canDriverInterface::driverSendData, Qt::QueuedConnection);
        connect( this, &canDriver::openSgn, worker, &canDriverInterface::openPort, Qt::QueuedConnection);
        connect( this, &canDriver::closeSgn, worker,&canDriverInterface::closePort, Qt::QueuedConnection);
        connect( this, &canDriver::quit, thread, &QThread::quit);
        connect( this, &canDriver::quit, worker, &canDriverInterface::deleteLater);
        connect( thread, &QThread::finished, thread, &QThread::deleteLater);
        thread->start();
    }

signals:
    void sendSgn(uchar msgid, canDriverInterface::canDataFrame frame, uint tmo);
    void openSgn(QByteArray COM, canDriverInterface::_canBaudRate BR, uint address, uint mask, QByteArray mode);
    void closeSgn(void);
    void quit(void);

    void received(uchar msgid, canDriverInterface::canDataFrame frame);
    void errors(uchar msgid, uint errmsg);

public:
    void send(uchar msgid, canDriverInterface::canDataFrame frame, uint tmo){
        errcode = canDriverInterface::_CAN_DRIVER_NA;
        err_msgid = 0;
        emit sendSgn(msgid, frame, tmo);
    }
    void open(QByteArray COM, canDriverInterface::_canBaudRate BR, uint address, uint mask, QByteArray mode){
        errcode = canDriverInterface::_CAN_DRIVER_NA;
        err_msgid = 0;
        emit openSgn(COM, BR, address, mask,  mode);
    }
    void close(){
        errcode = canDriverInterface::_CAN_DRIVER_NA;
        err_msgid = 0;
        emit closeSgn();
    }

    _inline canDriverInterface::_canDriverErrors getError(void){return errcode;}
    _inline canDriverInterface::canDataFrame getRxFrame(void){return rx_frame;}

private slots:

    void driverSendCompletedSlot(uchar msgid, canDriverInterface::canDataFrame frame){
        rx_frame = frame;
        rx_msgid = msgid;
        emit received(msgid, frame);
    }
    void driverErrorSlot(uchar msgid, uint errmsg){
        err_msgid = msgid;
        errcode = (canDriverInterface::_canDriverErrors) errmsg;
        emit errors(msgid, errmsg);
    }

private:
    canDriverInterface* worker;
    QThread* thread;

    canDriverInterface::canDataFrame rx_frame;
    uchar rx_msgid;

    canDriverInterface::_canDriverErrors errcode;
    uchar err_msgid;

};

#endif // CANDRIVERCLASSINTERFACE_H
