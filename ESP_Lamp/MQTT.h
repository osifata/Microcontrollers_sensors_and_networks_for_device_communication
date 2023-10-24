#include <PubSubClient.h>

PubSubClient mqtt_client(wifi_client);

void callback(char* topic, byte* payload, unsigned int length){
  Serial.println("Message received on topic: ");
  Serial.println(topic);
  Serial.println("Message is: ");
  for (int i=0; i < length; i++){
    Serial.println((char)payload[i]);
  }
  Serial.println();

   if ((char)payload[0] == 'o') {
    digitalWrite(led_pin, LOW);
  }
  else if((char)payload[0] == 'f') {
    digitalWrite(led_pin, HIGH);
  } 
}

void init_MQTT(){
  mqtt_client.setServer(mqtt_broker, mqtt_port);
  mqtt_client.setCallback(callback);
  while(!mqtt_client.connected()){
    Serial.println("Trying to connected");
    Serial.println(mqtt_broker);
    String client_id = "esp8266_" + id();
    bool success = mqtt_client.connect(client_id.c_str());
    if (success){
      Serial.println("Successfully connected with " + client_id);
    }else{
      Serial.println("Failed to connect with " + client_id);
      Serial.println(mqtt_client.state());
      delay(2000);
    }
  }
}

