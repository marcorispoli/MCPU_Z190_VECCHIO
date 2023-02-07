#ifndef EXPOSURE_MANAGER_H
    #define EXPOSURE_MANAGER_H

#include <QApplication>
/**
* \defgroup  exposureModule Exposure Management Module.
*
* This Module implements the Exposure sequences Workflow.
*
* # GENERAL OVERVIEW
*
* The Module uses the following files:
* - EXPOSURE/exposure.h: defines all the classes used in the module;
* - EXPOSURE/2D_exposure.cpp: implement the Manual 2D exposure sequence;
* - EXPOSURE/2D_AEC_exposure.cpp: implement the AEC 2D exposure sequence;
* - EXPOSURE/3D_exposure.cpp: implement the Manual 3D exposure sequence;
* - EXPOSURE/3D_AEC_exposure.cpp: implement the AEC 3D exposure sequence;
*
* # EXPOSURE PROCEDURES
*
* The module implements the following exposure procedures:
* - Standard 2D exposure;
* - Exposure 2D with AEC pre pulse;
* - Manual Tomography;
* - Tomography with AEC pre pulse.
*
* Every procedure is fully implemented in a dedicated workflow routine \n
* that is called by the workflowManager Module.
*
* In particolar:
* - 2D Exposure procedure: exposureManager::handle_2D_MANUAL();
* - 2D + AEC Exposure procedure: exposureManager::handle_2D_AEC();
* - 3D Exposure procedure: exposureManager::handle_3D_MANUAL();
* - 3D + AEC Exposure procedure: exposureManager::handle_3D_AEC();
*
* # EXPOSURE OPTIONS
*
* Only for the Manual 2D exposure sequence it is possible to enable
* the Detector synch signals and the Grid Synch signal usage.
*
* # EXPOSURE PREPARATION
*
* ## 2D Manual Exposure Preparation
*
* Before to start the execution of the 2D Workflow routine, \n
* the application shall set the Exposure parameter with the following
* functions:
*
* - exposureManager::setExposureOptions(): to set the options (grid and detector synch);
* - exposureManager::setPulseData(): to set the focus, kV and mAs to be executed in the pulse.
*
*   > NOTE: the use of exposureManager::setPulseData() before every exposure activation is mandatory\n
*   in order to validate the data. The function shall be used even if the data are not \n
*   changed from the previous exposure.
*
* ## 2D+AEC Exposure Preparation
*
* Before to start the execution of the 2D+AEC Workflow routine, \n
* the application shall set the Pre Pulse Exposure parameter with the following
* functions:
*
* - exposureManager::setPreData(): to set the focus, kV , mAs and Timeout-AEC to be executed in the pre-pulse.
*
*   > NOTE: the use of exposureManager::setPulseData() before every exposure activation is mandatory\n
*   in order to validate the data. The function shall be used even if the data are not \n
*   changed from the previous exposure.
*
*   > NOTE: the options are not applicable to this exposure workflow.
*
* When the Pre Pulse is completed, the workflow will wait for the Pulse data:
* - exposureManager::setPulseData()
*
*   > NOTE: the workflow will wait the AEC exposure data for a maximum of Timeout-AEC ms.
*
* ## 3D Manual Exposure Preparation
*
* Before to start the execution of the 3D Workflow routine, \n
* the application shall set the Exposure parameter with the following
* functions:
*
* - exposureManager::setTomoConfig(): to set the Tomography configuration;
* - exposureManager::setPulseData(): to set the focus, kV and mAs to be executed in the pulse.
*
*   > NOTE: the focus in Tomo is always LARGE;
*
*   > NOTE: the use of exposureManager::setPulseData() before every exposure activation is mandatory\n
*   in order to validate the data. The function shall be used even if the data are not \n
*   changed from the previous exposure.
*
*   > NOTE: the options are not applicable to this exposure workflow.
*
* ## 3D+AEC  Exposure Preparation
*
* Before to start the execution of the 3D+AEC Workflow routine, \n
* the application shall set the Exposure parameter with the following
* functions:
*
* - exposureManager::setTomoConfig(): to set the Tomography configuration;
* - exposureManager::setPreData(): to set the focus, kV , mAs and Timeout-AEC to be executed in the pre-pulse.
*
*   > NOTE: the focus in Tomo is always LARGE;
*
* When the Pre Pulse is completed, the workflow will wait for the Pulse data:
* - exposureManager::setPulseData()
*
*   > NOTE: the workflow will wait the AEC exposure data for a maximum of Timeout-AEC ms.
*
*   > NOTE: the options are not applicable to this exposure workflow.
*
* # EXPOSURE TECHNIQUE MODE USED
*
* In the 2D exposures the Technique mode is the 2Point. \n
* With this Technique, the Generator selects kV , mA, and mAs and
* the  exposure duration is a consequence.
*
*   > NOTE: With 2Point technique the mAs can be selected only with integers values.
*
* In the 3D exposures, the AEC pulse is performed with 2Point Technique.\n
* The Tomo pulses instead are set with the 3Point Technique because\n
* the mAs per pulse needs to be decimal and not integer.
*
* With 3 Point Technique, the Generator selects kV, mA, mS of the total exposure.
* In order to let the Generator to expose a given amount of mAs:
*
* - The Application Request to Generator to provide the mA and mS for the kV to be used for a integer mAs greater than the expected per pulse;
* - The Application limits the minimum Pulse width to prevent the exposure with very short time pulses with too high Anodic current ;
* - The Application changes the mA so that the mA * mS equal the expected mAs per pulse;
*
* The minimum exposure time per pulse is set to 25ms with a define:
* - exposureManager::MIN_TOMO_EXPOSURE_TIME ;
*
* # EXPOSURE PULSE DURATION STANDARD
*
* In the Tomography exposures where the 3 Point technique is used,\n
* the exposure pulses duration are set with a discrete range, \n
* based on the Renard R10 scale:
*
* R10 Scale: [1 , 1.25 , 1.6 , 2 , 2.5 , 3.2 , 4 , 5 , 6.3 , 8 , 10 , ... ]
*
*
*
*/

