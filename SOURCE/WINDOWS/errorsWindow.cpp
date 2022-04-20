#include "errorsWindow.h"
#include "ui_errorsWindow.h"




// Assignes the instance pointer to the static class pointer
// to be easier in the software to pint to the window
errorsWindow * errorsWindow::window = nullptr;
bool errorsWindow::errorWindowEna = false; //! < Disabled as default

/**
 * This is the Class constructor
 *
 * The error messages structure is created and the messages are translated
 * in the current system language;
 *
 * The static errorsWindow::window pointer is assigne to the class instance
 * as a static member so that the software can point to a static member instead of
 * a instance pointer.
 *
 * @param W width of the target screen
 * @param H height of the target screen
 * @param parent optionnal parent widget
 */
errorsWindow::errorsWindow(int W, int H, QWidget *parent) : windowsClass(W, H, parent),
    ui(new Ui::errorsWindow)
{
    ui->setupUi(this);

    // The first instance of the class assign the class static pointer
    errorsWindow::window = this;

    // Timer initialization
    timerWindow = 0;

    // Connect the GUI buttons
    connect(ui->scrollPushButton,SIGNAL(released()),this,SLOT(onScrollPushButton()),Qt::UniqueConnection);
    connect(ui->exitPushButton,SIGNAL(released()),this,SLOT(onExitPushButton()),Qt::UniqueConnection);

}

errorsWindow::~errorsWindow()
{
    delete ui;
}

/**
 * This is the overriden callback called when the window is open.
 *
 * The internal timer to auto close the window is initialized to
 * the target timeout: errorsWindow::_WINERR_TIMEOUT
 *
 * The content of the GUI is filled with the topItem error element
 */
void errorsWindow::initWindow(void)
{
    // Init the timer to close the window
    timerWindow = startTimer(10000);

    // Setup page properly
    setupErrorWindow();

}

/**
 * This is the exit window callback called from the windowsClass super member;
 *
 * The callback:
 * + Stops the Timer;
 * + Erase all the auto-expiring errors;
 * + Update the topItem error element;
 *
 */
void errorsWindow::exitWindow(void)
{
    // Stop the timer
    if(timerWindow) killTimer(timerWindow);
    timerWindow = 0;


    /*
    // If the TopItem is without errors, no other actions..

   if(!activeError) return;

    // Clears the error that auto expires
    QMap<QString, errorBlock>::iterator i;
    for (i = errorList.begin(); i != errorList.end(); ++i)
        if(i->isAutoExpire()) i->setActive(false);

    // Find the new top error in the list
    if(!errorPointer->isActive()){
        activeError = false;
        getTopError();
    }
*/

}

/**
 * This is the Timer callback when the timer expires
 *
 * The callback close the current window, opening the parent window.
 *
 * @param ev QTimerEvent passed by Qt
 */
void errorsWindow::timerEvent(QTimerEvent* ev)
{
    if(ev->timerId() == timerWindow) this->openParentWindow();

}

/**
 * This function initializes all the internal Widget
 * related to the error present in the topItem.
 *
 * In case the number of active errors exceeds 2
 * the Scroll button show up.
 */
void errorsWindow::setupErrorWindow(void)
{
    /*
    // No error condition is present
    if(activeError == false){
        ui->errorString->setText("[-----]");
        ui->titleString->setText("NO ERROR");
        ui->descriptionString->setPlainText("???");
        return;
    }

    // Shows the error codition in the top of the list
    ui->errorString->setText("[" + errorPointer.value().getCode() + "]");
    ui->titleString->setText(errorPointer.value().getTitle());
    ui->descriptionString->setPlainText(errorPointer.value().getDescription());
    QString stringa = "border-image: url(:" + errorPointer.value().getPixmapUID() + ");" ;
    ui->pixFrame->setStyleSheet(stringa);
    ui->pixFrame->show();

    // Verify if more than one error is present to show the scroll button
    int n = 0;
    QMap<QString, errorBlock>::iterator i;
    for (i = errorList.begin(); i != errorList.end(); ++i){
        if(i.value().isActive()) n++;
        if(n==2) break;
    }
    if(n==2) ui->scrollPushButton->show();
    else ui->scrollPushButton->hide();

    return;
*/
}

/**
 * This function is called when the user press the scroll error button.
 *
 * The Scroll button is enabled (and showed) only when more than one error
 * are active at the same time.
 *
 * The function steps forward the active code errors, starting from the topItem.
 */
void errorsWindow::onScrollPushButton(void){

    /*
    // No errors always tested
    if(!activeError) return;

    QMap<QString, errorBlock>::iterator i = errorPointer;
    i++;

    while(i != errorPointer){
        if(i.value().isActive()){
            errorPointer = i;
            break;
        }

        i++;
        if(i == errorList.end()) i = errorList.begin();
    }

    setupErrorWindow();
*/
}

/**
 * This function is called when the user press the exit button.
 *
 * The Exit button provides the operator to close the error window
 * without waiting the natural window close after a timeout
 *
 */
void errorsWindow::onExitPushButton(void){
    errorsWindow::closeErrorWindow();
}


/**
 * This method open the error window only if there is almost
 * one valid error code active.
 *
 * The window will open with the windowsClass::_WINDOW_POSTPONE flag.
 */
void errorsWindow::internalOpenErrorWindow(void){

    /*
    // No error are present
    if(topItem == _ErrorTable::ERROR_LAST) return;

    // Open the current window only if enabled and with POSTPONE flag
    if(errorsWindow::errorWindowEna) this->openWindow();
    */
}
