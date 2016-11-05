#include <WProgram.h>
#include <pins_arduino.h>

#define PIN_TO_BASEREG(pin)             (portInputRegister(digitalPinToPort(pin)))
#define PIN_TO_BITMASK(pin)             (digitalPinToBitMask(pin))
#define DIRECT_MODE_INPUT(base, mask)   ((*((base)+1)) &= ~(mask))
#define DIRECT_MODE_OUTPUT(base, mask)  ((*((base)+1)) |= (mask))
#define DIRECT_WRITE_LOW(base, mask)    ((*((base)+2)) &= ~(mask))
#define DIRECT_WRITE_HIGH(base, mask)   ((*((base)+2)) |= (mask))

void pinMode(uint8_t pin, uint8_t mode)
{
    switch (mode) {
    case INPUT_PULLUP:
        DIRECT_MODE_INPUT(PIN_TO_BASEREG(pin), PIN_TO_BITMASK(pin));
        DIRECT_WRITE_HIGH(PIN_TO_BASEREG(pin), PIN_TO_BITMASK(pin));
        break;
    case OUTPUT:
        DIRECT_MODE_OUTPUT(PIN_TO_BASEREG(pin), PIN_TO_BITMASK(pin));
        DIRECT_WRITE_LOW(PIN_TO_BASEREG(pin), PIN_TO_BITMASK(pin));
        break;
    default:
    case INPUT:
        DIRECT_MODE_INPUT(PIN_TO_BASEREG(pin), PIN_TO_BITMASK(pin));
        DIRECT_WRITE_LOW(PIN_TO_BASEREG(pin), PIN_TO_BITMASK(pin));
        break;
    }

}


