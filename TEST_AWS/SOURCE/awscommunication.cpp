#include "awscommunication.h"
#include <QStringConverter>
#include "startupWindow.h"


extern startupWindow*      window;

awsCommunication::awsCommunication(QString address, int port, QObject *parent)
    :  awsInterface(address, port, parent)
{
    void rxFromAws(QByteArray);
    void rxFromGantry(QByteArray);

    connect(this,SIGNAL(transmitFrameSgn(QByteArray)),this,SLOT(rxFromGantry(QByteArray)),Qt::UniqueConnection);
    connect(this,SIGNAL(receivedFrameSgn(QByteArray)),this,SLOT(rxFromAws(QByteArray)),Qt::UniqueConnection);
    connect(this,&awsCommunication::sendAsyncSgn,this,&awsCommunication::sendAsyncSlot,Qt::QueuedConnection);


    study = false;

    trx_fault = false;
    arm_enabled = true;
    arm_fault = false;

    ProjectionList.append("LCC");
    ProjectionList.append("LFB");
    ProjectionList.append("LISO");
    ProjectionList.append("LLM");
    ProjectionList.append("LLMO");
    ProjectionList.append("LML");
    ProjectionList.append("LMLO");
    ProjectionList.append("LSIO");
    ProjectionList.append("RCC");
    ProjectionList.append("RFB");
    ProjectionList.append("RISO");
    ProjectionList.append("RLM");
    ProjectionList.append("RLMO");
    ProjectionList.append("RML");
    ProjectionList.append("RMLO");
    ProjectionList.append("RSIO");
    setProjectionList.clear();



    selectedTomoId = "";
    selectedTomoConfiguration = "";


    readyForExposure = true;

}

void awsCommunication::bind(QString ip, uint port){
    this->restartServer(ip, port);
}

void awsCommunication::rxFromGantry(QByteArray data){
    // Decodes the data based on the Encoding format
    auto toUtf = QStringDecoder(QStringConverter::Encoding::Utf16LE);
    QString frame = toUtf(data);


    QString ddate = QDateTime::currentDateTimeUtc().toString();
    window->addLogEvent(ddate + " |           GANTRY:> " + data);

}

void awsCommunication::rxFromAws(QByteArray data){
    // Decodes the data based on the Encoding format
    auto toUtf = QStringDecoder(QStringConverter::Encoding::Utf16LE);
    QString frame = toUtf(data);

    QString ddate = QDateTime::currentDateTimeUtc().toString();
    window->addLogEvent(ddate + " |    AWS:> " + data);

}





void awsCommunication::EXEC_OpenStudy(void){
    if(protocol.getParams()->count() !=1 ) {sendNok(0, "INVALID PARAMETER NUMBER"); return;}

    if(study) sendNok(1, "ALREADY_OPEN_STUDY");
    else{
        if(this->protocol.getParams()->size())
            PatientName = this->protocol.getParams()->at(0);
        else PatientName = "";
        study = true;
        sendOk(0);

        window->setStudyName(PatientName);
    }
}

void awsCommunication::EXEC_Close(void){
    sendOk(0);
    study = false;
    PatientName = "";
    window->setStudyName(PatientName);

}

void awsCommunication::GET_RotationAngles(void){
    QList<QString> params;

    params.append(QString("%1").arg((int) window->getTrx()));
    params.append(QString("%1").arg((int) window->getArm()));
    params.append(QString("%1").arg((int) window->getTilt()));


    sendOk(&params);

}

void awsCommunication::GET_Accessories(void){
    QList<QString> params;

    params.append(window->getPotter());
    params.append(window->getPaddle());
    params.append(window->getAccessory());

    sendOk(&params);

}

void awsCommunication::trxCompletedSlot(void){
     disconnect(window, SIGNAL(trxCompletedSgn), this, SLOT(trxCompletedSlot));
     emit sendAsyncSgn(this->trxProtocol, 0, nullptr);
}


void awsCommunication::EXEC_TrxPosition(void){
    int angolo;

    if(protocol.getParams()->count() !=2 ){ sendNok(0, "INVALID PARAMETERS"); return;}
    if(!study){ sendNok(1, "ONLY_IN_OPEN_STUDY"); return;}
    if(window->rotBusy()){ sendNok(2, "TRX_BUSY"); return;}
    if(trx_fault){ sendNok(4, "TRX_IN_FAULT"); return;}

    uint speed = 400;
    if(protocol.getParams()->at(0) == "ANGLE"){
        angolo = protocol.getParams()->at(1).toInt();
        if( (angolo < -2700) || (angolo > 2700)) sendNok(6, "TARGET ANGLE NOT ALLOWED");
    }else  if(protocol.getParams()->at(0) == "TOMO_HOME"){
        speed = abs(window->getTomoRun(protocol.getParams()->at(1)));
        angolo = window->getTomoHome(protocol.getParams()->at(1));
    }else if(protocol.getParams()->at(0) == "TOMO_FINAL"){
        speed = abs(window->getTomoRun(protocol.getParams()->at(1)));
        angolo = window->getTomoFinal(protocol.getParams()->at(1));
    }else{
        sendNok(0, "INVALID PARAMETERS");
        return;
    }

    trxProtocol = protocol;
    connect(window, SIGNAL(trxCompletedSgn), this, SLOT(trxCompletedSlot), Qt::UniqueConnection);
    if(!window->moveTrx(angolo, speed)){
        disconnect(window, SIGNAL(trxCompletedSgn), this, SLOT(trxCompletedSlot));
        sendOk(0);
        return;
    }else{

        sendOk(255);
        return;

    }

}

