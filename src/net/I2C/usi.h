#ifndef USI_H
#define USI_H

#include <avr/io.h>
#include <inttypes.h>
#include "pindefs.h"

extern "C" {

    enum WireMode {
        NORMAL = 0b00,
        SPI = 0b01,
        TWI = 0b10,
        TWI_WAIT = 0b11
    };

    enum ClockMode {
        NONE = 0b000,
        SOFTWARE = 0b001,
        TIMER0_CMP = 0b010,
        EXT_POS = 0b100, // for i2c slave
        EXT_NEG = 0b110,
        EXT_POS_SOFT_CONT = 0b101,
        EXT_NEG_SOFT_CONT = 0b111
    };


    namespace USI
    {

    //isr's
    extern void (*startConditionHandler)();
    extern void(*overflowHandler)();

    extern volatile uint8_t & data;
    extern volatile uint8_t & buffer;

    //status
    inline void setStatus(bool start,
                          bool ovf,
                          bool stop,
                          bool collision,
                          uint8_t counter)
    {
        USISR = (start << USISIF)
                | (ovf << USIOIF)
                | (stop << USIPF)
                | (collision << USIDC)
                | ((counter & 0b1111) << USICNT0);
    }

    inline bool getStartCondIntFlag()
    {
        return USISR & _BV(USISIF);
    }
    inline void clearStartCondIntFlag()
    {
        USISR |= _BV(USISIF);
    }
    inline void releaseStartCondLock()
    {
        USISR &= ~_BV(USISIF);
    }
    inline bool getOvfIntFlag()
    {
        return USISR & _BV(USIOIF);
    }
    inline void clearOvfIntFlag()
    {
        USISR |= _BV(USIOIF);
    }
    inline void releaseOvfCondLock()
    {
        USISR &= ~_BV(USIOIF);
    }

    inline bool getStopCondIntFlag()
    {
        return USISR & _BV(USIPF);
    }
    inline void clearStopCondIntFlag()
    {
        USISR |= _BV(USIPF);
    }

    inline bool haveCollision()
    {
        return USISR & _BV(USIDC);
    }
    inline void clearCollisionFlag()
    {
        USISR |= _BV(USIDC);
    }

    inline uint8_t counterValue()
    {
        return USISR & 0b1111;
    }
    inline void setCounterValue(uint8_t val)
    {
        USISR = (USISR & ~0b00001111) | (val & 0b1111);
    }

    //control register
    inline void enableStartInt()
    {
        USICR |= _BV(USISIE);
    }
    inline void disableStartInt()
    {
        USICR &= ~_BV(USISIE);
    }

    inline void enableOvfInt()
    {
        USICR |= _BV(USIOIE);
    }
    inline void disableOvfInt()
    {
        USICR &= ~_BV(USIOIE);
    }

    inline void setWireMode(WireMode mode)
    {
        USICR = ((USICR & ~0b110000) | (((uint8_t)mode) << 4)) ;
    }
    inline WireMode getWireMode()
    {
        return (WireMode)((USICR >> 4) & 0b11);
    }

    inline void setClockMode(ClockMode mode)
    {
        USICR = (USICR & ~0b1110) | ((((uint8_t)mode) << 1) & 0b1110);
    }
    inline ClockMode getClockMode()
    {
        return (ClockMode)((USICR >> 1) & 0b111);
    }

    inline void shiftClockStrobeCounter()
    {
        USICR |= _BV(USICLK);
    }
    inline void toggleClockPortPin()
    {
        USICR |= _BV(USITC);
    }

    //direct accses to pin's
    inline void enableSDAOpenDrain()
    {
        DDR_USI |= _BV(PORT_USI_SDA);
    }
    inline void disableSDAOpenDrain()
    {
        DDR_USI &= ~ _BV(PORT_USI_SDA);
    }
    inline bool getSDAState()
    {
        return PIN_USI & (1 << PIN_USI_SDA);
    }
    inline void enableForceHoldSDA()
    {
        PORT_USI &= ~ _BV(PORT_USI_SDA);
    }
    inline void disableForceHoldSDA()
    {
        PORT_USI |= _BV(PORT_USI_SDA);
    }
    inline void enableSCLOpenDrain()
    {
        DDR_USI |= _BV(PORT_USI_SCL);
    }
    inline void disableSCLOpenDrain()
    {
        DDR_USI &= ~ _BV(PORT_USI_SCL);
    }
    inline bool getSCLState()
    {
        return PIN_USI & (1 << PIN_USI_SCL);
    }
    inline void enableForceHoldSCL()
    {
        PORT_USI &= ~ _BV(PORT_USI_SCL);
    }
    inline void disableForceHoldSCL()
    {
        PORT_USI |= _BV(PORT_USI_SCL);
    }

    }
}
#endif
