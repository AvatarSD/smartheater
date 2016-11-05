#ifndef WCONSTANTS
#define WCONSTANTS


#define NULL 0
#define delay(t) {for(auto ms = t; ms>0; ms--) _delay_ms(1);}
#define max(a,b) ((a)>(b)?(a):(b))
#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))


#endif
