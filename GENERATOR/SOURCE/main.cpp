#define MAIN_CPP

#include "application.h"
#include "applog.h"
#include <QFile>



/**
 * @brief qMain
 * This is the Application Main function.
 *
 *
 * @param argc
 * @param argv
 *  - -file: the debug messages are redirect to the Generator.log file;
 *  - -win: the application run with the debug Window;
 *  - No parameter for the Operating mode (no debug messages);
 * @return
 *
 * \ingroup   applicationModule
 */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    appLog(argc, argv, "C:/OEM/Logs/GANTRY/Generator.log", startupWindow::debugMessageHandler);

    // Initialize the WINDOW to nullptr so that in case of Operating mode
    // the application skips the WINDOW callbacks
    WINDOW = nullptr;
    // Create the Window Log if necessary
    if(appLog::isWindow){
        WINDOW = new startupWindow();
        WINDOW->show();
    }

    // Create the StatusManager class, responsible to handle the workflow
    STATUS = new workflowManager();
    EXPOSURE = new exposureManager();

    // Open the server for the external commands
    INTERFACE = new Interface("127.0.0.1", 10002);
    INTERFACE->Start();

    return a.exec();
}
