// Ford
// Immunity to Voltage Dropout: CI 260

const int pinSwitchHigh = 2;
//const int pinDiag = 3;
const int pinSwitchLow = 4;

void setup()
{
	Serial.begin(115200);
	Serial.println(F("Voltage Dropout: CI 260"));

	pinMode(pinSwitchLow, OUTPUT);
	digitalWrite(pinSwitchLow, LOW);

	pinMode(pinSwitchHigh, OUTPUT);
	digitalWrite(pinSwitchHigh, LOW);
}

void switchAB(boolean on)
{
	if (on)
	{
		digitalWrite(pinSwitchLow, LOW);
		digitalWrite(pinSwitchHigh, HIGH);
	}
	else
	{
		digitalWrite(pinSwitchHigh, LOW);
		digitalWrite(pinSwitchLow, HIGH);
	}

}
void switchF(boolean on)
{
	digitalWrite(pinSwitchLow, LOW);
	if (on)
	{
		digitalWrite(pinSwitchHigh, HIGH);
	}
	else
	{
		digitalWrite(pinSwitchHigh, LOW);
	}

}
// T - *100 us

void delay2(int T)
{
	if (T >= 10)
	{
		delay(T / 10);
	}
	else
	{
		delayMicroseconds(T * 100);
	}
}

void run_waveformA(int T)
{
	Serial.print(F("A:"));
	Serial.print(T);
	Serial.print(F("00 us"));
	Serial.println();

	for (int i = 0; i < 10; i++)
	{
		switchAB(false);
		delay2(T);
		switchAB(true);
		delay2((10 - i) * T);
	}
	switchAB(false);
	delay2(T);
	switchAB(true);

}

void run_waveformB(int T)
{
	Serial.print(F("B:"));
	Serial.print(T);
	Serial.print(F("00 us"));
	Serial.println();

	for (int i = 0; i < 10; i++)
	{
		switchAB(false);
		delay2((10 - i) * T);
		switchAB(true);
		delay2(T);
	}
}

void run_noise()
{
	int tnoise = 1000;
	int sumr = 0;
	int r;
	while (1)
	{
		switchF(false);

		r = random(50) + 1; // 1..50
		r = 50 / r; // 1..50
		delay2(r); // 1us..5ms
		sumr += r;
		if (sumr >= tnoise)
		{
			break;
		}

		switchF(true);

		r = random(50) + 1; // 1..50
		r = 50 / r; // 1..50
		delay2(r); // 1us..5ms
		sumr += r;
		if (sumr >= tnoise)
		{
			break;
		}
	}

	switchF(true);
}

void run_waveformF()
{
	Serial.print(F("F:10 sec"));
	Serial.println();

	run_noise();
	delay(2400);
	run_noise();
	delay(2400);

	switchF(false);
	delay(5000);

}
void run_waveform(char waveform, int T)
{
	switch (waveform)
	{
	case 'A':
		run_waveformA(T);
		break;

	case 'B':
		run_waveformB(T);
		break;

	case 'F':
		run_waveformF();
		break;
	}
}

void waveform_repetition(char waveform, int T, int repeat, int Tdelay)
{
	for (int i = 0; i < repeat; i++)
	{
		run_waveform(waveform, T);
		delay(Tdelay);
	}
}

int Tdelay = 1000;
int repeat = 3;
char waveform = '0';
int T = 0;

char warr[] =
{ 'A', 'B', 'F' };
int Tarr[] =
{ 1, 3, 5, 20, 50, 100, 300, 500 };

#define NELEMS(x)  (sizeof(x) / sizeof((x)[0]))
void loop()
{

	if (waveform == '0')
	{
		// all waveforms
		for (int w = 0; w < NELEMS(warr); w++)
		{
			if (T == 0 && warr[w] != 'F')
			{
				// all T
				for (int t = 0; t < NELEMS(Tarr); t++)
				{
					waveform_repetition(warr[w], Tarr[t], repeat, Tdelay);
				}
			}
			else
			{
				waveform_repetition(warr[w], T, repeat, Tdelay);
			}
		}
	}
	else
	{
		if (T == 0 && waveform != 'F')
		{
			// all T
			for (int t = 0; t < NELEMS(Tarr); t++)
			{
				waveform_repetition(waveform, Tarr[t], repeat, Tdelay);
			}
		}
		else
		{
			waveform_repetition(waveform, T, repeat, Tdelay);
		}
	}

}
