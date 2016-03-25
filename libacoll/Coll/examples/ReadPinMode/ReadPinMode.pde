#include <coll.h>

const int PIN = 5;

void setup()
{
	Serial.begin(9600);

	Serial.print("testing pin D");
	Serial.print(PIN);
	Serial.println();

	Serial.print(" current pinmode: readPinMode(PIN)=");
	Serial.print(readPinMode(PIN), DEC);
	Serial.println();

	Serial.print(" setting pinmode to OUTPUT");
	Serial.println();
	pinMode(PIN, OUTPUT);

	Serial.print(" current pinmode: readPinMode(PIN)=");
	Serial.print(readPinMode(PIN), DEC);
	Serial.println();

	Serial.print(" setting pinmode to INPUT");
	Serial.println();
	pinMode(PIN, INPUT);

	Serial.print(" current pinmode: readPinMode(PIN)=");
	Serial.print(readPinMode(PIN), DEC);
	Serial.println();

	Serial.println();
}

void loop()
{
}