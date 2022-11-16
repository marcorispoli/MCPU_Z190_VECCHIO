#include "application.h"

/*
 *      PROTOCOL COMMAND FORMAT:
 *      command->at(0) == "E"
 *      command->at(1) == Sequence
 *      command->at(2) == Command
 *
 *
 */

/**
 * @brief Server::SetExposurePre
 * @param command
 *  - (3): (float)  kV;
 *  - (4): (float) mAs;
 *  - (5): (string) focus: LARGE/SMALL;
 *
 * @return
 */
QList<QString> Server::SetExposurePre(QList<QString>* command){
    QList<QString> answer;    

    if(command->size() != 6) {
        answer.append("0 NOK 1");
        return answer;
    }

    Interface::tExposureFocus focus = Interface::_FOCUS_SMALL;
    if(command->at(5) == "LARGE") focus = Interface::_FOCUS_LARGE;

    STATUS->setPreData(focus, command->at(3).toFloat(), command->at(4).toFloat());
    answer.append(command->at(1)); // SEQ
    answer.append("OK");
    return answer;
}

/**
 * @brief Server::SetExposurePulse
 * @param command
 *  - (3): (float)  kV;
 *  - (4): (float) mAs;
 *  - (5): (string) focus: LARGE/SMALL;
 *  - (6): (string) detector synch: NO_DETECOR/DETECTOR
 *  - (7): (string) grid synch: NO_GRID/GRID
 *
 * @return
 */
QList<QString> Server::SetExposurePulse(QList<QString>* command){
    QList<QString> answer;

    if(command->size() != 8) {
        answer.append("0 NOK 1");
        return answer;
    }

    Interface::tExposureFocus focus = Interface::_FOCUS_SMALL;
    if(command->at(5) == "LARGE") focus = Interface::_FOCUS_LARGE;

    bool detector, grid;
    if(command->at(6) == "NO_DETECTOR") detector = false;
    else detector =true;
    if(command->at(7) == "NO_GRID") grid = false;
    else grid =true;

    STATUS->setPulseData(focus, command->at(3).toFloat(), command->at(4).toFloat(), detector, grid);
    answer.append(command->at(1)); // SEQ
    answer.append("OK");
    return answer;
}

/**
 * @brief Server::SetTomoConfig
 * @param command
 *  - (3): (uchar)  n-samples;
 *  - (4): (ushort) n-skip;
 *
 * @return
 */
QList<QString> Server::SetTomoConfig(QList<QString>* command){
    QList<QString> answer;

    if(command->size() != 5) {
        answer.append("0 NOK 1");
        return answer;
    }

    answer.append(command->at(1)); // SEQ

    if(STATUS->setTomoConfig(command->at(3).toUShort(), command->at(4).toUShort())) answer.append("OK");
    else answer.append("NOK 2");
    return answer;
}


/**
 * @brief Server::StartExposure
 * @param command
 *  - (3): (string)  exposure_type: "2D"/"2DAEC"/"3D"/"3DAEC";
 * @return
 */
QList<QString> Server::StartExposure(QList<QString>* command){
    QList<QString> answer;

    if(command->size() != 4) {
        answer.append("NOK 1");
        return answer;
    }

    if(!STATUS->isIdle()){
        answer.append("NOK 2");
        return answer;
    }

    // Clear the Post Exposure datalist
    if(command->at(3) == "2D") STATUS->start2DExposure();
    else if(command->at(3) == "2DAEC") STATUS->start2DAecExposure();
    else if(command->at(3) == "3D") STATUS->start3DExposure();

    answer.append(command->at(1)); // SEQ
    answer.append("OK");
    return answer;
}

/**
 * @brief Server::AbortExposure
 * @param command
 *  - No parameters..
 * @return
 */
QList<QString> Server::AbortExposure(QList<QString>* command){
    QList<QString> answer;

    STATUS->requestAbortProcedure();

    answer.append(command->at(1)); // SEQ
    answer.append("OK");
    return answer;
}


QList<QString> Server::GetPostExposure(QList<QString>* command){
    QList<QString> answer;

    QList<Interface::tPostExposureData> list = STATUS->getPostExposureList();

    answer.append(command->at(1)); // SEQ
    answer.append("OK");
    answer.append(QString("%1").arg(list.size()));

    for(int i=0; i< list.size(); i++){
        answer.append(QString("%1").arg(list.at(i).pulse_seq));
        answer.append(QString("%1").arg(list.at(i).foc));
        answer.append(QString("%1").arg(list.at(i).kV));
        answer.append(QString("%1").arg(list.at(i).mAs));
        answer.append(QString("%1").arg(list.at(i).mA));
        answer.append(QString("%1").arg(list.at(i).ms));
        answer.append(QString("%1").arg(list.at(i).result));
    }


    return answer;
}



