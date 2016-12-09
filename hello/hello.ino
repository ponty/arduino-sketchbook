
void setup() { 
 //Initialize serial and wait for port to open:
  Serial.begin(9600); 
  Serial.println(F("hello! (9600)")); 
  Serial.end();
  
  Serial.begin(115200); 
  Serial.println(F("hello! (115200)")); 
  Serial.end();
  
} 
void loop() { 
} 
