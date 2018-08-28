//  http://www.pjrc.com/teensy/td_libs_FreqMeasure.html
#include <FreqMeasure.h>

//  http://www.pjrc.com/teensy/td_libs_FreqCount.html
#include <FreqCount.h>

#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

// The complete list is available here: https://github.com/olikraus/u8g2/wiki/u8g2setupcpp
// Please update the pin numbers according to your setup. Use U8X8_PIN_NONE if the reset pin is not connected
//U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/U8X8_PIN_NONE);
U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* reset=*/U8X8_PIN_NONE);

//#include <U8x8lib.h>
//
//#ifdef U8X8_HAVE_HW_SPI
//#include <SPI.h>
//#endif
//
//U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE);          

// freq -> pin D5, pin D8

//  5V----IR_photo--*--10k---D9
//                  |--1k----D10

// R=max 1K
//    at 10kohm 37khz is filtered out

#define PIN_1K   10
#define PIN_10K  9
#define PIN_ANALOG_DIODE    0
#define PIN_ANALOG_CALIBRATION 1
void pull(int pin)
{
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
}
void release(int pin)
{
    pinMode(pin, INPUT);
}

void setup()
{
    Serial.begin(9600);
    Serial.println("begin");

//    u8x8.begin();
//    u8x8.setPowerSave(0);
//
//    u8x8.setFont(u8x8_font_chroma48medium8_r);
//    u8x8.draw2x2String(0,1,"Hello World!");
//    for(;;);
//    u8x8.refreshDisplay();        // for SSD1606/7  

    u8g2.begin();

    u8g2.firstPage();
    do
    {
        u8g2.setFont(u8g2_font_ncenB14_tr);
        u8g2.drawStr(1, 20, "IRTester");
        u8g2.drawStr(1, 41, "IRTester2");
        u8g2.drawStr(1, 62, "IRTester3");
        u8g2.drawFrame(0, 0, 128, 64);
    } while (u8g2.nextPage());

//    u8g2.clearBuffer();
//    u8g2.setFont(u8g2_font_ncenB14_tr);
//    u8g2.drawStr(1, 20, "IRTester");
//    u8g2.drawStr(1, 41, "IRTester2");
//    u8g2.drawStr(1, 62, "IRTester3");
//    u8g2.drawFrame(0, 0, 128, 64);
//
//    u8g2.sendBuffer();

//    pull(PIN_10K);
//    digitalWrite(PIN_10K, HIGH);
//
//    delay(100);

//    pull(PIN_10K);
    pull(PIN_1K);

}

void loop2()
{
    float calibr = 0;
    float a = 0;
    for (int i = 0; i < 10; i++)
    {
        // on 2 serial diodes -> 1.3V
        calibr += analogRead(PIN_ANALOG_CALIBRATION);
        a += analogRead(PIN_ANALOG_DIODE);
    }
    calibr /= 10;
    a /= 10;
    float volt = 1.3 / calibr * (1023.0 - a);
    int mV = (int) (volt * 1000);

    u8g2.firstPage();
    do
    {
        u8g2.setCursor(1, 20);
        u8g2.print("Voltage:");
        u8g2.setCursor(15, 40);
        u8g2.print(mV);
        u8g2.print("mV");
        u8g2.drawFrame(0, 0, 128, 64);
    } while (u8g2.nextPage());

    Serial.print("a:");
    Serial.print(a);
    Serial.print(" cal:");
    Serial.print(calibr);
    Serial.print("  V=");
    Serial.print(mV);
    Serial.println("mV");

    delay(1000);
}

void loop3()
{
    if (FreqCount.available())
    {
        unsigned long count = FreqCount.read();
        Serial.println(count);
    }
    delay(1000);
}

void loop()
{
    float fmax = 0;

    FreqMeasure.begin();
    delay(500);

    Serial.print("available:");
    Serial.print(FreqMeasure.available());
    if (FreqMeasure.available())
    {
        Serial.print(" ");
        int a = FreqMeasure.available();
        const int istart = 2; // drop first 2, they are random
        for (int i = 0; i < a; i++)
        {
            uint32_t n = FreqMeasure.read();
            if (i < istart)
            {
                continue;
            }
            float frequency = FreqMeasure.countToFrequency(n);
            Serial.print(" (");
            Serial.print(n);
            Serial.print(") ");
            Serial.print(frequency, 0);
            Serial.print(" ");

            // find shortes pulse width -> max freq
            if (frequency > fmax)
                fmax = frequency;
        }
    }
    FreqMeasure.end();
    Serial.print(" fmax:");
    Serial.print(fmax, 0);

    const int gate_ms = 1000;
    FreqCount.begin(gate_ms);
    delay(1100);
    unsigned long count = FreqCount.read();
    FreqCount.end();
    int perc = 100 * count / fmax;
//    float fc = 1000.0 * count / gate_ms;

    Serial.print(" count:");
    Serial.print(count);
//    Serial.print(" f:");
//    Serial.print(fc, 0);
    Serial.print(" perc:");
    Serial.print(perc);
    Serial.print("%");
    Serial.println();

    u8g2.firstPage();
    do
    {
        u8g2.setCursor(1, 20);
        u8g2.print("f:");
        if (fmax > 5000)
        {
            u8g2.print(fmax / 1000, 0);
            u8g2.print(" kHz");
        }
        else
        {
            u8g2.print(fmax, 0);
            u8g2.print(" Hz");
        }
        u8g2.setCursor(1, 40);
        u8g2.print("count:");
        u8g2.print(count);
        u8g2.setCursor(1, 60);
        u8g2.print("perc:");
        u8g2.print(perc);
        u8g2.print("%");
        u8g2.drawFrame(0, 0, 128, 64);
    } while (u8g2.nextPage());

//    delay(1000);              // wait for a second
}

