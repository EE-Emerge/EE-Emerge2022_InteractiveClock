#include "ws2812.h"
#include "Wire.h" 
#include "PCF8523.h"

#include <ws2811.h>
#include <WS2811Driver.h>
//#include <FastLED.h>
//#include "FastLED.h" // FastSPI Library from http://code.google.com/p/fastspi/
//#include <RTClib.h>           
//#include <EEPROM.h>         
//#include <Bounce2.h>           
//#include <Encoder.h>         
//#include <MD_Parola.h>
//#include <MD_MAX72xx.h>


PCF8523 RTC; // Establishes the chipset of the Real Time Clock
RTC_Millis RTCM;



//#define HARDWARE_TYPE MD_MAX72XX::FC16_HW

//#define MAX_DEVICES 4
//#define CLK_PIN   13
//#define DATA_PIN  11
//#define CS_PIN    10
//MD_Parola P = MD_Parola(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

#define LEDStripPin 15 // Pin used for the data to the LED strip
//#define menuPin 0 // Pin used for the menu button (green stripe)
#define numLEDs 27 // Number of LEDs in strip

// Setting up the LED strip
typedef struct {
    uint8_t green;
    uint8_t red;
    uint8_t blue;
} LED;

static LED leds[numLEDs] = {{0,0,0}};
//Encoder rotary1(3, 4); // Setting up the Rotary Encoder

DateTime old; // Variable to compare new and old time, to see if it has moved on.
int rotary1Pos  = 0;
int subSeconds; // 60th's of a second
int secondBrightness;
int secondBrightness2;
int breathBrightness;
long newSecTime; // Variable to record when a new second starts, allowing to create milli seconds
long oldSecTime;
long flashTime; //
long breathCycleTime;
#define holdTime 1500
int cyclesPerSec;
float cyclesPerSecFloat; // So can be used as a float in calcs
float fracOfSec;
float breathFracOfSec;
boolean demo;
#define demoTime 12 // seconds
long previousDemoTime;
long currentDemoTime;
boolean swingBack = false;

int timeHour;
int timeMin;
int timeSec;
int alarmMin; // The minute of the alarm  
int alarmHour; // The hour of the alarm 0-23
int alarmDay = 0; // The day of the alarm
boolean alarmSet; // Whether the alarm is set or not
int modeAddress = 0; // Address of where mode is stored in the EEPROM
int alarmMinAddress = 1; // Address of where alarm minute is stored in the EEPROM
int alarmHourAddress = 2; // Address of where alarm hour is stored in the EEPROM
int alarmSetAddress = 3; // Address of where alarm state is stored in the EEPROM
int alarmModeAddress = 4; // Address of where the alarm mode is stored in the EEPROM
boolean alarmTrig = false; // Whether the alarm has been triggered or not
long alarmTrigTime; // Milli seconds since the alarm was triggered
boolean countDown = false;
long countDownTime = 0;
long currentCountDown = 0;
long startCountDown;
int countDownMin;
int countDownSec;
int countDownFlash;
int demoIntro = 0;
int j = 0;
long timeInterval = 5;
long currentMillis;
long previousMillis = 0;
float LEDBrightness = 0;
float fadeTime;
float brightFadeRad;

int state = 0; // Variable of the state of the clock, with the following defined states 
#define clockState 0
#define alarmState 1
#define setAlarmHourState 2
#define setAlarmMinState 3
#define setClockHourState 4
#define setClockMinState 5
#define setClockSecState 6
#define countDownState 7
#define demoState 8
int mode; // Variable of the display mode of the clock
int modeMax = 6; // Change this when new modes are added. This is so selecting modes can go back beyond.
int alarmMode; // Variable of the alarm display mode
int alarmModeMax = 3;

//Bounce menuBouncer = Bounce(menuPin,20); // Instantiate a Bounce object with a 50 millisecond debounce time for the menu button
boolean menuButton = false; 
boolean menuPressed = false;
boolean menuReleased = false;
int advanceMove = 0;
boolean countTime = false;
long menuTimePressed;
long lastRotary;
int rotaryTime = 1000;

