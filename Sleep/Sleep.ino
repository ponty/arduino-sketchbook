// Adafruit Watchdog Library Sleep Example
//
// Simple example of how to do low power sleep with the watchdog timer.
//
// Author: Tony DiCola

#include <Adafruit_SleepyDog.h>

void setup() {
  Serial.begin(115200);
  while (!Serial) ; // wait for Arduino Serial Monitor (native USB boards)
  Serial.println("Adafruit Watchdog Library Sleep Demo!");
  Serial.println();
}

  unsigned long time;

void loop() {
  time = millis();
  Serial.println("delay 5000");
  delay(5000);
  Serial.print("millis change:");
  Serial.println(millis()-time);
  
  Serial.println("sleep 5000");
  time = millis();
  delay(10);
  int sleepMS = Watchdog.sleep(5000);

  Serial.print("I'm awake now!  I slept for ");
  Serial.print(sleepMS, DEC);
  Serial.println(" milliseconds.");
  Serial.print("millis change:");
  Serial.println(millis()-time);

  
  Serial.println("-------------------");
}
