#ifndef HWIFACE_H
#define HWIFACE_H

#include <inttypes.h>

// heater supply
#define DDR_HEATER DDRB
#define PORT_HEATER PORTB
#define PINNUM_HEATER 4
#define HEATER_OFFSTATE false

// power of status led
#define DDR_LED DDRB
#define PORT_LED PORTB
#define PINNUM_LED 1
#define LED_OFFSTATE true

// analog pin to testing presense of main power(supplying by network)
#define ADC_AC 2
#define DDR_AC DDRB
#define PORT_AC PORTB
#define PINNUM_AC 4
#define TRESHOLD_DATA_AC 80

// OneWire Line Supply
// +5v to sensor
#define DDR_SENS_SUPL DDRB
#define PORT_SENS_SUPL PORTB
#define PINNUM_SENS_SUPL  1
#define SENS_OFFSTATE true

class HWiface
{
public:
    bool haveSupply();
    uint16_t humidity();
    void turnOneWireLineOn();
    void turnOneWireLineOff();
    void turnHeaterOn();
    void turnHeaterOff();
    void turnLedOff();
    void turnLedOn();
    void init();

    static HWiface * getInstance()
    {
        static HWiface hw;
        return &hw;
    }

private:
    HWiface() {}
    HWiface(const HWiface &);
    HWiface & operator=(HWiface &);

};

#endif // HWIFACE_H
