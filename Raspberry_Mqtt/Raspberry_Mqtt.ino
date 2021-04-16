#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid = "INTERNET";
const char* password = "ZXCV@098";

//Raspberry Pi IP address so it can connect to MQTT broker
const char* mqtt_server = "192.168.0.108";

// Initializes the espClient
WiFiClient espClient;
PubSubClient client(espClient);

const int relay_1 = 5;
const int relay_2 = 4;
const int relay_3 = 12;
const int relay_4 = 14;

int relay_status_1 = 0;
int relay_status_2 = 0;
int relay_status_3 = 0;
int relay_status_4 = 0;

boolean stateChange = true;

//Connects to internet
void setup_wifi() {
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi connected - ESP IP address: ");
  Serial.println(WiFi.localIP());
}

// This functions prints the value topic, Whenever there is change in the topic.
void callback(String topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }

  if(topic == "relayControl/relay_1"){
      relay_status_1 = messageTemp.toInt();
  }
  else if(topic == "relayControl/relay_2"){
      relay_status_2 = messageTemp.toInt();
  }
  else if(topic == "relayControl/relay_3"){
      relay_status_3 = messageTemp.toInt();
  }
  else if(topic == "relayControl/relay_4"){
      relay_status_4 = messageTemp.toInt();
  }
  
  Serial.println();
  stateChange = true;
}

// This functions reconnects your ESP8266 to your MQTT broker
// Change the function below if you want to subscribe to more topics with your ESP8266 
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("SmartExtensionBox")) {
      Serial.println("connected");  
      // Subscribe or resubscribe to a topic
      client.subscribe("relayControl/relay_1");
      client.subscribe("relayControl/relay_2");
      client.subscribe("relayControl/relay_3");
      client.subscribe("relayControl/relay_4");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000); 
    }
  }
}

// The callback function is what receives messages from the topic
void setup() {
  pinMode(relay_1, OUTPUT);
  pinMode(relay_2, OUTPUT);
  pinMode(relay_3, OUTPUT);
  pinMode(relay_4, OUTPUT);

  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}
 
// It ensures that you smart extension box is connected to your broker
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  if(!client.loop())
    client.connect("SmartExtensionBox");

  if(stateChange)
  {
    digitalWrite(relay_1,relay_status_1);
    digitalWrite(relay_2,relay_status_2);
    digitalWrite(relay_3,relay_status_2);
    digitalWrite(relay_4,relay_status_2); 
    stateChange = false; 
  }
}
