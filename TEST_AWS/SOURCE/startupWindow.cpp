#include "startupWindow.h"
#include "ui_startupWindow.h"
#include "commandprotocol.h"
#include "awscommunication.h"

extern startupWindow* w1;
extern awsCommunication*   pAws;

startupWindow::startupWindow(QWidget *parent)
    : ui(new Ui::startupWindow)
{
    ui->setupUi(this);

    ui->languageLabel->setText(" CURRENT LANGUAGE: ENG");
    ui->messages->hide();

    timeEv = 0;
    trxBusy = false;
    armBusy = false;
    ui->trxEdit->setText("0");
    ui->armEdit->setText("0");
    ui->exposureTypeLabel->setText("");
    XraySequenceCode = "";
    sequenceStarted = false;
    ui->pulse0Frame->hide();
    ui->pulse1Frame->hide();
    ui->pulse2Frame->hide();
    ui->kV0->setText("kV:---");
    ui->kV1->setText("kV:---");
    ui->kV2->setText("kV:---");
    ui->mAs0->setText("mAs:---");
    ui->mAs1->setText("mAs:---");
    ui->mAs2->setText("mAs:---");
    ui->filter0->setText("Filter:---");
    ui->filter1->setText("Filter:---");
    ui->filter2->setText("Filter:---");


    timerTube = 0;
    tubeDeltaJouls =0;
    tubeJouls =0;
    tubeHu = 0;
    tubeTemp = 20;
    setTubeData(tubeHu,tubeTemp);

    XrayCodeList.clear();
    XrayCodeList.append("MAN_2D");
    XrayCodeList.append("AEC_2D");
    XrayCodeList.append("MAN_3D");
    XrayCodeList.append("AEC_3D");
    XrayCodeList.append("MAN_COMBO");
    XrayCodeList.append("AEC_COMBO");
    XrayCodeList.append("MAN_AE");
    XrayCodeList.append("AEC_AE");

    FilterList.clear();
    FilterList.append("Ag");
    FilterList.append("Al");
    FilterList.append("Mo");
    FilterList.append("Rh");
    FilterList.append("Cu");

    ColliList.clear();
    ColliList.append("COLLI_AUTO");
    ColliList.append("COLLI_24x30");
    ColliList.append("COLLI_18x24_C");
    ColliList.append("COLLI_18x24_L");
    ColliList.append("COLLI_18x24_R");
    ColliList.append("COLLI_9x21");
    ColliList.append("COLLI_10x24");
    ColliList.append("COLLI_PROSTHESIS");
    ColliList.append("COLLI_D75");
    ColliList.append("COLLI_BIOP2D");
    ColliList.append("COLLI_BIOP3D");
    ColliList.append("COLLI_TOMO");
    ColliList.append("COLLI_9x9");
    ColliList.append("COLLI_CUSTOM");

    ui->paddlesCombo->setCurrentText("PAD_UNDETECTED");
    ui->potterCombo->setCurrentText("POTTER_UNDETECTED");

    ui->thickSpin->setValue(0);
    ui->forceSpin->setValue(0);
    ui->thickSpin->setEnabled(false);
    ui->forceSpin->setEnabled(false);



    // Set the View to handle the rotation


    setStudyName("---");
    ui->trxEdit->setText("0");
    ui->armEdit->setText("0");
    ui->fromArmEdit->setText("---");
    ui->toArmEdit->setText("---");
    ui->armTiltEdit->setText("0");    
    ui->projectionsCombo->clear();
    ui->thickSpin->setValue(0);
    ui->forceSpin->setValue(0);

    // Projections
    ui->projectionsCombo->clear();
    ui->projectionsCombo->addItem("---");
    selectedProjection = "---";

    QList<tomoConfig>  config;
    tomoConfig item;

    item.set("WIDE","2400","-2400","400","200","200","19","2");
    config.append(item);
    item.set("INTERMEDIATE","1100","-1100","200","200","200","13","1");
    config.append(item);
    item.set("NARROW","750","-750","200","200","200","11","0");
    config.append(item);
    tomoCfg.insert("TOMO-CFG1", config);
    config.clear();

    item.set("WIDE","2700","-2700","300","300","300","24","1");
    config.append(item);
    item.set("INTERMEDIATE","1500","-1500","300","300","300","20","1");
    config.append(item);
    item.set("NARROW","1000","-1000","300","300","300","15","0");
    config.append(item);
    tomoCfg.insert("TOMO-CFG2", config);
    config.clear();

    item.set("WIDE","2700","-2700","400","400","400","25","1");
    config.append(item);
    item.set("INTERMEDIATE","1500","-1500","400","400","400","20","1");
    config.append(item);
    item.set("NARROW","1000","-1000","400","400","400","15","0");
    config.append(item);
    tomoCfg.insert("TOMO-CFG3", config);
    config.clear();

    QList<QString> lista;
    lista.append("TOMO-CFG1");
    lista.append("TOMO-CFG2");
    lista.append("TOMO-CFG3");
    ui->tomoCfgList->clear();
    ui->tomoCfgList->addItems(lista);


    lista.clear();
    lista.append("WIDE");
    lista.append("INTERMEDIATE");
    lista.append("NARROW");
    ui->tomoCfgId->clear();
    ui->tomoCfgId->addItems(lista);

    selectTomoConfig("TOMO-CFG1", "WIDE");



    setXrayEnable(false);
    xrayPushStat = false;
    ui->xrayPushButton->setText("X-RAY PUSH OFF");

    errorCondition = "";
    ui->errorFrame->hide();


    ui->xraySym->hide();
    ui->exposureFrame->hide();

    connect(this->ui->tomoCfgId, SIGNAL(currentTextChanged(QString)), this, SLOT(tomoIdChangedSlot(QString)));
    connect(this->ui->tomoCfgList, SIGNAL(currentTextChanged(QString)), this, SLOT(tomoListChangedSlot(QString)));
    connect(this->ui->projectionsCombo, SIGNAL(currentTextChanged(QString)), this, SLOT(projectionSelectionSlot(QString)));

    connect(this->ui->potterCombo, SIGNAL(currentTextChanged(QString)), this, SLOT(accessoriesSlot(QString)));
    connect(this->ui->paddlesCombo, SIGNAL(currentTextChanged(QString)), this, SLOT(accessoriesSlot(QString)));
    connect(this->ui->accessoriesCombo, SIGNAL(currentTextChanged(QString)), this, SLOT(accessoriesSlot(QString)));

    connect(this->ui->thickSpin, SIGNAL(valueChanged(int)), this, SLOT(breastSlot(int)));
    connect(this->ui->forceSpin, SIGNAL(valueChanged(int)), this, SLOT(breastSlot(int)));

    connect(ui->bindButton, SIGNAL(pressed()), this, SLOT(bindSlot()), Qt::UniqueConnection);

    connect(ui->generateError, SIGNAL(pressed()), this, SLOT(generateErrorSlot()), Qt::UniqueConnection);
    connect(ui->cancError, SIGNAL(pressed()), this, SLOT(cancelError()), Qt::UniqueConnection);


    timeEv = startTimer(500);
    timerTube = startTimer(10000);

    changeEvent = true;
}

startupWindow::~startupWindow()
{
    delete ui;
}

void startupWindow::initWindow(void)
{

    angolo = 0;
    timeEv = startTimer(1000);
}

void startupWindow::exitWindow(void)
{

    killTimer(timeEv);
    timeEv=0;
}

void startupWindow::bindSlot(void){

    pAws->bind(ui->addressEdit->text(), ui->portEdit->text().toUInt());
}

