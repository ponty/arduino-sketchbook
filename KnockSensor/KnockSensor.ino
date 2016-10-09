// https://www.arduino.cc/en/Tutorial/Knock

/* Knock Sensor

 This sketch reads a piezo element to detect a knocking sound.
 It reads an analog pin and compares the result to a set threshold.
 If the result is greater than the threshold, it writes
 "knock" to the serial port, and toggles the LED on pin 13.

 The circuit:
 * + connection of the piezo attached to analog in 0
 * - connection of the piezo attached to ground
 * 1-megohm resistor attached from analog in 0 to ground

 http://www.arduino.cc/en/Tutorial/Knock

 created 25 Mar 2007
 by David Cuartielles <http://www.0j0.org>
 modified 30 Aug 2011
 by Tom Igoe

 This example code is in the public domain.

 */


// these constants won't change:
const int ledPin = 13;      // led connected to digital pin 13
const int knockSensor = A0; // the piezo is connected to analog pin 0
const int threshold = 30; // threshold value to decide when the detected sound is a knock or not
const int threshold_low = 8;
const int wait = 100;    // ms
#define INTRO

// these variables will change:
int sensorReading = 0;   // variable to store the value read from the sensor pin
int ledState = LOW; // variable used to store the last LED status, to toggle the light

#ifdef INTRO
#include <rtttl.h>
const int octave = 0;
const int pinSpeaker = knockSensor;

const char song_P[] PROGMEM =
        "Indiana:d=4,o=5,b=250:e,8p,8f,8g,8p,1c6";

Rtttl player;
#endif


void setup()
{
    pinMode(ledPin, OUTPUT); // declare the ledPin as as OUTPUT
    Serial.begin(9600);       // use the serial port

#ifdef INTRO
    player.begin(pinSpeaker);
    player.play_P(song_P, octave);
#endif
    
    pinMode(knockSensor, INPUT);

}





int on = 0;
unsigned long last_time = 0;
void loop()
{
    // read the sensor and store it in the variable sensorReading:
    sensorReading = analogRead(knockSensor);
    unsigned long time = millis();
//    Serial.println(sensorReading);

    // if the sensor reading is greater than the threshold:
    if (sensorReading >= threshold)
    {
        if (!on)
            if (time - last_time >= wait)
            {
                // toggle the status of the ledPin:
                ledState = !ledState;

//                ledState=1;

                // send the string "Knock!" back to the computer, followed by newline
                Serial.println("Knock!");
                on = 1;
                last_time = time;
            }
    }
    if (sensorReading < threshold_low)
    {
        on = 0;
//        ledState=0;
    }
    
    // update the LED pin itself:
    digitalWrite(ledPin, ledState);

    //delay(100);  // delay to avoid overloading the serial port buffer
}