int LEDPosition;
int reverseLEDPosition;
int pendulumPos;
int fiveMins;
int odd;
int LEDOffset = 15;

int Hr,yr,mon,d;
int Mn, sc;

//Setup 60 GRB LED Strip on Launchpad pin 2
WS2811Driver ledStrip = WS2811Driver(60, 15, NEO_GRB);

void setup()
{
   Serial.begin(57600); // Starts the serial communications
   Serial.print("Program ");


   if (!RTC.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while(1);
   }
    RTC.adjust(DateTime(F(__DATE__), F(__TIME__)));
    
 /* uint16_t i;
  for(i=0; i<ledStrip.numPixels(); i++)
  {
    ledStrip.setPixelColor(i, 0, 0, 0);
  }
  
  ledStrip.setBrightness(255);      //Set LED strip brightness to max
  
  ledStrip.begin();   
  initStrip();*/
  
  // Set up all pins
  //pinMode(menuPin, INPUT_PULLUP);     // Uses the internal 20k pull up resistor. Pre Arduino_v.1.0.1 need to be "digitalWrite(menuPin,HIGH);pinMode(menuPin,INPUT);"
    
  // Start LEDs
 // LEDS.addLeds<WS2811, LEDStripPin, GRB>(leds, numLEDs); // Structure of the LED data. I have changed to from rgb to grb, as using an alternative LED strip. Test & change these if you're getting different colours. 
  
  // Start RTC
  Wire.begin(); // Starts the Wire library allows I2C communication to the Real Time Clock
  RTC.begin(); // Starts communications to the RTC
//  RTCM.begin();
  //P.begin();


  // Uncomment to reset all the EEPROM addresses. You will have to comment again and reload, otherwise it will not save anything each time power is cycled
  // write a 0 to all 512 bytes of the EEPROM
//  for (int i = 0; i < 512; i++)
//  {EEPROM.write(i, 0);}

  // Load any saved setting since power off, such as mode & alarm time  
  /*mode = EEPROM.read(modeAddress); // The mode will be stored in the address "0" of the EEPROM
  alarmMin = EEPROM.read(alarmMinAddress); // The mode will be stored in the address "1" of the EEPROM
  alarmHour = EEPROM.read(alarmHourAddress); // The mode will be stored in the address "2" of the EEPROM
  alarmSet = EEPROM.read(alarmSetAddress); // The mode will be stored in the address "2" of the EEPROM
  alarmMode = EEPROM.read(alarmModeAddress);*/

  // create a loop that calcuated the number of counted milliseconds between each second.
 //DateTime now = RTC.now(); // Fetches the time from RTC
 RTC.adjust(DateTime(F(__DATE__), F(__TIME__)));
  DateTime now = RTC.now();
  RTC.adjust(DateTime(now.year(), now.month(), now.day(), 0, now.minute(), now.second()));
 //DateTime now = RTCM.now();
//int startTime = millis();  
//   while (RTC.old() = RTC.new())

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
  //minimalClock(now);
}


void loop()
{
 /* //DateTime now = RTC.now(); // Fetches the time from RTC
 RTC.adjust(DateTime(F(__DATE__), F(__TIME__)));
  DateTime now = RTC.now();
  RTC.adjust(DateTime(now.year(), now.month(), now.day(), 0, now.minute(), now.second()));
 //DateTime now = RTCM.now();
//int startTime = millis();  
//   while (RTC.old() = RTC.new())

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
  Serial.println(now.day());*/
  
  // Check for any button presses and action accordingley
  //menuButton = menuBouncer.update();  // Update the debouncer for the menu button and saves state to menuButton
  //rotary1Pos = rotary1.read(); // Checks the rotary position
 
  //initStrip();
  // clear LED array
  memset(leds, 0, numLEDs * 3);
  
  // Check alarm and trigger if the time matches
  //timeDisplay(now);

  // Update LEDs
  //LEDS.show();
  //showStrip();
/*
//Max7219LED Clock
   Hr = now.hour();
   Mn = now.minute();
   sc=now.second();
   String s1= " " + String(Hr) + ":" ; 
   String s2= String(Mn);
   String s3= String(sc);
   String s= s1 + s2 ;
   if (strlen(Hr)==1)
   {
    String s1="0"+ s1;
    }
   if( strlen(Mn)== 1 )
   {
    String s2 = "0"+ s2 ;
    }
   P.print(s);
   Serial.print(s);*/
}