void startupWindow::setLanguage(QString tag){
    ui->languageLabel->setText("CURRENT LANGUAGE: " + tag);
}


void startupWindow::timerEvent(QTimerEvent* ev)
{
    if(ev->timerId() == timeEv)
    {
       bool ready;
       if(checkReadyForExposure() == 0) ready = true;
       else ready = false;;

       if(ready != readyForExposure){
           readyForExposure = ready;
       }

       if(ready){
           ui->frameReady->setStyleSheet("background-color: rgb(85, 170, 0);");
           ui->labelReady->show();
       }  else{
           ui->frameReady->setStyleSheet("background-color: rgb(67, 67, 67);");
           ui->labelReady->hide();
       }

       if(pAws->getConnectionStatus()) ui->connectionStatusLabel->setText("STATUS: CONNECTED");
       else ui->connectionStatusLabel->setText("STATUS: DISCONNECTED");

    }

    // Every 10 seconds: tube temp simulator
    if(ev->timerId() == timerTube)
    {
       if(tubeJouls > tubeDeltaJouls) tubeJouls-= tubeDeltaJouls;
       else tubeJouls = 0;
       uint tubetemp = 20 + tubeJouls * 40/ 250000;
       uint tubehu = 100 * (tubetemp - 20) / 40;
       tubeDeltaJouls = 80 * 10 * (tubetemp - 20)  / ( 40 * 60) ; // Dissipazione istantanea W/s
       setTubeData(tubehu,tubetemp);
    }

}


void startupWindow::addLogEvent(QString data){
    ui->logEdit->appendPlainText(data);
}

void startupWindow::setStudyName(QString data){
    ui->exposureFrame->hide();
    XraySequenceCode = "";

    if(data == ""){
        ui->studyName->setText("CLOSED");        
    } else ui->studyName->setText(data);
}

int startupWindow::getTrx(void){
    return ui->trxEdit->text().toInt();
}
int startupWindow::getArm(void){
    return ui->armEdit->text().toInt();
}
int startupWindow::getTilt(void){
    return ui->armTiltEdit->text().toInt();
}

QString startupWindow::getPotter(){
    return ui->potterCombo->currentText();
}
QString startupWindow::getPaddle(){
    return ui->paddlesCombo->currentText();
}
QString startupWindow::getThick(){
    return QString("%1").arg(ui->thickSpin->value());
}
QString startupWindow::getForce(){
    return QString("%1").arg(ui->forceSpin->value());
}
QString startupWindow::getAccessory(){
    return ui->accessoriesCombo->currentText();
}

bool startupWindow::rotBusy(void){
    if(trxBusy) return true;
    if(armBusy) return true;
}

void startupWindow::moveTrxCompleted(void){
    ui->trxEdit->setText(QString("%1").arg(targetTRX));
    trxBusy = false;
    emit trxCompletedSgn();
}

bool startupWindow::moveTrx(int angolo, uint speed, QString message){

    int TRX = ui->trxEdit->text().toInt();
    if(TRX == angolo) return false;  // Gia' in posizione
    targetTRX = angolo;
    ui->trxEdit->setText("---");
    int timerTrx = abs(TRX - angolo) * 1000 / speed;
    setMessage(message, timerTrx);
    trxBusy = true;
    QTimer::singleShot(timerTrx, this, &startupWindow::moveTrxCompleted);
    return true;
}

bool startupWindow::moveTrxToHome(QString id){
    int angolo = getTomoHome(id);
    uint speed = abs(getTomoRun(id));
    return moveTrx(angolo, speed, "TRX IS MOVING TO HOME ..");
}

uint startupWindow::moveTrxToFinal(QString id){
    int angolo = getTomoFinal(id);
    int TRX = ui->trxEdit->text().toInt();
    uint speed = abs(getTomoRun(id));
    int timerTrx = abs(TRX - angolo) * 1000 / speed;
    moveTrx(angolo, speed,"TOMO SCAN RUNNING ..");
    return  timerTrx;
}


void startupWindow::moveArmCompleted(void){
    ui->armEdit->setText(QString("%1").arg(targetARM));
    armBusy = false;
    emit armCompletedSgn();
}

bool startupWindow::moveArm(int angolo, uint speed){

    int ARM = ui->armEdit->text().toInt();
    if(ARM == angolo) return false;  // Gia' in posizione
    targetARM = angolo;
    ui->armEdit->setText("---");
    int timerArm = abs(ARM - angolo) * 1000 / speed;
    setMessage("ARM IS MOVING !!!!", timerArm);
    armBusy = true;
    QTimer::singleShot(timerArm, this, &startupWindow::moveArmCompleted);
    return true;
}


void startupWindow::setProjectionList(QList<QString> lista){
    changeEvent = false;
    ui->projectionsCombo->clear();
    ui->projectionsCombo->addItem("---");
    ui->projectionsCombo->addItems(lista);
    changeEvent = true;
}

bool startupWindow::selectProjection(QString proj, int from, int to){

    if(proj == "---"){
        selectedProjection = proj;
        changeEvent = false;
        ui->projectionsCombo->setCurrentText(proj);
        ui->fromArmEdit->setText("---");
        ui->toArmEdit->setText("---");
        changeEvent = true;
        return true;
    }

    if(ui->projectionsCombo->findText(proj, Qt::MatchExactly) < 1) return false;
    selectedProjection = proj;
    changeEvent = false;
    ui->projectionsCombo->setCurrentText(proj);
    ui->fromArmEdit->setText(QString("%1").arg(from));
    ui->toArmEdit->setText(QString("%1").arg(to));
    changeEvent = true;
    return true;
}

void startupWindow::projectionSelectionSlot(QString item){
    if(changeEvent){ // Richiesta di selezione da interfaccia: ripristina
        if(item == "---") pAws->gantryReqAbortProjection();
        else  pAws->gantryReqProjection(item);
        changeEvent = false;
        ui->projectionsCombo->setCurrentText(selectedProjection);
        return;
    }

}


bool startupWindow::selectTomoConfigFile(QString file){
    if(!tomoCfg.contains(file)) return false;
    changeEvent = true;
    tomoListChangedSlot(file);
    return true;
}

void startupWindow::selectTomoConfig(QString file, QString ID){

    ui->tomoCfgHome->setText("---");
    ui->tomoCfgFinal->setText("---");
    ui->tomoCfgAcc->setText("---");
    ui->tomoCfgDec->setText("---");
    ui->tomoCfgRun->setText("---");
    ui->tomoCfgSample->setText("---");
    ui->tomoCfgSkp->setText("---");

    if(!tomoCfg.contains(file)) return;

    QList<tomoConfig> lista;
    lista =  tomoCfg.value(file,lista);
    if(lista.count()==0) return;

    changeEvent = false;
    ui->tomoCfgId->setCurrentText(ID);
    ui->tomoCfgList->setCurrentText(file);
    changeEvent = true;

    for(int i=0; i<lista.count(); i++){
        if(lista.at(i).id == ID){
            ui->tomoCfgHome->setText(lista.at(i).home);
            ui->tomoCfgFinal->setText(lista.at(i).final);
            ui->tomoCfgAcc->setText(lista.at(i).acc);
            ui->tomoCfgDec->setText(lista.at(i).dec);
            ui->tomoCfgRun->setText(lista.at(i).run);
            ui->tomoCfgSample->setText(lista.at(i).sample);
            ui->tomoCfgSkp->setText(lista.at(i).skeep);
            return;
        }
    }
}

void startupWindow::tomoIdChangedSlot(QString id){
    if(!changeEvent) return;
    selectTomoConfig(ui->tomoCfgList->currentText(), id);
}

