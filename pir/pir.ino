int pirPin = 13; //digital 2

void setup(){
 Serial.begin(9600);
 pinMode(pirPin, INPUT);
}

void loop(){
  int pirVal = digitalRead(pirPin);
//  Serial.println("pirVal: " + pirVal);
  if(pirVal == LOW){ //was motion detected
    Serial.println("Motion Detected");
    delay(2000);
  }
}

