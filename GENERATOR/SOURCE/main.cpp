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
 * every debug message is redirect to the C:/OEM/Logs/LOGSERVICE/gantrygen.log file.
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

/**
 * @brief qMain
 * This is the Application Main function.
 *
 *
 * @param argc
 * @param argv
 *  - -log: the debug messages are redirect to the gantrygen.log file;
 *  - -win: the application run with the debug Window;
 *  - No parameter for the Operating mode (no debug messages);
 * @return
 *
 * \ingroup   applicationModule
 */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Initialize the WINDOW to nullptr so that in case of Operating mode
    // the application skips the WINDOW callbacks
    WINDOW = nullptr;

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
                WINDOW = new startupWindow();
                WINDOW->show();
                break;
            }else if(dato == "-dbg"){
                // The debug messages will be redirected to the console
                break;
            }else if(dato == "-log"){
                logfd = new QFile("C:/OEM/Logs/LOGSERVICE/GENERATOR/gantrygen.log");
                if (logfd->open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Append)){
                    // The debug messages will be redirected to the console
                    qInstallMessageHandler(dbgMessageOutputToFile);
                }
                break;
            }
        }
    }

    // Create the StatusManager class, responsible to handle the workflow
    STATUS = new workflowManager();
    EXPOSURE = new exposureManager();

    // Open the server for the external commands
    pServer = new Server("127.0.0.1", 10002);    
    pServer->Start();

    return a.exec();
}