void startupWindow::tomoListChangedSlot(QString file){
    if(!changeEvent) return;
    selectTomoConfig(file, ui->tomoCfgId->currentText());
}

QString startupWindow::getTomoCfgFile(void){
    return ui->tomoCfgList->currentText();
}
QString startupWindow::getTomoCfgId(void){
    return ui->tomoCfgId->currentText();
}

int startupWindow::getTomoHome(QString ID){

    QList<tomoConfig> lista;
    lista =  tomoCfg.value( ui->tomoCfgList->currentText(),lista);
    if(lista.count()==0) return 10000;

    for(int i=0; i<lista.count(); i++){
        if(lista.at(i).id == ID) return lista.at(i).home.toInt();
    }
    return 10000;
}

int startupWindow::getTomoFinal(QString ID){
    QList<tomoConfig> lista;
    lista =  tomoCfg.value( ui->tomoCfgList->currentText(),lista);
    if(lista.count()==0) return 10000;

    for(int i=0; i<lista.count(); i++){
        if(lista.at(i).id == ID) return lista.at(i).final.toInt();
    }
    return 10000;

}

int startupWindow::getTomoRun(QString ID){
    QList<tomoConfig> lista;
    lista =  tomoCfg.value( ui->tomoCfgList->currentText(),lista);
    if(lista.count()==0) return 10000;

    for(int i=0; i<lista.count(); i++){
        if(lista.at(i).id == ID) return lista.at(i).run.toInt();
    }
    return 10000;

}

int startupWindow::getTomoSamples(QString ID){
    QList<tomoConfig> lista;
    lista =  tomoCfg.value( ui->tomoCfgList->currentText(),lista);
    if(lista.count()==0) return 10000;

    for(int i=0; i<lista.count(); i++){
        if(lista.at(i).id == ID) return lista.at(i).sample.toInt();
    }
    return 10000;

}

void startupWindow::messageOff(void){
     ui->messages->hide();
}
void startupWindow::setMessage(QString msg, uint time){

    ui->messages->setText(msg);
    ui->messages->show();
    QTimer::singleShot(time, this, &startupWindow::messageOff);
}

void startupWindow::setTubeData(int anode, int housing){

    ui->tubeTemp->setText(QString("%1 °").arg(housing));
    ui->tubeHu->setText(QString("%1 \%").arg(anode));

    if( (tubeHu!=anode) || (tubeTemp!=housing ))
    {
        tubeHu = anode;
        tubeTemp = housing;
        pAws->gantrySetTubeTemperature(QString("%1").arg(tubeHu), QString("%1").arg(tubeTemp));
    }
}

void startupWindow::setCompressionReleaseMode(bool stat){
    if(!stat) ui->compMode->setCurrentText("KEEP");
    else ui->compMode->setCurrentText("RELEASE");
}

QString startupWindow::getCompressionReleaseMode(void){
    return  ui->compMode->currentText();
}

bool startupWindow::getXrayPush(void){
    return xrayPushStat;
}

void startupWindow::clearXrayButton(void){
    ui->xrayPushButton->setText("X-RAY PUSH OFF");
    xrayPushStat = false;
    emit xrayPushButtonSgn(false);
}

void startupWindow::emitXrayPushButtonPressed(void){
    if(!xrayPushStat){
        ui->xrayPushButton->setText("X-RAY PUSH ON");
        xrayPushStat = true;
        pAws->gantryXrayPushEvent();
        emit xrayPushButtonSgn(true);
    }else{

        ui->xrayPushButton->setText("X-RAY PUSH OFF");
        xrayPushStat = false;
        emit xrayPushButtonSgn(false);
    }
}
void startupWindow::emitXrayPushButtonReleased(void){
}

void startupWindow::setXrayEnable(bool stat){
    if(!stat){
        ui->xrayPushButton->disconnect();
        ui->xrayPushButton->setStyleSheet("background-color: rgba(0, 0, 127,100);color: rgb(255, 255, 255);");
    }else{
        connect(ui->xrayPushButton, SIGNAL(pressed()), this, SLOT(emitXrayPushButtonPressed()), Qt::UniqueConnection);
        connect(ui->xrayPushButton, SIGNAL(released()), this, SLOT(emitXrayPushButtonReleased()), Qt::UniqueConnection);
        ui->xrayPushButton->setStyleSheet("background-color: rgba(0, 0, 127,255);color: rgb(255, 255, 255);");
    }
}


void startupWindow::accessoriesSlot(QString data){
    if(!changeEvent) return;
    QString pot = ui->potterCombo->currentText();
    QString pad= ui->paddlesCombo->currentText();
    QString acc = ui->accessoriesCombo->currentText();

    if( (ui->paddlesCombo->currentText() == "PAD_UNDETECTED") || (ui->paddlesCombo->currentText() == "PAD_UNLOCKED") || (ui->potterCombo->currentText() == "POTTER_UNDETECTED")){
        ui->thickSpin->setValue(0);
        ui->forceSpin->setValue(0);
        ui->thickSpin->setEnabled(false);
        ui->forceSpin->setEnabled(false);
    }else{
        ui->thickSpin->setEnabled(true);
        ui->forceSpin->setEnabled(true);
    }

    pAws->gantrySetAccessory(pot, pad, acc);
}

QString startupWindow::getTubeTemp(){
    return QString("%1").arg(tubeTemp);
}
QString startupWindow::getTubeHu(){
    return QString("%1").arg(tubeHu);
}

void startupWindow::breastSlot(int val){
    if(!changeEvent) return;

    pAws->gantryCompressorData(QString("%1").arg(ui->thickSpin->value()), QString("%1").arg(ui->forceSpin->value()));

}
void startupWindow::generateErrorSlot(void){
    generateError(201);
}

void startupWindow::generateError(uint num){

    QString str =  QString("%1").arg(num);
    errorCondition = QString("%1").arg(str,(int) 5,'0');
    ui->errorLabel->setText("[M"+ errorCondition +"]");
    ui->errorFrame->show();
    pAws->gantryError(errorCondition);
}

void startupWindow::cancelError(void){
    errorCondition = "";
    ui->errorLabel->setText("");
    ui->errorFrame->hide();
}

int startupWindow::checkReadyForExposure(void){

    // Error condition
    if(errorCondition != "") return 1;

    // Valid Exposure Mode
    if(!XrayCodeList.contains(XraySequenceCode)) return 2;

    // Valid Projection selected
    if(ui->toArmEdit->text() == "---") return 3;
    if(ui->fromArmEdit->text() == "---") return 3;
    if(ui->armEdit->text() == "---") return 3;
    int from = ui->fromArmEdit->text().toInt();
    int to = ui->toArmEdit->text().toInt();
    int angolo = ui->armEdit->text().toInt();

    // Arm Acceptable range
    if(angolo > to) return 4;
    if(angolo < from) return 4;

    // Potter setting
    if(ui->potterCombo->currentText() == "POTTER_UNDETECTED") return 5;

    if(XraySequenceCode == "MAN_2D"){

    }else if(XraySequenceCode == "AEC_2D"){

    }else if(XraySequenceCode == "MAN_3D"){
        if(ui->potterCombo->currentText() != "POTTER_3D") return 5;

    }else if(XraySequenceCode == "AEC_3D"){
        if(ui->potterCombo->currentText() != "POTTER_3D") return 5;
    }else if(XraySequenceCode == "MAN_COMBO"){
        if(ui->potterCombo->currentText() != "POTTER_3D") return 5;
    }else if(XraySequenceCode == "AEC_COMBO"){
        if(ui->potterCombo->currentText() != "POTTER_3D") return 5;
    }else if(XraySequenceCode == "MAN_AE"){

    }else if(XraySequenceCode == "AEC_AE"){

    }

    // Collimator Accessories
    if(ui->potterCombo->currentText().contains("BIOPSY")){
        if(ui->accessoriesCombo->currentText().contains("PROTECTION")) return 6;
    }else if(!ui->accessoriesCombo->currentText().contains("PROTECTION")) return 6;

    // Paddle detection
    if(ui->paddlesCombo->currentText() == "PAD_UNDETECTED") return 7;
    if(ui->paddlesCombo->currentText() == "PAD_UNLOCKED") return 7;


    // Compression option
    if(Compression == "COMP_ENABLED") {
        if(ui->thickSpin->value()==0) return 8;
        if(ui->forceSpin->value()==0) return 8;
    }


    return 0;

}


