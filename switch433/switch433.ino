/*
  433 send
  
*/

#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();

void setup() {

  Serial.begin(9600);
  Serial.println("START");
  
  // Transmitter is connected to Arduino Pin #10  
  mySwitch.enableTransmit(10);

  // Optional set pulse length.
  mySwitch.setPulseLength(333);
  
  // Optional set protocol (default is 1, will work for most outlets)
  mySwitch.setProtocol(1);
  
  // Optional set number of transmission repetitions.
  mySwitch.setRepeatTransmit(15);


}

void loop() {
  Serial.println("ON");
  mySwitch.send(0x511557, 24);
  delay(5000);  
  
  Serial.println("OFF");
  mySwitch.send(0x511554, 24);
  delay(5000);  
}
