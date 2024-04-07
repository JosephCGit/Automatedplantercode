# Plantercode readme
This is the repository for the plantercode to make it easily accesible without having to email the file around. I also will include helpful links here.

# Libraries
This project uses the DHT20 and RTClib libraries to work, you can use the Arduino IDE to install the libraries or visit there githubs to retrieve the necessary files.

Credit to Rob Tillaart for DHT20 library located [here](https://github.com/RobTillaart/DHT20), you just need to download the DHT20.h and DHT20.cpp files into the project folder.

Credit to Adafruit and contributors for the RTClib library, it is split between two githubs one is the [RTClib](https://github.com/adafruit/RTClib/tree/master/src), just download the DS3231.cpp and RTClib.h and RTC.cpp files. 

The other one is the [Adafruit_BusIO](https://github.com/adafruit/Adafruit_BusIO) just download the Adafruit_I2CDevice.h and Adafruit_I2CDevice.cpp.

Credit to Timothy Woo and David Bendory for Readline method used in project. Taken from [LTEDemo.ino](https://github.com/botletics/Botletics-SIM7000/blob/main/examples/LTE_Demo/LTE_Demo.ino).

The code is subject to GNU public license v3.0 as per the terms of the license for the Botletics-SIM7000 [github](https://github.com/botletics/Botletics-SIM7000/blob/main/LICENSE). However it is also subject to the MIT license as included in the repositories of RTClib, Adafruit_BUSIO, and [DHT20](https://github.com/RobTillaart/DHT20/blob/master/LICENSE).
