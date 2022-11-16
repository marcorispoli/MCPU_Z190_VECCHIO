#include "application.h"
#include "ui_startupWindow.h"

startupWindow::startupWindow(QWidget *parent)
    : ui(new Ui::startupWindow)
{
    ui->setupUi(this);


    // Set the View to handle the rotation



    connect(ui->logUpdateButtonButton, SIGNAL(pressed()), this, SLOT(onLogUpdateButton()), Qt::UniqueConnection);
    connect(ui->logClearButton, SIGNAL(pressed()), this, SLOT(onLogClearButton()), Qt::UniqueConnection);
    connect(ui->logEnableCheck, SIGNAL(stateChanged(int)), this, SLOT(on_logEnableCheck_stateChanged(int)));

    connect(ui->startPre, SIGNAL(pressed()), this, SLOT(onStartPre()), Qt::UniqueConnection);
    connect(ui->startPulse, SIGNAL(pressed()), this, SLOT(onStartPulse()), Qt::UniqueConnection);
    connect(ui->start3DPre, SIGNAL(pressed()), this, SLOT(onStart3DPre()), Qt::UniqueConnection);
    connect(ui->start3DPulse, SIGNAL(pressed()), this, SLOT(onStart3DPulse()), Qt::UniqueConnection);

    connect(ui->abortRx, SIGNAL(pressed()), this, SLOT(onAbortRx()), Qt::UniqueConnection);


    pollingTimer  = startTimer(500);
    sysMsgTimer =0;
}

startupWindow::~startupWindow()
{
    delete ui;
}

void startupWindow::initWindow(void)
{


}

void startupWindow::exitWindow(void)
{

    if(pollingTimer)    killTimer(pollingTimer);

}


void startupWindow::timerEvent(QTimerEvent* ev)
{
    if(ev->timerId() == pollingTimer)
    {
        if(ui->detectorEna->isChecked()){
            ui->gridEna->setCheckState(Qt::Checked);
        }

        if(sysMsgTimer){
            sysMsgTimer--;
            if(sysMsgTimer==0) ui->sysMessages->setText("");
        }
        return;
    }


}

void startupWindow::onLogUpdateButton(void){


}
void startupWindow::onLogClearButton(void){
    ui->debugText->appendPlainText("");
    ui->debugText->clear();
}

void startupWindow::onLogRxSlot(QByteArray data){
    QString stringa = "";

    for(int j=0; j< data.size(); j++){
        stringa += QString("%1 ").arg((unsigned char) data[j]);
    }
    ui->debugText->appendPlainText(stringa);
}


void startupWindow::onStartPre(void){

    QList<QString> comando;
    comando.append("E");
    comando.append("1");
    comando.append("SetExposurePre");
    comando.append(ui->kVPre->text());
    comando.append(ui->mAsPre->text());
    if(ui->largeFocus->isChecked()) comando.append("LARGE");
    else comando.append("SMALL");

    INTERFACE->SetExposurePre(&comando);

    comando.clear();
    comando.append("E");
    comando.append("2");
    comando.append("StartExposure ");
    comando.append("2DAEC");

    INTERFACE->StartExposure(&comando);

}


void startupWindow::onStartPulse(void){

    QList<QString> comando;
    comando.append("E");
    comando.append("1");
    comando.append("SetExposurePulse ");
    comando.append(ui->kVPulse->text());
    comando.append(ui->mAsPulse->text());
    if(ui->largeFocus->isChecked()) comando.append("LARGE");
    else comando.append("SMALL");
    if(ui->detectorEna->isChecked()) comando.append("DETECTOR");
    else comando.append("NO_DETECTOR");
    if(ui->gridEna->isChecked()) comando.append("GRID");
    else comando.append("NO_GRID");

    qDebug() << comando;
    INTERFACE->SetExposurePulse(&comando);

    comando.clear();
    comando.append("E");
    comando.append("2");
    comando.append("StartExposure ");
    comando.append("2D");

    INTERFACE->StartExposure(&comando);

}

void startupWindow::onStart3DPulse(void){

    QList<QString> comando;
    comando.append("E");
    comando.append("1");
    comando.append("SetExposurePulse ");
    comando.append(ui->kVPulse_3D->text());

    float mAs = ui->mAsPulse_3D->text().toFloat() / ui->nPulse->text().toFloat();
    comando.append(QString("%1").arg(mAs));

    comando.append("LARGE");
    comando.append("DETECTOR");
    comando.append("NO_GRID");

    qDebug() << comando;
    INTERFACE->SetExposurePulse(&comando);

    comando.clear();
    comando.append("E");
    comando.append("2");
    comando.append("SetTomoConfig ");
    comando.append(ui->nPulse->text());
    comando.append(ui->nSkip->text());

    qDebug() << comando;
    INTERFACE->SetTomoConfig(&comando);

    comando.clear();
    comando.append("E");
    comando.append("2");
    comando.append("StartExposure ");
    comando.append("3D");

    qDebug() << comando;
    INTERFACE->StartExposure(&comando);

}
void startupWindow::onStart3DPre(void){



}

void startupWindow::onAbortRx(void){
    QList<QString> comando;
    comando.append("E");
    comando.append("1");
    comando.append("AbortExposure");
    INTERFACE->AbortExposure(&comando);
}

void startupWindow::on_logEnableCheck_stateChanged(int arg1)
{
    static bool connected = false;
    if(arg1){
        if(!connected) connect(pComm, SIGNAL(rxDataEventSgn(QByteArray)), this, SLOT(onLogRxSlot(QByteArray)), Qt::QueuedConnection);
        connected = true;
    }else{
        disconnect(pComm, SIGNAL(rxDataEventSgn(QByteArray)), this, SLOT(onLogRxSlot(QByteArray)));
        connected = false;

    }
}

void startupWindow::EventSetXrayEna(ushort seq, bool state){
    if(state) ui->rxMessage->setText("PRESS RX BUTTON!");
    else ui->rxMessage->setText("RELEASE RX BUTTON!");
};

void startupWindow::EventGetPulseData(ushort seq){
    QList<QString> comando;
    comando.append("E");
    comando.append("1");
    comando.append("SetExposurePulse ");
    comando.append(ui->kVPulse->text());
    comando.append(ui->mAsPulse->text());
    if(ui->largeFocus->isChecked()) comando.append("LARGE");
    else comando.append("SMALL");
    comando.append("DETECTOR");
    comando.append("GRID");

    INTERFACE->SetExposurePulse(&comando);
};

void startupWindow::EventStatus(ushort seq,uchar stat){};
void startupWindow::EventMessage(ushort seq,QString msg){
    ui->sysMessages->setText(msg);
    sysMsgTimer = 6;
};
void startupWindow::EventExposureError(ushort seq, uchar code){};


void startupWindow::EventXrayCompleted(ushort seq, uchar code, uchar error){};
void startupWindow::EventSwError(ushort seq, uchar error){};
