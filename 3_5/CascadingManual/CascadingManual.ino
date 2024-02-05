#include <SoftwareSerial.h>

#define START_PIN 2
#define END_PIN 9
#define BAUD_RATE 9600

SoftwareSerial next_node(10, 11);
int num_node = 0;

bool digits[10][8] = {
  {1,1,0,1,1,1,0,1},  // 0
  {0,1,0,1,0,0,0,0},  // 1
  {1,1,0,0,1,1,1,0},  // 2
  {1,1,0,1,1,0,1,0},  // 3
  {0,1,0,1,0,0,1,1},  // 4
  {1,0,0,1,1,0,1,1},  // 5
  {1,0,1,1,1,1,1,1},  // 6
  {1,1,0,1,0,0,0,0},  // 7
  {1,1,0,1,1,1,1,1},  // 8
  {1,1,1,1,1,0,1,1}   // 9
};

void setup() {
  for (int i = START_PIN; i <= END_PIN; i++) {
    pinMode(i, OUTPUT);
    digitalWrite(i, HIGH);
  }
  Serial.begin(BAUD_RATE);
  next_node.begin(BAUD_RATE);
  while (Serial.available() == 0) {}
  while (next_node.available() != 0) {
    next_node.read();
  }
  next_node.write(Serial.read());
  count_of_nodes();
  if (num_node != 1) {
    num_node = num_node / 2;
    num_node += 1;
  }
  //выводит общее число плат
  Serial.print(num_node);
  //выводит число соседей для данной платы
  Serial.print(num_node - 1);
}

void loop() {
  if (Serial.available() >= 1) {
    delay(20);
    if (Serial.available() >= num_node) {
      char user_input = Serial.read();
      int digit = (int)(user_input - '0');
      show_digit(digit);
    } else {
      turn_off();
    }
    for (int i = num_node; i >= 1; i--) {
      if (Serial.available() >= i) {
        for (int j = i - 1; j >= 0; j--) {
          next_node.write(Serial.read());
        }
      }
    }
    while (Serial.available()) {
      next_node.write(Serial.read());
    }
  }
}

void show_digit(int digit) {
  if (digit < 0 || digit > 9) {
    return;
  }
  for (int i = START_PIN; i <= END_PIN; i++) {
    digitalWrite(i, !digits[digit][i - START_PIN]);
  }
}

void turn_off() {
  for (int i = 2; i < 10; i++) {
    digitalWrite(i, HIGH);
  }
}

void count_of_nodes() {
  Serial.write('n');
  delay(50);
  unsigned long start_time = millis();
  
  while (millis() - start_time < 1000) {
    if (next_node.available() > 0) {
      num_node++;
      while(next_node.available() > 0){
      	next_node.read();
      }
      delay(50);
      count_of_nodes();
    }
    else{
      while (next_node.available() > 0) {
        next_node.read();
      }
      while (Serial.available() > 0) {
        Serial.read();
      }
    }
  }
}
