#ifndef CORE_H
#define CORE_H

#include <settings.h>
#include <indication.h>
#include <hwiface.h>
#include <DallasTemperature.h>


class AutoHeaterControl : public IAutoHeaterControl
{
public:
    AutoHeaterControl(ISettingsInt * settings,
                      HWiface * hardware);

    virtual void doHandle();

    void executeCommand(DeviceCommand cmd) override;
    void setDeviceMode(DeviceMode mode) override;
    DeviceMode getDeviceMode() const override;
    DeviceStatus getDeviceStatus() const override;
    Temp getTempAvg() const override;
    SensorNum getSensorsCount() const override;
    Temp getSensorTemp(SensorNum num) const override;
    SensorStatus getSensorStatus(SensorNum num) const override;

protected:
    virtual SensorNum searchSensors();
    virtual void eraceeeprom();

    ISettingsInt * settings;
    HWiface * hardware;

    Temp tempAvg;
    DeviceMode mode;
    SensorNum sensorsCount;
    DeviceStatus deviceStatus;

    Temp sensorsTemp[MAX_SENSORS];
    SensorStatus sensorsStatus[MAX_SENSORS];
};

class BasicAutoHeaterController : public AutoHeaterControl
{
public:
    BasicAutoHeaterController(DallasTemperature * sensors, ISettingsInt * settings,
                              HWiface * hardware, Indication * leds);

    void doHandle() override;

protected:
    SensorNum searchSensors() override;
    void eraceeeprom() override;

    void heaterHandler(const Temp & tempAvg, SensorNum sensorsReaded);

    DallasTemperature * sensors;
    Indication * leds;
};

#endif // CORE_H