int  startupWindow::setXrayExposureMode(QString CODE, QString COMP, QString COLL, QString TOMO){
    if(!XrayCodeList.contains(CODE)) return 2;
    XraySequenceCode = CODE;

    if((COMP!="COMP_ENABLED") && (COMP!="COMP_DISABLED")) return 3;
    Compression = COMP;

    if(!ColliList.contains(COLL)) return 4;
    Collimation = COLL;

    if(TOMO == "TomoNone") TomoId = "";
    else{
         if(ui->tomoCfgId->findText(TOMO) < 0) return 5;
         TomoId = TOMO;
    }


    ui->xraySym->hide();
    ui->pulse0Frame->hide();
    ui->pulse1Frame->hide();
    ui->pulse2Frame->hide();
    ui->kV0->setText("kV:---");
    ui->kV1->setText("kV:---");
    ui->kV2->setText("kV:---");
    ui->mAs0->setText("mAs:---");
    ui->mAs1->setText("mAs:---");
    ui->mAs2->setText("mAs:---");
    ui->filter0->setText("Filter:---");
    ui->filter1->setText("Filter:---");
    ui->filter2->setText("Filter:---");

    ui->prepFrame->hide();
    ui->exp1->hide();
    ui->exp2->hide();
    ui->exp3->hide();
    ui->exp4->hide();
    ui->exp5->hide();
    ui->exposureFrame->show();
    ui->colliLabel->setText("COLLIMATION: " + Collimation);
    ui->comprLabel->setText("COMPRESSION: " + Compression);
    ui->tomoIdLabel->setText("TOMO-ID: " + TomoId);

    if(XraySequenceCode == "MAN_2D"){
        ui->title0->setText("MAIN PULSE");
        ui->pulse0Frame->show();
        ui->exposureTypeLabel->setText("MANUAL 2D EXPOSURE ");
    }else if(XraySequenceCode == "AEC_2D"){
        ui->title0->setText("PRE PULSE");
        ui->title1->setText("MAIN PULSE");
        ui->pulse0Frame->show();
        ui->pulse1Frame->show();
        ui->exposureTypeLabel->setText("AEC 2D EXPOSURE ");
    }else if(XraySequenceCode == "MAN_3D"){
        ui->title0->setText("MAIN 3D PULSE");
        ui->pulse0Frame->show();
        ui->exposureTypeLabel->setText("MANUAL 3D EXPOSURE ");
    }else if(XraySequenceCode == "AEC_3D"){
        ui->title0->setText("PRE PULSE");
        ui->title1->setText("MAIN 3D PULSE");
        ui->pulse0Frame->show();
        ui->pulse1Frame->show();
        ui->exposureTypeLabel->setText("AEC 3D EXPOSURE ");
    }else if(XraySequenceCode == "MAN_COMBO"){
        ui->title0->setText("MAIN 2D PULSE");
        ui->title1->setText("MAIN 3D PULSE");
        ui->pulse0Frame->show();
        ui->pulse1Frame->show();
        ui->exposureTypeLabel->setText("MANUAL COMBO EXPOSURE ");
    }else if(XraySequenceCode == "AEC_COMBO"){
        ui->title0->setText("PRE PULSE");
        ui->title1->setText("MAIN 2D PULSE");
        ui->title2->setText("MAIN 3D PULSE");
        ui->pulse0Frame->show();
        ui->pulse1Frame->show();
        ui->pulse2Frame->show();
        ui->exposureTypeLabel->setText("AEC COMBO EXPOSURE ");
    }else if(XraySequenceCode == "MAN_AE"){
        ui->title0->setText("LOW ENERGY PULSE");
        ui->title1->setText("HIGH ENERGY PULSE");
        ui->pulse0Frame->show();
        ui->pulse1Frame->show();
        ui->exposureTypeLabel->setText("MANUAL AE EXPOSURE ");
    }else if(XraySequenceCode == "AEC_AE"){
        ui->title0->setText("LOW PRE PULSE");
        ui->title1->setText("LOW ENERGY PULSE");
        ui->title2->setText("HIGH ENERGY PULSE");
        ui->pulse0Frame->show();
        ui->pulse1Frame->show();
        ui->pulse2Frame->show();
        ui->exposureTypeLabel->setText("AEC AE EXPOSURE ");
    }
    return 0;
}

int  startupWindow::setXraySequenceData(QString KV, QString MAS, QString FILTER){

    kV = KV.toFloat();
    if((kV<20) || (kV>49)) return 4;

    mAs = MAS.toUInt();
    if((mAs <1) || (mAs>640)) return 5;

    if(!FilterList.contains(FILTER)) return 6;
    Filter = FILTER;

    if(sequenceStarted) return 7;

    return 0;

}

int  startupWindow::setXrayPulseData(QString KV, QString MAS, QString FILTER, QString TOMO){

    kV = KV.toFloat();
    if((kV<20) || (kV>49)) return 2;

    mAs = MAS.toUInt();
    if((mAs <1) || (mAs>640)) return 3;

    if(!FilterList.contains(FILTER)) return 4;
    Filter = FILTER;

    if(TOMO == "TomoNone") TomoId = "";
    else{
         if(ui->tomoCfgId->findText(TOMO) < 0) return 5;
         TomoId = TOMO;
    }

    tubeJouls += kV * mAs;
    pulseDataReady = true;
    return 0;

}

int startupWindow::getExposureCompletionData(QString option, uint pulse, QList<QString>* params){
    params->clear();


    if(option == "SEQUENCE"){
        params->append(exposureResult);
        params->append(QString("%1").arg(exposureError));
        params->append(QString("%1").arg(numPulse));
        return 0;

    }else if(option == "PULSE"){
        if((pulse > numPulse) ||(pulse == 0)) return 3;
        params->append(QString("%1").arg(kVFinal[pulse - 1]));
        params->append(QString("%1").arg(mAsFinal[pulse - 1]));
        params->append(QString("%1").arg(FilterFinal[pulse - 1]));
        return 0;
    }else if(option == "TOMO_ANGLE"){
        if(tomoNsamplesFinal == 0){
            params->append("0 0 ");
            return 0;
        }

        params->append(QString("%1").arg(tomoNsamplesFinal));
        for(uint i=0; i< tomoNsamplesFinal; i++)  params->append(QString("%1").arg(tomoTimeSample));
        return 0;
    }else if(option == "IA_SAMPLE"){
        if((pulse > numPulse) ||(pulse == 0)) return 3;
        params->append("2");
        params->append("145");
        params->append("145");
        return 0;
    }else if(option == "KV_SAMPLE"){
        if((pulse > numPulse) ||(pulse == 0)) return 3;
        params->append("2");
        params->append(QString("%1").arg(kVFinal[pulse - 1] + 0.1));
        params->append(QString("%1").arg(kVFinal[pulse - 1] - 0.1));
        return 0;
    }

    return 0;
}



