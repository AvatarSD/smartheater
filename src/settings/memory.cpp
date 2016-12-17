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
#include <settings.h>
#include <slaveaddres.h>


ISettingsExt * settings;
IAutoHeaterControl * control;
ITwiSlave * network;


#define readByte(word, addr) ((uint8_t)((word >> (addr * 8)) & 0xFF));

template<typename TypeSize>
int8_t writeWord(TypeSize & staticReg, uint8_t addr, uint8_t data)
{
    if(addr == 0) staticReg = 0;
    staticReg |= (TypeSize)(data << (addr * 8));
    if(addr == sizeof(staticReg) - 1) return OK;
    return ERR;
}


class GUID : public Composite<uint8_t[GUID_SIZE]>
{
public:
    static Error write(Address addr, uint8_t data, Num num)
    {
        return OK;
    }
    static ReadType read(Address addr, Num num)
    {
        return settings->getDeviceGUID(addr);
    }
};
class DeviceName : public Composite<uint8_t[DEVNAME_SIZE]>
{
public:
    static Error write(Address addr, uint8_t data, Num num)
    {
        return OK;
    }
    static ReadType read(Address addr, Num num = 0)
    {
        return settings->getDeviceName(addr);
    }
};
class DeviceSWver : public Composite<uint16_t>
{
public:
    static Error write(Address addr, uint8_t data, Num num)
    {
        return OK;
    }
    static ReadType read(Address addr, Num num = 0)
    {
        return settings->getDeviceSWver(addr);
    }
};
class DeviceHWver : public Composite<uint16_t>
{
public:
    static Error write(Address addr, uint8_t data, Num num)
    {
        return OK;
    }
    static ReadType read(Address addr, Num num = 0)
    {
        return settings->getDeviceHWver(addr);
    }
};

class RequiredTemp : public Composite<int16_t>
{
public:
    static Error write(Address addr, uint8_t data, Num num)
    {
        static RawTemp tmp;
        if(writeWord(tmp, addr, data) == OK)
            settings->setRequiredTempRaw(tmp);
        return OK;
    }
    static ReadType read(Address addr, Num num = 0)
    {
        static RawTemp tmp;
        if(addr == 0) tmp = settings->getRequiredTempRaw();
        return readByte(tmp, addr);
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
        return control->getSensorsCount();
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
        static RawTemp tmp;
        if(addr == 0) tmp = control->getTempAvg();
        return readByte(tmp, addr);
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
        return (uint8_t)control->getDeviceStatus();
    }
};
class Mode : public Composite<uint8_t>
{
public:
    static Error write(Address addr, uint8_t data, Num num)
    {
        control->setDeviceMode((DeviceMode)data);
        return OK;
    }
    static ReadType read(Address addr, Num num = 0)
    {
        return (uint8_t)control->getDeviceMode();
    }
};
class Control : public Composite<uint8_t>
{
public:
    static Error write(Address addr, uint8_t data, Num num)
    {
        control->executeCommand((DeviceCommand)data);
        return OK;
    }
    static ReadType read(Address addr, Num num = 0)
    {
        return 'x';
    }
};
class SensROM : public Composite<uint8_t[ROM_SIZE]>
{
public:
    static Error write(Address addr, uint8_t data, Num num)
    {
        return OK;
    }
    static ReadType read(Address addr, Num num = 0)
    {
        return settings->getSensorRom(num, addr);
    }
};
class SensTemp : public Composite<uint16_t>
{
public:
    static Error write(Address addr, uint8_t data, Num num)
    {
        return OK;
    }
    static ReadType read(Address addr, Num num = 0)
    {
        static RawTemp tmp;
        if(addr == 0) tmp = control->getSensorTemp(num);
        return readByte(tmp, addr);
    }
};
class SensStatus : public Composite<uint8_t>
{
public:
    static Error write(Address addr, uint8_t data, Num num)
    {
        settings->setSensorMode(num, (SensorMode)data);
        return OK;
    }
    static ReadType read(Address addr, Num num = 0)
    {
        uint8_t mode = settings->getSensorMode(num);
        uint8_t status = control->getSensorStatus(num);
        return ((uint8_t)(status << 4) & (mode));
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
class CommonShared : public
    Composite<GUID, DeviceName, DeviceSWver, DeviceHWver, SlaveAddress> {};
class Header : public
    Composite<SensorCount, RequiredTemp, TempAvg, Status, Mode, Control> {};
class Node : public Composite<SensROM, SensTemp, SensStatus> {};
class Nodes : public CompositeList<Node, MAX_SENSORS> {};
class MainMem : public
    Composite <CommonShared, Header, _res_, Nodes> {};

static_assert(sizeof(MainMem) == 256, "MainMen is not 256bytes in size");


typedef MainMem MemoryMammer ;
MappedMemory::MappedMemory(ISettingsExt * settngs,
                           IAutoHeaterControl * ctrl,
                           ITwiSlave * net)
{
    settings = settngs;
    control = ctrl;
    network = net;
    SlaveAddress::setISlaveAddress(network);
}
int8_t MappedMemory::write(uint8_t addr, uint8_t data)
{
    return MemoryMammer::write(addr, data);
}
int16_t MappedMemory::read(uint8_t addr)
{
    return MemoryMammer::read(addr);
}
uint16_t MappedMemory::mapsize()
{
    return sizeof(MemoryMammer);
}
