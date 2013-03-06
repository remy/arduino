#define OLED_DC 11
#define OLED_CS 12
#define OLED_CLK 10
#define OLED_MOSI 9   //(Pin on OLED labeled DATA)
#define OLED_RESET 13

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

float step = 900/16;
int FSR_Pin = A0; //analog pin 0

void setup() {
  display.begin(SSD1306_SWITCHCAPVCC);
  display.clearDisplay();
  display.display();  
}

void drawForce(double i) {
  display.drawLine(0, 12, 0, 31, WHITE);
  
  display.drawLine(63, 28, 63, 31, WHITE);
  display.drawLine(127, 12, 127, 31, WHITE);
  display.drawLine(31, 28, 31, 31, WHITE);
  
  display.drawLine(95, 28, 95, 31, WHITE);
  display.drawLine(0, 28, 127, 28, WHITE);
  display.drawLine(0, 12, 127, 12, WHITE);
  
  display.fillRect(2, 14, (124 * i), 13, 1);
}

void loop() {
  int FSRReading = analogRead(FSR_Pin); 
  
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  
  double val = (double)FSRReading / 900;

  if (FSRReading < step * 1) {
    display.print("Stop");
  } else if (FSRReading < step * 2) {
    display.print("Slowest");
  } else if (FSRReading < step * 3) {
    display.print("Way Way Slow");
  } else if (FSRReading < step * 4) {
    display.print("Way Slow");
  } else if (FSRReading < step * 5) {
    display.print("Less Way Slow");
  } else if (FSRReading < step * 6) {
    display.print("Slow");
  } else if (FSRReading < step * 7) {
    display.print("Easy-Going");
  } else if (FSRReading < step * 8) {
    display.print("Cruising");
  } else if (FSRReading < step * 9) {
    display.print("Moving Right Along");
  } else if (FSRReading < step * 10) {
    display.print("Moving Quick");
  } else if (FSRReading < step * 11) {
    display.print("Moving Quicker");
  } else if (FSRReading < step * 12) {
    display.print("Moving Pretty Quick");
  } else if (FSRReading < step * 13) {
    display.print("Fast");
  } else if (FSRReading < step * 14) {
    display.print("Faster");
  } else if (FSRReading < step * 15) {
    display.print("Fastest");
  } else if (FSRReading >= step * 15) {
    display.print("THIS
    is fastest");
  }  

  drawForce(val);

  display.display();
  
}

