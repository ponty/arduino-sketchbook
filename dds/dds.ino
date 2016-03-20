/*
 * A simple single freq AD9850 Arduino test script
 * Original AD9851 DDS sketch by Andrew Smallbone at <a class="linkification-ext" href="http://www.rocketnumbernine.com" title="Linkification: http://www.rocketnumbernine.com">www.rocketnumbernine.com</a>
 * Modified for testing the inexpensive AD9850 ebay DDS modules
 * Pictures and pinouts at nr8o.dhlpilotcentral.com
 * 9850 datasheet at <a class="linkification-ext" href="http://www.analog.com/static/imported-files/data_sheets/AD9850.pdf" title="Linkification: http://www.analog.com/static/imported-files/data_sheets/AD9850.pdf">http://www.analog.com/static/imported-files/data_sheets/AD9850.pdf</a>
 * Use freely
 */
 
#define FREQ1   1000
#define FREQ2   40000000
//#define FREQ3   100000

 #define W_CLK   A5//8       // Pin 8 - connect to AD9850 module word load clock pin (CLK)
 #define FQ_UD   A4//9       // Pin 9 - connect to freq update pin (FQ)
 #define DATA    A3//10       // Pin 10 - connect to serial data load pin (DATA)
 #define RESET   A2//11      // Pin 11 - connect to reset pin (RST).
 
 #define pulseHigh(pin) {digitalWrite(pin, HIGH); digitalWrite(pin, LOW); }
 
 // transfers a byte, a bit at a time, LSB first to the 9850 via serial DATA line
void tfr_byte(byte data)
{
  for (int i=0; i<8; i++, data>>=1) {
    digitalWrite(DATA, data & 0x01);
    pulseHigh(W_CLK);   //after each bit sent, CLK is pulsed high
  }
}
 
 // frequency calc from datasheet page 8 = <sys clock> * <frequency tuning word>/2^32
void sendFrequency(double frequency) {
  int32_t freq = frequency * 4294967295/125000000;  // note 125 MHz clock on 9850
  for (int b=0; b<4; b++, freq>>=8) {
    tfr_byte(freq & 0xFF);
  }
  tfr_byte(0x000);   // Final control byte, all 0 for 9850 chip
  pulseHigh(FQ_UD);  // Done!  Should see output
}
 
void setup() {
 // configure arduino data pins for output
  pinMode(FQ_UD, OUTPUT);
  pinMode(W_CLK, OUTPUT);
  pinMode(DATA, OUTPUT);
  pinMode(RESET, OUTPUT);
 
  pulseHigh(RESET);
  pulseHigh(W_CLK);
  pulseHigh(FQ_UD);  // this pulse enables serial mode - Datasheet page 12 figure 10
}
 
void loop() {
    float a= log(FREQ1);
    float b= (log(FREQ2)-log(FREQ1))/1000;
  for (int i=0;i<1000;i++)
  {
    float f= exp(a+b*i);
  sendFrequency(f);  // freq
  delay(10);
  }
  //sendFrequency(FREQ1);  // freq
  //delay(5000);
  //sendFrequency(FREQ2);  // freq
  //delay(5000);
  //sendFrequency(FREQ3);  // freq
  //delay(5000);
  //while(1);
}
