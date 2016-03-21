/* 
 ESP8266 CheckFlashConfig by Markus Sattler
 
 This sketch tests if the EEPROM settings of the IDE match to the Hardware
 
 */

void setup(void) {
    pinMode(2, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
    Serial.begin(115200);
}

void loop() {

    uint32_t realSize = ESP.getFlashChipRealSize();
    uint32_t ideSize = ESP.getFlashChipSize();
    FlashMode_t ideMode = ESP.getFlashChipMode();

  digitalWrite(2, LOW);   // Turn the LED on (Note that LOW is the voltage level
    Serial.printf("Flash real id:   %08X\n", ESP.getFlashChipId());
    Serial.printf("Flash real size: %u\n\n", realSize);

    Serial.printf("Flash ide  size: %u\n", ideSize);
    Serial.printf("Flash ide speed: %u\n", ESP.getFlashChipSpeed());
    Serial.printf("Flash ide mode:  %s\n", (ideMode == FM_QIO ? "QIO" : ideMode == FM_QOUT ? "QOUT" : ideMode == FM_DIO ? "DIO" : ideMode == FM_DOUT ? "DOUT" : "UNKNOWN"));

    if(ideSize != realSize) {
        Serial.println("Flash Chip configuration wrong!\n");
    } else {
        Serial.println("Flash Chip configuration ok.\n");
    }
 digitalWrite(2, HIGH);  // Turn the LED off by making the voltage HIGH

    delay(1000);
}
