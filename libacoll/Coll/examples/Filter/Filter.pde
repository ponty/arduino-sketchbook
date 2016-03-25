#include <coll.h>

void setup()
{
	Serial.begin(9600);
}

void loop()
{
	Serial.print("filteredAnalogRead=");
	Serial.print(filteredAnalogRead(0, 3, 1000), DEC);

	Serial.print(" analogRead=");
	Serial.print(analogRead(0), DEC);

	Serial.println();

	delay(1000);
}