#include "application.h"
#include <QStringConverter>

/**
 * @brief Interface::Interface
 *
 * @param
 * - ipaddress: IP where server will be bounded;
 * - port: bounding port
 *
 */
Interface::Interface(QString ipaddress, int port):applicationInterface(ipaddress, port)
{
    interfaceStatus = _SYSTEM_CONFIGURATION;
    wStatus = 0;
    configuration.configuration_ok = false;
    QTimer::singleShot(1,this, SLOT(handleInterfaceStatus()));
}


void Interface::handleInterfaceStatus(void){
    ushort delay;
    switch(interfaceStatus){
        case _SYSTEM_CONFIGURATION:
            delay = handleInterfaceConfigurationStatus();
            break;

        case _SYSTEM_RUNNING:
            delay = handleInterfaceRunningStatus();
            break;

        case _SYSTEM_DEVICE_RESET:
            delay = handleInterfaceResetDeviceStatus();
            break;

        case _SYSTEM_FAULT:
            delay = handleInterfaceFaultStatus();
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
            qDebug() << "INTERFACE IS WAITING FOR THE CONFIGURATION";
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
            wStatus =0;
            interfaceStatus = _SYSTEM_RUNNING;
            return 0;

    }

    return 10;
}



ushort Interface::handleInterfaceResetDeviceStatus(void){
    static pd4Nanotec* devPtr = nullptr;

    switch(wStatus){
    case 0:
        qDebug() << "DEVICE IN RESET" ;

        if((TRX) && (TRX->isConfigMode()))    devPtr = (pd4Nanotec*) TRX;
        else if((ARM) && (ARM->isConfigMode()))    devPtr = (pd4Nanotec*) ARM;
        else if((LIFT) && (LIFT->isConfigMode()))    devPtr = (pd4Nanotec*) LIFT;
        else if((BODY) && (BODY->isConfigMode()))    devPtr = (pd4Nanotec*) BODY;
        else if((SLIDE) && (SLIDE->isConfigMode()))    devPtr = (pd4Nanotec*) SLIDE;
        else {
            wStatus =0;
            interfaceStatus = _SYSTEM_RUNNING;
            return 0;
        }

        if((devPtr != TRX) && (TRX)) TRX->stop();
        if((devPtr != ARM) && (ARM)) ARM->stop();
        if((devPtr != LIFT) && (LIFT) ) LIFT->stop();
        if((devPtr != BODY) && (BODY)) BODY->stop();
        if((devPtr != SLIDE) && (SLIDE) ) SLIDE->stop();

        wStatus++;
        return 1;

        case 1:
            // Wait for the Stop executed
            if((devPtr != TRX) && (TRX) && (!TRX->isStopMode())) return 100;
            if((devPtr != ARM) && (ARM) && (!ARM->isStopMode())) return 100;
            if((devPtr != LIFT) && (LIFT) && (!LIFT->isStopMode())) return 100;
            if((devPtr != BODY) && (BODY) && (!BODY->isStopMode())) return 100;
            if((devPtr != SLIDE) && (SLIDE) && (!SLIDE->isStopMode())) return 100;
            wStatus++;
            return 1;

        case 2: // Run the Device configuration -------------------------
            devPtr->run();
            devPtr->enableConfiguration();
            wStatus++;
            return 1;

        case 3: // Waits the CONFIG status of the TRX
            if(!devPtr->isOperating()) return 100;
            wStatus++;
            return 1;

        case 4: // Waits the configuraiton Completion
            if(!devPtr->activateZeroSetting()) return 100;
            wStatus++;
            return 1;

        case 5: // Waits the configuraiton Completion
            if(devPtr->isCommandExecuting()) return 100;
            wStatus++;
            return 1;

        case 6:
            if((devPtr != TRX) && (TRX)) TRX->run();
            if((devPtr != ARM) && (ARM)) ARM->run();
            if((devPtr != LIFT) && (LIFT) ) LIFT->run();
            if((devPtr != BODY) && (BODY)) BODY->run();
            if((devPtr != SLIDE) && (SLIDE) ) SLIDE->run();

            wStatus =0;
            interfaceStatus = _SYSTEM_RUNNING;
            return 0;

    }

   return 100;

}

ushort Interface::handleInterfaceFaultStatus(void){
    bool ltrx,larm,lbody,lslide,llift;

    switch(wStatus){
    case 0:
        qDebug() << " INTERFACE FAULT";
        wStatus++;
        return 100;

    case 1:

        ltrx = ((TRX) && (TRX->isFault()));
        larm = ((ARM) && (ARM->isFault()));
        llift = ((LIFT) && (LIFT->isFault()));
        lbody = ((BODY) && (BODY->isFault()));
        lslide = ((SLIDE) && (SLIDE->isFault()));


        if((ltrx||larm||llift||lbody||lslide)){
            return 100;
        }else{
            wStatus =0;
            interfaceStatus = _SYSTEM_RUNNING;
            return 0;
        }

    }

    return 100;
}

ushort Interface::handleInterfaceRunningStatus(void){
    static uchar trx_input = 0;
    static uchar arm_input = 0;
    static uchar lift_input = 0;
    static uchar body_input = 0;
    static uchar slide_input = 0;
    uchar ltrx_input;
    uchar larm_input;
    uchar llift_input;
    uchar lbody_input;
    uchar lslide_input;

    if(     ((TRX) && (TRX->isConfigMode())) ||
            ((ARM) && (ARM->isConfigMode()))||
            ((LIFT) && (LIFT->isConfigMode()))||
            ((BODY) && (BODY->isConfigMode()))||
            ((SLIDE) && (SLIDE->isConfigMode()))
            ){

            // A device or more device has been reset
            wStatus =0;
            interfaceStatus = _SYSTEM_DEVICE_RESET;
            return 0;
    }

    if(     ((TRX) && (TRX->isFault())) ||
            ((ARM) && (ARM->isFault()))||
            ((LIFT) && (LIFT->isFault()))||
            ((BODY) && (BODY->isFault()))||
            ((SLIDE) && (SLIDE->isFault()))
            ){

            // A device or more device has been reset
            wStatus =0;
            interfaceStatus = _SYSTEM_FAULT;
            return 0;
    }


    if((TRX) )


   switch(wStatus){
   case 0:
       qDebug() << " INTERFACE RUNNING";
       wStatus++;
       return 100;
   case 1:
       if(TRX) ltrx_input = TRX->getDigitalInputs();
       if(ARM) larm_input = ARM->getDigitalInputs();
       if(LIFT) llift_input = LIFT->getDigitalInputs();
       if(BODY) lbody_input = BODY->getDigitalInputs();
       if(SLIDE) lslide_input = SLIDE->getDigitalInputs();

      // if((ltrx_input != trx_input))
    return 100;
   }

   return 100;

}
