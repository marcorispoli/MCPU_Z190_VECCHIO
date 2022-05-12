#include "gantrydevices.h"



void gantryDevices::deviceReceptionHandler(uchar id, canDriverInterface::canDataFrame frame){

    // Validate frame
    uchar crc=0;
    for( int i=0; i<8; i++)  crc ^= frame.data[i];
    if(crc){
        dbg_rxerrors++;
        return;
    }

    // Find the device target
    for(int i = 0; i< pDeviceList.size(); i++) {
        if(pDeviceList[i]->getId() == id){
             if(!pDeviceList[i]->decodeFrame(&frame)){
                 dbg_rxerrors++;
                 return;
             }
             return;
        }
    }
}

