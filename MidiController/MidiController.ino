#include <MIDI.h>

// max 6
#define ANALOG_IN  6

//#define LED 13          // LED pin on Arduino board

const int TUNE = -3;
const int VELOCITY = -2;
const int PROGRAM = -1;
const int VOLUME = 7;
const int BALANCE = 8;
const int PAN = 10;
const int EXPRESSION = 11;
const int REVERB = 91;
const int TREMOLO = 92;
const int CHORUS = 93;
const int DETUNE = 94;
const int PHASER = 95;
//const int PORTAMENTO = 65;
//const int VOLUME = 7;

const int8_t controls[] =
{ EXPRESSION, // 0
        PROGRAM, // 1
        VELOCITY, // 2
        REVERB, // 3
        CHORUS, // 4
        TUNE, // 5
        };

void setup()
{
//    pinMode(LED, OUTPUT);
    MIDI.begin(1);
    MIDI.turnThruOff();
//    Serial.begin(115200); // will change baud rate of MIDI traffic from 31250 to 115200

// pullup
    digitalWrite(0, HIGH);
}

const int Channel = 1;
byte velo = 0;
int8_t tune = 0;
void loop()
{
    if (MIDI.read())
    {
        kMIDIType type = MIDI.getType();
        byte data1 = MIDI.getData1();
        byte data2 = MIDI.getData2();
        byte channel = MIDI.getChannel();
//        digitalWrite(LED, HIGH);     // Blink the LED
        if (type == NoteOn and data2 != 0)
        {
            if (data2 > (127 - velo))
                data2 = 127;
            else
                data2 += velo;
            data1 += tune;
        }
        if (type == NoteOff or data2 == 0)
        {
            data1 += tune;
        }
        MIDI.send(type, data1, data2, channel);
    }
    else
    {
        //digitalWrite(LED, HIGH);     // Blink the LED

        static int16_t last_pot[ANALOG_IN];
        for (int i = 0; i < ANALOG_IN; i++)
        {
            int16_t pot = analogRead(i);
            if (abs(pot - last_pot[i]) > 1)
            {
                byte value = (byte)(pot >> 3);
                if (controls[i] == VELOCITY)
                {
                    velo = value;
                }
                else if (controls[i] == TUNE)
                {
                    tune = (value >> 3) - 8;
                }
                else if (controls[i] == PROGRAM)
                {
                    MIDI.sendProgramChange(value, Channel);
                }
                else
                {
                    MIDI.sendControlChange(controls[i], value, Channel);
                }

                last_pot[i] = pot;
            }
        }

    }
}