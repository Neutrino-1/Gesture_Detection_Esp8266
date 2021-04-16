#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"

#define DHTPIN 2 
#define DHTTYPE    DHT11
DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "INTERNET";//setting your ap ssid
const char* password = "ZXCV@098";//setting your ap psk
const char* mqtt_server = "192.168.0.104";
const char* topic = "sensor/dht"; //publish topic
 
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
    if (client.connect("DHTSensor")) {
      Serial.println("connected"); 
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
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}
 
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  if(!client.loop())
  {
    client.connect("DHTSensor");  
  }
            delay(15000);
            client.publish("sensor/dht",DHT_Reading.c_str());
            Serial.println("data published"+DHT_Reading);
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
