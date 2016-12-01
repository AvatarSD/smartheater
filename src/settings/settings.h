#ifndef SETTINGS_H
#define SETTINGS_H

#include <inttypes.h>
#include "eeprom.h"


#define RESERVED_SIZE 12

//settings
#define MAX_SENSORS 20
#define REQUIRED_TEMP 20
#define I2C_SLAVE_ADDRESS 0x04 // TODO for test

#define MULTICAST_ADDRESS 0x03



enum HeaterMode {
    DisableHeater = 0b00,
    EnableHeater = 0b01,
    Auto = 0b10
};

enum SearchMode {
    Search = 0b100,
    Erace = 0b1000
};

class ICoreState
{
public:
    virtual void setI2cAddress(uint8_t addr) {}
    virtual void setDeviceMode(uint8_t status) {}
};

class Settings;

class SettingsInternal
{
public:
    SettingsInternal(Settings & mem) : mem(&mem) {}

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

private:
    Settings * mem;
};

class SettingsExternal
{
public:
    SettingsExternal(Settings & mem) : mem(&mem) {}

    void setICoreState(ICoreState * statemachine);

    uint8_t getDeviceCount();
    uint8_t getRequiredTemp(uint8_t pos);
    uint8_t getTempAvg(uint8_t pos);
    uint8_t getGUID(uint8_t pos);
    uint8_t getI2cAddress();
    uint8_t getSensorRom(uint8_t num, uint8_t pos);
    uint8_t getSensorTemp(uint8_t num, uint8_t pos);
    uint8_t getSensorStatus(uint8_t num);
    uint8_t getDeviceModeStatus();

    void setRequiredTemp(uint8_t temp, uint8_t pos);
    void setSensorStatus(uint8_t num, uint8_t status);
    void setDeviceMode(uint8_t status);
    void setI2cAddress(uint8_t addr);

private:
    Settings * mem;
    ICoreState * state;
};

#endif // SETTINGS_H
