#ifndef MEMORY_H
#define MEMORY_H

#include <inttypes.h>
#include <mainmem.h>
#include <server.h>


class SettingsExternal;

class MemoryMap : public iMappedMemory
{
public:
    MemoryMap(SettingsExternal & settings);

    int8_t write(uint8_t addr, uint8_t data);
    int16_t read(uint8_t addr);
    uint16_t mapsize();

};



#endif
