#include <ESP8266WiFi.h>
#include <espnow.h>

// Structure example to receive data
// Must match the sender structure
typedef struct message {
    int relay1;
    int relay2;
    int relay3;
    int relay4;
} message;

// Create a struct_message called myData
message msg;

int relayStatus_1 = 0;
int relayStatus_2 = 0;
int relayStatus_3 = 0;
int relayStatus_4 = 0;

bool relayStatusChange = true;

// Callback function that will be executed when data is received
void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&msg, incomingData, sizeof(msg));
  Serial.print("Relay - 1");
  Serial.println(msg.relay1);
  relayStatus_1 = msg.relay1;
  
  Serial.print("Relay - 2");
  Serial.println(msg.relay2);
  relayStatus_2 = msg.relay2;
  
  Serial.print("Relay - 3");
  Serial.println(msg.relay3);
  relayStatus_3 = msg.relay3;
  
  Serial.print("Relay - 4");
  Serial.println(msg.relay4);
  relayStatus_4 = msg.relay4;
  Serial.println();

  relayStatusChange = true;
}
 
void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  pinMode(5,OUTPUT);
  pinMode(4,OUTPUT);
  pinMode(12,OUTPUT);
  pinMode(14,OUTPUT);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();        // we do not want to connect to a WiFi network

  if(esp_now_init() != 0) {
    Serial.println("ESP-NOW initialization failed");
    return;
  }

  esp_now_register_recv_cb(OnDataRecv);   // this function will get called once all data is sent

  Serial.println("Initialized.");
}

void loop() {
  if(relayStatusChange)
  {
    digitalWrite(5,relayStatus_1); 
    digitalWrite(14,relayStatus_2); 
    digitalWrite(4,relayStatus_3); 
    digitalWrite(12,relayStatus_4); 
  }
}
