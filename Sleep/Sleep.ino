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

void loop() {
  Serial.println("delay");
  delay(2000);
  
  //Serial.println("sleep");
  int sleepMS = Watchdog.sleep(2000);

  Serial.print("I'm awake now!  I slept for ");
  Serial.print(sleepMS, DEC);
  Serial.println(" milliseconds.");
  Serial.println();
}
