#pragma once

#include "config_hw.h"

#ifdef __MBED__
PwmOut spk(PIN_SPK);
#endif

void beep(int f)
{
#ifdef PIN_SPK

#ifdef __MBED__
    int us = 1000000 / f;
    spk.period_us(us);
    spk.write(0.5);
    wait_ms(10);
    spk.write(0);
#else
    tone(PIN_SPK, f, 10);
#endif

#endif
}

void beep_status(int index, bool error)
{
    int f;
    if (error)
        f = 2500 + 300 * index;
    else
        f = 500 + 100 * index;
    for (int i = 0; i < (index + 1); i++)
    {
        beep(f);
        wait_ms(100);
    }
}

void beep_up_down(bool up)
{
#ifdef PIN_SPK
#ifdef __MBED__
    spk.write(0.5);
    int us;
    for (int i = 0; i < 100; i++)
    {
        if (up)
        {
            us = 2000 - i * 10;
        }
        else
        {
            us = 1000 + i * 10;
        }
        spk.period_us(us);
        wait_us(us);
    }
    spk.write(0);
#else
    int us;
    for (int i = 0; i < 100; i++)
    {
        if (up)
        {
            us = 2000 - i * 10;
        }
        else
        {
            us = 1000 + i * 10;
        }
        tone(PIN_SPK, 1000000/us, 10);
        delayMicroseconds(us);
        noTone(PIN_SPK);
    }
#endif

#endif
}

void error_beep(int i)
{
    while (1)
    {
        switch (i)
        {
        case ERR_UNLOCK:
            beep(600);
            wait_ms(100);
            beep(800);
            wait_ms(100);
            beep(1000);
            wait_ms(100);
            break;

        case ERR_NO_FONA:
            beep(800);
            wait_ms(100);
            beep(1000);
            wait_ms(100);
            break;

        case ERR_NO_IMEI:
            beep(1000);
            wait_ms(100);
            beep(800);
            wait_ms(100);
            beep(600);
            wait_ms(100);
            break;

        }
    }
}

void start_beep()
{
    for (int i = 0; i < 100; i++)
    {
        beep(1000 + i * 10);

    }
    beep_up_down(true);
    wait_ms(100);
    wait_ms(100);
    beep_up_down(false);

    beep(600);
    wait_ms(100);
    beep(800);
    wait_ms(100);
    beep(1000);
}