/*void buttonCheck(Bounce menuBouncer, DateTime now)
{
  if (menuBouncer.fallingEdge()) // Checks if a button is pressed, if so sets countTime to true
    {
      countTime = true;
      Serial.println("rising edge");
    }
  if (menuBouncer.risingEdge()) // Checks if a button is released,
    {
      countTime = false;
      Serial.println("rising edge");
    } // if so sets countTime to false. Now the ...TimePressed will not be updated when enters the buttonCheck,
  if (countTime) // otherwise will menuBouncer.duration will 
    {
      menuTimePressed = menuBouncer.duration();
      if (menuTimePressed >= (holdTime - 100) && menuTimePressed <= holdTime)
        {
          
          LEDS.show();
          delay(100);
        }
    }
  menuReleased = menuBouncer.risingEdge();
  if (menuPressed == true) {Serial.println("Menu Button Pressed");}
  if (menuReleased == true) {Serial.println("Menu Button Released");}
  Serial.print("Menu Bounce Duration ");
  Serial.println(menuTimePressed);
  if (alarmTrig == true)
    {
      alarmTrig = false;
      alarmDay = now.day(); // When the alarm is cancelled it will not display until next day. As without it, it would start again if within a minute, or completely turn off the alarm.
      delay(300); // I added this 300ms delay, so there is time for the button to be released
      return; // This return exits the buttonCheck function, so no actions are performs
    }  
  switch (state)
    {
      case clockState: // State 0
        if (advanceMove == -1 && mode == 0)
          {
            mode = modeMax;
            advanceMove = 0;
          }
        else if(advanceMove != 0) //if displaying the clock, advance button is pressed & released, then mode will change
          {
            mode = mode + advanceMove;
            //EEPROM.write(modeAddress,mode);
            advanceMove = 0;
          }
        else if(menuReleased == true) 
          {
            if (menuTimePressed <= holdTime) {state = alarmState; newSecTime = millis();}// if displaying the clock, menu button is pressed & released, then Alarm is displayed 
            else {state = setClockHourState;} // if displaying the clock, menu button is held & released, then clock hour can be set
          }
        break;
      case alarmState: // State 1
        if (advanceMove == -1 && alarmMode <= 0)
          {
            alarmMode = alarmModeMax;
            alarmSet = 1;
          }
        else if (advanceMove == 1 && alarmMode >= alarmModeMax)
          {
            alarmMode = 0;
            alarmSet = 0;
          }
        else if (advanceMove != 0)
          {
            alarmMode = alarmMode + advanceMove;
            if (alarmMode == 0) {alarmSet = 0;}
            else {alarmSet = 1;}
          }          
        Serial.print("alarmState is ");
        Serial.println(alarmState);            
        Serial.print("alarmMode is ");
        Serial.println(alarmMode);
        //EEPROM.write(alarmSetAddress,alarmSet);
        //EEPROM.write(alarmModeAddress,alarmMode);
        advanceMove = 0;
        alarmTrig = false;
        if (menuReleased == true) 
          {
            if (menuTimePressed <= holdTime) {state = countDownState; j = 0;}// if displaying the alarm time, menu button is pressed & released, then clock is displayed
            else {state = setAlarmHourState;} // if displaying the alarm time, menu button is held & released, then alarm hour can be set
          }
        break;
      case setAlarmHourState: // State 2
        if (menuReleased == true) {state = setAlarmMinState;}
        else if (advanceMove == 1 && alarmHour >= 23) {alarmHour = 0;}
        else if (advanceMove == -1 && alarmHour <= 0) {alarmHour = 23;}
        else if (advanceMove != 0) {alarmHour = alarmHour + advanceMove;}
        //EEPROM.write(alarmHourAddress,alarmHour);
        advanceMove = 0;
        break;
      case setAlarmMinState: // State 3
        if (menuReleased == true)
          {
            state = alarmState;
            alarmDay = 0;
            newSecTime = millis();
          }
        else if (advanceMove == 1 && alarmMin >= 59) {alarmMin = 0;}
        else if (advanceMove == -1 && alarmMin <= 0) {alarmMin = 59;}
        else if (advanceMove != 0) {alarmMin = alarmMin + advanceMove;}
        //EEPROM.write(alarmMinAddress,alarmMin);
        advanceMove = 0;
        break;
      case setClockHourState: // State 4
        if (menuReleased == true) {state = setClockMinState;}
        else if (advanceMove == 1 && now.hour() == 23)
          {
            RTC.adjust(DateTime(now.year(), now.month(), now.day(), 0, now.minute(), now.second()));
            advanceMove = 0;
          }
        else if (advanceMove == -1 && now.hour() == 0)
          {
            RTC.adjust(DateTime(now.year(), now.month(), now.day(), 23, now.minute(), now.second()));
            advanceMove = 0;
          }
        else if (advanceMove != 0)
          {
            RTC.adjust(DateTime(now.year(), now.month(), now.day(), (now.hour() + advanceMove), now.minute(), now.second()));
            advanceMove = 0;
          }
        break;
      case setClockMinState: // State 5
        if (menuReleased == true) {state = setClockSecState;}
        else if (advanceMove == 1 && now.minute() == 59)
          {
            RTC.adjust(DateTime(now.year(), now.month(), now.day(), now.hour(), 0, now.second()));
            advanceMove = 0;
          }
        else if (advanceMove == -1 && now.minute() == 0)
          {
            RTC.adjust(DateTime(now.year(), now.month(), now.day(), now.hour(), 59, now.second()));
            advanceMove = 0;
          }
        else if (advanceMove != 0)
          {
            RTC.adjust(DateTime(now.year(), now.month(), now.day(), now.hour(), (now.minute() + advanceMove), now.second()));
            advanceMove = 0;
          }
        break;
      case setClockSecState: // State 6
        if (menuReleased == true) {state = clockState;}
        else if (advanceMove == 1 && now.second() == 59)
          {
            RTC.adjust(DateTime(now.year(), now.month(), now.day(), now.hour(), now.minute(), 0));
            advanceMove = 0;
          }
        else if (advanceMove == -1 && now.second() == 0)
          {
            RTC.adjust(DateTime(now.year(), now.month(), now.day(), now.hour(), now.minute(), 59));
            advanceMove = 0;
          }
        else if (advanceMove != 0)
          {
            RTC.adjust(DateTime(now.year(), now.month(), now.day(), now.hour(), now.minute(), (now.second() + advanceMove)));
            advanceMove = 0;
          }
        break;
      case countDownState: // State 7
        if(menuReleased == true) 
          {
            if (menuTimePressed <= holdTime)
              {
                if (countDown == true && countDownTime <= 0) {countDown = false; countDownTime = 0; currentCountDown = 0;}
                else if (countDown == false && countDownTime > 0) {countDown = true; startCountDown = now.unixtime();}
                else {state = demoState; demoIntro = 1; j = 0;}// if displaying the count down, menu button is pressed & released, then demo State is displayed 
              } 
            else {countDown = false; countDownTime = 0; currentCountDown = 0; j = 0;} // if displaying the clock, menu button is held & released, then the count down is reset
          }
        else if (advanceMove == -1 && currentCountDown <= 0)
          {
            countDown = false;
            countDownTime = 0;
            currentCountDown = 0;
            demoIntro = 0;          
          }
        else if (advanceMove == 1 && currentCountDown >= 3600)
          {
            countDown = false;
            countDownTime = 3600;           
          }
        else if (advanceMove != 0) //if displaying the count down, rotary encoder is turned then will change accordingley
          {
            countDown = false;
            countDownTime = currentCountDown - currentCountDown%60 + advanceMove*60; // This rounds the count down minute up to the next minute
          }
        advanceMove = 0;
        break;
      case demoState: // State 8
        //if(menuReleased == true) {state = clockState; mode = EEPROM.read(modeAddress);} // if displaying the demo, menu button pressed then the clock will display and restore to the mode before demo started
        break;
    }
  if (menuReleased || advanceMove !=0) {countTime = false;}
  Serial.print("Mode is ");
  Serial.println(mode);
  Serial.print("State is ");
  Serial.println(state);
}*/

