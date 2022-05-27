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
    void setLanguage(QString tag);

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

    bool rotBusy(void);
    bool moveTrx(int angolo, uint speed, QString message = "TRX IS MOVING .." );
    bool moveArm(int angolo, uint speed);
    bool isArmEnabled(void);

    bool selectProjection(QString, int , int);
    void setProjectionList(QList<QString> lista);

    bool selectTomoConfigFile(QString file);
    void selectTomoConfig(QString file, QString ID);
    int getTomoHome(QString ID);
    int getTomoFinal(QString ID);
    int getTomoRun(QString ID);
    int getTomoSamples(QString ID);

    void setMessage(QString msg, uint time);

    QString getTomoCfgFile(void);
    QString getTomoCfgId(void);

    void setCompressionReleaseMode(bool status);
    QString getCompressionReleaseMode(void);
    void setXrayEnable(bool stat);
    QMap<QString, QList<tomoConfig>> tomoCfg;

    bool getXrayPush(void);
    bool getXrayReady(void){return readyForExposure;}

    int setXrayExposureMode(QString CODE, QString COMP, QString COLL, QString TOMO_ID);
    int setXraySequenceData(QString kV, QString mAs, QString Filter);
    int setXrayPulseData(QString kV, QString mAs, QString Filter, QString TomoId);

    void startXray(void);
    int getExposureCompletionData(QString option, uint pulse, QList<QString>* params);

private:
    void setTubeData(int Anode, int Housing);
    void clearXrayButton(void);

private slots:
    void moveTrxCompleted(void);
    void moveArmCompleted(void);
    void bindSlot(void);
    void exportButtonSlot(void);
    void clearLogButtonSlot(void);
    void exitButtonSlot(void);


// WINDOWS MODULE VIRTUAL FUNCTIONS
protected:
    virtual void initWindow(void); //!< Override function for the opening activities
    virtual void exitWindow(void); //!< Override function for the exiting activities


signals:
    void xrayPushButtonSgn(bool stat);
    void trxCompletedSgn(void);
    void armCompletedSgn(void);

// GUI MODULE SLOTS
public slots:

    void timerEvent(QTimerEvent* ev);

    void tomoIdChangedSlot(QString);
    void tomoListChangedSlot(QString file);
    void projectionSelectionSlot(QString proj);

    void accessoriesSlot(QString proj);
    void breastSlot(int);


    void messageOff(void);
    void emitXrayPushButtonPressed(void);
    void emitXrayPushButtonReleased(void);
    void generateError(uint num);
    void generateErrorSlot(void);
    void cancelError(void);

    int checkReadyForExposure(void);

    void manual2DSequence(void);
    void Aec2DSequence(void);
    void manual3DSequence(void);
    void Aec3DSequence(void);
    void manualComboSequence(void);
    void AecComboSequence(void);
    void manualAESequence(void);
    void AecAESequence(void);
    void seqError(uint num);
    void sequenceCompleted(void);

private:
   Ui::startupWindow *ui;
    int timeEv;

    int angolo;
    int targetTRX;
    bool trxBusy;
    bool moveTrxToHome(QString id);
    uint moveTrxToFinal(QString id);
    void enableArm(void);

    int  targetARM;
    bool armBusy;

    float WS,HS;
    bool changeEvent;
    bool xrayPushStat;

    QString selectedProjection;

    // Temp Tube
    int tubeHu;
    int tubeTemp;

    QString errorCondition;

    bool readyForExposure;


    // Xray Seqeunce data
    QList<QString> XrayCodeList;
    QList<QString> FilterList;
    QList<QString> ColliList;


    // Dati di esposizione prossimo impulso
    QString XraySequenceCode;
    QString Collimation;
    QString Compression;
    QString TomoId;

    float kV;
    uint mAs;
    QString Filter;

    bool sequenceStarted;
    uint expSeq;
    uint numPulse;
    uint percPulse;
    bool pulseDataReady;

    // Exposizioni completate
    float kVFinal[4];
    uint mAsFinal[4];
    QString FilterFinal[4];
    QString exposureResult;
    uint exposureError;
    uint tomoNsamplesFinal;
    uint tomoTimeSample;


    int timerTube;
    uint tubeDeltaJouls ;
    uint tubeJouls ;


};


#endif // STARTUPWINDOW_H
