/*
 * millis.c
 *
 *  Created on: 6 вер. 2015 р.
 *      Author: sd
 */

#include <WProgram.h>
#include <avr/io.h>
#include <avr/interrupt.h>


#define PERIOD_REQUIRED 0.001
#define PRESCALER CLK_64
#define MODE NORMAL
#define OCR_MODE DISCONNECTED


enum TIMER_CLOCK_SOURSE {
    CLK_NONE = 0,
    CLK_1 = 1,
    CLK_8 = 2,
    CLK_64 = 3,
    CLK_256 = 4,
    CLK_1024 = 5,
    CLK_T0_FALL = 6,
    CLK_T0_RISE = 7
};

enum TIMER_MODE {
    NORMAL = 0,
    CTC = 2,
    PWM_PHCORR_FF = 1,
    PWM_PHCORR_OCR = 5,
    PWM_FAST_FF = 3,
    PWM_FAST_OCR = 7
};

enum TIMER_OCR_MODE {
    DISCONNECTED = 0,
    NON_INVERTED = 2,
    INVERTED = 3
};



ISR(TIMER0_COMPA_vect)
{
    MainTimer::tick();
}


MainTimer::MainTimer()
{
    millisInit();
}

void MainTimer::millisInit()
{
    TCCR0A = ((OCR_MODE&0b11)<<6) | ((OCR_MODE&0b11)<<4) | (MODE&0b11);
    TCCR0B = ((MODE&0b100)<<1) | PRESCALER;
    OCR0A = (uint8_t)(F_CPU*PERIOD_REQUIRED)/PRESCALER; //0x7C;//F9;

    // Timer/Counter 0 Interrupt(s) initialization
    TIMSK = (1<<OCIE0A);
}

uint32_t MainTimer::millis()
{
    return ms;
}

void MainTimer::tick()
{
    ms++;
}

uint32_t MainTimer::ms = 0;
MainTimer MainTimer::instance;
