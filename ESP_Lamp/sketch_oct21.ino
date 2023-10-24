#include "Config.h"
#include "wifi.h"
#include "Server.h"

void setup() {
  Serial.begin(9600);
  pinMode(led_pin, OUTPUT);
  start_AP_mode();
  server_init();
}

void loop() {
   server.handleClient();
  if (WiFi.status() == WL_CONNECTED) {
    mqtt_client.loop();
    delay(100);
  } else {
    start_AP_mode();
  }
}
