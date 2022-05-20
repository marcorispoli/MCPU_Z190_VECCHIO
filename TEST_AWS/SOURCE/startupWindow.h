#ifndef STARTUPWINDOW_H
#define STARTUPWINDOW_H

#include <QWidget>
#include "windows.h"

QT_BEGIN_NAMESPACE
namespace Ui { class startupWindow; }
QT_END_NAMESPACE


class tomoConfig{


public:
    QString id;
    QString home;
    QString final;
    QString acc;
    QString dec;
    QString run;
    QString sample;
    QString skeep;

    void set(QString ID, QString HOME, QString FINAL, QString ACC, QString DEC, QString RUN, QString SMP, QString SKP){
        id = ID;
        home = HOME;
        final = FINAL;
        acc = ACC;
        dec = DEC;
        run = RUN;
        sample = SMP;
        skeep = SKP;
    }
};

class startupWindow : public QWidget
{
    Q_OBJECT

public:
    startupWindow( QWidget *parent = nullptr);

    ~startupWindow();

    void addLogEvent(QString data);
    void setStudyName(QString data);
    int getTrx(void);
    int getArm(void);
    int getTilt(void);
    QString getPotter();
    QString getPaddle();
    QString getThick();
    QString getForce();
    QString getAccessory();

    QString getTubeTemp();
    QString getTubeHu();

    void moveTrx(int angolo);
    void moveArm(int angolo);
    void setTrx(int angolo);
    void setArm(int angolo);
    bool selectProjection(QString, int , int);
    void setProjectionList(QList<QString> lista);

    void selectTomoConfig(QString file, QString ID);
    bool selectTomoConfigFile(QString file);
    int getTomoHome(QString ID);
    int getTomoFinal(QString ID);
    int getTomoRun(QString ID);

    void setMessage(QString msg, uint time);

    QString getTomoCfgFile(void);
    QString getTomoCfgId(void);

    void setCompressionReleaseMode(bool status);
    QString getCompressionReleaseMode(void);
    void setXrayEnable(bool stat);
    QMap<QString, QList<tomoConfig>> tomoCfg;

    bool getXrayPush(void);

private:
    void setTubeData(int Anode, int Housing);


// WINDOWS MODULE VIRTUAL FUNCTIONS
protected:
    virtual void initWindow(void); //!< Override function for the opening activities
    virtual void exitWindow(void); //!< Override function for the exiting activities


signals:
    void xrayPushButtonSgn(bool stat);

// GUI MODULE SLOTS
public slots:
    void onPushButton(void);
    void timerEvent(QTimerEvent* ev);

    void tomoIdChangedSlot(QString);
    void tomoListChangedSlot(QString file);
    void projectionSelectionSlot(QString proj);

    void accessoriesSlot(QString proj);
    void breastSlot(int);


    void messageOff(void);
    void emitXrayPushButtonPressed(void);
    void emitXrayPushButtonReleased(void);
    void generateError(void);
    void cancelError(void);

    bool checkReadyForExposure(void);

private:
   Ui::startupWindow *ui;


    int timeEv;
    int angolo;
    float WS,HS;
    bool changeEvent;
    bool xrayPushStat;

    QString selectedProjection;

    // Temp Tube
    int tubeHu;
    int tubeTemp;

    QString errorCondition;

    bool readyForExposure;
};


#endif // STARTUPWINDOW_H
