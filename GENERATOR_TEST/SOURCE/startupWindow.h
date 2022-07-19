#ifndef STARTUPWINDOW_H
#define STARTUPWINDOW_H

#include <QWidget>
#include "windows.h"
#include "communication.h"

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
    void connectionButtonSlot(void);
    void onEventConnectionButtonSlot(void);

    void timerEvent(QTimerEvent* ev);
    void commandCurrentTextChanged(QString);

    void  onPollingButton(void);
    void  onClearButton(void);
    void  onSendButton(void);    
    void addLogEvent(QByteArray data);

private:
   Ui::startupWindow *ui;

   QByteArray pollingFrame;
   int pollingTimer;

    int timeEv;

};


#endif // STARTUPWINDOW_H
