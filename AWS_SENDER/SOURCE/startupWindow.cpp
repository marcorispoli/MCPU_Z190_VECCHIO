#include "startupWindow.h"
#include "ui_startupWindow.h"
#include "commandprotocol.h"
#include "awscommunication.h"
#include "QStringConverter"

extern startupWindow* w1;


startupWindow::startupWindow(QWidget *parent)
    : ui(new Ui::startupWindow)
{
    ui->setupUi(this);

    ui->messages->hide();

    timeEv = 0;
    // Set the View to handle the rotation


    ui->connectedLabel->hide();
    connect(ui->connectionButton, SIGNAL(pressed()), this, SLOT(connectionButtonSlot()), Qt::UniqueConnection);
    connect(&client, SIGNAL(clientConnection(bool)), this, SLOT(clientConnection(bool)), Qt::UniqueConnection);
    connect(&client, SIGNAL(rxData(QByteArray)), this, SLOT(clientRxData(QByteArray)), Qt::UniqueConnection);

    connect(ui->commandList, SIGNAL(currentTextChanged(QString)), this, SLOT(commandCurrentTextChanged(QString)),Qt::UniqueConnection);
    connect(ui->sendButton, SIGNAL(pressed()), this, SLOT(onSendButton()), Qt::UniqueConnection);
    connect(ui->pollingButton, SIGNAL(pressed()), this, SLOT(onPollingButton()), Qt::UniqueConnection);
    connect(ui->clearButton, SIGNAL(pressed()), this, SLOT(onClearButton()), Qt::UniqueConnection);

    pollingTimer = 0;

    // Init command list
    ui->commandList->clear();
    ui->commandList->addItem("GET_RotationAngles ");
    ui->commandList->addItem("GET_Accessories ");
    ui->commandList->addItem("GET_CompressorData ");
    ui->commandList->addItem("GET_TomoConfig NARROW ");
    ui->commandList->addItem("GET_TomoConfigFile TOMO-CFG1 ");
    ui->commandList->addItem("GET_TubeTemperature ");
    ui->commandList->addItem("GET_XrayPushStatus ");
    ui->commandList->addItem("GET_ReadyForExposure ");
    ui->commandList->addItem(" ");
    ui->commandList->addItem("EXEC_Close ");
    ui->commandList->addItem("EXEC_OpenStudy PROVA ");
    ui->commandList->addItem("EXEC_AbortProjections ");
    ui->commandList->addItem("EXEC_PowerOff ");
    ui->commandList->addItem("EXEC_ResetError ");
    ui->commandList->addItem("EXEC_TrxPosition ANGLE 0 ");
    ui->commandList->addItem("EXEC_TrxPosition TOMO_HOME  WIDE  ");
    ui->commandList->addItem("EXEC_TrxPosition TOMO_FINAL WIDE  ");
    ui->commandList->addItem("EXEC_ArmPosition RCC 0 -5 5 ");
    ui->commandList->addItem(" ");
    ui->commandList->addItem("SET_ExposureMode MAN_2D COMP_ENABLED COLLI_AUTO TomoNone ");
    ui->commandList->addItem("SET_ExposureMode AEC_2D COMP_ENABLED COLLI_AUTO TomoNone");
    ui->commandList->addItem("SET_ExposureMode MAN_3D COMP_ENABLED COLLI_AUTO WIDE");
    ui->commandList->addItem("SET_ExposureMode AEC_3D COMP_ENABLED COLLI_AUTO WIDE");
    ui->commandList->addItem("SET_ExposureMode MAN_COMBO COMP_ENABLED COLLI_AUTO WIDE");
    ui->commandList->addItem("SET_ExposureMode AEC_COMBO COMP_ENABLED COLLI_AUTO WIDE");
    ui->commandList->addItem("SET_ExposureMode MAN_AE COMP_ENABLED COLLI_AUTO TomoNone");
    ui->commandList->addItem("SET_ExposureMode AEC_AE COMP_ENABLED COLLI_AUTO TomoNone");
    ui->commandList->addItem(" ");
    ui->commandList->addItem("EXEC_StartXraySequence 25 100 Rh ");
    ui->commandList->addItem("EXEC_StartXraySequence 35 10 Rh ");
    ui->commandList->addItem("EXEC_StartXraySequence 28 100 Al");
    ui->commandList->addItem("SET_PulseData 28.0 100 Ag TomoNone ");
    ui->commandList->addItem("SET_PulseData 28.0 100 Al INTERMEDIATE ");
    ui->commandList->addItem(" ");
    ui->commandList->addItem("SET_CompressionRelease KEEP ");
    ui->commandList->addItem("SET_CompressionRelease RELEASE ");
    ui->commandList->addItem("SET_ProjectionList RCC LCC RMLO LMLO ");
    ui->commandList->addItem("SET_XrayPushEnable ");


}

startupWindow::~startupWindow()
{
    delete ui;
}

void startupWindow::initWindow(void)
{

    timeEv = startTimer(1000);
}

void startupWindow::exitWindow(void)
{

    killTimer(timeEv);
    timeEv=0;
}


void startupWindow::timerEvent(QTimerEvent* ev)
{
    if(ev->timerId() == timeEv)
    {


    }
    if(ev->timerId() == pollingTimer)
    {
        client.txData(pollingFrame);
        addLogEvent("AWS: " + pollingFrame);

    }

}

void startupWindow::clientConnection(bool stat){
    if(stat) ui->connectedLabel->show();
    else ui->connectedLabel->hide();
}

void startupWindow::connectionButtonSlot(void){
    client.Start(ui->addressEdit->text(), ui->portEdit->text().toUInt());
}

void  startupWindow::commandCurrentTextChanged(QString item){
  // QString comando =  commandProtocol::formatCommand(QStringConverter::Encoding::Utf16LE, 1, item);
  ui->sendCommand->setText(item);
}

void  startupWindow::onPollingButton(void){

   ui->pollingCommand->setText(ui->sendCommand->text());
   pollingFrame =  commandProtocol::formatCommand(QStringConverter::Encoding::Utf16LE, 1, ui->pollingCommand->text());
   if(pollingTimer) killTimer(pollingTimer);
   pollingTimer = startTimer(ui->pollingTime->value());

}
void  startupWindow::onClearButton(void){
  if(pollingTimer) killTimer(pollingTimer);
  pollingTimer = 0;
  pollingFrame = "";
  ui->pollingCommand->setText("");

}

void  startupWindow::onSendButton(void){
  QByteArray comando =  commandProtocol::formatCommand(QStringConverter::Encoding::Utf16LE, 1, ui->sendCommand->text());
  client.txData(comando);
  addLogEvent("AWS: " + comando);
}

void startupWindow::clientRxData(QByteArray data){
    addLogEvent("   GANTRY: " + data);
}

void startupWindow::addLogEvent(QByteArray data){
    // Decodes the data based on the Encoding format
    auto toUtf = QStringDecoder(QStringConverter::Encoding::Utf16LE);
    QString frame = toUtf(data);


    QString ddate = QDateTime::currentDateTimeUtc().toString();
    ui->logEdit->appendPlainText(ddate + " | " + data);
}
