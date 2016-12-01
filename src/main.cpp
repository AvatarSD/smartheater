
#include <hwiface.h>
#include <memory.h>
#include <server.h>
#include <settings.h>
#include <core.h>
#include <usiTwiSlave.h>
#include <usi.h>
#include <DallasTemperature.h>
#include <indication.h>
#include <mainmem.h>


// num of data line pin
#define ONEWIREPIN 3

int main()
{
    auto hardware = HWiface::getInstance();
    hardware->init();
    auto usi = USI::instance();

    OneWire wire(ONEWIREPIN);
    auto eeprommem = Settings::instance();

    Indication leds(*hardware);


    SettingsExternal settingsExt(*eeprommem);
    MemoryMap memory(settingsExt);

    UsiTwiSlave network(usi);
    network.setAddress(I2C_SLAVE_ADDRESS);
    network.setMulticastAddress(MULTICAST_ADDRESS);
    I2CSlaveServer server(&network, &memory);

    DallasTemperature sensors(&wire);

    SettingsInternal settingsInt(*eeprommem);
    CoreLogic logic(server, sensors, settingsInt, *hardware, leds);
    settingsExt.setICoreState(&logic);

    sei();

    while(1)
        logic.mainCycle();

    return 0;
}
