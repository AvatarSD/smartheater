/*
 * ADC.cpp
 *
 *  Created on: 28 вер. 2015 р.
 *      Author: sd
 */

#include "adc.h"
#include "avr/io.h"
#include "avr/interrupt.h"
#include "util/delay.h"



void Analog::init()
{
	ADCSRA = _BV(ADEN);
	ADMUX= 0;
	ADCSRB = 0;
	DIDR0 = 0;
}

uint16_t Analog::read(uint8_t pin)
{
	// mux to selected pin
	ADMUX = (pin & 0b00001111);

	// Delay needed for the stabilization of the ADC input voltage
	_delay_us(2);

	// Start the AD conversion
	ADCSRA|=_BV(ADSC);

	//wait end of conversion
	while(!(ADCSRA&_BV(ADIF)));

	// read the result
	return ADCW;
}
