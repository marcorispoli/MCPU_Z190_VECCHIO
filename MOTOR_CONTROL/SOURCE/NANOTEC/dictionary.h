#ifndef DICTIONARY_H
#define DICTIONARY_H


/*!
 * \defgroup  canOpenDictionaryModule Object Dictionary Implementation Module.
 *
 * The CanOpen based device makes use of the Object Dictionary objects \n
 * in order to handle the internal register content.
 *
 * This class allow to:
 * - crate an Object Dictionary from a can data stream;
 * - Retrive the can data stream from a created Object dictionary;
 * - Get and Set the register value;
 * - Get the error condition
 *
 * The Object register is defined by the following items:
 * - Index: this is the register address (ushort) ;
 * - Subindex: this is the register sub address (uchar);
 * - Register Data Type: this defines the data size: 1 Byte, 2Byte, 3Byte or 4Bytes are the possible sizes
 * - Register Value: is always a uint value, but it is truncated to the register size.
 *
 * \ingroup nanotecModule
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

/**
 * @brief This class implements the necessary tools to handle the Object Registers
 *
 *
 * \ingroup   canOpenDictionaryModule
 */
class canOpenDictionary
{
public:

    /**
     * @brief canOpenDictionary class creator
     *
     * Creates an Object dictionary. After creation the OD is set to invalid.
     */
    canOpenDictionary(void){
        odType = SDO_NOT_VALID;
    }
    ~canOpenDictionary(){};

    /// This enumeration defines the code of the Read/Write SDO operations
    typedef enum{
        SDO_NOT_VALID =0, //!< Invalid SDO
        INIT_BLOCK_TRANSFER=0x20, //!< Write a Domain block data
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


    QString printError(void);//!< Returns the string error of a failed Read?Write operation
    void set(QByteArray* data); //!< Set the OD content from the can frame content
    QByteArray get(void); //!< Get can frame content from OD content
    void clear(void); //!< Clear the content of a OD

    uint formatVal(uint val); //!< Format a value size based on the OD type
    uint getVal(void); //!< Get the OD data value
    void setVal(uint data); //!< Set the internal data of the proper data size
    void setOd(ushort index, uchar sub, canOpenDictionary::_ODDataType type, uint val);//!< Fills the complete OD register


    _inline bool isError(void){ return (odType == SDO_ACK_ERR) ; } //!< Function returnig if the last SDO Read/Write has been executed successfully
    _inline ushort getIndex(void){ return (index ) ; } //!< returns the register index
    _inline uchar getSubIndex(void){ return (subindex ) ; } //!< returns the register sub index
    _inline _ODDataType getType(void){ return (odType ) ; } //!< returns the OD data type




private:
    _ODDataType odType; //!< Rd/Write data code (defines the data size)
    ushort index;       //!< Register index feld
    uchar subindex;     //!< Register sub index field
    uchar b[4];         //!< Data array
    uint value;        //!< Formatted  value
};





#endif // DICTIONARY_H
