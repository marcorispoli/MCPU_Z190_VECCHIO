#include "application.h"


/**
 * @brief EventMessage
 *
 * This function formats the EventMessage EVENT frame.
 *
 * This EVENT is sent to Gantry always an error message \n
 * is received from the Sedecal Generator.
 *
 * The command frame is: <E SEQ EventMessage error_message_string >
 *
 * @param
 * - seq: is the sequence code of the EVENT frame;
 * - msg: error message string sent to the Client.
 *
 * \ingroup InterfaceModule
 */
void Server::EventMessage(ushort seq, QString msg){
    QList<QString> event;    
    event.append("EventMessage");
    event.append(msg);
    sendEvent(seq, &event);
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
 * The command frame is: <E SEQ EventSetXrayEna signal_stat > \n
 *
 * Where:
 * - signal_stat:
 *  - 0 = request to clear the signal;
 *  - 1 = request to activate the signal;
 *
 * @param
 * - seq: is the sequence code of the EVENT frame;
 * - state:
 *  - true: activation request;
 *  - false: deactivation request;
 *
 * \ingroup InterfaceModule
 */
void Server::EventSetXrayEna(ushort seq, bool state){

    QList<QString> event;
    event.append("EventSetXrayEna");
    if(state) event.append(QString("%1").arg((uchar)1));
    else event.append(QString("%1").arg((uchar)0));
    sendEvent(seq, &event);
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
 * The command frame is: <E SEQ EventGetPulseData > \n
 *
 *
 * @param
 * - seq: is the sequence code of the EVENT frame;
 *
 * \ingroup InterfaceModule
 */
void Server::EventGetPulseData(ushort seq){

    QList<QString> event;
    event.append("EventGetPulseData");
    sendEvent(seq, &event);

    // This is a convenient function of the Debug Window
    if(WINDOW) WINDOW->EventGetPulseData(seq);
    return ;
}

/**
 * @brief EventXrayCompleted
 *
 * This function formats the EventXrayCompleted EVENT frame.
 *
 * This EVENT is sent to Gantry aftyer an Exposure sequence completes.
 *
 * The command frame is: <E SEQ EventXrayCompleted ris_code masPre masPulse error > \n
 *
 * Where:
 * - ris_code: is one of the ocdes in Interface::tExposureResult;
 * - masPre: total mAs for the pre pulse (if there is one)
 * - masPulse: total mAs for pulse
 * - error: is one of the error codes in the Interface::tExposurErrors
 *
 * @param
 * - seq: is the sequence code of the EVENT frame;
 * - result: is the exposure result code. see Interface::tExposureResult;
 * - preMas: total mAs for pre pulse
 * - pulseMas: total mAs for pulse
 * - error: in case of error, this is the error code. See Interface::tExposurErrors.
 *
 * \ingroup InterfaceModule
 */
void Server::EventXrayCompleted(ushort seq, Interface::tExposureResult result, float preMas, float pulseMas, uchar error){

    QList<QString> event;
    event.append("EventXrayCompleted");
    event.append(QString("%1").arg(result));
    event.append(QString("%1").arg(preMas));
    event.append(QString("%1").arg(pulseMas));
    event.append(QString("%1").arg(error));
    sendEvent(seq, &event);
    return ;
}

/**
 * @brief EventStatus
 *
 * This function formats the EventStatus EVENT frame.
 *
 * This EVENT is sent to Gantry when the Generator Internal status changes.
 *
 * The command frame is: <E SEQ EventStatus genstat anodeHu genHu filStat rotSpeed>
 *
 * Where:
 *  - genstat: is the code of the internal generator status. See R2CP::tGenStatus_Stat;
 *  - anodeHu: is the percentage of HU cumulated into the Anode;
 *  - genHu: is the percentage of HU cumulated into the Generator;
 *  - filStat: 0=OFF, 1 =ON;
 *  - rotSpeed: 0=OFF, 1= LOW SPEED, 2=HIGH SPEED;
 *
 * @param
 *  - seq: is the sequence code of the EVENT frame;
 *
 *
 * \ingroup InterfaceModule
 */
void Server::EventStatus(ushort seq){

    if(WINDOW) WINDOW->EventStatus();

    uchar stat = R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.GeneratorStatus;
    uchar anodeHU = R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.AccumulatedAnodeHU;
    uchar generatorHU = R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.AccumulatedGenHU ;
    bool filamentStat = R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.ExposureSwitches.Fields.FilStatus;
    uchar rotSpeed = R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.CurrentRotorSpeed;

    QList<QString> event;
    event.append("EventStatus");
    event.append(QString("%1").arg(stat));
    event.append(QString("%1").arg(anodeHU));
    event.append(QString("%1").arg(generatorHU));
    event.append(QString("%1").arg(filamentStat));
    event.append(QString("%1").arg(rotSpeed));


    sendEvent(seq, &event);
    return ;
}

