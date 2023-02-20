#ifndef DEVICE_PROTOCOL_H
#define DEVICE_PROTOCOL_H

#include "QObject"
#include "can_device_protocol.h"

/*!
 * \defgroup  deviceModule Device Protocol implementation
 *
 *
 */


class deviceProtocol: public canDeviceProtocol
{
    Q_OBJECT

public:
    explicit deviceProtocol();
    ~deviceProtocol(){};


    typedef enum{
        _WORKFLOW_INITIALIZATION = 0,
        _WORKFLOW_GET_REVISION,
        _WORKFLOW_IDLE,
    }PROTOCOL_WORKFLOW_e;

    typedef enum{
        STATUS_SYSTEM = 0,
        STATUS_BATTERY,
        STATUS_LEN
    }PROTOCOL_STATUS_ENUM_e;

    typedef enum{
        DATA_OUTPUTS = 0,
        DATA_LEN
    }PROTOCOL_DATA_ENUM_e;


signals:


public slots:


private slots:

    void workflowINITIALIZATION(void);

public:
    uchar cmdTest;
    uchar cmdParam[4];
    bool abortCmd;

protected:



private:
    PROTOCOL_WORKFLOW_e workflow;
    uchar subWorkflow;
    uchar sequence;
    bool  rxOk;



};
#endif // DEVICE_PROTOCOL_H
