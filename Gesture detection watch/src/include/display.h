#include "SSD1306Wire.h"
#include "OLEDDisplayUi.h"

void initDispaly();

void initUI();

void clearDisplay();

void setDisplayTime(String currentTime);

void uiControl();

void changeFrame();

void onDispaly();

void turnOffDisplay();

void setAction(String preditctedAction);

void setGraphingValue(int16_t x, int16_t y, int16_t lx, int16_t ly, int bpm);

int16_t remainingTimeBudget();

int currentDispaly();