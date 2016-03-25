#include <coll.h>

// Difference with str and Serial.println(str) uncommented:
//      14 bytes (13 ascii chars + null terminator

//char str[] = "Hallo, world!";

void setup() {
    Serial.begin(9600);

    //Serial.println(str);

    Serial.print("freeMemory1()=");    Serial.println(freeMemory1());
    //Serial.print("freeMemory2()=");    Serial.println(freeMemory2());
    //Serial.print("freeMemory3()=");    Serial.println(freeMemory3());
}


void loop() {
}
