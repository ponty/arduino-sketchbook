#include <Wire.h>
#include <i2cdetect.h>

void setup() {
  Serial.begin(9600);
  Serial.println("\n=============");
  Serial.println(__FILE__);
  Serial.println(__DATE__);
  Serial.println(__TIME__);
  Wire.begin();
  Serial.println("i2cdetect example\n");
  Serial.print("Scanning address range 0x03-0x77\n\n");
}

void loop() {
  i2cdetect();  // default range from 0x03 to 0x77
  delay(4000);
}
