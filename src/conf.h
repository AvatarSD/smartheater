#ifndef CONF_H
#define CONF_H


// heater supply
#define DDR_HEATER DDRB
#define PORT_HEATER PORTB
#define PINNUM_HEATER 4
#define HEATER_OFFSTATE false

// power of status led
#define DDR_LED DDRB
#define PORT_LED PORTB
#define PINNUM_LED 1
#define LED_OFFSTATE true

// analog pin to testing presense of main power(supplying by network)
#define ADC_AC 2
#define DDR_AC DDRB
#define PORT_AC PORTB
#define PINNUM_AC 4
#define TRESHOLD_DATA_AC 80

// OneWire Line Supply
// +5v to sensor
#define DDR_SENS_SUPL DDRB
#define PORT_SENS_SUPL PORTB
#define PINNUM_SENS_SUPL  1
#define SENS_OFFSTATE true
// num of data line pin
#define ONEWIREPIN 3

//settings
#define MAX_SENSORS 20
#define REQUIRED_TEMP 20
#define I2C_SLAVE_ADDRESS 0x04 // TODO for test

#define MULTICAST_ADDRESS 0x03


enum SensorStatusEnum {
	NoAvailable = 0x00,
	Disable = 0x01,
	Active = 0x02,
	NoResponse = 0x03
};

enum HeaterMode {
	DisableHeater = 0b00,
	EnableHeater = 0b01,
	Auto = 0b10
};

enum SearchMode{
	Search = 0b100,
	Erace = 0b1000
};

#endif // CONF_H
