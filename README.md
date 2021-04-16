# Gesture_Detection_Esp8266

A Gesture detection watch based on esp8266 and Random Forest Algorithm. 
This watch determine the gesture performed and trasmitt over ESP-NOW protocol to "Smart-Extension-Box" which can in turn control home appliance

Current Watch Feature inlcudes:
* Auto display turn on and turn off
* Gesture detection
* NTP time sync
* Haptic feedback

![Gesture detection](https://github.com/Neutrino-1/Gesture_Detection_Esp8266/blob/master/_Demo/slam.gif)

Current Extension box Feature inlcudes:
* ESP-NOW integrated
* Can control 4 different appliance
* Has high current protection

![Control devices](https://github.com/Neutrino-1/Gesture_Detection_Esp8266/blob/master/_Demo/Appliance%20control%20gif.gif)

### Getting Started

You can build both the watch as well as the smart extension box from the circuit diagram.

For the smart extension box just upload "Esp_now_receiver.ino" code
For the watch open the folder "Gesture detection watch" with **VScode** along with **Platform**.io extension.
all the other necessary files will open along with it.

Change the external library to your arduino library path in **platformio.ini** file

```
lib_extra_dirs = C:\Users\*USER NAME*\Documents\Arduino\libraries
```

### Library Prerequisites

* [ESP-Core](https://github.com/esp8266/Arduino) - by Ivan Grokhotkov
* [TimeLib](https://github.com/PaulStoffregen/Time) - by PaulStoffregen
* [Adafruit 6050](https://github.com/adafruit/Adafruit_MPU6050) - adafruit
* [Wifi Manager](https://github.com/tzapu/WiFiManager) - tzapu
* [OLED](https://github.com/ThingPulse/esp8266-oled-ssd1306) - ThingPulse

### Installing

Open the Arduino IDE and follow the below steps:
```
Sketch -> include library -> ManageLibrary -> *Search for the above libraries*
```
Or
follow the instructions on respective library repository.

More detailed explanation : [Youtube link](https://www.youtube.com/watch?v=T60t42GjSB0&list=PLE8R81lmjiMwe2pfSp-84f4UspCjc2v2A)

## License 
[LICENSE.md](https://github.com/Neutrino-1/Gesture_Detection_Esp8266/blob/master/LICENSE.md) file for details
