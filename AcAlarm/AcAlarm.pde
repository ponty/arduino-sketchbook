//#include <Tone.h>
#include <rtttl.h>
#include <avr/power.h>

#include "hw_config.h"
#include "config.h"

#if USE_NARCOLEPTIC > 0
#include <Narcoleptic.h>
#define delay  Narcoleptic.delay
#endif

bool g_debug = 0;

Rtttl player;

void read_debug()
{
    pinMode(pin_debug_input, INPUT);           // set pin to input
    digitalWrite(pin_debug_input, HIGH);       // turn on pullup resistors

    g_debug = !digitalRead(pin_debug_input);

    digitalWrite(pin_debug_input, LOW);       // turn off pullup resistors

    pinMode(pin_debug_led, g_debug ? OUTPUT : INPUT);
}

void power_setup()
{
    //  power_all_disable();
    //  power_timer0_enable();

    power_adc_disable();
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
    player.begin(pin_speaker);
    player.play_P(start_song_P, octave);
}

void led_debug_mode_x(bool on, int mode)
{
    if (g_debug && (LED_DEBUG_MODE==mode))
    {
        digitalWrite(pin_debug_led, on);
    }
}
void led_debug_mode_0(bool on)
{
    led_debug_mode_x(on, 0);
}
void led_debug_mode_1(bool on)
{
    led_debug_mode_x(on, 1);
}
void led_debug_mode_2(bool on)
{
    led_debug_mode_x(on, 2);
}

bool is_period_on()
{
    bool on = false;

    for (int i = 0; i < PERIOD_MS; i++)
    {
        bool current = digitalRead(pin_input);
        led_debug_mode_0(current);
        on |= current;
        delay(1); // 1 ms
    }

    led_debug_mode_1(on);
    return on;
}

bool is_on()
{
    for (int i = 0; i < 3; i++)
    {
        if(!is_period_on())
        {
            led_debug_mode_2(false);
            return false;
        }
        if (!g_debug)
        {
            delay(500);
        }
    }

    led_debug_mode_2(true);
    return true;
}

void loop(void)
{
    read_debug();

    if (is_on())
    {
        if (!g_debug)
        {
            player.play_P(alarm_song_P, octave);
            for (int i = 0; i < WAIT_SEC; i++)
            {
                delay(1000);
//                if (!is_on())
//                {
//                    break;
//                }
            }
        }
    }

}

