#ifndef STARTUPWINDOW_H
#define STARTUPWINDOW_H


#include <QWidget>



QT_BEGIN_NAMESPACE
namespace Ui { class startupWindow; }
QT_END_NAMESPACE


class startupWindow : public QWidget
{
    Q_OBJECT

public:
    startupWindow( QWidget *parent = nullptr);
    ~startupWindow();

    static void debugMessageHandler(QtMsgType type, QString msg); //!< Handles the debug messages in case of -win Application
    inline static startupWindow* instance = nullptr; //!< Assigned to the unique instance of the class

    void EventStatus(void);
    void EventMessage(ushort seq,QString msg);    
    void EventSetXrayEna(ushort seq, bool state);
    void EventXrayCompleted(ushort seq, uchar code, uchar error);    
    void EventGetPulseData(void);


// WINDOWS MODULE VIRTUAL FUNCTIONS
protected:
    virtual void initWindow(void); //!< Override function for the opening activities
    virtual void exitWindow(void); //!< Override function for the exiting activities


signals:


// GUI MODULE SLOTS
public slots:

    void onLogClearButton(void);
    void onDebugClearButton(void);

    void onLogRxSlot(QByteArray);

    void onStart2DPre(void);
    void onStart2DPulse(void);
    void onStart3DPre(void);
    void onStart3DPulse(void);
    void onStart2DTest(void);

    void onAbortRx(void);
    void onAecSendButton(void);




    void timerEvent(QTimerEvent* ev);

private slots:
    void on_logEnableCheck_stateChanged(int arg1);

private:
   Ui::startupWindow *ui;

   QByteArray pollingFrame;
   int pollingTimer;
   int polling;

    ulong generalDebugIndex;

};


#endif // STARTUPWINDOW_H
