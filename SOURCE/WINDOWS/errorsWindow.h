#ifndef ERRORSWINDOW_H
#define ERRORSWINDOW_H

#include <QWidget>
#include "windows.h"
#include "errorTable.h"

namespace Ui {
class errorsWindow;
}

/**
 * This is the main window class implementing the system error management.
 *
 * # ABSTRACT
 *
 * The system shall handle two main error types:
 * + Static errors: errors that remain alive until a proper command clears it;
 * + Temporary errors: errors that automatically expires as soon as they are signaled;
 *
 * Every error shall be uniquely identified with a string code composed by five
 * numeric characters preposed with the character 'E': ex E00001.
 * This code is documented in the Manual of the application.
 * Although the error code is unique, there could be several conditions that
 * cause the same error code.
 *
 * The error content is a string that is translated with the QtLanguage external tool.
 * The translation string (inthe QtLanguage translator tool) shall be formatted as follows:
 *
 *  \verbatim
   "Title string\n"
   "Description string ....."
   \endverbatim
 *
 *  The "Title String" shall be the first (optionally unique) line of the translaton.
 *  This string will be visualized into the Error GUI as a separate string
 *  representing the Title of the error: usually it is the Class of the error;
 *
 *  The "Description String" shall descrives the error. It can be composed of several lines.
 *  This field can be optional if the Title should be enough descriptive.
 *
 * The error code can optionally  be associated with a Pixmap:
 * in this case the Pixmap shall be loaded in the assets file into the
 * /errorPixmap directory.
 *
 * # ERROR WINDOW FEATURES
 *
 * This class is derived from the windowsClass inheriting
 * the window management system.
 *
 * This class handles the error activation and visualization,
 * providing the following feature:
 *
 * + A GUI interface showing the content of an error message activated;
 * + A self resetting error activation option;
 * + Multi error visualization and scrolling;
 * + Non interference with the activities of the operating windows;
 *
 * When a given error is activated, the Error Window
 * will show up for a predefined amount of seconds errorsWindow::_WINERR_TIMEOUT.
 * After the timer expires the Window will close:
 * + all the temporary errors (auto-expire errors) will be cleared when the error window closes;
 * + all the non temporary error will remain active.
 *
 * In case more than one error is active, a SCROLL button
 * appear in the error GUI providing a method to scroll all
 * the active alarm.
 * NOTE: only one active alarm at a time is showed in the GUI.
 *
 * The error window can be closed early either with a GUI push button
 * or by mean of a dedicated public method.
 *
 * In order to prevent interferences with other windows in the system
 * the following rules apply:
 * 1) The error handling commands will postpone the openWindow events;
 * 2) The parent window callback initWindow() and exitWindow() are not called;
 *
 * # USAGE
 * ## Define the internal ID code to identify a given error
 * Add the error label into the typedef enum _ErrorTable in erorTable.h file:
 * \verbatim
   typedef enum{
        ERROR_1 = 0,
        ERROR_2,
        ERROR_3,
        .....
        Insert a progressive label here ...
        .....
        ERROR_LAST // This code shall be the last code
    } _ErrorTable;
  \endverbatim
 * ## Setup the message content
 * Add the definition of the error message into the errorsWindow::setupErrors() method
 * in the errorsWindow.cpp file:
 * \verbatim
   Example:
   errorList[_ErrorTable::ERROR_1] = *( new errorBlock("E00001",QApplication::translate("E00001","ERRORE 1 DESCR TAG"),"pixmap-name-err1"));
   errorList[_ErrorTable::ERROR_2] = *( new errorBlock("E00002",QApplication::translate("E00002","ERRORE 2 DESCR TAG"),"pixmap-name-err2"));
   errorList[_ErrorTable::ERROR_3] = *( new errorBlock("E00003",QApplication::translate("E00003","ERRORE 3 DESCR TAG"),"pixmap-name-err3"));
   where:
   _ErrorTable::ERROR_1 is the error label
   "E00001": is the string error code
   "ERRORE 1 DESCR TAG"  is the tag handled by the QtLanguage translator to translate the message
   "pixmap-name-err1" is the name of the pixmap of the error 1 in the example. Set "" if the error has no pixmap at all
  \endverbatim
 * # Error handling methods
 * + In order to activate an error use: errorsWindow::activateError();
 * + In order to clear an error use:  errorsWindow::clearError();
 * + In order to reopen the error window: errorsWindow::openErrorWindow();
 * + In order to early close the error window use: errorsWindow::closeErrorWindow();
 * + In order to disable the error window opening use: errorsWindow::enableErrorWindow();
 * + In order to check if any error is active use: errorsWindow::isAnyError();
 * + In order to check if a given error is active use: errorsWindow::isInError();
 *
 *
 */
class errorsWindow : public windowsClass
{
    Q_OBJECT

public:
    explicit errorsWindow(int W, int H, QWidget *parent = nullptr);
    ~errorsWindow();
    virtual void initWindow(void); //!< Overriden function for the opening activities
    virtual void exitWindow(void); //!< Overriden function for the exiting activities


    //! Call this method to re-open the error window
    /**
     * The error window will open only in case an error is active.
     *
     * The command is not istantly executed but it will be put into the Event Queue
     */
    static _inline void openErrorWindow(void){
        errorsWindow::window->internalOpenErrorWindow();
    }

    //! Call this method to force to close the error window
    /**
     * The error window will close and all the auto-expiring errors reset.
     *
     * The command is not istantly executed but it will be put into the Event Queue
     */
    static _inline void closeErrorWindow(void){
        errorsWindow::window->openParentWindow();
    }

    //! Call this static class method to enable the error window to open.
    static _inline  void enableErrorWindow(bool status){
        errorsWindow::errorWindowEna = status;
    }


private slots:
    void onScrollPushButton(void); //!< Action pressing the scroll button (when active)
    void onExitPushButton(void);   //!< Action pressing the exit button
    void timerEvent(QTimerEvent* ev);//! QObject Timer overridden method to handle the timer events

private:
    Ui::errorsWindow *ui;           //!< Pointer to the UI Widget content
    static errorsWindow * window;   //!< Pointer to the errorWindow instance
    static bool errorWindowEna;     //!< Enable/Disable open flag

    void internalOpenErrorWindow(void);     //!< Re-open the error Window (only if any error is pending    
    void setupErrorWindow(void);            //!< Initialize the widgets of the page
    int timerWindow; //!< Timer to close the window after a timeout

    #define _WINERR_TIMEOUT 10000 //!< Timeout in ms to close the error window

};

#endif // ERRORSWINDOW_H
