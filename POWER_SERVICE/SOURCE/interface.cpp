#include "server.h"
#include <QStringConverter>

// Protocol from the Master to the Device Server
void SocketItem::socketRxData()
{
    QByteArray dataFrame;

    if(socket->bytesAvailable()==0) return;
    QByteArray data = socket->readAll();
    if(data.size() < 3) return;



}

