#include "Arduino.h"

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include "hw_config.h"
#include "sw_config.h"

// http://www.pjrc.com/teensy/td_libs_FreqCount.html
#include <FreqCount.h>
#include <FreqMeasure.h>

#ifdef PWM_TEST
#include "test.h"
#endif


LiquidCrystal_I2C lcd(LCD_I2C_CONFIG);

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
    Serial.begin(9600);

#ifdef PWM_TEST
    pwm_test_setup();
#endif
    
    lcd.begin(16, 2);

    freq_setup();
}
float frq_old = 0;

void display_freq(float frq)
{
    lcd.setCursor(0, 0);
    if (frq > 9955)
    {
        lcd.print(frq/1000, 2);
        lcd.print(" kHz     ");
    }
    else
    {
        Serial.print("frq:");
        Serial.println(frq);
        lcd.print(frq, 2);
        lcd.print(" Hz      ");
    }
}
void display_amp(float amp)
{
    lcd.setCursor(0, 1);
    lcd.print(amp,3);
    lcd.print(" V    ");
}



uint16_t read_peak(float frq)
{
	uint16_t peak = analogRead(PIN_AMP);
	return peak;
}


float read_freq_period()
{
	FreqMeasure.begin();
    uint32_t pp=0;
    int32_t i=0;
    while (FreqMeasure.available()<2)         // wait until counter ready
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

	FreqMeasure.end();
    return FreqMeasure.countToFrequency(pp);
}

void loop()
{
#ifdef PWM_TEST
	pwm_test_loop();
#endif
	
    uint16_t gate_time;
    float frq = 0;
    if (read_freq(10) >= 100) // 10kHz
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
//        Serial.print("frq:");
//        Serial.println(frq);
    }


//    if (frq != frq_old)
    {

        frq_old = frq;

        display_freq(frq);

        uint16_t peak = read_peak(frq);
        display_amp(5.0 / 1024 * peak);
    }

}
