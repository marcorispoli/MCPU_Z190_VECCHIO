#include "dictionary.h"
#include <QTimer>
#include <time.h>


/**
 * This function print out the error code meaning
 */
QString canOpenDictionary::printError(void){
    if(odType != SDO_ACK_ERR) return "";
    uint error = b[0] + b[1] * 256 + b[2] * 256*256 + b[3] *256*256*256;

    switch(error){
        case 0x05030000: return "Toggle bit not changed!";
        case 0x05040001: return "Command specifier unknown!";break;
        case 0x06010000: return "Unsupported access!";break;
        case 0x06010002: return "Read Only Entry!";break;
        case 0x06020000: return "Object not existing!";break;
        case 0x06040041: return "Object cannot be PDO mapped!";break;
        case 0x06040042: return "Mapped PDO exceeds PDO!";break;
        case 0x06070012: return "Parameter lenght too long!";break;
        case 0x06070013: return "Parameter lenght too short!";break;
        case 0x06090011: return "Sub index not existing!";break;
        case 0x06090031: return "Value too great!";break;
        case 0x08000022: return "Data cannot be read or stored in this state!";break;
        default: return QString("Unrecognized error code:0x%1").arg(error,1,16);
    }

}

/**
 * This funtion fills the Object Dictionary from the can frame
 * @param data
 */
void canOpenDictionary::set(QByteArray* data){


    index = (ushort) (data->at(1) &0xFF) +  (((ushort) (data->at(2)&0xFF)) << 8) ;
    subindex = data->at(3);
    odType = (_ODDataType) data->at(0);
    for(int i=4; i<data->size(); i++) b[i-4] = data->at(i);
    return;
}

/**
 * This function returns the can frame from the Object Dictionary content
 *
 * @return QByteArray the can data frame format
 */
QByteArray canOpenDictionary::get(void){
    QByteArray data;
    data.append((uchar) odType);
    data.append((uchar) index);
    data.append((uchar) (index>>8));
    data.append((uchar) subindex);
    data.append((uchar) b[0]);
    data.append((uchar) b[1]);
    data.append((uchar) b[2]);
    data.append((uchar) b[3]);
    return data;
}

/**
 * @brief canOpenDictionary::clear
 */
void canOpenDictionary::clear(void){
    odType = SDO_NOT_VALID;
}

/**
 * This function returns the value of the OD based on the data type.
 *
 * @return (uint) formatted content
 */
uint canOpenDictionary::getVal(void){
    if((odType == WR_1BYTE) || (odType == RD_1BYTE)) return (uint) b[0];
    if((odType == WR_2BYTE) || (odType == RD_2BYTE)) return (uint) b[0] + (uint) b[1] * 256;
    if((odType == WR_3BYTE) || (odType == RD_3BYTE)) return (uint) b[0] + (uint) b[1] * 256 + (uint) b[2] * 256 * 256;
    if((odType == WR_4BYTE) || (odType == RD_4BYTE)) return (uint) b[0] + (uint) b[1] * 256 + (uint) b[2] * 256 * 256+ (uint) b[3] * 256 * 256 * 256;
    return 0;
}

/**
 * This function format the parameter value to the size of the OD.
 *
 * The parameter is an unsigned long value.
 * It is truncated to 1 Byte/2Byte/3Byte or 4 Byte \n
 * based on the actual format of the Object Dictionary.
 *
 * # USAGE
 *
 * When it is needed to make a comparison from an arbitrary value and a content of \n
 * an Object register, before to make the comparison, the arbitrary value shall be \n
 * formatted to be compliance with the size of the Register value.\n
 * For example, if the Object register is a 2 byte data size,\n
 * before o be compared with an arbitrary uint variable, the vaRIABLE SHALL BE FORMATTED:
 *
 *
 *  uint variable = ... someting\n
 *  OD.getVal() == variable --> WRONG COMPARISON\n
 *
 *
 *  uint variable = ... someting\n
 *  OD.getVal() == OD.formatVal(variable) --> CORRECT COMPARISON\n
 *
 * @param val: this is the value that needs to be formatted
 * @return formatted result value
 */
uint canOpenDictionary::formatVal(uint val){
    if((odType == WR_1BYTE) || (odType == RD_1BYTE)) return (uint) (val&0xFF);
    if((odType == WR_2BYTE) || (odType == RD_2BYTE)) return (uint) (val&0xFFFF);
    if((odType == WR_3BYTE) || (odType == RD_3BYTE)) return (uint) (val&0xFFFFFF);
    if((odType == WR_4BYTE) || (odType == RD_4BYTE)) return val;
    else return (0);
}

/**
 * This function assignes the content of the register \n
 * formatting it with the register size.
 *
 * The register should be created before with the proper \n
 * data content.
 *
 * @param data: this is the data value to be set into the register.
 */
void canOpenDictionary::setVal(uint data){
    uint v = formatVal(data);
    this->value = v;
    b[0] = (uchar) (v & 0xFF); v = v >>8;
    b[1] = (uchar) (v & 0xFF); v = v >>8;
    b[2] = (uchar) (v & 0xFF); v = v >>8;
    b[3] = (uchar) (v & 0xFF); v = v >>8;
}

/**
 * This function fills the OD register with all the internal items
 * @param index
 * @param sub
 * @param type
 * @param val
 */
void canOpenDictionary::setOd(ushort index, uchar sub, canOpenDictionary::_ODDataType type, uint val){
    odType = type;
    this->index = index;
    this->subindex = sub;
    setVal(val);
}