void setAlarmDisplay()
{

  for (int i = 0; i < numLEDs; i++)
    {
      fiveMins = i%5;
      if (fiveMins == 0)
        {
          leds[i].red = 100;
          leds[i].green = 100;
          leds[i].blue = 100;
        }
    }

  if (alarmSet == 0)
    {
      for (int i = 0; i < numLEDs; i++) // Sets background to red, to state that alarm IS NOT set
        {
          fiveMins = i%5;
          if (fiveMins == 0)
            {
              leds[i].red = 20;
              leds[i].green = 0;
              leds[i].blue = 0;
            }  
        }     
    }
  else
    {
      for (int i = 0; i < numLEDs; i++) // Sets background to green, to state that alarm IS set
        {
          fiveMins = i%5;
          if (fiveMins == 0)
            {
              leds[i].red = 0;
              leds[i].green = 20;
              leds[i].blue = 0;
            }  
        }     
    }
  if (alarmHour <= 11)
    {
      leds[(alarmHour*5+LEDOffset)%60].red = 255;
    }
  else
    {
      leds[((alarmHour - 12)*5+LEDOffset+59)%60].red = 25;    
      leds[((alarmHour - 12)*5+LEDOffset)%60].red = 255;
      leds[((alarmHour - 12)*5+LEDOffset+1)%60].red = 25;
    }
  leds[(alarmMin+LEDOffset)%60].green = 100;
  flashTime = millis();
  if (state == setAlarmHourState && flashTime%300 >= 150)
    {
      leds[(((alarmHour%12)*5)+LEDOffset+59)%60].red = 0;   
      leds[(((alarmHour%12)*5)+LEDOffset)%60].red = 0;
      leds[(((alarmHour%12)*5)+LEDOffset+1)%60].red = 0; 
    }
  if (state == setAlarmMinState && flashTime%300 >= 150)
    {
      leds[(alarmMin+LEDOffset)%60].green = 0;
    }
  leds[(alarmMode+LEDOffset)%60].blue = 255;
}

