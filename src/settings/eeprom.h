#ifndef EEPROM_H
#define EEPROM_H

#include <inttypes.h>


#define GUID_SIZE 16
#define ROM_SIZE 8
#define MAX_SENSORS 20

enum SensorStatusEnum {
    NoAvailable = 0x00,
    Disable = 0x01,
    Active = 0x02,
    NoResponse = 0x03
};

class Settings
{
public:
    void getSensorRom(uint8_t num, uint8_t * rom);
    uint8_t getSensorRom(uint8_t num, uint8_t pos);
    void setSensorRom(uint8_t num, const uint8_t * rom);

    float getRequiredTemp();
    uint8_t getRequiredTemp(uint8_t pos);
    void setRequiredTemp(uint8_t temp, uint8_t pos);

    uint8_t getDeviceCount();
    void setDeviceCount(uint8_t count);

    uint8_t getI2cAddress();
    void setI2cAddress(uint8_t addr);

    void setTempAvg(const float & temp);
    uint8_t getTempAvg(uint8_t pos);

    uint8_t getSensorTemp(uint8_t num, uint8_t pos);
    void setSensorTemp(uint8_t num, const float & temp);

    uint8_t getDeviceModeStatus();
    void setDeviceMode(uint8_t status);

    uint8_t getSensorStatus(uint8_t num);
    void setSensorStatus(uint8_t num, uint8_t status);


    uint8_t getGUID(uint8_t pos);

    void scanEEpromForRomsCount();

    static Settings * instance();

private:
    Settings() {}
    Settings(const Settings &) = default;
    const Settings & operator = (const Settings &) = delete;
};


#endif // EEPROM_H
