#define SPEED_1      5 //правое колесо
#define DIR_1        4
 
#define SPEED_2      6//левое колесо
#define DIR_2        7

#define FD 10
#define CD 20

const int trig_pin_front = 9;
const int echo_pin_front = 3;

const int trig_pin_left = 11;
const int echo_pin_left = 10;

int time_rotate = 400;
float time_waiting = 200;

void setup() {
  for (int i = 4; i < 8; i++){
    pinMode(i, OUTPUT);
  }
  pinMode(trig_pin_front, OUTPUT);
  pinMode(echo_pin_front, INPUT);
  pinMode(trig_pin_left, OUTPUT);
  pinMode(echo_pin_left, INPUT);
}

void wheels(bool right_wheel, bool left_wheel, int right_speed, int left_speed){
  digitalWrite(DIR_1, right_wheel);
  digitalWrite(DIR_2, left_wheel);
  analogWrite(SPEED_1, right_speed);
  analogWrite(SPEED_2, left_speed);
}

void move_forward(int car_speed){
  wheels(true, false, car_speed, car_speed);
}

void move_back(int car_speed){
  wheels(false, true, car_speed, car_speed);
}

void rotate_right(int car_speed){
  wheels(true, true, car_speed, car_speed);
}

void rotate_left(int car_speed){
  wheels(false, false, car_speed, car_speed);
}

void stop(){
  wheels(true, true, 0, 0);
}

void turn_right(int car_speed, float steepness){
  if(steepness > 1){
    steepness = 1;
  }
  if(steepness < 0){
    steepness = 0;
  }
  wheels(true, false, car_speed, car_speed * steepness);
}

void turn_left(int car_speed, float steepness){
  if(steepness > 1){
    steepness = 1;
  }
  if(steepness < 0){
    steepness = 0;
  }
  wheels(true, false, car_speed * steepness, car_speed);
}

int dist(const int trig, const int echo){
  int duration, distance;
  digitalWrite(trig, LOW); 
  delayMicroseconds(2); 
  digitalWrite(trig, HIGH);
  delayMicroseconds(10); 
  digitalWrite(trig, LOW); 
  duration = pulseIn(echo, HIGH); 
  distance = duration / 58;
  return distance;
}

void turn_right_90(){
  float start_time = millis();
  while(millis() - start_time < time_rotate) {
    rotate_right(255);
  }
  stop();
}
void turn_left_90(){
  float start_time = millis();
  while(millis() - start_time < time_rotate) {
    rotate_left(255);
  }
  stop();
}

// Функция поворота на 270 градусов
void turn_270(){
  float start_time = millis();
  while(millis() - start_time < time_rotate*3) {
    rotate_right(255);
  }
  stop();
}

void loop() {
  int distance_front = dist(trig_pin_front, echo_pin_front);
  int distance_left = dist(trig_pin_left, echo_pin_left);

  static int state = 0; // Начальное состояние

  if (state == 0) {
    move_forward(255); // Начало -> есть ли левая стенка?
    if (distance_left < CD) { // Есть левая стенка
      if (distance_front > FD) { // Нет стены впереди
        time = millis();
        while(millis()-time < time_waiting){
          move_forward(255); // Двигаться вперед
        }
      } else { // Есть стена впереди
        state = 1; // Переход к следующему состоянию
      }
    } else { // Нет левой стенки
      state = 2; // Переход к следующему состоянию
    }
  } else if (state == 1) { // Пройдена левая стенка и есть стена впереди
      time = millis();
      while(millis()-time < time_waiting){
        turn_right_90(); // Повернуть направо
      }
      state = 0; // Вернуться в начальное состояние
  } else if (state == 2) { // Нет левой стенки
      if (distance_front < FD*2){ //есть стена впереди
        time = millis();
        while(millis()-time < time_waiting){
          turn_left_90(); // Повернуть налево
          move_forward(255); // Двигаться вперед
        }
        state = 0; // Вернуться в начальное состояние
    }
    else{
      time = millis();
      while(millis()-time < time_waiting){
        move_forward(255); // Двигаться вперед
      }
      state = 0; // Вернуться в начальное состояние
    }
  }
}