/**
 * @brief This class implements the functions and variables to handle the Exposure Workflows
 *
 *
 * \ingroup exposureModule
 */
class exposureManager{

public:

    /**
     * @brief Constructor of the class
     *
     * The contstructor initialize all the exposure variables
     */
    exposureManager(){

        // Exposure data invalidation to prevent the use of not initialized exposure data
        preExposureData.valid = false;
        pulseExposureData.valid = false;
        tomoConfig.valid = false;

        // Defines the initial Tomo configuration
        tomoConfig.samples = 11;
        tomoConfig.skip = 1;
        tomoConfig.changed = true;

        // Initialize the exposure options
        exposureOptions.use_detector = true;
        exposureOptions.use_grid = true;

        // Init of the workflow error condition
        exposureError= false;


    }

    /// This is the enumeration for the Exposure limitations constants
    typedef enum{
        MIN_TOMO_EXPOSURE_TIME = 25, //!< [25] Defines the minimum Tomo pulse duration time
    }tExposureConstrains;

    /// This is the enumeration for the focal spot selection
    typedef enum{
        _FOCUS_SMALL = 0, //!< [0] Small focus
        _FOCUS_LARGE = 1  //!< [1] Large Focus
    }tExposureFocus;

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
    }tExposureErrors;

    /// This structure provide the varible of an exposure
    typedef struct{
        ushort tmo;     //!< Exposure timeout when applicable;
        float kV;       //!< kV to be used;
        float mAs;      //!< mAs
        float mS;       //!< milliseconds for exposure
        float mA;       //!< Anodic current in mA
        tExposureFocus focus;    //!< focus to be used
        bool valid;     //!< data are valid for he next exposure
        uchar DB;       //!< databank in case of post exposure data
    }_tExposureData;

    /// This is the structure defineing the Tomo configuration scan
    typedef struct{
        uchar samples;  //!< Number of valid samles in the scan
        uchar skip;     //!< Number of initial pulses to be skip
        ushort EW;      //!< Maximum integration time
        bool valid;     //!< Data are valid for the next exposure
        bool changed;   //!< The configuration has been changed
    }_tTomoConfig;

    /// This is the structure defining the exposure options
    typedef struct{
        bool use_detector;  //!< The exposure will use the detector synch signals
        bool use_grid;      //!< The exposure will use the grid synch signals
    }_tExposurOpt;



    /**
     * @brief
     * This function set the options to be applied in the next exposure sequence.
     *
     * The avaialble options are:
     * - use of the detector synchronization;
     * - use of the grid synchronization;
     *
     *  > NOTE: disabling the grid synchronization implies the detector sych disable as well.
     *  > NOTE: use_detector and use_grid options applies only to 2D Manual exposure.
     *
     * @param use_detector: this option enable the synch with the detector
     * @param use_grid: this option activate the sync with the grif device
     *
     */
    void setExposureOptions(bool use_detector , bool use_grid ){

       exposureOptions.use_detector = use_detector;
       exposureOptions.use_grid = use_grid;

    }

    /**
     * @brief
     * This function set the exposure data for the Pre Pulse in an AEC sequence.
     *
     * @param focus: is the current focus. See tExposureFocus;
     * @param kv: is the kV value in decimal format;
     * @param mAs: mAs in integer format;
     *
     */
    void setPreData(uchar focus, float kv, float mAs, ushort tmo){
        preExposureData.tmo = tmo;
        preExposureData.focus = (tExposureFocus) focus;
        preExposureData.kV = kv;
        preExposureData.mAs =  mAs;
        preExposureData.valid = true;
    }

    /**
     * @brief
     * This function set the exposure data for the Pulse.
     *
     * @param focus: is the current focus. See tExposureFocus;
     * @param kv: is the kV value in decimal format;
     * @param mAs: mAs in integer format;
     *
     */
    void setPulseData(uchar foc, float kv, float mAs){

        pulseExposureData.focus = (tExposureFocus) foc;
        pulseExposureData.kV = kv;
        pulseExposureData.mAs = mAs;
        pulseExposureData.valid = true;
    }

    /**
     * @brief
     * This function set the configuration of the next tomo sequence.
     *
     * @param nsamples: number of valid samples;
     * @param nskip: number of initial pulses to be skip;
     * @param ew: maximum integration time;
     *
     */
    void setTomoConfig(uchar nsamples, uchar nskip, ushort ew){

        // Determines if the sequence shall rebuild the procedure
        if( (nsamples != tomoConfig.samples) || (nskip != tomoConfig.skip)) tomoConfig.changed = true;
        else tomoConfig.changed = false;

        tomoConfig.samples = nsamples;
        tomoConfig.skip = nskip;
        tomoConfig.EW = ew;
        tomoConfig.valid = true;
        return;
    }

    /**
     * @brief This function adds post exposure data to the current PostExposure list
     *
     * This function is used to adds incomuing postExposure data that the Generator sends
     * every completed pulse.
     *
     * @param db: data bank code
     * @param foc: focus of the pulse
     * @param kV: kV of the pulse
     * @param mAs: mAs per pulse
     * @param mA: Anodic current mA
     * @param ms: pulse endurance in ms
     * @param result: result code
     */
    void setPostExposureData(uchar db, uchar foc, float kV, float mAs, float mA, float ms, uchar result){
        _tExposureData data;

        data.DB = db;
        data.focus = (tExposureFocus) foc;
        data.kV = kV;
        data.mAs = mAs;
        data.mA = mA;
        data.mS = ms;
        postExposure.append(data);
    }

    /**
     * @brief This function returns the closed lower or higher time from the reinard R10 scale
     *
     * The Function returns the next R10 value or the Previous R10 value \n
     * from the requested parameter.
     *
     * @param ms: this is the nominal time value
     * @param next: true = next value, false = previous value
     * @return the requested time of the R10 scale
     * @return 0: value not found
     */
    float getR10Time(float ms, bool next){
        static float r10[] = {1,1.25,1.6,2,2.5,3.2,4,5,6.3,8,10};
        float mult = 1;

        if( ms >= 10000) mult = 10000;
        else if( ms >= 1000) mult = 1000;
        else if( ms >= 100) mult = 100;
        else if( ms >= 10) mult = 10;
        ms = ms/mult;

        for(int i=9; i>=0;i--){
            if(ms == r10[i]) return r10[i]* mult;
            else if(ms > r10[i]){
                if(next) return r10[i+1] * mult;
                else return r10[i]* mult;
            }
        }
        return 0;
    }

