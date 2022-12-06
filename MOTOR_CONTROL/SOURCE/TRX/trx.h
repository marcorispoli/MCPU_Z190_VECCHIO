#ifndef TRX_H
#define TRX_H

/*!
 * \defgroup  trxModule TRX communication protocol module.
 *
 * This Module implements thecommunication with the TRX.
 *
 *
 */

#include <QtCore>
#include "pd4.h"


class trxModule: public pd4Nanotec
{
    Q_OBJECT

public:
    explicit trxModule();
    ~trxModule(){};


signals:


public slots:

  ushort faultCallback(void); //!< Override Fault procedures

private slots:

public:

protected:


private:


};



#endif // TRX_H
