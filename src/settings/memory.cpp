/*
 * by S.D.
 * avatarsd.com
 */

/* I2C Memory Map
 * __________________HEADER_____________________
 * |__START__|__SIZE__|_DESCRIPTIONS_|_OPTIONS_|
 * |0        |16      |guid          |ro       |
 * |16       |1       |i2c addr      |ro       |
 * |17       |2       |require temp  |rw       |
 * |19       |1       |sensors count |ro       |
 * |20       |2       |[res]stat&ctl |rw       |
 * ---------------------------------------------
 * |SIZE: 22 |
 *
 * ________________Device Node__________________
 * |__START__|__SIZE__|_DESCRIPTIONS_|_OPTIONS_|
 * |0        |8       |ROM           |ro       |
 * |8        |2       |Temp          |ro       |
 * |10       |1       |[res]stat&ctl |rw       |
 * ---------------------------------------------
 * |SIZE: 11 |
 *
 * ___________________Main__________________
 * |__START__|__SIZE__|____DESCRIPTIONS____|
 * |0        |21      |HEADER              |
 * |22       |13      |[res]               |
 * |35+N     |46+N    |Nodes(N={0..19})    |
 * -----------------------------------------
 * |SIZE:256 |
 *
 */


#include "memory.h"
#include <polymorphmemory.h>
#include <settings.h>



SettingsExternal * settng;


class GUID : public Composite<uint8_t[GUID_SIZE]>
{
public:
    static Error write(Address addr, uint8_t data, Num num)
    {
        return OK;
    }
    static ReadType read(Address addr, Num num)
    {
        return settng->getGUID(addr);
    }
};
class SlaveAddress : public Composite<uint8_t>
{
public:
    static uint8_t newAddr;
    static bool flag;

    static Error write(Address addr, uint8_t data, Num num)
    {
        if(data == MULTICAST_ADDR) {
            settng->setI2cAddress(data);
            return OK;
        }
        newAddr = data;
        flag = true;
        return OK;
    }
    static ReadType read(Address addr, Num num = 0)
    {
        if(flag)
            settng->setI2cAddress(newAddr);
        flag = false;
        return settng->getI2cAddress();
    }
};
uint8_t SlaveAddress::newAddr = 0;
bool SlaveAddress::flag = false;

class RequiredTemp : public Composite<int16_t>
{
public:
    static Error write(Address addr, uint8_t data, Num num)
    {
        settng->setRequiredTemp(data, addr);
        return OK;
    }
    static ReadType read(Address addr, Num num = 0)
    {
        return settng->getRequiredTemp(addr);
    }
};
class SensorCount : public Composite<uint8_t>
{
public:
    static Error write(Address addr, uint8_t data, Num num)
    {
        return OK;
    }
    static ReadType read(Address addr, Num num = 0)
    {
        return settng->getDeviceCount();
    }
};
class TempAvg : public Composite<uint16_t>
{
public:
    static Error write(Address addr, uint8_t data, Num num)
    {
        return OK;
    }
    static ReadType read(Address addr, Num num = 0)
    {
        return settng->getTempAvg(addr);
    }
};
class Status : public Composite<uint8_t>
{
public:
    static Error write(Address addr, uint8_t data, Num num)
    {
        return OK;
    }
    static ReadType read(Address addr, Num num = 0)
    {
        return settng->getDeviceModeStatus();
    }
};
class Control : public Composite<uint8_t>
{
public:
    static Error write(Address addr, uint8_t data, Num num)
    {
        settng->setDeviceMode(data);
        return OK;
    }
    static ReadType read(Address addr, Num num = 0)
    {
        return 'C';
    }
};

class ROM : public Composite<uint8_t[ROM_SIZE]>
{
public:
    static Error write(Address addr, uint8_t data, Num num)
    {
        return OK;
    }
    static ReadType read(Address addr, Num num = 0)
    {
        return settng->getSensorRom(num, addr);
    }
};
class Temp : public Composite<uint16_t>
{
public:
    static Error write(Address addr, uint8_t data, Num num)
    {
        return OK;
    }
    static ReadType read(Address addr, Num num = 0)
    {
        return settng->getSensorTemp(num, addr);
    }
};
class SensorStatus : public Composite<uint8_t>
{
public:
    static Error write(Address addr, uint8_t data, Num num)
    {
        settng->setSensorStatus(num, data);
        return OK;
    }
    static ReadType read(Address addr, Num num = 0)
    {
        return settng->getSensorStatus(num);
    }
};

class _res_ : public Composite<uint8_t[RESERVED_SIZE]>
{
public:
    static Error write(Address addr, uint8_t data, Num num)
    {
        return OK;
    }
    static ReadType read(Address addr, Num num = 0)
    {
        return 0x00;
    }
};




class Header : public Composite<GUID, SlaveAddress,
    SensorCount, RequiredTemp, TempAvg, Status, Control> {};
class Node : public Composite<ROM, Temp, SensorStatus> {};
class Nodes : public CompositeList<Node, MAX_SENSORS> {};

class MainMem : public Composite<Header, _res_, Nodes> {};



static_assert(sizeof(MainMem) == 256, "MainMen is not 256bytes in size");





typedef MainMem MemoryMammer ;


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
