#include "canopeninterface.h"


bool canOpenInterface::readOdRegister(int odIndex,uint tmo){

    if(odIndex >= deviceRegisters.count()) return false;

    // Register already under reading process
    if(deviceRegisters[odIndex].getOdStat() == odRegister::_OD_ACTION_READ_PENDING) return true;

    // Send the frame to the can driver
    deviceRegisters[odIndex].setOdStat(odRegister::_OD_ACTION_READ_PENDING);
    canTxFrame(deviceRegisters[odIndex].toReadCanFrame(), odIndex,tmo);
    return true;
}

bool canOpenInterface::writeOdRegister(int odIndex,uint tmo){

    if(odIndex >= deviceRegisters.count()) return false;

    // Register already under reading process
    if(deviceRegisters[odIndex].getOdStat() == odRegister::_OD_ACTION_WRITE_PENDING) return true;

    // Send the frame to the can driver
    deviceRegisters[odIndex].setOdStat(odRegister::_OD_ACTION_WRITE_PENDING);
    canTxFrame(deviceRegisters[odIndex].toWriteCanFrame(), odIndex,tmo);
    return true;
}

bool canOpenInterface::executeCommand(int odIndex,uint tmo){

    if(odIndex >= deviceRegisters.count()) return false;

    // Command already requested and under processing
    if(deviceRegisters[odIndex].getOdStat() == odRegister::_OD_ACTION_COMMAND_REQUEST) return true;
    if(deviceRegisters[odIndex].getOdStat() == odRegister::_OD_ACTION_COMMAND_EXECUTING) return true;

    // Writes the command register
    deviceRegisters[odIndex].setOdStat(odRegister::_OD_ACTION_COMMAND_REQUEST);
    canTxFrame(deviceRegisters[odIndex].toWriteCanFrame(), odIndex,tmo);
    return true;
}



 void canOpenInterface::deviceRxSlot(canDriverInterface::canDataFrame frame,  int odIndex){
    if(odIndex >= deviceRegisters.count()) return;
    if(nodeId != odRegister::getNodeId(frame)){
        deviceRegisters[odIndex].setOdStat(odRegister::_OD_ACTION_TYPE_FORMAT_ERROR);
        emit registerActionCompleteSgn(odIndex);
        return;
    }

    if(odRegister::isReadFrame(frame)){
         if(deviceRegisters[odIndex].getOdStat() == odRegister::_OD_ACTION_COMMAND_EXECUTING){

             // Command is completed.
             deviceRegisters[odIndex].setOdStat(odRegister::_OD_ACTION_COMMAND_COMPLETED);

         }else if(deviceRegisters[odIndex].getOdStat() == odRegister::_OD_ACTION_READ_PENDING){

            // Set the proper error status and error code
            deviceRegisters[odIndex].setOdStat(odRegister::_OD_ACTION_READ_COMPLETED);
            deviceRegisters[odIndex].setData(frame);
        }else{
            deviceRegisters[odIndex].setOdStat(odRegister::_OD_ACTION_TYPE_FORMAT_ERROR);
        }

    }else if(odRegister::isWriteFrame(frame)){

        if(deviceRegisters[odIndex].getOdStat() == odRegister::_OD_ACTION_COMMAND_REQUEST){

            // If this is a Command check if the command is accepted or already executed
            if(frame.data[4] == canOpenInterface::CMD_READY){
                deviceRegisters[odIndex].setOdStat(odRegister::_OD_ACTION_COMMAND_COMPLETED);
            }else if(frame.data[4] == canOpenInterface::CMD_EXECUTING){
                deviceRegisters[odIndex].setOdStat(odRegister::_OD_ACTION_COMMAND_EXECUTING);
            }else deviceRegisters[odIndex].setOdStat(odRegister::_OD_ACTION_TYPE_FORMAT_ERROR);

        }else if(deviceRegisters[odIndex].getOdStat() == odRegister::_OD_ACTION_WRITE_PENDING){
            deviceRegisters[odIndex].setOdStat(odRegister::_OD_ACTION_WRITE_COMPLETED);
            deviceRegisters[odIndex].setData(frame);
        }else{
            deviceRegisters[odIndex].setOdStat(odRegister::_OD_ACTION_TYPE_FORMAT_ERROR);
        }

    }else{

        if(deviceRegisters[odIndex].getOdStat() == odRegister::_OD_ACTION_COMMAND_REQUEST){
            deviceRegisters[odIndex].setOdStat(odRegister::_OD_ACTION_COMMAND_REQUEST_ERROR);
        } else if(deviceRegisters[odIndex].getOdStat() == odRegister::_OD_ACTION_COMMAND_EXECUTING){
            deviceRegisters[odIndex].setOdStat(odRegister::_OD_ACTION_COMMAND_COMPLETED_WITH_ERROR);
        }else if(deviceRegisters[odIndex].getOdStat() == odRegister::_OD_ACTION_READ_PENDING){
            deviceRegisters[odIndex].setOdStat(odRegister::_OD_ACTION_READ_ERROR);
        }else if(deviceRegisters[odIndex].getOdStat() == odRegister::_OD_ACTION_WRITE_PENDING){
            deviceRegisters[odIndex].setOdStat(odRegister::_OD_ACTION_WRITE_ERROR);
        }

        // Set the proper error status and error code
        deviceRegisters[odIndex].setError(frame);
    }

    // Emit the action completed signal
    emit registerActionCompleteSgn(odIndex);
}





