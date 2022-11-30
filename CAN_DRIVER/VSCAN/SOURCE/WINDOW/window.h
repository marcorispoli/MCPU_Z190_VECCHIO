#ifndef WINDOW_H
#define WINDOW_H

/*!
 * \defgroup  windowModule Debug Window Module.
 *
 * This Module implements an optional debug GUI.
 *
 * When the application is launched with the -win option,\n
 * the application generates a GUI Window interface in order to:
 *
 * - Logs the CAN frames data traffic;
 * - Logs the internal debug strings;
 * - Provides Debug functions to interact with the BUS actors.
 *
 *
 */

#include <QWidget>



QT_BEGIN_NAMESPACE
namespace Ui { class debugWindow; }
QT_END_NAMESPACE

/**
 * @brief The debugWindow class
 *
 * \ingroup windowModule
 */
class debugWindow : public QWidget
{
    Q_OBJECT

public:
    debugWindow( QWidget *parent = nullptr);
    ~debugWindow();



// WINDOWS MODULE VIRTUAL FUNCTIONS
protected:
    virtual void initWindow(void); //!< Override function for the opening activities
    virtual void exitWindow(void); //!< Override function for the exiting activities


signals:


// GUI MODULE SLOTS
public slots:

    void onLogClearButton(void);
    void onDebugClearButton(void);
    void receivedCanFrame(ushort canId, QByteArray data);
    void sendToCan(ushort canId, QByteArray data );

    void onDebug(QByteArray data);
    void timerEvent(QTimerEvent* ev);
    void on_logEnableCheck_stateChanged(int arg1);
    void on_logEnableEthCheck_stateChanged(int arg1);


private:
   Ui::debugWindow *ui;

   QByteArray pollingFrame;
   int pollingTimer;
   int polling;
   ulong generalDebugIndex;

};


#endif // STARTUPWINDOW_H
