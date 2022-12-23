#include "application.h"


/**
 * @brief EventMessage
 *
 * This function formats the EventMessage EVENT frame.
 *
 * This EVENT is sent to Gantry always an error message \n
 * is received from the Sedecal Generator.
 *
 * The command frame is: <E EventMessage error_message_string >
 *
 * @param
 * - msg: error message string sent to the Client.
 *
 */
void Interface::EventMessage(QString msg){
    QList<QString> params;
    params.append(msg);
    sendEvent("EventMessage", &params);
    return ;
}


/**
 * @brief EventSetXrayEna
 *
 * This function formats the EventSetXrayEna EVENT frame.
 *
 * This EVENT is sent to Gantry when the Application needs to
 * change the status of the Gantry XRAY_ENA signal on the Hardware Bus.
 *
 * The command frame is: <E EventSetXrayEna signal_stat > \n
 *
 * Where:
 * - signal_stat:
 *  - 0 = request to clear the signal;
 *  - 1 = request to activate the signal;
 *
 * @param
 * - state:
 *  - true: activation request;
 *  - false: deactivation request;
 *
 */
void Interface::EventSetXrayEna(bool state){

    QList<QString> params;
    if(state) params.append(QString("%1").arg((uchar)1));
    else params.append(QString("%1").arg((uchar)0));
    sendEvent("EventSetXrayEna", &params);
    return ;
}

/**
 * @brief EventGetPulseData
 *
 * This function formats the EventGetPulseData EVENT frame.
 *
 * This EVENT is sent to Gantry when the Application needs to
 * request the Exposure Pulse data, during an AEC sequence,\n
 * after the pre pulse has been completed.
 *
 * The command frame is: <E EventGetPulseData > \n
 *
 */
void Interface::EventGetPulseData(void){

    sendEvent("EventGetPulseData",nullptr);

    // This is a convenient function of the Debug Window
    if(WINDOW) WINDOW->EventGetPulseData();
    return ;
}

/**
 * @brief EventXrayCompleted
 *
 * This function formats the EventXrayCompleted EVENT frame.
 *
 * This EVENT is sent to Gantry aftyer an Exposure sequence completes.
 *
 * The command frame is: <E EventXrayCompleted ris_code masPre masPulse error > \n
 *
 * Where:
 * - ris_code: is one of the ocdes in exposureManager::tExposureResult;
 * - masPre: total mAs for the pre pulse (if there is one)
 * - masPulse: total mAs for pulse
 * - error: is one of the error codes in the Application::tExposureErrors
 *
 * @param
 * - result: is the exposure result code. see exposureManager::tExposureResult;
 * - preMas: total mAs for pre pulse
 * - pulseMas: total mAs for pulse
 * - error: in case of error, this is the error code. See Application::tExposureErrors.
 *
 */
void Interface::EventXrayCompleted(uchar result, float preMas, float pulseMas, uchar error){

    QList<QString> params;

    params.append(QString("%1").arg(result));
    params.append(QString("%1").arg(preMas));
    params.append(QString("%1").arg(pulseMas));
    params.append(QString("%1").arg(error));
    sendEvent("EventXrayCompleted", &params);
    return ;
}

/**
 * @brief EventStatus
 *
 * This function formats the EventStatus EVENT frame.
 *
 * This EVENT is sent to Gantry when the Generator Internal status changes.
 *
 * The command frame is: <E EventStatus genstat anodeHu genHu filStat rotSpeed>
 *
 * Where:
 *  - genstat: is the code of the internal generator status. See R2CP::tGenStatus_Stat;
 *  - anodeHu: is the percentage of HU cumulated into the Anode;
 *  - genHu: is the percentage of HU cumulated into the Generator;
 *  - filStat: 0=OFF, 1 =ON;
 *  - rotSpeed: 0=OFF, 1= LOW SPEED, 2=HIGH SPEED;
 *
 *
 */
void Interface::EventStatus(void){

    if(WINDOW) WINDOW->EventStatus();

    uchar stat = R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.GeneratorStatus;
    uchar anodeHU = R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.AccumulatedAnodeHU;
    uchar generatorHU = R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.AccumulatedGenHU ;
    bool filamentStat = R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.ExposureSwitches.Fields.FilStatus;
    uchar rotSpeed = R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.CurrentRotorSpeed;

    QList<QString> params;
    params.append(QString("%1").arg(stat));
    params.append(QString("%1").arg(anodeHU));
    params.append(QString("%1").arg(generatorHU));
    params.append(QString("%1").arg(filamentStat));
    params.append(QString("%1").arg(rotSpeed));

    sendEvent("EventStatus", &params);
    return ;
}

