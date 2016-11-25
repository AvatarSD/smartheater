#include "settings.h"
#include <avr/eeprom.h>
#include "memory.h"
#include <conf.h>




struct SensorNode {
    uint8_t rom[ROM_SIZE];
    uint8_t status;
};

namespace backstagemem
{

int16_t EEMEM requiredTemp = REQUIRED_TEMP * 16;
uint8_t EEMEM slaveAddress = I2C_SLAVE_ADDRESS;

uint8_t EEMEM GUID[GUID_SIZE] = { // TODO autogen guid
    0x66, 0x11, 0x70, 0x8B,
    0xC4, 0x76, 0x41, 0x96,
    0x91, 0x12, 0xA0, 0x91,
    0x44, 0xF0, 0xBF, 0x9C
};

uint8_t deviceCount = 0;
int16_t tempAvg = 0;

SensorNode EEMEM sensorNodes[MAX_SENSORS];
int16_t sensorTemps[MAX_SENSORS];

}



namespace settingsinternal
{

float getRequiredTemp()
{
    return ((float)eeprom_read_word((uint16_t *)&backstagemem::requiredTemp)) / 16;
}

uint8_t getI2cAddress()
{
    return eeprom_read_byte(&backstagemem::slaveAddress);
}

void setDeviceCount(uint8_t count)
{
    backstagemem::deviceCount = count;
}

void setTempAvg(const float & temp)
{
    backstagemem::tempAvg = (int16_t)(temp * 16);
}

void setSensorRom(uint8_t num, const uint8_t * rom)
{
    eeprom_write_block(rom, backstagemem::sensorNodes[num].rom, ROM_SIZE);
}

void setSensorStatus(uint8_t num, RomStatus status)
{
    eeprom_write_byte(&backstagemem::sensorNodes[num].status, (uint8_t)status);
}

void setSensorTemp(uint8_t num, const float & temp)
{
    backstagemem::sensorTemps[num] = (int16_t)(temp * 16);
}

const uint8_t * getSensorRom(uint8_t num)
{
    static uint8_t rom[ROM_SIZE];
    eeprom_read_block(rom, backstagemem::sensorNodes[num].rom, ROM_SIZE);
    return rom;
}

RomStatus getSensorStatus(uint8_t num)
{
    return (RomStatus)eeprom_read_byte(&backstagemem::sensorNodes[num].status);
}


}



namespace settingsextetnal
{

uint8_t getGUID(uint8_t pos)
{
    return eeprom_read_byte(&backstagemem::GUID[pos]);
}

uint8_t getTempAvg(uint8_t pos)
{
    return (uint8_t)(backstagemem::tempAvg >> (8 * pos)) & 0xff;
}

uint8_t getDeviceCount()
{
    return backstagemem::deviceCount;
}

uint8_t getI2cAddress()
{
    return settingsinternal::getI2cAddress();
}

uint8_t getRequiredTemp(uint8_t pos)
{
    return eeprom_read_byte(((uint8_t *)&backstagemem::requiredTemp) + pos);
}

void setRequiredTemp(uint8_t temp, uint8_t pos)
{
    static int16_t tempTmp = 0;

    tempTmp |= (uint16_t)(temp << (pos * 8));
    if(pos == sizeof(tempTmp) - 1) {
        eeprom_write_word((uint16_t *)&backstagemem::requiredTemp, (uint16_t)tempTmp);
        tempTmp = 0;
    }
}

uint8_t getSensorRom(uint8_t num, uint8_t pos)
{
    return eeprom_read_byte(&backstagemem::sensorNodes[num].rom[pos]);
}

uint8_t getSensorTemp(uint8_t num, uint8_t pos)
{
    return eeprom_read_byte(((uint8_t *)&backstagemem::sensorTemps[num]) + pos);
}

uint8_t getSensorStatus(uint8_t num)
{
    return eeprom_read_byte(&backstagemem::sensorNodes[num].status);
}

void setSensorStatus(uint8_t num, uint8_t status)
{
    eeprom_write_byte(&backstagemem::sensorNodes[num].status, status);
}


}
