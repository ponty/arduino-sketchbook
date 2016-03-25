#include "WProgram.h"
#include "ReadVcc.h"

#if !defined(ADMUX)
#	warning ADMUX is not defined -> no readVcc()
#	define NO_READVCC
#endif

#if !defined(REFS0)
#	warning REFS0 is not defined -> no readVcc()
#	define NO_READVCC
#endif

#if !defined(MUX3)
#	warning MUX3 is not defined -> no readVcc()
#	define NO_READVCC
#endif

#if !defined(MUX2)
#	warning MUX2 is not defined -> no readVcc()
#	define NO_READVCC
#endif

#if !defined(MUX1)
#	warning MUX1 is not defined -> no readVcc()
#	define NO_READVCC
#endif

#if !defined(ADCSRA)
#	warning ADCSRA is not defined -> no readVcc()
#	define NO_READVCC
#endif

#if !defined(ADSC)
#	warning ADSC is not defined -> no readVcc()
#	define NO_READVCC
#endif

#if !defined(ADCL)
#	warning ADCL is not defined -> no readVcc()
#	define NO_READVCC
#endif

#if !defined(ADCH)
#	warning ADCH is not defined -> no readVcc()
#	define NO_READVCC
#endif

#if !defined(NO_READVCC)

// from specification
const uint32_t bandgap_voltage = 1100; //mV

/// Result in mV
/// original source: http://code.google.com/p/tinkerit/wiki/SecretVoltmeter
int16_t readVcc()
{
	int16_t result;
	// Read 1.1V reference against AVcc
	ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
	delay(2); // Wait for Vref to settle
	ADCSRA |= _BV(ADSC); // Convert
	while (bit_is_set(ADCSRA, ADSC))
	;
	result = ADCL;
	result |= ADCH << 8;

	// Back-calculate AVcc in mV
	result = (bandgap_voltage * 1024L) / result;

	return result;
}

/// Calculate error resulting from resolution.
/// Result in mV
int16_t readVccError(int16_t vcc)
{
	return 0.000001 * vcc * vcc;
}

#endif
