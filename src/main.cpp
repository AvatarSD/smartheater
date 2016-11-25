
#include <hwiface.h>
#include <server.h>

#include <core.h>




int main()
{
    HWiface::init();


    I2CSlaveServer * server = I2CSlaveServer::getInstance();
    server->setup();

    CoreLogic logic;

    sei();

    logic.searchSensors();

    while(1)
        logic.mainCycle();

    return 0;
}
