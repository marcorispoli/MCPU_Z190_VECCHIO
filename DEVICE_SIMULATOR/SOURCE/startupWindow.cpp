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
    ui->batt1Value->setValue(13);
    ui->batt2Value->setValue(13);
    ui->BattEna->setChecked(true);

    ui->PowerOn->setChecked(false);
    ui->PowerAcDc->setChecked(true);
    ui->AcDcValue->setValue(24);
    ui->powerDown->hide();

    ui->CARM_CW->setChecked(false);
    ui->CARM_CCW->setChecked(false);
    ui->CARM_UP->setChecked(false);
    ui->CARM_DWN->setChecked(false);
    ui->TILT_UP->setChecked(false);
    ui->TILT_DWN->setChecked(false);
    ui->CMP_UP->setChecked(false);
    ui->CMP_DWN->setChecked(false);
    ui->BODY_CW->setChecked(false);
    ui->BODY_CCW->setChecked(false);

    ui->XrayButton->setChecked(false);
    ui->xrayLed->hide();
    ui->xrayLmp1->hide();
    ui->xrayLmp2->hide();

    ui->Emergency->setChecked(false);
    ui->CARMSafety->setChecked(false);
    ui->ClosedDoor->setChecked(true);

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

void startupWindow::exitWindow(void)
{


}



void startupWindow::timerEvent(QTimerEvent* ev)
{
    if(ev->timerId() == timeEv)
    {
    }

}

