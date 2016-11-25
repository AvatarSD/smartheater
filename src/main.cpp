
#include <hwiface.h>
#include <server.h>

#include <core.h>




int main()
{
    HWiface::init();

    I2CSlaveServer * server = I2CSlaveServer::getInstance();
    server->setup();

    CoreLogic * logic = CoreLogic::instance();

    sei();

    while(1)
        logic->mainCycle();

    return 0;
}
