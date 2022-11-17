#include "application.h"
#include "ui_startupWindow.h"

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QByteArray localMsg = msg.toLocal8Bit();

    switch (type) {
    case QtDebugMsg:
         WINDOW->onDebug(localMsg);
        break;
    case QtInfoMsg:

        break;
    case QtWarningMsg:

        break;
    case QtCriticalMsg:

        break;
    case QtFatalMsg:

        break;
    }
}

startupWindow::startupWindow(QWidget *parent)
    : ui(new Ui::startupWindow)
{
    ui->setupUi(this);
    generalDebugIndex = 0;

    // Set the View to handle the rotation


    connect(ui->logClearButton, SIGNAL(pressed()), this, SLOT(onLogClearButton()), Qt::UniqueConnection);
    connect(ui->debugClearButton, SIGNAL(pressed()), this, SLOT(onDebugClearButton()), Qt::UniqueConnection);
    connect(ui->logEnableCheck, SIGNAL(stateChanged(int)), this, SLOT(on_logEnableCheck_stateChanged(int)));

    connect(ui->startPre, SIGNAL(pressed()), this, SLOT(onStartPre()), Qt::UniqueConnection);
    connect(ui->startPulse, SIGNAL(pressed()), this, SLOT(onStartPulse()), Qt::UniqueConnection);
    connect(ui->start3DPre, SIGNAL(pressed()), this, SLOT(onStart3DPre()), Qt::UniqueConnection);
    connect(ui->start3DPulse, SIGNAL(pressed()), this, SLOT(onStart3DPulse()), Qt::UniqueConnection);

    connect(ui->abortRx, SIGNAL(pressed()), this, SLOT(onAbortRx()), Qt::UniqueConnection);


    pollingTimer  = startTimer(500);


    qInstallMessageHandler(myMessageOutput);
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


        return;
    }


}


void startupWindow::onLogClearButton(void){
    ui->r2cpText->appendPlainText("");
    ui->r2cpText->clear();
}
void startupWindow::onDebugClearButton(void){
    ui->debugText->appendPlainText("");
    ui->debugText->clear();
}

void startupWindow::onLogRxSlot(QByteArray data){
    tSocket_Msg_ptr pEthMsg = reinterpret_cast< tSocket_Msg_ptr >( data.data() );

    QString stringa = QString("%1> ").arg(generalDebugIndex++);
    stringa += QString("SQ:%1 ").arg(pEthMsg->header.sequence);
    stringa += QString("PR:%1 ").arg(pEthMsg->header.priority);
    stringa += QString("ORG:%1 ").arg(pEthMsg->header.iss_node);
    stringa += QString("DST:%1 ").arg(pEthMsg->header.dest_node);
    stringa += QString("IDX:%1 ").arg(pEthMsg->header.index);
    switch(pEthMsg->header.index){
        case NETWORK_COMMANDS_ENTRY: stringa+="(NET) "; break;
        case R2CP_COMMANDS_ENTRY: stringa+="(R2CP) "; break;
        case GENERATOR_COMMANDS_ENTRY: stringa+="(GENERATOR) "; break;
        case SYSTEM_COMMANDS_ENTRY: stringa+="(SYSTEM) "; break;
        case PATIENT_COMMANDS_ENTRY: stringa+="(PATIENT) "; break;
        case SERVICE_COMMANDS_ENTRY: stringa+="(SERVICE) "; break;
    }
    stringa += QString("SUBIDX:%1 ").arg(pEthMsg->header.subindex);

    ushort len = htons( pEthMsg->header.len );
    stringa += QString("LEN:%1 \n      DATA:").arg(len);

    for(int j=0; j< len; j++){
        stringa += QString("[%1]%2 ").arg(j).arg((unsigned char) pEthMsg->data[j]);
    }

    ui->r2cpText->appendPlainText(stringa);


}

void startupWindow::onDebug(QByteArray data){
    QString stringa = QString("%1> %2").arg(generalDebugIndex++).arg(data);
    if(ui->debugEnable->isChecked())   ui->debugText->appendPlainText(stringa);
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


    INTERFACE->SetExposurePulse(&comando);

    comando.clear();
    comando.append("E");
    comando.append("2");
    comando.append("SetTomoConfig ");
    comando.append(ui->nPulse->text());
    comando.append(ui->nSkip->text());


    INTERFACE->SetTomoConfig(&comando);

    comando.clear();
    comando.append("E");
    comando.append("2");
    comando.append("StartExposure ");
    comando.append("3D");


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

void startupWindow::EventStatus(){
    QString stat ;
    QString anodeHU ;
    QString generatorHU;
    QString filamentStat ;
    QString rotSpeed ;
    QString prx ;

    if(R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.ExposureSwitches.Fields.ExpsignalStatus)
        prx = "PRX: ON";
    else
        prx = "PRX: OFF";

    switch(R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.GeneratorStatus){
        case R2CP::Stat_Standby:stat = "STATUS: STANDBY";break;
        case R2CP::Stat_Error:stat = "STATUS: ERROR";break;
        case R2CP::Stat_WaitFootRelease:stat = "STATUS: WAIT FOOT RELEASE";break;
        case R2CP::Stat_GoigToShutdown:stat = "STATUS: SHUTDOWN";break;
        case R2CP::Stat_Service:stat = "STATUS: SERVICE";break;
        case R2CP::Stat_Initialization:stat = "STATUS: INIT";break;
        case R2CP::Stat_Preparation:stat = "STATUS: EXPOSURE PREPARATION";break;
        case R2CP::Stat_ExpInProgress:stat = "STATUS: EXPOSURE PROGRESS";break;
        case R2CP::Stat_Ready:stat = "STATUS: EXPOSURE READY";break;
    }

    anodeHU = QString("ANODE-HU: %1").arg(R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.AccumulatedAnodeHU);
    generatorHU = QString("GEN-HU: %1").arg(R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.AccumulatedGenHU);
    if(R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.ExposureSwitches.Fields.FilStatus)
        filamentStat = "FILAMENT: ON";
    else
        filamentStat = "FILAMENT: OFF";
    switch(R2CP::CaDataDicGen::GetInstance()->radInterface.generatorStatusV6.CurrentRotorSpeed){
    case 0:rotSpeed = "ROTOR: OFF";break;
    case 1:rotSpeed = "ROTOR: LOW SPEED";break;
    case 2:rotSpeed = "ROTOR: HIGH SPEED";break;
    }

    ui->status->setText( stat + "\n" + anodeHU + "\n" + generatorHU + "\n" + filamentStat + "\n" + rotSpeed + "\n" + prx);

};
void startupWindow::EventMessage(ushort seq,QString msg){
    qDebug() << "EVENT_MESSAGE" + msg;

};


void startupWindow::EventXrayCompleted(ushort seq, uchar code, uchar error){};
