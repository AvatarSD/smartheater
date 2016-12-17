/*
 *  AvatarSD
 *  avatarsd.com
 */

#include <memory.h>
#include <core.h>
#include <usiTwiSlave.h>


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
    UsiTwiSlave network(USI::instance(), settings, MULTICAST_ADDR);
    MappedMemory memory(settings, &logic, &network);
    I2CSlaveServer server(&network, &memory);

    hardware->init();
    network.init();
    sei();

    while(1)
        logic.doHandle();

    return 0;
}
