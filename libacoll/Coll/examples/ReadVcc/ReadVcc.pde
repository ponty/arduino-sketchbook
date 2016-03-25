#include <coll.h>

void setup()
{
	Serial.begin(9600);
}

void loop()
{
	int16_t vcc = readVcc();
	Serial.print("VCC=");
	Serial.print(vcc, DEC);
	Serial.print(" mV");
	Serial.print(" error=");
	Serial.print(readVccError(vcc), DEC);
	Serial.print(" mV");

	Serial.println();

	delay(1000);
}