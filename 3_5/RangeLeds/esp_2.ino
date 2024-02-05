#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h>
#include "Config.h"

// Setup for LEDs
const int ledPins[] = {D5, D6, D7}; // Update with the actual GPIO pins used for LEDs
const int number_of_leds = 3;

// Function to light up LEDs based on distance
void lightLEDsBasedOnDistance(int distance) {
  for (int i = 0; i < number_of_leds; i++) {
    digitalWrite(ledPins[i], LOW); // Turn off all LEDs initially
  }
  
  int ledsToLight = distance > 10 ? 1 : (distance > 5 ? 2 : 3);
  
  for (int i = 0; i < ledsToLight; i++) {
    digitalWrite(ledPins[i], HIGH); // Turn on the LED
  }
}

// Callback function for incoming MQTT messages
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  String message;
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  Serial.println(message);
  lightLEDsBasedOnDistance(message.toInt());
}

// Initialize WiFi and MQTT
WiFiClientSecure espClient;
PubSubClient client(espClient);

void setup_wifi() {
  // Connect to WiFi network
  WiFi.begin(ssidCLI, passwordCLI);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void setup() {
  Serial.begin(9600);
  setup_wifi(); // Connect to Wi-Fi
  espClient.setInsecure();
  client.setServer(mqtt_broker, mqtt_port);
  client.setCallback(callback);

  // Initialize LED pins
  for (int i = 0; i < number_of_leds; i++) {
    pinMode(ledPins[i], OUTPUT);
  }

  // Connect to MQTT Broker
  if (!client.connected()) {
    while (!client.connected()) {
      Serial.print("Connecting to MQTT Broker...");
      if (client.connect("ESP8266LEDSubscriber")) {
        Serial.println("connected");
        client.subscribe(TOPIC_DATA); // Subscribe to the distance topic
      } else {
        Serial.print("failed with state ");
        Serial.print(client.state());
        delay(2000);
      }
    }
  }
}

void loop() {
  client.loop();
}