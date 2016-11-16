/**
 * Example sketch for writing to and reading from a slave in transactional
 * manner
 *
 * NOTE: You must not use delay() or I2C communications will fail, use
 * tws_delay() instead (or preferably some smarter timing system)
 *
 * On write the first byte received is considered the register addres to
 * modify/read
 * On each byte sent or read the register address is incremented (and it will
 * loop back to 0)
 *
 * You can try this with the Arduino I2C REPL sketch at
 * https://github.com/rambo/I2C/blob/master/examples/i2crepl/i2crepl.ino
 * If you have bus-pirate remember that the older revisions do not like the
 * slave streching the clock, this leads to all sorts of weird behaviour
 *
 * To read third value (register number 2 since counting starts at 0) send "[ 8
 * 2 [ 9 r ]", value read should be 0xBE
 * If you then send "[ 9 r r r ]" you should get 0xEF 0xDE 0xAD as response
 * (demonstrating the register counter looping back to zero)
 *
 * You need to have at least 8MHz clock on the ATTiny for this to work (and in
 * fact I have so far tested it only on ATTiny85 @8MHz using internal
 * oscillator)
 * Remember to "Burn bootloader" to make sure your chip is in correct mode
 */

/**
 * Pin notes by Suovula, see also http://hlt.media.mit.edu/?p=1229
 *
 * DIP and SOIC have same pinout, however the SOIC chips are much cheaper,
especially if you buy more than 5 at a time
 * For nice breakout boards see https://github.com/rambo/attiny_boards
 *
 * Basically the arduino pin numbers map directly to the PORTB bit numbers.
 *
// I2C
arduino pin 0 = not(OC1A) = PORTB <- _BV(0) = SOIC pin 5 (I2C SDA, PWM)
arduino pin 2 =           = PORTB <- _BV(2) = SOIC pin 7 (I2C SCL, Analog 1)
// Timer1 -> PWM
arduino pin 1 =     OC1A  = PORTB <- _BV(1) = SOIC pin 6 (PWM)
arduino pin 3 = not(OC1B) = PORTB <- _BV(3) = SOIC pin 2 (Analog 3)
arduino pin 4 =     OC1B  = PORTB <- _BV(4) = SOIC pin 3 (Analog 2)
 */

#include "server.h"
#include <TinyWireS.h>

uint8_t I2C_SLAVE_ADDRESS = 0x4;

volatile uint8_t i2c_regs[] = /*{
    0xDE,
    0xAD,
    0xBE,
    0xEF,
    'S',
    '.',
    'D',
    '.'
};*/
{
    "When switching between tri-state ({DDxn, PORTxn} = 0b00) and output high "
    "({DDxn, PORTxn} = 0b11), an intermediate state with either pull-up enabled "
    "{DDxn, PORTxn} = 0b01) or output low ({DDxn, PORTxn} = 0b10) must occur."

};

// Tracks the current register pointer position
volatile uint16_t reg_position;
const uint16_t reg_size = sizeof(i2c_regs);

USI_TWI_S TinyWireS;

/**
 * This is called for each read request we receive, never put more than one byte
 * of data (with TinyWireS.send) to the
 * send-buffer when using this callback
 */
void requestEvent()
{
    TinyWireS.send(i2c_regs[reg_position]);
    // Increment the reg position on each read, and loop back to zero
    reg_position++;
    if(reg_position >= reg_size)
        reg_position = 0;
}

/**
 * The I2C data received -handler
 *
 * This needs to complete before the next incoming transaction (start, data,
 * restart/stop) on the bus does
 * so be quick, set flags for long running tasks to be called from the mainloop
 * instead of running them directly,
 */
void receiveEvent(uint8_t howMany)
{
    if(howMany < 1)
    {
        // Sanity-check
        return;
    }
    if(howMany > TWI_RX_BUFFER_SIZE)
    {
        // Also insane number
        return;
    }
    reg_position = TinyWireS.receive();
    howMany--;
    if(!howMany)
    {
        // This write was only to set the buffer for next read
        return;
    }
    while(howMany--)
    {
        i2c_regs[reg_position] = TinyWireS.receive();
        reg_position++;
        if(reg_position >= reg_size)
            reg_position = 0;
    }
}

void setup()
{
    TinyWireS.begin(I2C_SLAVE_ADDRESS);
    TinyWireS.onReceive(receiveEvent);
    TinyWireS.onRequest(requestEvent);
}

void loop()
{
    /**
     * This is the only way we can detect stop condition
     * (http://www.avrfreaks.net/index.php?name=PNphpBB2&file=viewtopic&p=984716&sid=82e9dc7299a8243b86cf7969dd41b5b5#984716)
     * it needs to be called in a very tight loop in order not to miss any
     * (REMINDER: Do *not* use delay() anywhere, use tws_delay() instead).
     * It will call the function registered via TinyWireS.onReceive(); if there is
     * data in the buffer on stop.
     */
    TinyWireS_stop_check();
}