#define OLED_DC 11
#define OLED_CS 12
#define OLED_CLK 10
#define OLED_MOSI 9   //(Pin on OLED labeled DATA)
#define OLED_RESET 13

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

void setup() {
  display.begin(SSD1306_SWITCHCAPVCC);
  
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.print(" Miles (50%)");
  
    display.drawLine(0, 12, 0, 31, WHITE);
  
  display.drawLine(63, 28, 63, 31, WHITE);
  display.drawLine(127, 12, 127, 31, WHITE);
  display.drawLine(31, 28, 31, 31, WHITE);
  
  display.drawLine(95, 28, 95, 31, WHITE);
  display.drawLine(0, 28, 127, 28, WHITE);
  display.drawLine(0, 12, 127, 12, WHITE);
  
  display.fillRect(2, 14, (124 * 0.5), 13, 1);

    display.setTextColor(WHITE);
    display.setCursor(5,20);
    display.print("Acquiring Satellites");

  display.display();
}


void loop() {

}

