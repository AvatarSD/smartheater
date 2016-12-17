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
    delayMs(1200);
}

void Indication::indicateCount(uint16_t count)
{
    hardware->turnOneWireLineOff();
    delayMs(1000);

    for(; 0 < count; count--) {
        hardware->turnOneWireLineOn();
        delayMs(250);
        hardware->turnOneWireLineOff();
        delayMs(250);
    }
}

void Indication::delayMs(uint16_t ms)
{
    for(; ms > 0; ms--)
        _delay_ms(1);
}
