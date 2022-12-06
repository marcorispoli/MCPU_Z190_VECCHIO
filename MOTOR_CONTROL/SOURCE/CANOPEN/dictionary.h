#ifndef DICTIONARY_H
#define DICTIONARY_H

/*!
 * \defgroup  canOpenModule CanOpen Protocol implementation.
 *
 * This Module implements the CanOpen protocol.
 *
 *
 */

#include <QtCore>

#define _OD_MASK(x,y) (x)
#define _OD_VAL(x,y) (y)
#define OD_MASK(x) _OD_MASK(x)
#define OD_VAL(x) _OD_VAL(x)

#define _OD_IDX(x,y,z) (x)
#define OD_IDX(x) _OD_IDX(x)
#define _OD_SBX(x,y,z) (y)
#define OD_SBX(x) _OD_SBX(x)
#define _OD_TP(x,y,z) (z)
#define OD_TP(x) _OD_TP(x)

class canOpenDictionary
{
public:

    canOpenDictionary(void){
        odType = SDO_NOT_VALID;
    }
    ~canOpenDictionary(){};

    QString printError(void);//!< Returns the string error of a failed Read?Write operation
    void set(QByteArray* data); //!< Set the OD content from the can frame content
    QByteArray get(void); //!< Get can frame content from OD content
    void clear(void); //!< Clear the content of a OD
    ulong getVal(void); //!< Get the OD data value
    ulong formatVal(ulong val); //!< Format a value size based on the OD type

    /// Function returnig if the last SDO Read/Write has been executed successfully
    _inline bool isError(void){ return (odType == SDO_ACK_ERR) ; }

    /// This enumeration defines the code of the Read/Write SDO operations
    typedef enum{
        SDO_NOT_VALID =0, //!< Invalid SDO
        WR_1BYTE=0x2F,  //!< Write Data Code for 1Byte data load
        WR_2BYTE=0x2B,  //!< Write Data Code for 2Byte data load
        WR_3BYTE=0x27,  //!< Write Data Code for 3Byte data load
        WR_4BYTE=0x23,  //!< Write Data Code for 4Byte data load
        WR_ACK_OK=0x60, //!< Write Returned Data Code for Write success
        RD_COMMAND = 0x40, //!< Read Command Code for read SDO frame
        RD_1BYTE=0x4F,  //!< Read Returned Code for 1Byte data read
        RD_2BYTE=0x4B,  //!< Read Returned Code for 2Byte data read
        RD_3BYTE=0x47,  //!< Read Returned Code for 3Byte data read
        RD_4BYTE=0x43,  //!< Read Returned Code for 4Byte data read
        SDO_ACK_ERR=0x80,//!< Read/Write SDO Failed error
    }_ODDataType;


    _ODDataType odType; //!< Rd/Write data code
    ushort index;
    uchar subindex;
    uchar b[4];

};





#endif // DICTIONARY_H
