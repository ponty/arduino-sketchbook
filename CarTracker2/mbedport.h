#pragma once

#include "mbed.h"

#include "config_hw.h"

#define  INFO(STRING)        pc.printf(STRING "\r\n")
#define  F(STRING)           STRING

Serial pc(PIN_DEBUG_TX, PIN_DEBUG_RX);

void debug_init()
{
    pc.baud(115200);
}

void reboot()
{
    NVIC_SystemReset();
}

int main()
{
    setup();
    while(1)
        loop();
}
