#define d2_pin 2
#define d3_pin 3
#define d4_pin 4
#define d5_pin 5

int butt_pin[] = {d4_pin, d5_pin};
bool butt_state[2][2];
bool d2_val[] = {LOW, HIGH};
bool d3_val[] = {HIGH, LOW};

String butt_pressed = "Pressed: ";
bool any_butt_press = false;

void setup() {
  pinMode(d2_pin, OUTPUT);
  pinMode(d3_pin, OUTPUT);
  pinMode(d4_pin, INPUT_PULLUP);
  pinMode(d5_pin, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {
  for (int k = 0; k < 2; k++) {
    digitalWrite(d2_pin, d2_val[k]);
    digitalWrite(d3_pin, d3_val[k]);

    for (int i = 0; i < 2; i++) {
      butt_state[k][i] = digitalRead(butt_pin[i]);
    }
  }
  for (int k = 0; k < 2; k++) {
    for (int i = 0; i < 2; i++) {
      if (!butt_state[k][i]) {
        if (any_butt_press) {
          butt_pressed += " and ";
        }
        butt_pressed += String(k * 2 + i + 1);
        any_butt_press = true;
      }
    }
  }

  if (any_butt_press) {
    Serial.println(butt_pressed);
  }

  delay(100);
}