#include "server.h"

/*
 *      PROTOCOL COMMAND FORMAT:
 *
 *      "Sequence Command Params .."
 *
 *
 */

void Server::EventSysMessages(ushort seq, QList<ulong>* list){

    if(!list->size()) return;

    QList<QString> event;
    event.append(QString("%1").arg(seq));
    event.append("EventMessage");
    for(int i=0; i<list->size(); i++)
        event.append(QString("%1").arg(list->at(i)));

    sendEvent(&event);
    return ;
}

void Server::EventExposureError(ushort seq, uchar code){

    QList<QString> event;
    event.append(QString("%1").arg(seq));
    event.append("EventExposureError");
    event.append(QString("%1").arg(code));
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

void Server::EventStatus(ushort seq, uchar stat){

    QList<QString> event;
    event.append(QString("%1").arg(seq));
    event.append("EventStatus");
    event.append(QString("%1").arg(stat));

    sendEvent(&event);
    return ;
}

void Server::EventSwError(ushort seq, uchar error){

    QList<QString> event;
    event.append(QString("%1").arg(seq));
    event.append("EventSwError");
    event.append(QString("%1").arg(error));

    sendEvent(&event);
    return ;
}
