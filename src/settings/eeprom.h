#ifndef EEPROM_H
#define EEPROM_H

#include <inttypes.h>

#define VER(MAJOR, MINOR) ((MAJOR << 8)|(MINOR))

// hardWare defined settings
#define GUID_SIZE 16
#define DEVNAME_SIZE 4

#define DEV_NAME {'a', 'h', 't', 'r'}
#define DEV_SW_VER VER(1, 5)
#define DEV_HW_VER VER(0, 1)

// here is a default one
#define MULTICAST_ADDR 0x50

//default instance for first-time program
#define I2C_SLAVE_ADDRESS 0x13

//device specific macro
#define ROM_SIZE 8
#define MAX_SENSORS 20
#define RESERVED_SIZE 4
#define MAX_SENSORS 20

//default instance for first-time program
#define REQUIRED_TEMP 20
#define DEV_DEFAULT_STATUS HeaterMode::Auto


enum SensorStatusEnum {
    NoAvailable = 0x00,
    Disable = 0x01,
    Active = 0x02,
    NoResponse = 0x03
};

enum HeaterMode {
    DisableHeater = 0b00,
    EnableHeater = 0b01,
    Auto = 0b10
};

enum SearchMode {
    Search = 0b100,
    Erace = 0b1000
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

    void scanEEpromForRomsCount();

    uint8_t getDeviceGUID(uint8_t pos);
    uint8_t getDeviceName(uint8_t pos);
    uint8_t getDeviceSWver(uint8_t pos);
    uint8_t getDeviceHWver(uint8_t pos);


    static Settings * instance();

private:
    Settings() {}
    Settings(const Settings &) = default;
    const Settings & operator = (const Settings &) = delete;
};


#endif // EEPROM_H
