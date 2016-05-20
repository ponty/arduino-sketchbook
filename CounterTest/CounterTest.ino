#define PIN_KEY1    3
#define PIN_KEY2    4
#define PIN_KEY3    5

#define PIN_OUT1    7

int g_pushed = 0;

#define PULSE_WIDTH    100

void pulse(int n)
{
	for (int i = 0; i < n; i++)
	{
		digitalWrite(PIN_OUT1, HIGH);
		delay(PULSE_WIDTH);
		digitalWrite(PIN_OUT1, LOW);
		delay(PULSE_WIDTH);
	}
}

void setup()
{
	//Initialize serial and wait for port to open:
	Serial.begin(9600);
	Serial.println("start");

	pinMode(PIN_KEY1, INPUT);
	pinMode(PIN_KEY2, INPUT);
	pinMode(PIN_KEY3, INPUT);

	pinMode(PIN_OUT1, OUTPUT);
}

void loop()
{
	int b1 = digitalRead(PIN_KEY1);
	int b2 = digitalRead(PIN_KEY2);
	int b3 = digitalRead(PIN_KEY3);

	int none = !b1 && !b2 && !b3;
	if (g_pushed)
	{
		if (none)
		{
			g_pushed = 0;
			Serial.println("released");
		}
	}
	else
	{
		if (!none)
		{
			g_pushed = 1;

			if (b1)
			{
				pulse(1);
			}
			else if (b2)
			{
				pulse(10);
			}
			else if (b3)
			{
				pulse(100);
			}
			Serial.println("pressed");
		}

	}

	delay(1);
}
