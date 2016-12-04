#include "eeprom.h"
#include <avr/eeprom.h>


Settings * Settings::instance()
{
    static Settings conf;
    return &conf;
}


struct SensorNode {
    uint8_t rom[ROM_SIZE];
    uint8_t status;
};

namespace backstagemem
{

uint8_t EEMEM GUID[GUID_SIZE] = { // TODO autogen guid
    0x66, 0x11, 0x70, 0x8B,
    0xC4, 0x76, 0x41, 0x96,
    0x91, 0x12, 0xA0, 0x91,
    0x44, 0xF0, 0xBF, 0x9C
};
uint8_t EEMEM deviceName[DEVNAME_SIZE] = DEV_NAME;
uint16_t EEMEM deviceHWver = DEV_HW_VER;
const uint16_t deviceSWver = DEV_SW_VER;

uint8_t EEMEM slaveAddress = I2C_SLAVE_ADDRESS;

int16_t EEMEM requiredTemp = REQUIRED_TEMP * 16;

uint8_t deviceCount = 0;
int16_t tempAvg = 0;

uint8_t EEMEM deviceStatus = 0;

SensorNode EEMEM sensorNodes[MAX_SENSORS];
int16_t sensorTemps[MAX_SENSORS];

}

// TODO rename symbos to math(save instead set)
// todo minimise dependency


uint8_t Settings::getDeviceCount()
{
    return backstagemem::deviceCount;
}

uint8_t Settings::getI2cAddress()
{
    return eeprom_read_byte(&backstagemem::slaveAddress);
}

float Settings::getRequiredTemp()
{
    return ((float)eeprom_read_word((uint16_t *)&backstagemem::requiredTemp)) / 16;
}

void Settings::setDeviceCount(uint8_t count)
{
    backstagemem::deviceCount = count;
}

void Settings::setTempAvg(const float & temp)
{
    backstagemem::tempAvg = (int16_t)(temp * 16);
}

void Settings::setSensorRom(uint8_t num, const uint8_t * rom)
{
    eeprom_write_block(rom, backstagemem::sensorNodes[num].rom, ROM_SIZE);
}

void Settings::setSensorTemp(uint8_t num, const float & temp)
{
    backstagemem::sensorTemps[num] = (int16_t)(temp * 16);
}

void Settings::getSensorRom(uint8_t num, uint8_t * rom)
{
    eeprom_read_block(rom, backstagemem::sensorNodes[num].rom, ROM_SIZE);
}

void Settings::scanEEpromForRomsCount()
{
    uint8_t i = 0;
    for(; i < MAX_SENSORS; i++)
        if(getSensorStatus(i) == NoAvailable)
            break;
    setDeviceCount(i);
}

uint8_t Settings::getDeviceGUID(uint8_t pos)
{
    return eeprom_read_byte(&backstagemem::GUID[pos]);
}

uint8_t Settings::getDeviceName(uint8_t pos)
{
    return eeprom_read_byte(&backstagemem::deviceName[pos]);
}

uint8_t Settings::getDeviceSWver(uint8_t pos)
{
    return *(((uint8_t *) &backstagemem::deviceSWver) + pos);
}

uint8_t Settings::getDeviceHWver(uint8_t pos)
{
    return eeprom_read_byte(((uint8_t *)&backstagemem::deviceHWver) + pos);
}

uint8_t Settings::getTempAvg(uint8_t pos)
{
    return (uint8_t)(backstagemem::tempAvg >> (8 * pos)) & 0xff;
}


uint8_t Settings::getRequiredTemp(uint8_t pos)
{
    return eeprom_read_byte(((uint8_t *)&backstagemem::requiredTemp) + pos);
}

void Settings::setRequiredTemp(uint8_t temp, uint8_t pos)
{
    static int16_t tempTmp = 0;

    tempTmp |= (uint16_t)(temp << (pos * 8));
    if(pos == sizeof(tempTmp) - 1) {
        eeprom_write_word((uint16_t *)&backstagemem::requiredTemp, (uint16_t)tempTmp);
        tempTmp = 0;
    }
}

uint8_t Settings::getSensorRom(uint8_t num, uint8_t pos)
{
    return eeprom_read_byte(&backstagemem::sensorNodes[num].rom[pos]);
}

uint8_t Settings::getSensorTemp(uint8_t num, uint8_t pos)
{
    return (uint8_t)(backstagemem::sensorTemps[num] >> (8 * pos)) & 0xff;
}

uint8_t Settings::getSensorStatus(uint8_t num)
{
    return eeprom_read_byte(&backstagemem::sensorNodes[num].status);
}

void Settings::setSensorStatus(uint8_t num, uint8_t status)
{
    eeprom_write_byte(&backstagemem::sensorNodes[num].status, status);
}

void Settings::setDeviceMode(uint8_t status)
{
    eeprom_write_byte((uint8_t *)&backstagemem::deviceStatus, status);
}

uint8_t Settings::getDeviceModeStatus()
{
    return eeprom_read_byte((uint8_t *)&backstagemem::deviceStatus);
}

void Settings::setI2cAddress(uint8_t addr)
{
    eeprom_write_byte(&backstagemem::slaveAddress, addr);
}
