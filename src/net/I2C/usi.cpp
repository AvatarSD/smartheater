#include "usi.h"
#include <avr/interrupt.h>

//data regs:
volatile uint8_t & data = USIDR;
volatile uint8_t & buffer = USIBR;

void (*USI::startConditionHandler)();
void(*USI::overflowHandler)();

ISR(USI_START_VECTOR)
{
    if(USI::startConditionHandler)
        USI::startConditionHandler();
}

ISR(USI_OVERFLOW_VECTOR)
{
    if(USI::overflowHandler)
        USI::overflowHandler();
}
