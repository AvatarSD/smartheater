#ifndef SETTINGS_H
#define SETTINGS_H

#include <inttypes.h>
#include <conf.h>


namespace settingsinternal
{

void setDeviceCount(uint8_t count);
void setTempAvg(const float & temp);
float getRequiredTemp();
uint8_t getI2cAddress();
void setSensorRom(uint8_t num, const uint8_t * rom);
void setSensorStatus(uint8_t num, SensorStatusEnum status);
void setSensorTemp(uint8_t num, const float & temp);
void getSensorRom(uint8_t num, uint8_t * rom);
SensorStatusEnum getSensorStatus(uint8_t num);
uint8_t getDeviceCount();
HeaterMode getDeviceModeStatus();
void scanEEpromForRomsCount();

}

namespace settingsextetnal
{

uint8_t getDeviceCount();
uint8_t getRequiredTemp(uint8_t pos);
void setRequiredTemp(uint8_t temp, uint8_t pos);
uint8_t getTempAvg(uint8_t pos);
uint8_t getGUID(uint8_t pos);
uint8_t getI2cAddress();
uint8_t getSensorRom(uint8_t num, uint8_t pos);
uint8_t getSensorTemp(uint8_t num, uint8_t pos);
uint8_t getSensorStatus(uint8_t num);
void setSensorStatus(uint8_t num, uint8_t status);
void setDeviceMode(uint8_t status);
uint8_t getDeviceModeStatus();
void setI2cAddress(uint8_t addr);

}

#endif // SETTINGS_H
