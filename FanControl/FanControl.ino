#include <avr/power.h>

#include "hw_config.h"
#include "config.h"

#if USE_NARCOLEPTIC > 0
#include <Narcoleptic.h>
#define delay  Narcoleptic.delay
#endif

//bool g_debug = 0;
//
//void read_debug()
//{
//    pinMode(pin_debug_input, INPUT);           // set pin to input
//    digitalWrite(pin_debug_input, HIGH);       // turn on pullup resistors
//
//    g_debug = !digitalRead(pin_debug_input);
//
//    digitalWrite(pin_debug_input, LOW);       // turn off pullup resistors
//
//    pinMode(pin_debug_led, g_debug ? OUTPUT : INPUT);
//}

void power_setup()
{
    //  power_all_disable();
    //  power_timer0_enable();

//    power_adc_disable();
    power_spi_disable();
    //  power_timer0_disable();
    //  power_timer1_disable();
    //  power_timer2_disable();
    power_usart0_disable();

    // turn on all pullups for lower power consumption
    //PORTB = PORTC = PORTD = 0xFF;

    // turn off pullups on non empty ports 
    //digitalWrite(pin_speaker, 0);
    //digitalWrite(pin_input, 0);
    //digitalWrite(pin_debug_led, 0);

}

void setup(void)
{
    power_setup();
}

void pulse()
{
	pinMode(pin_out, OUTPUT); 
	pinMode(pin_out_gnd, OUTPUT);
	
	digitalWrite(pin_out, 1);
	digitalWrite(pin_out_gnd, 0);
    
	delay(10);
	
	pinMode(pin_out, INPUT); 
	pinMode(pin_out_gnd, INPUT); 
	
}

void loop(void)
{
//    read_debug();

	int light = analogRead(pin_light);
	if (light > LIGHT_LIMIT)
    {
		pulse();
		
        for (int i = 0; i < 60; i++)
        {
            delay(1000);
        }
    }
    delay(1000);
}

