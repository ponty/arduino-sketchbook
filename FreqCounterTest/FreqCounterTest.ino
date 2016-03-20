#include <FreqCounter.h>
#include <util/delay.h>     /* for _delay_ms() */

void setup() {
  Serial.begin(19200);                    // connect to the serial port
  Serial.println("Frequency Counter");
  
  pinMode(5, OUTPUT);
  analogWrite(5, 128);
  // f=[76.2939453125, 305.17578125, 1220.703125, 9765.625, 78125.0]
  // div: 1,8,64,256,1024,
  TCCR0B &= ~7;
  TCCR0B |= 1;
  _delay_ms(1000);
  //delay(10000);
}

//unsigned long frq;
void loop() {

 FreqCounter::f_comp= 0;             // Set compensation to 12
 FreqCounter::start(1000);            // Start counting with gatetime of 100ms
 while (FreqCounter::f_ready == 0)         // wait until counter ready
 {
 }
 float f=1.25*FreqCounter::f_freq;            // read result
 Serial.print(" f="); Serial.print(f);   

 Serial.print(" f_freq=");Serial.print(FreqCounter::f_freq);                // print result
 Serial.print(" f_debug=");Serial.print(FreqCounter::f_debug);                // print result
 Serial.print(" f_tics=");Serial.print(FreqCounter::f_tics);                // print result
 Serial.print(" f_period=");Serial.print(FreqCounter::f_period);                // print result
 Serial.print(" f_mlt=");Serial.print(FreqCounter::f_mlt);                // print result
 Serial.print(" f_comp=");Serial.print(FreqCounter::f_comp);                // print result
 Serial.print(" TCNT1=");Serial.print(TCNT1);                // print result
 Serial.print(" TCCR1A=");Serial.print(TCCR1A);                // print result
 Serial.print(" TCCR1B=");Serial.print(TCCR1B);                // print result
 Serial.print(" TCCR2A=");Serial.print(TCCR2A);                // print result
 Serial.print(" TCCR2B=");Serial.print(TCCR2B);                // print result
 Serial.println();                // print result
 
 //delay(10000);
  _delay_ms(1000);
}
