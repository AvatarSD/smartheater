#ifndef CORE_H
#define CORE_H


#include <DallasTemperature.h>
#include <server.h>


class CoreLogic
{
public:
    void mainCycle();
    void searchSensors();
    void eraceeeprom();

    static CoreLogic * instance(I2CSlaveServer * server);
    static CoreLogic * instance();

    I2CSlaveServer * getServer() const;

private:
    static CoreLogic * logic;

    OneWire wire;
    DallasTemperature sensors;
    I2CSlaveServer * server;

    void heaterHandler(const float & tempAvg, uint16_t deviceReaded);


    CoreLogic(I2CSlaveServer * server);
    CoreLogic(const CoreLogic &) = default;
    CoreLogic & operator =(const CoreLogic &) = default;
};

#endif // CORE_H