void setClockDisplay(DateTime now)
{
  for (int i = 0; i < numLEDs; i++)
    {
      fiveMins = i%5;
      if (fiveMins == 0)
        {
          leds[i].red = 10;
          leds[i].green = 10;
          leds[i].blue = 10;
        }
    } 
  if (now.hour() <= 11) {leds[(now.hour()*5+LEDOffset)%60].red = 255;}
  else
    {
      leds[((now.hour() - 12)*5+LEDOffset+59)%60].red = 255;
      leds[((now.hour() - 12)*5+LEDOffset)%60].red = 255;   
      leds[((now.hour() - 12)*5+LEDOffset+1)%60].red = 255;
    }
  flashTime = millis();
  if (state == setClockHourState && flashTime%300 >= 150)
    {
      leds[(((now.hour()%12)*5)+LEDOffset+59)%60].red = 0;   
      leds[((now.hour()%12)*5+LEDOffset)%60].red = 0;
      leds[(((now.hour()%12)*5)+LEDOffset+1)%60].red = 0; 
    }
  if (state == setClockMinState && flashTime%300 >= 150) {leds[(now.minute()+LEDOffset)%60].green = 0;}
  else {leds[(now.minute()+LEDOffset)%60].green = 255;}
  if (state == setClockSecState && flashTime%300 >= 150) {leds[(now.second()+LEDOffset)%60].blue = 0;}
  else {leds[(now.second()+LEDOffset)%60].blue = 255;}
}

