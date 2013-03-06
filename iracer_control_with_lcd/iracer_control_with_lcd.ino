int masterPin = 4;
int isMaster = 0;
int msgToken = 0;

int incomingByte = 0;
char gBtMsg[256];
char gBTAdr[13];
char gBtCmd[256];

int gBtKnownMACTotal = 1;
char* gBtKnownMAC[1]; //This is set to hold only two MAC adresses

// LCD values
#define OLED_DC 11
#define OLED_CS 12
#define OLED_CLK 10
#define OLED_MOSI 9   //(Pin on OLED labeled DATA)
#define OLED_RESET 13

// pressure sensor values
float step = 900/16;
int FSR_Pin = A0; //analog pin 0

// joystick
const int selectPin = 7;  
const int joystick_xPin = A2; // H  
const int joystick_yPin = A1; // V
int oldX = 0;  
int oldY = 0;  
int oldSelect = 0;  

// loop throttle
unsigned long time;

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//SoftwareSerial mySerial(1, 2); // RX, TX

Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);


void BtReceive(void){
  bool keepReading = true;
  int index = 0;
  
  gBtMsg[0] = '\0';
  
//  SdLog("reading message...");
  
  while(keepReading){
    keepReading = false;
    
    if (Serial.available() > 0) {
      // read the incoming byte:
      incomingByte = Serial.read();
      if(incomingByte != 13){
        gBtMsg[index++] = incomingByte;
        keepReading = true;
      }
    }
  }
  
  gBtMsg[index] = '\0';
  if (index > 0) {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("<- ");
    display.print(gBtMsg);
    display.display();
//    SdLog(gBtMsg);
    delay(100);
  }
//  SdLog(strcat("in: ", gBtMsg));
}

void BtSend(char* i_pBtCmd, bool i_ln = true){

  if(i_ln){
    Serial.println(i_pBtCmd);
  } else{
    Serial.print(i_pBtCmd);
  }
  
  //Debug start
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("-> ");
    display.print(i_pBtCmd);
    display.display();
//    SdLog(gBtMsg);
    delay(100);
  //Debug end

//  delay(100);
  BtReceive();
  
}


void BtInit(void){
  isMaster = digitalRead(masterPin); 
  bool btConnect = false;

  gBtKnownMAC[0] = "001205119333"; // iracer
  
  int matchedMac = -1;
  
  SdLog(">>>> start");
 
  Serial.begin(115200);
  
  // test if we're connected already
  int numVal = 10;

  while(numVal > 1){
    BtSend("$$$", false);
    BtSend("GK");
    
    if(strlen(gBtMsg) > 0) {
      numVal = atoi(gBtMsg);
    }
    
    BtSend("---");
    delay(1000);
  }

  if(numVal == 1){
    btConnect = true;
    SdLog(">>>> early connection");
    delay(1000);
    
    return;
  }
    
  BtSend("$$$", false);
  
  if(true){
    SdLog(">>>> Set To Master");

    BtSend("SM,1");

    BtSend("I,5");
    BtSend("---"); 
   
    while(!btConnect){
      delay(100);
      BtReceive();
      //SdLog("New message: ");
      int msgLen = strlen(gBtMsg);
      bool foundKnownMAC = false;
      char m[4];
//      SdLog(itoa(msgLen, m, 10));
      if(msgLen > 0){
        
        if(msgLen >= 12){
          char* doneMsg = &gBtMsg[msgLen - 12];
          
          gBtMsg[12] = '\0';
          
          SdLog(gBtMsg);  

          for(int i = 0; i < gBtKnownMACTotal && !foundKnownMAC; i++){
            if(!strcmp(gBtMsg, gBtKnownMAC[i])){
              foundKnownMAC = true;
              matchedMac = i;
            }
          }
          
          //delay(2 * 1000);
          if (foundKnownMAC) {
            SdLog("MATCH!!!");
          } else {
            SdLog("No good :(");
          }

          //delay(2 * 1000);
          
          if(!strcmp(doneMsg, "Inquiry Done")){
//            digitalWrite(lightB,HIGH);

            SdLog(doneMsg);
            
            if(matchedMac != -1){     
              SdLog("found 1 known mac");

              gBtCmd[0] = 'C';
              gBtCmd[1] = ',';
              
              for(int i = 0; i < 12; i++){
                gBtCmd[i + 2] = gBtKnownMAC[matchedMac][i];
                //gBtMsg[i];
              }
              
              gBtCmd[15] = '\0';
                  
              BtSend("$$$", false);
              BtSend(gBtCmd);
              BtSend("---");      

              delay(2000);
//              digitalWrite(lightG,HIGH);
              
              while(!btConnect){
                delay(1000);
                BtSend("$$$", false);
                BtSend("GK");
                
                int numVal = 0;
      
                if(strlen(gBtMsg) > 0) {
                  numVal= atoi(gBtMsg);
                }
                
                if(numVal == 1){
                  btConnect = true;
                  SdLog("Is connected !!!!!!");
                }
                BtSend("---");
              }
            } else {
              SdLog("Unable to find match - reset to try again");
            }
          }
        }
      }
    }
    
//    digitalWrite(lightG,LOW);
//    digitalWrite(lightB,LOW);
    msgToken = 1;
  } else{
    SdLog(">>>> Set To Slave <<<<");
    BtSend("SM,0");
    BtSend("---"); 
    
    while(!btConnect){

      BtSend("$$$", false);
      BtSend("GK");
      
      int numVal = 0;
      
      if(strlen(gBtMsg) > 0) {
        numVal= atoi(gBtMsg);
      }
      
      if(numVal == 1){                  
        btConnect = true;
        SdLog("Is connected !!!!!!");
      }

      BtSend("---");              
    }
  }
  
  SdLog("#### end ####");
//  digitalWrite(lightR,LOW);
}

