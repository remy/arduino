#include <SoftwareSerial.h>

SoftwareSerial portOne(2, 3);

void setup() {
  Serial.begin(57600);
  
  portOne.begin(57600);
  portOne.listen();
  
  
}

void loop() {
  while (portOne.available() > 0) {
    char c = portOne.read();
    Serial.write(c);
  }
}
