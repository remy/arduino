void setup(){
  //make sure your bluesmirf module is
  //configured at 115200 baudrate setting
  Serial.begin(115200);
  //let's test arduino led
  pinMode(13, OUTPUT);
}
void loop(){
  if(Serial.available()){
    //read first characer received
    unsigned char charreceived = Serial.read();
    
    switch(charreceived){
      case 'a':
        digitalWrite(13, HIGH);
        Serial.println("Arduino Led On");
        break;
      case 'b':
        digitalWrite(13, LOW);
        Serial.println("Arduino Led Off");
        break;
      default:
        break;
    }
    
    //flush remaining characters
    //we only want first character
    Serial.flush();
  }
  delay(10);
}
