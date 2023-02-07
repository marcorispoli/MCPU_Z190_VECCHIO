#define _HTONS_IMPLEMENTATION
#include "application.h"
#include <QTimer>


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

static int16_t sendCR2CPData(byte *pMessage , word datalength){
    return COMMUNICATION->sendData(pMessage, datalength);
}

Communication::Communication( QObject *parent)
{

    connect(&client, SIGNAL(clientConnection(bool)), this, SLOT(clientConnection(bool)), Qt::UniqueConnection);
    connect(&client, SIGNAL(rxData(QByteArray)), this, SLOT(clientRxData(QByteArray)), Qt::QueuedConnection);
    connection_status = false;
    smartHubConnected = false;
    generatorConnected = false;

    // CR2CP Initialization
    R2CP_Eth = new CR2CP_Eth(sendCR2CPData, 0, (CaDataDic*) R2CP::CaDataDicGen::GetInstance(), 0,0);
    R2CP::CaDataDicGen::GetInstance()->Initialitation();
    R2CP::CaDataDicGen::GetInstance()->SetCommunicationForm(R2CP_Eth);
}

void Communication::smartHubConnectionEvent(void){
    smartHubConnected = true;
    qDebug() << "SH Connected";
}

void Communication::generatorConnectionEvent(bool stat){

    if(generatorConnected != stat){
        generatorConnected = stat;
        if(stat) qDebug() << "Generator Connected";
        else qDebug() << "Generator Disconnected";
    }

}

void Communication::generatorReceivedStatusEvent(void){
    if(!generatorConnected)   qDebug() << "Generator Connected";
    generatorConnected = true;


    INTERFACE->EventStatus();
}

void Communication::generatorReceivedProcedureDefinitionEvent(byte id)
{

}

void Communication::PostExposureEvent(uchar db_number, uchar foc, float kV, float mAs, float mA, float ms, uchar result){
    EXPOSURE->setPostExposureData(db_number,foc, kV, mAs, mA, ms, result);
}


void Communication::clientConnection(bool stat){
    connection_status = stat;
    smartHubConnected = false;
    generatorConnected = false;

    if(stat ==true){
        qDebug() << "Socket Connected";
    }else{
        qDebug() << "Socket Disonnected";
    }

}

void Communication::start(void){

    client.Start(QString(Application::SH_IP_ADDRESS), (int) Application::SH_PORT);
}

int16_t Communication::sendData(byte *pMessage , word datalength){
    QByteArray data;
    for(int i=0; i<datalength; i++) data.append((char) pMessage[i]);

    COMMUNICATION->client.txData(data);
    return datalength;
}


void Communication::clientRxData(QByteArray data){

    if( !R2CP_Eth ) return;

    uint framelen;
    QByteArray nextdata = data;
    QByteArray curdata;

    while(1){
        if(nextdata.size() < 8) return;
        framelen = nextdata[6] * 256 + nextdata[7];
        if(nextdata.size() > framelen + 8){
            curdata = nextdata.left(framelen + 8);
            emit rxDataEventSgn(curdata);
            nextdata = nextdata.right(nextdata.size() - framelen - 8);
            R2CP_Eth->ProcessMessage((byte*) curdata.data());
        }else{
            emit rxDataEventSgn(nextdata);
            R2CP_Eth->ProcessMessage((byte*) nextdata.data());
            return;
        }
    }

    return;
}


