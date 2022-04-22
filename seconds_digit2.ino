#include "msp430.h"
#include "stdint.h"
#include "ws2812.h"
#include "Wire.h"
#include "PCF8523.h"
#include <SoftwareSerial.h>

#define TXD 4
#define RXD 3
#define dip1 5
#define dip2 6
#define dip3 7 
#define dip4 14

uint32_t currentDigit;
uint8_t address = 0;
uint8_t second;
uint8_t minute;
uint8_t hour;

SoftwareSerial RS485_serial (RXD, TXD);
/*
    Digit is a number between 0 to 9. 

    Show digit will turn on the specific color for the led row.
    All other digits will be turned off. 
*/

PCF8523 RTC; // Establishes the chipset of the Real Time Clock
RTC_Millis RTCM;

static void ShowDigit(uint32_t digit, ColorMap color);
void minimalClock(uint32_t currentDigit);

const unsigned long MCLK_HZ = 16000000;          // SMCLK frequency in Hz

void setup()
{

   if (digitalRead(dip1) == HIGH) {
      address += 8;
   }
   if (digitalRead(dip2) == HIGH) {
      address += 4;
   }
   if (digitalRead(dip3) == HIGH) {
      address += 2;
   }
   if (digitalRead(dip4) == HIGH) {
      address += 1;
   }

  
   
   Serial.begin(4800); // Starts the serial communications
   Serial.print("Program ");

   /*
   if (!RTC.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while(1);
   }
    RTC.adjust(DateTime(F(__DATE__), F(__TIME__)));
*/
/*
   if (CALBC1_16MHZ==0xFF)    // If calibration constant erased
    {
        while(1);              // do not load, trap CPU!!
    }

    // configure clock to 16 MHz
    BCSCTL1 = CALBC1_16MHZ;    // DCO = 16 MHz
    DCOCTL = CALDCO_16MHZ;
    */
    initStrip(); 
    showStrip(); 
    /*
    // Start RTC
    Wire.begin(); // Starts the Wire library allows I2C communication to the Real Time Clock
    RTC.begin(); // Starts communications to the RTC
    */
}


void loop()
{

  uint32_t msg[14];

  for (int i = 0; i < 14; i++) {
    if (Serial.available()) {
      msg[i] = Serial.read();
    }
  }

  for (int i = 0; i < 10; i++) {
    if ((msg[i] == 0xAB) && (msg[i+4] == 0xBA)) {
      second = msg[i+1];
      minute = msg[i+2];
      hour = msg[i+3];
    }
  }

 switch(address) {
    case 0:
      currentDigit = hour%12;
      break;
    case 1:
      currentDigit = (hour%12)/10;
      break;
    case 2:
      currentDigit = minute%10;
      break;
    case 3:
      currentDigit = minute/10;
      break;
    case 4:
      currentDigit = second%10;
      break;
    case 5:
      currentDigit = second/10;
      break;
    default: 
      break;
   }
    

  
  /*RTCM.adjust(DateTime(F(__DATE__), F(__TIME__)));
  DateTime now = RTC.now();
  RTCM.adjust(DateTime(now.year(), now.month(), now.day(), 0, now.minute(), now.second()));*/
  minimalClock(currentDigit);
  Serial.print("Hour time is... ");
  //Serial.println(now.hour());
  Serial.print("Min time is... ");
  //Serial.println(now.minute());
  Serial.print("Sec time is... ");
  //Serial.println(now.second());
  Serial.print("Year is... ");
  //Serial.println(now.year());
  Serial.print("Month is... ");
  //Serial.println(now.month());
  Serial.print("Day is... ");
  //Serial.println(now.day());

  
}

void minimalClock(uint32_t currentDigit)
{
    //uint32_t currentDigit = now.second()/10;
    ShowDigit(currentDigit, COLOR_GREEN);
}


static void ShowDigit(uint32_t digit, ColorMap color)
{
    clearStrip();
    setLEDColor(digit, COLOR_GREEN);
    setLEDColor(19-digit, COLOR_GREEN);
    setLEDColor(20+digit, COLOR_GREEN);
    setLEDColor(39-digit, COLOR_GREEN);
    setLEDColor(40+digit, COLOR_GREEN);    
    showStrip();
}
