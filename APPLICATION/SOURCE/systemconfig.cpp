#include "systemconfig.h"
systemConfig * systemConfig::pConfig = nullptr;

systemConfig::systemConfig()
{
    systemConfig::pConfig = this;
}
