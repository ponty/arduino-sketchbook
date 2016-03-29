

//    double pwmarr[] =
//    { 62500.0, 7812.5, 976.5625, 244.140625, 61.03515625 };

void pwm_test_setup()
{
    pinMode(5, OUTPUT);
    analogWrite(5, 128);
}

void pwm_test_loop()
{
	static int xi = 0;
	static int mode = 2;

    xi++;
    if (xi > 5)
    {
        xi = 0;
		mode++;
		if (mode > 5)
		{
			mode = 2;
		}
		TCCR0B = TCCR0B & 0b11111000 | mode;
    }

}

