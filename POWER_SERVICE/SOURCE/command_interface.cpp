#include <QApplication>
#include "command_interface.h"

extern powerService*   pDevice;


void powerService::initCommandInterface(void){

    // Carica tutto il set di comandidi interfaccia
    commandInterface->commandList.insert("CONNECT" ,CONNECT_Command);
    commandInterface->commandList.insert("DISCONNECT" ,DISCONNECT_Command);
    commandInterface->commandList.insert("RECONNECT" ,RECONNECT_Command);
    commandInterface->commandList.insert("CONFIG" ,CONFIG_Command);
    commandInterface->commandList.insert("QUIT" ,QUIT_Command);

}

void CONNECT_Command(deviceCommandInterface* parent, uint socketId, QList<QByteArray>* command){
    QList<QByteArray> answer;
    answer.append(command->at(0));
    answer.append("OK");
    parent->sendToClient(socketId, answer);

    if(pDevice->Connect()){
        // Already connected
        answer.clear();
        answer.append(EVENT_CONNECTION);
        parent->sendToClient(0, answer);
    }

}

void DISCONNECT_Command(deviceCommandInterface* parent, uint socketId, QList<QByteArray>* command){
    QList<QByteArray> answer;
    answer.append(command->at(0));
    answer.append("OK");
    parent->sendToClient(socketId, answer);

    pDevice->Disconnect();
}

void RECONNECT_Command(deviceCommandInterface* parent, uint socketId, QList<QByteArray>* command){
    QList<QByteArray> answer;
    answer.append(command->at(0));
    answer.append("OK");
    parent->sendToClient(socketId, answer);

    pDevice->Reconnect();
}

void CONFIG_Command(deviceCommandInterface* parent, uint socketId, QList<QByteArray>* command){
    parent->sendToClient(socketId, *command);
}

void QUIT_Command(deviceCommandInterface* parent, uint socketId, QList<QByteArray>* command){
    parent->sendToClient(socketId, *command);
}
