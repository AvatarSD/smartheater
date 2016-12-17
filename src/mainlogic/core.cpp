#include "core.h"


AutoHeaterControl::AutoHeaterControl(ISettingsInt * settings,
                                     HWiface * hardware) :
    settings(settings),
    hardware(hardware)
{
    this->tempAvg = 0;
    this->mode = settings->getDeviceMode();
    this->sensorsCount = settings->getSensorsCount();
}

void AutoHeaterControl::setDeviceMode(DeviceMode mode)
{
    this->mode = mode;
    if(mode == EnableHeater)
        hardware->turnHeaterOn();
    else if(mode == DisableHeater)
        hardware->turnHeaterOff();

}

void AutoHeaterControl::executeCommand(DeviceCommand cmd)
{
    if(cmd == Search) {
        this->searchSensors();
        return;
    } else if(cmd == Erace) {
        this->eraceeeprom();
        return;
    }
}

DeviceStatus AutoHeaterControl::getDeviceStatus() const
{
    return deviceStatus;
}

Temp AutoHeaterControl::getTempAvg() const
{
    return tempAvg;
}

Temp AutoHeaterControl::getSensorTemp(SensorNum num) const
{
    return sensorsTemp[num];
}

SensorStatus AutoHeaterControl::getSensorStatus(uint8_t num) const
{
    return sensorsStatus[num];
}

SensorNum AutoHeaterControl::getFoundedSensorsCount() const
{
    return sensorsCount;
}


BasicAutoHeaterController::BasicAutoHeaterController(
    DallasTemperature * sensors,
    ISettingsInt * config,
    HWiface * hardware,
    Indication * leds)
    :
    AutoHeaterControl(config, hardware),
    sensors(sensors),
    leds(leds)
{

}

void BasicAutoHeaterController::eraceeeprom()
{
    DeviceAddress nullRom = {
        0, 0, 0, 0,
        0, 0, 0, 0
    };
    for(SensorNum i = 0; i < MAX_SENSORS; i++) {
        settings->setSensorRom(i, nullRom);
        settings->setSensorMode(i, SensorMode::Err);
        sensorsTemp[i] = 0;
    }
    sensorsCount = 0;
}

void BasicAutoHeaterController::heaterHandler(const Temp & tempAvg,
                                              SensorNum sensorsReaded)
{
    if(sensorsReaded == 0) {
        hardware->turnHeaterOff();
        return;
    }

    if(sensorsReaded > 0) {
        if(settings->getDeviceMode() == DeviceMode::NormalAuto) {
            if(tempAvg < settings->getRequiredTemp())
                hardware->turnHeaterOn();
            else hardware->turnHeaterOff();
        } else if(settings->getDeviceMode() == DeviceMode::InverseAuto) {
            if(tempAvg < settings->getRequiredTemp())
                hardware->turnHeaterOff();
            else hardware->turnHeaterOn();
        }
    }
}
void BasicAutoHeaterController::searchSensors()
{
    hardware->turnOneWireLineOn();
    sensors->begin();
    eraceeeprom();
    DeviceAddress deviceAddress;
    SensorNum deviceCounter = 0;
    sensors->getOneWire()->reset_search();
    while(sensors->getOneWire()->search(deviceAddress)) {
        if((sensors->validAddress(deviceAddress)) &&
                (deviceCounter < MAX_SENSORS)) {
            settings->setSensorRom(deviceCounter, deviceAddress);
            sensorsStatus[deviceCounter] = SensorStatus::Active;
            deviceCounter++;
        }
    }
    sensorsCount = deviceCounter;
}

void BasicAutoHeaterController::doHandle()
{
    SensorNum deviceReaded = 0, deviceCount = settings->getSensorsCount();
    Temp tempAvg = 0;
    DeviceAddress deviceAddress;

    hardware->turnOneWireLineOn();
    sensors->begin();
    sensors->requestTemperatures();

    for(SensorNum i = 0; i < deviceCount; i++) {
        //checking - if available
        if(settings->getSensorMode(i) == SensorMode::Err) {
            sensorsStatus[i] = SensorStatus::NoAvailable;
            continue;
        }
        //checking - if enabled
        if(settings->getSensorMode(i) == SensorMode::Disable) {
            sensorsStatus[i] = SensorStatus::Off;
            continue;
        }
        //reading sensor
        settings->getSensorRom(i, deviceAddress);
        Temp tempTmp = sensors->getTempC((uint8_t *)deviceAddress);
        //if no response - continue
        if(tempTmp == DEVICE_DISCONNECTED_C) {
            sensorsStatus[i] = SensorStatus::NoResponse;
            continue;
        }
        //set status
        sensorsStatus[i] = SensorStatus::Active;
        //calc temp
        deviceReaded++;
        tempAvg += tempTmp;
        //save temperature;
        sensorsTemp[i] = tempTmp;
    }
    //calc temp
    tempAvg /= deviceReaded;
    //save temperature averege
    this->tempAvg = tempAvg;
    //call handler
    heaterHandler(tempAvg, deviceReaded);
    //indicate temp on led
    leds->indicateAll(deviceReaded, tempAvg);
}
