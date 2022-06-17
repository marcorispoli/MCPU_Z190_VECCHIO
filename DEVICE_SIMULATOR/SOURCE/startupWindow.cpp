#include "startupWindow.h"
#include "ui_startupWindow.h"
#include "power_service.h"
#include <QFileDialog>


extern powerService*  pService;

startupWindow::startupWindow(QWidget *parent)
    : ui(new Ui::startupWindow)
{
    ui->setupUi(this);


}

startupWindow::~startupWindow()
{
    delete ui;
}

void startupWindow::initWindow(void)
{

    QString revision = QString("FW REVISION: %1").arg(pService->revisionreg.data[0]) + "." + QString("%1").arg(pService->revisionreg.data[1]) + "." + QString("%2").arg(pService->revisionreg.data[2]);
    ui->revisionLabel->setText(revision);

    // Power Panel
    ui->PowerOnButton->setChecked(false);
    ui->PowerAcDc->setChecked(true);    
    ui->BattEna->setChecked(true);
    ui->batt1Value->setValue(13);
    ui->batt2Value->setValue(13);
    ui->MotorEnabled->setChecked(false);
    ui->MOTOR_DC_OK->setChecked(false);

    // Pedalboard
    ui->CARM_UP->setChecked(false);
    ui->CARM_DWN->setChecked(false);
    ui->CMP_UP->setChecked(false);
    ui->CMP_DWN->setChecked(false);

    // Xray Panel
    ui->XrayButton->setChecked(false);
    ui->xrayLed->hide();
    ui->xrayLmp1->hide();
    ui->xrayLmp2->hide();
    ui->BurninJumper->setChecked(false);

    // Safety Panel
    ui->Emergency->setChecked(false);
    ui->MotorSafety->setChecked(false);
    ui->ClosedDoor->setChecked(true);

    // Bus Hardware Panel
    ui->HW_CARM_DWN->setChecked(false);
    ui->HW_CARM_UP->setChecked(false);
    ui->HW_CMP_DWN->setChecked(false);
    ui->HW_CMP_UP->setChecked(false);
    ui->HW_COMP_ENA->setChecked(false);
    ui->HW_COMP_ON->setChecked(false);
    ui->HW_DET_EW->setChecked(false);
    ui->HW_DET_PREP->setChecked(false);
    ui->HW_GRID_ENA->setChecked(false);
    ui->HW_HV_ON->setChecked(false);



}

bool startupWindow::getEmergencyStat(void){
    return ui->Emergency->isChecked();
}
bool startupWindow::getPowerdownStat(void){
    return (! ui->PowerAcDc->isChecked());
}
bool startupWindow::getMotorSafetySwitchStat(void){
    return (ui->MotorSafety->isChecked());
}
bool startupWindow::getBatteryEnableStat(void){
    return (ui->BattEna->isChecked());
}

bool startupWindow::getCArmUpStat(void){
    return (ui->CARM_UP->isChecked());
}
bool startupWindow::getCArmDwnStat(void){
    return (ui->CARM_DWN->isChecked());
}

bool startupWindow::getTiltUpStat(void){
    return (ui->HW_TILT_UP->isChecked());
}
bool startupWindow::getTiltDwnStat(void){
    return (ui->HW_TILT_DWN->isChecked());
}

bool startupWindow::getRotCwStat(void){
    return (ui->HW_ROT_CW->isChecked());
}

bool startupWindow::getRotCCwStat(void){
    return (ui->HW_ROT_CCW->isChecked());
}
bool startupWindow::getBodyCwStat(void){
    return (ui->BODY_CW->isChecked());
}
bool startupWindow::getBodyCCwStat(void){
    return (ui->BODY_CW->isChecked());
}


bool startupWindow::getCompressorOnStat(void){
    return (ui->HW_COMP_ON->isChecked());
}

bool startupWindow::getXRayReqStat(void){
    return (ui->XrayButton->isChecked());
}
bool startupWindow::getBurninStat(void){
    return (ui->BurninJumper->isChecked());
}



bool startupWindow::getCloseDoorStat(void){
    return (ui->ClosedDoor->isChecked());
}

bool startupWindow::getPedalCarmUpStat(void){
    return (ui->CARM_UP->isChecked());
}

bool startupWindow::getPedalCarmDwnStat(void){
    return (ui->CARM_DWN->isChecked());
}

bool startupWindow::getPedalCmpUpStat(void){
    return (ui->CMP_UP->isChecked());
}
bool startupWindow::getPedalCmpDwnStat(void){
    return (ui->CMP_DWN->isChecked());
}

bool startupWindow::getSymOutputStat(uchar index){

   switch(index){
   case 0:  return (ui->OUT_0->isChecked());
   case 1:  return (ui->OUT_1->isChecked());
   case 2:  return (ui->OUT_2->isChecked());
   case 3:  return (ui->OUT_3->isChecked());
   case 4:  return (ui->OUT_4->isChecked());
   case 5:  return (ui->OUT_5->isChecked());
   case 6:  return (ui->OUT_6->isChecked());
   case 7:  return (ui->OUT_7->isChecked());
   case 8:  return (ui->OUT_8->isChecked());
   case 9:  return (ui->OUT_9->isChecked());
   case 10:  return (ui->OUT_10->isChecked());
   }

}


void startupWindow::exitWindow(void)
{


}



void startupWindow::timerEvent(QTimerEvent* ev)
{
    if(ev->timerId() == timeEv)
    {
    }

}

