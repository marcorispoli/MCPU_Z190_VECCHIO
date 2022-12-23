#include "application.h"


/**
 * @brief SetExposurePre
 *
 * Gantry sends this Command in order to set the next
 * pre-pulse exposure data in an AEC Exposure type (2D or 3D).
 *
 * The frame format is: <C SEQ SetExposurePre kV mAs focus tmo>
 *
 * @param
 *  - kV: (float)  kV of the pre pulse;
 *  - mAs: (float) mAs of the pre-pulse;
 *  - focus: (string) [LARGE/SMALL] sets the focal spot to be used;
 *      > NOTE: in case of Tomo sequence, the focus is always Large
 *  - tmo: timeout waiting AEC data after pre pulse.
 *
 * @return
 * - 1: in case of wong number of parameters;
 * - 0: in case of command success;
 *
 */
uint Interface::SetExposurePre(QList<QString>* command, QList<QString>* answer){

    if(answer) answer->clear();
    if(command->size() != 7) {
        return 1;
    }
    exposureManager::tExposureFocus focus = exposureManager::_FOCUS_SMALL;
    if(command->at(5) == "LARGE") focus = exposureManager::_FOCUS_LARGE;

    EXPOSURE->setExposureOptions(true, true); // Use both detector synch and grid synch
    EXPOSURE->setPreData(focus, command->at(3).toFloat(), command->at(4).toFloat(), command->at(6).toUShort());

    return 0;
}

/**
 * @brief SetExposurePulse
 *
 * Gantry sends this Command in order to set the next Pulse exposure data.
 *
 * Gantry shall set the Pulse exposure data before to starts a Manual exposure (2D or 3D)\n
 * or in AEC to set the pulse after the Pre Pulse is completed.
 *  > This function uses the Synchronization signals and cannot be used as a test exposure.
 *
 * The frame format is: <C SEQ SetExposurePulse kV mAs focus >
 *
 *
 * @param
 *  - kV: (float)  ;
 *  - mAs: (float) The *Total* number of mAs of the Pulse;
 *      > In case of the Tomo sequence, the mAs of a single pulse is the mAs/n-pulse!
 *  - focus: (string) : LARGE/SMALL;
 *      > In case of Tomo only the LARGE FOCUS is used, and this parameter is ignored;
 *
 * @return
 * - 1: in case of wong number of parameters;
 * - 0: in case of command success;
 */
uint Interface::SetExposurePulse(QList<QString>* command, QList<QString>* answer){
    if(answer) answer->clear();

    if(command->size() != 6) {
        return 1;
    }

    exposureManager::tExposureFocus focus = exposureManager::_FOCUS_SMALL;
    if(command->at(5) == "LARGE") focus = exposureManager::_FOCUS_LARGE;

    EXPOSURE->setExposureOptions(true, true); // Use both detector synch and grid synch
    EXPOSURE->setPulseData(focus, command->at(3).toFloat(), command->at(4).toFloat());
    return 0;
}




/**
 * @brief SetExposureTestPulse
 *
 * Gantry sends this Command in order to set the next Pulse exposure data.
 *
 * Gantry shall set the Pulse exposure data before to starts a Manual exposure (2D or 3D)\n
 * or after the pre pulse termines and the Pulse data has been calculated.
 *
 * The frame format is: <C SEQ SetExposurePulse kV mAs focus sync_type grid_use>
 *
 *
 * @param
 *  - kV: (float)  ;
 *  - mAs: (float) The *Total* number of mAs of the Pulse;
 *      > In case of the Tomo sequence, the mAs of a single pulse is the mAs/n-pulse!
 *  - focus: (string) : LARGE/SMALL;
 *      > In case of Tomo this parameter is ignored;
 *  - sync_type: (string) [NO_DETECOR/DETECTOR];
 *      - NO_DETECTOR: the synch signal will be ignored;
 *      - DETECTOR: the Generator will synchronize the exposure with the Synch signal of the Hardware Bus;
 *          > In case of Tomo this parameter is ignored;
 *  - grid_use: (string) grid synch: NO_GRID/GRID
 *      - NO_GRID: the Generator will ignore the Grid synchronization;
 *      - GRID: the Generator will synchronize the exposure with the Grid signal of the Hardware Bus;
 *          > In case of Tomo this parameter is ignored;
 *
 * @return
 * - 1: in case of wong number of parameters;
 * - 0: in case of command success;
 *
 */
uint Interface::SetExposureTestPulse(QList<QString>* command, QList<QString>* answer){
    if(answer) answer->clear();

    if(command->size() != 8) {
        return 1;
    }

    exposureManager::tExposureFocus focus = exposureManager::_FOCUS_SMALL;
    if(command->at(5) == "LARGE") focus = exposureManager::_FOCUS_LARGE;

    bool detector, grid;
    if(command->at(6) == "NO_DETECTOR") detector = false;
    else detector =true;
    if(command->at(7) == "NO_GRID") grid = false;
    else grid =true;

    EXPOSURE->setPulseData(focus, command->at(3).toFloat(), command->at(4).toFloat());
    EXPOSURE->setExposureOptions(detector, grid);
    return 0;
}


