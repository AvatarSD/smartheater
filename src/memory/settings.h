#ifndef SETTINGS_H
#define SETTINGS_H

#include <inttypes.h>


namespace settingsinternal
{

enum RomStatus {
    NoAvailable = 0x00,
    Disable = 0x01,
    Active = 0x02,
    NoResponse = 0x03
};




void setDeviceCount(uint8_t count);
void setTempAvg(const float & temp);
float getRequiredTemp();
uint8_t getI2cAddress();


void setSensorRom(uint8_t num, const uint8_t * rom);
void setSensorStatus(uint8_t num, RomStatus status);
void setSensorTemp(uint8_t num, const float & temp);
const uint8_t * getSensorRom(uint8_t num);
RomStatus getSensorStatus(uint8_t num);



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


}

#endif // SETTINGS_H
