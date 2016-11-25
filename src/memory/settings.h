#ifndef SETTINGS_H
#define SETTINGS_H

#include <inttypes.h>

namespace settingsinternal
{

void setDeviceCount(uint8_t count);
void setTempAvg(const float & temp);
float getRequiredTemp();
uint8_t getI2cAddress();


//void setSensorRom(const uint8_t * rom);
//const uint8_t * getSensorRom();

}

namespace settingsextetnal
{

uint8_t getDeviceCount();
uint8_t getRequiredTemp(uint8_t pos);
void setRequiredTemp(uint8_t temp, uint8_t pos);
uint8_t getTempAvg(uint8_t pos);
uint8_t getGUID(uint8_t pos);
uint8_t getI2cAddress();

}

#endif // SETTINGS_H
