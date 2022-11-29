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


    void EventStatus(void);
    void EventMessage(ushort seq,QString msg);    
    void EventSetXrayEna(ushort seq, bool state);
    void EventXrayCompleted(ushort seq, uchar code, uchar error);    
    void EventGetPulseData(ushort seq);


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

    void onDebug(QByteArray data);


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
