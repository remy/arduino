#include <SoftwareSerial.h>

SoftwareSerial mySerial(3, 1); // RX, TX

void setup()  {
 mySerial.begin(9600);
}
 
void loop()
{
 mySerial.println("Can it be true?! A serial monitor indeed!");
 delay(10);
}