void SdLog(char* msg) {
  if (strlen(msg) > 0) {
//    Serial.println(msg);
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);

    display.setCursor(0, 0);
    display.print(msg);
    display.display();
    delay(500);
  }
}

void setup() {
  pinMode(selectPin, INPUT);

  // set up the LCD's number of columns and rows: 
  display.begin(SSD1306_SWITCHCAPVCC);
  display.clearDisplay();
  display.display();  

  SdLog("Ready");
  
  BtInit();
  time = millis();
}

boolean near_centre(int value) {
  return (8 < value && value < 12);
}

void loop() {
  int FSRReading = analogRead(FSR_Pin); 
  
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  
  int joystick_x;  
  int joystick_y;  
  int select;  
  joystick_x = map(analogRead(joystick_xPin), 0, 1023, 1, 20);  
  joystick_y = map(analogRead(joystick_yPin), 0, 1023, 1, 20);  
  select = !digitalRead(selectPin);

  double val = (double)FSRReading / 900;
  
  char speed = 0x00;
  char direction = 0x10;
  
  if((oldX != joystick_x) || (oldY != joystick_y) || (oldSelect != select)) {
    oldX = joystick_x;
    oldY = joystick_y;
    oldSelect = select;
  }
  
  if (joystick_x == 10 && joystick_y == 10) { // stop
    direction = 0x00;
  } else if (near_centre(joystick_x) && 10 < joystick_y) { // forward
    direction = 0x10;
  } else if (near_centre(joystick_x) && joystick_y < 10) { // backwards
    direction = 0x20;
  } else if (joystick_x > 10 && near_centre(joystick_y)) { // left no drive
    direction = 0x30;
  } else if (10 > joystick_x && near_centre(joystick_y)) { // right no drive
    direction = 0x40;
  } else if (joystick_x > 10 && joystick_y > 10) { // left forward
    direction = 0x50;
  } else if (10 > joystick_x && joystick_y > 10) { // right forward
    direction = 0x60;
  } else if (joystick_x > 10 && 10 > joystick_y) { // left backwards
    direction = 0x70;
  } else if (10 > joystick_x && 10 > joystick_y) { // right backwards
    direction = 0x80;
  }

  
  display.print("X: ");
  display.print(joystick_x);
  display.print(" Y: ");
  display.print(joystick_y);
  if (select) {  
    display.print(" select");
  }  
  
  if (FSRReading < step * 1) {
//    display.print("Stop");
    speed = 0x00;
  } else if (FSRReading < step * 2) {
//    display.print("Slowest");
    speed = 0x01;
  } else if (FSRReading < step * 3) {
//    display.print("Way Way Slow");
    speed = 0x02;
  } else if (FSRReading < step * 4) {
//    display.print("Way Slow");
    speed = 0x03;
  } else if (FSRReading < step * 5) {
//    display.print("Less Way Slow");
    speed = 0x04;
  } else if (FSRReading < step * 6) {
//    display.print("Slow");
    speed = 0x05;
  } else if (FSRReading < step * 7) {
//    display.print("Easy-Going");
    speed = 0x06;
  } else if (FSRReading < step * 8) {
//    display.print("Cruising");
    speed = 0x07;
  } else if (FSRReading < step * 9) {
//    display.print("Moving Right Along");
    speed = 0x08;
  } else if (FSRReading < step * 10) {
//    display.print("Moving Quick");
    speed = 0x09;
  } else if (FSRReading < step * 11) {
//    display.print("Moving Quicker");
    speed = 0x0A;
  } else if (FSRReading < step * 12) {
//    display.print("Moving Pretty Quick");
    speed = 0x0B;
  } else if (FSRReading < step * 13) {
//    display.print("Fast");
    speed = 0x0C;
  } else if (FSRReading < step * 14) {
//    display.print("Faster");
    speed = 0x0D;
  } else if (FSRReading < step * 15) {
//    display.print("Fastest");
    speed = 0x0E;
  } else if (FSRReading >= step * 15) {
//    display.print("THIS is fastest");
    speed = 0x0F;
  }  

  drawForce(val);

  display.display();
  
  unsigned long now = millis();
  
  if (now - time > 100) {
    if (direction == 0x00 && speed > 0x00) {
      direction = 0x10;
    }
    Serial.write(speed + direction);
    time = millis();
  }
  
  delay(10);
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

