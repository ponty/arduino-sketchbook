#include "WProgram.h"
#include "Filter.h"

/// Good for noisy AVCC, can remove pulses.
/// Wait until 'n' identical sample.
/// After 'timeout' sample return -1.
uint16_t filteredAnalogRead(uint8_t analogPin, uint8_t n, uint16_t timeout)
{
	uint8_t counter = 0;
	uint16_t last = -1; // invalid

	// mode filter
	for (uint16_t i = 0; i < timeout; i++)
	{
		uint16_t x = analogRead(analogPin);
		if (x == last)
		{
			counter++;
			if (counter >= n)
				return x;
		}
		else
		{
			counter = 0;
			last = x;
		}
	}
	return -1;
}

