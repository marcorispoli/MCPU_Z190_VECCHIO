#ifndef DEVICE_REGISTERS_H
#define DEVICE_REGISTERS_H

#include <QObject>



/**
 * \defgroup GANTRY_DEVICE_DATA_REGISTER Gantry Device Data Register
 * \ingroup GANTRY_DEVICE_INTERFACE
 *
 * @brief The deviceDataRegisterClass class
 * \nosubgrouping
 *
 * This class implements the Device's Register Data type.
 *
 * # General description
 *
 * In the Gantry Device protocol, a central element is represented by the ***Device Register***.
 *
 * The Device register is a four bytes array,
 * with a status:
 * + _DEVREG_READY: the data content is ready to be read;
 * + _DEVREG_PENDING: the data content is to be updating.
 *
 * When the application requests to read or write a register,
 * its internal status changes to _DEVREG_PENDING until the device acknowledges the command.
 *
 * # Class Usage
 * This class is used into the deviceClass in order to implement
 * a list of *Status*, *Param* or *Data* registers handled by the
 * target Device.
 *
 *
 */
class deviceDataRegisterClass
{
public:
    /// Register status
    typedef enum{
        _DEVREG_READY  = 0,     // The data content is ready
        _DEVREG_PENDING  = 1    // The data content is updating
    }_devreg_status;


    /**
     * @brief deviceDataRegisterClass
     * Constructo of the class.
     *
     * @param curstat: this is the initial status of the register.
     * It is set to _DEVREG_READY as default.
     *
     */
    explicit deviceDataRegisterClass(_devreg_status curstat = _DEVREG_PENDING){
        for(int i=0; i<4; i++) data[i] = 0;
        status = curstat;
        init = false;
    }

    /**
     * @brief setRegister
     * This function update a register with an array of bytes.
     *
     * @param pdata: this is the pointer to a four bytes array;
     * @param stat: this is the initial status of the register;
     *
     * @return true if the register value is changed.
     */
    bool setRegister(uchar* pdata, _devreg_status stat = _DEVREG_READY){
        bool changed = false;
        for(int i=0; i<4; i++){
            if(data[i] != pdata[i]){
                data[i] = pdata[i];
                changed = true;
            }
        }
        status = stat;

        // If this is the first read, the set return always true (changed!)
        if(!init){
            init = true;
            return true;
        }else return changed;
    }


public:
    uchar  status;         //< True = ready, FALSE = pending
    uchar  data[4];        //< Register Data content
    bool   init;           //< Regiter initialized
};


#endif // DEVICE_REGISTERS_H

