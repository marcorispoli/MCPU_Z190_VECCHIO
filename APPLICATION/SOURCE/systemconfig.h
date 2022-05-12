#ifndef SYSTEMCONFIG_H
#define SYSTEMCONFIG_H

#include "configfile.h"

/// Defines the Device Can Address of the gantry's device
#define _POWER_SERVICE_BOARD_ADDRESS    (0x01)
#define _COMPRESSOR_BOARD_ADDRESS       (0x02)
#define _COLLIMATOR_BOARD_ADDRESS       (0x03)
#define _GRID_BOARD_ADDRESS             (0x04)

class systemIni: public configFile
{
    public:

    #define REVISION     2
    #define CONFIG_FILENAME     "C:\\Users\\marco\\Documents\\WORKSPACE\\system.ini"
    #define AWS_ADDRESS_PARAM   "AWS_ADDRESS"
    #define SERVICE_ADDRESS     "SERVICE_ADDRESS"
    #define GANTRY_DISPLAY      "GANTRY_DISPLAY"
    #define SERVICE_DISPLAY     "SERVICE_DISPLAY"
    #define CAN_CH0_COM         "CAN_CH0_COM"
    #define CAN_CH1_COM         "CAN_CH1_COM"

    systemIni():configFile( (const configFile::fileDescriptorT)
    {   CONFIG_FILENAME, REVISION,
        {{
            { AWS_ADDRESS_PARAM,     {{"127.0.0.1", "10001"}},  "Adress of the AWS command socket"},
            { SERVICE_ADDRESS,       {{"127.0.0.1", "10002"}},  "Adress of the Service command socket"},
            { GANTRY_DISPLAY,        {{"1366","768"}},          "Width and Height of Gantry display"},
            { CAN_CH0_COM,           {{"COM8", "1000"}},          "CAN CH0 definitions"},
            { CAN_CH1_COM,           {{"COM9", "1000"}},          "CAN CH1 definitions"},
        }}
    })
    {
        this->loadFile();
    };


private:
};


class systemConfig
{
public:
    systemConfig();


public:
    systemIni sysini;   //! Instance of the system configuration file

private:
    static systemConfig * pConfig; //!< static pointer to the class object instance in the application


};

#endif // SYSTEMCONFIG_H
