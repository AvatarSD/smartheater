/*
 * ADC.h
 *
 *  Created on: 28 вер. 2015 р.
 *      Author: sd
 */

#ifndef ADC_ANALOG_H_
#define ADC_ANALOG_H_

#include <inttypes.h>

class Analog
{
public:
	static void init();
	static uint16_t read(uint8_t pin);

private:
	Analog() {}
	Analog( const Analog& );
	Analog& operator=( Analog& );
};



#endif /* ADC_ANALOG_H_ */
