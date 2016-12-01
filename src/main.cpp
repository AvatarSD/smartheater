
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
    I2CSlaveServer server(&memory, &network, I2C_SLAVE_ADDRESS, MULTICAST_ADDRESS);

    DallasTemperature sensors(&wire);

    SettingsInternal settingsInt(*eeprommem);
    CoreLogic logic(server, sensors, settingsInt, *hardware, leds);
    settingsExt.setICoreState(&logic);

    sei();

    while(1)
        logic.mainCycle();

    return 0;
}
