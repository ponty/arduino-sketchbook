/* FreqMeasure - Example with LCD output
 * http://www.pjrc.com/teensy/td_libs_FreqMeasure.html
 *
 * This example code is in the public domain.
 */
#include <FreqMeasure.h>
#include <LiquidCrystal.h>

//LiquidCrystal lcd(5, 4, 3, 2, 1, 0);
LiquidCrystal lcd(6,  // RS
                  7,  // EN
                  0,   // D4
                  1,   // D5
                  2,   // D6
                  3    // D7
                  );



void setup() {
  //Serial.begin(57600);
  lcd.begin(16, 2);
  lcd.print("Freq:");
  FreqMeasure.begin();
}


double sum=0;
int count=0;

void loop() {
  if (FreqMeasure.available()) {
    // average several reading together
    sum = sum + FreqMeasure.read();
    count = count + 1;
    if (count > 3) {
      double frequency = F_CPU / (sum / count);
      lcd.setCursor(0, 1);
      lcd.print(frequency);
      lcd.print("       ");
      sum = 0;
      count = 0;
    }
  }
}
