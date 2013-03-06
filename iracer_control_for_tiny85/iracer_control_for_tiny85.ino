int incomingByte = 0;
char gBtMsg[256];
char gBTAdr[13];
char gBtCmd[256];

char* gBtKnownMAC = "001205119333"; // MAC adress of the iRacer

char* inquiryDone = "Inquiry Done";

// ATtiny85 pin configuration: http://hlt.media.mit.edu/?p=1695
//const int A1 = 2;
//const int A2 = 4;
//const int A3 = 3;

// pressure sensor values
float step = 900/16;
int FSR_Pin = A1; //analog pin 0

bool btConnect = false;

// joystick
const int joystick_xPin = A2; // H  
const int joystick_yPin = A3; // V

const int rx=0; // goes in to TX
const int tx=1; // goes in to RX

#include <SoftwareSerial.h>

SoftwareSerial mySerial(rx, tx);
//SoftwareSerial mySerial(2, 3); // RX, TX

long _map(long x, long in_min, long in_max, long out_min, long out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


void BtReceive(void){
  bool keepReading = true;
  int index = 0;
  
  gBtMsg[0] = '\0';
  
  while(keepReading){
    keepReading = false;
    
    if (mySerial.available() > 0) {
      // read the incoming byte:
      incomingByte = mySerial.read();
      if(incomingByte != 13){
        gBtMsg[index++] = incomingByte;
        keepReading = true;
      }
    }
  }
  
  gBtMsg[index] = '\0';
  
  if (index > 0) {
//  Serial.print("<- ");
//  Serial.println(gBtMsg);
  
    delay(100);
  }
}

void BtSend(char* i_pBtCmd, bool i_ln = true){

  if(i_ln){
    mySerial.println(i_pBtCmd);
  } else{
    mySerial.print(i_pBtCmd);
  }
  
//  Serial.print("-> ");
//  Serial.println(i_pBtCmd);
  
  delay(100);
  BtReceive();
}

void testConnection(void) {
  // test if we're connected already
  int numVal = 10;

  while(numVal > 1){
    delay(1000);
    BtSend("$$$", false);
    BtSend("GK");
    
    if(strlen(gBtMsg) > 0) {
      numVal = atoi(gBtMsg);
    }
    
    BtSend("---");
  }

  // 1 means connected, 0 means we're not
  if(numVal == 1){
    btConnect = true;
  }
}

// original BT code (though I've adapted quite a bit):
// http://www.instructables.com/id/BlueTooth-Link-with-auto-detect-connect/?ALLSTEPS
void BtInit(void){
  bool matchedMac = false;
  bool foundKnownMAC = false;
  
  testConnection();

  if (btConnect) {
    return;
  }
    
  BtSend("$$$", false);
  
  // set to bluetooth module to master
  BtSend("SM,1");
  BtSend("I,5");
  BtSend("---"); 
   
  while(!btConnect){
    delay(100);
    BtReceive();
    
    int msgLen = strlen(gBtMsg);
    
    if(msgLen >= 12){
      char* doneMsg = &gBtMsg[msgLen - 12];
      
      gBtMsg[12] = '\0';
      
      if(!foundKnownMAC && !strcmp(gBtMsg, gBtKnownMAC)){
        foundKnownMAC = true;
        matchedMac = true;
      }
      
      if(matchedMac && !strcmp(doneMsg, inquiryDone)){      
        // Had to comment this code out in favour of hard coding the 
        // connection command C,<mac addr> because it was causing
        // the entire ATtiny85 to crash and not run - regardless
        // of whether this code was encountered.
//          gBtCmd[0] = 'C';
//          gBtCmd[1] = ',';
//          
//          for(int i = 0; i < 12; i++){
//            gBtCmd[i + 2] = gBtKnownMAC[i];
//          }
//                              
//          gBtCmd[15] = '\0';
              
          BtSend("$$$", false);
//          BtSend(gBtCmd);
          BtSend("C,001205119333");
          BtSend("---");      

          delay(2000);
          
          while(!btConnect){
            testConnection();
          }
      } else {
//        Serial.println("Unable to find match - reset to try again");
      }
    }
  }
}

void setup() {
//  Serial.begin(9600);
  
  // IMPORTANT: SoftwareSerial can't run at 115200bps - it causes
  // the response to be entirely garbled, so you need to slow 
  // down to 9600 - then the response is readable
  // via: http://arduino.cc/forum/index.php?PHPSESSID=59e3c7945ecb957e269045f1c6bd9c76&topic=74548.msg562633#msg562633

  pinMode(rx,INPUT);
  pinMode(tx,OUTPUT);

  mySerial.begin(9600);          // Start bluetooth serial at 9600

  delay(250);
  
  BtInit();
}

boolean near_centre(int value) {
  return (8 < value && value < 12);
}

void loop() {  
  int FSRReading = analogRead(FSR_Pin);
  
  int joystick_x;  
  int joystick_y;  
  // maps our value from 1 through 20 (ATtiny85 didn't seem to have map function)
  joystick_x = _map(analogRead(joystick_xPin), 0, 1023, 1, 20);  
  
  // to get the compiler past this line, I had to patch the ld binary via 
  // https://github.com/TCWORLD/ATTinyCore/tree/master/PCREL%20Patch%20for%20GCC
  joystick_y = _map(analogRead(joystick_yPin), 0, 1023, 1, 20);  

  double val = (double)FSRReading / 900;
  
  char speed = 0x00;
  char direction = 0x10;
  
  if (joystick_x == 10 && joystick_y == 10) { // stop
    direction = 0x00;
  } else if (near_centre(joystick_x) && 10 < joystick_y) { // forward
    direction = 0x10;
  } else if (near_centre(joystick_x) && joystick_y < 10) { // backwards
    direction = 0x20;
  } else if (joystick_x > 10 && near_centre(joystick_y)) { // left no drive
//    direction = 0x30; // we'd never want to be stationary
    direction = 0x50; 
  } else if (10 > joystick_x && near_centre(joystick_y)) { // right no drive
//    direction = 0x40; // also never be still if entirely left or right
    direction = 0x60;
  } else if (joystick_x > 10 && joystick_y > 10) { // left forward
    direction = 0x50;
  } else if (10 > joystick_x && joystick_y > 10) { // right forward
    direction = 0x60;
  } else if (joystick_x > 10 && 10 > joystick_y) { // left backwards
    direction = 0x70;
  } else if (10 > joystick_x && 10 > joystick_y) { // right backwards
    direction = 0x80;
  }
  
  // TODO change to use map - then the speed can read directly from the map
  if (FSRReading < step * 1) {
    speed = 0x00;
  } else if (FSRReading < step * 2) {
    speed = 0x01;
  } else if (FSRReading < step * 3) {
    speed = 0x02;
  } else if (FSRReading < step * 4) {
    speed = 0x03;
  } else if (FSRReading < step * 5) {
    speed = 0x04;
  } else if (FSRReading < step * 6) {
    speed = 0x05;
  } else if (FSRReading < step * 7) {
    speed = 0x06;
  } else if (FSRReading < step * 8) {
    speed = 0x07;
  } else if (FSRReading < step * 9) {
    speed = 0x08;
  } else if (FSRReading < step * 10) {
    speed = 0x09;
  } else if (FSRReading < step * 11) {
    speed = 0x0A;
  } else if (FSRReading < step * 12) {
    speed = 0x0B;
  } else if (FSRReading < step * 13) {
    speed = 0x0C;
  } else if (FSRReading < step * 14) {
    speed = 0x0D;
  } else if (FSRReading < step * 15) {
    speed = 0x0E;
  } else if (FSRReading >= step * 15) {
    speed = 0x0F;
  }
  
  if (direction == 0x00 && speed > 0x00) {
    direction = 0x10;
  }
  
//  Serial.println(speed + direction);
  
  mySerial.write(speed + direction);

  delay(250);
}

