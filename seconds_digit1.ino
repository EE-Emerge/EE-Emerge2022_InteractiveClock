#include "msp430.h"
#include "stdint.h"
#include "ws2812.h"
#include "Wire.h"
#include "PCF8523.h"

/*
    Digit is a number between 0 to 9. 

    Show digit will turn on the specific color for the led row.
    All other digits will be turned off. 
*/

PCF8523 RTC; // Establishes the chipset of the Real Time Clock
RTC_Millis RTCM;

const unsigned long MCLK_HZ = 16000000;          // SMCLK frequency in Hz

void setup()
{

    RTC.adjust(DateTime(F(__DATE__), F(__TIME__)));


   if (CALBC1_16MHZ==0xFF)    // If calibration constant erased
    {
        while(1);              // do not load, trap CPU!!
    }

    // configure clock to 16 MHz
    BCSCTL1 = CALBC1_16MHZ;    // DCO = 16 MHz
    DCOCTL = CALDCO_16MHZ;
    
   
    initStrip();  // ***** HAVE YOU SET YOUR NUM_LEDS DEFINE IN WS2812.H? ******
    
  // Start RTC
  Wire.begin(); // Starts the Wire library allows I2C communication to the Real Time Clock
  RTC.begin(); // Starts communications to the RTC
 
}


void loop()
{
  RTCM.adjust(DateTime(F(__DATE__), F(__TIME__)));
  DateTime now = RTC.now();
  RTCM.adjust(DateTime(now.second()));
  minimalClock(now);
  showStrip();
 clearStrip();
}

void minimalClock(DateTime now)
{
  
    //uint32_t currentDigit = now.second()%10;
    //ShowDigit(currentDigit, COLOR_GREEN);
   // clearStrip();
    setLEDColor(now.second()%10, COLOR_GREEN);
    setLEDColor(19-now.second()%10, COLOR_GREEN);
    setLEDColor(20+now.second()%10, COLOR_GREEN);
    setLEDColor(39-now.second()%10, COLOR_GREEN);
    setLEDColor(40+now.second()%10, COLOR_GREEN);

   
   
}


static void ShowDigit(uint32_t digit, ColorMap color)
{
    

}
