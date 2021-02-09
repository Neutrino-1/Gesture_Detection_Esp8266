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

// Callback function that will be executed when data is received
void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&msg, incomingData, sizeof(msg));
  Serial.print("Relay - 1");
  Serial.println(msg.relay1);
  Serial.print("Relay - 2");
  Serial.println(msg.relay2);
  Serial.print("Relay - 3");
  Serial.println(msg.relay3);
  Serial.print("Relay - 4");
  Serial.println(msg.relay4);
  Serial.println();
}
 
void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
  
}
