#include "canDriverClassInterface.h"


Q_GLOBAL_STATIC(bool, canDriverClass)

canDriverInterface::canDriverInterface(void)
{
    initDriver = false;
    if(!canDriverClass.exists()){
        qRegisterMetaType<canDataFrame>("canDataFrame");
    }
}

canDriverInterface::~canDriverInterface(void){
    *canDriverClass = false;
}

bool canDriverInterface::openDriverPort(QString COM, QString BR, uint address, uint mask){

    this->COM = COM;
    this->BR = BR;

    // Init both the hardware and the thread (only the first instance)
    if((!canDriverClass.exists()) || (*canDriverClass == false)){
        *canDriverClass = true;

        if(!hardwareInit(COM, BR)) return false;
        if(!setStandardFilter(address, mask)) return false;

        this->moveToThread(&workerThread);
        connect(this, SIGNAL(workFinished()), &workerThread, SLOT(quit())); // WorkFinished close the Thread
        connect(&workerThread, SIGNAL(finished()), &workerThread, SLOT(deleteLater()));  // Thread finished  delete the thread
        connect(this, SIGNAL(workFinished()), this, SLOT(deleteLater()));   // WorkFinished delete the worker

        workerThread.start();
        initDriver = true;
        return true;
    }

    // Init the can hardware, not the working thread
    if(!initDriver){
        if(!hardwareInit(COM, BR)) return false;
        if(!openPort(address, mask, mode)) return false;
        initDriver = true;
        return true;
    }

    // If the port has been already open it can only open a new filter
    if(!openPort(address, mask, mode)) return false;
    return true;
}

void canDriverInterface::runDriver(void){
    run();
}
void canDriverInterface::stopDriver(void){
    pause();
}



void canDriverInterface::driverTxSlot(canDataFrame frame, int odIndex, uint tmo, QObject* device){
    canDataFrame rxFrame = driverTxRxData(frame, tmo);

    // If no answer: error frame with 0 code
    if(rxFrame.canId==0) {
        rxFrame.data[0] = 0x80;
        rxFrame.data[4] = 0;
        rxFrame.data[5] = 0;
        rxFrame.data[6] = 0;
        rxFrame.data[7] = 0;
    }

    // Call the receiving slot of the device that requested the frame
    QGenericArgument arg1 = Q_ARG(canDataFrame,rxFrame );
    QGenericArgument arg2 = Q_ARG(int,odIndex );
    QMetaObject::invokeMethod(device, "deviceRxSlot",Qt::QueuedConnection, arg1, arg2 );

}

canDriverUserClass::canDriverUserClass(const canDriverInterface& drv){
    driver = &drv;
    connect(this, SIGNAL(txToDriver(canDriverInterface::canDataFrame,int,uint,QObject*)),
           driver, SLOT(driverTxSlot(canDriverInterface::canDataFrame,int,uint,QObject*)), Qt::QueuedConnection);
};

canDriverUserClass::~canDriverUserClass(void){

    // Disconnect any connection with the driver
    disconnect(this);

};
