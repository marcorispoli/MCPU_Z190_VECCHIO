#include "device.h"


deviceClass::deviceClass(uint id, uchar revMaj, uchar revMin, uchar revSub, QObject *parent)
{
    ID = id;
    revisionreg = dataregT(revMaj,revMin,revSub,0);

}

