#ifndef SETTINGS_H
#define SETTINGS_H

#include <inttypes.h>
#include <usiTwiSlave.h>

// helper
#define VER(MAJOR, MINOR) ((MAJOR << 8)|(MINOR))
#define tempToRaw(x) ((RawTemp)(x*16))
#define tempFromRaw(x) (((Temp)x)/16)

// hardWare defined settings
#define GUID_SIZE 16
#define ROM_SIZE 8
#define DEVNAME_SIZE 4
#define RESERVED_SIZE 3

#define DEV_NAME {'a', 'h', 't', 'r'}
#define DEV_SW_VER VER(1, 5)
#define DEV_HW_VER VER(0, 1)

// device specific macro
#define MAX_SENSORS 20

// default instance for first-time program
#define I2C_SLAVE_ADDRESS 0x13
#define REQUIRED_TEMP 20
#define DEFAULT_DEVICE_MODE DeviceMode::NormalAuto


enum SensorStatus {
    NoAvailable = 0x00,
    Active = 0x01,
    Off = 0x02,
    NoResponse = 0x03,
};

enum SensorMode {
    Err = 0x00,
    Enable = 0x01,
    Disable = 0x02
};

enum DeviceStatus {
    HeaterOff = 0x00,
    HeaterOn = 0x01
};

enum DeviceMode {
    DisableHeater = 0b00,
    EnableHeater = 0b01,
    NormalAuto = 0b10,
    InverseAuto = 0b11
};

enum DeviceCommand {
    Erace = 0b01,
    Search = 0b10
};

typedef float Temp;
typedef uint16_t RawTemp;
typedef uint8_t SensorNum;


class IAutoHeaterControl
{
public:
    virtual void executeCommand(DeviceCommand cmd);
    virtual void setDeviceMode(DeviceMode mode);
    virtual DeviceStatus getDeviceStatus() const;
    virtual Temp getTempAvg() const;
    virtual SensorNum getFoundedSensorsCount() const;
    virtual Temp getSensorTemp(SensorNum num) const;
    virtual SensorStatus getSensorStatus(SensorNum num) const;

};

class ISettingsInt
{
public:
    virtual void setSensorMode(SensorNum sensor, SensorMode mode);
    virtual void setSensorRom(SensorNum num, const uint8_t * rom);
    virtual void getSensorRom(SensorNum num, uint8_t * rom) const;
    virtual SensorNum getSensorsCount() const;
    virtual SensorMode getSensorMode(SensorNum num) const;
    virtual I2CAddress getAddress() const;
    virtual Temp getRequiredTemp() const;
    virtual DeviceMode getDeviceMode() const;
};

class ISettingsExt : public ISlaveAddress
{
public:
    virtual uint8_t getDeviceGUID(uint8_t pos) const;
    virtual uint8_t getDeviceName(uint8_t pos) const;
    virtual uint8_t getDeviceSWver(uint8_t pos) const;
    virtual uint8_t getDeviceHWver(uint8_t pos) const;

    virtual void setAddress(I2CAddress addr);
    virtual void setDeviceMode(DeviceMode status);
    virtual void setRequiredTempRaw(RawTemp temp);
    virtual void setSensorMode(SensorNum sensor, SensorMode mode);
    virtual I2CAddress getAddress() const;
    virtual DeviceMode getDeviceMode() const;
    virtual RawTemp getRequiredTempRaw() const;
    virtual SensorNum getSensorsCount() const;
    virtual uint8_t getSensorRom(SensorNum sensor, uint8_t pos) const;
    virtual SensorMode getSensorMode(SensorNum sensor) const;
};


#endif // SETTINGS_H
