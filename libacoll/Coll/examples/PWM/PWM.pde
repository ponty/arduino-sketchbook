#include <coll.h>

void setup()
{
	Serial.begin(9600);

	// Set pin 9's PWM frequency to 3906 Hz (31250/8 = 3906)
	// Note that the base frequency for pins 3, 9, 10, and 11 is 31250 Hz
	setPwmFrequency(9, 8);
	Serial.print("PWM frequency 31250/8=");
	Serial.println(getPwmFrequency(9, 8));

	// Set pin 6's PWM frequency to 62500 Hz (62500/1 = 62500)
	// Note that the base frequency for pins 5 and 6 is 62500 Hz
	setPwmFrequency(6, 1);
	Serial.print("PWM frequency 62500/1=");
	Serial.println(getPwmFrequency(6, 1));

	// Set pin 10's PWM frequency to 31 Hz (31250/1024 = 31)
	setPwmFrequency(10, 1024);
	Serial.print("PWM frequency 31250/1024=");
	Serial.println(getPwmFrequency(10, 1024));
}

void loop()
{
}