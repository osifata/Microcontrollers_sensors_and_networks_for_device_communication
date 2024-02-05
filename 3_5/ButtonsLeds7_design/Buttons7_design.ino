#define BUTTON_PIN 2 
#define BOUNCE_TIME 50
#define PRESSED LOW
#define START_PIN 3
#define END_PIN 10

volatile long int press_time = 0;
volatile bool check_button = false;
volatile int holding_time = 0;

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
  for(int i=START_PIN; i <=END_PIN; i++) {
  	pinMode(i, OUTPUT);
    digitalWrite(i, HIGH);
  }
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  Serial.begin(9600);
  attachInterrupt(0, process_button_click, RISING);
}

void loop() {
  if(check_button == true and
     millis() - press_time > BOUNCE_TIME and
     digitalRead(BUTTON_PIN) == PRESSED
    ) {
  	    holding_time = (millis() - press_time + 2999) / 3000;
        if (holding_time > 9) {
          holding_time = 0;
        }
        show_digit(holding_time);
  	    Serial.println("Pressed");
    	Serial.println(holding_time);
        check_button = false;
  }
}

void process_button_click() {
  if (check_button == false) {
    press_time = millis();
    check_button = true;
  }
}

void show_digit(int digit) {
  if (digit<0 or digit > 9) {
    return;
  }
  for(int i=START_PIN; i <=END_PIN; i++) {
    digitalWrite(i, !digits[digit][i-START_PIN]);
  }
}
