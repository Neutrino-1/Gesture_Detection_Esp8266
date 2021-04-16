#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"

#define DHTPIN 2 
#define DHTTYPE    DHT11
DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "INTERNET";//setting your ap ssid
const char* password = "ZXCV@098";//setting your ap psk
const char* mqttServer = "iot.reyax.com";
const char* mqttUserName = "RRfNu9f2du";  // MQTT username
const char* mqttPwd = "RWS6mVdw8P";  // MQTT password
const char* clientID = "RRfNu9f2du00001"; // client id
const char* topic = "api/request"; //publish topic
 
unsigned long prevMillis = 0;
const long interval = 1000;
String msgStr = "";      // MQTT message buffer

String DHT_Reading = "";
 
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
      client.subscribe("api/response/notification/get@RRfNu9f2du00001"); 
      Serial.println("api/response/notification/get@RRfNu9f2du00001");
      
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
  dht.begin();
  client.setServer(mqttServer, 1883);
  client.setCallback(callback);
  client.setBufferSize(1024);
  client.setKeepAlive(60);
}
 
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  {
            delay(15000);
             
            msgStr="{\"action\": \"notification/insert\",\"deviceId\": \"vVdz6ZQPDSobfnBs5qNMUjnSpU50pZ0s8EI8\",\"notification\": {\"notification\": \"DHT\",\"parameters\": {\"DHT_Value\": \""+DHT_Reading+"\"}}}";

            byte arrSize = msgStr.length() + 1;
            char msg[arrSize];
 
            Serial.print("PUBLISH DATA:");
            Serial.println(msgStr);
            msgStr.toCharArray(msg, arrSize);
            client.publish(topic, msg);
            msgStr = "";
            delay(50);
         }
  
  }

//subscribe call back
void callback(char*topic, byte* payload, unsigned int length) {
  Serial.print("Messagearrived in topic: ");
  Serial.println(topic);
  Serial.print("Message:");
  for (int i = 0; i< length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();  
  Serial.print("Message size :");
  Serial.println(length);
  Serial.println();
  Serial.println("-----------------------");
}

void getSensorData()
{
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  } 

  DHT_Reading = "hum :"+String(h)+"temp :"+String(h);
}
