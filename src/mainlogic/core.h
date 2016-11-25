#ifndef CORE_H
#define CORE_H


#include <DallasTemperature.h>


class CoreLogic
{
public:
    CoreLogic();
    void mainCycle();
    void searchSensors();
    void eraceeeprom();

private:
    OneWire wire;
    DallasTemperature sensors;

};

#endif // CORE_H
