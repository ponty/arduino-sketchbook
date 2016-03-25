#include <coll.h>

const int pinDigDacIn = A0;
const int pinAnDacOut = 1;
const int target = 123; // 0-1023

OpAmpDac dac(pinDigDacIn, pinAnDacOut);

void setup()
{
	Serial.begin(9600);
	dac.begin();
	
	int16_t vcc = readVcc();
	bool ok = dac.write(target);
	int real = analogRead(pinAnDacOut);
	int min = dac.readMin();
	int max = dac.readMax();
	
	Serial.println();
	Serial.print("OpAmpDac test");
	Serial.println();
	
	Serial.print(" DAC output target=");	Serial.print(target);
	Serial.print(" U=");Serial.print(target/1024.0*vcc);	Serial.print(" mV");
	Serial.println();
	Serial.print(" DAC output real=");	Serial.print(real);
	Serial.print(" U=");Serial.print(real/1024.0*vcc);	Serial.print(" mV");
	Serial.println();
	Serial.print(" success=");	Serial.print(ok);
	Serial.println();
	Serial.print(" VCC=");Serial.print(vcc);	Serial.print(" mV");
	Serial.println();

	Serial.print(" DAC output min=");	Serial.print(min);
	Serial.print(" U=");Serial.print(min/1024.0*vcc);	Serial.print(" mV");
	Serial.println();
	
	Serial.print(" DAC output max=");	Serial.print(max);
	Serial.print(" U=");Serial.print(max/1024.0*vcc);	Serial.print(" mV");
	Serial.println();
}

void loop()
{
	dac.refresh();
}

