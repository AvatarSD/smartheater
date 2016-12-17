
//#include <hwiface.h>
#include <memory.h>
#include <eeprom.h>
#include <core.h>
#include <usiTwiSlave.h>
//#include <DallasTemperature.h>
//#include <indication.h>


// num of data line pin, hardware-defined
#define ONEWIREPIN 3
#define MULTICAST_ADDR 0x50

int main()
{
    auto hardware = HWiface::instance();
    auto settings = Settings::instance();

    OneWire wire(ONEWIREPIN);
    DallasTemperature sensors(&wire);
    Indication leds(hardware);
    BasicAutoHeaterController logic(&sensors, settings, hardware, &leds);
    UsiTwiSlave network(USI::instance(), MULTICAST_ADDR);
    MappedMemory memory(settings, &logic, &network);
    I2CSlaveServer server(&network, &memory);

    hardware->init();
    network.init(settings->getAddress());
    sei();

    while(1)
        logic.doHandle();

    return 0;
}