void startupWindow::startXray(void){

    sequenceStarted = true;
    ui->xraySym->show();
    ui->exposureFrame->show();

    ui->prepFrame->hide();
    ui->exp1->hide();
    ui->exp2->hide();
    ui->exp3->hide();
    ui->exp4->hide();
    ui->exp5->hide();


    ui->colliLabel->setText("COLLIMATION: " + Collimation);
    ui->comprLabel->setText("COMPRESSION: " + Compression);
    ui->tomoIdLabel->setText("TOMO-ID: " + TomoId);

    tomoNsamplesFinal = 0;
    numPulse = 0;
    percPulse = 0;
    expSeq = 0;
    if(XraySequenceCode == "MAN_2D"){
        QTimer::singleShot(100, this, &startupWindow::manual2DSequence);
    }else if(XraySequenceCode == "AEC_2D"){
        QTimer::singleShot(1, this, &startupWindow::Aec2DSequence);
    }else if(XraySequenceCode == "MAN_3D"){
        if(TomoId == ""){
            seqError(901);
            return;
        }
        QTimer::singleShot(1, this, &startupWindow::manual3DSequence);
    }else if(XraySequenceCode == "AEC_3D")  QTimer::singleShot(1, this, &startupWindow::Aec3DSequence);
    else if(XraySequenceCode == "MAN_COMBO")  QTimer::singleShot(1, this, &startupWindow::manualComboSequence);
    else if(XraySequenceCode == "AEC_COMBO")  QTimer::singleShot(1, this, &startupWindow::AecComboSequence);
    else if(XraySequenceCode == "MAN_AE")  QTimer::singleShot(1, this, &startupWindow::manualAESequence);
    else if(XraySequenceCode == "AEC_AE")  QTimer::singleShot(1, this, &startupWindow::AecAESequence);

    tubeJouls += kV * mAs;
}

void startupWindow::seqError(uint num){
    exposureError = num;
    generateError(num);
    ui->xraySym->hide();
    sequenceStarted = false;

    if(numPulse){
        mAsFinal[numPulse-1] = (uint) ((float) mAs * (float) percPulse / 5);
        FilterFinal[numPulse-1] = Filter;
        kVFinal[numPulse-1] = kV;
        exposureResult = "PARTIAL";

        if(numPulse == 3){
            ui->kV2->setText(QString("kV:%1").arg(kVFinal[numPulse-1]));
            ui->mAs2->setText(QString("mAs:%1").arg(mAsFinal[numPulse-1]));
            ui->filter2->setText(QString("Filter:%1").arg(FilterFinal[numPulse-1]));
        }else  if(numPulse == 2) {
            ui->kV1->setText(QString("kV:%1").arg(kVFinal[numPulse-1]));
            ui->mAs1->setText(QString("mAs:%1").arg(mAsFinal[numPulse-1]));
            ui->filter1->setText(QString("Filter:%1").arg(FilterFinal[numPulse-1]));
        }else{
            ui->kV0->setText(QString("kV:%1").arg(kVFinal[numPulse-1]));
            ui->mAs0->setText(QString("mAs:%1").arg(mAsFinal[numPulse-1]));
            ui->filter0->setText(QString("Filter:%1").arg(FilterFinal[numPulse-1]));
        }
    }else  exposureResult = "NO_DOSE";

    pAws->gantryXraySequenceCompleted(exposureResult);

    if(ui->compMode->currentText() != "KEEP"){
        ui->thickSpin->setValue(0);
        ui->forceSpin->setValue(0);
    }

    setXrayEnable(false);
    clearXrayButton();
}

void startupWindow::sequenceCompleted(void){
     exposureError = 0;
     ui->xraySym->hide();
     sequenceStarted = false;
     setXrayEnable(false);
     clearXrayButton();

     if(ui->compMode->currentText() != "KEEP"){
         ui->thickSpin->setValue(0);
         ui->forceSpin->setValue(0);
     }

     if(numPulse == 3){
         ui->kV2->setText(QString("kV:%1").arg(kVFinal[numPulse-1]));
         ui->mAs2->setText(QString("mAs:%1").arg(mAsFinal[numPulse-1]));
         ui->filter2->setText(QString("Filter:%1").arg(FilterFinal[numPulse-1]));
     }else  if(numPulse == 2) {
         ui->kV1->setText(QString("kV:%1").arg(kVFinal[numPulse-1]));
         ui->mAs1->setText(QString("mAs:%1").arg(mAsFinal[numPulse-1]));
         ui->filter1->setText(QString("Filter:%1").arg(FilterFinal[numPulse-1]));
     }else{
         ui->kV0->setText(QString("kV:%1").arg(kVFinal[numPulse-1]));
         ui->mAs0->setText(QString("mAs:%1").arg(mAsFinal[numPulse-1]));
         ui->filter0->setText(QString("Filter:%1").arg(FilterFinal[numPulse-1]));
     }

     pAws->gantryXraySequenceCompleted("COMPLETED");

}

//_____________________________________________________________________________________________________________________________________________
//___________________ MANUAL 2D _______________________________________________________________________________________________________________
//_____________________________________________________________________________________________________________________________________________
void startupWindow::manual2DSequence(void){ // 100 ms slot
    if(!getXrayPush()) {
        seqError(902);
        return;
    }

    if(expSeq == 0) ui->prepFrame->show();
    else if(expSeq == 20){ numPulse = 1; ui->exp1->show(); percPulse = 1;}
    else if(expSeq == 25){ ui->exp2->show(); percPulse++;}
    else if(expSeq == 30){ ui->exp3->show(); percPulse++;}
    else if(expSeq == 35){ ui->exp4->show(); percPulse++;}
    else if(expSeq == 40){ ui->exp5->show(); percPulse++;}
    else if(expSeq == 45){
        exposureResult = "COMPLETED";
        mAsFinal[numPulse-1] = mAs ;
        FilterFinal[numPulse-1] = Filter;
        kVFinal[numPulse-1] = kV;
        sequenceCompleted();
        return;
    }
    expSeq++;
    QTimer::singleShot(100, this, &startupWindow::manual2DSequence);
    return;

}

