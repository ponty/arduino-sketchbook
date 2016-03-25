#include "Arduino.h"

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include "hw_config.h"
#include "sw_config.h"

// http://www.pjrc.com/teensy/td_libs_FreqCount.html
#include <FreqCount.h>
#include <FreqMeasure.h>

//#include "freq.h"

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, NEGATIVE);  // Set the LCD I2C address

const bool PWM_TEST = false;


void freq_setup()
{
    FreqCount.begin(1000);
}

float read_freq(uint16_t gate_time)
{
    FreqCount.begin(gate_time);
    while (!FreqCount.available())         // wait until counter ready
    {
    }
    uint32_t f = FreqCount.read();            // read result
    FreqCount.end();
    return f;            // read result

}

void setup()
{
    if (PWM_TEST)
    {
        pinMode(5, OUTPUT);
        analogWrite(5, 128);
//    setPwmFrequency(5, 1);
    }

    lcd.begin(16, 2);

    freq_setup();
//    FreqMeasure.begin();
}
float frq_old = 0;
int mode = 2;

void display_freq(float frq)
{
    lcd.setCursor(0, 0);
    if (frq > 9955)
    {
        lcd.print(frq/1000, 2);
        lcd.print(" kHz   ");
    }
    else
    {
        lcd.print(frq, 0);
        lcd.print(" Hz     ");
    }
}
void display_amp(float amp)
{
    lcd.setCursor(0, 1);
    lcd.print(amp,3);
    lcd.print(" V    ");
}

int xi = 0;
void pwm_test()
{
//    double pwmarr[] =
//    { 62500.0, 7812.5, 976.5625, 244.140625, 61.03515625 };
	
    xi++;
    if (xi > 5)
    {
        xi = 0;
        if (PWM_TEST)
        {
            mode++;
            if (mode > 5)
            {
                mode = 2;
            }
//        mode=5;
            TCCR0B = TCCR0B & 0b11111000 | mode;
        }
    }
    
}

//uint16_t read_peak(float frq)
//{
//    uint16_t peak = 0;
//    uint16_t n = 0;
//    if (frq > 10000)
//    {
//        n = 1;
//    }
//    else if (frq > 1000)
//    {
//        n = 10;
//    }
//    else if (frq > 100)
//    {
//        n = 100;
//    }
//    else if (frq > 10)
//    {
//        n = 1000;
//    }
//    else if (frq > 1)
//    {
//        n = 10000;
//    }
//    else
//    {
//        n = 10000;
//    }
//
//    for (int i = 0; i < n; i++)
//    {
//        uint16_t value = analogRead(PIN_AMP);
//        if (value > peak)
//        {
//            peak = value;
//        }
//    }
//	return peak;
//}

uint16_t read_peak(float frq)
{
	// discharge capacitor with ground
    pinMode(A0+PIN_AMP, OUTPUT);
    delayMicroseconds(100); 
    pinMode(A0+PIN_AMP, INPUT);

    // wait before read
    delayMicroseconds(100); 
    
	uint16_t peak = analogRead(PIN_AMP);
	return peak;
}


#define TIMEOUT	2000
float read_freq_period()
{
	FreqMeasure.begin();
    uint32_t pp=0;
    int32_t i=0;
    while (!FreqMeasure.available())         // wait until counter ready
    {
        i++;
        if (i>TIMEOUT)
        {
        	FreqMeasure.end();
            return 0;
        }
        delayMicroseconds(1000); // 1 ms
    }
    uint8_t avail = FreqMeasure.available();
    for(i=0;i<avail;i++)
    	pp=FreqMeasure.read();

    // clear buffer
//    FreqMeasure.read();
//    FreqMeasure.read();
//    FreqMeasure.read();
//    FreqMeasure.read();
//    FreqMeasure.read();
//    FreqMeasure.read();
//    FreqMeasure.read();
//    FreqMeasure.read();
//    FreqMeasure.read();
//    FreqMeasure.read();
//    FreqMeasure.read();
//    FreqMeasure.read();

//    return 16000000.0 / pp*1000;
	FreqMeasure.end();
    return FreqMeasure.countToFrequency(pp)*1000;
}

void loop()
{
	pwm_test();
	
    uint16_t gate_time;
    float frq = 0;
    if (read_freq(10) >= 10) // 1kHz
    {
        gate_time = 100;
        float frq_raw = read_freq(gate_time);
        static float frq_raw_old = 0;
        if (abs(frq_raw - frq_raw_old) == 1)
        {
            // avoid flickering
            frq_raw = frq_raw_old;
        }
        frq_raw_old = frq_raw;
        frq = frq_raw;
        if (gate_time == 100)
        {
            frq = 10 * frq;
        }
    }
    else
    {
        frq = read_freq_period();
    }


//    if (frq != frq_old)
    {

        frq_old = frq;

        display_freq(frq);

        uint16_t peak = read_peak(frq);
        display_amp(5.0 / 1024 * peak);
    }

}
