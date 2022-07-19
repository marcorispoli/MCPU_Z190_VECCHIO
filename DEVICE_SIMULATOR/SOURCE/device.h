#ifndef DEVICE_H
#define DEVICE_H

#include <QObject>
#include "can_protocol.h"
#include "can_client.h"



class deviceClass: public QObject
{
    Q_OBJECT

public:
    explicit deviceClass(ushort canId,  QObject *parent = nullptr);

signals:
    void txToCanDriver(QByteArray);

public slots:
    void rxFromCanDriver(QByteArray);
    void clientConfigured(bool stat){canReady(stat);};

    virtual void canReady(bool ){};

public:    
    canClient* can; // Pointer to the Can Device Driver
    canProtocol* protocol;

protected:
    void StartClient(void);

private:
    ushort canId;
};

#endif // DEVICE_H
