#include <ESP8266WiFi.h>
#include <espnow.h>

// Structure example to receive data
// Must match the sender structure
typedef struct MessageFromWatch {
    int id;
    int relay_1;
    int relay_2;
    int relay_3;
} MessageFromWatch;

// Create a struct_message called myData
MessageFromWatch relayData;

// Callback function that will be executed when data is received
void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&relayData, incomingData, sizeof(relayData));
  Serial.print("Int: ");
  Serial.println(relayData.relay_1);
  Serial.print("Int: ");
  Serial.println(relayData.relay_2);
  Serial.print("Int: ");
  Serial.println(relayData.relay_3);
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
