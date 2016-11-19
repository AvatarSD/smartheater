#include "memory.h"
#include <avr/eeprom.h>
#include <polymorphmemory.h>



#define GUID_SIZE 16
#define ROM_SIZE 8
#define RESERVED_SIZE 14
#define MAX_SENSORS 20


class GUID : public Composite<uint8_t[GUID_SIZE]>
{
public:
    static Error write(Address addr, uint8_t data, Num num)
    {
        return OK;
    }
    static ReadType read(Address addr, Num num)
    {
        static const uint8_t GUID[GUID_SIZE] = {0x66, 0x11, 0x70, 0x8B,
                                                0xC4, 0x76, 0x41, 0x96,
                                                0x91, 0x12, 0xA0, 0x91,
                                                0x44, 0xF0, 0xBF, 0x9C
                                               };
        return GUID[addr];

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
        return 'A';
    }
};
class RequiredTemp : public Composite<uint16_t>
{
public:
    static Error write(Address addr, uint8_t data, Num num)
    {
        return OK;
    }
    static ReadType read(Address addr, Num num = 0)
    {
        return 'T';
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
        return 'Q';
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
        return '_';
    }
};




class Header : public
    Composite<GUID, SlaveAddress, RequiredTemp, SensorCount, Status, Control> {};
class Node : public Composite<ROM, Temp, SensorStatus> {};
class Nodes : public CompositeList<Node, MAX_SENSORS> {};

class MainMem : public Composite<Header, _res_, Nodes> {};



static_assert(sizeof(MainMem) == 256, "MainMen is not 256bytes in size");
typedef MainMem MainMemore;



int8_t memory::write(uint8_t addr, uint8_t data)
{
    return MainMemore::write(addr, data);
}

int16_t memory::read(uint8_t addr)
{
    return MainMemore::read(addr);
}

uint16_t memory::mapsize()
{
    return sizeof(MainMemore);
}
