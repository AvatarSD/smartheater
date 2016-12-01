#include "hwiface.h"
#include <avr/io.h>
#include <util/delay.h>
#include "ADC/adc.h"
//#include <conf.h>

#include <avr/interrupt.h>


#define setbit(port,num,onoff) port=(onoff<<num)|(port&~(1<<num))

void HWiface::init()
{
    turnHeaterOff();
    Analog::init();

}


bool HWiface::haveSupply()
{
    uint8_t ddrBackup = DDR_AC;
    DDR_AC &= ~_BV(PINNUM_AC);

    _delay_us(10);

    uint16_t adcData = Analog::read(ADC_AC);

    bool ret_val;
    if(adcData > TRESHOLD_DATA_AC)
        ret_val = true;
    else
        ret_val = false;

    DDR_AC = ddrBackup;

    return ret_val;
}

void HWiface::turnOneWireLineOn()
{
    DDR_SENS_SUPL |= _BV(PINNUM_SENS_SUPL);
    setbit(PORT_SENS_SUPL, PINNUM_SENS_SUPL, !SENS_OFFSTATE);
}

void HWiface::turnOneWireLineOff()
{
    DDR_SENS_SUPL |= _BV(PINNUM_SENS_SUPL);
    setbit(PORT_SENS_SUPL, PINNUM_SENS_SUPL, SENS_OFFSTATE);
}

void HWiface::turnHeaterOn()
{
    DDR_HEATER |= _BV(PINNUM_HEATER);
    PORT_HEATER |= _BV(PINNUM_HEATER);
}

void HWiface::turnHeaterOff()
{
    DDR_HEATER |= _BV(PINNUM_HEATER);
    PORT_HEATER &= ~_BV(PINNUM_HEATER);
}

// todo hardware led
//void HWiface::turnLedOff()
//{
//    DDR_LED &= ~_BV(PINNUM_LED);
//}

//void HWiface::turnLedOn()
//{
//    DDR_LED |= _BV(PINNUM_LED);
//    setbit(PORT_LED, PINNUM_LED, !LED_OFFSTATE);
//}
