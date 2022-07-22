#include "startupWindow.h"
#include "ui_startupWindow.h"
#include "QStringConverter"
#include "QDateTime"

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
    connect(ui->eventConnectionButton, SIGNAL(pressed()), this, SLOT(onEventConnectionButtonSlot()), Qt::UniqueConnection);
    connect(ui->commandList, SIGNAL(currentTextChanged(QString)), this, SLOT(commandCurrentTextChanged(QString)),Qt::UniqueConnection);
    connect(ui->sendButton, SIGNAL(pressed()), this, SLOT(onSendButton()), Qt::UniqueConnection);
    connect(ui->pollingButton, SIGNAL(pressed()), this, SLOT(onPollingButton()), Qt::UniqueConnection);
    connect(ui->clearButton, SIGNAL(pressed()), this, SLOT(onClearButton()), Qt::UniqueConnection);

    pollingTimer = 0;

    // Init command list
    ui->commandList->clear();
    ui->commandList->addItem("EXEC_Close ");


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

}

void startupWindow::connectionStat(bool stat){
    if(stat) ui->connectedLabel->show();
    else ui->connectedLabel->hide();
    return;
}

void startupWindow::connectionButtonSlot(void){

}
void startupWindow::onEventConnectionButtonSlot(void){

}



void  startupWindow::commandCurrentTextChanged(QString item){
  // QString comando =  commandProtocol::formatCommand(QStringConverter::Encoding::Utf16LE, 1, item);
  ui->sendCommand->setText(item);
}

void  startupWindow::onPollingButton(void){

   ui->pollingCommand->setText(ui->sendCommand->text());
 //  pollingFrame =  commandProtocol::formatCommand(QStringConverter::Encoding::Utf16LE, 1, ui->pollingCommand->text());
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
    /*
  QByteArray comando =  commandProtocol::formatCommand(QStringConverter::Encoding::Utf16LE, 1, ui->sendCommand->text());
  client.txData(comando);
  addLogEvent("AWS: " + comando);
  */
}



void startupWindow::addLogEvent(QByteArray data){

    uchar val = data.at(0);
    uchar prio = val >> 6;
    uchar seq = val & 0x3F;
    uchar dest = data.at(1);
    uchar srs = data.at(2);
    uchar idx = data.at(3);
    uchar sub = data.at(4);
    uchar func = data.at(5);
    ushort len = data.at(6)*256 + data.at(7);

    QString stringa = QString("PRIO:%1, SEQ:%2, DEST:%3, SOURCE:%4, IDX:%5, SUB:%6, FUNC:%7, LEN:%8").arg(prio).arg(seq).arg(dest).arg(srs).arg(idx).arg(sub).arg(func).arg(len);



    QString ddate = QDateTime::currentDateTimeUtc().toString();
    ui->logEdit->appendPlainText(ddate + " | " + stringa);
}
