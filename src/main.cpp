
#include <DallasTemperature.h>
#include <hwiface.h>
#include <conf.h>


int main()
{
    sei();
/*
    OneWire wire(ONEWIREPIN);
    DallasTemperature sensors(&wire);

    HWiface::turnOneWireLineOn();
    sensors.begin();

    while(1)
    {
        auto deviceCount = sensors.getDeviceCount();
        float tempAvg = 0;
        for(auto i = 0; i<deviceCount; i++)
            tempAvg += sensors.getTempCByIndex(i);
        tempAvg /= deviceCount;


        if(tempAvg < 20) HWiface::turnHeaterOn();
        else HWiface::turnHeaterOff();
    }*/

    while(1)
    {
        HWiface::turnHeaterOn();
        _delay_ms(1000);
        HWiface::turnHeaterOff();
        _delay_ms(500);
        HWiface::turnOneWireLineOn();
        _delay_ms(1000);
        HWiface::turnOneWireLineOff();
        _delay_ms(500);
    }

    return 0;
}