public:

    // Exposure parameters
    QList<_tExposureData> postExposure; //!< list of the post exposure events
    bool    abortAecTimeoutRequest;     //!< set to true when the timeout of the AEC data is expired

    _tExposureData preExposureData;     //!< Exposure data for the Pre Pulse
    _tExposureData pulseExposureData;   //!< Exposure data for the Pulse
    _tExposurOpt   exposureOptions;     //!< Exposure options
    _tTomoConfig   tomoConfig;          //!< Tomo configuration data

    void handle_2D_MANUAL(void);        //!< Workflow routine for 2D manual exposure
    void handle_3D_MANUAL(void);        //!< Workflow routine for 2D+AEC  exposure
    void handle_2D_AEC(void);           //!< Workflow routine for 3D manual exposure
    void handle_3D_AEC(void);           //!< Workflow routine for 3D+AEC  exposure

private:

    float totalPremAs;                  //!< used to calc the total mAs of a pre pulse
    float totalPulsemAs;                //!< used to calc the total mAs of a pulse
    uchar current_status;               //!< used to store the current GeneratorStatus
    bool exposureError;                 //!< used to set the error condition during the exposure sequence
    unsigned char error_code;           //!< used to set the error code during the exposure sequence
};
#endif // EXPOSURE_MANAGER_H
