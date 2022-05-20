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

    ui->messages->hide();

    timeEv = 0;
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

    tubeHu = 0;
    tubeTemp = 23;
    setTubeData(tubeHu,tubeTemp);

    setXrayEnable(false);
    xrayPushStat = false;

    errorCondition = "";
    ui->errorFrame->hide();


    connect(this->ui->tomoCfgId, SIGNAL(currentTextChanged(QString)), this, SLOT(tomoIdChangedSlot(QString)));
    connect(this->ui->tomoCfgList, SIGNAL(currentTextChanged(QString)), this, SLOT(tomoListChangedSlot(QString)));
    connect(this->ui->projectionsCombo, SIGNAL(currentTextChanged(QString)), this, SLOT(projectionSelectionSlot(QString)));

    connect(this->ui->potterCombo, SIGNAL(currentTextChanged(QString)), this, SLOT(accessoriesSlot(QString)));
    connect(this->ui->paddlesCombo, SIGNAL(currentTextChanged(QString)), this, SLOT(accessoriesSlot(QString)));
    connect(this->ui->accessoriesCombo, SIGNAL(currentTextChanged(QString)), this, SLOT(accessoriesSlot(QString)));

    connect(this->ui->thickSpin, SIGNAL(valueChanged(int)), this, SLOT(breastSlot(int)));
    connect(this->ui->forceSpin, SIGNAL(valueChanged(int)), this, SLOT(breastSlot(int)));


    connect(ui->generateError, SIGNAL(pressed()), this, SLOT(generateError()), Qt::UniqueConnection);
    connect(ui->cancError, SIGNAL(pressed()), this, SLOT(cancelError()), Qt::UniqueConnection);

    timeEv = startTimer(500);
    changeEvent = true;
}

startupWindow::~startupWindow()
{
    delete ui;
}

void startupWindow::initWindow(void)
{
    connect(ui->openButton,SIGNAL(released()),this,SLOT(onPushButton()),Qt::UniqueConnection);

    angolo = 0;
    timeEv = startTimer(1000);
}

void startupWindow::exitWindow(void)
{
    disconnect(ui->openButton);

    killTimer(timeEv);
    timeEv=0;
}

void startupWindow::onPushButton(void){
}



void startupWindow::timerEvent(QTimerEvent* ev)
{
    if(ev->timerId() == timeEv)
    {
       bool ready = checkReadyForExposure();
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
    }

}

void startupWindow::addLogEvent(QString data){
    ui->logEdit->appendPlainText(data);
}

void startupWindow::setStudyName(QString data){
    if(data == "")  ui->studyName->setText("CLOSED");
    else ui->studyName->setText(data);
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

void startupWindow::moveTrx(int angolo){
    ui->trxEdit->setText("---");
}
void startupWindow::moveArm(int angolo){
    ui->armEdit->setText("---");
}

void startupWindow::setTrx(int angolo){
    ui->trxEdit->setText(QString("%1").arg(angolo/100));

}
void startupWindow::setArm(int angolo){
    ui->armEdit->setText(QString("%1").arg(angolo));

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

void startupWindow::emitXrayPushButtonPressed(void){
    xrayPushStat = true;
    pAws->gantryXrayPushEvent();
    emit xrayPushButtonSgn(true);
}
void startupWindow::emitXrayPushButtonReleased(void){
    xrayPushStat = false;
    emit xrayPushButtonSgn(false);
}

void startupWindow::setXrayEnable(bool stat){
    if(!stat){
        ui->xrayPushButton->disconnect();
        ui->xrayPushButton->setStyleSheet("background-color: rgba(0, 0, 127,100);");
    }else{
        connect(ui->xrayPushButton, SIGNAL(pressed()), this, SLOT(emitXrayPushButtonPressed()), Qt::UniqueConnection);
        connect(ui->xrayPushButton, SIGNAL(released()), this, SLOT(emitXrayPushButtonReleased()), Qt::UniqueConnection);
        ui->xrayPushButton->setStyleSheet("background-color: rgba(0, 0, 127,255);");
    }
}


void startupWindow::accessoriesSlot(QString data){
    if(!changeEvent) return;
    QString pot = ui->potterCombo->currentText();
    QString pad= ui->paddlesCombo->currentText();
    QString acc = ui->accessoriesCombo->currentText();

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

void startupWindow::generateError(void){
    errorCondition = "00200";
    ui->errorLabel->setText("[M00200]");
    ui->errorFrame->show();

    pAws->gantryError(errorCondition);
}

void startupWindow::cancelError(void){
    errorCondition = "";
    ui->errorLabel->setText("");
    ui->errorFrame->hide();
}

bool startupWindow::checkReadyForExposure(void){
    if(errorCondition != "") return false;
    if(ui->toArmEdit->text() == "---") return false;
    if(ui->fromArmEdit->text() == "---") return false;
    if(ui->armEdit->text() == "---") return false;

    int from = ui->fromArmEdit->text().toInt();
    int to = ui->toArmEdit->text().toInt();
    int angolo = ui->armEdit->text().toInt();
    if(angolo > to) return false;
    if(angolo < from) return false;

    if(ui->potterCombo->currentText() == "POTTER_UNDETECTED") return false;
    if(ui->accessoriesCombo->currentText() == "UNDETECTED") return false;
    if(ui->paddlesCombo->currentText() == "PAD_UNDETECTED") return false;
    if(ui->paddlesCombo->currentText() == "PAD_UNLOCKED") return false;


    return true;

}
