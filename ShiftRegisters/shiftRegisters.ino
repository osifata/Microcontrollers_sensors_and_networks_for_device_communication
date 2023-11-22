// Latch pin (ST_CP) of 74HC595 to pin 5
int latchPin = 5;      
// Clock pin (SH_CP) of 74HC595 to pin 3
int clockPin = 3;
// Data pin (DS of 74HC595 to pin 7
int dataPin = 7;      

// 2 3 4 5 6 7 8 9 
// A B . C D E G F
byte digits[10] = {
  0b11011101,  // 0
  0b01010000,  // 1
  0b11001110,  // 2
  0b11011010,  // 3
  0b01010011,  // 4
  0b10011011,  // 5
  0b10111111,  // 6
  0b11010000,  // 7
  0b11011111,  // 8
  0b11111011   // 9
};


bool flag = false;
int time_digits[4] = {};// MM:SS (no colon)
unsigned long previous_millis = 0;
const long interval = 1000; // 1 second interval to update time

void setup(){
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  Serial.begin(9600);
}

void loop(){
  if (!flag && Serial.available() >= 4) {
    for(int i = 0; i < 4; i++) {
     time_digits[i] = (int)(Serial.read() - '0');
    }
    flag = true;
  }
  if (flag && (millis() - previous_millis >= interval)) {
    previous_millis = millis();
    time_digits[3]++;
    if (time_digits[3] > 9){
      time_digits[3] = 0;
      time_digits[2]++;
      if (time_digits[2] > 5){
        time_digits[2] = 0;
        time_digits[1]++;
        if (time_digits[1] > 9){
          time_digits[1] = 0;
          time_digits[0]++;
          if (time_digits[0] > 5){
            time_digits[0] = 0;
          }
        }
      }
    }
    for(int i = 0; i < 4; i++){
      show_number(time_digits[i]);
    }
  }
}

void show_number(int digit)
{
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, LSBFIRST, digits[digit]); 
  digitalWrite(latchPin, HIGH);
}
