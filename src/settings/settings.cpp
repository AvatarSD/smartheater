#include "settings.h"
#include <corestateserver.h>



HeaterMode SettingsInternal::getDeviceModeStatus()
{
    return (HeaterMode) mem->getDeviceModeStatus();
}

uint8_t SettingsInternal::getDeviceCount()
{
    return mem->getDeviceCount();
}

uint8_t SettingsInternal::getI2cAddress()
{
    return mem->getI2cAddress();
}

float SettingsInternal::getRequiredTemp()
{
    return mem->getRequiredTemp();
}

void SettingsInternal::setDeviceCount(uint8_t count)
{
    mem->setDeviceCount(count);
}

void SettingsInternal::setTempAvg(const float & temp)
{
    mem->setTempAvg(temp);
}

void SettingsInternal::setSensorRom(uint8_t num, const uint8_t * rom)
{
    mem->setSensorRom(num, rom);
}

void SettingsInternal::setSensorStatus(uint8_t num, SensorStatusEnum status)
{
    mem->setSensorStatus(num, status);
}

void SettingsInternal::setSensorTemp(uint8_t num, const float & temp)
{
    mem->setSensorTemp(num, temp);
}

void SettingsInternal::getSensorRom(uint8_t num, uint8_t * rom)
{
    mem->getSensorRom(num, rom);
}

SensorStatusEnum SettingsInternal::getSensorStatus(uint8_t num)
{
    return (SensorStatusEnum)mem->getSensorStatus(num);
}

void SettingsInternal::scanEEpromForRomsCount()
{
    mem->scanEEpromForRomsCount();
}



void SettingsExternal::setICoreState(ICoreState * statemachine)
{
    state = statemachine;
}


uint8_t SettingsExternal::getDeviceCount()
{
    return mem->getDeviceCount();
}

uint8_t SettingsExternal::getDeviceModeStatus()
{
    return mem->getDeviceModeStatus();
}

uint8_t SettingsExternal::getGUID(uint8_t pos)
{
    return mem->getGUID(pos);
}

uint8_t SettingsExternal::getI2cAddress()
{
    return mem->getI2cAddress(); //TODO
}

uint8_t SettingsExternal::getTempAvg(uint8_t pos)
{
    return mem->getTempAvg(pos);
}

uint8_t SettingsExternal::getRequiredTemp(uint8_t pos)
{
    return mem->getRequiredTemp(pos);
}

void SettingsExternal::setRequiredTemp(uint8_t temp, uint8_t pos)
{
    mem->setRequiredTemp(temp, pos);
}

uint8_t SettingsExternal::getSensorRom(uint8_t num, uint8_t pos)
{
    return mem->getSensorRom(num, pos);
}

uint8_t SettingsExternal::getSensorTemp(uint8_t num, uint8_t pos)
{
    return mem->getSensorTemp(num, pos);
}

uint8_t SettingsExternal::getSensorStatus(uint8_t num)
{
    return mem->getSensorStatus(num);
}

void SettingsExternal::setSensorStatus(uint8_t num, uint8_t status)
{
    mem->setSensorStatus(num, status);
}


void SettingsExternal::setI2cAddress(uint8_t addr)
{
    mem->setI2cAddress(addr);
    state->setI2cAddress(addr);
}

void SettingsExternal::setDeviceMode(uint8_t status)
{
    state->setDeviceMode(status);
    if(status > 0b11)
        mem->setDeviceMode(status);
}
