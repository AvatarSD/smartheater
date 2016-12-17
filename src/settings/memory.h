#ifndef MAPPEDMEMORY_H
#define MAPPEDMEMORY_H

#include <server.h>
#include <settings.h>


class MappedMemory : public IMappedMemory
{
public:
    MappedMemory(ISettingsExt * settings,
                 IAutoHeaterControl * control,
                 ITwiSlave * network);

    int8_t write(uint8_t addr, uint8_t data);
    int16_t read(uint8_t addr);
    uint16_t mapsize();

};


#endif // MAPPEDMEMORY_H
