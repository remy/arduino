// These are indexes into the groundPins array
//#define MIDDLE 0  // G
//#define UPPER_L 1 // F
//#define LOWER_L 2 // E
//#define BOTTOM 3  // D
//#define LOWER_R 4 // C
//#define UPPER_R 5 // B
//#define TOP 6     // A

#define TOP 0     // A
#define UPPER_R 1 // B
#define LOWER_R 2 // C
#define BOTTOM 3  // D
#define LOWER_L 4 // E
#define UPPER_L 5 // F
#define MIDDLE 6  // G

int groundPins[7] = {2, 3, 4, 5, 6, 7, 8};
int digitPins[4] = {9, 10, 11, 12}; // positive voltage supply for each of the 4 digits
int ON = HIGH;
int OFF = LOW;
int number[10][7]; // holds information about which segments are needed for each of the 10 numbers
int digit[4]; // holds values for each of the 4 display digits

int pirPin = 13;

int hits = 0;

unsigned long start;

void setup()
{
  Serial.begin(9600);
  initNumber();
  
  pinMode(pirPin, INPUT);

  for(int i=0; i < 7; i++) {
    pinMode(groundPins[i], OUTPUT);
    digitalWrite(groundPins[i], HIGH);
  }
  for(int i=0; i < 4; i++) {
    pinMode(digitPins[i], OUTPUT);
    digitalWrite(digitPins[i], LOW);
  }
  
  start = millis();
}

void loop() {
  unsigned long time = millis() - start;
  int n = time / 1000;

  setDigit(n);

  for(int g=0; g < 7; g++) {
    digitalWrite(groundPins[g], LOW);
    for(int i=0; i < 4; i++) {
      if (digit[i] < 0) {
        continue;
      }
      digitalWrite(digitPins[i], number[digit[i]][g]);
    }
    delay(1);
    digitalWrite(groundPins[g], HIGH);
  }
}

void setDigit(int n) {
  n = n % 2000;
  digit[0] = n % 10;
  digit[1] = (n / 10) % 10;
  if ((digit[1] == 0) && (n < 10)) {
    digit[1] = -1;
  }
  digit[2] = (n / 100) % 10;
  if ((digit[2] == 0) && (n < 100)) {
    digit[2] = -1;
  }
  digit[3] = (n / 1000) % 10;
  if (digit[3] == 0) {
    digit[3] = -1;
  }
}

int getDelay() {
  if (millis() > 10000) {
    return 1;
  } else {
    return (int) (((10000 - millis()) / 10000.0) * 125);
  }
} 

void initNumber() {
  number[0][MIDDLE]  = OFF;
  number[0][UPPER_L] = ON;
  number[0][LOWER_L] = ON;
  number[0][BOTTOM]  = ON;
  number[0][LOWER_R] = ON;
  number[0][UPPER_R] = ON;
  number[0][TOP]     = ON;

  number[1][MIDDLE]  = OFF;
  number[1][UPPER_L] = OFF;
  number[1][LOWER_L] = OFF;
  number[1][BOTTOM]  = OFF;
  number[1][LOWER_R] = ON;
  number[1][UPPER_R] = ON;
  number[1][TOP]     = OFF;

  number[2][MIDDLE]  = ON;
  number[2][UPPER_L] = OFF;
  number[2][LOWER_L] = ON;
  number[2][BOTTOM]  = ON;
  number[2][LOWER_R] = OFF;
  number[2][UPPER_R] = ON;
  number[2][TOP]     = ON;

  number[3][MIDDLE]  = ON;
  number[3][UPPER_L] = OFF;
  number[3][LOWER_L] = OFF;
  number[3][BOTTOM]  = ON;
  number[3][LOWER_R] = ON;
  number[3][UPPER_R] = ON;
  number[3][TOP]     = ON;

  number[4][MIDDLE]  = ON;
  number[4][UPPER_L] = ON;
  number[4][LOWER_L] = OFF;
  number[4][BOTTOM]  = OFF;
  number[4][LOWER_R] = ON;
  number[4][UPPER_R] = ON;
  number[4][TOP]     = OFF;

  number[5][MIDDLE]  = ON;
  number[5][UPPER_L] = ON;
  number[5][LOWER_L] = OFF;
  number[5][BOTTOM]  = ON;
  number[5][LOWER_R] = ON;
  number[5][UPPER_R] = OFF;
  number[5][TOP]     = ON;

  number[6][MIDDLE]  = ON;
  number[6][UPPER_L] = ON;
  number[6][LOWER_L] = ON;
  number[6][BOTTOM]  = ON;
  number[6][LOWER_R] = ON;
  number[6][UPPER_R] = OFF;
  number[6][TOP]     = ON;

  number[7][MIDDLE]  = OFF;
  number[7][UPPER_L] = OFF;
  number[7][LOWER_L] = OFF;
  number[7][BOTTOM]  = OFF;
  number[7][LOWER_R] = ON;
  number[7][UPPER_R] = ON;
  number[7][TOP]     = ON;

  number[8][MIDDLE]  = ON;
  number[8][UPPER_L] = ON;
  number[8][LOWER_L] = ON;
  number[8][BOTTOM]  = ON;
  number[8][LOWER_R] = ON;
  number[8][UPPER_R] = ON;
  number[8][TOP]     = ON;

  number[9][MIDDLE]  = ON;
  number[9][UPPER_L] = ON;
  number[9][LOWER_L] = OFF;
  number[9][BOTTOM]  = ON;
  number[9][LOWER_R] = ON;
  number[9][UPPER_R] = ON;
  number[9][TOP]     = ON;
}
