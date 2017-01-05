#ifndef MAPPEDMEMORY_H
#define MAPPEDMEMORY_H

#include <server.h>
#include <settings.h>
#include <idcells.h>


#define readByte(word, addr) ((uint8_t)((word >> (addr * 8)) & 0xFF));

template<typename TypeSize>
int8_t writeWord(TypeSize & staticReg, uint8_t addr, uint8_t data)
{
    if(addr == 0) staticReg = 0;
    staticReg |= (TypeSize)(data << (addr * 8));
    if(addr == sizeof(staticReg) - 1) return OK;
    return ERR;
}

class RequiredTemp : public IMemory
{
public:
    RequiredTemp(ISettingsExt * settings) : settings(settings) {}
    Error write(Address addr, uint8_t data, Num num)
    {
        if(writeWord(tmpRead, addr, data) == OK)
            settings->setRequiredTempRaw(tmpRead);
        return OK;
    }
    ReadType read(Address addr, Num num = 0)
    {
        if(addr == 0) tmpWrite = settings->getRequiredTempRaw();
        return readByte(tmpWrite, addr);
    }
    size_t size()
    {
        return sizeof(uint16_t);
    }
private:
    ISettingsExt * settings;
    RawTemp tmpRead;
    RawTemp tmpWrite;
};
class SensorCount : public IMemory
{
public:
    SensorCount(IAutoHeaterControl * control) : control(control) {}
    Error write(Address addr, uint8_t data, Num num)
    {
        return OK;
    }
    ReadType read(Address addr, Num num = 0)
    {
        return control->getSensorsCount();
    }
    size_t size()
    {
        return 1;
    }
private:
    IAutoHeaterControl * control;
};
class TempAvg : public IMemory
{
public:
    TempAvg(IAutoHeaterControl * control) : control(control) {}
    Error write(Address addr, uint8_t data, Num num)
    {
        return OK;
    }
    ReadType read(Address addr, Num num = 0)
    {
        if(addr == 0) tmp = control->getTempAvg();
        return readByte(tmp, addr);
    }
    size_t size()
    {
        return sizeof(uint16_t);
    }
private:
    IAutoHeaterControl * control;
    RawTemp tmp;
};
class Status : public IMemory
{
public:
    Status(IAutoHeaterControl * control) : control(control) {}
    Error write(Address addr, uint8_t data, Num num)
    {
        return OK;
    }
    ReadType read(Address addr, Num num = 0)
    {
        return (uint8_t)control->getDeviceStatus();
    }
    size_t size()
    {
        return 1;
    }
private:
    IAutoHeaterControl * control;
};
class Mode : public IMemory
{
public:
    Mode(IAutoHeaterControl * control) : control(control) {}
    Error write(Address addr, uint8_t data, Num num)
    {
        control->setDeviceMode((DeviceMode)data);
        return OK;
    }
    ReadType read(Address addr, Num num = 0)
    {
        return (uint8_t)control->getDeviceMode();
    }
    size_t size()
    {
        return 1;
    }
private:
    IAutoHeaterControl * control;
};
class Control : public IMemory
{
public:
    Control(IAutoHeaterControl * control) : control(control) {}
    Error write(Address addr, uint8_t data, Num num)
    {
        control->executeCommand((DeviceCommand)data);
        return OK;
    }
    ReadType read(Address addr, Num num = 0)
    {
        return 'x';
    }
    size_t size()
    {
        return 1;
    }
private:
    IAutoHeaterControl * control;
};
class SensROM : public IMemory
{
public:
    SensROM(ISettingsExt * settings) : settings(settings) {}
    Error write(Address addr, uint8_t data, Num num)
    {
        return OK;
    }
    ReadType read(Address addr, Num num = 0)
    {
        return settings->getSensorRom(num, addr);
    }
    size_t size()
    {
        return ROM_SIZE;
    }
private:
    ISettingsExt * settings;
};
class SensTemp : public IMemory
{
public:
    SensTemp(IAutoHeaterControl * control) : control(control) {}
    Error write(Address addr, uint8_t data, Num num)
    {
        return OK;
    }
    ReadType read(Address addr, Num num = 0)
    {
        if(addr == 0) tmp = control->getSensorTemp(num);
        return readByte(tmp, addr);
    }
    size_t size()
    {
        return 1;
    }
private:
    IAutoHeaterControl * control;
    RawTemp tmp;
};
class SensStatus : public IMemory
{
public:
    SensStatus(IAutoHeaterControl * control, ISettingsExt * settings) :
        control(control), settings(settings) {}
    Error write(Address addr, uint8_t data, Num num)
    {
        settings->setSensorMode(num, (SensorMode)data);
        return OK;
    }
    ReadType read(Address addr, Num num = 0)
    {
        uint8_t mode = settings->getSensorMode(num);
        uint8_t status = control->getSensorStatus(num);
        return ((uint8_t)(status << 4) & (mode));
    }
    size_t size()
    {
        return 1;
    }
private:
    IAutoHeaterControl * control;
    ISettingsExt * settings;

};
class _res_ : public IMemory
{
public:
    Error write(Address addr, uint8_t data, Num num)
    {
        return OK;
    }
    ReadType read(Address addr, Num num = 0)
    {
        return OK;
    }
    size_t size()
    {
        return RESERVED_SIZE;
    }
};



class Header : public
    Composite<SensorCount, RequiredTemp, TempAvg, Status, Mode, Control>
{
public:
    Header(IAutoHeaterControl * control, ISettingsExt * settings) :
        Composite(&senscount, &reqtemp, &avgtemp, &status, &mode, &this->control),
        senscount(control), reqtemp(settings), avgtemp(control),
        status(control), mode(control), control(control) {}

private:
    SensorCount senscount;
    RequiredTemp reqtemp;
    TempAvg avgtemp;
    Status status;
    Mode mode;
    Control control;
};
class Node : public Composite<SensROM, SensTemp, SensStatus>
{
public:
    Node(IAutoHeaterControl * control, ISettingsExt * settings) :
        Composite(&rom, &temp, &status), rom(settings), temp(control),
        status(control, settings) {}
private:
    SensROM rom;
    SensTemp temp;
    SensStatus status;
};
class Nodes : public CompositeList
{
public:
    Nodes(IAutoHeaterControl * control, ISettingsExt * settings) :
        CompositeList(&node, MAX_SENSORS), node(control, settings) {}

private:
    Node node;

};
class MainMem : public
    Composite <CommonShared, Header, _res_, Nodes>
{
public:
    MainMem(IAutoHeaterControl * control, ISettingsExt * settings) :
        Composite(&shared, &header, &res, &nodes), shared(settings),
        header(control, settings), nodes(control, settings) {}

    void setNetworkObject(ISlaveAddress * iAddress)
    {
        shared.setNetworkObject(iAddress);
    }

private:
    CommonShared shared;
    Header header;
    _res_ res;
    Nodes nodes;
};


#endif // MAPPEDMEMORY_H
