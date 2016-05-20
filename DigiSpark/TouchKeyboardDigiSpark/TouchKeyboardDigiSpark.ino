#include "cfg_hw.h"
#include "cfg_secret.h"

#ifdef ARDUINO_AVR_DIGISPARK
#include "DigiKeyboard.h"
#endif

int g_pushed = 0;

void mydelay(int ms)
{
#ifdef ARDUINO_AVR_DIGISPARK
    // It's better to use DigiKeyboard.delay() over the regular Arduino delay()
    // if doing keyboard stuff because it keeps talking to the computer to make
    // sure the computer knows the keyboard is alive and connected
    DigiKeyboard.delay(ms);
#else
    delay(ms);
#endif
}

void send_keys(const char* s)
{
#ifdef ARDUINO_AVR_DIGISPARK
    // this is generally not necessary but with some older systems it seems to
    // prevent missing the first character after a delay:
    DigiKeyboard.sendKeyStroke(0);

    // Type out this string letter by letter on the computer (assumes US-style
    // keyboard)
    DigiKeyboard.println(s);
#else
    Serial.println(s);
#endif
}

void setup()
{
#ifdef ARDUINO_AVR_DIGISPARK
    // don't need to set anything up to use DigiKeyboard

#else
    //Initialize serial and wait for port to open:
    Serial.begin(9600);
    Serial.println("start");
#endif

    pinMode(PIN_KEY1, INPUT);
    pinMode(PIN_KEY2, INPUT);
    pinMode(PIN_KEY3, INPUT);

}

void loop()
{
    int b1 = digitalRead(PIN_KEY1);
    int b2 = digitalRead(PIN_KEY2);
    int b3 = digitalRead(PIN_KEY3);
    int none = !b1 && !b2 && !b3;
    if (g_pushed)
    {
        if (none)
        {
            g_pushed = 0;
//            Serial.println("released");
        }
    }
    else
    {
        if (!none)
        {
            g_pushed = 1;

            if (b1)
            {
                send_keys (SECRET1);
            }
            else if (b2)
            {
                send_keys (SECRET2);
            }
            else if (b3)
            {
                send_keys (SECRET3);
            }

//            Serial.println("pressed");
        }

    }

    mydelay(1);
}
