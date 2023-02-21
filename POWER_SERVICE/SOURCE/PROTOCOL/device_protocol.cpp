#include "application.h"
#include "can_device_protocol.h"

deviceProtocol::deviceProtocol(void):canDeviceProtocol(Application::DEVICE_ID, Application::IP_CAN_ADDRESS, Application::CAN_PORT)
{
    workflow = _WORKFLOW_INITIALIZATION;
    subWorkflow = 0;
    QTimer::singleShot(100,this, SLOT(workflowINITIALIZATION()));

    cmdTest = 0;
    abortCmd = false;
}



void deviceProtocol::workflowINITIALIZATION(void){
    workflow = _WORKFLOW_INITIALIZATION;

    // Waiting for the answer
    if(isCommunicationPending()){
        QTimer::singleShot(1,this, SLOT(workflowINITIALIZATION()));
        return;
    }

    switch(subWorkflow){
        case 0:

            if(!accessRegister(canDeviceProtocolFrame::READ_REVISION)){
                QTimer::singleShot(100,this, SLOT(workflowINITIALIZATION()));
                return;
            }

            subWorkflow++;
            QTimer::singleShot(10,this, SLOT(workflowINITIALIZATION()));
            return;

        case 1:
            if(!isCommunicationOk()){
                qDebug() << "REVISION FRAME TIMEOUT: " << getFrameErrorStr();
                subWorkflow = 0;
                QTimer::singleShot(100,this, SLOT(workflowINITIALIZATION()));
                return;
            }

            qDebug() << "DEVICE REVISION: " << (uchar) revisionRegister.d[0] << "." <<  revisionRegister.d[1] << "." << revisionRegister.d[2];
            subWorkflow = 2;
            QTimer::singleShot(1,this, SLOT(workflowINITIALIZATION()));
            break;

        case 2:

            if(abortCmd){
                subWorkflow = 102;
                QTimer::singleShot(0,this, SLOT(workflowINITIALIZATION()));
                return;
            }

            if(cmdTest){
                subWorkflow = 100;
                QTimer::singleShot(0,this, SLOT(workflowINITIALIZATION()));
                return;
            }

            QTimer::singleShot(100,this, SLOT(workflowINITIALIZATION()));
            return;

        case 100:
            commandRegister.valid = false;
            if(!accessRegister(canDeviceProtocolFrame::COMMAND_EXEC,cmdTest,cmdParam[0],cmdParam[1],cmdParam[2],cmdParam[3])){
                cmdTest = 0;
                subWorkflow = 2;
                QTimer::singleShot(1,this, SLOT(workflowINITIALIZATION()));
            }else qDebug() << "COMMAND TEST";

            cmdTest = 0;
            subWorkflow++;
            QTimer::singleShot(1,this, SLOT(workflowINITIALIZATION()));
            return;

        case 101:
            if(!isCommunicationOk()){
                qDebug() << "COMMAND FRAME TIMEOUT";
                cmdTest = 0;
                subWorkflow = 2;
                QTimer::singleShot(1,this, SLOT(workflowINITIALIZATION()));
                return;
            }

            switch(commandRegister.status){
                case canDeviceProtocolFrame::CAN_COMMAND_EXECUTING:

                    subWorkflow = 102;
                    QTimer::singleShot(100,this, SLOT(workflowINITIALIZATION()));
                    return;
                case canDeviceProtocolFrame::CAN_COMMAND_ERROR:
                    qDebug() << "COMMAND:" << commandRegister.command <<  "- ERRCODE: " <<  canDeviceProtocolFrame::getCommandErrorStr(commandRegister.error);
                    cmdTest = 0;
                    subWorkflow = 2;
                    QTimer::singleShot(10,this, SLOT(workflowINITIALIZATION()));
                    return;

                case canDeviceProtocolFrame::CAN_COMMAND_EXECUTED:
                    qDebug() << "COMMAND:" << commandRegister.command << "COMPLETED. B0:" << commandRegister.b0 << ", B1:" << commandRegister.b1;
                    cmdTest = 0;
                    subWorkflow = 2;
                    QTimer::singleShot(10,this, SLOT(workflowINITIALIZATION()));
                    return;
            }

            break;

        case 102:
            if(abortCmd){
                if(!abortCommand()){
                    QTimer::singleShot(100,this, SLOT(workflowINITIALIZATION()));
                    return;
                }else{
                    qDebug() << "COMMAND ABORT";
                    abortCmd = false;
                }
            }else if(!accessRegister(canDeviceProtocolFrame::READ_COMMAND)){
                cmdTest = 0;
                subWorkflow = 2;
                QTimer::singleShot(1,this, SLOT(workflowINITIALIZATION()));
                return;
            }

            subWorkflow = 101;
            QTimer::singleShot(1,this, SLOT(workflowINITIALIZATION()));
            return;

    }
}


