#include "ws2812.h"
#include "Wire.h"
#include "PCF8523.h"


PCF8523 RTC; // Establishes the chipset of the Real Time Clock
RTC_Millis RTCM;

#define LEDStripPin 15 // Pin used for the data to the LED strip
#define numLEDs 16 // Number of LEDs in strip

// Setting up the LED strip
typedef struct {
    uint8_t green;
    uint8_t red;
    uint8_t blue;
} LED;

static LED leds[numLEDs]; //= {{0,0,0}}; 

int LEDOffset = 30;
int next_flag = 0;

//Setup 60 GRB LED Strip on Launchpad pin 2
//WS2811Driver ledStrip = WS2811Driver(27, 15, NEO_GRB);
//static void ShowDigit(uint32_t digit, ColorMap color);
const unsigned long MCLK_HZ = 16000000;  
void setup()
{
   Serial.begin(4800); // Starts the serial communications
   Serial.print("Program ");

   
   if (!RTC.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while(1);
   }
    RTC.adjust(DateTime(F(__DATE__), F(__TIME__)));

 /*WDTCTL = WDTPW + WDTHOLD;  // Stop WDT
    if (CALBC1_16MHZ==0xFF)    // If calibration constant erased
    {
        while(1);              // do not load, trap CPU!!
    }
    DCOCTL = 0; // Select lowest DCOx and MODx
    
    // configure clock to 16 MHz
    // clk cycle = 62.5ns
    BCSCTL1 = CALBC1_16MHZ;    // DCO = 16 MHz
    DCOCTL = CALDCO_16MHZ;
    /*
        Enable global interrupts
    * /
    __bis_SR_register(GIE);
    /*
        Initialize Neopixel array. 
        All Neopixels will default to off. 
    * /
    initStrip();  // ***** HAVE YOU SET YOUR NUM_LEDS DEFINE IN WS2812.H? ******
*/


   if (CALBC1_16MHZ==0xFF)    // If calibration constant erased
    {
        while(1);              // do not load, trap CPU!!
    }

    // configure clock to 16 MHz
    BCSCTL1 = CALBC1_16MHZ;    // DCO = 16 MHz
    DCOCTL = CALDCO_16MHZ;
    
   // BSP_Init();

   //gradualFill(NUM_LEDS, 0xFF, 0x00, 0x00);  // red

    initStrip();  // ***** HAVE YOU SET YOUR NUM_LEDS DEFINE IN WS2812.H? ******
    

    // set strip color red
    //fillStrip(0xFF, 0x00, 0x00);

    // show the strip
    showStrip(); 

  
  // Set up all pins
  //pinMode(menuPin, INPUT_PULLUP);     // Uses the internal 20k pull up resistor. Pre Arduino_v.1.0.1 need to be "digitalWrite(menuPin,HIGH);pinMode(menuPin,INPUT);"
    
  // Start LEDs
 // LEDS.addLeds<WS2811, LEDStripPin, GRB>(leds, numLEDs); // Structure of the LED data. I have changed to from rgb to grb, as using an alternative LED strip. Test & change these if you're getting different colours. 
  
  // Start RTC
  Wire.begin(); // Starts the Wire library allows I2C communication to the Real Time Clock
  RTC.begin(); // Starts communications to the RTC
 
}


void loop()
{
  RTCM.adjust(DateTime(F(__DATE__), F(__TIME__)));
  DateTime now = RTC.now();
  RTCM.adjust(DateTime(now.year(), now.month(), now.day(), 0, now.minute(), now.second()));
 

  minimalClock(now);
  Serial.print("Hour time is... ");
  Serial.println(now.hour());
  Serial.print("Min time is... ");
  Serial.println(now.minute());
  Serial.print("Sec time is... ");
  Serial.println(now.second());

  Serial.print("Year is... ");
  Serial.println(now.year());
  Serial.print("Month is... ");
  Serial.println(now.month());
  Serial.print("Day is... ");
  Serial.println(now.day());
  

}

