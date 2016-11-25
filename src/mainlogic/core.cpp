#include "core.h"
#include <settings.h>
#include <indication.h>
#include <conf.h>
#include <hwiface.h>


CoreLogic::CoreLogic() : wire(ONEWIREPIN), sensors(&wire)
{

}

void CoreLogic::eraceeeprom()
{
    DeviceAddress nullRom = {0, 0, 0, 0,
                             0, 0, 0, 0
                            };
    for(uint8_t i = 0; i < MAX_SENSORS; i++) {
        settingsinternal::setSensorRom(i, nullRom);
        settingsinternal::setSensorStatus(i, SensorStatusEnum::NoAvailable);
        settingsinternal::setSensorTemp(i, 0);
    }
    settingsinternal::setDeviceCount(0);
}

void CoreLogic::searchSensors()
{
    HWiface::turnOneWireLineOn();
    sensors.begin();
    eraceeeprom();
    DeviceAddress deviceAddress;
    uint16_t deviceCounter = 0;
    wire.reset_search();
    while(wire.search(deviceAddress)) {
        if((sensors.validAddress(deviceAddress)) && (deviceCounter < MAX_SENSORS)) {
            settingsinternal::setSensorRom(deviceCounter, deviceAddress);
            settingsinternal::setSensorStatus(deviceCounter, SensorStatusEnum::Active);
            deviceCounter++;
        }
    }
    settingsinternal::setDeviceCount(deviceCounter);
}

void CoreLogic::mainCycle()
{
    HWiface::turnOneWireLineOn();
    sensors.begin();

    sensors.requestTemperatures();

    uint16_t deviceReaded = 0, deviceCount = settingsinternal::getDeviceCount();
    float tempAvg = 0;
    DeviceAddress deviceAddress;

    for(uint16_t i = 0; i < deviceCount; i++) {

        if(settingsinternal::getSensorStatus(i) == SensorStatusEnum::Disable)
            continue;

        settingsinternal::getSensorRom(i, deviceAddress);
        float tempTmp = sensors.getTempC((uint8_t *)deviceAddress);

        if(tempTmp == DEVICE_DISCONNECTED_C) {
            settingsinternal::setSensorStatus(i, SensorStatusEnum::NoResponse);
            continue;
        }

        if(settingsinternal::getSensorStatus(i) != SensorStatusEnum::Active)
            settingsinternal::setSensorStatus(i, SensorStatusEnum::Active);
        deviceReaded++;
        tempAvg += tempTmp;
        settingsinternal::setSensorTemp(i, tempTmp);
    }
    tempAvg /= deviceReaded;
    settingsinternal::setTempAvg(tempAvg);


    if((tempAvg < settingsinternal::getRequiredTemp()) &&
            (deviceReaded > 0)) HWiface::turnHeaterOn();
    else HWiface::turnHeaterOff();


    indicateAll(deviceReaded, tempAvg);
}
