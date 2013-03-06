#include <LiquidCrystal.h>

int FSR_Pin = A0; //analog pin 0
// rs, rw, enable, d4, d5, d6, d7
LiquidCrystal lcd(2, 3, 4, 9, 10, 11, 12);

int last[] = {0,0,0,0};

float step = 900/16;

void setup(){
  // set up the LCD"s number of columns and rows: 
  lcd.begin(20, 4);
  lcd.print("Ready");
  Serial.begin(9600);
}

void loop(){
  int FSRReading = analogRead(FSR_Pin); 
  lcd.clear();
/*
  for (int i = 3; i > 0; i--) {
    lcd.setCursor(0, i);
    lcd.print(last[i]);
    last[i] = last[i-1];
  }
  */
  lcd.setCursor(0, 0);
  last[0] = FSRReading;
  lcd.print(FSRReading);
  
  lcd.setCursor(0, 1);

  if (FSRReading < step * 1) {
      lcd.print("Stop");
  } else if (FSRReading < step * 2) {
      lcd.print("Slowest");
  } else if (FSRReading < step * 3) {
      lcd.print("Way Way Slow");
  } else if (FSRReading < step * 4) {
    lcd.print("Way Slow");
  } else if (FSRReading < step * 5) {
    lcd.print("Less Way Slow");
  } else if (FSRReading < step * 6) {
    lcd.print("Slow");
  } else if (FSRReading < step * 7) {
    lcd.print("Easy-Going");
  } else if (FSRReading < step * 8) {
    lcd.print("Cruising");
  } else if (FSRReading < step * 9) {
    lcd.print("Moving Right Along");
  } else if (FSRReading < step * 10) {
    lcd.print("Moving Quick");
  } else if (FSRReading < step * 11) {
    lcd.print("Moving Quicker");
  } else if (FSRReading < step * 12) {
    lcd.print("Moving Pretty Darn Quick");
  } else if (FSRReading < step * 13) {
    lcd.print("Fast");
  } else if (FSRReading < step * 14) {
    lcd.print("Faster");
  } else if (FSRReading < step * 15) {
    lcd.print("Fastest");
  } else if (FSRReading >= step * 15) {
    lcd.print("I lied, this is fastest");
  }


  Serial.println(FSRReading);
  delay(250); //just here to slow down the output for easier reading
}

