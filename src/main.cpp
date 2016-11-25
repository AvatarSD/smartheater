
#include <hwiface.h>
#include <server.h>
#include <settings.h>
#include <core.h>




int main()
{
    HWiface::init();

    I2CSlaveServer * server = I2CSlaveServer::getInstance();
    server->setup(settingsinternal::getI2cAddress());

    CoreLogic * logic = CoreLogic::instance(server);

    sei();

    while(1)
        logic->mainCycle();

    return 0;
}
