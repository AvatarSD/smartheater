#ifndef HWIFACE_H
#define HWIFACE_H

#include <inttypes.h>

class HWiface
{
public:
    static bool haveSupply();
    static uint16_t humidity();
    static void turnOneWireLineOn();
    static void turnOneWireLineOff();
    static void turnHeaterOn();
    static void turnHeaterOff();
    static void turnLedOff();
    static void turnLedOn();
    static void init();

private:
    HWiface() {}
    HWiface( const HWiface& );
    HWiface& operator=( HWiface& );

};

#endif // HWIFACE_H
