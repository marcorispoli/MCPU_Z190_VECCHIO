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

    void initWindow(void); //!< Override function for the opening activities
    void exitWindow(void); //!< Override function for the exiting activities
    void setConnection(bool);

public slots:

    void timerEvent(QTimerEvent* ev);



private:
   Ui::startupWindow *ui;
   int timeEv;


public:
   bool getEmergencyStat(void);
   bool getPowerdownStat(void);
   bool getMotorSafetySwitchStat(void);
   bool getBatteryEnableStat(void);
   bool getCArmUpStat(void);
   bool getCArmDwnStat(void);
   bool getTiltUpStat(void);
   bool getTiltDwnStat(void);
   bool getRotCwStat(void);
   bool getRotCCwStat(void);
   bool getBodyCwStat(void);
   bool getBodyCCwStat(void);

   bool getCompressorOnStat(void);
   bool getXRayReqStat(void);
   bool getBurninStat(void);
   bool getCloseDoorStat(void);
   bool getPedalCarmUpStat(void);
   bool getPedalCarmDwnStat(void);
   bool getPedalCmpUpStat(void);
   bool getPedalCmpDwnStat(void);


private slots:
    void onStartButtonPressed(void);
};


#endif // STARTUPWINDOW_H