// Check if alarm is active and if is it time for the alarm to trigger
void alarm(DateTime now)
{
  if ((alarmMin == now.minute()%60) && (alarmHour == now.hour()%24)) //check if the time is the same to trigger alarm
    {
      alarmTrig = true;
      alarmTrigTime = millis();
    }
}

void alarmDisplay() // Displays the alarm
{
  switch (alarmMode)
    {
      case 1:
        // set all LEDs to a dim white
        for (int i = 0; i < numLEDs; i++)
          {
            leds[i].red = 100;
            leds[i].green = 100;
            leds[i].blue = 100;
          }
        break;
      case 2:
        LEDPosition = ((millis() - alarmTrigTime)/300);
        reverseLEDPosition = 60 - LEDPosition;
        if (LEDPosition >= 0 && LEDPosition <= 29)
          {
            for (int i = 0; i < LEDPosition; i++)
              {
                leds[(i+LEDOffset)%60].red = 5;
                leds[(i+LEDOffset)%60].green = 5;
                leds[(i+LEDOffset)%60].blue = 5;
              }
          }
        if (reverseLEDPosition <= 59 && reverseLEDPosition >= 31)
          {
            for (int i = 59; i > reverseLEDPosition; i--)
              {
                leds[(i+LEDOffset)%60].red = 5;
                leds[(i+LEDOffset)%60].green = 5;
                leds[(i+LEDOffset)%60].blue = 5;
              }              
          }
        if (LEDPosition >= 30)
          {
            for (int i = 0; i < numLEDs; i++)
              {
                leds[(i+LEDOffset)%60].red = 5;
                leds[(i+LEDOffset)%60].green = 5;
                leds[(i+LEDOffset)%60].blue = 5;
              }           
          }            
        break;
      case 3:
        fadeTime = 60000;
        brightFadeRad = (millis() - alarmTrigTime)/fadeTime; // Divided by the time period of the fade up.
        if (millis() > alarmTrigTime + fadeTime) LEDBrightness = 255;
        else LEDBrightness = 255.0*(1.0+sin((1.57*brightFadeRad)-1.57));
        Serial.println(brightFadeRad);
        Serial.println(LEDBrightness);
        for (int i = 0; i < numLEDs; i++)
          {
            leds[i].red = LEDBrightness;
            leds[i].green = LEDBrightness;
            leds[i].blue = LEDBrightness;
          }
        break;


    }
}

//  
void countDownDisplay(DateTime now)
{
  flashTime = millis();
  if (countDown == true)
    {
      currentCountDown = countDownTime + startCountDown - now.unixtime();
      if (currentCountDown > 0)
        {
          countDownMin = currentCountDown / 60;
          countDownSec = currentCountDown%60 * 4; // have multiplied by 4 to create brightness
          for (int i = 0; i < countDownMin; i++) {leds[(i+LEDOffset+1)%60].blue = 240;} // Set a blue LED for each complete minute that is remaining 
          leds[(countDownMin+LEDOffset+1)%60].blue = countDownSec; // Display the remaining secconds of the current minute as its brightness      
        }
      else
        {
          countDownFlash = now.unixtime()%2;
          if (countDownFlash == 0)
            {
              for (int i = 0; i < numLEDs; i++) // Set the background as all off
                {
                  leds[i].red = 0;
                  leds[i].green = 0;
                  leds[i].blue = 0;
                }
            }
          else
            {
              for (int i = 0; i < numLEDs; i++) // Set the background as all blue
                {
                  leds[i].red = 0;
                  leds[i].green = 0;
                  leds[i].blue = 255;
                }
            }
        }
    }
  else
    {
      currentCountDown = countDownTime;
      if (countDownTime == 0)
        {
          currentMillis = millis();
          switch (demoIntro)
            {
              case 0:
                for (int i = 0; i < j; i++) {leds[(i+LEDOffset+1)%60].blue = 20;}
                if (currentMillis - previousMillis > timeInterval) {j++; previousMillis = currentMillis;}
                if (j == numLEDs) {demoIntro = 1;}
                break;
              case 1:
                for (int i = 0; i < j; i++) {leds[(i+LEDOffset+1)%60].blue = 20;}
                if (currentMillis - previousMillis > timeInterval) {j--; previousMillis = currentMillis;}
                if (j < 0) {demoIntro = 0;}
                break;
            }
        }
      else if (countDownTime > 0 && flashTime%300 >= 150)
        {
          countDownMin = currentCountDown / 60; //
          for (int i = 0; i < countDownMin; i++) {leds[(i+LEDOffset+1)%60].blue = 255;} // Set a blue LED for each complete minute that is remaining
        }
    }
}

