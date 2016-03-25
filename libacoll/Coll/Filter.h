#pragma once

#include "WProgram.h"

uint16_t filteredAnalogRead(uint8_t analogPin, uint8_t n = 3,
		uint16_t timeout = 255);

