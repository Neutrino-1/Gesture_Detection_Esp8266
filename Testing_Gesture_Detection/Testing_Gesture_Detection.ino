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

#include <ESP8266WiFi.h>
#include <espnow.h>

// REPLACE WITH RECEIVER MAC Address
uint8_t receiverAddress[] = {0x84, 0xCC, 0xA8, 0x83, 0x76, 0xBE}; // 84:CC:A8:83:76:BE

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
    if(gesture == "LeftSwipeGesture")
   {
    relayStatus_1 = !relayStatus_1;
     myData.relay1 = relayStatus_1; 
   }
   else if(gesture == "RightSwipeGesture")
   {
    relayStatus_2 = !relayStatus_2;
      myData.relay2 = relayStatus_2;    
   }
   else if(gesture == "SlamGesture")
   {   relayStatus_3 = !relayStatus_3;
      myData.relay3 = relayStatus_3;    
   }
   esp_now_send(receiverAddress, (uint8_t *) &myData, sizeof(myData));
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

  Serial.println("ESP NOW Initialized.");
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
