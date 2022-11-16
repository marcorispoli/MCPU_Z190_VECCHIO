#ifndef TYPEDEF_H
#define TYPEDEF_H

    typedef unsigned char byte;
    typedef unsigned short word;
    typedef unsigned int  dword;
    typedef unsigned long  udword;
    typedef unsigned char uint8_t;
    typedef short int16_t;
    typedef int int32_t;




    #ifndef _HTONS_IMPLEMENTATION
        extern unsigned short htons(unsigned short val);
        extern unsigned int htonl(unsigned int val);
    #endif



    #define SH_NODE_ID              1
    #define APPLICATION_NODE_ID 	17
    #define GENERATOR_NODE_ID       6
    #define DIRECT_WORKSTATION_ID   1
    #define DETECTOR_WORKSTATION_ID 5


    #define IP_ADDRESS  "192.6.1.201"
    #define HUB_PORT    10000

    #include <QObject>

    namespace Interface
    {
        typedef enum{
            _STATUS_NOT_READY = 0,
            _STATUS_READY = 1,
            _STATUS_ERROR = 2,
        }tStatus;

        typedef enum{
            _EXPOSURE_COMPLETED = 0,
            _EXPOSURE_ABORT     = 1,
            _EXPOSURE_PARTIAL   = 2,
        }tExposureResult;

        typedef enum{
          _EXP_ERR_NONE = 0,
          _EXP_ERR_CP,
          _EXP_ERR_RAX_ABORT,
          _EXP_ERR_PROCEDURE_SETUP,
          _EXP_ERR_PRE_VALIDATION,
          _EXP_ERR_PULSE_VALIDATION,
          _EXP_ERR_GENERATOR_COMMAND,
          _EXP_ERR_GENERATOR_ERRORS,
          _EXP_ERR_GENERATOR_STATUS,
          _EXP_ERR_ABORT_REQUEST,
        }tExposurErrors;

        typedef enum{
          _SW_ERR_NONE = 0,
          _SW_ERR_GENERATOR_SETUP,
        }tSwErrors;

        typedef enum{
            _FOCUS_SMALL = 0,
            _FOCUS_LARGE = 1
        }tExposureFocus;

        typedef struct {
            uchar pulse_seq;
            uchar foc;
            float kV;
            float mAs;
            float mA;
            float ms;
            uchar result;
        }tPostExposureData;
    }
#endif