void awsCommunication::armCompletedSlot(void){
    disconnect(window, SIGNAL(armCompletedSgn), this, SLOT(armCompletedSlot));
    emit sendAsyncSgn(this->armProtocol, 0, nullptr);

}
void awsCommunication::EXEC_ArmPosition(void){
    int angolo, from, to;
    QString projection;

    if(protocol.getParams()->count() !=4 ){ sendNok(0, "INVALID PARAMETERS"); return;}

    if(!study){ sendNok(1, "ONLY_IN_OPEN_STUDY"); return;}
    if(window->rotBusy()){ sendNok(2, "ARM_BUSY"); return;}
    if(!arm_enabled){ sendNok(3, "ARM_DISABLED: BREAST COMPRESSED"); return;}
    if(arm_fault){ sendNok(4, "ARM_IN_FAULT"); return;}
    projection = protocol.getParams()->at(0);

    // Check Projection
    if(!setProjectionList.contains(projection)){ sendNok(5, "WRONG PROJECTION NAME"); return;}
    angolo = protocol.getParams()->at(1).toInt();
    if((angolo > 180)|| (angolo < -180)) { sendNok(6, "ANGLE OUT OF RANGE"); return;}
    from = protocol.getParams()->at(2).toInt();
    if((from > 180)|| (from < -180)) { sendNok(6, "ANGLE OUT OF RANGE"); return;}
    to = protocol.getParams()->at(3).toInt();
    if((to > 180)|| (to < -180)) { sendNok(6, "ANGLE OUT OF RANGE"); return;}
    if(from > to) { sendNok(6, "ANGLE OUT OF RANGE"); return;}

    if(!window->selectProjection(projection, from,to)) { sendNok(5, "PROJECTION NOT IN THE VALID LIST"); return;}

    armProtocol = protocol;
    connect(window, SIGNAL(armCompletedSgn), this, SLOT(armCompletedSlot), Qt::UniqueConnection);
    if(!window->moveArm(angolo, 1000)){
        disconnect(window, SIGNAL(armCompletedSgn), this, SLOT(armCompletedSlot));
        sendOk(0);
        return;
    }else{
        sendOk(255);
        return;
    }

    return;
}

void awsCommunication::SET_ProjectionList(void){
    if(!study){ sendNok(1, "ONLY_IN_OPEN_STUDY"); return;}
    if(protocol.getParams()->count()==0){
        setProjectionList.clear();
    }

    // Verifies the list content
    setProjectionList.clear();
    for(int i=0; i<protocol.getParams()->count(); i++ ){
        if(!ProjectionList.contains(protocol.getParams()->at(i))){
            sendNok(2, "WRONG PROJECTION NAME"); return;
        }
        setProjectionList.append(protocol.getParams()->at(i));
    }

    window->setProjectionList(setProjectionList);
    sendOk(0);

}

void awsCommunication::EXEC_AbortProjections(void){
    if(protocol.getParams()->count() !=0 ){ sendNok(0, "INVALID PARAMETERS"); return;}
    if(!study){ sendNok(1, "ONLY_IN_OPEN_STUDY"); return;}
    window->selectProjection("---", 0,0);    
    sendOk(0);
}

void awsCommunication::GET_CompressorData(void){

    QList<QString> params;

    params.append(window->getThick());
    params.append(window->getForce());
    params.append(window->getPaddle());
    sendOk(&params);
}

void awsCommunication::GET_TomoConfigFile(void){
    if(protocol.getParams()->count() !=1 ){ sendNok(0, "INVALID PARAMETERS"); return;}

    QString filename = protocol.getParams()->at(0);
    if(!window->selectTomoConfigFile(filename)) { sendNok(1, "CONFIG FILE NOT FOUND"); return;}
    sendOk(0);
}

void awsCommunication::GET_TomoConfig(void){
    if(protocol.getParams()->count() !=1 ){ sendNok(0, "INVALID PARAMETERS"); return;}

    QString ID = protocol.getParams()->at(0);

    QList<tomoConfig> lista;
    lista =  window->tomoCfg.value(window->getTomoCfgFile(),lista);
    if(lista.count()==0) { sendNok(1, "INVALID IDENTIFIER OR CONFIGURATION FILE NOT SELECTED"); return;}

    window->selectTomoConfig(window->getTomoCfgFile(), ID);
    QList<QString> params;

    for(int i=0; i<lista.count(); i++){
        if(lista.at(i).id == ID){
            params.append(lista.at(i).home);
            params.append(lista.at(i).final);
            params.append(lista.at(i).acc);
            params.append(lista.at(i).dec);
            params.append(lista.at(i).run);
            params.append(lista.at(i).sample);
            params.append(lista.at(i).skeep);
            sendOk(&params);
            return;
        }
    }
    sendNok(1, "INVALID IDENTIFIER OR CONFIGURATION FILE NOT SELECTED");
    return;
}


