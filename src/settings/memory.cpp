#include "memory.h"
#include <polymorphmemory.h>


uint8_t SlaveAddress::newAddr = 0;
bool SlaveAddress::flag = false;


typedef MainMem MemoryMammer ;

SettingsExternal * settng;


MemoryMap::MemoryMap(SettingsExternal & settings)
{
    settng = &settings;
}

int8_t MemoryMap::write(uint8_t addr, uint8_t data)
{
    return MemoryMammer::write(addr, data);
}

int16_t MemoryMap::read(uint8_t addr)
{
    return MemoryMammer::read(addr);
}

uint16_t MemoryMap::mapsize()
{
    return sizeof(MemoryMammer);
}
