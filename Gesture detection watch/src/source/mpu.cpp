#include "../include/mpu.h"
Adafruit_MPU6050 mpu;

float features[NUM_AXES * NUM_SAMPLES]; //stores the gesture (before detection)
double baseline[NUM_AXES]; // Used for callibration

Eloquent::ML::Port::RandomForest classifier; //This class is inside the model.h file

// REPLACE WITH RECEIVER MAC Address (A4:CF:12:D9:25:66)
uint8_t broadcastAddress[] = {0x2C, 0xF4, 0x32, 0x5E, 0x1E, 0x64};

//Create a struct_message called relay values
struct_message relay_values;

bool guestureStarted = false;
int toggle = 0;
void setupMPU()
{
    Serial.println("find MPU6050 chip");
    if (!mpu.begin())
    {
        Serial.println("Failed to find MPU6050 chip");
        while (1)
        {
            delay(10);
        }
    }
    mpu.setAccelerometerRange(MPU6050_RANGE_16_G);
    mpu.setGyroRange(MPU6050_RANGE_250_DEG);
    mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
    delay(100);
}

boolean calculateMotion(int currentDisplay)
{
    float ax, ay, az;
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);
    
    ax = constrain(a.acceleration.x - baseline[0], -TRUNCATE, TRUNCATE);
    ay = constrain(a.acceleration.y - baseline[1], -TRUNCATE, TRUNCATE);
    az = constrain(a.acceleration.z - baseline[2], -TRUNCATE, TRUNCATE);

    if (motionDetected(ax, ay, az) && currentDisplay == 1) {
        
        Serial.println("Motion detected!");
        delay(10);
        recordIMU();
        classify();
        return true;
    }

    if (a.gyro.x > 5 && (a.acceleration.y < 3 || a.acceleration.y > -3)) //Detect motion only when Y axis parallel to ground
    {
        guestureStarted = true;
    }
    else if (a.gyro.x < -5)
    {
        if (guestureStarted)
        {
            guestureStarted = false;
            return true;
        }
    }
    

    return false;
}


/*This function will do the detection*/
void classify() {
    WiFi.mode(WIFI_STA);
    Serial.print("Detected gesture: ");
    String gesture = classifier.predictLabel(features);
    Serial.println(gesture);
    setAction(gesture);
    delay(1000);
    if(gesture == "Left swipe gesture")
    {
        setAction("Turned on light");
        relay_values.relay_1 = 1;
    }
    else if(gesture == "Right swipe gesture")
    {
        setAction("Turned off light");
        relay_values.relay_1 = 0;
    }
    else if(gesture == "Slam gesture")
    {   
        relay_values.relay_2 = !toggle;
        setAction("Toggle soldering station!");
    }
    esp_now_send(broadcastAddress, (uint8_t *) &relay_values, sizeof(relay_values));
    delay(3000);
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

void calibrate()
{
    sensors_event_t a, g, temp;
    Serial.println("Calibrating...");
    for (int i = 0; i < 10; i++) {
        mpu.getEvent(&a, &g, &temp);
        delay(100);
    }
     Serial.println("done Calibrating!");
    baseline[0] = double(a.acceleration.x);
    baseline[1] = double(a.acceleration.y);
    baseline[2] = double(a.acceleration.z); 
    baseline[3] = double(g.acceleration.z); 
    baseline[4] = double(g.acceleration.z); 
    baseline[5] = double(g.acceleration.z);  
}