#ifndef CORE_H
#define CORE_H

#include <corestateserver.h>


class I2CSlaveServer;
class DallasTemperature;
class SettingsInternal;
class HWiface;
class Indication;

class CoreLogic : public ICoreState
{
public:
    CoreLogic(I2CSlaveServer & server, DallasTemperature & sensors,
              SettingsInternal & config, HWiface & hardware, Indication & leds);

    void mainCycle();
    void searchSensors();
    void eraceeeprom();

    void setI2cAddress(uint8_t addr);
    void setDeviceMode(uint8_t status);

private:
    I2CSlaveServer * server;
    DallasTemperature * sensors;
    SettingsInternal * config;
    HWiface * hardware;
    Indication * leds;

    void heaterHandler(const float & tempAvg, uint16_t deviceReaded);



};

#endif // CORE_H
