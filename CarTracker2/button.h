#pragma once

#include "config_hw.h"

#ifdef __MBED__
InterruptIn button(PIN_BUTTON);
#endif


bool button_read()
{
#ifdef __MBED__
    return button.read();
#else
    return digitalRead(PIN_BUTTON);
#endif
}

void flip()
{
    // against noise
    for (int i = 0; i < 200; i++)
    {
        wait_ms(1);
        if (!button_read())
        {
            return;
        }
    }

    alarm_mode = !alarm_mode;
    if (alarm_mode)
    {
        beep_up_down(true);

    }
    else
    {
        beep_up_down(false);
    }
}

void button_init()
{
#ifdef __MBED__
    button.rise(&flip);
#else
    attachInterrupt(digitalPinToInterrupt(PIN_BUTTON), flip, RISING);
#endif
}