//_____________________________________________________________________________________________________________________________________________
//___________________ AEC 2D _______________________________________________________________________________________________________________
//_____________________________________________________________________________________________________________________________________________
void startupWindow::Aec2DSequence(void){
    if(!getXrayPush()) {
        seqError(902);
        return;
    }

    if(expSeq == 0){        
        ui->prepFrame->show();
    }else if(expSeq == 20){ numPulse = 1; ui->exp1->show(); percPulse = 1;}
    else if(expSeq == 22){ ui->exp2->show(); percPulse++;}
    else if(expSeq == 24){ ui->exp3->show(); percPulse++;}
    else if(expSeq == 26){ ui->exp4->show(); percPulse++;}
    else if(expSeq == 28){ ui->exp5->show(); percPulse++;}
    else if(expSeq == 30){
        mAsFinal[numPulse-1] = mAs ;
        FilterFinal[numPulse-1] = Filter;
        kVFinal[numPulse-1] = kV;
        pulseDataReady = false;
        ui->kV0->setText(QString("kV:%1").arg(kV));
        ui->mAs0->setText(QString("mAs:%1").arg(mAs));
        ui->filter0->setText(QString("Filter:%1").arg(Filter));
        pAws->gantryPulseCompleted();
    }
    else if(expSeq == 31){
        // Wait for the AWS data
        if(!pulseDataReady){
            QTimer::singleShot(100, this, &startupWindow::Aec2DSequence);
            return;
        }else{
            ui->exp1->hide();
            ui->exp2->hide();
            ui->exp3->hide();
            ui->exp4->hide();
            ui->exp5->hide();
            percPulse = 0;
            expSeq++;
            QTimer::singleShot(100, this, &startupWindow::Aec2DSequence);
            return;
        }
    }
    else if(expSeq == 32){ numPulse = 2; ui->exp1->show(); percPulse=1;}
    else if(expSeq == 37){ ui->exp2->show(); percPulse++;}
    else if(expSeq == 42){ ui->exp3->show(); percPulse++;}
    else if(expSeq == 47){ ui->exp4->show(); percPulse++;}
    else if(expSeq == 52){ ui->exp5->show(); percPulse++;}
    else if(expSeq == 57){
        exposureResult = "COMPLETED";
        mAsFinal[numPulse-1] = mAs ;
        FilterFinal[numPulse-1] = Filter;
        kVFinal[numPulse-1] = kV;
        sequenceCompleted();
        return;
    }
    expSeq++;
    QTimer::singleShot(100, this, &startupWindow::Aec2DSequence);
    return;
}

//_____________________________________________________________________________________________________________________________________________
//___________________ MANUAL 3D _______________________________________________________________________________________________________________
//_____________________________________________________________________________________________________________________________________________
void startupWindow::manual3DSequence(void){
    static uint steps;

    if(!getXrayPush()) {
        seqError(902);
        return;
    }

    // Attesa posizionamento TRX in corso (o ARM)
    if(expSeq == 0){
        if(TomoId==""){seqError(901); return;}
        ui->prepFrame->show();
        if(!rotBusy()) expSeq++;
        QTimer::singleShot(100, this, &startupWindow::manual3DSequence);
        return;
    }

    // Posizionamento TRX in Home se previsto
    if(expSeq == 1){        
        moveTrxToHome(TomoId);
        expSeq++;
        QTimer::singleShot(100, this, &startupWindow::manual3DSequence);
        return;
    }

    // Attesa TRX in HOME
    if(expSeq == 2){
        if(!rotBusy()) expSeq++;
        QTimer::singleShot(100, this, &startupWindow::manual3DSequence);
        return;
    }

    // Partenza braccio Tomo
    if(expSeq == 3){
        tomoNsamplesFinal = getTomoSamples(TomoId);
        tomoTimeSample = mAs * 1000 /(190 * tomoNsamplesFinal);
        steps =  moveTrxToFinal(TomoId) / 500;
        ui->exp1->show(); percPulse++;
        QTimer::singleShot(100, this, &startupWindow::manual3DSequence);
        expSeq++;
        return;
    }

    if(expSeq == 3+steps) { numPulse = 1; ui->exp2->show(); percPulse=1;}
    else if(expSeq == 3+steps*2) { ui->exp3->show(); percPulse++;}
    else if(expSeq == 3+steps*3) { ui->exp4->show(); percPulse++;}
    else if(expSeq == 3+steps*4) { ui->exp5->show(); percPulse++;}
    else if(expSeq == 3+steps*5)
    {
        exposureResult = "COMPLETED";
        mAsFinal[numPulse-1] = mAs ;
        FilterFinal[numPulse-1] = Filter;
        kVFinal[numPulse-1] = kV;
        sequenceCompleted();
        return;
    }
    expSeq++;
    QTimer::singleShot(100, this, &startupWindow::manual3DSequence);
    return;

}

//_____________________________________________________________________________________________________________________________________________
//___________________ AEC 3D _______________________________________________________________________________________________________________
//_____________________________________________________________________________________________________________________________________________
void startupWindow::Aec3DSequence(void){
    static uint steps;
    if(!getXrayPush()) {
        seqError(902);
        return;
    }

    // Attesa posizionamento TRX in corso (o ARM)
    if(expSeq == 0){
        if(TomoId==""){seqError(901); return;}
        ui->prepFrame->show();
        if(!rotBusy()) expSeq++;
        QTimer::singleShot(100, this, &startupWindow::Aec3DSequence);
        return;
    }

    // Posizionamento TRX in Home
    if(expSeq == 1){
        moveTrxToHome(TomoId);
        expSeq++;
        QTimer::singleShot(100, this, &startupWindow::Aec3DSequence);
        return;
    }

    // Attesa TRX in HOME
    if(expSeq == 2){
        if(!rotBusy()) expSeq++;
        QTimer::singleShot(100, this, &startupWindow::Aec3DSequence);
        return;
    }

    // Pre impulso
    else if(expSeq == 3){numPulse = 1; ui->exp1->show(); percPulse=1;}
    else if(expSeq == 5){ ui->exp2->show(); percPulse++;}
    else if(expSeq == 7){ ui->exp3->show(); percPulse++;}
    else if(expSeq == 9){ ui->exp4->show(); percPulse++;}
    else if(expSeq == 11){ ui->exp5->show(); percPulse++;}
    else if(expSeq == 13){
        mAsFinal[numPulse-1] = mAs ;
        FilterFinal[numPulse-1] = Filter;
        kVFinal[numPulse-1] = kV;
        pulseDataReady = false;
        ui->kV0->setText(QString("kV:%1").arg(kV));
        ui->mAs0->setText(QString("mAs:%1").arg(mAs));
        ui->filter0->setText(QString("Filter:%1").arg(Filter));
        pAws->gantryPulseCompleted();
        expSeq++;
        QTimer::singleShot(100, this, &startupWindow::Aec3DSequence);
        return;
    }

    // Wait for the AWS data
    if(expSeq == 14){
        if(!pulseDataReady){
            QTimer::singleShot(100, this, &startupWindow::Aec3DSequence);
            return;
        }else{
            ui->exp1->hide();
            ui->exp2->hide();
            ui->exp3->hide();
            ui->exp4->hide();
            ui->exp5->hide();
            expSeq++;
            QTimer::singleShot(100, this, &startupWindow::Aec3DSequence);
            return;
        }
    }

    // Posizionamento TRX in Home (se diverso da TomoId iniziale
    if(expSeq == 15){
        moveTrxToHome(TomoId);
        expSeq++;
        QTimer::singleShot(100, this, &startupWindow::Aec3DSequence);
        return;
    }

    // Attesa TRX in HOME
    if(expSeq == 16){
        if(!rotBusy()) expSeq++;
        QTimer::singleShot(100, this, &startupWindow::Aec3DSequence);
        return;
    }

    // Partenza braccio Tomo
    if(expSeq == 17){
        tomoNsamplesFinal = getTomoSamples(TomoId);
        tomoTimeSample = mAs * 1000 /(190 * tomoNsamplesFinal);
        steps =  moveTrxToFinal(TomoId) / 500;
        percPulse = 0;
        numPulse=2;
        ui->exp1->show(); percPulse++;
        QTimer::singleShot(100, this, &startupWindow::Aec3DSequence);
        expSeq++;
        return;
    }

    if(expSeq == 18+steps) { ui->exp2->show(); percPulse++;}
    else if(expSeq == 18+steps*2) { ui->exp3->show(); percPulse++;}
    else if(expSeq == 18+steps*3) { ui->exp4->show(); percPulse++;}
    else if(expSeq == 18+steps*4) { ui->exp5->show(); percPulse++;}
    else if(expSeq == 18+steps*5)
    {
        exposureResult = "COMPLETED";
        mAsFinal[numPulse-1] = mAs ;
        FilterFinal[numPulse-1] = Filter;
        kVFinal[numPulse-1] = kV;
        sequenceCompleted();
        return;
    }
    expSeq++;
    QTimer::singleShot(100, this, &startupWindow::Aec3DSequence);
    return;

}


