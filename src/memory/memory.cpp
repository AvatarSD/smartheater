#include "memory.h"
#include <polymorphmemory.h>
#include "settings.h"


class GUID : public Composite<uint8_t[GUID_SIZE]>
{
public:
    static Error write(Address addr, uint8_t data, Num num)
    {
        return OK;
    }
    static ReadType read(Address addr, Num num)
    {
        return settingsextetnal::getGUID(addr);
    }
};
class SlaveAddress : public Composite<uint8_t>
{
public:
    static Error write(Address addr, uint8_t data, Num num)
    {
        return OK;
    }
    static ReadType read(Address addr, Num num = 0)
    {
        return settingsextetnal::getI2cAddress();
    }
};
class RequiredTemp : public Composite<int16_t>
{
public:
    static Error write(Address addr, uint8_t data, Num num)
    {
        settingsextetnal::setRequiredTemp(data, addr);
        return OK;
    }
    static ReadType read(Address addr, Num num = 0)
    {
        return settingsextetnal::getRequiredTemp(addr);
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
        return settingsextetnal::getDeviceCount();
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
        return settingsextetnal::getTempAvg(addr);
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
        return 'S';
    }
};
class Control : public Composite<uint8_t>
{
public:
    static Error write(Address addr, uint8_t data, Num num)
    {
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
        if(addr == 1)
            return num;
        return 'r';
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
        return 't';
    }
};
class SensorStatus : public Composite<uint8_t>
{
public:
    static Error write(Address addr, uint8_t data, Num num)
    {
        return OK;
    }
    static ReadType read(Address addr, Num num = 0)
    {
        return 's';
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




class Header : public
    Composite<GUID, SlaveAddress, SensorCount, RequiredTemp, TempAvg, Status, Control> {};
class Node : public Composite<ROM, Temp, SensorStatus> {};
class Nodes : public CompositeList<Node, MAX_SENSORS> {};

class MainMem : public Composite<Header, _res_, Nodes> {};



static_assert(sizeof(MainMem) == 256, "MainMen is not 256bytes in size");
typedef MainMem MainMemory;



namespace memory
{

int8_t write(uint8_t addr, uint8_t data)
{
    return MainMemory::write(addr, data);
}

int16_t read(uint8_t addr)
{
    return MainMemory::read(addr);
}

uint16_t mapsize()
{
    return sizeof(MainMemory);
}

}
