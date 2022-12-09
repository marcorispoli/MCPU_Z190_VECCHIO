#ifndef STARTUPWINDOW_H
#define STARTUPWINDOW_H


#include <QWidget>



QT_BEGIN_NAMESPACE
namespace Ui { class debugWindow; }
QT_END_NAMESPACE


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
    void onCanReady(bool stat);  //!< Connect the signal canReady from the canClient

    void onDebugClearButton(void);
    void onDebug(QByteArray data);

    void onZeroSettingButton(void);
    void onPositionA(void);
    void onPositionB(void);
    void onStopCommand(void);

    void onLogClearButton(void);


    void timerEvent(QTimerEvent* ev);

private slots:
    void on_logEnableCheck_stateChanged(int arg1);

private:
   Ui::debugWindow *ui;

   QByteArray pollingFrame;
   int pollingTimer;
   int polling;

    ulong generalDebugIndex;

};


#endif // STARTUPWINDOW_H
