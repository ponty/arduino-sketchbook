/// source: http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1273055713

#include <assert.h>

void TestDac::testAll()
{ //Select here what you want to test:
	triangleWave(0, 1000, 100);
	triangleWave(500, 505, 1);
	triangleWave(-5, 10, 1);
	triangleWave(0, 1023, 1023); // measure settlingTime1
	triangleWave(0, 30, 5);
	triangleWave(0, 100, 15); // measure overshoot
	triangleWave(0, 1000, 200, 200); // test leakagecurrent
	rand(); // measure settlingTime2
}

void TestDac::test(int val, int _delay, bool refresh)
{
	int actualVal, error, absError;
	if (!refresh)
	{
		if (!dac.write(val))
			;//Serial << ("DAC fault "); 
	}
	else if (!dac.refresh())
		;//Serial << ("DAC refresh fault ");
	delay(_delay);
	actualVal = dac.read();
	error = actualVal - dac.targetVal;
	absError = abs(error);
	if (absError > maxError)
		maxError = absError;
	if (dac.settlingTime2 > maxSettlingTime2)
		maxSettlingTime2 = dac.settlingTime2;
}

void TestDac::triangleWave(int begin, int end, int step, int _delay)
{
	maxError = 0, maxSettlingTime2 = 0;
	Serial.print("Test triangle wave ");
	Serial.print("    ");
	
	scopeTrigger();
	int i = begin;
	while (i >= begin)
	{
		test(i, _delay);
		i += step;
		if (i >= end)
			step = step * -1;
	}
	printMaxError();
	Serial.print(" Max time2: "); 
	Serial.print(maxSettlingTime2);
	
	Serial.print("  ");Serial.print(begin);
	Serial.print("->");Serial.print(end);
	Serial.print("/");Serial.print(-step);
	Serial.print(" delay=");Serial.print(_delay);

	Serial.println();
}

void TestDac::rand()
{
	maxError = 0, maxSettlingTime2 = 0;
	Serial.print("Test random            ");
	scopeTrigger();
	for (int i = 0; i < 25; i++)
		test(random(1024));
	printMaxError();
	Serial.print(" Max time2: ");
	Serial.print(maxSettlingTime2);
	Serial.println();
}


void TestDac::printMaxError()
{
	Serial.print("Max err: ");
	Serial.print(maxError);
}

void TestDac::scopeTrigger()
{
	const int triggerPin = 4;
	pinMode(triggerPin, OUTPUT);
	digitalWrite(triggerPin, HIGH);
	digitalWrite(triggerPin, LOW);
}

