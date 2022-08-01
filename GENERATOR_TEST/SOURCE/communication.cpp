#define _HTONS_IMPLEMENTATION
#include "communication.h"
#include <QStringConverter>
#include "startupWindow.h"
#include "Typedef.h"
#include <QtEndian>


unsigned short htons(unsigned short val){
    unsigned short us;
    qToBigEndian<unsigned short>(val, &us);
    return us;
}
unsigned int htonl(unsigned int val){
    unsigned short ui;
    qToBigEndian<unsigned int>(val, &ui);
    return ui;
}

extern startupWindow* window;
extern Communication* pComm;

static int16_t sendCR2CPData(byte *pMessage , word datalength){
    return pComm->sendData(pMessage, datalength);
}

Communication::Communication( QObject *parent)
{
    pComm = this;
    connect(&client, SIGNAL(clientConnection(bool)), this, SLOT(clientConnection(bool)), Qt::UniqueConnection);
    connect(&client, SIGNAL(rxData(QByteArray)), this, SLOT(clientRxData(QByteArray)), Qt::UniqueConnection);
    connection_status = false;

    R2CP_Eth = new CR2CP_Eth(sendCR2CPData, 0, (CaDataDic*) R2CP::CaDataDicGen::GetInstance(), 0,0);
    R2CP::CaDataDicGen::GetInstance()->Initialitation();
    R2CP::CaDataDicGen::GetInstance()->SetCommunicationForm(R2CP_Eth);
}

void Communication::clientConnection(bool stat){
    connection_status = stat;
    byte sh_id = 1;
    byte loc_id = 3;

    window->connectionStat(stat);


    if(stat ==true){
        qDebug() << "Socket Connected";
        R2CP::CaDataDicGen::GetInstance()->Network_ConnectionRequest_Event(sh_id, loc_id);
    }else qDebug() << "Socket Disonnected";

}

void Communication::start(void){
    client.Start(IP_ADDRESS, HUB_PORT);
}

int16_t Communication::sendData(byte *pMessage , word datalength){
    QByteArray data;
    for(int i=0; i<datalength; i++) data.append((char) pMessage[i]);

    pComm->client.txData(data);
    return datalength;
}

void Communication::clientRxData(QByteArray data){

    byte* dataPtr = (byte*) data.data();
    if( R2CP_Eth )   R2CP_Eth->ProcessMessage(dataPtr);
    return;
}

