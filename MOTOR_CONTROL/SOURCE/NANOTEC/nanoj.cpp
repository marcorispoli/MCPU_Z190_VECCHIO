#include "application.h"
#include "pd4_dictionary.h"

void pd4Nanotec::nanojWritedata(void){


    if(nanojStr.txBlock.size() < 8){
        nanojStr.txBlock[0] |= 1; // End of data block
        nanojStr.txBlock[0] |= (8 - nanojStr.txBlock.size()) * 2; // Ignore ll the data bit

        for(int i = nanojStr.txBlock.size(); i<8; i++ ) nanojStr.txBlock.append((uchar) 0);
    }

    emit txToCan(0x600 + deviceId, nanojStr.txBlock);

    // Activate the timeout handler, in case no answer should be received
    nanojStr.rxblock = true;
    sdoRxTx.sdo_rx_ok = false;
    sdoRxTx.sdo_rxtx_completed = false;
    sdoRxTx.sdo_rx_tx_pending = true;

}

#define NANOJ_CONTROL   OD_1F51_02
#define NANOJ_DATA      OD_1F50_02_INIT
#define NANOJ_STATUS    OD_1F57_02




ushort pd4Nanotec::subNanojProgramUpload(bool force){
    static unsigned short vmmchk = 0;
    static bool flag;

    switch(wSubStatus){
    case 0:
        if(nanojStr.vector == nullptr){
            qDebug() << QString("DEVICE (%1): NANOJ PROGRAM NOT PRESENT").arg(deviceId);
            nanojUploaded = true;
            return 0;
        }

        vmmchk = 0;
        // Calculate the checksum
        for(unsigned int i=0; i < nanojStr.sizeofNanoj;i++) vmmchk += (ushort) nanojStr.vector[i];

        readSDO(NANOJ_USER_PARAM); // Reads the user parameter containing the stored nanoj checksum
        wSubStatus++;
        return 5;

    case 1:
        if(!sdoRxTx.sdo_rx_ok) {
            qDebug() << QString("DEVICE (%1): ERROR READING OD %2.%3").arg(deviceId).arg(sdoRxTx.txSDO.getIndex(),1,16).arg(sdoRxTx.txSDO.getSubIndex());
            wSubStatus = 0;
            return 0;
        }

        wSubStatus++;
        return 1;

    case 2:

        if(( sdoRxTx.rxSDO.getVal() == vmmchk) && (!force)){

            nanojUploaded = true;
            qDebug() << QString("DEVICE (%1): NANOJ-PROGRAM ALREADY UPLOADED").arg(deviceId);
            wSubStatus = 0;
            return 0;
        }
        writeSDO(NANOJ_CONTROL, VMM_INIT); // initialize flash
        wSubStatus++;
        return 5;

    case 3:
        if(!sdoRxTx.sdo_rx_ok) {
            qDebug() << QString("DEVICE (%1): ERROR WRITING OD %2.%3").arg(deviceId).arg(sdoRxTx.txSDO.getIndex(),1,16).arg(sdoRxTx.txSDO.getSubIndex());
            wSubStatus = 0;
            return 0;
        }

        qDebug() << QString("DEVICE (%1): DELETE FLASH").arg(deviceId);
        writeSDO(NANOJ_CONTROL, VMM_DELETE); // Delete flash
        wSubStatus++;
        return 5;

    case 4:
        if(!sdoRxTx.sdo_rx_ok) {
            qDebug() << QString("DEVICE (%1): ERROR WRITING OD %2.%3").arg(deviceId).arg(sdoRxTx.txSDO.getIndex(),1,16).arg(sdoRxTx.txSDO.getSubIndex());
            wSubStatus = 0;
            return 0;
        }

        // Reset Node
        resetNode();
        qDebug() << QString("DEVICE (%1): RESET NODE").arg(deviceId);
        wSubStatus++;
        return 2000;

    case 5:
        readSDO(OD_2700_02); // Read an arbitrary register to be sure that the reset node is completed
        wSubStatus++;
        return 100;

    case 6:
        if(!sdoRxTx.sdo_rx_ok) {
            qDebug() << QString("DEVICE (%1): ERROR READING OD %2.%3").arg(deviceId).arg(sdoRxTx.txSDO.getIndex(),1,16).arg(sdoRxTx.txSDO.getSubIndex());
            wSubStatus = 0;
            return 0;
        }


        writeSDO(NANOJ_CONTROL, VMM_INIT); // Initialize again the Flash
        wSubStatus++;
        return 5;

    case 7:
        if(!sdoRxTx.sdo_rx_ok) {
            qDebug() << QString("DEVICE (%1): ERROR WRITING OD %2.%3").arg(deviceId).arg(sdoRxTx.txSDO.getIndex(),1,16).arg(sdoRxTx.txSDO.getSubIndex());
            wSubStatus = 0;
            return 0;
        }


        nanojStr.vector_index = 0;
        wSubStatus++;
        return 1;

    case 8:
        writeSDO(NANOJ_DATA,0); // Initialize the  Data Ram  -----------------------------------------------------------------------------------------------
        wSubStatus++;
        return 5;

    case 9:
        if(!sdoRxTx.sdo_rx_ok) {
            qDebug() << QString("DEVICE (%1): ERROR WRITING OD %2.%3").arg(deviceId).arg(sdoRxTx.txSDO.getIndex(),1,16).arg(sdoRxTx.txSDO.getSubIndex());
            wSubStatus = 0;
            return 0;
        }
        nanojStr.num_block_items = 0;
        flag = false;
        wSubStatus++;
        return 1;

    case 10: // Writes max 1024 the data in ram -------------------
        nanojStr.txBlock.clear();
        if(flag) nanojStr.txBlock.append((uchar) 0x10);
        else nanojStr.txBlock.append((uchar) 0x00);
        flag = !flag;


        for(int i=0; i<7; i++){
            if(nanojStr.vector_index < nanojStr.sizeofNanoj){
                nanojStr.txBlock.append((uchar) nanojStr.vector[nanojStr.vector_index]);
                nanojStr.num_block_items++;
                nanojStr.vector_index++;
                if(nanojStr.num_block_items == 1024) break;
            }else break;
        }

        nanojWritedata();
        wSubStatus++;
        return 5;

    case 11:
        if(!sdoRxTx.sdo_rx_ok) {
            qDebug() << QString("DEVICE (%1): ERROR WRITING NANOJ BLOCK").arg(deviceId);
            wSubStatus = 0;
            return 0;
        }
        wSubStatus++;
        return 1;

    case 12:
        if((nanojStr.vector_index == nanojStr.sizeofNanoj) || (nanojStr.num_block_items == 1024)){
            wSubStatus = 13;
            return 1;
        }

        wSubStatus = 10;
        return 1;

    case 13: // Store Data ram in Flash
        writeSDO(NANOJ_CONTROL, VMM_WRITE);
        wSubStatus++;
        return 5;

    case 14:
        if(!sdoRxTx.sdo_rx_ok) {
            qDebug() << QString("DEVICE (%1): ERROR STORING DATA IN FLASH").arg(deviceId);
            wSubStatus = 0;
            return 0;
        }

        readSDO(NANOJ_CONTROL);
        wSubStatus++;
        return 5;

    case 15:
        if(!sdoRxTx.sdo_rx_ok) {
            qDebug() << QString("DEVICE (%1): ERROR STORING DATA IN FLASH (READ CONTROL)").arg(deviceId);
            wSubStatus = 0;
            return 0;
        }
        if(sdoRxTx.rxSDO.getVal() != 0){
            wSubStatus--;
            return 100;
        }

        wSubStatus++;
        return 1;

    case 16:
        readSDO(NANOJ_STATUS);
        wSubStatus++;
        return 5;

    case 17:
        if(!sdoRxTx.sdo_rx_ok) {
            qDebug() << QString("DEVICE (%1): ERROR STORING DATA IN FLASH (READ STATUS)").arg(deviceId);
            wSubStatus = 0;
            return 0;
        }

        if(sdoRxTx.rxSDO.getVal() != 0){
            qDebug() << QString("DEVICE (%1): ERROR READING STATUS").arg(deviceId);
            wSubStatus = 0;
            return 0;
        }

        if(nanojStr.vector_index == nanojStr.sizeofNanoj){
            // Transfer completed
            wSubStatus++;
            return 1;
        }
        wSubStatus = 8;
        return 1;

     case 18:
        qDebug() << "SAVE THE CHECKSUM OF THE NANOJ PROGRAM";
        writeSDO(NANOJ_USER_PARAM, vmmchk);
        wSubStatus++;
        return 5;

    case 19:
        if(!sdoRxTx.sdo_rx_ok) {
            qDebug() << QString("DEVICE (%1): ERROR SAVING THE CHECKSUM").arg(deviceId);
            wSubStatus = 0;
            return 0;
        }

        writeSDO(RESET_USER_PARAM,RESET_CODE); // Reset the User param of the Reset flag
        wSubStatus++;
        return 5;

     case 20:
        if(!sdoRxTx.sdo_rx_ok) {
            qDebug() << QString("DEVICE (%1): ERROR WRITING THE RESET FLAG IN USER PARAM").arg(deviceId);
            wSubStatus = 0;
            return 0;
        }

        // Saves the User parameters
        writeSDO(SAVE_USER_PARAM, 1);
        wSubStatus++;
        return 5;

    case 21:
        if(!sdoRxTx.sdo_rx_ok) {
            qDebug() << QString("DEVICE (%1): ERROR SAVING THE CHECKSUM").arg(deviceId);
            wSubStatus = 0;
            return 0;
        }

        wSubStatus++;
        return 1;

    case 22:
        readSDO(SAVE_USER_PARAM);
        wSubStatus++;
        return 5;

    case 23:
        if(!sdoRxTx.sdo_rx_ok) {
            qDebug() << QString("DEVICE (%1): ERROR READING THE USER PARAM CONTROL REGISTER").arg(deviceId);
            wSubStatus = 0;
            return 0;
        }

        if(sdoRxTx.rxSDO.getVal()!=0){
            wSubStatus--;
            return 100;
        }


        qDebug() << "NANOJ PROGRAM SUCCESSFULLY UPLOADED";
        nanojUploaded = true;
        wSubStatus = 0;
        return 0;


    }


}

