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

    #define SH_IP_ADDRESS  "192.6.1.201"  //!< Smart Hub Ip address
    #define SH_PORT    10000              //!< Smart Hub Port Number


    #include <QObject>

     /**
     * @brief Application constants definition
     *
     *  This Namespace defines all the MAIN constants that are used along the \n
     *  Application in order to interface the Generator and the Gantry.
     *
     * \ingroup MainModule
     */
    namespace Interface
    {

        /// This is the Enumeration of the Node Ids in the R2CP network
        typedef enum{
            SH_NODE_ID =1,              //!< [1] Node ID of the Smart Hub
            GENERATOR_NODE_ID = 6,      //!< [6] Generator Node ID
            APPLICATION_NODE_ID = 17,   //!< [17] Node ID of the Application
        }tR2cpNodeIds;

        /// This is the Enumeration of the available Workstations
        typedef enum{
            DIRECT_WORKSTATION_ID =1,    //!< [1] Workstation ID for Synch eposures
            DETECTOR_WORKSTATION_ID = 5  //!< [5] Workstation ID for direct exposures modes
        }tWorkstations;

        /// This is the enumeration of the Workflow Status
        typedef enum{
            _STATUS_NOT_READY = 0, //!< [0] Not ready to execute exposures
            _STATUS_READY = 1,     //!< [1] Ready to execute exposures
            _STATUS_ERROR = 2,     //!< [2] The Application is in Error condition
        }tStatus;

        /// This is the Enumeration code for the ExposureComplete Event
        typedef enum{
            _EXPOSURE_COMPLETED = 0, //!< [0] Exposure completed with success
            _EXPOSURE_ABORT     = 1, //!< [1] Exposure aborted without xray releasing
            _EXPOSURE_PARTIAL   = 2, //!< [2] Exposure partially executed
        }tExposureResult;

        /// This is the Enumeration of possible errors during exposure sequence
        typedef enum{
          _EXP_ERR_NONE = 0,                    //!< [0] No Errors code
          _EXP_ERR_XRAY_ENA_EARLY_RELEASED=1,   //!< [1] XRAY-ENA released during the Exposure
          _EXP_ERR_PROCEDURE_SETUP=2,           //!< [2] Error during Exposure preparation
          _EXP_ERR_PRE_VALIDATION=3,            //!< [3] Error of the Pre Pulse Validation
          _EXP_ERR_PULSE_VALIDATION=4,          //!< [4] Error of the Pulse Validation
          _EXP_ERR_GENERATOR_ERRORS=5,          //!< [5] Error signalled by the Generator
          _EXP_ERR_GENERATOR_STATUS=6,          //!< [6] Wrong Generator STatus during exposure
          _EXP_ERR_ABORT_REQUEST=7,             //!< [7] Error due to a Gantry Abort Request command
          _EXP_ERR_MAS_OUT_OF_RANGE=8,          //!< [8] Error due to mAs out of range (respect the available integration time)
          _EXP_ERR_AEC_TMO=9,                   //!< [9] Error Timeout AEC
        }tExposurErrors;


        /// This is the structure receiving the Post Exposure data from the generator
        typedef struct {
            uchar pulse_seq;    //!< Data Bank number
            uchar foc;          //!< Focal spot used
            float kV;           //!< kV of the shot
            float mAs;          //!< mAs of the shot
            float mA;           //!< Anodic current in mA
            float ms;           //!< milli seconds endurance
            uchar result;       //!< Result code
        }tPostExposureData;
    }


#endif