//_____________________________________________________________________________________________________________________________________________
//___________________ MANUAL COMBO _______________________________________________________________________________________________________________
//_____________________________________________________________________________________________________________________________________________
void startupWindow::manualComboSequence(void){
    static uint steps;

    if(!getXrayPush()) {
        seqError(902);
        return;
    }

    // Impulso 2D
    if(expSeq == 0) ui->prepFrame->show();
    else if(expSeq == 20){ numPulse = 1; ui->exp1->show(); percPulse = 1;}
    else if(expSeq == 25){ ui->exp2->show(); percPulse++;}
    else if(expSeq == 30){ ui->exp3->show(); percPulse++;}
    else if(expSeq == 35){ ui->exp4->show(); percPulse++;}
    else if(expSeq == 40){ ui->exp5->show(); percPulse++;}
    else if(expSeq == 45){
        mAsFinal[numPulse-1] = mAs ;
        FilterFinal[numPulse-1] = Filter;
        kVFinal[numPulse-1] = kV;
        pulseDataReady = false;
        ui->kV0->setText(QString("kV:%1").arg(kV));
        ui->mAs0->setText(QString("mAs:%1").arg(mAs));
        ui->filter0->setText(QString("Filter:%1").arg(Filter));
        pAws->gantryPulseCompleted();
        expSeq++;
        QTimer::singleShot(100, this, &startupWindow::manualComboSequence);
        return;
    }

    // Wait for the AWS data
    if(expSeq == 46){
        if(!pulseDataReady){
            QTimer::singleShot(100, this, &startupWindow::manualComboSequence);
            return;
        }else{
            if(TomoId==""){seqError(901); return;}

            ui->exp1->hide();
            ui->exp2->hide();
            ui->exp3->hide();
            ui->exp4->hide();
            ui->exp5->hide();
            expSeq++;
            QTimer::singleShot(100, this, &startupWindow::manualComboSequence);
            return;
        }
    }

    // Posizionamento TRX in Home (se diverso da TomoId iniziale
    if(expSeq == 47){
        moveTrxToHome(TomoId);
        expSeq++;
        QTimer::singleShot(100, this, &startupWindow::manualComboSequence);
        return;
    }

    // Attesa TRX in HOME
    if(expSeq == 48){
        if(!rotBusy()) expSeq++;
        QTimer::singleShot(100, this, &startupWindow::manualComboSequence);
        return;
    }

    // Partenza braccio Tomo
    if(expSeq == 49){
        tomoNsamplesFinal = getTomoSamples(TomoId);
        tomoTimeSample = mAs * 1000 /(190 * tomoNsamplesFinal);
        steps =  moveTrxToFinal(TomoId) / 500;
        percPulse = 0;
        numPulse=2;
        ui->exp1->show(); percPulse++;
        QTimer::singleShot(100, this, &startupWindow::manualComboSequence);
        expSeq++;
        return;
    }

    if(expSeq == 50+steps) { ui->exp2->show(); percPulse++;}
    else if(expSeq == 50+steps*2) { ui->exp3->show(); percPulse++;}
    else if(expSeq == 50+steps*3) { ui->exp4->show(); percPulse++;}
    else if(expSeq == 50+steps*4) { ui->exp5->show(); percPulse++;}
    else if(expSeq == 50+steps*5)
    {
        exposureResult = "COMPLETED";
        mAsFinal[numPulse-1] = mAs ;
        FilterFinal[numPulse-1] = Filter;
        kVFinal[numPulse-1] = kV;
        sequenceCompleted();
        return;
    }

    expSeq++;
    QTimer::singleShot(100, this, &startupWindow::manualComboSequence);
}

//_____________________________________________________________________________________________________________________________________________
//___________________ AEC COMBO _______________________________________________________________________________________________________________
//_____________________________________________________________________________________________________________________________________________
void startupWindow::AecComboSequence(void){
    static uint steps;

    if(!getXrayPush()) {
        seqError(902);
        return;
    }

    if(expSeq == 0){
        ui->prepFrame->show();
    }else if(expSeq == 20){ numPulse = 1; ui->exp1->show(); percPulse = 1;}
    else if(expSeq == 22){ ui->exp2->show(); percPulse++;}
    else if(expSeq == 24){ ui->exp3->show(); percPulse++;}
    else if(expSeq == 26){ ui->exp4->show(); percPulse++;}
    else if(expSeq == 28){ ui->exp5->show(); percPulse++;}
    else if(expSeq == 30){
        mAsFinal[numPulse-1] = mAs ;
        FilterFinal[numPulse-1] = Filter;
        kVFinal[numPulse-1] = kV;
        pulseDataReady = false;
        ui->kV0->setText(QString("kV:%1").arg(kV));
        ui->mAs0->setText(QString("mAs:%1").arg(mAs));
        ui->filter0->setText(QString("Filter:%1").arg(Filter));
        pAws->gantryPulseCompleted();
    }
    else if(expSeq == 31){
        // Wait for the AWS data
        if(!pulseDataReady){
            QTimer::singleShot(100, this, &startupWindow::AecComboSequence);
            return;
        }else{
            ui->exp1->hide();
            ui->exp2->hide();
            ui->exp3->hide();
            ui->exp4->hide();
            ui->exp5->hide();
            percPulse = 0;
            expSeq++;
            QTimer::singleShot(100, this, &startupWindow::AecComboSequence);
            return;
        }
    }
    else if(expSeq == 32){ numPulse = 2; ui->exp1->show(); percPulse=1;}
    else if(expSeq == 37){ ui->exp2->show(); percPulse++;}
    else if(expSeq == 42){ ui->exp3->show(); percPulse++;}
    else if(expSeq == 47){ ui->exp4->show(); percPulse++;}
    else if(expSeq == 52){ ui->exp5->show(); percPulse++;}
    else if(expSeq == 57){
        mAsFinal[numPulse-1] = mAs ;
        FilterFinal[numPulse-1] = Filter;
        kVFinal[numPulse-1] = kV;
        pulseDataReady = false;
        ui->kV1->setText(QString("kV:%1").arg(kV));
        ui->mAs1->setText(QString("mAs:%1").arg(mAs));
        ui->filter1->setText(QString("Filter:%1").arg(Filter));
        pAws->gantryPulseCompleted();
    }

    // Wait for the AWS data
    if(expSeq == 58){
        if(!pulseDataReady){
            QTimer::singleShot(100, this, &startupWindow::AecComboSequence);
            return;
        }else{
            if(TomoId==""){seqError(901); return;}

            ui->exp1->hide();
            ui->exp2->hide();
            ui->exp3->hide();
            ui->exp4->hide();
            ui->exp5->hide();
            expSeq++;
            QTimer::singleShot(100, this, &startupWindow::AecComboSequence);
            return;
        }
    }

    // Posizionamento TRX in Home (se diverso da TomoId iniziale
    if(expSeq == 59){
        moveTrxToHome(TomoId);
        expSeq++;
        QTimer::singleShot(100, this, &startupWindow::AecComboSequence);
        return;
    }

    // Attesa TRX in HOME
    if(expSeq == 60){
        if(!rotBusy()) expSeq++;
        QTimer::singleShot(100, this, &startupWindow::AecComboSequence);
        return;
    }

    // Partenza braccio Tomo
    if(expSeq == 61){
        tomoNsamplesFinal = getTomoSamples(TomoId);
        tomoTimeSample = mAs * 1000 /(190 * tomoNsamplesFinal);
        steps =  moveTrxToFinal(TomoId) / 500;
        percPulse = 0;
        numPulse=3;
        ui->exp1->show(); percPulse++;
        QTimer::singleShot(100, this, &startupWindow::AecComboSequence);
        expSeq++;
        return;
    }

    if(expSeq == 62+steps) { ui->exp2->show(); percPulse++;}
    else if(expSeq == 62+steps*2) { ui->exp3->show(); percPulse++;}
    else if(expSeq == 62+steps*3) { ui->exp4->show(); percPulse++;}
    else if(expSeq == 62+steps*4) { ui->exp5->show(); percPulse++;}
    else if(expSeq == 62+steps*5)
    {
        exposureResult = "COMPLETED";
        mAsFinal[numPulse-1] = mAs ;
        FilterFinal[numPulse-1] = Filter;
        kVFinal[numPulse-1] = kV;
        sequenceCompleted();
        return;
    }
    expSeq++;
    QTimer::singleShot(100, this, &startupWindow::AecComboSequence);
    return;
}

