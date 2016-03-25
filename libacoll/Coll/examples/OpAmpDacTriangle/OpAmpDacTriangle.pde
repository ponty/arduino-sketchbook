/// Triangle wave

#include <WProgram.h>
#include <coll.h>

const int pinDigDacIn = A0;
const int pinAnDacOut = 1;
const int triggerPin = A2;

OpAmpDac dac(pinDigDacIn, pinAnDacOut);

void scopeTrigger()
{
	pinMode(triggerPin, OUTPUT);
	digitalWrite(triggerPin, HIGH);
	digitalWrite(triggerPin, LOW);
}

void triangleWave(int begin, int end, int step, int _delay)
{
	scopeTrigger();
	int i = begin;
	while (1)
	{
		dac.write(i);
		delay(_delay);
		i += step;
		if (i >= end || i <= begin)
			step = step * -1;
	}
}

void setup()
{
	Serial.begin(9600);
	dac.begin();

	Serial.println("start triangle wave");
	triangleWave(0, 444, 5, 0);
}

void loop()
{
}

