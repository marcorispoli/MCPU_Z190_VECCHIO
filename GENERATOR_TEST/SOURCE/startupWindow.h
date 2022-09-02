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


    void connectionStat(bool stat);

// WINDOWS MODULE VIRTUAL FUNCTIONS
protected:
    virtual void initWindow(void); //!< Override function for the opening activities
    virtual void exitWindow(void); //!< Override function for the exiting activities


signals:


// GUI MODULE SLOTS
public slots:
    void onLogUpdateButton(void);
    void onLogClearButton(void);
    void onLogRxSlot(QByteArray);


    void connectionButtonSlot(void);
    void onGetSystemMessagesButtonSlot(void);
    void onClearSystemMessagesButtonSlot(void);

    void onGetProtocolButtonSlot(void);
    void onSetProtocolVersion6ButtonSlot(void);
    void onSetProtocolVersion5ButtonSlot(void);

    void onSelectManualButtonSlot(void);
    void onSelectAecButtonSlot(void);
    void onSelectTomoButtonSlot(void);
    void onSelectAecTomoButtonSlot(void);

    void onAECButtonSlot(void);


    void onRecetionGenStatusSlot(void);



    void timerEvent(QTimerEvent* ev);
    void setStatus(QString stringa);
    void updateSystemMessages(QString stringa);


private slots:
    void on_logEnableCheck_stateChanged(int arg1);

private:
   Ui::startupWindow *ui;

   QByteArray pollingFrame;
   int pollingTimer;
   int polling;



};


#endif // STARTUPWINDOW_H
