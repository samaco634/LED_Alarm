#include <Wire.h>
#include "RTClib.h" //adafruit RTC lib
#include <TimeLib.h>
#include <TimeAlarms.h>

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN        6 // On Trinket or Gemma, suggest changing this to 1

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 28 // Popular NeoPixel ring size
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

RTC_DS3231 myRTC;
DateTime dt;

uint8_t ledOnTime = 9; // 9:00am every day
uint8_t ledOffTime = 19; // 19:00 -> 7:00pm every day

uint32_t syncProvider()
{
  dt = myRTC.now();
  return dt.unixtime();
}

void setup() {
  // put your setup code here, to run once:
    Serial.begin(57600);
    Wire.begin();
    delay(500);

    DateTime compiled = DateTime(F(__DATE__), F(__TIME__));

      if (! myRTC.begin()) {
        Serial.println("Couldn't find RTC");
        Serial.flush();
        while (1) delay(10);
      }
    DateTime now = myRTC.now();

    if(now < compiled)
    {
    myRTC.adjust(compiled);
    }

    setSyncProvider(syncProvider);

    if (timeStatus() != timeSet) 
       Serial.println("Unable to sync with the RTC");
    else
       Serial.println("RTC has set the system time");     
    
 
    // create the alarms, to trigger functions at specific times
    Alarm.alarmRepeat(ledOnTime,00,0,MorningAlarm);  // 9:00am every day
    Alarm.alarmRepeat(ledOffTime,00,0,EveningAlarm);  // 19:00 -> 7:00pm every day

    pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
    
    if((now.hour()>= ledOnTime) &&(now.hour()<ledOffTime))
    {
   // The first NeoPixel in a strand is #0, second is 1, all the way up
   // to the count of pixels minus one.
      for(int i=0; i<NUMPIXELS; i++) { // For each pixel...
    
        // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
        pixels.setPixelColor(i, pixels.Color(0, 150, 0)); 
        pixels.show();   // Send the updated pixel colors to the hardware.
    
        delay(300); // Pause before next pass through loop
      }     
    }
    else
    {
      pixels.clear(); // Set all pixel colors to 'off'
      pixels.show();   // Send the updated pixel colors to the hardware.
    }
}

void loop() {
  // put your main code here, to run repeatedly:
    Alarm.delay(1000); 
}

// functions to be called when an alarm triggers
void MorningAlarm() {
  // write here the task to perform every morning
  Serial.println("Tturn light off");
  
  for(int i=0; i<NUMPIXELS; i++) { // For each pixel...

    // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
    // Here we're using a moderately bright green color:
    pixels.setPixelColor(i, pixels.Color(0, 150, 0)); 
    pixels.show();   // Send the updated pixel colors to the hardware.

    delay(300); // Pause before next pass through loop
  } 
}
void EveningAlarm() {
  // write here the task to perform every evening
  Serial.println("Turn light off");
  pixels.clear(); // Set all pixel colors to 'off'
  pixels.show();   // Send the updated pixel colors to the hardware.
}
