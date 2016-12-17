#include "indication.h"

#include <util/delay.h>
#include <hwiface.h>

Indication::Indication(HWiface * hardware) :
    hardware(hardware)
{
}

void Indication::indicateAll(uint16_t deviceCount, const float & temperature)
{
    uint16_t temp = (temperature + 100) * 10;
    indicateCount(deviceCount);
    indicateCount((temp / 10) % 20);
    indicateCount(temp % 10);
    _delay_ms(1200);
}

void Indication::indicateCount(uint16_t count)
{
    hardware->turnOneWireLineOff();
    _delay_ms(1000);

    for(; 0 < count; count--) {
        hardware->turnOneWireLineOn();
        _delay_ms(250);
        hardware->turnOneWireLineOff();
        _delay_ms(250);
    }
}
