#include "application.h"

/*
 *      PROTOCOL COMMAND FORMAT:
 *
 *      "Sequence Command Params .."
 *
 *
 */

void Server::EventMessage(ushort seq, QString msg){


    QList<QString> event;
    event.append(QString("%1").arg(seq));
    event.append("EventMessage");
    event.append(msg);

    sendEvent(&event);
    return ;
}



void Server::EventSetXrayEna(ushort seq, bool state){

    QList<QString> event;
    event.append(QString("%1").arg(seq));
    event.append("EventSetXrayEna");
    if(state) event.append(QString("%1").arg((uchar)1));
    else event.append(QString("%1").arg((uchar)0));
    sendEvent(&event);
    return ;
}

void Server::EventGetPulseData(ushort seq){

    QList<QString> event;
    event.append(QString("%1").arg(seq));
    event.append("EventGetPulseData");
    sendEvent(&event);
    return ;
}

void Server::EventXrayCompleted(ushort seq, uchar code, uchar error){

    QList<QString> event;
    event.append(QString("%1").arg(seq));
    event.append("EventXrayCompleted");
    event.append(QString("%1").arg(code));
    event.append(QString("%1").arg(error));
    sendEvent(&event);
    return ;
}

void Server::EventStatus(ushort seq, bool force){

    if(WINDOW) WINDOW->EventStatus();

    // Only in Standby sends the relevant status content to the target
    if((!force) &&(R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.GeneratorStatus != R2CP::Stat_Standby)) return;

    uchar stat = R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.GeneratorStatus;
    uchar anodeHU = R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.AccumulatedAnodeHU;
    uchar generatorHU = R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.AccumulatedGenHU ;
    bool filamentStat = R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.ExposureSwitches.Fields.FilStatus;
    uchar rotSpeed = R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.CurrentRotorSpeed;

    QList<QString> event;
    event.append(QString("%1").arg(seq));
    event.append("EventStatus");
    event.append(QString("%1").arg(stat));
    event.append(QString("%1").arg(anodeHU));
    event.append(QString("%1").arg(generatorHU));
    event.append(QString("%1").arg(filamentStat));
    event.append(QString("%1").arg(rotSpeed));


    sendEvent(&event);
    return ;
}

