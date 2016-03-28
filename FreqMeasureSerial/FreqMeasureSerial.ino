/* FreqMeasure - Example with serial output
 * http://www.pjrc.com/teensy/td_libs_FreqMeasure.html
 *
 * This example code is in the public domain.
 */
#include <FreqMeasure.h>

void setup() {
  Serial.begin(9600);
  Serial.println("begin");
  FreqMeasure.begin();
}

void loop() {
  Serial.print("available:");
  Serial.print(FreqMeasure.available());
  if (FreqMeasure.available()) {
    Serial.print(" ");
    int a = FreqMeasure.available();
    for (int i=0;i<a;i++)
    {
    uint32_t n = FreqMeasure.read();
    float frequency = FreqMeasure.countToFrequency(n);
    //Serial.print(n);
    //Serial.print(" ");
    Serial.print(frequency);
    Serial.print(" ");
    }
  }
  Serial.println();
  delay(1000);              // wait for a second
}

