#include <ESP8266WiFi.h>
#include <espnow.h>

// REPLACE WITH RECEIVER MAC Address
uint8_t receiverAddress[] = {0x48, 0x3F, 0xDA, 0x05, 0xC8, 0xFB};   //48:3F:DA:05:C8:FB


// Structure example to send data
// Must match the receiver structure
typedef struct struct_message {
    int relay1;
    int relay2;
    int relay3;
    int relay4;
} struct_message;

// Create a struct_message called myData
struct_message myData;

int relayStatus_1 = 0;
int relayStatus_2 = 0;
int relayStatus_3 = 0;
int relayStatus_4 = 0;

// Callback when data is sent
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.print("Last Packet Send Status: ");
  if (sendStatus == 0){
    Serial.println("Delivery success");
  }
  else{
    Serial.println("Delivery fail");
  }
}
 
void setup() {
  // Init Serial Monitor
  Serial.begin(115200);
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);   
  esp_now_register_send_cb(OnDataSent);   // this function will get called once all data is sent
  esp_now_add_peer(receiverAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);

  Serial.println("Initialized.");
}
 
void loop() {
  if (Serial.available()) {
    
    char serialData = Serial.read();
    Serial.println(serialData);
   if(serialData == 'a')
   {
    relayStatus_1 = !relayStatus_1;
     myData.relay1 = relayStatus_1; 
   }
   else if(serialData == 'b')
   {
    relayStatus_2 = !relayStatus_2;
      myData.relay2 = relayStatus_2;    
   }
   else if(serialData == 'c')
   {   relayStatus_3 = !relayStatus_3;
      myData.relay3 = relayStatus_3;    
   }
   else if(serialData == 'd')
   {
     relayStatus_4 = !relayStatus_4;
      myData.relay4 = relayStatus_4;    
   }
    // Send message via ESP-NOW
    esp_now_send(receiverAddress, (uint8_t *) &myData, sizeof(myData));
  }
}
