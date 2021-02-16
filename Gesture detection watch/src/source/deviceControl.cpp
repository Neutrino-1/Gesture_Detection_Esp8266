#include "../include/deviceControl.h"

// REPLACE WITH RECEIVER MAC Address 

uint8_t broadcastAddressRev[] = {0x84, 0xCC, 0xA8, 0x83, 0x76, 0xBE};

void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus)
{
     Serial.print("Last Packet Send Status: ");
  if (sendStatus == 0){
    Serial.println("Delivery success");
  }
  else{
    Serial.println("Delivery fail");
  }
}

void setupEspNow()
{
 // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);   
  esp_now_register_send_cb(OnDataSent);   // this function will get called once all data is sent
  esp_now_add_peer(broadcastAddressRev, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);

  Serial.println("ESP NOW Initialized.");
}

