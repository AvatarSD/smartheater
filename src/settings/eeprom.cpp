#include "eeprom.h"
#include <avr/eeprom.h>


struct SensorsNodes {
    uint8_t rom[ROM_SIZE];
    SensorMode mode;
};

namespace backstagemem
{
//system
uint8_t EEMEM GUID[GUID_SIZE] = { // TODO autogen guid
    0x66, 0x11, 0x70, 0x8B,
    0xC4, 0x76, 0x41, 0x96,
    0x91, 0x12, 0xA0, 0x91,
    0x44, 0xF0, 0xBF, 0x9C
};
uint16_t EEMEM deviceHWver = DEV_HW_VER;
const uint16_t deviceSWver = DEV_SW_VER;
const uint8_t deviceName[DEVNAME_SIZE] = DEV_NAME;

//device
I2CAddress EEMEM slaveAddress = I2C_SLAVE_ADDRESS;
RawTemp EEMEM requiredTemp = tempToRaw(REQUIRED_TEMP);
DeviceMode EEMEM deviceMode = DEFAULT_DEVICE_MODE;
SensorsNodes EEMEM sensorNodes[MAX_SENSORS];
}

Settings * Settings::instance()
{
    static Settings conf;
    return &conf;
}

uint8_t Settings::getDeviceGUID(uint8_t pos) const
{
    return eeprom_read_byte(&backstagemem::GUID[pos]);
}
uint8_t Settings::getDeviceName(uint8_t pos) const
{
    return eeprom_read_byte(&backstagemem::deviceName[pos]);
}
uint8_t Settings::getDeviceSWver(uint8_t pos) const
{
    return *(((uint8_t *) &backstagemem::deviceSWver) + pos);
}
uint8_t Settings::getDeviceHWver(uint8_t pos) const
{
    return eeprom_read_byte(((uint8_t *)&backstagemem::deviceHWver) + pos);
}

void Settings::setSensorRom(SensorNum num, const uint8_t * rom)
{
    eeprom_write_block(rom, backstagemem::sensorNodes[num].rom, ROM_SIZE);
}
void Settings::setAddress(I2CAddress addr)
{
    eeprom_write_byte((uint8_t *)&backstagemem::slaveAddress, addr);
}
void Settings::setDeviceMode(DeviceMode mode)
{
    eeprom_write_byte((uint8_t *)&backstagemem::deviceMode, mode);
}
void Settings::setRequiredTempRaw(RawTemp temp)
{
    eeprom_write_word((uint16_t *)&backstagemem::requiredTemp, temp);
}
void Settings::setSensorMode(SensorNum num, SensorMode mode)
{
    eeprom_write_byte((uint8_t *)&backstagemem::sensorNodes[num].mode, mode);
}

void Settings::getSensorRom(SensorNum num, uint8_t * rom) const
{
    for(uint8_t i = 0; i < ROM_SIZE; i++) rom[i] = getSensorRom(num, i);
}
Temp Settings::getRequiredTemp() const
{
    return tempFromRaw(getRequiredTempRaw());
}
RawTemp Settings::getRequiredTempRaw() const
{
    return (RawTemp)eeprom_read_word(&backstagemem::requiredTemp);
}
uint8_t Settings::getSensorRom(SensorNum num, uint8_t pos) const
{
    return eeprom_read_byte(&backstagemem::sensorNodes[num].rom[pos]);
}
I2CAddress Settings::getAddress() const
{
    return (I2CAddress)eeprom_read_byte(&backstagemem::slaveAddress);
}
DeviceMode Settings::getDeviceMode() const
{
    return (DeviceMode)eeprom_read_byte((uint8_t *)&backstagemem::deviceMode);
}
SensorNum Settings::getSensorsCount() const
{
    uint8_t i = 0;
    for(; i < MAX_SENSORS; i++)
        if(getSensorMode(i) == Err)
            break;
    return i;
}
SensorMode Settings::getSensorMode(SensorNum num) const
{
    return (SensorMode)eeprom_read_byte((uint8_t *)
                                        &backstagemem::sensorNodes[num].mode);
}
