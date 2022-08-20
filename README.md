# Overview
This is a simple project for my close relative "smart LED Lamp" aka addressable led that shine trought crystal to create a pretty nice effect. Color/Brightness... etc. can be changed via 2 buttons. It use 2x refurbished LG LGABC21865, nominal capacity was 2800mAh current capacity is slightly above 2000mAh after cell test.

## What does project contains?
### Source code:
- Libraries used: [Adafruit NeoPixel](https://github.com/adafruit/Adafruit_NeoPixel), [EEPROM](https://docs.arduino.cc/learn/built-in-libraries/eeprom), [Arduino default library <Arduino.h>](https://github.com/arduino/ArduinoCore-avr/blob/master/cores/arduino/Arduino.h)
- [My code](/src/main.cpp)

### Schematic:
Check out: [Schematic](/docs/schematics/main.pdf)

### Info about used Hardware:

- [Charging board](https://oshwlab.com/Little_Arc/TP4056) You can buy it everywhere, Amazon, Aliexpress... etc. Now with USB C also. **I did a small modification to mine, set the charging current to 580mA instead of 1000mA. (To prevent chip overheating and when using bad input charger) Visible in schematic 2kΩ resistor instead of 1,2kΩ IC TP4056, PIN 2(PROG)**
- [Boost Converter board](https://oshwlab.com/Little_Arc/MT3608) **For stable operation and limit the voltage ripple from switching. It's recommended to use smoothing capacitator, I set my boost converter to 4,6V because higher voltage will result only in higher power losses.**
- [My own ATtiny85 boar](/docs/images/ATtiny85%20board.jpg), [ATtiny85 datasheet](https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-2586-AVR-8-bit-Microcontroller-ATtiny25-ATtiny45-ATtiny85_Datasheet.pdf)
- 2x 18650 accumulators (You can use what ever type of Lion/LiPo **1S only!**)
- 3D printed case. You will need to design your own because of different shape for crystal (or what ever you want to use).

## Get started
1. Install [Visual Studio Code](https://code.visualstudio.com/)
2. Install [PlatformIO IDE](https://platformio.org/) extension (Depending on your Python version you can get error like [this](https://community.platformio.org/t/platformio-installation-failed/17419))
![PlatfromIO Image1](https://github.com/Vassterak/ESP32-LEDControl/blob/main/docs/media/installation/install1.JPG)
3. Clone repository
4. Open PlatformIO inside Visual Studio Code and click on **Open Project** and import cloned repository. 1. select PlatformIO (RED icon) 2. Import project (BLUE icon)
![PlatfromIO Image2](https://github.com/Vassterak/ESP32-LEDControl/blob/main/docs/media/installation/install2.JPG)
5. Check if you have all required libraries
![PlatfromIO Image3](https://github.com/Vassterak/ESP32-LEDControl/blob/main/docs/media/installation/install3.JPG)

6. How to upload new code to ATtiny85? Use other Arduino in ISP mode. [Tutorial.](https://www.instructables.com/How-to-Program-an-Attiny85-From-an-Arduino-Uno/)
## Result
<p align="center">
  <img src="/docs/images/LEDLamp.gif" alt="video of finished project">
</p>


## License
[License](/LICENSE)
