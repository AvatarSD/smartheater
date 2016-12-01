#ifndef CORESTATE_H
#define CORESTATE_H

#include <inttypes.h>

class ICoreState
{
public:

    virtual void setI2cAddress(uint8_t addr);
    virtual void setDeviceMode(uint8_t status);
};

#endif // CORESTATE_H
