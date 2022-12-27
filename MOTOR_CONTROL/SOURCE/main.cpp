#define MAIN_CPP

#include "applog.h"
#include "application.h"


/**
 * @brief qMain
 * This is the Application Main function.
 *
 *
 * @param argc
 * @param argv
 *  - -file: the debug messages are redirect to the gantrygen.log file;
 *  - -win: the application run with the debug Window;
 *  - -console: the debug messages are forwarded to the console
 *  - No Options: no debug messages are handled
 * @return
 *
 * \ingroup   applicationModule
 */
int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    appLog(argc, argv, "C:/OEM/Logs/GANTRY/Motors.log", debugWindow::debugMessageHandler);

    // Initialize the WINDOW to nullptr so that in case of Operating mode
    // the application skips the WINDOW callbacks
    WINDOW = nullptr;
    TRX = nullptr;
    ARM = nullptr;
    LIFT = nullptr;
    BODY = nullptr;
    SLIDE = nullptr;


    // Create the Window Log if necessary
    if(appLog::isWindow){
        WINDOW = new debugWindow();
        WINDOW->show();
    }

    // Open the server for the external commands
    INTERFACE = new Interface(Application::IP_INTERFACE_ADDRESS, Application::INTERFACE_PORT);
    INTERFACE->Start();

    TRX = new trxModule();

    return a.exec();
}

