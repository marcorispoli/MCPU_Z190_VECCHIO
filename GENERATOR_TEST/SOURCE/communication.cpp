#include "communication.h"
#include <QStringConverter>
#include "startupWindow.h"


extern startupWindow*      window;

Communication::Communication( QObject *parent)
{

    connect(&client, SIGNAL(clientConnection(bool)), this, SLOT(clientConnection(bool)), Qt::UniqueConnection);
    connect(&client, SIGNAL(rxData(QByteArray)), this, SLOT(clientRxData(QByteArray)), Qt::UniqueConnection);
    connection_status = false;

    sequence = 0;
}

void Communication::clientConnection(bool stat){
    connection_status = stat;

    window->connectionStat(stat);

}

void Communication::start(void){
    client.Start(IP_ADDRESS, HUB_PORT);
}

void Communication::eventConnection(void){

    QByteArray data;
    data.append((uchar) GANTRY_ID );
    client.txData(buildFrame(1, sequence, SMART_HUB_ID, NETWORK_IDX, 0x4, _EVENT_FRAME, 1, data));

}


void Communication::clientRxData(QByteArray data){

    uchar dest = data.at(1);
    uchar idx = data.at(3);
    if((dest != GANTRY_ID) && (dest != BROADCAST_ID)){
        //window->addLogEvent(data);
        return;
    }


    if(idx == 0xA0) handleNetworkFrame(&data);
    else if(idx == 0x20) handleGeneratorFrame(&data);

    //window->addLogEvent(data);
    return;
}

void Communication::handleNetworkFrame(QByteArray* data){

    uchar val = data->at(0);
    uchar prio = val >> 6;
    uchar seq = val & 0x3F;    
    uchar srs = data->at(2);    
    uchar sub = data->at(4);
    uchar func = data->at(5);
    ushort len = data->at(6)*256 + data->at(7);


    QByteArray answer;
    if(sub == 0x3){ // HEARTBIT
        uchar node_id =  data->at(8);
//        uchar sequence = data->at(9);
        if(node_id != GANTRY_ID) return;

        answer.append((uchar) GANTRY_ID);
        answer.append((uchar) sequence++);

        client.txData(buildFrame(prio, seq, srs, NETWORK_IDX, 0x3, _EVENT_FRAME, 2, answer));
    }

    return;
}
void Communication::handleGeneratorFrame(QByteArray* data){


    return;
}