void awsCommunication::EXEC_ResetError(void){
    window->cancelError();
    sendOk(0);
}

void awsCommunication::powerdownSlot(void){
    QApplication::quit();
}

void awsCommunication::EXEC_PowerOff(void){
    if(study){ sendNok(1, "ONLY_IN_CLOSE_STUDY"); return;}
    sendOk(0);
    window->setMessage("SYSTEM POWER OFF IN 5 SECONDS", 5000);
    QTimer::singleShot(5000, this, &awsCommunication::powerdownSlot);

}

void awsCommunication::GET_TubeTemperature(void){
    QList<QString> params;


    params.append(QString("%1").arg(window->getTubeHu()));
    params.append(QString("%1").arg(window->getTubeTemp()));
    sendOk(&params);
}

void awsCommunication::SET_CompressionRelease(void){
     if(protocol.getParams()->count() !=1 ){ sendNok(0, "INVALID PARAMETERS"); return;}
     QString status = protocol.getParams()->at(0);

     if (status == "KEEP") window->setCompressionReleaseMode(false);
     else window->setCompressionReleaseMode(true);

    sendOk(0);
}

void awsCommunication::SET_XrayPushEnable(void){
    if(!study){ sendNok(1, "ONLY_IN_OPEN_STUDY"); return;}
    if(!readyForExposure){ sendNok(2, "NOT READY FOR EXPOSURE"); return;}
    window->setXrayEnable(true);
    sendOk(0);
}

void awsCommunication::GET_ReadyForExposure(void){
    int err = window->checkReadyForExposure();
    if(err == 0) sendOk(0);
    else sendNok(err);
}

void awsCommunication::GET_XrayPushStatus(void){

    if(!window->getXrayPush()) sendOk(0);
    else sendOk(1);
}

void awsCommunication::SET_ExposureMode(void){
    if(!study){ sendNok(1, "ONLY_IN_OPEN_STUDY"); return;}
    if(protocol.getParams()->count() !=4 ){ sendNok(0, "INVALID PARAMETERS"); return;}

    int err = window->setXrayExposureMode(protocol.getParams()->at(0),
                                          protocol.getParams()->at(1),
                                          protocol.getParams()->at(2),
                                          protocol.getParams()->at(3));

    if(err) { sendNok(err, "INVALID PARAMETERS VALUE"); return;}
    sendOk(0);
}

void awsCommunication::EXEC_StartXraySequence(void){
    if(!study){ sendNok(1, "ONLY_IN_OPEN_STUDY"); return;}
    if(protocol.getParams()->count() !=3 ){ sendNok(0, "INVALID PARAMETERS"); return;}
    if(!window->getXrayReady()){ sendNok(2, "NOT READY"); return;}
    if(!window->getXrayPush()) { sendNok(3, "X-RAY PUSH NOT PRESSED"); return;}

    int err = window->setXraySequenceData(protocol.getParams()->at(0),
                                          protocol.getParams()->at(1),
                                          protocol.getParams()->at(2));

    if(err) { sendNok(err, "INVALID PARAMETERS VALUE"); return;}

    // Start sequence
    sendOk(0);
    window->startXray();
}

void awsCommunication::SET_PulseData(void){
    if(!study){ sendNok(1, "ONLY_IN_OPEN_STUDY"); return;}
    if(protocol.getParams()->count() !=4 ){ sendNok(0, "INVALID PARAMETERS"); return;}

    int err = window->setXrayPulseData(protocol.getParams()->at(0),
                                          protocol.getParams()->at(1),
                                          protocol.getParams()->at(2),
                                          protocol.getParams()->at(3));

    if(err) { sendNok(err, "INVALID PARAMETERS VALUE"); return;}
    sendOk(0);
}


//________________________________________________________________________________//

void awsCommunication::gantryReqProjection(QString item){
    GANTRY_SelectProjection(item);
}
void awsCommunication::gantryReqAbortProjection(void){
    GANTRY_AbortProjection();
}
void awsCommunication::gantrySetAccessory(QString potter, QString paddle, QString colli){
    GANTRY_SetAccessories(potter, paddle, colli);
}
void awsCommunication::gantrySetTubeTemperature(QString anode, QString housing){
    GANTRY_SetTubeTemperature(anode, housing);
}

void awsCommunication::gantryXrayPushEvent(void){
    GANTRY_XrayPushEvent();
}

void awsCommunication::gantryCompressorData(QString thick, QString force){
    GANTRY_CompressorData(thick, force);
}

void awsCommunication::gantryError(QString error){
    GANTRY_ERROR(error);
}
void awsCommunication::gantryXraySequenceCompleted(QString result){
    GANTRY_XraySequenceCompleted(result);
}

void awsCommunication::gantryPulseCompleted(void){
    GANTRY_PulseCompleted();
}






