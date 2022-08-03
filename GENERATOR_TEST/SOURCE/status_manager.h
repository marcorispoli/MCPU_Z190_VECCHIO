#ifndef STATUS_MANAGER_H
    #define STATUS_MANAGER_H

#include <QObject>



class statusManager : public QObject
{
    Q_OBJECT

public:
    explicit statusManager(QObject *parent = nullptr);


signals:


public slots:


public:

    typedef enum{
      SMS_SMART_HUB_CONNECTION = 0,
      SMS_WAIT_GENERATOR_CONNECTION,
      SMS_SERVICE,
      SMS_IDLE,
      SMS_MESSAGE_NOTIFY,
      SMS_EXECUTING_2D_MANUAL,
      SMS_EXECUTING_2D_AEC,
      SMS_EXECUTING_3D_MANUAL,
      SMS_EXECUTING_3D_AEC,
      SMS_EXECUTING_AE_MANUAL,
      SMS_EXECUTING_AE_AEC,
    }_statusManagerState;

private slots:
    void handleCurrentStatus(void);

private:
    _statusManagerState internalState;
    uchar subStatus;

    void handle_SMS_MESSAGE_NOTIFY(void);
    void handle_SMS_IDLE(void);
};

#endif // STATUS_MANAGER_H
