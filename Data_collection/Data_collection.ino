// Basic demo for accelerometer readings from Adafruit MPU6050
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

//File system to store the guesture
#include <FS.h>
#include <LittleFS.h>

//display for the oled
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)

#define NUM_SAMPLES 100
#define NUM_AXES 6
#define TRUNCATE 20
#define ACCEL_THRESHOLD 5
int INT_PIN  = 0;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Adafruit_MPU6050 mpu;

int sample_read = 0; //

double baseline[NUM_AXES];

float features[NUM_AXES * NUM_SAMPLES];

volatile boolean addFeature = false;

volatile boolean isCaptureStarted = false;

void ICACHE_RAM_ATTR IntCallback()
{
  addFeature = true;  
  isCaptureStarted = true;
}

void setup(void) {
  Serial.begin(115200);
  pinMode(INT_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(INT_PIN), IntCallback, FALLING);
  if (!LittleFS.begin()) {
    Serial.println("LittleFS mount failed");
    return;
  }
  while(!isCaptureStarted)
  {
    delay(10);
  }
  readFile();
  
    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
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

void loop()
{
   /* Get new sensor events with the readings */
  
  float ax, ay, az;
  float gx, gy, gz;
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  if(addFeature)
  {
    File file = LittleFS.open("./Gesture.txt", "a");
  if (!file) {
    Serial.println("Failed to open file for appending");
    return;
  }
    if (file.print("\n\t\tNew Feature\n\n\n")) {
    Serial.println("Message appended");
  } else {
    Serial.println("Append failed");
  }
  file.close();
  addFeature = false;
  testdrawchar("Added new feature!");
  delay(1000);
  testdrawchar("start recording");
  }

    ax = constrain(a.acceleration.x - baseline[0], -TRUNCATE, TRUNCATE);
    ay = constrain(a.acceleration.y - baseline[1], -TRUNCATE, TRUNCATE);
    az = constrain(a.acceleration.z - baseline[2], -TRUNCATE, TRUNCATE);
    gx = constrain(g.acceleration.x - baseline[0], -TRUNCATE, TRUNCATE);
    gy = constrain(g.acceleration.y - baseline[1], -TRUNCATE, TRUNCATE);
    gz = constrain(g.acceleration.z - baseline[2], -TRUNCATE, TRUNCATE);

 if (!motionDetected(ax, ay, az)) {
      delay(10);
      return;
    }
    recordIMU();
    delay(100);
}

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
    testdrawchar("Start recording");
}

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

    // print the data in CSV format
    //appendFile(""+String(ax)+","+String(ay)+","+String(az)+","+String(gx)+","+String(gy)+","+String(gz)+"\n");
    int index = i*NUM_AXES;
    features[index] = ax;
    features[index+1] = ay;
    features[index+2] = az;
    features[index+3] = gx;
    features[index+4] = gy;
    features[index+5] = gz;
    }
    for(int i = 0; i < 10; i++)
    {
      Serial.println();
    }
     for(int i = 0; i < NUM_SAMPLES;i++)
    {
      int index = i*NUM_AXES;
      String data = String(features[index])+","+String(features[index+1])+","+String(features[index+2])+","+(features[index+3])+","+(features[index+4])+","+(features[index+5])+"\n"; 
      Serial.print(data);
    } 
    appendFile();   
}

bool motionDetected(float ax, float ay, float az) {
    return (abs(ax) + abs(ay) + abs(az)) > ACCEL_THRESHOLD;
}

void testdrawchar(String text) {
  display.clearDisplay();
  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner
  display.print(text);
  display.display();
  delay(1000);
}

void readFile() {
  File file = LittleFS.open("./Gesture.txt", "r");
  if (!file) {
    Serial.println("Failed to open file for reading");
    return;
  }

  Serial.print("Read from file: ");
  while (file.available()) {
    Serial.write(file.read());
  }
  file.close();
}

void appendFile() {
  String data;
  File file = LittleFS.open("./Gesture.txt", "a");
  if (!file) {
    Serial.println("Failed to open file for appending");
    return;
  }
  for(int i = 0; i < NUM_SAMPLES;i++)
  {
      int index = i*NUM_AXES;
      data = String(features[index])+","+String(features[index+1])+","+String(features[index+2])+","+(features[index+3])+","+(features[index+4])+","+(features[index+5])+"\n"; 
  if (file.print(""+data)) {
    Serial.println("Message appended");
  } else {
    Serial.println("Append failed");
  }
  }
  
   if (file.print("\n\n")) {
    Serial.println("added space");
  } else {
    Serial.println("Append failed");
  }
  file.close();
  testdrawchar("done recording!");
  delay(1000);
  testdrawchar("start recording!");
}
