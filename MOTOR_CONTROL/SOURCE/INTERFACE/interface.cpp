#include "application.h"
#include <QStringConverter>

/**
 * @brief Server::Server
 *
 * @param
 * - ipaddress: IP where server will be bounded;
 * - port: bounding port
 *
 */
Interface::Interface(QString ipaddress, int port):QTcpServer()
{
    socketList.clear();
    localip = QHostAddress(ipaddress);
    localport = port;
    idseq=0;

    interfaceStatus = _SYSTEM_CONFIGURATION;
    wStatus = 0;
    configuration.configuration_ok = false;
    QTimer::singleShot(1,this, SLOT(handleInterfaceStatus()));
}

/**
 * @brief Server::~Server
 *
 * When the server should be destroyed, it shall disconnect all the clients
 * already connected.
 *
 */
Interface::~Interface()
{
    if(socketList.size()){
        for(int i = 0; i<socketList.size(); i++ ){
            if(socketList[i]->socket != nullptr) {
                socketList[i]->socket->close();
                socketList[i]->socket->deleteLater();
            }
        }
    }
}

void Interface::handleInterfaceStatus(void){
    ushort delay;
    switch(interfaceStatus){
        case _SYSTEM_CONFIGURATION:
            delay = handleInterfaceConfigurationStatus();
            if(!delay){
                wStatus =0;
                interfaceStatus = _SYSTEM_RUNNING;
            }
            break;


    case _SYSTEM_RUNNING:
        delay = handleInterfaceRunningStatus();
        if(!delay){
            wStatus =0;
            interfaceStatus = _SYSTEM_CONFIGURATION;
        }
        break;

        default: delay = 100;
    }

    QTimer::singleShot(delay,this, SLOT(handleInterfaceStatus()));
}

ushort Interface::handleInterfaceConfigurationStatus(void){
    switch(wStatus){
        case 0:
            // Stops the Device communication until the configuration is received from the master
            if(TRX)  TRX->stop();
            if(ARM)  ARM->stop();
            if(BODY) BODY->stop();
            if(LIFT) LIFT->stop();
            if(SLIDE) SLIDE->stop();
            wStatus++;
            return 1;

        case 1: // Waits for the system stop mode ack
            if((TRX) && (!TRX->isStopMode())) return 100;
            if((ARM) && (!ARM->isStopMode())) return 100;
            if((LIFT) && (!LIFT->isStopMode())) return 100;
            if((BODY) && (!BODY->isStopMode())) return 100;
            if((SLIDE) && (!SLIDE->isStopMode())) return 100;
            qDebug() << " INTERFACE IN STOP MODE";
            wStatus++;
            return 1;

        case 2: // Wait the configuration from the Master
            if(!configuration.configuration_ok) return 100;
            wStatus++;
            return 1;

        case 3: // Run the TRX configuration -------------------------
            if(!TRX){
                wStatus = 7;
                return 1;
            }

            TRX->run();
            TRX->enableConfiguration();
            wStatus++;
            return 1;

        case 4: // Waits the CONFIG status of the TRX
            if(!TRX->isOperating()) return 100;
            wStatus++;
            return 1;

        case 5: // Waits the configuraiton Completion
            if(!TRX->activateZeroSetting()) return 100;
            wStatus++;
            return 1;

        case 6: // Waits the configuraiton Completion
            if(TRX->isCommandExecuting()) return 100;
            if(TRX)  TRX->stop();
            wStatus++;
            return 1;

        case 7: // Run the ARM configuration -------------------------
            if(!ARM){
                wStatus = 11;
                return 1;
            }

            ARM->run();
            ARM->enableConfiguration();
            wStatus++;
            return 1;

        case 8: // Waits the CONFIG status of the TRX
            if(!ARM->isOperating()) return 100;
            wStatus++;
            return 1;

        case 9: // Waits the configuraiton Completion
            if(!ARM->activateZeroSetting()) return 100;
            wStatus++;
            return 1;

        case 10: // Waits the configuraiton Completion
            if(ARM->isCommandExecuting()) return 100;
            ARM->stop();
            wStatus++;
            return 1;

        case 11: // Run the LIFT configuration -------------------------
            if(!LIFT){
                wStatus = 15;
                return 1;
            }

            LIFT->run();
            LIFT->enableConfiguration();
            wStatus++;
            return 1;

        case 12: // Waits the CONFIG status of the TRX
            if(!LIFT->isOperating()) return 100;
            wStatus++;
            return 1;

        case 13: // Waits the configuraiton Completion
            if(!LIFT->activateZeroSetting()) return 100;
            wStatus++;
            return 1;

        case 14: // Waits the configuraiton Completion
            if(LIFT->isCommandExecuting()) return 100;
            LIFT->stop();
            wStatus++;
            return 1;

        case 15: // Run the BODY configuration -------------------------
            if(!BODY){
                wStatus = 19;
                return 1;
            }

            BODY->run();
            BODY->enableConfiguration();
            wStatus++;
            return 1;

        case 16: // Waits the CONFIG status of the TRX
            if(!BODY->isOperating()) return 100;
            wStatus++;
            return 1;

        case 17: // Waits the configuraiton Completion
            if(!BODY->activateZeroSetting()) return 100;
            wStatus++;
            return 1;

        case 18: // Waits the configuraiton Completion
            if(BODY->isCommandExecuting()) return 100;
            BODY->stop();
            wStatus++;
            return 1;


        case 19: // Run the BODY configuration -------------------------
            if(!SLIDE){
                wStatus = 200;
                return 1;
            }

            SLIDE->run();
            SLIDE->enableConfiguration();
            wStatus++;
            return 1;

        case 20: // Waits the CONFIG status of the TRX
            if(!SLIDE->isOperating()) return 100;
            wStatus++;
            return 1;

        case 21: // Waits the configuraiton Completion
            if(!SLIDE->activateZeroSetting()) return 100;
            wStatus++;
            return 1;

        case 22: // Waits the configuraiton Completion
            if(SLIDE->isCommandExecuting()) return 100;
            SLIDE->stop();
            wStatus = 200;
            return 1;


        case 200:
            if(TRX)  TRX->run();
            if(ARM)  ARM->run();
            if(LIFT)  LIFT->run();
            if(BODY)  BODY->run();
            if(SLIDE)  SLIDE->run();

            qDebug() << " CONFIGURATION COMPLETED";
            return 0;

    }

    return 10;
}

ushort Interface::handleInterfaceRunningStatus(void){
   return 100;

}
