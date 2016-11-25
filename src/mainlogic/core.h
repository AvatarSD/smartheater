#ifndef CORE_H
#define CORE_H


#include <DallasTemperature.h>


class CoreLogic
{
public:
    void mainCycle();
    void searchSensors();
    void eraceeeprom();

    static CoreLogic * instance();

private:
    OneWire wire;
    DallasTemperature sensors;

    void heaterHandler(const float & tempAvg, uint16_t deviceReaded);


    CoreLogic();
    CoreLogic(const CoreLogic &) = default;
    CoreLogic & operator =(const CoreLogic &) = default;
};

#endif // CORE_H
