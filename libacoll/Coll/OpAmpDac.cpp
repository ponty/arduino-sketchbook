/* Simple 10 bit DAC for the Arduino

 The settling time is max. 20ms.
 Don't touch C1 / R1 during run.

 //               C1 100nF 10% MKT
 //                _____||____
 //               |     ||    |  (VDD=5V, GND=0V)
 //           R1  |  |\       |
 //     -----56k-----|- \     |
 //pinDigDacIn       |    \ __|____ DAC out 0 ... 5V
 //           R2     |    /  |
 //     5V --10k-----|+ /    |
 //               |  |/      |
 //               |          |
 //           R3 10k         |
 //               |          |
 //               |          |
 //              GND         pinAnDacOut
 //
 //            5V |      _       _
 //               |     | |     | |
 //  I/O 2        |     | |     | |
 //          2.5V |_____| |_____| |______
 //               |
 //               |
 //            0V |______________________
 //
 //               |______
 //               |      \_______
 // DAC out       |              \_______
 // (not to scale)|
 //               |______________________

 LM358
 0-3.5V  load=0
 0-2.5V  load=100 ohm

 */
/// source: http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1273055713

#include <WProgram.h>
#include "OpAmpDac.h"

OpAmpDac::OpAmpDac(const byte pinDigDacIn, const byte pinAnDacOut,
		const byte _overshoot) :
	dacUpdownPin(pinDigDacIn), UDacPin(pinAnDacOut), overshoot(_overshoot)
{
	// break simulator
	//	write(512); // set to 2,5V
	//	write(512); // the first conversion can be wrong
}

void OpAmpDac::begin()
{
	write(512); // set to 2,5V
	write(512); // the first conversion can be wrong
}

/// Write a value to the DAC. 0 = 0V, 1023 = 5V.
bool OpAmpDac::write(int val)
{
	targetVal = val;
	if (targetVal > 1023)
		targetVal = 1023;
	if (targetVal < 0)
		targetVal = 0;

	if (abs(read() - targetVal) > overshoot) // avoid overshoot from setDac() for small value changes
		if (!setDac())
			return false;
	if (!fineTune())
		return false;
	if (abs(read() - targetVal) > 1)
		return false; // final error check
	return true;
}

/// Because of leakage current, refresh the DAC periodically (10 sec. for 1 LSB error).
bool OpAmpDac::refresh()
{
	if (!fineTune())
		return false;
	return true;
}

/// Read the actual output voltage.
int OpAmpDac::read() const// not inline
{
	return analogRead(UDacPin);
}

/// Read the actual output voltage.
//inline int OpAmpDac::fastRead() const
//{
//	return analogRead(UDacPin);
//}

bool OpAmpDac::setDac()
{
	const byte timeout1(255); // maxSettlingTime1 = 195
	int targetCorr;

	if (read() == targetVal)
		return true;
	if (read() < targetVal)
	{
		targetCorr = targetVal - overshoot; // reduce overshoot caused by adc delay
		dacUp();
		for (settlingTime1 = 0; settlingTime1 < timeout1; settlingTime1++)
		{
			if (read() >= targetCorr)
			{
				dacHold();
				break;
			}
		}
	}
	else
	{
		targetCorr = targetVal + overshoot;
		dacDown();
		for (settlingTime1 = 0; settlingTime1 < timeout1; settlingTime1++)
		{
			if (read() <= targetCorr)
			{
				dacHold();
				break;
			}
		}
	}
	dacHold(); // end always with hold, in case of timeout
	if (settlingTime1 >= timeout1)
		return false;
	else
		return true;
}

bool OpAmpDac::fineTune() // produces no overshoot
{
	const byte timeout2(80); // maxSettlingTime2 ~ 20
	const byte halfLsbCorrection(1);

	if (read() == targetVal)
		return true; // avoid ripple at refresh()
	if (read() < targetVal)
	{
		for (settlingTime2 = 0; settlingTime2 < timeout2; settlingTime2++)
		{
			dacUp();
			dacHold(); // finetuning with short pulse
			if (read() >= targetVal)
			{
				for (int i = 0; i < halfLsbCorrection; i++)
					dacUp(); // reduce error to 0
				break;
			}
		}
	}
	else
	{
		for (settlingTime2 = 0; settlingTime2 < timeout2; settlingTime2++)
		{
			dacDown();
			dacHold(); // finetuning with short pulse
			if (read() <= targetVal)
			{
				for (int i = 0; i < halfLsbCorrection; i++)
					dacDown(); // reduce error to 0
				break;
			}
		}
	}
	dacHold(); // end always with hold, in case of timeout
	if (settlingTime2 >= timeout2)
		return false;
	else
		return true;
}

void OpAmpDac::dacUp() const
{
	digitalWrite(dacUpdownPin, LOW);
	pinMode(dacUpdownPin, OUTPUT);
}
void OpAmpDac::dacDown() const
{
	digitalWrite(dacUpdownPin, HIGH);
	pinMode(dacUpdownPin, OUTPUT);
}

void OpAmpDac::dacHold() const
{
	pinMode(dacUpdownPin, INPUT); // high impedance tristate
	digitalWrite(dacUpdownPin, LOW); // disable pull up resistor 1*)
}

