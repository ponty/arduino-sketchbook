/*
 Simple example for receiving
 
 https://github.com/sui77/rc-switch/
 */

#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();

void setup()
{
	Serial.begin(9600);
	mySwitch.enableReceive(0);  // Receiver on interrupt 0 => that is pin #2
	Serial.println("scanning..");
}

void loop()
{
	if (mySwitch.available())
	{

		int value = mySwitch.getReceivedValue();

		if (value == 0)
		{
			Serial.print("Unknown encoding");
		}
		else
		{
			Serial.print("0x");
			Serial.print(mySwitch.getReceivedValue(), HEX);
			Serial.print(" / ");
			Serial.print(mySwitch.getReceivedBitlength());
			Serial.print("bit ");
			Serial.print("Protocol: ");
			Serial.print(mySwitch.getReceivedProtocol());
			Serial.print(" PulseLength: ");
			Serial.print(mySwitch.getReceivedDelay());
			Serial.print(" microseconds");

			Serial.println();
		}

		mySwitch.resetAvailable();
	}
}
