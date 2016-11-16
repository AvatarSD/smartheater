
#include <DallasTemperature.h>
#include <hwiface.h>
#include <conf.h>

#include <stdlib.h>



#include <server.h>



#define REQUIRED_TEMP 20


void indicateAll(uint16_t deviceCount, const float & temp);
void indicateCount(int16_t count);


int main()
{
    sei();

    I2CSlaveServer * server = I2CSlaveServer::getInstance();
    server->setup();
    while(1);

    /*OneWire wire(ONEWIREPIN);
    DallasTemperature sensors(&wire);
    while(1)
        {
            HWiface::turnOneWireLineOn();
            sensors.begin();
            sensors.requestTemperatures();
            uint16_t deviceCount = sensors.getDeviceCount();
            float tempAvg = 0;
            for(uint16_t i = 0; i<deviceCount; i++)
                tempAvg += sensors.getTempCByIndex(i);
            tempAvg /= deviceCount;
            if((tempAvg < REQUIRED_TEMP)&&(deviceCount > 0)) HWiface::turnHeaterOn();
            else HWiface::turnHeaterOff();
            indicateAll(deviceCount, tempAvg);
        }*/
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
    indicateCount((int16_t)temp % 20);
    indicateCount((int16_t)(temp * 10) % 10);
    _delay_ms(1500);
}

void indicateCount(int16_t count)
{
    count = abs(count);
    HWiface::turnOneWireLineOff();
    _delay_ms(1000);

    for(; 0 < count; count--) {
        HWiface::turnOneWireLineOn();
        _delay_ms(250);
        HWiface::turnOneWireLineOff();
        _delay_ms(250);
    }
}
