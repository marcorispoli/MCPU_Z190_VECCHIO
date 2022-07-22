#ifndef COMMAND_INTERFACE_H
#define COMMAND_INTERFACE_H

#include "device_command_interface.h"
#include "power_service.h"


static void CONNECT_Command(deviceCommandInterface* parent, uint socketId, QList<QByteArray>* command);
static void DISCONNECT_Command(deviceCommandInterface* parent, uint socketId, QList<QByteArray>* command);
static void RECONNECT_Command(deviceCommandInterface* parent, uint socketId, QList<QByteArray>* command);
static void CONFIG_Command(deviceCommandInterface* parent, uint socketId, QList<QByteArray>* command);
static void QUIT_Command(deviceCommandInterface* parent, uint socketId, QList<QByteArray>* command);

#define EVENT_CONNECTION "CONNECTED"
#define EVENT_DISCONNECTION "DISCONNECTED"


#endif
