
#include <hwiface.h>
#include <memory.h>
#include <settings.h>
#include <core.h>
#include <usiTwiSlave.h>
#include <usi.h>
#include <DallasTemperature.h>
#include <indication.h>


// num of data line pin, hardware-defined
#define ONEWIREPIN 3

int main()
{
    auto hardware = HWiface::getInstance();
    hardware->init();
    auto usi = USI::instance();

    OneWire wire(ONEWIREPIN);
    auto eeprommem = Settings::instance();

    SettingsExternal settingsExt(*eeprommem);
    MemoryMap memory(settingsExt);

    SettingsInternal settingsInt(*eeprommem);

    UsiTwiSlave network(usi, MULTICAST_ADDR);
    I2CSlaveServer server(&network, &memory);
    network.init(settingsInt.getI2cAddress());

    DallasTemperature sensors(&wire);

    Indication leds(*hardware);
    CoreLogic logic(server, sensors, settingsInt, *hardware, leds);
    settingsExt.setICoreState(&logic);

    sei();

    while(1)
        logic.mainCycle();

    return 0;
}
