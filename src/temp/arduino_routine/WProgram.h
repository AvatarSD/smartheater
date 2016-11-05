#ifndef WPROGRAM_h
#define WPROGRAM_h


#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <inttypes.h>


#define noInterrupts() cli()
#define interrupts() sei()

#define delayMicroseconds(t) _delay_us(t)

#define INPUT 0x0
#define OUTPUT 0x1
#define INPUT_PULLUP 0x2
void pinMode(uint8_t pin, uint8_t mode);



class MainTimer
{
public:
	static uint32_t millis();

	static void tick();

private:
	MainTimer();
	MainTimer( const MainTimer& );
	MainTimer& operator=( MainTimer& );

	static void millisInit();

	static uint32_t ms ;
	static MainTimer instance;
};

#define millis() MainTimer::millis()




#endif


