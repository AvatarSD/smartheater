#include "settings.h"
#include <avr/eeprom.h>
#include "memory.h"



#define REQUIRED_TEMP 20*16
#define I2C_SLAVE_ADDRESS 0x04 // todo for test

namespace backstagemem
{

int16_t EEMEM requiredTemp = REQUIRED_TEMP;
uint8_t EEMEM slaveAddress = I2C_SLAVE_ADDRESS;

uint8_t EEMEM GUID[GUID_SIZE] = {
    0x66, 0x11, 0x70, 0x8B,
    0xC4, 0x76, 0x41, 0x96,
    0x91, 0x12, 0xA0, 0x91,
    0x44, 0xF0, 0xBF, 0x9C
};

uint8_t deviceCount = 0;
int16_t tempAvg = 0;


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



}
