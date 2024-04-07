//  DHT20.cpp and DHT20.h provided by Rob Tillaart, code for readings taken and edited from demo for DHT20
//     URL: https://github.com/RobTillaart/DHT20
//  RTClib.h, RTClib.cpp, and RTC_DS3231.cpp provided by Adafruit contributors
//     URL: https://github.com/adafruit/RTClib
//  Adafruit_I2CDevice.h and Adafruit_I2CDevice.cpp provided by Adafruit contributors
//  readline method taken from LTEDemo on Botletics github contributed to by Timothy Woo and David Bendory
//  https://github.com/botletics/Botletics-SIM7000/blob/main/examples/LTE_Demo/LTE_Demo.ino

//  File written and completed by Joseph Carter 03/20/2024
//  File to control Valve and Lights for a Planter for maintaining plants, menu included for user control using USB cable

// Humidity and Moisture readings handled by DHT20 header file
#include "DHT20.h"
//Create DHT20 object
DHT20 DHT;

//Default Hour to turn on is 9 am and default Hour off is 6 pm.
int Hron = 9;
int Hroff = 18;
//Create SoilMoistureValue variable
int soilMoistureValue = 0;
//Set Dry and wet thresholds
const int DryValue = 400; 
const int WetValue = 360; 

// Date and time functions using a DS3231 RTC connected via I2C and Wire lib
#include "RTClib.h"
// readline method to read multiple chars for changing time on and timeoff, see note above
uint8_t readline(char *buff, uint8_t maxbuff, uint16_t timeout = 0);

//Create DS3231 object named rtc, days of week are specified for setting time, but not used in code otherwise.
RTC_DS3231 rtc;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};


void setup () {
  Serial.begin(9600);
  Wire.begin();
  DHT.begin();
  //Set pinmode of 4 and 7 to output so that they will output full 5V
  pinMode(4, OUTPUT);
  pinMode(7, OUTPUT);
  if (! rtc.begin()) {
    Serial.flush();
    while (1) delay(10);
  }
  //Print menu for user on startup
  printmenu();
}
// Menu of user choices printed to Serial monitor
void printmenu(void) {
  Serial.println();
  Serial.println(" Automated Planter Control System Menu:");
  Serial.println("***************************************");
  Serial.println(" 1. Print Clock time");
  Serial.println(" 2. Print Humidity and Temperature Reading");
  Serial.println(" 3. Set time for lights on and lights off");
  Serial.println(" 4. Print Current lights on and lights off time");
  Serial.println(" 5. Reset Time to file compilation");
  Serial.println(" Enter Number for Selection in Message box of Serial Monitor");
}


