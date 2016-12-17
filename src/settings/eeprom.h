#ifndef EEPROM_H
#define EEPROM_H

#include "settings.h"


class Settings :
    public ISettingsInt,
    public ISettingsExt
{
public:
    //ISettingsGeneral
    uint8_t getDeviceGUID(uint8_t pos) const;
    uint8_t getDeviceName(uint8_t pos) const;
    uint8_t getDeviceSWver(uint8_t pos) const;
    uint8_t getDeviceHWver(uint8_t pos) const;

    //ISettingsInt
    void setSensorRom(SensorNum num, const uint8_t * rom);
    void getSensorRom(SensorNum num, uint8_t * rom) const;
    Temp getRequiredTemp() const;

    //ISettingsExt
    void setAddress(I2CAddress addr);
    void setDeviceMode(DeviceMode mode);
    void setRequiredTempRaw(RawTemp temp);
    void setSensorMode(SensorNum num, SensorMode mode);
    RawTemp getRequiredTempRaw() const;
    uint8_t getSensorRom(SensorNum sensor, uint8_t pos) const;

    //common
    I2CAddress getAddress() const;
    DeviceMode getDeviceMode() const;
    SensorNum getSensorsCount() const;
    SensorMode getSensorMode(SensorNum sensor) const;

    //instance
    static Settings * instance();

private:
    Settings() {}
    Settings(const Settings &) = default;
    const Settings & operator = (const Settings &) = delete;
};


#endif // EEPROM_H
