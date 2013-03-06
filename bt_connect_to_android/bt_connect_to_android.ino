#include <SoftwareSerial.h>

int masterPin = 4;
int isMaster = 0;
int msgToken = 0;

int incomingByte = 0;
char gBtMsg[256];
char gBTAdr[13];
char gBtCmd[256];

int gBtKnownMACTotal = 1;
char* gBtKnownMAC[1]; //This is set to hold only two MAC adresses

#define OLED_DC 11
#define OLED_CS 12
#define OLED_CLK 10
#define OLED_MOSI 9   //(Pin on OLED labeled DATA)
#define OLED_RESET 13

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
    delay(1000);
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
    delay(1000);
  //Debug end

//  delay(100);
  BtReceive();
  
}


void BtInit(void){
  isMaster = digitalRead(masterPin); 
  bool btConnect = false;
//  digitalWrite(lightR,HIGH);

  // gBtKnownMAC[0] = "10BF48F4C9F0"; // nexus 7
  gBtKnownMAC[0] = "001205119333"; // iracer
  
  int matchedMac = -1;
  
  SdLog("#### start ####");
 
  Serial.begin(115200); 
  BtSend("$$$", false);
  
  if(true){
    SdLog(">>>> Set To Master <<<<");

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
    delay(1000);
  }
}

void setup() {
//  Serial.begin(9600);
  // set up the LCD's number of columns and rows: 
  display.begin(SSD1306_SWITCHCAPVCC);
  display.clearDisplay();
  display.display();  

  SdLog("Ready");
  
  BtInit();
}

void receiveMsg(){  
  for(int i = 0; i < 10; i++){
    if (Serial.available() > 0) {
      // read the incoming byte:
      incomingByte = Serial.read();
      if(incomingByte == 'R'){
        SdLog("receiving message.");
//        digitalWrite(lightB, LOW);
        delay(50);
        SdLog("receiving message...");
//        digitalWrite(lightB,HIGH);
      }
    }
    
    delay(100);
  }
}

void sendMsg(){
  delay(50);  
  Serial.print("R");
  delay(50);  
  Serial.print("R");
  delay(50);  
  Serial.print("R");
}

void loop() 
{  
//  return;
  if(msgToken == 1){
    sendMsg();
    delay(1000);
  }else{
    receiveMsg();
  }
}

