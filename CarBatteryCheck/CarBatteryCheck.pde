#include <SevSeg.h>
#include <TimedAction.h>

#include "hw_config.h"
#include "sw_config.h"
#include "calibration.h"

//#if USE_NARCOLEPTIC > 0
//#include <Narcoleptic.h>
//#define delay  Narcoleptic.delay
//#endif

TimedAction g_timer_switch_mode = TimedAction(PERIOD, switch_mode);
TimedAction g_timer_display_off = TimedAction(ON_TIME, display_off);

SevSeg myDisplay;

const float Vmax = use_internal ? Vref : Vcc; // V
const float divider = R2 / (R1 + R2);
const float MULTIPLIER = 100 * (Vmax * calibration_multipilier) / divider
        / 1023;

char g_display_string[] = "    "; //Used for sprintf
bool g_mode = false;

void setup()
{
    if (use_internal)
        analogReference (INTERNAL);

//    Serial.begin(9600);

    myDisplay.Begin(displayType, numberOfDigits, pin_digit1, pin_digit2,
            pin_digit3, pin_digit4, pin_segA, pin_segB, pin_segC, pin_segD,
            pin_segE, pin_segF, pin_segG, pin_segDP);

    myDisplay.SetBrightness(100);

    switch_mode();
}

void measure()
{
    int16_t Ax = analogRead(pin_input - A0);
    float Vx;
    if (use_external_ref)
    {
        int16_t Aref = analogRead(pin_Vref_ext - A0);
        Vx = Ax * MULTIPLIER * Vref_ext / (Aref * Vmax / 1023);
    }
    else
    {
        Vx = Ax * MULTIPLIER;
    }
    int16_t voltage = Vx + 0.5;

    // 11.60 V  --->   0%
    // 12.60 V  ---> 100%
    int16_t percent = voltage - 1160;
    if (percent < -200)
        g_mode = true;
    sprintf(g_display_string, "%4d", g_mode ? voltage : percent);
//    Serial.println("measure");
}

void update_display()
{
    myDisplay.DisplayString(g_display_string, g_mode ? 2 : 0);
//    Serial.println(g_display_string);
}

void switch_mode()
{
//    Serial.println("switch_mode");
    g_mode = !g_mode;
    measure();
    //update_display();
//    g_timer_display_off.reset();
//    g_timer_display_off.enable();

}
void display_off()
{
    g_timer_display_off.setInterval(PERIOD);
//    Serial.println("display_off");
//    g_timer_display_off.disable();
//    sprintf(g_display_string, "    ");
    //g_display_string="    ";
//    update_display();
//    myDisplay.DisplayString("    ", 0);
    delay(PERIOD - ON_TIME);
}

void loop()
{
    g_timer_switch_mode.check();
    g_timer_display_off.check();

    update_display();
}
