#ifndef DEVICECLASS_H
#define DEVICECLASS_H

#include <QObject>
#include "canDriverClassInterface.h"

class deviceDataRegisterClass
{
public:
    explicit deviceDataRegisterClass(bool curstat = true){
        for(int i=0; i<4; i++) data[i] = 0;
        status = curstat;
    }

    bool setRegister(uchar* pdata, bool stat = true){
        bool changed = false;
        for(int i=0; i<4; i++){
            if(data[i] != pdata[i]){
                data[i] = pdata[i];
                changed = true;
            }
        }
        status = stat;
        return changed;
    }

public:
    bool  status;         //< True = ready, FALSE = pending
    uchar data[4];        //< Register Data content
};

class deviceClass: public QObject
{
    Q_OBJECT

    explicit deviceClass();

    /// Definition of the Command Frame Codes
    typedef enum{
        _DEVCMD_READSTAT,       //< Read Status register
        _DEVCMD_WRITEPAR,       //< Write Parameter register
        _DEVCMD_WRITEDATA,      //< Write Data register
        _DEVCMD_STOREPAR,       //< Store Parameter register
        _DEVCMD_EXEC            //< Execute Command
    }_DEVICE_COMMAND_FIELD;


    #define _DEVICE_SEQ_DATA_FRAME 0
    #define _DEVICE_CMD_DATA_FRAME 1
    #define _DEVICE_REGINDEX_DATA_FRAME 2
    #define _DEVICE_CRC_DATA_FRAME 7

    // Data register content
    #define _DEVICE_DATA0_DATA_FRAME 3
    #define _DEVICE_DATA1_DATA_FRAME 4
    #define _DEVICE_DATA2_DATA_FRAME 5
    #define _DEVICE_DATA3_DATA_FRAME 6

    // SPECIAL STATUS REGISTERS ADDRESS
    #define _DEVICE_RESERVED_REGSITER  0
    #define _DEVICE_SYSTEM_STATUS      1
    #define _DEVICE_ERRORS_STATUS      2


    /** Register setting options
     *  The Priority is:
     *  -   _REG_DEV_NO_ACTION
     *  -   _REG_DEV_FORCE
    */
    typedef enum {
        _REG_DEV_NOP = 0,           //< No options
        _REG_DEV_FORCE = 0x1,       //< Force the signaling in the case the register is not changed
        _REG_DEV_NO_ACTION = 0x2,   //< No signaling in case of data is changed
    }_RegisterSetOptions;


signals:
    /**
     * @brief This signal is generated when a Status register changes its value.
     *
     * When a decoded can frame relates to a READ-STATUS command, the STATUS register
     * content received from the device is compared with the current value. If the value should
     * change, the signal can be generated.
     *
     * Thera are options in the decodeFrame() function to change the normal behavior of this signal:
     * - _REG_DEV_NO_ACTION: prevent in any case to emit the signal;
     * - _REG_DEV_FORCE: force to send the signal even when no change
     * @param index: this is the index of the changed Status register
     */
    void statusChangedSgn(uchar index);


    void sendToDevice(uchar seq, uchar id, uchar command, uchar d0, uchar d1, uchar d2, uchar d3); //< Signal to be connected to the Can Driver send

public:
    /**
     * @brief This function upload a Parameter register content into the target device
     * @param seq: this is the seqeunce message number
     * @param index; this is the index address of the register (1:255)
     * @param parameter: this is the parameter content (deviceDataRegisterClass type)
     */
    void uploadParam(uchar seq, uchar index, deviceDataRegisterClass parameter);

    /**
     * @brief This function upload a Data register content into the target device
     * @param seq: this is the sequence message number
     * @param index; this is the index address of the register (1:255)
     * @param parameter: this is the Data register content (deviceDataRegisterClass type)
     */
    void uploadData(uchar seq, uchar index, deviceDataRegisterClass data);

    /**
     * @brief This function retrive the content of a given Status register of the target device
     * @param seq: this is the message seqeunce number
     * @param index: this is the requested index Status register
     */
    void downloadStatus(uchar seq, uchar index);

    /**
     * @brief This function decodes the incoming CAN frame based on the Device Can Protocol Interface
     * @param rxFrame: this is the incoming Can frame
     * @param options: options related to the Status register change event (statusChangedSgn) See: _RegisterSetOptions;
     * @return
     * + TRUE: the frame has been correctly decoded;
     * + FALSE: the frame didn't match with the protocol criteria.
     */
    bool decodeFrame(canDriverInterface::canDataFrame* rxFrame, _RegisterSetOptions options = _REG_DEV_NOP);

    /// Return the Address of the current device
    _inline uchar getId(){return ID;}

private:
    QList<deviceDataRegisterClass> paramList; //< List of the device parameters (read/write)
    QList<deviceDataRegisterClass> dataList; //< List of the device data (read/write)
    QList<deviceDataRegisterClass> statusList; //< List of the status registers (readOnly)

    uchar ID; //< ID address of the device
};

#endif // DEVICECLASS_H

