#include "application.h"


/**
 * @brief GetStatus
 *
 * Gantry sends this command EVENT in order to get the whole status of the devices
 *
 * The frame format is: <E SEQ GetStatus >
  *
 * @param
 *  - No parameters..
 *
 * @return
 * - "OK [trx-block] [arm-block] [lift-block] [body-block] [slide-block]"
 *
 * Where:
 *  - each block report the device status block;
 *
 *  The device block format is:
 *  - connection status: (String) [CONNECTED/NOT_CONNECTED];
 *  - operating status:  (String) [READY/NOT_ZERO_SET/NOT_INITIALIZED/ERROR]
 *  - current activity:  (String) [INITIALIZING/ZERO_SETTING/POSITIONING/NO_ACTIVITY]
 *  - encoder position:  (int)    hundredths of degree or millimeters
 *
 *
 * \ingroup InterfaceModule
 */
QList<QString> Interface::GetStatus(QList<QString>* command){
    QList<QString> answer;

/*
    answer.append("OK");
    answer.append(QString("%1").arg(EXPOSURE->postExposure.size()));

    for(int i=0; i< EXPOSURE->postExposure.size(); i++){
        if(EXPOSURE->postExposure.at(i).focus == exposureManager::_FOCUS_LARGE)  answer.append("LARGE");
        else answer.append("SMALL");
//        answer.append(QString("%1").arg(list.at(i).pulse_seq));
        answer.append(QString("%1").arg(EXPOSURE->postExposure.at(i).kV));
        answer.append(QString("%1").arg(EXPOSURE->postExposure.at(i).mAs));
        answer.append(QString("%1").arg(EXPOSURE->postExposure.at(i).mA));
        answer.append(QString("%1").arg(EXPOSURE->postExposure.at(i).mS));
//        answer.append(QString("%1").arg(list.at(i).result));
    }

    return answer;*/
}



