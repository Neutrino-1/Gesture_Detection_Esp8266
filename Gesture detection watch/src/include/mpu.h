#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <ESP8266WiFi.h>
#include "../include/model.h"
#include "../include/display.h"
#include "../include/deviceControl.h"

// Must match the receiver structure
typedef struct struct_message {
    int relay1;
    int relay2;
    int relay3;
    int relay4;
} struct_message;

#define TRUNCATE 20 // To limit the readings between +/- 20
#define ACCEL_THRESHOLD 5 //Adjust this to change when the detection should trigger
#define NUM_SAMPLES 100 //Number of smaples to be read, during motion detection
#define NUM_AXES 6 //acc 3 axis + gyro 3 axis

void setupMPU();
bool calculateMotion(int currentDisplay);
void classify();
void recordIMU();
void calibrate();
bool motionDetected(float ax, float ay, float az);