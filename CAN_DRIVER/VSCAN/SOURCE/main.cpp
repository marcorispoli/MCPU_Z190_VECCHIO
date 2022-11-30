#define MAIN_CPP

#include "application.h"

#include <QFile>
static QFile* logfd;

/**
 * @brief dbgWindowMsg
 * This is the Callback of the Debug messages in case of the
 * application should be run with the -win option.
 *
 * In case the application should be launched with -win option
 * every debug message is redirect to the Application Window.
 *
 * @param type This is the type of the debug message
 * @param context
 * @param msg this is the string of the message
 *
 * \ingroup   applicationModule
 */
void dbgWindowMsg(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QByteArray localMsg = msg.toLocal8Bit();

    switch (type) {
    case QtDebugMsg:
        // Redirect the message handling to the Window
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

/**
 * @brief noDebugMessageHandler
 * This is the handler of the debug messages when the application
 * is launched without parameters.
 *
 * When the application is launched for the operating mode,
 * no debug messages shall be produced.
 *
 * @param type This is the type of the debug message
 * @param context
 * @param msg this is the string of the message
 *
 * \ingroup   applicationModule
 */
void noDebugMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{

}

/**
 * @brief noDebugMessageHandler
 *
 * This is the Callback of the Debug messages in case of the
 * application should be run with the -log option.
 *
 * In case the application should be launched with -log option
 * every debug message is redirect to the C:/OEM/Gantry/Logs/candriver.log file.
 *
 * @param type This is the type of the debug message
 * @param context
 * @param msg this is the string of the message
 *
 * \ingroup   applicationModule
 */
void dbgMessageOutputToFile(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QByteArray localMsg = msg.toLocal8Bit();
    logfd->write(localMsg.data());
    logfd->write("\n");
    logfd->flush();
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QString info;
    bool canloopback = false;

    // Identifies the starting mode
    if(argc == 1){
        // No messages
        qInstallMessageHandler(noDebugMessageHandler);
    }else{
        for(int i=1; i<argc; i++)
        {
            QString dato = QString(argv[i]);
            if(dato == "-win"){
                qInstallMessageHandler(dbgWindowMsg);
                WINDOW = new debugWindow();
                WINDOW->show();
            }else if(dato == "-dbg"){
                // The debug messages will be redirected to the console
            }else if(dato == "-log"){
                logfd = new QFile("C:/OEM/Gantry/Logs/candriver.log");
                if (logfd->open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Append)){
                    // The debug messages will be redirected to the console
                    qInstallMessageHandler(dbgMessageOutputToFile);
                }
            }else if(dato == "-canLoopback"){
                canloopback = true;
            }
        }
    }


    INTERFACE = new Server(Application::SERVER_IP, Application::SERVER_PORT);

    CAN = new canDriver();    
    CAN->driverOpen(Application::CAN_BAUDRATE, canloopback);

    QObject::connect(CAN,SIGNAL(receivedCanFrame(ushort , QByteArray )), INTERFACE, SLOT(receivedCanFrame(ushort , QByteArray)),Qt::QueuedConnection);
    QObject::connect(INTERFACE,SIGNAL(sendToCan(ushort , QByteArray )), CAN, SLOT(sendOnCanSlot(ushort , QByteArray)),Qt::QueuedConnection);
    INTERFACE->Start();



    return a.exec();
}
