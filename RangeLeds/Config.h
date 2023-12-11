String ssidAP = "ESP_WIFI_Sofa"; // имя контроллера и точки доступа
String passwordAP = "ESP8266123"; // пароль точки доступа

char* TOPIC_HELLO = "lab/sensors/stair/range/state";
char* TOPIC_DATA = "lab/sensors/stair/range/data";

char* ssidCLI = "Jojo";
char* passwordCLI = "Lalali-lolo";

char* mqtt_broker = "broker.emqx.io";

const int mqtt_port = 1883;

const int led = 2;

const int trigPin = 12; 
const int echoPin = 14;  