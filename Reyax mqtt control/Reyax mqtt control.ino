#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid = "INTERNET";//setting your ap ssid
const char* password = "ZXCV@098";//setting your ap psk
const char* mqttServer = "iot.reyax.com";
const char* mqttUserName = "RRfNu9f2du";  // MQTT username
const char* mqttPwd = "RWS6mVdw8P";  // MQTT password
const char* clientID = "RRfNu9f2du50001"; // client id
const char* topic = "api/request"; //publish topic
 
unsigned long prevMillis = 0;
const long interval = 1000;
String msgStr = "";      // MQTT message buffer

 
WiFiClient espClient;
PubSubClient client(espClient);

 
void setup_wifi() {
  delay(10);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
 
void reconnect() {
  while (!client.connected()) {
    if (client.connect(clientID, mqttUserName, mqttPwd)) {
      Serial.println("MQTT connected");
      client.subscribe("api/command/80/#"); 
      client.subscribe("api/command_update/80/#");       
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);  // wait 5sec and retry
    }
  }
}
 
void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqttServer, 1883);
  client.setCallback(callback);

}
 
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  delay(1000);
  }

//subscribe call back
void callback(char*topic, byte* payload, unsigned int length) {
  Serial.print("Messagearrived in topic: ");
  Serial.println(topic);
  Serial.print("Message:");
  for (int i = 0; i< length; i++) {
    Serial.print((char)payload[i]);
    //get relay values here
  }
  Serial.println();  
  Serial.print("Message size :");
  Serial.println(length);
  Serial.println();
  Serial.println("-----------------------");
}