/**
 * @brief SetTomoConfig
 *
 * Gantry sends this Command in order to set the Tomo configuration.\n
 * The Tomo configuration is composed by the Number of pulses, number of pulse-skip\n
 * and the maximum integration time.
 *
 * The frame format is: <C SEQ SetTomoConfig n_pulses n_skip tmo>
 *
 *
 * @param
 *  - n_pulses: (uchar)  Number of *effective* tomo pulses in the scan;
 *  - n_skip: (ushort) Number of Detector Exposure Window the generator shall skip before to start XRAY pulses;
 *  - tmo (ushort) Maximum integration time in ms
 *
 * @return
 * - 1: in case of wong number of parameters;
 * - 0: in case of command success;
 *
 */
uint Interface::SetTomoConfig(QList<QString>* command, QList<QString>* answer){
    if(answer) answer->clear();

    if(command->size() != 6) {
        return 1;
    }

    EXPOSURE->setTomoConfig(command->at(3).toUShort(), command->at(4).toUShort(), command->at(5).toUShort());
    return 0;
}


/**
 * @brief StartExposure
 *
 * Gantry sends this Command in order to start a given Exposure.\n
 * Possible Exposures are:
 *  - 2D: is a Manual single 2D pulse exposure;
 *  - 2DAEC: is a 2D exposure with pre-pulse and a second pulse based on the pre;
 *  - 3D: i a Manual 3D exposure (tomo);
 *  - 3DAEC: is a 3D exposure with pre-pulse and a subsequent tomo scan based on the pre;
 *
 *
 * The frame format is: <C SEQ StartExposure exposure_type >
 *
 *
 * @param
 *  - exposure_type: (string) [2D/2DAEC/3D/3DAEC];
 *
 * @return
 * - 1: in case of wong number of parameters;
 * - 2: in case of not Idle Status;
 * - 0: in case of command success;
 */
uint Interface::StartExposure(QList<QString>* command, QList<QString>* answer){
    if(answer) answer->clear();

    if(command->size() != 4) {
        return 1;
    }

    if(!STATUS->isIdle()){
        return 2;
    }

    // Clear the Post Exposure datalist
    if(command->at(3) == "2D") STATUS->start2DExposure();
    else if(command->at(3) == "2DAEC") STATUS->start2DAecExposure();
    else if(command->at(3) == "3D") STATUS->start3DExposure();
    else if(command->at(3) == "3DAEC") STATUS->start3DAecExposure();

    return 0;
}

/**
 * @brief AbortExposure
 *
 * This Command termines an incoming Exposure.
 *
 * The frame format is: <C SEQ AbortExposure  >
 *
 * @param
 *  - No parameters..
 *
 * @return
 * - 0: always success
 *
 */
uint Interface::AbortExposure(QList<QString>* command, QList<QString>* answer){
    if(answer) answer->clear();

    STATUS->requestAbortProcedure();
    return 0;
}

/**
 * @brief GetPostExposure
 *
 * Gantry sends this Command to request the list of the\n
 * last exposure pulses.
 *
 * The frame format is: <C SEQ GetPostExposure  >
 *
 * @param
 *  - No parameters..
 *
 * @return
 * - "OK 0 num_blocks [block1] .. [block-n]"
 *
 * Where:
 *  - num_blocks: number of available pulse blocks;
 *  - block 1-n: pulse data-blocks;
 *
 *  The data block format is:[focus, kV, mAs, mA, ms]:
 *  - focus: [SMALL/LARGE] is the focal spot used;
 *  - kV: kV of the pulse;
 *  - mAs of the pulse;
 *  - mA: anodic current of the pulse;
 *  - ms: endurance in ms of the pulse;
 *
 *
 */
uint Interface::GetPostExposure(QList<QString>* command, QList<QString>* answer){
    if(answer) answer->clear();

    answer->append(QString("%1").arg(EXPOSURE->postExposure.size()));

    for(int i=0; i< EXPOSURE->postExposure.size(); i++){
        if(EXPOSURE->postExposure.at(i).focus == exposureManager::_FOCUS_LARGE)  answer->append("LARGE");
        else answer->append("SMALL");
//        answer->append(QString("%1").arg(list.at(i).pulse_seq));
        answer->append(QString("%1").arg(EXPOSURE->postExposure.at(i).kV));
        answer->append(QString("%1").arg(EXPOSURE->postExposure.at(i).mAs));
        answer->append(QString("%1").arg(EXPOSURE->postExposure.at(i).mA));
        answer->append(QString("%1").arg(EXPOSURE->postExposure.at(i).mS));
//        answer->append(QString("%1").arg(list.at(i).result));
    }

    return 0;
}



