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
//#define OUTPUT 0x1
//#define INPUT_PULLUP 0x2

#define digitalPinToPort(P) (P/8)
#define digitalPinToBitMask(P) (_BV((P%8)))

#define portInputRegister(P) (volatile uint8_t*)(&PINB)
//#define portOutputRegister(P) (volatile uint8_t*)(&PORTB)
//#define portModeRegister(P) (volatile uint8_t*)(&DDRB)

void pinMode(uint8_t dPin, uint8_t mode);
unsigned long millis();

#define NULL 0

#define delay(t) {for(auto ms = t; ms>0; ms--) _delay_ms(1);}

#define max(a,b) ((a)>(b)?(a):(b))
#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))

#endif
