#include "Arduino.h"

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include "hw_config.h"
#include "sw_config.h"

// http://www.pjrc.com/teensy/td_libs_FreqCount.html
#include <FreqCount.h>
// test: 62500

//#include <FreqMeasure.h>

#include "freq.h"

// initialize the library with the numbers of the interface pins
//LiquidCrystal lcd(  //
//        LCD_RS,  // RS
//        LCD_EN,  // EN
//        LCD_D4,   // D4
//        LCD_D5,   // D5
//        LCD_D6,   // D6
//        LCD_D7    // D7
//        );
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, NEGATIVE);  // Set the LCD I2C address

const bool PWM_TEST = false;
void setup()
{
//    Serial.begin(9600);
    if (PWM_TEST)
    {
        pinMode(5, OUTPUT);
        analogWrite(5, 128);
//    setPwmFrequency(5, 1);
    }
    // set up the LCD's number of columns and rows: 
    lcd.begin(16, 2);
    // Print a message to the LCD.
//    lcd.print("hello, world!");

    freq_setup();
}
float frq_old = 0;
int mode = 2;
//uint32_t read_period()
//{
//    return pulseIn(5, LOW) + pulseIn(5, HIGH);
////    return pulseIn2(5, LOW);
////    uint32_t pp=0;
////    while (!pp)         // wait until counter ready
////    {
////        pp=FreqPeriod::getPeriod();
////    }
////    return 16000000.0 / pp;
//}

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
//    lcd.print(" ");
//    lcd.print(5.0 / 1024 * amp);
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

uint16_t read_peak(float frq)
{
    uint16_t peak = 0;
    uint16_t n = 0;
    if (frq > 10000)
    {
        n = 1;
    }
    else if (frq > 1000)
    {
        n = 10;
    }
    else if (frq > 100)
    {
        n = 100;
    }
    else if (frq > 10)
    {
        n = 1000;
    }
    else if (frq > 1)
    {
        n = 10000;
    }
    else
    {
        n = 10000;
    }

    for (int i = 0; i < n; i++)
    {
        uint16_t value = analogRead(PIN_AMP);
        if (value > peak)
        {
            peak = value;
        }
    }
	return peak;
}

void loop()
{
	pwm_test();
	
    uint16_t gate_time;
    if (read_freq(10) >= 10)
    {
        gate_time = 100;
    }
    else
    {
        gate_time = 1000;
    }
    float frq_raw = read_freq(gate_time);
    static float frq_raw_old = 0;
    if (abs(frq_raw - frq_raw_old) == 1)
    {
        // avoid flickering
        frq_raw = frq_raw_old;
    }
    frq_raw_old = frq_raw;

//    Serial.println(frq);
    float frq = frq_raw;
    if (gate_time == 100)
    {
        frq = 10 * frq;
    }
//    if (frq != frq_old)
    {

        frq_old = frq;

        display_freq(frq);

        uint16_t peak = read_peak(frq);
        display_amp(5.0 / 1024 * peak);
    }

}
