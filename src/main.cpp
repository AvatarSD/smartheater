#include <DallasTemperature.h>
#include <hwiface.h>
#include <conf.h>
#include <server.h>
#include <settings.h>
#include <indication.h>


void searchSensors()
{


}


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
        for(uint16_t i = 0; i < deviceCount; i++) {


            DeviceAddress deviceAddress;
            if(!sensors.getAddress(deviceAddress, i)) {
                settingsinternal::setSensorStatus(i, SensorStatusEnum::NoResponse);
                continue;
            }

            settingsinternal::setSensorStatus(i, SensorStatusEnum::Active);
            settingsinternal::setSensorRom(i, deviceAddress);
            float tempTmp = sensors.getTempC((uint8_t *)deviceAddress);





            //float tempTmp = sensors.getTempCByIndex(i);
            tempAvg += tempTmp;
            settingsinternal::setSensorTemp(i, tempTmp);
        }
        tempAvg /= deviceCount;
        settingsinternal::setTempAvg(tempAvg);

        if((tempAvg < settingsinternal::getRequiredTemp()) &&
                (deviceCount > 0)) HWiface::turnHeaterOn();
        else HWiface::turnHeaterOff();

        indicateAll(deviceCount, tempAvg);
    }

    return 0;
}
