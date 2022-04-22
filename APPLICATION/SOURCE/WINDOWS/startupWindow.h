#ifndef STARTUPWINDOW_H
#define STARTUPWINDOW_H

#include <QWidget>
#include "windows.h"

QT_BEGIN_NAMESPACE
namespace Ui { class startupWindow; }
QT_END_NAMESPACE

class startupWindow : public windowsClass
{
    Q_OBJECT

public:
    startupWindow(int W, int H, QWidget *parent = nullptr);
    ~startupWindow();

// WINDOWS MODULE VIRTUAL FUNCTIONS
protected:
    virtual void initWindow(void); //!< Override function for the opening activities
    virtual void exitWindow(void); //!< Override function for the exiting activities


// GUI MODULE SLOTS
public slots:
    void onPushButton(void);
    void onErr1PushButton(void);
    void onErr2PushButton(void);
    void onErr3PushButton(void);
    void onOpenErrWindowPushButton(void);
    void onExitPushButton(void);

private:
    Ui::startupWindow *ui;
    void timerEvent(QTimerEvent* ev);

    int timeEv;
    int angolo;

    float WS,HS;

};


#endif // STARTUPWINDOW_H