void loop () {

    DateTime now = rtc.now();
    while (Serial.available() > 0) {
    
    //char variable named option created and set to be equal to next user input, selection is printed for debug purposes
    char option = Serial.read();
    Serial.println(option);
    //Switch case based on option started, each option will print the menu again to remind user of options
    switch (option) {
      //Print only time on clock, excludes date info.
        case '1': {
          Serial.print("Current time: ");
          Serial.print(now.hour(), DEC);
          Serial.print(':');
          Serial.print(now.minute(), DEC);
          Serial.print(':');
          Serial.print(now.second(), DEC);
          Serial.println();
          printmenu();
          break;
        }
        //Reads Humidity, temperature and soil moisture reading at current time. DHT20 reading code taken and edited from DHT20 demo by Rob Tillaart
        // https://github.com/RobTillaart/DHT20/blob/master/examples/DHT20/DHT20.ino
        case '2':{
          //Starts reading humidity and temperature data
          uint32_t start = micros();
          int status = DHT.read();
          uint32_t stop = micros();

      Serial.println();
      Serial.println("Humidity (%)\tTemp (°C)\tTime (µs)");

    //Displays data up to 1 decimal point
    Serial.print(DHT.getHumidity(), 1);
    Serial.print("\t\t");
    Serial.print(DHT.getTemperature(), 1);
    Serial.print("\t\t");
    Serial.print(stop - start);
    Serial.print("\t\t");
    Serial.print("\n");
    soilMoistureValue = analogRead(A0);
    Serial.println("Soil Moisture Reading:");
    Serial.println(soilMoistureValue);
    printmenu();
    break;
          
   }
  // Updates timeon and timeoff based on user input
        case '3': {
          
          // Array to hold hour input
          char time[2];
          // Clears serial port to avoid errors, without this 48 is added to input
          while (Serial.available())
              Serial.read();
          //user input collected for hour for lights to turn off, input converted into integer
          Serial.println("Input Hour for Lights to turn off(00-23)");
          while (! Serial.available() ) {
          }
          readline(time, 1);
          Hroff = atoi(time);
          //check just in case user accidentaly inputs time over allowed time
          if(Hroff>23){
            Serial.println("Not a valid time, try again.");
            printmenu();
            break;
          }
          //Print inputted Hroff for debugging
          Serial.println("This is Hroff");
          Serial.println(Hroff);
          //Similar input collection to above
          Serial.println("Input Hour for Lights to turn on(00-23)");
          while (! Serial.available() ) {
          }
          readline(time, 1);
          Hron = atoi(time);
          //similar check to above
          if(Hron>23){
            Serial.println("Not a valid time, try again.");
            printmenu();
            break;
          }
          Serial.println("This is Hron");
          Serial.println(Hron);
          Serial.println();
          printmenu();
          break;
        }
        // prints message Telling time lights turn off and time lights turn on
        case '4': {
          
          Serial.print("Time off  ");
          Serial.print(Hroff);
          Serial.print(":00");
          Serial.println();
          Serial.print("Time on   ");
          Serial.print(Hron);
          Serial.print(":00");
          printmenu();
          break;
        }
        //Sets time on real time clock to file compilation time
        case '5': {
          rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
        }
        //Catches errors in input such as inputting non numerical options or numbers outside of options.
        default : {
          
          Serial.println("Input doesn't match any menu options try again.");
          printmenu();
          break;
        }
    }
    }
  
  //Two if loops used in case user wants lights to turn on earlier in day or later in day
  if(Hron<Hroff){
    //If time on real time clock is higher than time lights should turn on, write 5V to light switch
    if(now.hour()>=Hron && now.hour()<Hroff){
      digitalWrite(7,HIGH);
      //digitalWrite(8,HIGH);
    }
    //If time on real time clock is greater than time lights should turn off, write 0V to light switch
    else if (now.hour()>=Hroff){
      digitalWrite(7,LOW);
      //digitalWrite(8,LOW);
    }
  }
  else{
    //similar to above 
    if(now.hour()>=Hron){
      digitalWrite(7,HIGH);
      //digitalWrite(8,HIGH);
    }
    //similar to above
    else if (now.hour()>=Hroff && now.hour()<Hron){
      digitalWrite(7,LOW);
      //digitalWrite(8,LOW);
    }
  }
  
  //Read soil moisture sensor, then compare value to dry and wet threshold to determine if plant needs to be watered
  soilMoistureValue = analogRead(A0);
  //If the moisture reading is higher than the dryvalue (If sensor is drier than dry threshold) write 5V to relay
  if(soilMoistureValue>DryValue){
    digitalWrite(4,HIGH);
  }
  //If moisture reading is lower than wetvalue (If sensor is wetter than wet threshold) write 0V to relay
  else if (soilMoistureValue<WetValue){
    digitalWrite(4,LOW);

  }
  
}
// Used to collect multiple char input, taken from LTEDemo from botletics github contributed by Timothy Woo and David Bendory
// https://github.com/botletics/Botletics-SIM7000/blob/main/examples/LTE_Demo/LTE_Demo.ino
uint8_t readline(char *buff, uint8_t maxbuff, uint16_t timeout) {
  uint16_t buffidx = 0;
  boolean timeoutvalid = true;
  if (timeout == 0) timeoutvalid = false;

  while (true) {
    if (buffidx > maxbuff) {
      break;
    }

    while (Serial.available()) {
      char c =  Serial.read();

      if (c == '\r') continue;
      if (c == 0xA) {
        if (buffidx == 0)   // the first 0x0A is ignored
          continue;

        timeout = 0;         // the second 0x0A is the end of the line
        timeoutvalid = true;
        break;
      }
      buff[buffidx] = c;
      buffidx++;
    }

    if (timeoutvalid && timeout == 0) {
      break;
    }
    delay(1);
  }
  buff[buffidx] = 0;  // null term
  return buffidx;
}