void minimalClock(DateTime now)
{

  unsigned char hourPos = (now.hour()%12)*5; 

if (((hourPos+LEDOffset)%60-5 != ((now.minute()+LEDOffset)%60)) && (((hourPos+LEDOffset)%60-5 != (now.second()+LEDOffset)%60))) {
    setLEDColor((hourPos+LEDOffset)%60-5, COLOR_OFF);
    showStrip();
  }
     if ((((now.minute()+LEDOffset)%60-1 != (hourPos+LEDOffset)%60)) && (((now.minute()+LEDOffset)%60-1 != (now.second()+LEDOffset)%60))) {
      //if ((now.minute()+LEDOffset)==26) {
        //if(((hourPos+LEDOffset)%27) != 26) {
          setLEDColor((now.minute()+LEDOffset)%60-1, COLOR_OFF);
          showStrip();
          if (((now.minute()+LEDOffset)%60 == (hourPos+LEDOffset)%60)) {
             setLEDColor((now.minute()+LEDOffset)%60, COLOR_RED);
          }

        //}
      //}

    }

    if ((((now.second()+LEDOffset)%60-1 != (hourPos+LEDOffset)%60)) && ((now.second()+LEDOffset)%60-1 != (now.minute()+LEDOffset)%60)) {
         //if ((now.second()+LEDOffset)==26) {
            //if((((hourPos+LEDOffset)%27) != 26) && (((now.minute()+LEDOffset)%27) != 26))  {
                if (next_flag == 1) {
                  setLEDColor(59, COLOR_OFF);
                  next_flag = 0;
                }
                else {
                  setLEDColor((now.second()+LEDOffset)%60-1, COLOR_OFF);
                  if ((now.second()+LEDOffset)%60 == 0) {
                    next_flag = 1;
                  }
                }
                if (((now.second()+LEDOffset)%60 == (hourPos+LEDOffset)%60)) {
                  setLEDColor((now.second()+LEDOffset)%60, COLOR_RED);
                }
                if (((now.second()+LEDOffset)%60 == (now.minute()+LEDOffset)%60)) {
                  setLEDColor((now.second()+LEDOffset)%60, COLOR_GREEN);
                }

                showStrip();
            //}  
         //}
    }

   
    setLEDColor((now.second()+LEDOffset)%60, COLOR_BLUE);
    showStrip();

    setLEDColor((now.minute()+LEDOffset)%60, COLOR_GREEN);
    showStrip();

    setLEDColor((hourPos+LEDOffset)%60, COLOR_RED);
    showStrip();
    


}



