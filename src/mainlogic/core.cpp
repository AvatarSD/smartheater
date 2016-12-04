#include "core.h"
#include <settings.h>
#include <indication.h>
#include <hwiface.h>
#include <DallasTemperature.h>
#include <server.h>



CoreLogic::CoreLogic(I2CSlaveServer & server,
                     DallasTemperature & sensors,
                     SettingsInternal & config,
                     HWiface & hardware,
                     Indication & leds) :
    server(&server),
    sensors(&sensors),
    config(&config),
    hardware(&hardware),
    leds(&leds)
{
    this->config->scanEEpromForRomsCount();
}

void CoreLogic::eraceeeprom()
{
    DeviceAddress nullRom = {0, 0, 0, 0,
                             0, 0, 0, 0
                            };
    for(uint8_t i = 0; i < MAX_SENSORS; i++) {
        config->setSensorRom(i, nullRom);
        config->setSensorStatus(i, SensorStatusEnum::NoAvailable);
        config->setSensorTemp(i, 0);
    }
    config->setDeviceCount(0);
}

void CoreLogic::heaterHandler(const float & tempAvg, uint16_t deviceReaded)
{
    if((tempAvg < config->getRequiredTemp()) &&
            (deviceReaded > 0)) hardware->turnHeaterOn();
    else hardware->turnHeaterOff();
}

void CoreLogic::searchSensors()
{
    hardware->turnOneWireLineOn();
    sensors->begin();
    eraceeeprom();
    DeviceAddress deviceAddress;
    uint16_t deviceCounter = 0;
    sensors->getOneWire()->reset_search();
    while(sensors->getOneWire()->search(deviceAddress)) {
        if((sensors->validAddress(deviceAddress)) && (deviceCounter < MAX_SENSORS)) {
            config->setSensorRom(deviceCounter, deviceAddress);
            config->setSensorStatus(deviceCounter, SensorStatusEnum::Active);
            deviceCounter++;
        }
    }
    config->setDeviceCount(deviceCounter);
}

void CoreLogic::mainCycle()
{
    hardware->turnOneWireLineOn();
    sensors->begin();

    sensors->requestTemperatures();

    uint16_t deviceReaded = 0, deviceCount = config->getDeviceCount();
    float tempAvg = 0;
    DeviceAddress deviceAddress;

    for(uint16_t i = 0; i < deviceCount; i++) {

        if(config->getSensorStatus(i) == SensorStatusEnum::Disable)
            continue;

        config->getSensorRom(i, deviceAddress);
        float tempTmp = sensors->getTempC((uint8_t *)deviceAddress);

        if(tempTmp == DEVICE_DISCONNECTED_C) {
            config->setSensorStatus(i, SensorStatusEnum::NoResponse);
            continue;
        }

        if(config->getSensorStatus(i) != SensorStatusEnum::Active)
            config->setSensorStatus(i, SensorStatusEnum::Active);
        deviceReaded++;
        tempAvg += tempTmp;
        config->setSensorTemp(i, tempTmp);
    }
    tempAvg /= deviceReaded;
    config->setTempAvg(tempAvg);


    if(config->getDeviceModeStatus() == Auto)
        heaterHandler(tempAvg, deviceReaded);

    leds->indicateAll(deviceReaded, tempAvg);
}

void CoreLogic::setI2cAddress(uint8_t addr)
{
    server->getNetworkObject()->setAddress(addr);
}

void CoreLogic::setDeviceMode(uint8_t status)
{
    if(status == Search) {
        this->searchSensors();
        return;
    } else if(status == Erace) {
        this->eraceeeprom();
        return;
    }

    else if(status == EnableHeater)
        hardware->turnHeaterOn();
    else if(status == DisableHeater)
        hardware->turnHeaterOff();

}
