#include "WProgram.h"
#include "PinMode.h"
#include "pins_arduino.h"

/// inverse of pinMode()
uint8_t readPinMode(uint8_t pin)
{
	// not a pin
	if (pin==255)
			return 255;

	uint8_t bit = digitalPinToBitMask(pin);
	uint8_t port = digitalPinToPort(pin);
	volatile uint8_t *reg;

	if (port == NOT_A_PIN)
		return 255;

	reg = portModeRegister(port);

	return *reg & bit ? OUTPUT : INPUT;
}

