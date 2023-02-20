#include "application.h"
#include "ui_window.h"



debugWindow::debugWindow(QWidget *parent)
    : ui(new Ui::debugWindow)
{
    debugWindow::instance = this;
    ui->setupUi(this);
    generalDebugIndex = 0;

    // Set the View to handle the rotation


    connect(ui->logClearButton, SIGNAL(pressed()), this, SLOT(onLogClearButton()), Qt::UniqueConnection);
    connect(ui->debugClearButton, SIGNAL(pressed()), this, SLOT(onDebugClearButton()), Qt::UniqueConnection);
    connect(ui->logEnableCheck, SIGNAL(stateChanged(int)), this, SLOT(on_logEnableCheck_stateChanged(int)));
    connect(ui->commandTest, SIGNAL(pressed()), this, SLOT(onCommandTest()), Qt::UniqueConnection);
    connect(ui->abortCommand, SIGNAL(pressed()), this, SLOT(onAbortCommand()), Qt::UniqueConnection);



    pollingTimer  = startTimer(500);

}

debugWindow::~debugWindow()
{
    delete ui;
}

void debugWindow::initWindow(void)
{


}

void debugWindow::exitWindow(void)
{

    if(pollingTimer)    killTimer(pollingTimer);

}


void debugWindow::timerEvent(QTimerEvent* ev)
{
    if(ev->timerId() == pollingTimer)
    {


        return;
    }


}


void debugWindow::onLogClearButton(void){
    ui->r2cpText->appendPlainText("");
    ui->r2cpText->clear();
}
void debugWindow::onDebugClearButton(void){
    ui->debugText->appendPlainText("");
    ui->debugText->clear();
}
void debugWindow::onCommandTest(void){
    PROTOCOL->cmdTest = ui->cmdTestCode->text().toInt();
    PROTOCOL->cmdParam[0] = ui->cmdPar0->text().toInt();
    PROTOCOL->cmdParam[1] = ui->cmdPar1->text().toInt();
    PROTOCOL->cmdParam[2] = ui->cmdPar2->text().toInt();
    PROTOCOL->cmdParam[3] = ui->cmdPar3->text().toInt();

}
void debugWindow::onAbortCommand(void){
    PROTOCOL->abortCmd = true;
    qDebug() << "Abort Request";

}



void debugWindow::on_logEnableCheck_stateChanged(int arg1)
{
    /*
    static bool connected = false;
    if(arg1){
        if(!connected) connect(COMMUNICATION, SIGNAL(rxDataEventSgn(QByteArray)), this, SLOT(onLogRxSlot(QByteArray)), Qt::QueuedConnection);
        connected = true;
    }else{
        disconnect(COMMUNICATION, SIGNAL(rxDataEventSgn(QByteArray)), this, SLOT(onLogRxSlot(QByteArray)));
        connected = false;

    }
    */
}




void debugWindow::debugMessageHandler(QtMsgType type, QString msg){
    if(!debugWindow::instance) return;
    if(debugWindow::instance->ui->debugEnable->isChecked())   debugWindow::instance->ui->debugText->appendPlainText(msg);

}