/*
#include "ws2812.h"
#include "Wire.h"
#include "PCF8523.h"


  static int state=0;  // Remember LED state for toggling purposes
//DateTime now ;

PCF8523 RTC; // Establishes the chipset of the Real Time Clock
RTC_Millis RTCM;

#define LEDStripPin 15 // Pin used for the data to the LED strip
#define numLEDs 16 // Number of LEDs in strip

// Setting up the LED strip
typedef struct {
    uint8_t green;
    uint8_t red;
    uint8_t blue;
} LED;

static LED leds[numLEDs]; //= {{0,0,0}}; 

int LEDOffset = 0;
int next_flag = 0;
int led_count = 0;


//Setup 60 GRB LED Strip on Launchpad pin 2
//WS2811Driver ledStrip = WS2811Driver(27, 15, NEO_GRB);
//static void ShowDigit(uint32_t digit, ColorMap color);
const unsigned long MCLK_HZ = 16000000;  


void setup()
{
   /*Serial.begin(4800); // Starts the serial communications
   Serial.print("Program ");

   
   if (!RTC.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while(1);
   }* /
    //RTC.adjust(DateTime(F(__DATE__), F(__TIME__)));
    //DateTime now = RTC.now();
 WDTCTL = WDTPW + WDTHOLD;  // Stop WDT
    if (CALBC1_16MHZ==0xFF)    // If calibration constant erased
    {
        while(1);              // do not load, trap CPU!!
    }

    DCOCTL = 0; // Select lowest DCOx and MODx
    
    // configure clock to 16 MHz
    // clk cycle = 62.5ns
    BCSCTL1 = CALBC1_16MHZ;    // DCO = 16 MHz
    DCOCTL = CALDCO_16MHZ;

    /*
        Enable global interrupts
    * /
    __bis_SR_register(GIE);

    /*
        Initialize Neopixel array. 
        All Neopixels will default to off. 
    * /
    initStrip();  // ***** HAVE YOU SET YOUR NUM_LEDS DEFINE IN WS2812.H? ******



 /*  if (CALBC1_16MHZ==0xFF)    // If calibration constant erased
    {
        while(1);              // do not load, trap CPU!!
    }

    // configure clock to 16 MHz
    BCSCTL1 = CALBC1_16MHZ;    // DCO = 16 MHz
    DCOCTL = CALDCO_16MHZ;
    
   // BSP_Init();

   //gradualFill(NUM_LEDS, 0xFF, 0x00, 0x00);  // red

    initStrip();  // ***** HAVE YOU SET YOUR NUM_LEDS DEFINE IN WS2812.H? ******
    

    // set strip color red
    //fillStrip(0xFF, 0x00, 0x00);

    // show the strip
    //showStrip(); 

  
  // Set up all pins
  //pinMode(menuPin, INPUT_PULLUP);     // Uses the internal 20k pull up resistor. Pre Arduino_v.1.0.1 need to be "digitalWrite(menuPin,HIGH);pinMode(menuPin,INPUT);"
    
  // Start LEDs
 // LEDS.addLeds<WS2811, LEDStripPin, GRB>(leds, numLEDs); // Structure of the LED data. I have changed to from rgb to grb, as using an alternative LED strip. Test & change these if you're getting different colours. 
  
  // Start RTC
  Wire.begin(); // Starts the Wire library allows I2C communication to the Real Time Clock
  RTC.begin(); // Starts communications to the RTC
    TA0CTL=0b0000001011010010; 
  TACCR0=20000; // Set TACCR0 = 2000 to generate a 1ms timebase @ 16MHz with a divisor of 8
  TACCTL0=BIT4; // Enable interrupts when TAR = TACCR0
 * /
}


void loop()
{
  RTCM.adjust(DateTime(F(__DATE__), F(__TIME__)));
    DateTime now = RTC.now();
  RTCM.adjust(DateTime(now.year(), now.month(), now.day(), 0, now.minute(), now.second()));
  minimalClock(now);
  /*if (state == 1) {
    state = 0;
    minimalClock();
  }

  //delay(1000);* /
  /*Serial.print("Hour time is... ");
  Serial.println(now.hour());
  Serial.print("Min time is... ");
  Serial.println(now.minute());
  Serial.print("Sec time is... ");
  Serial.println(now.second());

  Serial.print("Year is... ");
  Serial.println(now.year());
  Serial.print("Month is... ");
  Serial.println(now.month());
  Serial.print("Day is... ");
  Serial.println(now.day());* /
  

}

/*
void loop()
{
  if(state)
  {

        minimalClock(now);
  state=0;
  }
}
// The address function that follows this vector statement is placed in the specified location Interrupt Vector table 
#pragma vector=TIMER0_A0_VECTOR
__interrupt  void timerA0ISR(void)
{
// Timer A0 Interrupt service routine
  static int msCount=0;// Count milliseconds to allow a 1 second pulse
  
  msCount++;
  if (msCount >= 100)
  {
    msCount = 0;
    state=1;             // toggle state
  }
}* /


void minimalClock(DateTime now)
{

/*setLEDColor(led_count, COLOR_BLUE);
led_count++;
if (led_count == 60) {
  led_count = 0;
}
showStrip();* /


  unsigned char hourPos = (now.hour()%12)*5; 

if (((hourPos+LEDOffset)%60-5 != ((now.minute()+LEDOffset)%60)) && (((hourPos+LEDOffset)%60-5 != (now.second()+LEDOffset)%60))) {
    setLEDColor((hourPos+LEDOffset)%60-5, COLOR_OFF);
    showStrip();
  }
     if ((((now.minute()+LEDOffset)%60-1 != (hourPos+LEDOffset)%60)) && (((now.minute()+LEDOffset)%60-1 != (now.second()+LEDOffset)%60))) {
      //if ((now.minute()+LEDOffset)==26) {
        //if(((hourPos+LEDOffset)%27) != 26) {
          setLEDColor((now.minute()+LEDOffset)%60-1, COLOR_OFF);
          showStrip();
          if (((now.minute()+LEDOffset)%60 == (hourPos+LEDOffset)%60)) {
             setLEDColor((now.minute()+LEDOffset)%60, COLOR_RED);
          }

        //}
      //}

    }

    if ((((now.second()+LEDOffset)%60-1 != (hourPos+LEDOffset)%60)) && ((now.second()+LEDOffset)%60-1 != (now.minute()+LEDOffset)%60)) {
         //if ((now.second()+LEDOffset)==26) {
            //if((((hourPos+LEDOffset)%27) != 26) && (((now.minute()+LEDOffset)%27) != 26))  {
                if (next_flag == 1) {
                  setLEDColor(59, COLOR_OFF);
                  next_flag = 0;
                }
                else {
                  setLEDColor((now.second()+LEDOffset)%60-1, COLOR_OFF);
                  if ((now.second()+LEDOffset)%60 == 0) {
                    next_flag = 1;
                  }
                }
                if (((now.second()+LEDOffset)%60 == (hourPos+LEDOffset)%60)) {
                  setLEDColor((now.second()+LEDOffset)%60, COLOR_RED);
                }
                if (((now.second()+LEDOffset)%60 == (now.minute()+LEDOffset)%60)) {
                  setLEDColor((now.second()+LEDOffset)%60, COLOR_GREEN);
                }

                showStrip();
            //}  
         //}
    }

   
    setLEDColor((now.second()+LEDOffset)%60, COLOR_BLUE);
    showStrip();

    setLEDColor((now.minute()+LEDOffset)%60, COLOR_GREEN);
    showStrip();

    setLEDColor((hourPos+LEDOffset)%60, COLOR_RED);
    showStrip();
    


}*/
