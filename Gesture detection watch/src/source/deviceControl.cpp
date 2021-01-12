#include "../include/deviceControl.h"

// REPLACE WITH RECEIVER MAC Address (A4:CF:12:D9:25:66)
//2C:F4:32:5E:1E:64

uint8_t broadcastAddressRev[] = {0x2C, 0xF4, 0x32, 0x5E, 0x1E, 0x64};

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
  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  esp_now_add_peer(broadcastAddressRev, ESP_NOW_ROLE_SLAVE,1, NULL, 0);
  Serial.println(esp_now_get_peer_channel(broadcastAddressRev));
  delay(500); 
}

