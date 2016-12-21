#ifndef MAPPEDMEMORY_H
#define MAPPEDMEMORY_H

#include <server.h>
#include <settings.h>
#include <idcells.h>

extern ISettingsExt * settings;
extern IAutoHeaterControl * control;
extern ITwiSlave * network;




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

class Header : public
    Composite<SensorCount, RequiredTemp, TempAvg, Status, Mode, Control> {};
class Node : public Composite<SensROM, SensTemp, SensStatus> {};
class Nodes : public CompositeList<Node, MAX_SENSORS> {};
class MainMem : public
    Composite <CommonShared, Header, _res_, Nodes> {};

static_assert(sizeof(MainMem) == 256, "MainMen is not 256bytes in size");


template<typename MemoryMap>
class MappedMemory : public IMemory
{
public:
    MappedMemory(ISettingsExt * settngs,
                 IAutoHeaterControl * ctrl,
                 ITwiSlave * net)
    {
        settings = settngs;
        control = ctrl;
        network = net;
        SlaveAddress::setISlaveAddress(network);
        CommonShared::setSettings(settings);
    }

    int8_t write(uint8_t addr, uint8_t data)
    {
        return MemoryMap::write(addr, data);
    }
    int16_t read(uint8_t addr)
    {
        return MemoryMap::read(addr);
    }
    uint16_t size()
    {
        return MemoryMap::size();
    }

};


#endif // MAPPEDMEMORY_H
