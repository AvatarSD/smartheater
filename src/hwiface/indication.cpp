#include "indication.h"

#include <util/delay.h>
#include <hwiface.h>

void indicateAll(uint16_t deviceCount, const float & temperature)
{
    uint16_t temp = (temperature + 100) * 10;
    indicateCount(deviceCount);
    indicateCount((temp / 10) % 20);
    indicateCount(temp % 10);
    _delay_ms(1200);
}

void indicateCount(uint16_t count)
{
    HWiface::turnOneWireLineOff();
    _delay_ms(1000);

    for(; 0 < count; count--) {
        HWiface::turnOneWireLineOn();
        _delay_ms(250);
        HWiface::turnOneWireLineOff();
        _delay_ms(250);
    }
}
