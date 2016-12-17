#ifndef INDICATION_H
#define INDICATION_H

#include <inttypes.h>


class HWiface;

class Indication
{
public:
    Indication(HWiface * hardware);
    void indicateAll(uint16_t deviceCount, const float & temperature);
    void indicateCount(uint16_t count);

private:
    HWiface * hardware;

};


#endif // INDICATION_H
