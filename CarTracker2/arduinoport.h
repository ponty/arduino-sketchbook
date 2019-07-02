#pragma once

unsigned long sleep_millis=0;

#define     D0          0
#define     D1          1
#define     D2          2
#define     D3          3
#define     D4          4
#define     D5          5
#define     D6          6
#define     D7          7
#define     D8          8
#define     D9          9
#define     D10          10
#define     D11          11
#define     D12          12
#define     D13          13

#define debug_serial  Serial

#define  INF(STRING)        debug_serial.print(F(STRING))
#define  INFLN(STRING)      debug_serial.println(F(STRING))
#define  INFVAR(VAR)        debug_serial.print(VAR)
#define  INFC(VAR)          debug_serial.write(VAR)

void debug_init()
{
    while (!Serial)
        ;

    debug_serial.begin(115200);
}

#include "SoftwareReset.h"
void reboot()
{
    softwareReset (SKETCH);
}

void wait(int sec)
{
    delay(10);
    for (int i = 0; i < sec; ++i)
    {
//        delay(1000);
        Watchdog.sleep(1000);
        sleep_millis += 1000;
//        Narcoleptic.delay(1000);
    }
}

void wait_ms(int ms)
{
    delay(ms);
}

class Timer
{
    bool run;
    unsigned long start_value;
    unsigned long value;
public:
    Timer()
    {
        reset();
    }
    unsigned long all_millis()
    {
        return millis()+sleep_millis;
    }
    void start()
    {
        start_value = all_millis();
        run = true;
    }
    void stop()
    {
        value = all_millis();
        run = false;
    }
    void reset()
    {
        value = 0;
        start_value = 0;
        run = false;
    }
    unsigned long read()
    {
        if (run)
            value = all_millis();

        return (value - start_value) / 1000;
    }

};
