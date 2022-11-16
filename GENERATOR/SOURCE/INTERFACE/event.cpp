#include "application.h"

/*
 *      PROTOCOL COMMAND FORMAT:
 *
 *      "Sequence Command Params .."
 *
 *
 */

void Server::EventMessage(ushort seq, QString msg){

#ifdef _WINDOW_ON
    WINDOW->EventMessage(seq, msg);
#endif

    QList<QString> event;
    event.append(QString("%1").arg(seq));
    event.append("EventMessage");
    event.append(msg);

    sendEvent(&event);
    return ;
}

void Server::EventExposureError(ushort seq, uchar code){
#ifdef _WINDOW_ON
    WINDOW->EventExposureError( seq,  code);
#endif
    QList<QString> event;
    event.append(QString("%1").arg(seq));
    event.append("EventExposureError");
    event.append(QString("%1").arg(code));
    sendEvent(&event);
    return ;
}

void Server::EventSetXrayEna(ushort seq, bool state){
#ifdef _WINDOW_ON
    WINDOW->EventSetXrayEna( seq,  state);
#endif

    QList<QString> event;
    event.append(QString("%1").arg(seq));
    event.append("EventSetXrayEna");
    if(state) event.append(QString("%1").arg((uchar)1));
    else event.append(QString("%1").arg((uchar)0));
    sendEvent(&event);
    return ;
}

void Server::EventGetPulseData(ushort seq){
#ifdef _WINDOW_ON
    WINDOW->EventGetPulseData( seq);
#endif

    QList<QString> event;
    event.append(QString("%1").arg(seq));
    event.append("EventGetPulseData");
    sendEvent(&event);
    return ;
}

void Server::EventXrayCompleted(ushort seq, uchar code, uchar error){
#ifdef _WINDOW_ON
    WINDOW->EventXrayCompleted( seq,  code,  error);
#endif

    QList<QString> event;
    event.append(QString("%1").arg(seq));
    event.append("EventXrayCompleted");
    event.append(QString("%1").arg(code));
    event.append(QString("%1").arg(error));
    sendEvent(&event);
    return ;
}

void Server::EventStatus(ushort seq, uchar stat){
#ifdef _WINDOW_ON
    WINDOW->EventStatus( seq,  stat);
#endif

    QList<QString> event;
    event.append(QString("%1").arg(seq));
    event.append("EventStatus");
    event.append(QString("%1").arg(stat));

    sendEvent(&event);
    return ;
}

void Server::EventSwError(ushort seq, uchar error){
#ifdef _WINDOW_ON
    WINDOW->EventSwError( seq,  error);
#endif

    QList<QString> event;
    event.append(QString("%1").arg(seq));
    event.append("EventSwError");
    event.append(QString("%1").arg(error));

    sendEvent(&event);
    return ;
}
