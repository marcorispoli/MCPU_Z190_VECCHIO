#include "dictionary.h"
#include <QTimer>
#include <time.h>


/**
 * This function print out the error code meaning
 */
QString canOpenDictionary::printError(void){
    if(odType != SDO_ACK_ERR) return "";
    ulong error = b[0] + b[1] * 256 + b[2] * 256*256 + b[3] *256*256*256;

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
 * @return (ulong) formatted content
 */
ulong canOpenDictionary::getVal(void){
    if((odType == WR_1BYTE) || (odType == RD_1BYTE)) return (ulong) b[0];
    if((odType == WR_2BYTE) || (odType == RD_2BYTE)) return (ulong) b[0] + (ulong) b[1] * 256;
    if((odType == WR_3BYTE) || (odType == RD_3BYTE)) return (ulong) b[0] + (ulong) b[1] * 256 + (ulong) b[2] * 256 * 256;
    else return (ulong) b[0] + (ulong) b[1] * 256 + (ulong) b[2] * 256 * 256+ (ulong) b[3] * 256 * 256 * 256;
}

/**
 * This function format the parameter value to the size of the OD.
 *
 * The parameter is an unsigned long value.
 * It is truncated to 1 Byte/2Byte/3Byte or 4 Byte \n
 * based on the actual format of the Object Dictionary.
 *
 * @param val: this is the value that needs to be formatted
 * @return formatted result value
 */
ulong canOpenDictionary::formatVal(ulong val){
    if((odType == WR_1BYTE) || (odType == RD_1BYTE)) return (ulong) (val&0xFF);
    if((odType == WR_2BYTE) || (odType == RD_2BYTE)) return (ulong) (val&0xFFFF);
    if((odType == WR_3BYTE) || (odType == RD_3BYTE)) return (ulong) (val&0xFFFFFF);
    else return (val);
}
