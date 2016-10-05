#include <avr/io.h>
#include <avr/wdt.h>

int ledPin = LED_BUILTIN;

void setup()
{
  MCUSR=0;
  wdt_disable();
  Serial.begin(9600);
  Serial.println("BOOT!");
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  delay(500);
}

void loop(){
  if (Serial.available() > 0){
    char cmd = Serial.read();
    if (cmd == 'r'){
      Serial.println("r received!");
      wdt_enable(WDTO_1S);
      delay(1000);
      Serial.println("1 SEC.");
      delay(1000);
      Serial.println("1 SEC.");
    }
  }
  wdt_reset();
  Serial.println("loop");
  digitalWrite(ledPin, HIGH);
  delay(500);
  digitalWrite(ledPin, LOW);
  delay(500);
}