//display library for the oled
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// For accelerometer and gyro readings from MPU6050
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include "model.h" //Replace this with your model file

#define TRUNCATE 20 // To limit the readings between +/- 20
#define ACCEL_THRESHOLD 5 //Adjust this to change when the detection should trigger
#define NUM_SAMPLES 100 //Number of smaples to be read, during motion detection
#define NUM_AXES 6 //acc 3 axis + gyro 3 axis

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)

float features[NUM_AXES * NUM_SAMPLES]; //stores the gesture (before detection)
int INT_PIN  = 0; //Button interrupt
volatile boolean StartDetection = false; //When to detect
double baseline[NUM_AXES]; // Used for callibration

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Adafruit_MPU6050 mpu;

Eloquent::ML::Port::RandomForest classifier; //This class is inside the model.h file

//Make sure the interrupt call back function is above the setup function
void ICACHE_RAM_ATTR IntCallback()
{
  StartDetection = true;
}

/*Code for callibration
 *Reads the value from mpu when the device is still 
 *set that as the offest for the reading
*/
void calibrate()
{
    sensors_event_t a, g, temp;
    Serial.println("Calibrating...");
    testdrawchar("Calibrating...");
    for (int i = 0; i < 10; i++) {
        mpu.getEvent(&a, &g, &temp);
        delay(100);
    }
     Serial.println("done Calibrating!");
     testdrawchar("done Calibrating...");
    baseline[0] = double(a.acceleration.x);
    baseline[1] = double(a.acceleration.y);
    baseline[2] = double(a.acceleration.z); 
    baseline[3] = double(g.acceleration.z); 
    baseline[4] = double(g.acceleration.z); 
    baseline[5] = double(g.acceleration.z);  
    testdrawchar("Do some gesture!");
}

/*This function records the sensor data when the motion is triggered*/
void recordIMU() {
    float ax, ay, az;
    float gx, gy, gz;
    sensors_event_t a, g, temp;
    for (int i = 0; i < NUM_SAMPLES; i++) {
         mpu.getEvent(&a, &g, &temp);
  
    ax = constrain(a.acceleration.x - baseline[0], -TRUNCATE, TRUNCATE);
    ay = constrain(a.acceleration.y - baseline[1], -TRUNCATE, TRUNCATE);
    az = constrain(a.acceleration.z - baseline[2], -TRUNCATE, TRUNCATE);
    gx = constrain(g.acceleration.x - baseline[0], -TRUNCATE, TRUNCATE);
    gy = constrain(g.acceleration.y - baseline[1], -TRUNCATE, TRUNCATE);
    gz = constrain(g.acceleration.z - baseline[2], -TRUNCATE, TRUNCATE);

    int index = i*NUM_AXES;
    features[index] = ax;
    features[index+1] = ay;
    features[index+2] = az;
    features[index+3] = gx;
    features[index+4] = gy;
    features[index+5] = gz;
    
    }
}

/*This function detedcts the motion of the device*/
bool motionDetected(float ax, float ay, float az) {
    return (abs(ax) + abs(ay) + abs(az)) > ACCEL_THRESHOLD;
}

/*To draw text on the OLED screen*/
void testdrawchar(String text) {
  display.clearDisplay();
  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner
  display.print(text);
  display.display();
  delay(1000);
}

/*This function will do the detection*/
void classify() {
    Serial.print("Detected gesture: ");
    String gesture = classifier.predictLabel(features);
    Serial.println(gesture);
    testdrawchar(gesture);
    delay(2000);
    testdrawchar("Do some gesture!");
}

void setup() {
  Serial.begin(115200);
  pinMode(INT_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(INT_PIN), IntCallback, FALLING);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  while(!StartDetection)
  {
    delay(10);
  }
  testdrawchar("booting...");
    // Try to initialize!
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  mpu.setAccelerometerRange(MPU6050_RANGE_4_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  Serial.println("");
  delay(100);
  calibrate();
}

void loop() {
  // put your main code here, to run repeatedly:
    float ax, ay, az; 
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);
  
    ax = constrain(a.acceleration.x - baseline[0], -TRUNCATE, TRUNCATE);
    ay = constrain(a.acceleration.y - baseline[1], -TRUNCATE, TRUNCATE);
    az = constrain(a.acceleration.z - baseline[2], -TRUNCATE, TRUNCATE);

    if (!motionDetected(ax, ay, az)) {
        delay(10);
        return;
    }
    Serial.println("Motion detected!");
      recordIMU();
      classify(); 
}