//_____________________________________________________________________________________________________________________________________________
//___________________ MANUAL AE _______________________________________________________________________________________________________________
//_____________________________________________________________________________________________________________________________________________
void startupWindow::manualAESequence(void){
    if(!getXrayPush()) {
        seqError(902);
        return;
    }

    if(expSeq == 0){
        ui->prepFrame->show();
    }else if(expSeq == 20){ numPulse = 1; ui->exp1->show(); percPulse = 1;}
    else if(expSeq == 22){ ui->exp2->show(); percPulse++;}
    else if(expSeq == 24){ ui->exp3->show(); percPulse++;}
    else if(expSeq == 26){ ui->exp4->show(); percPulse++;}
    else if(expSeq == 28){ ui->exp5->show(); percPulse++;}
    else if(expSeq == 30){
        mAsFinal[numPulse-1] = mAs ;
        FilterFinal[numPulse-1] = Filter;
        kVFinal[numPulse-1] = kV;
        pulseDataReady = false;
        ui->kV0->setText(QString("kV:%1").arg(kV));
        ui->mAs0->setText(QString("mAs:%1").arg(mAs));
        ui->filter0->setText(QString("Filter:%1").arg(Filter));
        pAws->gantryPulseCompleted();
    }
    else if(expSeq == 31){
        // Wait for the AWS data
        if(!pulseDataReady){
            QTimer::singleShot(100, this, &startupWindow::manualAESequence);
            return;
        }else{
            ui->exp1->hide();
            ui->exp2->hide();
            ui->exp3->hide();
            ui->exp4->hide();
            ui->exp5->hide();
            percPulse = 0;
            expSeq++;
            QTimer::singleShot(100, this, &startupWindow::manualAESequence);
            return;
        }
    }
    else if(expSeq == 32){ numPulse = 2; ui->exp1->show(); percPulse=1;}
    else if(expSeq == 37){ ui->exp2->show(); percPulse++;}
    else if(expSeq == 42){ ui->exp3->show(); percPulse++;}
    else if(expSeq == 47){ ui->exp4->show(); percPulse++;}
    else if(expSeq == 52){ ui->exp5->show(); percPulse++;}
    else if(expSeq == 57){
        exposureResult = "COMPLETED";
        mAsFinal[numPulse-1] = mAs ;
        FilterFinal[numPulse-1] = Filter;
        kVFinal[numPulse-1] = kV;
        sequenceCompleted();
        return;
    }
    expSeq++;
    QTimer::singleShot(100, this, &startupWindow::manualAESequence);
    return;
}

//_____________________________________________________________________________________________________________________________________________
//___________________ AEC AE _______________________________________________________________________________________________________________
//_____________________________________________________________________________________________________________________________________________
void startupWindow::AecAESequence(void){
    if(!getXrayPush()) {
        seqError(902);
        return;
    }

    if(expSeq == 0){
        ui->prepFrame->show();
    }else if(expSeq == 20){ numPulse = 1; ui->exp1->show(); percPulse = 1;}
    else if(expSeq == 22){ ui->exp2->show(); percPulse++;}
    else if(expSeq == 24){ ui->exp3->show(); percPulse++;}
    else if(expSeq == 26){ ui->exp4->show(); percPulse++;}
    else if(expSeq == 28){ ui->exp5->show(); percPulse++;}
    else if(expSeq == 30){
        mAsFinal[numPulse-1] = mAs ;
        FilterFinal[numPulse-1] = Filter;
        kVFinal[numPulse-1] = kV;
        pulseDataReady = false;
        ui->kV0->setText(QString("kV:%1").arg(kV));
        ui->mAs0->setText(QString("mAs:%1").arg(mAs));
        ui->filter0->setText(QString("Filter:%1").arg(Filter));
        pAws->gantryPulseCompleted();
    }
    else if(expSeq == 31){
        // Wait for the AWS data
        if(!pulseDataReady){
            QTimer::singleShot(100, this, &startupWindow::AecAESequence);
            return;
        }else{
            ui->exp1->hide();
            ui->exp2->hide();
            ui->exp3->hide();
            ui->exp4->hide();
            ui->exp5->hide();
            percPulse = 0;
            expSeq++;
            QTimer::singleShot(100, this, &startupWindow::AecAESequence);
            return;
        }
    }
    else if(expSeq == 32){ numPulse = 2; ui->exp1->show(); percPulse=1;}
    else if(expSeq == 37){ ui->exp2->show(); percPulse++;}
    else if(expSeq == 42){ ui->exp3->show(); percPulse++;}
    else if(expSeq == 47){ ui->exp4->show(); percPulse++;}
    else if(expSeq == 52){ ui->exp5->show(); percPulse++;}
    else if(expSeq == 57){
        mAsFinal[numPulse-1] = mAs ;
        FilterFinal[numPulse-1] = Filter;
        kVFinal[numPulse-1] = kV;
        pulseDataReady = false;
        ui->kV1->setText(QString("kV:%1").arg(kV));
        ui->mAs1->setText(QString("mAs:%1").arg(mAs));
        ui->filter1->setText(QString("Filter:%1").arg(Filter));
        pAws->gantryPulseCompleted();
    }
    else if(expSeq == 58){
        // Wait for the AWS data
        if(!pulseDataReady){
            QTimer::singleShot(100, this, &startupWindow::AecAESequence);
            return;
        }else{
            ui->exp1->hide();
            ui->exp2->hide();
            ui->exp3->hide();
            ui->exp4->hide();
            ui->exp5->hide();
            percPulse = 0;
            expSeq++;
            QTimer::singleShot(100, this, &startupWindow::AecAESequence);
            return;
        }
    }
    else if(expSeq == 59){ numPulse = 3; ui->exp1->show(); percPulse=1;}
    else if(expSeq == 64){ ui->exp2->show(); percPulse++;}
    else if(expSeq == 69){ ui->exp3->show(); percPulse++;}
    else if(expSeq == 74){ ui->exp4->show(); percPulse++;}
    else if(expSeq == 79){ ui->exp5->show(); percPulse++;}
    else if(expSeq == 84){
        exposureResult = "COMPLETED";
        mAsFinal[numPulse-1] = mAs ;
        FilterFinal[numPulse-1] = Filter;
        kVFinal[numPulse-1] = kV;
        sequenceCompleted();
        return;
    }
    expSeq++;
    QTimer::singleShot(100, this, &startupWindow::AecAESequence);
    return;
}

