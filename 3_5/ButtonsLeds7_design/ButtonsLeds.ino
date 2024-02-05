#define BUTTON_PIN 2
#define LED_PIN 11
#define PRESSED HIGH
#define DEBOUNCE_TIME 50
#define HOLD_TIME_STEP 500
#define DIM_STEP 65
#define MAX_BRIGHTNESS 255
#define MIN_BRIGHTNESS 0

volatile bool buttonPressed = false;
volatile bool buttonState = LOW;
volatile long buttonTime = 0;
volatile long lastDebounceTime = 0;

int led_brightness = 0;
bool led_on = false;
bool increaseBrightness = true;

void setup() {
  pinMode(BUTTON_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  attachInterrupt(0, interruptServiceRoutine, CHANGE);
  Serial.begin(9600);
}

void interruptServiceRoutine() {
  if ((millis() - lastDebounceTime) > DEBOUNCE_TIME) {
    buttonState = digitalRead(BUTTON_PIN);
    if(buttonState == PRESSED)
      buttonPressed = true;
      lastDebounceTime = millis();
  }
}

void loop() {

  if(buttonPressed == true) {
    led_on = !led_on;
    buttonPressed = false;

    if (led_on && led_brightness == 0) led_brightness = MAX_BRIGHTNESS;
    else if (!led_on) led_brightness = 0;

    analogWrite(LED_PIN, led_brightness); 
    buttonTime = millis();
    Serial.println("Button is pressed ");
  }

  else if(led_on && buttonState == PRESSED && millis() - buttonTime >= HOLD_TIME_STEP) {
    buttonTime = millis();
    if ( increaseBrightness ) {
      led_brightness += DIM_STEP;
      if ( led_brightness >= MAX_BRIGHTNESS) {
        increaseBrightness = false;
        led_brightness = MAX_BRIGHTNESS;
      }
    }
    else {
      led_brightness -= DIM_STEP;
      if ( led_brightness <= MIN_BRIGHTNESS ) {
        increaseBrightness = true;
        led_brightness = MIN_BRIGHTNESS;
      }
  }

  analogWrite(LED_PIN, led_brightness);
  Serial.println("Changed LED brightness");
  }
}