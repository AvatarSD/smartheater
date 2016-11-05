
    /*
     * sd@ubuntu:~/projects/avtopoliv/avtoheater/src/temp$ git clone https://github.com/PaulStoffregen/OneWire.git
     */
#include <DallasTemperature.h>


#define ONEWIREPIN 3

int main()
{

    OneWire wire(ONEWIREPIN);
    DallasTemperature sensors(&wire);

    sensors.begin();



	return 0;
}
