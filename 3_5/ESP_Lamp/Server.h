#include "MQTT.h"
#include <ESP8266WebServer.h>


ESP8266WebServer server(web_port);

void handle_root(){
  String page_code = "<form action=\"/wifi\" method=\"POST\">";
  page_code += "SSID: <input type=\"text\" name=\"ssid\"><br>";
  page_code += "Password: <input type=\"text\" name=\"password\"><br>";
  page_code += "<input type=\"submit\" value=\"Connect\">";
  page_code += "</form>";

  server.send(200, "text/html", page_code);
}

void handle_wifi() {
  String ssid = server.arg("ssid");
  String pass = server.arg("password");

  char ssid_char[ssid.length() + 1];
  ssid.toCharArray(ssid_char, ssid.length() + 1);
  char pass_char[pass.length() + 1];
  pass.toCharArray(pass_char, pass.length() + 1);

  if (init_wifi(false, ssid_char, pass_char)) {
    init_MQTT();
    String topic = "sofa/command";
    mqtt_client.subscribe(topic.c_str());
    Serial.println("See me at " + topic);
  }

  server.sendHeader("Location", "/");
  server.send(303);
}


void server_init(){
  server.on("/", HTTP_GET, handle_root);
  server.on("/wifi", HTTP_POST, handle_wifi);
  server.begin();
  Serial.println("HTTP server is on, on port " + String(web_port));
}
