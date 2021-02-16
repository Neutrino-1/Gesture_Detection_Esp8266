#include "../include/wifiControl.h"

void startWiFiManager()
{
    // WiFi.mode(WIFI_STA);
    // WiFi.begin("INTERNET","ZXCV@098");
    // WiFi.setAutoReconnect(true);
    WiFiManager wifiManager;
    wifiManager.setDebugOutput(false);
    if (!wifiManager.autoConnect("Enter Credentials", "password"))
    {
        Serial.println("failed to connect, we should reset as see if it connects");
        delay(3000);
        ESP.reset();
        delay(5000);
    }
}

void disconnetWifi()
{
    WiFi.mode(WIFI_AP);
    WiFi.channel(1);
}

boolean wifiConnected()
{
    return WiFi.status() != WL_CONNECTED;
}
