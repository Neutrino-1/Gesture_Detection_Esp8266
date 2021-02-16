#include "../include/main.h"

void setup()
{
  // put your setup code here, to run once:
  pinMode(navButton, INPUT_PULLUP);
  pinMode(13, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(0), ISR, FALLING);
  Serial.begin(115200);
  Serial.println("Starting...");
  startWiFiManager();
  initDispaly();
  delay(100);
  setupMPU();
  while (wifiConnected());
  startTime();
  delay(3000);
  calibrate();
  initUI();
  disconnetWifi();
  idleTime = millis();
  setupEspNow();
}

void loop()
{
  if (remainingTimeBudget() > 0)
  {
  }

  if (pressed)
  {
    changeFrame();
    pressed = false;
  }

  if (timeStatus() != timeNotSet)
  {
    if (now() != prevDisplay)
    { //update the display only if time has changed
      prevDisplay = now();
      setDisplayTime(digitalClockValue());
      //Serial.println(digitalClockValue());
    }
  }

  if (calculateMotion(currentDispaly()))
  {
    if (!displayOnStatus)
    {
      onDispaly();
      displayOnStatus = true;
    }
    idleTime = millis();
  }
  else if (!calculateMotion(currentDispaly()) && displayOnStatus && millis() - idleTime > 15000)
  {
    displayOnStatus = false;
    turnOffDisplay();
    idleTime = millis();
  }
}

ICACHE_RAM_ATTR void ISR()
{
  if (!displayOnStatus)
  {
    onDispaly();
    displayOnStatus = false;
  }
  else
  {
    pressed = true;
  }
}
