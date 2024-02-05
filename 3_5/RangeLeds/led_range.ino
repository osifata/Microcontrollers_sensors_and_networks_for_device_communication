#include "Config.h"
#include "WIFI.h"
#include "MQTT.h"
#include "Range.h"

void setup() {
  Serial.begin(9600);
  WIFI_init(false); // Initialize in client mode
  MQTT_init(); // Initialize MQTT
  range_init(); // Initialize the rangefinder
}

void loop() {
  mqtt_cli.loop(); // Ensure MQTT connection is alive
  int range = get_range(); // Get the distance measurement
  mqtt_cli.publish(TOPIC_DATA, String(range).c_str()); // Publish the distance
  delay(1000); // Wait for a second before next measurement
}