void runDemo(DateTime now)
{
  currentDemoTime = now.unixtime();
  currentMillis = millis();
 
  switch (demoIntro)
    {
      case 0:
        timeDisplay(now);
        if (currentDemoTime - previousDemoTime > demoTime) {previousDemoTime = currentDemoTime; mode++;}
        break;
      case 1:
        for (int i = 0; i < j; i++) {leds[i].red = 255;}
        if (currentMillis - previousMillis > timeInterval) {j++; previousMillis = currentMillis;}
        if (j == numLEDs) {j = 0; demoIntro++;}
        break;
      case 2:
        for (int i = j; i < numLEDs; i++) {leds[i].red = 255;}
        if (currentMillis - previousMillis > timeInterval) {j++; previousMillis = currentMillis;}
        if (j == numLEDs) {j = 0; demoIntro++;}
        break;
      case 3:
        for (int i = 0; i < j; i++) {leds[i].green = 255;}
        if (currentMillis - previousMillis > timeInterval) {j++; previousMillis = currentMillis;}
        if (j == numLEDs) {j = 0; demoIntro++;}
        break;
      case 4:
        for (int i = j; i < numLEDs; i++) {leds[i].green = 255;}
        if (currentMillis - previousMillis > timeInterval) {j++; previousMillis = currentMillis;}
        if (j == numLEDs) {j = 0; demoIntro++;}
        break;
      case 5:
        for (int i = 0; i < j; i++) {leds[i].blue = 255;}
        if (currentMillis - previousMillis > timeInterval) {j++; previousMillis = currentMillis;}
        if (j == numLEDs) {j = 0; demoIntro++;}
        break;
      case 6:
        for (int i = j; i < numLEDs; i++) {leds[i].blue = 255;}
        if (currentMillis - previousMillis > timeInterval) {j++; previousMillis = currentMillis;}
        if (j == numLEDs) {j = 0; demoIntro++;}
        break;
      case 7:
        for (int i = 0; i < j; i++) {leds[i].red = 255; leds[i].green = 255; leds[i].blue = 255;}
        if (currentMillis - previousMillis > timeInterval) {j++; previousMillis = currentMillis;}
        if (j == numLEDs) {j = 0; demoIntro++;}
        break;
      case 8:
        for (int i = j; i < numLEDs; i++) {leds[i].red = 255; leds[i].green = 255; leds[i].blue = 255;}
        if (currentMillis - previousMillis > timeInterval) {j++; previousMillis = currentMillis;}
        if (j == numLEDs)
          {
            demoIntro = 0;
            mode = 0;
            Serial.print("Mode is ");
            Serial.println(mode);
            Serial.print("State is ");
            Serial.println(state);
          }
        break;
    }
}

void timeDisplay(DateTime now)
{ 
  switch (mode)
    {
      case 0:
        minimalClock(now);
        break;
      default: // Keep this here and add more timeDisplay modes as defined cases.
        {
          mode = 0;
        }
    }
}


void minimalClock(DateTime now)
{
  unsigned char hourPos = (now.hour()%12)*5;
  //setLEDColor((hourPos+LEDOffset)%60, 255, 0, 0);
  leds[(hourPos+LEDOffset)%60].red = 255;
  leds[(now.minute()+LEDOffset)%60].green = 255;
  leds[(now.second()+LEDOffset)%60].blue = 255;
}
