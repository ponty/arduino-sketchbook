#include <MIDI.h>
/*
  Basic I/O MIDI tutorial
  by Franky
  28/07/2009
*/

#define LED 13   		// LED pin on Arduino board

void setup() {
  pinMode(LED, OUTPUT);
  MIDI.begin(4);            	// Launch MIDI with default options
				// input channel is set to 4
  //Serial.begin(9600); // will change baud rate of MIDI traffic from 31250 to 115200
}

void loop() {
//  if (MIDI.read()) {
    digitalWrite(LED,HIGH);     // Blink the LED
    MIDI.sendNoteOn(72,127,1);  // Send a Note (pitch 42, velo 127 on channel 1)
    delay(1000);		// Wait for a second
    MIDI.sendNoteOff(72,0,1);   // Stop the note
    digitalWrite(LED,LOW);    	
    delay(1000);		// Wait for a second
  //}
}
