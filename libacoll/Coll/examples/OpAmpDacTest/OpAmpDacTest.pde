/// source: http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1273055713

/// DacDemo.pde is the test program which is used to test the application.  
/// As a good practise, the libraries Streaming.h and Flash.h should always be used. 
/// Download these libraries from Mikal Hart here: http://arduiniana.org.

#include <WProgram.h>
#include <coll.h>
#include "TestDac.h"

const int pinDigDacIn = A0;
const int pinAnDacOut = 1;

OpAmpDac dac(pinDigDacIn, pinAnDacOut);
TestDac testDac;

void setup()
{
	Serial.begin(9600);
	dac.begin();
	randomSeed(0);
}

void loop()
{
	testDac.testAll();
	Serial.println();
	delay(5000);
}

