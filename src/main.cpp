
#include <DallasTemperature.h>
#include <hwiface.h>
#include <conf.h>
#include <server.h>
#include <settings.h>


void indicateAll(uint16_t deviceCount, const float & temp);
void indicateCount(uint16_t count);


int main()
{
    HWiface::init();

    sei();

    I2CSlaveServer * server = I2CSlaveServer::getInstance();
    server->setup();

    OneWire wire(ONEWIREPIN);
    DallasTemperature sensors(&wire);

    while(1) {
        HWiface::turnOneWireLineOn();
        sensors.begin();
        sensors.requestTemperatures();
        uint16_t deviceCount = sensors.getDeviceCount();
        settingsinternal::setDeviceCount(deviceCount);

        float tempAvg = 0;
        for(uint16_t i = 0; i < deviceCount; i++)
            tempAvg += sensors.getTempCByIndex(i);
        tempAvg /= deviceCount;
        settingsinternal::setTempAvg(tempAvg);

        if((tempAvg < settingsinternal::getRequiredTemp()) &&
                (deviceCount > 0)) HWiface::turnHeaterOn();
        else HWiface::turnHeaterOff();

        indicateAll(deviceCount, tempAvg);
    }
    /* while(1)
    {
        HWiface::turnHeaterOn();
        _delay_ms(1000);
        HWiface::turnHeaterOff();
        _delay_ms(500);
        HWiface::turnOneWireLineOn();
        _delay_ms(1000);
        HWiface::turnOneWireLineOff();
        _delay_ms(500);
    }*/
    /* while(1)
    {
        HWiface::turnOneWireLineOn();
        sensors.begin();
        indicateCount(sensors.getDeviceCount());
        _delay_ms(2000);
    }*/
    /* while(1)
    {
        HWiface::turnOneWireLineOn();
        sensors.begin();

        sensors.requestTemperatures();
        auto deviceCount = sensors.getDeviceCount();

        float tempAvg = 0;
        for(auto i = 0; i<deviceCount; i++)
            tempAvg += sensors.getTempCByIndex(i);
        tempAvg /= deviceCount;

        if(((int)tempAvg)%2) HWiface::turnHeaterOn();
        else HWiface::turnHeaterOff();
    }*/
    return 0;
}

void indicateAll(uint16_t deviceCount, const float & temp)
{
    indicateCount(deviceCount);
    indicateCount((int16_t)temp % 20); // todo fix temp indication
    indicateCount((int16_t)(temp * 10) % 10);
    _delay_ms(1500);
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
