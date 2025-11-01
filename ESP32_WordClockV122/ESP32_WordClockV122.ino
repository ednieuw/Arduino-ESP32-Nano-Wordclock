/* 
 Author      : Ed Nieuwenhuys ednieuw.nl
 Changes V001: Derived from ESP32C3S3_WordClockV026.ino
 Changes V003: Clock and SK6812 LEDstrip working
...
 Changes V094: Replaced NIMBLE with ESP32 BLE
 Changes V095: Cleanup code. New function LEDstartup. Webserver to core 1. Loop is again 210,000L/s -> xTaskCreatePinnedToCore(WebServerTask,"WebServerTask", 4096, NULL, 1, NULL, 1 ); 
 Changes V096: Removed ESP32 BLE. Caused restarts in ESP32 3.2.0. NimBLE works fine and probably better.
 Changes V097: Webpage remembers and also shows last small or long menu display
 Changes V098: Cleanup code in ReworkInputString(). Changed if(Mem.WIFIcredentials == SET_AND_OK)  WiFi.reconnect(); in every minute. 
               After power faillure the clock starts faster than a WIFI-router.  .bin upload repaired. Did not upload since V091
 Changes V099: EDSOFT bij 256 LED-klok. Removed: sntp_set_time_sync_notification_cb(NTPnotify); Seems unnecessary. 
 Changes V100: Disconnect for a day and try again to connect WIFI. Added NLM1M2M3M4L161 design.  Filename changed: without Arduino in it
 Changes V101: IS did not recolour per second -> repaired. HTML-Menu appears on time after upload. Added HOURLYHETISWAS with Q7.
 Changes V102: Corrected that every second the time was printed
 Changes V103: M1M2M3M4 clock did not respond to HET IS WAS colouring. Time printing every minute had no linefeed
 Changes V104: If length BLEbroadcastName < 4 fill in the name WordClock. If BLE connection inactive for 15 minutes -> disconnect. Focus back to webpage input field
 Changes V105: Menu optie Q8 is RAINBOW Option Q8 changed to '(' EdSoftLEDSOn and Q9 -> ')' HetIsWasOff in menu as new option
 Changes V106: Fast rainbow added. ~ in menu --> Hourly changing display mode added for demo purposes
 Changes V107: Print DisplayChoice() added.  case '~': Mem.RandomDisplay 1 = per minute or 2= per hour 0 is off 
 Changes V108: Added BLEbroadcastname in webpage title. Bug fix in menu U Demo mode. Runs with Arduino core 2.0.17 en Espressif core 3.3.0
 Changes V109: Option Y Rainbow test operates outside WIFI when entered via webpage
 Changes V110: Use of Mem.WIFIcredentials optimized to restore WIFI connection after power loss 
 Changes V111: Made CheckRestoreWIFIconnectivity(). Removed RAINBOWFAST
 Changes V112: Tested in several clocks
 Changes V113: In Ulrich design Klok No47 --> Shirley Janssen  NLM1M2M3M4L161
 Changes V114: is V113
 Changes V115: Time functions updated. You can compile now By Arduino pin (default)
 Changes V116: Removed redundant code
 Changes V117: Added remote control. 
 Changes V118: OTA more stable. void SetStatusLED() changed to turn of LED completely also in ESP32 core 3
 Changes V119: IR-Remote works. Default Time-zone is entered when ResetCredentials(). IR-remote Power ON/OFF turns ON/OFF clock display 
 Changes V120: Mem.IRremote -> Mem.UseRotary H03. H001 and H002 -> H01 and H02. in wtekstAppend()  if(Mem.MCUrestarted == 0) Tekstprint(sptext); 
 Changes V121: Optimized IR-remote. Use Power button to use IR-remote input for 2 minutes
 Changes V122: Optimized SetDS3231Time()  if(DS3231Installed)   SetDS3231Time(); Removed stray input from IRremote

 If remote is off and turned on --> restart ESP32
 Power ON/OFF start entering time. Switch to DS3231
 OK leaves learning mode. No action anymore from remote entries
 Learning mode only from BLE or serial


*********************
How to compile: 
Install ESP32 boards
Board: ESP32 core version >3.2.0 
Partition Scheme: With FAT
Pin Numbering: By Arduino pin (default)   
               By GPIO number (legacy).     -- > When using NEOpixel change 
USB mode: Normal (Tiny USB)
**********************

Select below, with only one #define selected, the clock type

*/
// =============================================================================================================================
// ------------------>   Define only one clock type
//#define FOURLANGUAGECLOCK       // Four-language clock with 625 LEDs 
#define NL144CLOCK              // Dutch display for 12 x 12 Front
//#define NL92CLOCK               // Dutch display for one LED behind every character
//#define NLM1M2M3M4L161          // NL EdSoft Qclock design
//#define DE144CLOCK              // German display for 12 x 12 Front
//#define NLM1M2M3M4L94           // NL clock with four extra LEDs for the minutes to light up 
//#define NLM1M2M3M4L114          // NL clock with four extra LEDs for the minutes to light up
//#define NLM1M2M3M4L144          // NL clock with four extra LEDs for the minutes to light up 
//#define NLM1M2M3M4L256          // NL clock with four extra LEDs for the minutes to light up

//------------------------------------------------------------------------------              //
// ESP32 Definition of installed modules
// Define the modules installed in the clock by removing the // before the #define
//--------------------------------------------
//#define ONEWIREKEYPAD3x1       // Use a 3x1 keypad with one wire
//#define ONEWIREKEYPAD3x4       // Use a 3x4 keypad with one wire

//------------------------------------------------------------------------------              //
// ESP32 Includes defines and initialisations
//-------------------------------------------
                      #if ESP_ARDUINO_VERSION >= ESP_ARDUINO_VERSION_VAL (3, 2, 0)            // Use EdSoftLED with ESP32 compiler.
#define USEEDSOFTLED
                      #endif
                      #ifdef USEEDSOFTLED
#include <EdSoftLED.h>         // https://github.com/ednieuw/EdSoftLED for LED strip WS2812 or SK6812 
                      #else
#include <Adafruit_NeoPixel.h> // https://github.com/adafruit/Adafruit_NeoPixel  *** USE GPIO NUMBERING and compile By GPIO number (legacy) 
                      #endif
#include <Preferences.h>
#include <NimBLEDevice.h>      // For BLE communication. !!!Use NimBLE version 2.x.x  https://github.com/h2zero/NimBLE-Arduino
#include <WiFi.h>              // Used for web page 
#include <WebServer.h>         // Used for web page 
#include <esp_sntp.h>          // for NTP
#include <esp_wps.h>           // For WPS
#include <Update.h>            // For Over-the-Air (OTA)
#include <ESPmDNS.h>           // To show BLEname in router
#include <DNSServer.h>         // For the web page to enter SSID and password of the WIFI router 
#include <Wire.h>              // Ter zijner tijd Wire functies gaan gebruiken. Staan al klaar in de code 
#include <RTClib.h>            // Used for connected DS3231 RTC // Reference https://adafruit.github.io/RTClib/html/class_r_t_c___d_s3231.html
#include <Encoder.h>           // For rotary encoder
#include <Keypad.h>            // For 3x1 membrane keypad instead of rotary encoder by Mark Stanley & Alexander Brevig 
#include <IRremote.hpp>        // IR remote control

//------------------------------------------------------------------------------              //
// ARDUINO Definition of installed language word clock
//------------------------------------------------------------------------------
                     #ifdef FOURLANGUAGECLOCK
#define NL
#define UK
#define DE
#define FR    
                     #endif //FOURLANGUAGECLOCK
                     #if defined NLM1M2M3M4L94 ||defined NLM1M2M3M4L114 || (defined NLM1M2M3M4L256) || (defined NLM1M2M3M4L144) || (defined NLM1M2M3M4L161)
#define NLM1M2M3M4
                     #endif

                     #ifdef NL144CLOCK
const uint32_t NUM_LEDS  =  144+4;                                    // How many leds in  strip? + 4 for the minutes
const byte MATRIX_WIDTH  =  12;                                       // Grid size For digital display mode.  
const byte MATRIX_HEIGHT =  12;                                       // Grid size For digital display mode.
// const uint32_t NUM_LEDS  =  256;                                   // Temporary for testinf with 16x16LED WS2812. To clear all LEDs. During uploading LEDs get randomly On and Off
// const byte MATRIX_WIDTH  =  16;                                    // Grid size For digital display mode.
// const byte MATRIX_HEIGHT =  16;                                    // Grid size For digital display mode.

#define HET     ColorLeds("Het",     0,   2, MINColor);   
#define IS      ColorLeds("is",      4,   5, SECColor);  ColorLeds("", 8,10, 0); Is = true;   // Turn off the WAS LEDs
#define WAS     ColorLeds("was",     8,  10, SECColor);  ColorLeds("", 4, 5, 0); Is = false;  // Turn off the IS LEDs
#define PRECIES ColorLeds("precies", 17, 23, LetterColor);
#define MTIEN   ColorLeds("tien",    12, 15, LetterColor); 
#define MVIJF   ColorLeds("vijf",    25, 28, LetterColor); 
#define KWART   ColorLeds("kwart",   30, 34, LetterColor);
#define VOOR    ColorLeds("voor",    44, 47, LetterColor);
#define OVER    ColorLeds("over",    38, 41, LetterColor);
#define HALF    ColorLeds("half",    48, 51, LetterColor);
#define MIDDER  ColorLeds("midder",  53, 58, LetterColor);
#define VIJF    ColorLeds("vijf",    66, 69, LetterColor);
#define TWEE    ColorLeds("twee",    60, 63, LetterColor);
#define EEN     ColorLeds("een",     72, 74, LetterColor);
#define VIER    ColorLeds("vier",    77, 80, LetterColor);
#define TIEN    ColorLeds("tien",    91, 94, LetterColor);
#define TWAALF  ColorLeds("twaalf",  84, 89, LetterColor);
#define DRIE    ColorLeds("drie",    96, 99, LetterColor);
#define NEGEN   ColorLeds("negen",  102,106, LetterColor);
#define ACHT    ColorLeds("acht",   114,117, LetterColor);
#define NACHT   ColorLeds("nacht",  114,118, LetterColor);
#define ZES     ColorLeds("zes",    110,112, LetterColor);
#define ZEVEN   ColorLeds("zeven",  121,125, LetterColor);
#define ELF     ColorLeds("elf",    128,130, LetterColor);
#define NOEN    ColorLeds("noen",   139,142, LetterColor);
#define UUR     ColorLeds("uur",    134,136, LetterColor);
#define EDSOFT  ColorLeds("EdSoft", 148,148, LetterColor);
#define MIN1    ColorLeds("M1",     144,144, LetterColor); 
#define MIN2    ColorLeds("M2",     145,145, LetterColor);                                    // MINColor); 
#define MIN3    ColorLeds("M3",     146,146, LetterColor);
#define MIN4    ColorLeds("M4",     147,147, LetterColor);  
#define X_OFF   ColorLeds("",         0,  2, 0);
#define X_ON    ColorLeds("",         0,  2, LetterColor);
                     #endif //NL144CLOCK
                     #ifdef NL92CLOCK
const uint32_t NUM_LEDS  =  92;                                                               // How many leds in  strip?
const byte MATRIX_WIDTH  =  10;                                                               // Grid size For digital display mode.  
const byte MATRIX_HEIGHT =  10;                                                               // Grid size For digital display mode.

#define HET     ColorLeds("Het",     0,   2, MINColor); 
#define IS      ColorLeds("is",      3,   4, SECColor);  ColorLeds("", 5, 7, 0); Is = true;   // Turn off the WAS LEDs
#define WAS     ColorLeds("was",     5,   7, SECColor);  ColorLeds("", 3, 4, 0); Is = false;  // Turn off the IS LEDs
#define MVIJF   ColorLeds("vijf",    12, 15, LetterColor); 
#define MTIEN   ColorLeds("tien",    8,  11, LetterColor);
#define KWART   ColorLeds("kwart",   16, 20, LetterColor);
#define VOOR    ColorLeds("voor",    21, 24, LetterColor);
#define OVER    ColorLeds("over",    32, 35, LetterColor);
#define PRECIES ColorLeds("precies", 25, 31, LetterColor);
#define HALF    ColorLeds("half",    36, 39, LetterColor);
#define ELF     ColorLeds("elf",     40, 42, LetterColor);
#define VIJF    ColorLeds("vijf",    47, 50, LetterColor);
#define TWEE    ColorLeds("twee",    43, 46, LetterColor);
#define EEN     ColorLeds("een",     51, 53, LetterColor);
#define VIER    ColorLeds("vier",    54, 57, LetterColor);
#define TIEN    ColorLeds("tien",    64, 67, LetterColor);
#define TWAALF  ColorLeds("twaalf",  58, 63, LetterColor);
#define DRIE    ColorLeds("drie",    68, 71, LetterColor);
#define NEGEN   ColorLeds("negen",   72, 76, LetterColor);
#define ACHT    ColorLeds("acht",    80, 83, LetterColor);
#define ZES     ColorLeds("zes",     77, 79, LetterColor);
#define ZEVEN   ColorLeds("zeven",   84, 88, LetterColor);
#define UUR     ColorLeds("uur",     89,91, LetterColor);
#define EDSOFT  ColorLeds("EdSoft",  92,92, LetterColor);
#define X_OFF   ColorLeds("",         0,  2, 0);
#define X_ON    ColorLeds("",         0,  2, LetterColor);
#define MIN1    ColorLeds("M1",     144,144, LetterColor); 
#define MIN2    ColorLeds("M2",     145,145, LetterColor);                                    // MINColor); 
#define MIN3    ColorLeds("M3",     146,146, LetterColor);
#define MIN4    ColorLeds("M4",     147,147, LetterColor);  
#define X_OFF   ColorLeds("",         0,  2, 0);
#define X_ON    ColorLeds("",         0,  2, LetterColor);
                     #endif //NL92CLOCK

                     #ifdef DE144CLOCK
const uint32_t NUM_LEDS  =  144;                                                              // How many leds in  strip? 
const byte MATRIX_WIDTH  =  12;                                                               // Grid size For digital display mode.  
const byte MATRIX_HEIGHT =  12;                                                               // Grid size For digital display mode.
#define ES      ColorLeds("Es",       1,   2, LetterColor);   
#define IST     ColorLeds("ist",      4,   6, LetterColor);  ColorLeds("", 8, 10, 0); Is = true;
#define WAR     ColorLeds("war",      8,  10, LetterColor);  ColorLeds("", 4,  6, 0); Is = false;
#define GENAU   ColorLeds("genau",   18,  22, LetterColor);
#define MZEHN   ColorLeds("zehn",    13,  16, LetterColor);
#define MFUNF   ColorLeds("funf",   383, 386, LetterColor);
#define VIERTEL ColorLeds("viertel",375, 381, LetterColor);
#define ZWANZIG ColorLeds("zwanzig",413, 419, LetterColor);
#define KURZ    ColorLeds("kurz",   421, 424, LetterColor);
#define VOR     ColorLeds("vor",    433, 435, LetterColor);
#define NACH    ColorLeds("nach",   427, 430, LetterColor);
#define HALB    ColorLeds("halb",   465, 468, LetterColor);
#define FUNF    ColorLeds("funf",   471, 474, LetterColor);
#define EINS    ColorLeds("eins",   483, 486, LetterColor);
#define VIERDE  ColorLeds("vier",   478, 481, LetterColor);
#define ZEHN    ColorLeds("zehn",   514, 517, LetterColor);
#define ZWOLF   ColorLeds("zwolf",  520, 524, LetterColor);
#define DREI    ColorLeds("drei",   532, 535, LetterColor);
#define NEUN    ColorLeds("neun",   526, 529, LetterColor);
#define ACHTDE  ColorLeds("acht",   565, 568, LetterColor);
#define SECHS   ColorLeds("sechs",  570, 574, LetterColor);
#define SIEBEN  ColorLeds("sieben", 580, 585, LetterColor);
#define ZWEI    ColorLeds("zwei",   575, 578, LetterColor);
#define ELFDE   ColorLeds("elf",    614, 616, LetterColor);
#define UHR     ColorLeds("uhr",    621, 623, LetterColor);
                     #endif //DE144CLOCK
                     #ifdef NLM1M2M3M4L94
const uint32_t NUM_LEDS  =  94;                                    // How many leds in  strip? + 4 for the minutes
const byte MATRIX_WIDTH  =  10;                                    // NA Not used in this clock. 10 is fine 
const byte MATRIX_HEIGHT =  10;                                    // NA Not used in this clock. 10 is fine 
                   
#define HET     ColorLeds("Het",      0,   2, MINColor);           // Special adapted clock face versie Kris. IS and WAS are equal. There is no WAS
#define IS      ColorLeds("is",       3,   4, SECColor);     Is = true;    //ColorLeds("", 3, 4, 0); 
#define WAS     ColorLeds("was",      3,   4, SECColor);     Is = false;  // ColorLeds("", 3, 4, 0); 
#define MVIJF   ColorLeds("vijf",     5,   8, LetterColor); 
#define MVOOR   ColorLeds("voor",     9,  12, LetterColor);
#define MTIEN   ColorLeds("tien",    13,  16, LetterColor); 
#define OVER    ColorLeds("over",    17,  20, LetterColor);
#define KWART   ColorLeds("kwart",   21,  25, LetterColor);
#define OVER2   ColorLeds("over",    26,  29, LetterColor);
#define HALF    ColorLeds("half",    30,  33, LetterColor);
#define VOOR    ColorLeds("voor",    34,  37, LetterColor);
#define EEN     ColorLeds("een",     38,  40, LetterColor);
#define DRIE    ColorLeds("drie",    41,  44, LetterColor);
#define TWEE    ColorLeds("twee",    45,  48, LetterColor);
#define VIER    ColorLeds("vier",    49,  52, LetterColor);
#define VIJF    ColorLeds("vijf",    53,  56, LetterColor);
#define ZES     ColorLeds("zes",     57,  59, LetterColor);
#define NEGEN   ColorLeds("negen",   60,  64, LetterColor);
#define ZEVEN   ColorLeds("zeven",   65,  69, LetterColor);
#define ACHT    ColorLeds("acht",    70,  73, LetterColor);
#define TIEN    ColorLeds("tien",    74,  77, LetterColor);
#define ELF     ColorLeds("elf",     78,  80, LetterColor);
#define TWAALF  ColorLeds("twaalf",  84,  89, LetterColor);
#define UUR     ColorLeds("uur",     81,  83, LetterColor);
#define MIN1    ColorLeds("M1",      90,  90, LetterColor); 
#define MIN2    ColorLeds("M2",      91,  91, LetterColor);       //MINColor); 
#define MIN3    ColorLeds("M3",      92,  92, LetterColor);
#define MIN4    ColorLeds("M4",      93,  93, LetterColor);  
#define EDSOFT  ColorLeds("EdSoft",  94,  94, LetterColor);  
                     #endif //NLM1M2M3M4L94
                     #ifdef NLM1M2M3M4L114 
const uint32_t NUM_LEDS  =  114;                                                              // How many leds in  strip? + 4 for the minutes
const byte MATRIX_WIDTH  =  11;                                                               // NA Not used in this clock. 10 is fine 
const byte MATRIX_HEIGHT =  10;                                                               // NA Not used in this clock. 10 is fine 
                   
#define HET     ColorLeds("Het",      0,   2, MINColor);                                      // Special adapted clock face
#define IS      ColorLeds("is",       4,   5, SECColor);     Is = true;    //ColorLeds("", 4, 5, 0); IS and WAS are equal. There is no WAS
#define WAS     ColorLeds("was",      4,   5, SECColor);     Is = false;  // ColorLeds("", 4, 5, 0); 
#define MVIJF   ColorLeds("vijf",     7,  10, LetterColor); 
#define MVOOR   ColorLeds("voor",    11,  14, LetterColor);
#define MTIEN   ColorLeds("tien",    18,  21, LetterColor); 
#define OVER    ColorLeds("over",    22,  25, LetterColor);
#define KWART   ColorLeds("kwart",   28,  32, LetterColor);
#define OVER2   ColorLeds("over",    33,  36, LetterColor);
#define HALF    ColorLeds("half",    40,  43, LetterColor);
#define VOOR    ColorLeds("voor",    44,  47, LetterColor);
#define EEN     ColorLeds("een",     51,  53, LetterColor);
#define DRIE    ColorLeds("drie",    55,  58, LetterColor);
#define TWEE    ColorLeds("twee",    62,  65, LetterColor);
#define VIER    ColorLeds("vier",    66,  69, LetterColor);
#define VIJF    ColorLeds("vijf",    70,  73, LetterColor);
#define ZES     ColorLeds("zes",     74,  76, LetterColor);
#define NEGEN   ColorLeds("negen",   77,  81, LetterColor);
#define ZEVEN   ColorLeds("zeven",   83,  87, LetterColor);
#define ACHT    ColorLeds("acht",    88,  91, LetterColor);
#define TIEN    ColorLeds("tien",    92,  95, LetterColor);
#define ELF     ColorLeds("elf",     96,  98, LetterColor);
#define TWAALF  ColorLeds("twaalf", 104, 109, LetterColor);
#define UUR     ColorLeds("uur",     99, 101, LetterColor);
#define MIN1    ColorLeds("M1",     110, 110, LetterColor); 
#define MIN2    ColorLeds("M2",     111, 111, LetterColor);                                   // MINColor); 
#define MIN3    ColorLeds("M3",     112, 112, LetterColor);
#define MIN4    ColorLeds("M4",     113, 113, LetterColor);    
#define EDSOFT  ColorLeds("EdSoft", 114, 124, LetterColor);
                     #endif //NLM1M2M3M4L114

                     #ifdef NLM1M2M3M4L144
const uint32_t NUM_LEDS  =  144;                                                              // How many leds in  strip? + 4 for the minutes
const byte MATRIX_WIDTH  =  12;                                                               // NA Not used in this clock. 10 is fine 
const byte MATRIX_HEIGHT =  12;                                                               // NA Not used in this clock. 10 is fine 
                   
#define HET     ColorLeds("Het",      0,   4, MINColor);           // Special adapted clock face
#define IS      ColorLeds("is",       5,   7, SECColor);     Is = true;    //ColorLeds("", 4, 5, 0); IS and WAS are equal. There is no WAS
#define WAS     ColorLeds("was",      5,   7, SECColor);     Is = false;  // ColorLeds("", 4, 5, 0); 
#define MVIJF   ColorLeds("vijf",     8,  13, LetterColor); 
#define MVOOR   ColorLeds("voor",    14,  19, LetterColor);
#define MTIEN   ColorLeds("tien",    20,  25, LetterColor); 
#define OVER    ColorLeds("over",    26,  31, LetterColor);
#define KWART   ColorLeds("kwart",   32,  39, LetterColor);
#define OVER2   ColorLeds("over",    39,  45, LetterColor);
#define HALF    ColorLeds("half",    46,  51, LetterColor);
#define VOOR    ColorLeds("voor",    52,  57, LetterColor);
#define EEN     ColorLeds("een",     58,  62, LetterColor);
#define DRIE    ColorLeds("drie",    63,  68, LetterColor);
#define TWEE    ColorLeds("twee",    69,  74, LetterColor);
#define VIER    ColorLeds("vier",    75,  80, LetterColor);
#define VIJF    ColorLeds("vijf",    81,  86, LetterColor);
#define ZES     ColorLeds("zes",     87,  91, LetterColor);
#define NEGEN   ColorLeds("negen",   92,  99, LetterColor);
#define ZEVEN   ColorLeds("zeven",  100, 107, LetterColor);
#define ACHT    ColorLeds("acht",   108, 113, LetterColor);
#define TIEN    ColorLeds("tien",   114, 119, LetterColor);
#define ELF     ColorLeds("elf",    120, 124, LetterColor);
#define TWAALF  ColorLeds("twaalf", 130, 139, LetterColor);
#define UUR     ColorLeds("uur",    125, 129, LetterColor);
#define MIN1    ColorLeds("M1",     140, 140, LetterColor); 
#define MIN2    ColorLeds("M2",     141, 141, LetterColor);       //MINColor); 
#define MIN3    ColorLeds("M3",     142, 142, LetterColor);
#define MIN4    ColorLeds("M4",     143, 143, LetterColor); 
#define EDSOFT  ColorLeds("EdSoft", 144, 144, LetterColor);   
                     #endif //NLM1M2M3M4L144

                     #ifdef NLM1M2M3M4L161
const uint32_t NUM_LEDS  =  161;                                                              // How many leds in  strip? + 4 for the minutes
const byte MATRIX_WIDTH  =  13;                                                               // NA Not used in this clock. 10 is fine 
const byte MATRIX_HEIGHT =  13;                                                               // NA Not used in this clock. 10 is fine 
                   
#define HET     ColorLeds("Het",      0,   4, MINColor);                                      // Special adapted clock face. IS and WAS are equal. There is no WAS
#define IS      ColorLeds("is",       5,   7, SECColor);     Is = true;                       //ColorLeds("", 4, 5, 0); 
#define WAS     ColorLeds("was",      5,   7, SECColor);     Is = false;                      // ColorLeds("", 4, 5, 0); 
#define MVIJF   ColorLeds("vijf",     8,  14, LetterColor); 
#define MVOOR   ColorLeds("voor",    15,  21, LetterColor);
#define MTIEN   ColorLeds("tien",    22,  28, LetterColor); 
#define OVER    ColorLeds("over",    29,  35, LetterColor);
#define KWART   ColorLeds("kwart",   36,  44, LetterColor);
#define OVER2   ColorLeds("over",    45,  51, LetterColor);
#define HALF    ColorLeds("half",    52,  58, LetterColor);
#define VOOR    ColorLeds("voor",    59,  65, LetterColor);
#define EEN     ColorLeds("een",     66,  70, LetterColor);
#define DRIE    ColorLeds("drie",    71,  77, LetterColor);
#define TWEE    ColorLeds("twee",    78,  84, LetterColor);
#define VIER    ColorLeds("vier",    85,  91, LetterColor);
#define VIJF    ColorLeds("vijf",    92,  98, LetterColor);
#define ZES     ColorLeds("zes",     99, 103, LetterColor);
#define NEGEN   ColorLeds("negen",  104, 112, LetterColor);
#define ZEVEN   ColorLeds("zeven",  113, 121, LetterColor);
#define ACHT    ColorLeds("acht",   122, 128, LetterColor);
#define TIEN    ColorLeds("tien",   129, 135, LetterColor);
#define ELF     ColorLeds("elf",    136, 140, LetterColor);
#define UUR     ColorLeds("uur",    141, 145, LetterColor);
#define TWAALF  ColorLeds("twaalf", 146, 156, LetterColor);
#define MIN1    ColorLeds("M1",     160, 160, LetterColor); 
#define MIN2    ColorLeds("M2",     159, 159, LetterColor);       //MINColor); 
#define MIN3    ColorLeds("M3",     158, 158, LetterColor);
#define MIN4    ColorLeds("M4",     157, 157, LetterColor); 
#define EDSOFT  ColorLeds("EdSoft", 161, 161, LetterColor);   
                     #endif //NLM1M2M3M4L161

                     #ifdef NLM1M2M3M4L256
const uint32_t NUM_LEDS  =  256;                                                              // How many leds in  strip? + 4 for the minutes
const byte MATRIX_WIDTH  =  16;                                                               // NA Not used in this clock. 16 is fine 
const byte MATRIX_HEIGHT =  16;                                                               // NA Not used in this clock. 16 is fine 
                 
#define HET     ColorLeds("Het",       0,   7, MINColor);                                     // Special adapted clock face. IS and WAS are equal. There is no WAS
#define IS      ColorLeds("is",        8,  12, SECColor);     Is = true;    //ColorLeds("", 8,10, 0); 
#define WAS     ColorLeds("was",       8,  12, SECColor);     Is = false;  // ColorLeds("", 4, 5, 0); 
#define MVIJF   ColorLeds("vijf",     13,  23, LetterColor); 
#define MVOOR   ColorLeds("voor",     24,  35, LetterColor);
#define MTIEN   ColorLeds("tien",     36,  46, LetterColor); 
#define OVER    ColorLeds("over",     47,  57, LetterColor);
#define KWART   ColorLeds("kwart",    58,  72, LetterColor);
#define OVER2   ColorLeds("over",     73,  83, LetterColor);
#define HALF    ColorLeds("half",     84,  94, LetterColor);
#define VOOR    ColorLeds("voor",     95, 105, LetterColor);
#define EEN     ColorLeds("een",     106, 113, LetterColor);
#define DRIE    ColorLeds("drie",    114, 124, LetterColor);
#define TWEE    ColorLeds("twee",    125, 135, LetterColor);
#define VIER    ColorLeds("vier",    136, 146, LetterColor);
#define VIJF    ColorLeds("vijf",    147, 157, LetterColor);
#define ZES     ColorLeds("zes",     158, 165, LetterColor);
#define NEGEN   ColorLeds("negen",   166, 180, LetterColor);
#define ZEVEN   ColorLeds("zeven",   181, 195, LetterColor);
#define ACHT    ColorLeds("acht",    196, 206, LetterColor);
#define TIEN    ColorLeds("tien",    207, 217, LetterColor);
#define ELF     ColorLeds("elf",     218, 225, LetterColor);
#define TWAALF  ColorLeds("twaalf",  234, 251, LetterColor);
#define UUR     ColorLeds("uur",     226, 233, LetterColor);
#define MIN1    ColorLeds("M1",      252, 252, LetterColor); 
#define MIN2    ColorLeds("M2",      253, 253, LetterColor);       //MINColor); 
#define MIN3    ColorLeds("M3",      254, 254, LetterColor);
#define MIN4    ColorLeds("M4",      255, 255, LetterColor); 
#define EDSOFT  ColorLeds("EdSoft",  256, 256, LetterColor);
                     #endif //NLM1M2M3M4L256
                                             #ifdef FOURLANGUAGECLOCK                                             
const uint32_t  NUM_LEDS = 625;                                                               // How many leds in  strip?
const byte MATRIX_WIDTH  =  25;                                                               // Grid size For digital display mode.
const byte MATRIX_HEIGHT =  25;                                                               // Grid size For digital display mode.
                                             #endif //FOURLANGUAGECLOCK                    
                     #ifdef NL
#define HET     ColorLeds("Het",      0,   2, LetterColor);   
#define IS      ColorLeds("is",       4,   5, LetterColor); 
#define WAS     ColorLeds("was",      8,  10, LetterColor); 
#define PRECIES ColorLeds("precies", 43,  49, LetterColor);
#define MTIEN   ColorLeds("tien",    38,  41, LetterColor); 
#define MVIJF   ColorLeds("vijf",    51,  54, LetterColor); 
#define KWART   ColorLeds("kwart",   56,  60, LetterColor);
#define VOOR    ColorLeds("voor",    96,  99, LetterColor);
#define OVER    ColorLeds("over",    90,  93, LetterColor);
#define HALF    ColorLeds("half",   100, 103, LetterColor);
#define MIDDER  ColorLeds("midder", 105, 110, LetterColor);
#define VIJF    ColorLeds("vijf",   144, 147, LetterColor);
#define TWEE    ColorLeds("twee",   138, 141, LetterColor);
#define EEN     ColorLeds("een",    150, 152, LetterColor);
#define VIER    ColorLeds("vier",   155, 158, LetterColor);
#define TIEN    ColorLeds("tien",   195, 198, LetterColor);
#define TWAALF  ColorLeds("twaalf", 188, 193, LetterColor);
#define DRIE    ColorLeds("drie",   200, 203, LetterColor);
#define NEGEN   ColorLeds("negen",  206, 210, LetterColor);
#define ACHT    ColorLeds("acht",   244, 247, LetterColor);
#define NACHT   ColorLeds("nacht",  244, 248, LetterColor);
#define ZES     ColorLeds("zes",    240, 242, LetterColor);
#define ZEVEN   ColorLeds("zeven",  251, 255, LetterColor);
#define ELF     ColorLeds("elf",    258, 260, LetterColor);
#define NOEN    ColorLeds("noen",   295, 298, LetterColor);
#define UUR     ColorLeds("uur",    290, 292, LetterColor);
#define EDSOFT  ColorLeds("EdSoft", 300, 311, LetterColor);
#define X_OFF   ColorLeds("",         0,   2, 0);
#define X_ON    ColorLeds("",         0,   2, LetterColor);
                     #endif //NL
                     #ifdef UK
#define IT      ColorLeds("| It",   347, 348, UKLetterColor);   
#define ISUK    ColorLeds("is",     344, 345, UKLetterColor); 
#define WASUK   ColorLeds("was",    340, 342, UKLetterColor);
#define EXACTUK ColorLeds("exact",  351, 355, UKLetterColor);
#define HALFUK  ColorLeds("half",   357, 360, UKLetterColor); 
#define TWENTY  ColorLeds("twenty", 394, 399, UKLetterColor); 
#define MFIVE   ColorLeds("five",   388, 391, UKLetterColor);
#define QUARTER ColorLeds("quarter",400, 406, UKLetterColor);
#define MTEN    ColorLeds("ten",    409, 411, UKLetterColor);
#define PAST    ColorLeds("past",   446, 449, UKLetterColor);
#define TO      ColorLeds("to",     443, 444, UKLetterColor);
#define SIXUK   ColorLeds("six",    439, 441, UKLetterColor);
#define TWO     ColorLeds("two",    451, 453, UKLetterColor);
#define FIVE    ColorLeds("five",   457, 460, UKLetterColor);
#define TWELVE  ColorLeds("twelve", 494, 499, UKLetterColor);
#define TEN     ColorLeds("ten",    488, 490, UKLetterColor);
#define ELEVEN  ColorLeds("eleven", 500, 505, UKLetterColor);
#define FOUR    ColorLeds("four",   507, 510, UKLetterColor);
#define NINE    ColorLeds("nine",   545, 548, UKLetterColor);
#define THREE   ColorLeds("three",  538, 542, UKLetterColor);
#define EIGHT   ColorLeds("eight",  553, 557, UKLetterColor);
#define ONE     ColorLeds("one",    597, 599, UKLetterColor);
#define SEVEN   ColorLeds("seven",  589, 593, UKLetterColor);
#define OCLOCK  ColorLeds("O'clock",605, 610, UKLetterColor);
                      #endif //UK
                      #ifdef DE
#define ES      ColorLeds("\nEs",   334, 335, DELetterColor);   
#define IST     ColorLeds("ist",    330, 332, DELetterColor); 
#define WAR     ColorLeds("war",    326, 328, DELetterColor); 
#define GENAU   ColorLeds("genau",  364, 368, DELetterColor);
#define MZEHN   ColorLeds("zehn",   370, 373, DELetterColor);
#define MFUNF   ColorLeds("funf",   383, 386, DELetterColor);
#define VIERTEL ColorLeds("viertel",375, 381, DELetterColor);
#define ZWANZIG ColorLeds("zwanzig",413, 419, DELetterColor);
#define KURZ    ColorLeds("kurz",   421, 424, DELetterColor);
#define VOR     ColorLeds("vor",    433, 435, DELetterColor);
#define NACH    ColorLeds("nach",   427, 430, DELetterColor);
#define HALB    ColorLeds("halb",   465, 468, DELetterColor);
#define FUNF    ColorLeds("funf",   471, 474, DELetterColor);
#define EINS    ColorLeds("eins",   483, 486, DELetterColor);
#define VIERDE  ColorLeds("vier",   478, 481, DELetterColor);
#define ZEHN    ColorLeds("zehn",   514, 517, DELetterColor);
#define ZWOLF   ColorLeds("zwolf",  520, 524, DELetterColor);
#define DREI    ColorLeds("drei",   532, 535, DELetterColor);
#define NEUN    ColorLeds("neun",   526, 529, DELetterColor);
#define ACHTDE  ColorLeds("acht",   565, 568, DELetterColor);
#define SECHS   ColorLeds("sechs",  570, 574, DELetterColor);
#define SIEBEN  ColorLeds("sieben", 580, 585, DELetterColor);
#define ZWEI    ColorLeds("zwei",   575, 578, DELetterColor);
#define ELFDE   ColorLeds("elf",    614, 616, DELetterColor);
#define UHR     ColorLeds("uhr",    621, 623, DELetterColor);
                      #endif //DE
                      #ifdef FR
#define IL      ColorLeds("| Il",    13,  14, FRLetterColor);   
#define EST     ColorLeds("est",     16,  18, FRLetterColor); 
#define ETAIT   ColorLeds("etait",   20,  24, FRLetterColor);
#define EXACT   ColorLeds("exact",   31,  35, FRLetterColor);
#define SIX     ColorLeds("six",     27,  29, FRLetterColor); 
#define DEUX    ColorLeds("deux",    64,  67, FRLetterColor); 
#define TROIS   ColorLeds("trois",   69,  73, FRLetterColor);
#define ONZE    ColorLeds("onze",    83,  86, FRLetterColor);
#define QUATRE  ColorLeds("quatre",  75,  80, FRLetterColor);
#define MINUIT  ColorLeds("minuit", 113, 118, FRLetterColor);
#define DIX     ColorLeds("dix",    120, 122, FRLetterColor);
#define CINQ    ColorLeds("cinq",   133, 136, FRLetterColor);
#define NEUF    ColorLeds("neuf",   129, 132, FRLetterColor);
#define MIDI    ColorLeds("midi",   125, 128, FRLetterColor);
#define HUIT    ColorLeds("huit",   164, 167, FRLetterColor);
#define SEPT    ColorLeds("sept",   171, 174, FRLetterColor);
#define UNE     ColorLeds("une",    184, 186, FRLetterColor);
#define HEURE   ColorLeds("heure",  178, 182, FRLetterColor);
#define HEURES  ColorLeds("heures", 177, 182, FRLetterColor);
#define ET      ColorLeds("et",     213, 214, FRLetterColor);
#define MOINS   ColorLeds("moins",  216, 220, FRLetterColor);
#define LE      ColorLeds("le",     222, 223, FRLetterColor);
#define DEMI    ColorLeds("demie",  229, 233, FRLetterColor);
#define QUART   ColorLeds("quart",  263, 267, FRLetterColor);
#define MDIX    ColorLeds("dix",    270, 272, FRLetterColor);
#define VINGT   ColorLeds("vingt",  282, 286, FRLetterColor);
#define MCINQ   ColorLeds("cinq",   277 ,280, FRLetterColor);
#define DITLEHEURE DitLeHeure();
                      #endif //FR
//------------------------------------------------------------------------------              //
// SPIFFS storage
//--------------------------------------------
Preferences FLASHSTOR;

//------------------------------------------------------------------------------              //
// COLOURS stored in Mem.DisplayChoice
//--------------------------------------------   

const byte DEFAULTCOLOUR = 0;
const byte HOURLYCOLOUR  = 1;          
const byte WHITECOLOR    = 2;
const byte OWNCOLOUR     = 3;
const byte OWNHETISWAS   = 4;
const byte WHEELCOLOR    = 5;
const byte DIGITAL       = 6;
const byte HOURHETISWAS  = 7;
const byte RAINBOW       = 8;
const byte LASTITEM      = 8;                                                                 // Last display choice item number

//------------------------------------------------------------------------------              //
// PIN Assigments for Arduino Nano ESP32
// Use GPIO numbering when compiling with:  By GPIO number (legacy).
//------------------------------------------------------------------------------ 
 
enum DigitalPinAssignments {      // Digital hardware constants ATMEGA 328 ----
 SERRX        = D0,               // Connects to Bluetooth TX
 SERTX        = D1,               // Connects to Bluetooth RX
 encoderPinB  = D2, //5,          // D2 left (labeled CLK on decoder) no interrupt pin (Use GPIO pin numbering for rotary encoder lib)  
 encoderPinA  = D3, //6,          // D3 right (labeled DT on decoder) on interrupt pin
 clearButton  = D4, //7,          // D4 switch (labeled SW on decoder)
 IRReceiverPin= D4,               // D4 Infrared receiver pin instead of rotary encoder
 LED_PIN      = D5, //8,          // D5 / GPIO 8 Pin to control colour SK6812/WS2812 LEDs (replace D5 with 8 for NeoPixel lib)
 EmptyD6      = D6,               // D6 Empty
 EmptyD7      = D7,               // D7 Empty
 EmptyD8      = D8,               // D8Empty 
 PCB_LED_D09  = D9,               // D9
 PCB_LED_D10  = D10,              // D10
 secondsPin   = D13 //48,         // D13  GPIO48 (#ifdef LED_BUILTIN  #undef LED_BUILTIN #define LED_BUILTIN 48 #endif)
 };
 
enum AnaloguePinAssignments {     // Analogue hardware constants ----
 EmptyA0      = A0,               // Empty
 EmptyA1      = A1,               // Empty
 PhotoCellPin = A2,               // LDR pin
 OneWirePin   = A3,               // OneWirePin
 SDA_pin      = A4,               // SDA pin
 SCL_pin      = A5,               // SCL pin
 EmptyA6      = A6,               // Empty
 EmptyA7      = A7};              // Empty

//------------------------------------------------------------------------------
const uint32_t black    = 0x000000, orangered     = 0xFF4000, red        = 0xFF0000, chartreuse   = 0x7FFF00;
const uint32_t brown    = 0x503000, cyberyellow   = 0xFFD300, orange     = 0xFF8000; 
const uint32_t yellow   = 0xFFFF00, cadmiumyellow = 0xFFF600, dyellow    = 0xFFAA00, chromeyellow = 0xFFA700;
const uint32_t green    = 0x00FF00, brightgreen   = 0x66FF00, apple      = 0x80FF00, grass        = 0x00FF80;  
const uint32_t amaranth = 0xE52B50, edamaranth    = 0xFF0050, amber      = 0xFF7E00;
const uint32_t marine   = 0x0080FF, darkviolet    = 0x800080, pink       = 0xFF0080, purple       = 0xFF00FF; 
const uint32_t blue     = 0x0000FF, cerulean      = 0x007BA7, sky        = 0x00FFFF, capri        = 0x00BFFF;
const uint32_t edviolet = 0X7500BC, frenchviolet  = 0X8806CE, coquelicot = 0xFF3800;
const uint32_t greenblue= 0x00F2A0, hotmagenta    = 0xFF00BF, dodgerblue = 0x0073FF, screamingreen= 0x70FF70;
      uint32_t white    = 0xFFFFFF, lgray         = 0x666666, wgray      = 0xAAAAAA;
      uint32_t gray     = 0x333333, dgray         = 0x222222;  
//------------------------------------------------------------------------------              //
// LED
//------------------------------------------------------------------------------
// 
// Create a mutex (semaphore) to protect LED updates
//SemaphoreHandle_t ledMutex;
/*
Adafruit_NeoPixel LEDstrip;
Adafruit_NeoPixel LED6812strip = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRBW + NEO_KHZ800); // NEO_RGBW
Adafruit_NeoPixel LED2812strip = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRB  + NEO_KHZ800); // NEO_RGB NEO_GRB
*/
                     #ifdef USEEDSOFTLED
EdSoftLED LEDstrip ;//    = EdSoftLED();                                                         // Use EdSoftLED with ESP32 compiler V3.x.x. Neopixel crashes
EdSoftLED LED6812strip = EdSoftLED(NUM_LEDS, LED_PIN, SK6812WRGB);
EdSoftLED LED2812strip = EdSoftLED(NUM_LEDS, LED_PIN, WS2812RGB);
bool UsedEDSOFTLED = true;
                      #else
// LED_PIN = 8;  // bug in Neopixel library. Does not translate D5 to GPIO 8
Adafruit_NeoPixel LEDstrip;
Adafruit_NeoPixel LED6812strip = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRBW + NEO_KHZ800); // NEO_RGBW
Adafruit_NeoPixel LED2812strip = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRB  + NEO_KHZ800); // NEO_RGB NEO_GRB
bool UsedEDSOFTLED = false;
                      #endif
                      
uint32_t MINColor      = chromeyellow;
uint32_t SECColor      = chromeyellow;  
                                             #ifdef FOURLANGUAGECLOCK
uint32_t DefaultColor  = white;   
uint32_t LetterColor   = white; 
uint32_t UKLetterColor = green;   
uint32_t DELetterColor = red;
uint32_t FRLetterColor = yellow;
uint32_t UKDefaultColor= UKLetterColor;
uint32_t DEDefaultColor= DELetterColor;
uint32_t FRDefaultColor= FRLetterColor;
                                             #else
uint32_t DefaultColor  = chromeyellow;   
uint32_t LetterColor   = chromeyellow;   
                                             #endif //FOURLANGUAGECLOCK

uint32_t OwnColour     = 0X002345DD;        // Blueish
uint32_t WheelColor    = blue;
uint32_t HourColor[] ={  white,     darkviolet, cyberyellow, capri,         orange,        apple,
                         greenblue, cerulean,   edviolet,    cadmiumyellow, green,         edamaranth,
                         red,       yellow,     coquelicot,  pink,          apple,         hotmagenta,
                         green,     orangered,  brightgreen, dodgerblue,    screamingreen, blue,
                         white,     darkviolet, chromeyellow };     

// Definition of the digits 0 - 9, 3 wide, 5 high. 
// 10111
// 10101
// 11101 turn your head 90 degrees left and you see a TWO 
const byte PROGMEM Getal[10][3][5]  = { 
                     { {1, 1, 1, 1, 1}, {1, 0, 0, 0, 1}, {1, 1, 1, 1, 1} },  //0
                     { {1, 0, 0, 0, 1}, {1, 1, 1, 1, 1}, {0, 0, 0, 0, 1} },  //1
                     { {1, 0, 1, 1, 1}, {1, 0, 1, 0, 1}, {1, 1, 1, 0, 1} },  //2
                     { {1, 0, 1, 0, 1}, {1, 0, 1, 0, 1}, {1, 1, 1, 1, 1} },  //3
                     { {1, 1, 1, 0, 0}, {0, 0, 1, 0, 0}, {1, 1, 1, 1, 1} },  //4
                     { {1, 1, 1, 0, 1}, {1, 0, 1, 0, 1}, {1, 0, 1, 1, 1} },  //5
//                   { {1, 1, 1, 0, 1}, {1, 0, 1, 0, 1}, {1, 0, 0, 1, 1} },  //5 An other 5
                     { {1, 1, 1, 1, 1}, {0, 0, 1, 0, 1}, {0, 0, 1, 1, 1} },  //6
                     { {1, 1, 0, 0, 0}, {1, 0, 0, 0, 0}, {1, 1, 1, 1, 1} },  //7
                     { {1, 1, 1, 1, 1}, {1, 0, 1, 0, 1}, {1, 1, 1, 1, 1} },  //8
                     { {1, 1, 1, 0, 1}, {1, 0, 1, 0, 1}, {1, 1, 1, 1, 1} }   //9
                     }; 
//--------------------------------------------
// DS3231 CLOCK MODULE
//--------------------------------------------
#define DS3231_I2C_ADDRESS          0x68
#define DS3231_TEMPERATURE_MSB      0x11
#define DS3231_TEMPERATURE_LSB      0x12

RTC_DS3231 RTCklok; 
bool DS3231Installed = false;                                                                 // True if the DS3231 is detected

//------------------------------------------------------------------------------              //
// KY-040 ROTARY
//------------------------------------------------------------------------------                       
Encoder myEnc(encoderPinA, encoderPinB);                                                      // Use digital pin  for encoder    
long     Looptime             = 0;
byte     RotaryPress          = 0;                                                            // Keeps track display choice and how often the rotary is pressed.
bool     ChangeLightIntensity = false;                                                        // Increase or decrease slope light intensity        
uint32_t RotaryPressTimer     = 0;
byte     NoofRotaryPressed    = 0;

//--------------------------------------------                                                //
// One-wire keypad
//--------------------------------------------
bool     ChangeTime           = false;                                                        // Flag to change time within 60 seconds       
uint64_t KeyLooptime          = 0;
String   KeypadString         ="";


//------------------------------------------------------------------------------              //
// KEYPAD 3x1
//          -------- GND
//  R Y G   -------- Pin D8
//          -------- Pin D3
//          -------- Pin D4
// COLPIN is used as dummy pin that must  be LOW when there is input from keypad 
//--------------------------------------------
const byte ROWS   = 3; 
const byte COLS   = 1; 
const byte COLPIN = D12;                                                                      // Column that is always LOW. Mimic with a not used pin
char keys[ROWS][COLS] = {{'R'}, {'Y'}, {'G'}};
byte rowPins[ROWS] = { D8, D3, D4};                                                           // Connect to the row pinouts of the keypad
byte colPins[COLS] = {COLPIN};                                                                // Connect to the column pinouts of the keypad
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS );

//------------------------------------------------------------------------------              //
// LDR PHOTOCELL
//------------------------------------------------------------------------------
//                                                                                            //
const byte SLOPEBRIGHTNESS    = 50;                                                           // Steepness of with luminosity of the LED increases
const int  MAXBRIGHTNESS      = 255;                                                          // Maximum value in bits  for luminosity of the LEDs (1 - 255)
const byte LOWBRIGHTNESS      = 5;                                                            // Lower limit in bits of Brightness ( 0 - 255)   
byte       TestLDR            = 0;                                                            // If true LDR info is printed every second in serial monitor
int        OutPhotocell;                                                                      // stores reading of photocell;
int        MinPhotocell       = 999;                                                          // stores minimum reading of photocell;
int        MaxPhotocell       = 1;                                                            // stores maximum reading of photocell;
uint32_t   SumLDRreadshour    = 0;
uint32_t   NoofLDRreadshour   = 0;

//-------------------------------------------- 
// IR-RECEIVER
//-------------------------------------------- 
#define NO_LED_SEND_FEEDBACK_CODE                                                             // do not flash the BUILTIN LED on Arduino
#define MAX_BUTTONS 16                                                                        // 0-9 (10 buttons) + up, down, left, right, power, OK (6 buttons)


String EnteredDigits = "";                                                                    // Store typed digits InputString
String ButtonNames[] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", 
                             "UP", "DOWN", "LEFT", "RIGHT", "POWER", "OK"};                   // Runtime button info (not saved, just for display)                                    
int currentLearningIndex = 0;
uint32_t IR_StartTime    = 0;                                                                 // Time IR remote Power is on turn. Off after 1 minute
bool IR_PowerOnstate     = false;                                                             // Is the power On or Off
bool learningMode        = false;
struct StoredButtonMapping                                                                    // Button mapping structure for storage
{
  uint8_t  protocol;                                                                          // decode_type_t as byte
  uint16_t command;
  uint16_t address;
  bool     learned;
};

//------------------------------------------------------------------------------              //
// CLOCK initialysations
//--------------------------------------------                                 
static uint32_t msTick;                                                                       // Number of millisecond ticks since we last incremented the second counter
byte      lastminute = 0, lasthour = 0, lastday = 0, sayhour = 0;
bool      Demo                 = false;
bool      FireDisplay          = false;
bool      Zelftest             = false;
bool      Is                   = true;                                                        // Toggle of displaying Is or Was
struct    tm timeinfo;                                                                        // storage of time 

//--------------------------------------------                                                //
// BLE   //#include <NimBLEDevice.h>
//--------------------------------------------
BLEServer *pServer      = NULL;
BLECharacteristic * pTxCharacteristic;
bool deviceConnected    = false;
bool oldDeviceConnected = false;
std::string ReceivedMessageBLE = "";
unsigned long BLEConnectedSince = 0;

#define SERVICE_UUID           "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"                         // UART service UUID
#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"

//------------------------------------------------------------------------------              //
// WIFI and webserver
//----------------------------------------
#define NOT_SET       0
#define SET           1
#define SET_AND_OK    2
#define IN_AP_NOT_SET 3
#include "Webpage.h"                                                                          // The Clock web page
#include "SoftAP.h"                                                                           // The web page to enter SSID and password of the WIFI router 
#include "OTAhtml.h"                                                                          // OTA update page
WiFiEventId_t wifiEventHandler;                                                               // To stop the interrupts or callbacks triggered by WiFi.onEvent(WiFiEvent);, you need to deregister the event handler.
bool WIFIwasConnected      = false;                                                           // Is WIFI connected?
bool apMode                = false;
bool apModeTrigger         = false;
const char* AP_SSID        = "StartWordclock";
const char* AP_PASSWORD    = "wordclock";
//String DelayedRework ="";
WebServer server(80);                                                                         // For OTA Over the air uploading
DNSServer dnsServer;
bool shouldReboot          = false;
bool OptionYRainbow        = false;

//----------------------------------------                                                    //
// WPS
//----------------------------------------
#define ESP_WPS_MODE      WPS_TYPE_PBC
#define ESP_MANUFACTURER  "ESPRESSIF"
#define ESP_MODEL_NUMBER  "ESP32"
#define ESP_MODEL_NAME    "ESPRESSIF IOT"
#define ESP_DEVICE_NAME   "ESP STATION"
static esp_wps_config_t config;
//------------------------------------------------------------------------------              //
// Common
//----------------------------------------
#define   MAXTEXT 255
char      sptext[MAXTEXT];                                                                    // For common print use 
bool      LEDsAreOff        = false;                                                          // If true LEDs are off except time display
bool      NoTextInLeds      = false;                                                          // Flag to control printing of the text in function ColorLeds()
int       Previous_LDR_read = 512;                                                            // The actual reading from the LDR + 4x this value /5
uint16_t  MilliSecondValue  = 10;                                                             // The duration of a second  minus 1 ms. Used in Demo mode
uint32_t  Loopcounter       = 0;
struct    EEPROMstorage {                                                                     // Data storage in EEPROM to maintain them after power loss
  byte DisplayChoice    = 0;
  byte TurnOffLEDsAtHH  = 0;
  byte TurnOnLEDsAtHH   = 0;
  byte LanguageChoice   = 0;
  byte LightReducer     = 0;
  int  LowerBrightness  = 0;
  int  UpperBrightness  = 0;
  int  NVRAMmem[24];                                                                          // LDR or other readings
  byte BLEOn            = 1;
  byte NTPOn            = 1;
  byte WIFIOn           = 1;  
  byte StatusLEDOn      = 1;
  int  MCUrestarted     = 0;                                                                  // No of times WIFI reconnected 
  byte DCF77On          = 0;
  byte UseRotary        = 0;                                                                  // Use coding for Rotary encoder ==1 or 3x1 membrane keypad ==2
  byte UseDS3231        = 0;                                                                  // Use the DS3231 time module 
  byte LEDstrip         = 0;                                                                  // 0 = SK6812 LED strip. 1 = WS2812 LED strip
  byte FiboChrono       = 0;                                                                  // true = Fibonacci, false = chrono clock display
  byte NoExUl           = 0;                                                                  // 0 = Normal, 1 = Extreme, 2 = Ultimate display of colours
  int  WIFIcredentials  = 0;                                                                  // Status of the WIFI connection. SSID&PWD set or in AP mode
  int  IntFuture2       = 0;                                                                  // For future use
  int  IntFuture3       = 0;                                                                  // For future use
  byte HetIsWasOff      = 0;                                                                  // Turn On or Off HET IS WAS   
  byte EdSoftLEDSOn     = 0;                                                                  // EdSoft text on/off   
  byte RandomDisplay    = 0;                                                                  // For future use 
  byte ByteFuture3      = 0;                                                                  // For future use   
  byte UseBLELongString = 0;                                                                  // Send strings longer than 20 bytes per message. Possible in IOS app BLEserial Pro 
  uint32_t OwnColour    = 0;                                                                  // Self defined colour for clock display
  uint32_t DimmedLetter = 0;
  uint32_t BackGround   = 0;
  char SSID[30];                                                                              // 
  char Password[40];                                                                          // 
  char BLEbroadcastName[30];                                                                  // Name of the BLE beacon
  char Timezone[50];
  int  Checksum        = 0;
}  Mem; 

struct IRRemoteStorage
{
  uint16_t learnedRemoteAddress  = 0;
  byte     learnedRemoteProtocol = 0;
  bool     remoteIdentified      = false;
  StoredButtonMapping buttons[30];                                                            // MAX_BUTTONS = 15 but space for 15 more
  int      Checksum              = 0;
} IRMem;
//--------------------------------------------                                                //
// Menu
//0        1         2         3         4
//1234567890123456789012345678901234567890----  
bool LastMenuformat = true;                                                                   // Small=true of full=false menu
char menu[][40] = {
 "A SSID B Password C BLE beacon name",
 "D Date (D15012021) T Time (T132145)",
 "E Timezone  (E<-02>2 or E<+01>-1)",
 "F Own colour (Hex FWWRRGGBB)",
 "G Scan WIFI networks",
 "H H01 rotary H02 membrane H03 remote",
 "} Learn IR remote",  
 "I Info menu, II long menu ",
 "J DS3231 RTC module On/Off",
                        #ifdef DCF77MOD
 "JJ DCF77 receiver On/Off",
                       #endif //DCF77MOD
 "K LDR reads/sec On/Off", 
 "N Display off between Nhhhh (N2208)",
 "O Display On/Off",
 "P Status LED On/Off", 
 "Q Display choice (Q0) ~ Changing",
 "R Reset settings, @ Restart",
 "U Demo (msec) (U200) Y LED test",
// "--Light intensity settings (1-250)--",
 "S Slope, L Min, M Max  (S50 L5 M200)",
 "W WIFI X NTP& Z WPS CCC BLE + Fast BLE",
 "#nnn Selftest, RTC: ! See, & Update",
 ") HETISWAS  On/Off, ( EdSoft On/Off",
 "Ed Nieuwenhuys Oct 2025" };
 
 char menusmall[][40] = {
 "F Own colour (Hex FWWRRGGBB)",
 "I Menu, II long menu",
 "N Display off between Nhhhh (N2208)",
 "O Display On/Off",
 "Q Display colour choice (Q0)",
 "R Reset settings, @ Restart" };
//  -------------------------------------   End Definitions  ---------------------------------------

//--------------------------------------------                                                //
// ARDUINO Setup
//--------------------------------------------
void setup() 
{
 Serial.begin(115200);                                                                        // Setup the serial port to 115200 baud
 Wire.begin();
 int32_t Tick = millis(); 
 SetStatusLED(10,0,0);                                                                        // Set the status LED to red
 InitStorage();                                                                               // Load settings from storage and check validity  
 StartLeds();                                                                                 // LED RainbowCycle  
 while (!Serial && ( (millis()-Tick) < 3000) ) { LEDstartup(orangered); delay(512); }         // Wait max 3 sec to establish serial connection
 LEDstartup(capri); Tekstprintln("Serial started\nStored settings loaded\nLED strip started");// InitStorage and StartLEDs must be called first
 Mem.MCUrestarted++;                                                                          // MCU Restart counter     
 if(Mem.MCUrestarted>5) { Reset();  ResetCredentials(); }                                     // If the MCU restarts during Setup() so often Reset all 
 StoreStructInFlashMemory();                                                                  // 
 sprintf(sptext, "Using %s LED library", UsedEDSOFTLED?"EDSOFTLED":"NEOPIXEL"); Tekstprintln(sptext);                                                             // 
 if(Mem.UseRotary==1) {LEDstartup(pink);  InitRotaryMod(); Tekstprintln("Rotary available"); }// Start the Rotary encoder
 if(Mem.UseRotary==2) {LEDstartup(grass); InitKeypad3x1(); Tekstprintln("Keypad available"); }// Start the Keypad 3x1 
 if(Mem.BLEOn)        {LEDstartup(blue);StartBLEService(); Tekstprintln("BLE started"); }     // Start BLE service 
 if(Mem.WIFIOn)       {LEDstartup(purple); ConnectWIFI();  Tekstprintln("WIFI started");}     // Start WIFI and optional NTP if Mem.WIFIOn = 1 
 if(Mem.UseRotary==3) {LEDstartup(white);Init_IRreceiver();Tekstprintln("IRremote started");} // Start IR remote
 InitTimeSystem();    {LEDstartup(yellow);          Tekstprintln("\nTime system started");}   // Initialize NTP + RTC + DS3231 sync
 Previous_LDR_read = ReadLDR();                                                               // Set the initial LDR reading 
 GetTijd(true); // Tekstprintln("");                                                          // Get the time and print it
 LEDstartup(green);                                                                           // Set the status LED to green
 Displaytime(); Tekstprintln("");                                                             // Print the tekst time in the display 
 SWversion();                                                                                 // Print the menu + version 
 Mem.MCUrestarted = 0;                                                                        // Startup went well; Set MCUrestart counter to 0    
 StoreStructInFlashMemory();                                                                  // 
 msTick = millis();                                                                           // start the seconds loop counter
}

//--------------------------------------------                                                //
// ARDUINO Loop
//--------------------------------------------
void loop() 
{
 Loopcounter++;
 if (Demo)            Demomode();                                                             // 
 else if(FireDisplay) FireMode();                                                             //
 else                 EverySecondCheck();                                                     // Let the second led tick and run the clock program
 CheckDevices();
}

//--------------------------------------------                                                //
// COMMON Check connected input devices
//--------------------------------------------
void CheckDevices(void)
{
 CheckBLE();                                                                                  // Something with BLE to do?
 SerialCheck();                                                                               // Check serial port every second 
 if (OptionYRainbow ) { ReworkInputString("Y"); OptionYRainbow=false;}                        // The LED library’s timing can get interrupted by background Wi-Fi interrupts, causing flicker in the strip.
 if (Mem.UseRotary==1) RotaryEncoderCheck();
 if (Mem.UseRotary==2) Keypad3x1Check();
//  if (Mem.UseRotary==3) IrReceiverDecode();                                                 // --> Every100mSecCheck
                                  #ifdef ONEWIREKEYPAD3x4   
 OnewireKeypad3x4Check(); 
                                  #endif  //ONEWIREKEYPAD3x4
                                  #ifdef ONEWIREKEYPAD3x1   
 OnewireKeypad3x1Check(); 
                                  #endif  //ONEWIREKEYPAD3x1
}
//--------------------------------------------                                                //
// COMMON Update routine 
// Performs tasks every second
//--------------------------------------------
void EverySecondCheck(void)
{
 static int Toggle = 0;
// uint32_t msLeap = millis() - msTick;                                                         // 
 if((millis() - msTick) % 100) Every100mSecCheck();
// msLeap = millis() - msTick;
 if ((millis() - msTick) >999)                                                                // Every second enter the loop
 {
  msTick = millis();
  GetTijd(false);                                                                             // Get the time for the seconds 
  Toggle = 1-Toggle;                                                                          // Used to turn On or Off Leds
  UpdateStatusLEDs(Toggle);
  SetSecondColour();                                                                          // Set the colour per second of 'IS' and 'WAS' 
  DimLeds(TestLDR);                                                                           // Every second an intensity check and update from LDR reading 
  if (shouldReboot) { delay(3000);   ESP.restart(); }                                         // After finish OTA update restarts after 3 seconds. 1 second is too fast
  if (timeinfo.tm_min != lastminute) EveryMinuteUpdate();                                     // Enter the every minute routine after one minute; 
  Loopcounter=0;
 }  
}
//--------------------------------------------                                                //
// COMMON Update routine 
// Performs tasks every second
//--------------------------------------------
void Every100mSecCheck(void)
{
 if (Mem.UseRotary==3) IrReceiverDecode();
}
//--------------------------------------------                                                //
// COMMON Update routine done every minute
//-------------------------------------------- 
void EveryMinuteUpdate(void)
{   
 lastminute = timeinfo.tm_min;  
 CheckRestoreWIFIconnectivity();                                                              // Check if WIFI is sill connected and if not restore it
 if (Mem.RandomDisplay == 1) { ChangeRandomDisplay(); SetSecondColour();} 
 if (IR_PowerOnstate && ((millis() - IR_StartTime) > 120000) ) ToggleIRpower();                // Turn off Power af 120 seconds 
 GetTijd(false);
 Displaytime();  
 DimLeds(true);   
 if(timeinfo.tm_hour != lasthour) EveryHourUpdate(); 
}
//--------------------------------------------                                                //
// COMMON Update routine done every hour
//--------------------------------------------
void EveryHourUpdate(void)
{
 lasthour = timeinfo.tm_hour;
 if (!Mem.StatusLEDOn) SetStatusLED(0,0,0);                                                   // If for some reason the LEDs are ON and after a MCU restart turn them off.  
 if( (timeinfo.tm_hour == Mem.TurnOffLEDsAtHH) && (Mem.TurnOffLEDsAtHH != Mem.TurnOnLEDsAtHH))
       { LEDsAreOff = true;  ClearScreen(); }                                                 // Is it time to turn off the LEDs?
 if(timeinfo.tm_hour == Mem.TurnOnLEDsAtHH)
   { LEDsAreOff = false;   lastminute = 99;     Displaytime(); }                              // Force a minute update
 Mem.NVRAMmem[lasthour] =(byte)((SumLDRreadshour / NoofLDRreadshour?NoofLDRreadshour:1));     // Update the average LDR readings per hour
 SumLDRreadshour  = 0;
 NoofLDRreadshour = 0;
 if (Mem.RandomDisplay == 2) { ChangeRandomDisplay(); SetSecondColour();}                     // If RandomDisplay is ON change the display chouce
 if (timeinfo.tm_mday != lastday) EveryDayUpdate();  
}
//--------------------------------------------                                                //
// COMMON Update routine done every day
//--------------------------------------------
void EveryDayUpdate(void)
{
 lastday           = timeinfo.tm_mday; 
 Previous_LDR_read = ReadLDR();                                                               // to have a start value and reset the Min Max measurements 
 MinPhotocell      = Previous_LDR_read;                                                       // Stores minimum reading of photocell;
 MaxPhotocell      = Previous_LDR_read;                                                       // Stores maximum reading of photocell;
 if(DS3231Installed && !Mem.UseDS3231) SetDS3231Time();                                       // If on NTP time and DS3231 installed set the correct the time in the DS3231
 StoreStructInFlashMemory();                                                                  // Update Mem struct once a day to store Mem.NVRAM measurements
}

//--------------------------------------------                                                //
// COMMON Update routine for the status LEDs
//-------------------------------------------- 
void UpdateStatusLEDs(int Toggle)
{
 if(Mem.StatusLEDOn)   
   {
    SetStatusLED((Toggle && WiFi.localIP()[0]==0) * 20, 
                 (Toggle && WiFi.localIP()[0]!=0) * 20 , 
                 (Toggle && deviceConnected) * 20);
    SetPCBLED09(   Toggle * 10);                                                              // Left LED
    SetPCBLED10((1-Toggle) * 10);                                                             // Right LED
    SetNanoLED13((1-Toggle) * 50);                                                            // LED on ESP32 board
   }
   else
   {
    SetStatusLED(0, 0, 0); 
    SetPCBLED09(0);                                                                           //
    SetPCBLED10(0);                                                                           //
    SetNanoLED13(0);      
   }
}
//--------------------------------------------                                                //
// COMMON Control the RGB LEDs on the Nano ESP32
// Analog range 0 - 512. 0 is LED Off, 512 is max intensity
// 512 is LED off. Therefore the value is subtracted from 512
// in core 3 the value to write is 13-bit 8191 to turn off the led completely
//--------------------------------------------
void SetStatusLED(int Red, int Green, int Blue)                                               // If LED should be off, use digitalWrite instead of analogWrite
{
 analogWrite(LED_RED,   Red   == 0 ? 8191 : (512 - Red));
 analogWrite(LED_GREEN, Green == 0 ? 8191 : (512 - Green));
 analogWrite(LED_BLUE,  Blue  == 0 ? 8191 : (512 - Blue));
}
//--------------------------------------------                                                //
// COMMON Control orange LED D13 on the Arduino 
//--------------------------------------------
void SetNanoLED13(int intensity) {analogWrite(secondsPin, intensity);}
//--------------------------------------------                                                //
// COMMON Control the RGB LED on the PCB
//--------------------------------------------
void SetPCBLED09(int intensity) {analogWrite(PCB_LED_D09, intensity);}
void SetPCBLED10(int intensity) {analogWrite(PCB_LED_D10, intensity);}

//--------------------------------------------                                                //
// COMMON check for serial input
//--------------------------------------------
void SerialCheck(void)
{
 String SerialString; 
 while (Serial.available())
    { 
     char c = Serial.read();                                                                  // Serial.write(c);
     if (c>31 && c<128) SerialString += c;                                                    // Allow input from Space - Del
     else c = 0;                                                                              // Delete a CR
    }
 if (SerialString.length()>0) 
    {
     ReworkInputString(SerialString);                                                         // Rework ReworkInputString();
     SerialString = "";
    }
}

//--------------------------------------------                                                //
// COMMON Reset to default settings. BLE On, WIFI NTP Off
//--------------------------------------------
void Reset(void)
{
 Mem.Checksum         = 25065;                                                                //
 Mem.DisplayChoice    = DEFAULTCOLOUR;                                                        // Default colour scheme 
 Mem.OwnColour        = green;                                                                // Own designed colour.
 Mem.DimmedLetter     = dgray;
 Mem.BackGround       = black; 
 Mem.LanguageChoice   = 0;                                                                    // 0 = NL, 1 = UK, 2 = DE, 3 = FR, 4 = Wheel
 Mem.LightReducer     = SLOPEBRIGHTNESS;                                                      // Factor to dim ledintensity with. Between 0.1 and 1 in steps of 0.05
 Mem.UpperBrightness  = MAXBRIGHTNESS;                                                        // Upper limit of Brightness in bits ( 1 - 1023)
 Mem.LowerBrightness  = LOWBRIGHTNESS;                                                        // Lower limit of Brightness in bits ( 0 - 255)
 Mem.TurnOffLEDsAtHH  = 0;                                                                    // Display Off at nn hour
 Mem.TurnOnLEDsAtHH   = 0;                                                                    // Display On at nn hour Not Used
 Mem.HetIsWasOff      = 0;                                                                    // Turn On or Off HET IS WAS   
 Mem.EdSoftLEDSOn     = 0;                                                                    // Turn On or Off EDSOFT LEDs
 Mem.RandomDisplay    = 0;                                                                    // Choose every day another display
 Mem.ByteFuture3      = 0;                                                                    // 
 Mem.BLEOn            = 1;                                                                    // default BLE On
 Mem.UseBLELongString = 0;                                                                    // Default off. works only with iPhone/iPad with BLEserial app
 Mem.NTPOn            = 0;                                                                    // NTP default off
 Mem.WIFIOn           = 0;                                                                    // WIFI default off
 Mem.MCUrestarted     = 0;                                                                    // MCU Restart counter
 Mem.WIFIcredentials  = NOT_SET;                                                              // Status of the WIFI connection
 //Mem.UseRotary      = 0;    // Do not erase this setting with a reset                       // Use the rotary coding
 Mem.DCF77On          = 0;                                                                    // Default off
 Mem.UseDS3231        = 0;                                                                    // Default off
 //Mem.LEDstrip       = 0;    // Do not erase this setting with a reset                       // 0 = SK6812, 1=WS2812
 Previous_LDR_read    = ReadLDR();                                                            // Read LDR to have a start value. max = 4096/8 = 255
 MinPhotocell         = Previous_LDR_read;                                                    // Stores minimum reading of photocell;
 MaxPhotocell         = Previous_LDR_read;                                                    // Stores maximum reading of photocell;                                            
 TestLDR              = 0;                                                                    // If true LDR display is printed every second
 uint16_t learnedRemoteAddress  = 0;
 uint8_t  learnedRemoteProtocol = 0;
 bool     remoteIdentified      = false;
 for (int i = 0; i < MAX_BUTTONS; i++)  IRMem.buttons[i].learned = 0;
 Tekstprintln("**** Reset of preferences ****"); 
 ResetCredentials();
 StoreStructInFlashMemory();                                                                  // Update Mem struct       
 GetTijd(false);                                                                              // Get the time and store it in the proper variables
 SWversion();                                                                                 // Display the version number of the software
 Displaytime();
}
//--------------------------------------------                                                //
// COMMON Reset to empty credential settings WIFI, NTP, BLE ON
//--------------------------------------------
void ResetCredentials(void)
{
 strcpy(Mem.SSID,"");                                                                         // Default SSID
 strcpy(Mem.Password,"");                                                                     // Default password
 strcpy(Mem.BLEbroadcastName,"WordClock");
 strcpy(Mem.Timezone,"CET-1CEST,M3.5.0,M10.5.0/3");                                           // Central Europe, Amsterdam, Berlin etc.
 Mem.WIFIcredentials  = 0;                                                                    // Status of the WIFI connection  
 Mem.WIFIOn           = 1;                                                                    // WIFI on
 Mem.NTPOn            = 1;
 Mem.BLEOn            = 1;                                                                    // default BLE On
 StoreStructInFlashMemory();                                                                  // Update Mem struct   
}
//--------------------------------------------                                                //
// COMMON common print routines
// %s - String
// %d - Integer (decimal)
// %04X - Hex with 4 digits, uppercase, leading zeros
// %08X - Hex with 8 digits, uppercase, leading zeros
// PRIX32 - 32-bit hex for uint32_t values
//--------------------------------------------
void Tekstprint(char const *tekst)    { if(Serial) Serial.print(tekst);  SendMessageBLE(tekst); } //sptext[0]=0; } 
void Tekstprintln(char const *tekst)  { sprintf(sptext,"%s\n",tekst); Tekstprint(sptext); }
void TekstSprint(char const *tekst)   { printf(tekst); } //sptext[0]=0;}                      // printing for Debugging purposes in serial monitor 
void TekstSprintln(char const *tekst) { sprintf(sptext,"%s\n",tekst); TekstSprint(sptext); }
//--------------------------------------------                                                //
// COMMON Print web menu page and BLE menu
// 0 = text to print, 1 = header of web page with menu, 2 = footer of web page
// html_info but be empty before starting: --> html_info[0] = 0; 
//--------------------------------------------
void WTekstappend(char const *tekst, char const *prefixtekst, char const *suffixtekst, bool newline) 
{
 if (newline) { sprintf(sptext, "%s\n", tekst); } 
 else {         sprintf(sptext, "%s", tekst);   }
 if(Mem.MCUrestarted == 0) Tekstprint(sptext);                                                // Otherwise the menu is printed during startup proces.
 size_t needed = strlen(prefixtekst) + strlen(tekst) + strlen(suffixtekst) + strlen("<br>");  // Estimate how much space will be added
 if (strlen(html_info) + needed > MAXSIZE_HTML_INFO - 1) 
     { strcat(html_info, "<br> *** Increase MAXSIZE_HTML_INFO ***<br>");   return;  }
 strcat(html_info, prefixtekst);                                             
 strcat(html_info, tekst);
 strcat(html_info, suffixtekst);
 if (newline) { strcat(html_info, "<br>"); }   // Append to html_info
}

void WTekstprintln(char const *tekst) { WTekstappend(tekst, "", "", true);}
void WTekstprintln(char const *tekst, char const *prefixtekst, char const *suffixtekst) 
                                      { WTekstappend(tekst, prefixtekst, suffixtekst, true); }

void WTekstprint(char const *tekst)   { WTekstappend(tekst, "", "", false);}
void WTekstprint(char const *tekst, char const *prefixtekst, char const *suffixtekst) 
                                      { WTekstappend(tekst, prefixtekst, suffixtekst, false);}

//--------------------------------------------                                                //
// COMMON Constrain a string with integers
// The value between the first and last character in a string is returned between the low and up bounderies
//--------------------------------------------
int SConstrainInt(String s,byte first,byte last,int low,int up){return constrain(s.substring(first, last).toInt(), low, up);}
int SConstrainInt(String s,byte first,          int low,int up){return constrain(s.substring(first).toInt(), low, up);}
//--------------------------------------------                                                //
// COMMON Init and check contents of EEPROM
//--------------------------------------------
void InitStorage(void)
{
 // if(!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED)){ Tekstprintln("Card Mount Failed");   return;}
 // else Tekstprintln("SPIFFS mounted"); 

 GetStructFromFlashMemory();
 if( Mem.Checksum != 25065)
   {
    sprintf(sptext,"Checksum (25065) invalid: %d\n Resetting to default values",Mem.Checksum); 
    Tekstprintln(sptext); 
    Reset();                                                                                  // If the checksum is NOK the Settings were not set
   }
 Mem.LanguageChoice  = _min(Mem.LanguageChoice, 4);                                           // Constrain the value to valid ranges 
 Mem.DisplayChoice   = _min(Mem.DisplayChoice, LASTITEM);                                     // Constrain the value to valid ranges 
 Mem.RandomDisplay   = 0;                                                                     // Turn it default off
 if(Mem.OwnColour == 0) Mem.OwnColour = green;                                                // If memory is empty cq black colour then store default value, green  
 Mem.LightReducer    = constrain(Mem.LightReducer,1,250);                                     // 
 Mem.LowerBrightness = constrain(Mem.LowerBrightness, 1, 250);                                // 
 Mem.UpperBrightness = _min(Mem.UpperBrightness, 255); 
// if(strlen(Mem.Password)<5 || strlen(Mem.SSID)<3)     Mem.WIFIOn = Mem.NTPOn = 0;           // If ssid or password invalid turn WIFI/NTP off
 if (strlen(Mem.BLEbroadcastName)<4) strcpy(Mem.BLEbroadcastName,"WordClock");
 if(Mem.LEDstrip  > 1) Mem.LEDstrip = 0;                                                      // Default SK6812 
 StoreStructInFlashMemory();
}
//--------------------------------------------                                                //
// COMMON Store mem.struct in FlashStorage or SD
//--------------------------------------------
void StoreStructInFlashMemory(void)
{
  FLASHSTOR.begin("Mem",false);       //  delay(100);
  FLASHSTOR.putBytes("Mem", &Mem , sizeof(Mem) );
  FLASHSTOR.end();          
  
// Can be used as alternative
//  SPIFFS
//  File myFile = SPIFFS.open("/MemStore.txt", FILE_WRITE);
//  myFile.write((byte *)&Mem, sizeof(Mem));
//  myFile.close();
 }
//--------------------------------------------                                                //
// COMMON Save IR remote settings 
//--------------------------------------------
 // Save IR remote settings separately
void StoreIRRemoteInFlashMemory(void)
{
  FLASHSTOR.begin("IRRemote", false);
  FLASHSTOR.putBytes("IRRemote", &IRMem, sizeof(IRMem));
  FLASHSTOR.end();
}

//--------------------------------------------                                                //
// COMMON Get data from FlashStorage
//--------------------------------------------
void GetStructFromFlashMemory(void)
{
 FLASHSTOR.begin("Mem", false);
 FLASHSTOR.getBytes("Mem", &Mem, sizeof(Mem) );
 FLASHSTOR.end(); 
// Can be used as alternative if no SD card
//  File myFile = SPIFFS.open("/MemStore.txt");  FILE_WRITE); myFile.read((byte *)&Mem, sizeof(Mem));  myFile.close();
 sprintf(sptext,"Mem.Checksum = %d",Mem.Checksum); Tekstprintln(sptext); 
}
//--------------------------------------------                                                //
// COMMON  Load IR remote settings from FlashStorage
// Preferences.h
//--------------------------------------------

void GetIRRemoteFromFlashMemory(void)
{
  FLASHSTOR.begin("IRRemote", true);
  FLASHSTOR.getBytes("IRRemote", &IRMem, sizeof(IRMem));
  FLASHSTOR.end();
}
//--------------------------------------------                                                //
// COMMON Version info
//--------------------------------------------
void SWversion(void) {SWversion(LastMenuformat);}                                             // LastMenuformat is default after startup true (= small menu)   
void SWversion(bool Small) 
{ 
 #define FILENAAM (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)    
 html_info[0] = 0;                                                                           // Empty the info web page to be used in void WTekstprintln() 
 LastMenuformat = Small;
 PrintLine(35);
 if(Small) {for (uint8_t i = 0; i < sizeof(menusmall) / sizeof(menusmall[0]); WTekstprintln(menusmall[i++]) ); }
 else      {for (uint8_t i = 0; i < sizeof(menu) / sizeof(menu[0]);           WTekstprintln(menu[i++]) ); }                                     
 PrintLine(35);
 sprintf(sptext,"Display off between: %02dh - %02dh",Mem.TurnOffLEDsAtHH, Mem.TurnOnLEDsAtHH);  WTekstprintln(sptext);
 PrintDisplayChoice(false);                                                                     WTekstprintln(sptext);
 sprintf(sptext, "RandomDisplay is %s", Mem.RandomDisplay==1 ? "ON/min" : 
                                        Mem.RandomDisplay==2 ? "ON/hour" : "OFF");              WTekstprintln(sptext);//
 sprintf(sptext,"Slope: %d     Min: %d     Max: %d ",
                 Mem.LightReducer, Mem.LowerBrightness,Mem.UpperBrightness);                    WTekstprintln(sptext);
 if(!Small) {sprintf(sptext,"SSID: %s", Mem.SSID);                                              WTekstprintln(sptext); }
// sprintf(sptext,"Password: %s", Mem.Password);                                                WTekstprintln(sptext);
 sprintf(sptext,"BLE name: %s", Mem.BLEbroadcastName);                                          WTekstprintln(sptext,"<span class=\"verdana-red\">","</span>");
 sprintf(sptext,"IP-address: %d.%d.%d.%d", WiFi.localIP()[0], WiFi.localIP()[1], 
                                           WiFi.localIP()[2], WiFi.localIP()[3] );              WTekstprint(sptext);
 sprintf(sptext,"/update");                                                                     WTekstprintln(sptext," <a href=" , "> /update</a>");                                            
 if(!Small) {sprintf(sptext,"Timezone:%s", Mem.Timezone);                                       WTekstprintln(sptext); }
 sprintf(sptext,"%s %s %s %s", Mem.WIFIOn?"WIFI=On":"WIFI=Off", 
                               Mem.NTPOn? "NTP=On":"NTP=Off",
                               Mem.BLEOn? "BLE=On":"BLE=Off",
                               Mem.UseBLELongString? "FastBLE=On":"FastBLE=Off" );              WTekstprintln(sptext);
 char fftext[20];              
 if(!Small) {sprintf(fftext,"%s", Mem.UseDS3231?" DS3231=On":" DS3231=Off"); }
 if(!Small) {sprintf(sptext,"%s %s",Mem.UseRotary==0 ?"No Rotary":
                        Mem.UseRotary==1 ?"Rotary=ON":
                        Mem.UseRotary==2 ?"Membrane=ON":
                        Mem.UseRotary==3 ?"IR-remote=ON":"NOP",fftext);                         WTekstprintln(sptext); }                           
 if(!Small) { sprintf(sptext,"%s strip with %d LEDs (switch %%)", 
                 Mem.LEDstrip==0?"SK6812":Mem.LEDstrip==1?"WS2812":"NOP",(int) NUM_LEDS);       WTekstprintln(sptext); }
  if(!Small) {sprintf(sptext,"Software: %s",FILENAAM);                                          WTekstprintln(sptext);}  // VERSION);
  if(!Small) {sprintf(sptext,"ESP32 Arduino core version: %d.%d.%d", 
          ESP_ARDUINO_VERSION_MAJOR,ESP_ARDUINO_VERSION_MINOR,ESP_ARDUINO_VERSION_PATCH);       WTekstprintln(sptext); }
 GetTijd(false);                                                                              // Get the time and store it in the proper variables
 PrintRTCTime();                                                                                
 PrintLine(35);                                                                               //
}

//--------------------------------------------                                                //
// COMMON PrintLine
//--------------------------------------------
void PrintLine(byte Lengte)
{
 for(int n=0; n<Lengte; n++) sptext[n]='_';
 sptext[Lengte] = 0;
 WTekstprintln(sptext);
 sptext[0] = 0;                                                                               // Suppress a second print of sptext
}

//--------------------------------------------                                                //
// COMMON Input from Bluetooth, Serial or HTML page
//--------------------------------------------
void ReworkInputString(String InputString)
{
 if(InputString.length()> 40){Serial.printf("Input string too long (max40)\n"); return;}      // If garbage return
 InputString.trim();                                                                          // Remove CR, LF etc.
 sptext[0] = 0;                                                                               // Suppress a second print of sptext 
 if(InputString[0] > 31 && InputString[0] < 127)                                              // Does the string start with a letter?
  {
  char cmd = toupper(InputString[0]);                                                         // Convert to uppercase once
  bool validLength = false;
  int len = InputString.length();
  switch(cmd) 
   {
    case 'A':                                                                                 // SSID setting
      validLength = (len > 4 && len < 30);
      if(validLength) 
        {
        InputString.substring(1).toCharArray(Mem.SSID, len);
        sprintf(sptext, "SSID set: %s", Mem.SSID);
        Mem.WIFIcredentials = NOT_SET;    
        WIFIwasConnected = false;    
        } 
      else sprintf(sptext, "**** Length fault. Use between 4 and 30 characters ****");
      break;
      
    case 'B':                                                                                 // Password setting
      if(InputString.equals("BBBB")) 
      {
        sprintf(sptext, "%s,**** Length fault. Use between 5 and 40 characters ****", Mem.Password);
        break;
      }
      validLength = (len > 4 && len < 40);
      if(validLength) 
        {
        InputString.substring(1).toCharArray(Mem.Password, len);
        sprintf(sptext, "Password set: %s\n Enter @ to reset ESP32 and connect to WIFI and NTP\n WIFI and NTP are turned ON", Mem.Password);
        Mem.NTPOn = Mem.WIFIOn = 1;                                                           // Turn both on
        Mem.WIFIcredentials = NOT_SET;
        WIFIwasConnected = false;
        } 
      else sprintf(sptext, "**** Length fault. Use between 5 and 40 characters ****");
      break;

    case 'C':                                                                                 // BLE settings
      if(InputString.equals("CCC")) 
       {
        Mem.BLEOn = 1 - Mem.BLEOn;
        sprintf(sptext, "BLE is %s after restart", Mem.BLEOn ? "ON" : "OFF");
        break;
       }
      validLength = (len > 4 && len < 30);
      if(validLength) 
       {
        InputString.substring(1).toCharArray(Mem.BLEbroadcastName, len);
        sprintf(sptext, "BLE broadcast name set: %s", Mem.BLEbroadcastName);
        Mem.BLEOn = 1;
       } 
      else sprintf(sptext, "**** Length fault. Use between 4 and 30 characters ****");
      break;
      
    case 'D':                                                                                 // Date entry
      if(len == 9) 
       {
        timeinfo.tm_mday = (int)SConstrainInt(InputString, 1, 3, 0, 31);
        timeinfo.tm_mon = (int)SConstrainInt(InputString, 3, 5, 0, 12) - 1;
        timeinfo.tm_year = (int)SConstrainInt(InputString, 5, 9, 2000, 9999) - 1900;
        if(DS3231Installed)   SetDS3231Time();
        else sprintf(sptext, "No external RTC module detected");
       } 
      else sprintf(sptext, "****\nLength fault. Enter Dddmmyyyy\n****");
      break;
      
    case 'E':                                                                                 // Time zone setting
      validLength = (len > 2);
      if(validLength) 
       {
        InputString.substring(1).toCharArray(Mem.Timezone, len);
        sprintf(sptext, "Timezone set: %s", Mem.Timezone);
       } 
      else sprintf(sptext, "**** Length fault. Use more than 2 characters ****");
      break;
      
    case 'F':                                                                                 // Font color setting
      if(len == 9) 
       {
        LetterColor = Mem.OwnColour = HexToDec(InputString.substring(1, 9));
        sprintf(sptext, "Font colour stored: 0X%08" PRIX32, Mem.OwnColour);
        Tekstprintln("**** Own colour changed ****");
        LedsOff();
        SetSecondColour();                                                                    // Set the colour per second of 'IS' and 'WAS' 
        Displaytime();
        sptext[0]=0;                                                                          // Suppress a second print of sptext
       } 
      else sprintf(sptext, "****Length fault. Enter Frrggbb hexadecimal (0 - F)****\nStored: 0X%08" PRIX32, Mem.OwnColour);
      break;
      
    case 'G':                                                                                 // Scan WIFI stations
      if(len == 1) 
       {
        //ScanWIFI();
        CheckforWIFINetwork(true);
        if(WIFIwasConnected) WiFi.reconnect();
       } 
      else sprintf(sptext, "**** Length fault. Enter G ****");
      break;
      
    case 'H':                                                                                 // Use rotary encoder
      if(len == 3) 
       {
        Mem.UseRotary = (byte)SConstrainInt(InputString, 2, 0, 3);                           // keep the range between 0 and 3
        if(Mem.UseRotary > 3) Mem.UseRotary = 0;
        sprintf(sptext, "\nUse of rotary encoder is %s\nUse of membrane keypad is %s", 
                Mem.UseRotary == 1 ? "ON" : "OFF", Mem.UseRotary == 2 ? "ON" : "OFF");
        Tekstprintln(sptext);
        if(Mem.UseRotary > 0)  {Mem.NTPOn = 0;              Mem.UseDS3231 = 1;  }             // Configure related settings based on rotary use 
        else                   {Mem.WIFIOn = Mem.NTPOn = 1; Mem.UseDS3231 = 0;}
        sprintf(sptext, "Use DS3231 is %s, WIFI is %s, NTP is %s\n *** Restart clock with @ ***", 
                Mem.UseDS3231 ? "ON" : "OFF", Mem.WIFIOn ? "ON" : "OFF", Mem.NTPOn ? "ON" : "OFF");
        } 
      else sprintf(sptext, "**** Fault. Enter H000 (none), H001=Rotary, H002=Membrane ****\nUse rotary encoder is %s\nUse membrane keypad %s\nUse IR-remote control is %s",
                    Mem.UseRotary == 1 ? "ON" : "OFF", Mem.UseRotary == 2 ? "ON" : "OFF",Mem.UseRotary==3  ? "ON" : "OFF");
      break;
     

    case 'I':                                                                                 // Menu
      SWversion(len == 1);                                                                    // true for small menu, false for full menu
      break;
      
    case 'J':                                                                                 // Use DS3231 RTC module
      if(len == 1) 
      {
        Mem.UseDS3231 = 1 - Mem.UseDS3231;
        Mem.NTPOn = (1 - Mem.UseDS3231);
        if(Mem.WIFIOn == 0) Mem.NTPOn = 0;                                                    // If WIFI is Off then No NTP
        sprintf(sptext, "Use DS3231 is %s, WIFI is %s, NTP is %s", 
                Mem.UseDS3231 ? "ON" : "OFF", Mem.WIFIOn ? "ON" : "OFF", Mem.NTPOn ? "ON" : "OFF");
       } 
      else sprintf(sptext, "**** Length fault. Enter J ****");
      break;
      
    case 'K':                                                                                 // Test LDR
      TestLDR = 1 - TestLDR;
      sprintf(sptext, "TestLDR: %s", TestLDR ? "On\n   Bits, Out, loops per second and time" : "Off\n");
      break;
      
    case 'L':                                                                                 // Lower brightness
      validLength = (len > 1 && len < 5);
      if(validLength) 
       {
        Mem.LowerBrightness = (byte)SConstrainInt(InputString, 1, 0, 255);
        sprintf(sptext, "Lower brightness: %d bits", Mem.LowerBrightness);
       } 
      else sprintf(sptext, "**** Input fault. \nEnter Lnnn where n between 1 and 255");
      break;
      
    case 'M':                                                                                 // Max brightness
      validLength = (len > 1 && len < 5);
      if(validLength) 
       {
        Mem.UpperBrightness = SConstrainInt(InputString, 1, 1, 255);
        sprintf(sptext, "Upper brightness changed to: %d bits", Mem.UpperBrightness);
       } 
      else sprintf(sptext, "**** Input fault. \nEnter Mnnn where n between 1 and 255");
      break;
      
    case 'N':                                                                                 // Turn off display between hours
      sprintf(sptext, "**** Length fault N. ****");
      if(len == 1) { Mem.TurnOffLEDsAtHH = Mem.TurnOnLEDsAtHH = 0;  } 
      else if(len == 5) 
       {
        Mem.TurnOffLEDsAtHH = (byte)InputString.substring(1, 3).toInt();
        Mem.TurnOnLEDsAtHH  = (byte)InputString.substring(3, 5).toInt();
       }
      Mem.TurnOffLEDsAtHH = _min(Mem.TurnOffLEDsAtHH, 23);
      Mem.TurnOnLEDsAtHH  = _min(Mem.TurnOnLEDsAtHH, 23);
      sprintf(sptext, "Display is OFF between %2d:00 and %2d:00", Mem.TurnOffLEDsAtHH, Mem.TurnOnLEDsAtHH);
      break;
      
    case 'O':                                                                                 // Turn On/Off Display
      if(len == 1) 
       {
        LEDsAreOff = !LEDsAreOff;
        sprintf(sptext, "Display is %s", LEDsAreOff ? "OFF" : "ON");
        if(LEDsAreOff) { ClearScreen(); }
        else 
         {
          Tekstprintln(sptext);
          lastminute = 99;                                                                    // Force display update
          sptext[0]=0;                                                                        // Suppress a second print of sptext
         }
       } 
      else sprintf(sptext, "**** Length fault O. ****");
      break;
      
    case 'P':                                                                                 // Status LEDs On/Off
      if(len == 1) {
        Mem.StatusLEDOn = !Mem.StatusLEDOn;
        UpdateStatusLEDs(0);
        sprintf(sptext, "StatusLEDs are %s", Mem.StatusLEDOn ? "ON" : "OFF");
      } else sprintf(sptext, "**** Length fault P. ****");
      break;
      
    case 'Q':                                                                                 // Display choice
      if(len == 1)                                                                            // Print display options menu   
       {
        Tekstprintln("  Q0= Default yellow colour");
        Tekstprintln("  Q1= Hourly colour");
        Tekstprintln("  Q2= All white");
        Tekstprintln("  Q3= All Own colour");
        Tekstprintln("  Q4= Own colour, HETISWAS changing");
        Tekstprintln("  Q5= Wheel colour");
        Tekstprintln("  Q6= Digital display");
        Tekstprintln("  Q7= Hourly colour, HETISWAS changing");        
        Tekstprintln("  Q8= Rainbow colour");
        if (Mem.RandomDisplay) Tekstprintln("Random Display changing in ON\nChoose a display to stop it");
        Tekstprintln("In menu: ~1 or ~ is changing per minute, ~2 changing per hour.");
        sptext[0] = 0;                                                                        // Suppress a second print of sptext
       } 
      else 
      {
       byte dp = (byte)InputString.substring(1, 2).toInt();
       if(dp>LASTITEM) dp=0;
       Mem.DisplayChoice = dp;
       PrintDisplayChoice(); 
       sptext[0] = 0;                                                                         // Suppress a second print of sptext
       Mem.RandomDisplay = 0;                                                                 // turn Off random changing display
      }
      lastminute = 99;                                                                        // Force a minute update
      break;
      
    case 'R':                                                                                 // Reset to default settings
      if(InputString.equals("RRRRR"))                                                         // Delete WIFI settings and set defaults
      {
        Reset();
        ResetCredentials();
        ESP.restart();
        break;
      }
      if(InputString.equals("RRR"))                                                           // Delete WIFI settings only
       { 
        ResetCredentials();
        sprintf(sptext, "\nSSID and password deleted. \nWIFI, NTP and BLE is On\n Enter @ to restart");
        break;
       }
      if(len == 1)                                                                            // Set to default settings
        {
        Reset();
        sprintf(sptext, "\nReset to default values: Done");
        lastminute = 99;                                                                      // Force a minute update
        Displaytime();
       } 
      else sprintf(sptext, "**** Length fault R. ****");
      break;
      
    case 'S':                                                                                 // Slope factor for brightness
      validLength = (len > 1 && len < 5);
      if(validLength) 
       {
        Mem.LightReducer = (byte)SConstrainInt(InputString, 1, 1, 255);
        sprintf(sptext, "Slope brightness changed to: %d%%", Mem.LightReducer);
       } 
      else sprintf(sptext, "**** Input fault. \nEnter Snnn where n between 1 and 255");
      break;
      
    case 'T':                                                                                 // Time setting
      if(len == 7) 
       {
        timeinfo.tm_hour = (int)SConstrainInt(InputString, 1, 3, 0, 23);
        timeinfo.tm_min = (int)SConstrainInt(InputString, 3, 5, 0, 59);
        timeinfo.tm_sec = (int)SConstrainInt(InputString, 5, 7, 0, 59);
        if(DS3231Installed)   SetDS3231Time();
        else sprintf(sptext, "No external RTC module detected");
       } 
      else sprintf(sptext, "**** Length fault. Enter Thhmmss ****");
      break;
      
    case 'U':                                                                                 // Demo mode
      if(len == 1) 
      { 
        if(Demo)  {   lastminute = 99;  Demo = false; }                                       // Force a minute update and turn off demo mode
        else { MilliSecondValue = 200;  Demo = true;  }                                       // If Demomode was off turn it on with 200 msec delay 
        sprintf(sptext, "Demo mode: %s, %d msec/min", Demo ? "ON" : "OFF", MilliSecondValue);    
       } 
      else if(len > 1 && len < 6) 
       {
        MilliSecondValue = InputString.substring(1, 5).toInt();
        Demo = true;
        sprintf(sptext, "Demo mode: %s, %d msec/min", Demo ? "ON" : "OFF", MilliSecondValue);
       } 
      else sprintf(sptext, "**** Length fault U. Demo mode (Unnn or U) ****");
      break;
      
    case 'W':                                                                                 // WIFI toggle
      if(len == 1) 
       {
        Mem.WIFIOn = 1 - Mem.WIFIOn;
        Mem.NTPOn = Mem.WIFIOn;                                                               // If WIFI is off turn NTP also off
        sprintf(sptext, "WIFI is %s after restart", Mem.WIFIOn ? "ON" : "OFF");
       } 
      else sprintf(sptext, "**** Length fault. Enter W ****");
      break;
      
    case 'X':                                                                                 // NTP toggle
      if(len == 1) 
       {
        Mem.NTPOn = 1 - Mem.NTPOn;
        sprintf(sptext, "NTP is %s after restart", Mem.NTPOn ? "ON" : "OFF");
       } 
      else sprintf(sptext, "**** Length fault. Enter X ****");
      break;
      
    case 'Y':                                                                                 // Play lights
      Play_Lights();
      lastminute = 99;                                                                        // Force a minute update
      sprintf(sptext, "**** Play Lights");
      break;
      
    case 'Z':                                                                                 // Start WPS
      sprintf(sptext, "**** Start WPS on your router");
      WiFi.onEvent(WiFiEvent);
      WiFi.mode(WIFI_STA);
      Serial.println("Starting WPS");
      wpsInitConfig();
      wpsStart();
      break;
      
    case '!':                                                                                 // Print times
      if(len == 1) PrintAllClockTimes();
      sprintf(sptext,"\n");
      break;
      
    case '@':                                                                                 // Reset ESP
      if(len == 1) 
       {
        Tekstprintln("\n*********\n ESP restarting\n*********\n");
        ESP.restart();
       } 
      else sprintf(sptext, "**** Length fault. Enter @ ****");
      break;
      
    case '#':                                                                                 // Self test
      if(len == 1) 
       {
        Zelftest = 1 - Zelftest;
        sprintf(sptext, "Zelftest: %s", Zelftest ? "ON" : "OFF");
        Selftest();
        Displaytime();
       } 
      else if(len > 1 && len < 6) 
       {
        MilliSecondValue = InputString.substring(1, 5).toInt();
        Zelftest = 1 - Zelftest;
        sprintf(sptext, "Zelftest: %s", Zelftest ? "ON" : "OFF");
        Selftest(MilliSecondValue);
        Displaytime();
       } 
      else sprintf(sptext, "**** Length fault #. Demo mode (#nnnn or #) ****");
      break;
      
    case '$':                                                                                 // Fire display
      FireDisplay = 1 - FireDisplay;
      sprintf(sptext, "FireDisplay: %s", FireDisplay ? "On" : "Off");
      ClearScreen();
      Displaytime();
      break;
      
    case '%':                                                                                 // LED strip type
      if(len == 1) 
       {
        Mem.LEDstrip = 1 - Mem.LEDstrip;
        sprintf(sptext, "LED strip is %s after restart", Mem.LEDstrip ? "WS2812" : "SK6812");
       } 
      else sprintf(sptext, "**** Length fault . ****");
      break;

     case '^':                                                                                 // *** Empty ***
      if(len == 1) 
       {
       // sprintf(sptext, "--- %s ---", Mem.ByteFutureX ? "OFF" : "ON");
       sprintf(sptext, "**** No Use option . ****");
       } 
      else sprintf(sptext, "**** Length fault . ****");
      break;

    case '&':                                                                                 // Force NTP update
      if(len == 1) 
       {
        SetDS3231Time();
        SetRTCTime();
        PrintAllClockTimes();
        sprintf(sptext,"");
       } 
      else sprintf(sptext, "**** Length fault &. ****");
      break;
      
    case '(':  
      Mem.EdSoftLEDSOn = !Mem.EdSoftLEDSOn;
      sprintf(sptext, "EDSOFT is %s", Mem.EdSoftLEDSOn ? "ON" : "OFF");
      break;

    case ')':
      Mem.HetIsWasOff = !Mem.HetIsWasOff;
      sprintf(sptext, "HET IS WAS is %s", Mem.HetIsWasOff ? "OFF" : "ON");
      break;

    case '{':  
      if (Mem.UseRotary==3)
       {
         StartIRLearning();   // Initializes learning mode,Resets all button data,Prompts for first button
         sprintf(sptext, "Learning IR started");
       }
      else sprintf(sptext, "IR remote is OFF (Turn on with } )\nor no IR remote installed");
      break;

    case '}':                                                                                 // *** Empty ***
      if(len == 1) 
       {
       // sprintf(sptext, "--- %s ---", Mem.ByteFutureX ? "OFF" : "ON");
       sprintf(sptext, "**** No Use option . ****");
       } 
      else sprintf(sptext, "**** Length fault . ****");
      break;

    case '~':
      if(len == 1) 
       {
        if (Mem.RandomDisplay > 0) Mem.RandomDisplay = 0;
        else                       Mem.RandomDisplay = 1;
       }
      if(len == 2) 
       {
        byte ff = (byte)InputString.substring(1, 2).toInt();
        if (ff < 3) Mem.RandomDisplay = ff;
       }
      Mem.DisplayChoice = DEFAULTCOLOUR;
      sprintf(sptext, "Random display is %s", Mem.RandomDisplay==1 ? "ON/min" : Mem.RandomDisplay==2 ? "ON/hour" : "OFF");
      break;

    case '+':                                                                                 // BLE string toggle
      if(len == 1) 
       {
        Mem.UseBLELongString = 1 - Mem.UseBLELongString;
        sprintf(sptext, "Fast BLE is %s", Mem.UseBLELongString ? "ON" : "OFF");
       } 
      else sprintf(sptext, "**** Length fault. Enter + ****");
      break;
      
    case '_':                                                                                 // Store in flash
      if(len > 1 && len < 4) 
       {
        byte ff = (byte)InputString.substring(1, 3).toInt();
        sprintf(sptext, "No use: %d", ff);
        Tekstprintln(sptext);
       }
      break;

    case '=':                                                                                 // Print permanent Mem memory
      if(len == 1)  PrintMem(); 
      else sprintf(sptext, "**** Length fault. Enter = ****");
      break;     

    case '0': 
    case '1': 
    case '2':                                                             // Time entry compatibility mode
      if(len == 6)                                                        // The first digit of InputString is 0, 1 or 2
       {
        timeinfo.tm_hour = (int)SConstrainInt(InputString, 0, 2, 0, 23);
        timeinfo.tm_min  = (int)SConstrainInt(InputString, 2, 4, 0, 59);
        timeinfo.tm_sec  = (int)SConstrainInt(InputString, 4, 6, 0, 59);
        if(DS3231Installed)   SetDS3231Time();
        else sprintf(sptext, "No external RTC module detected");
       }   
      else sprintf(sptext, "**** Length fault. Enter Thhmmss ****");
      break;
  }
  Tekstprintln(sptext);
  StoreStructInFlashMemory();                                                                 // Update EEPROM
 }
InputString = "";
}
//--------------------------------------------                                                //
// COMMON Print Display choice
// Default print the text
//--------------------------------------------
void PrintDisplayChoice() {PrintDisplayChoice(true);}
void PrintDisplayChoice(bool PrintIt)
{
 byte dp = Mem.DisplayChoice;
 sprintf(sptext,"Display choice: %s",
        dp==0?"Yellow+":dp==1?"Hourly":dp==2?"White"  :dp==3?"All Own":
        dp==4?"Own+"   :dp==5?"Wheel" :dp==6?"Digital":dp==7?"Hourly+":
        dp==8?"Rainbow":      "NOP");        
 if (PrintIt) Tekstprintln(sptext);
}
//--------------------------------------------                                                //
// COMMON Print time input method
// Rotary, Membrane or IR-remote
//--------------------------------------------
 void PrintTimeInputMethod(byte Im)
 {
  char Method[10];
  switch(Im)
   {
     case 0: strcpy(Method, "None");     break;
     case 1: strcpy(Method, "Rotary");   break;
     case 2: strcpy(Method, "Membrane"); break;
     case 3: strcpy(Method, "IrRemote"); break;
    default: strcpy(Method, "ErrorOOR"); break; 
   }
  sprintf(sptext, "Time inputMethod: %s", Method);
 }
//--------------------------------------------                                                //
// COMMON Print permanent Mem memory
//--------------------------------------------
void PrintMem(void)
{
 PrintDisplayChoice();
 sprintf(sptext," TurnOffLEDsAtHH: %d",Mem.TurnOffLEDsAtHH);                Tekstprintln(sptext);
 sprintf(sptext,"  TurnOnLEDsAtHH: %d",Mem.TurnOnLEDsAtHH);                 Tekstprintln(sptext);
 sprintf(sptext,"  LanguageChoice: %d",Mem.LanguageChoice);                 Tekstprintln(sptext);
 sprintf(sptext,"    LightReducer: %d",Mem.LightReducer);                   Tekstprintln(sptext);
 sprintf(sptext," LowerBrightness: %d",Mem.LowerBrightness);                Tekstprintln(sptext);
 sprintf(sptext," UpperBrightness: %d",Mem.UpperBrightness);                Tekstprintln(sptext);
 for (int i=0;i<12;i++) { sprintf(sptext,"%03d ",Mem.NVRAMmem[i]);          Tekstprint(sptext); }   Tekstprintln("");
 for (int i=12;i<24;i++){ sprintf(sptext,"%03d ",Mem.NVRAMmem[i]);          Tekstprint(sptext); }   Tekstprintln("");
 sprintf(sptext,"   DisplayChoice: %d",Mem.DisplayChoice);                  Tekstprintln(sptext);                                                                   
 sprintf(sptext,"           BLEOn: %s",Mem.BLEOn ? "ON" : "OFF");           Tekstprintln(sptext);
 sprintf(sptext,"           NTPOn: %s",Mem.NTPOn ? "ON" : "OFF");           Tekstprintln(sptext);
 sprintf(sptext,"          WIFIOn: %s",Mem.WIFIOn ? "ON" : "OFF");          Tekstprintln(sptext);
 sprintf(sptext,"     StatusLEDOn: %s",Mem.StatusLEDOn ? "ON" : "OFF");     Tekstprintln(sptext);
 sprintf(sptext,"    MCUrestarted: %d",Mem.MCUrestarted );                  Tekstprintln(sptext);                                              
 sprintf(sptext,"         DCF77On: %s",Mem.DCF77On  ? "ON" : "OFF");        Tekstprintln(sptext);
 sprintf(sptext,"       UseRotary: %d",Mem.UseRotary);                      Tekstprintln(sptext);
 PrintTimeInputMethod(Mem.UseRotary);
 sprintf(sptext,"       UseDS3231: %s",Mem.UseDS3231 ? "ON" : "OFF");       Tekstprintln(sptext);
 sprintf(sptext,"       LED strip: %s", Mem.LEDstrip?"WS2812":"SK6812" );   Tekstprintln(sptext);
 sprintf(sptext,"      FiboChrono: %s",Mem.FiboChrono ? "FIBO" : "CHRONO"); Tekstprintln(sptext);
 sprintf(sptext,"          NoExUl: %s", Mem.NoExUl ? "ON" : "OFF");         Tekstprintln(sptext);
 byte wc = Mem.WIFIcredentials; 
 sprintf(sptext," WIFIcredentials: %s", wc==0? "Not SET" : wc==1? "SET" : wc==2? "SET&OK": 
                                   wc==3? "in AP not SET":"Unknown code");  Tekstprintln(sptext);
 sprintf(sptext,"      IntFuture2: %d",Mem.IntFuture2 );                    Tekstprintln(sptext);
 sprintf(sptext,"      IntFuture3: %d",Mem.IntFuture3 );                    Tekstprintln(sptext);
 sprintf(sptext,"      HET IS WAS: %s", Mem.HetIsWasOff ? "OFF" : "ON");    Tekstprintln(sptext);
 sprintf(sptext,"       EDSOFT is: %s", Mem.EdSoftLEDSOn ? "ON" : "OFF");   Tekstprintln(sptext);
 sprintf(sptext,"   RandomDisplay: %s", Mem.RandomDisplay? "ON" : "OFF");   Tekstprintln(sptext);
 sprintf(sptext,"     ByteFuture3: %d", Mem.ByteFuture3);                   Tekstprintln(sptext);
 sprintf(sptext,"UseBLELongString: %s",Mem.UseBLELongString?"ON":"OFF");    Tekstprintln(sptext);
 sprintf(sptext,"     Font colour: 0X%08" PRIX32, Mem.OwnColour);           Tekstprintln(sptext);
 sprintf(sptext,"    DimmedLetter: 0X%08" PRIX32,Mem.DimmedLetter);         Tekstprintln(sptext);
 sprintf(sptext,"      BackGround: 0X%08" PRIX32,Mem.BackGround);           Tekstprintln(sptext);
 sprintf(sptext,"            SSID: %s",Mem.SSID);                           Tekstprintln(sptext);
 // sprintf(sptext,"BackGround: %s",Mem.Password);                          Tekstprintln(sptext);
 sprintf(sptext,"BLEbroadcastName: %s",Mem.BLEbroadcastName);               Tekstprintln(sptext);
 sprintf(sptext,"        Timezone: %s",Mem.Timezone);                       Tekstprintln(sptext);
 sprintf(sptext,"        Checksum: %d",Mem.Checksum);                       Tekstprintln(sptext);
 PrintAllMappings();                                                                          // Print the IR remote keys
 sptext[0]=0;                                                                                 // Suppress a second print of sptext
}
//--------------------------------------------                                                //
// LDR reading are between 0 and 255. 
// ESP32 analogue read is between 0 - 4096 --   is: 4096 / 8
//--------------------------------------------
int ReadLDR(void)
{
 return analogRead(PhotoCellPin)/16;
}

//--------------------------------------------                                                //
//  CLOCK Set second colour
//  Set the colour per second of 'IS' and 'WAS'
//---------------------------------------------
void SetSecondColour(void)
{
                             #ifdef FOURLANGUAGECLOCK
  return;                                                                                     // 4-language clock uses SetColours() once per minute
                             #endif  //FOURLANGUAGECLOCK    
 static byte kleur = 0;                                                                      //  color increases per second in RAINBOW
 MINColor = FuncCRGBW(15 + timeinfo.tm_min * 4, 255 - timeinfo.tm_min * 4, 0, 0); 
 SECColor = FuncCRGBW(15 + timeinfo.tm_sec * 4, 255 - timeinfo.tm_sec * 4, 0, 0 );   
                                                                                              // Light up  IS or WAS with the proper colour  
 switch (Mem.DisplayChoice)
  {
   case DEFAULTCOLOUR: LetterColor = DefaultColor;                                     break; // Yellow text with changing MIN and SEC  
   case HOURLYCOLOUR : LetterColor = MINColor = SECColor = HourColor[timeinfo.tm_hour];break; // A colour every hour, not changing MIN and SEC
   case WHITECOLOR   : LetterColor = MINColor = SECColor = white;                      break; // All white
   case OWNCOLOUR    : LetterColor = MINColor = SECColor = Mem.OwnColour;              break; // Own colour
   case OWNHETISWAS  : LetterColor = Mem.OwnColour;                                    break; // Own colour except HET IS WAS  
   case WHEELCOLOR   : LetterColor = MINColor = SECColor = Wheel((timeinfo.tm_min*4)); break; // Colour of all letters changes per minute
   case DIGITAL      : MINColor = SECColor = 0;                                        break; // Digital display of time. No IS WAS turn color off in display
   case HOURHETISWAS : LetterColor = HourColor[timeinfo.tm_hour];                      break; // A colour every hour changing MIN and SEC
   case RAINBOW      : LetterColor = MINColor = SECColor = Wheel(kleur++);             break; // Colour of all letters changes per second 
  }
 if(Mem.HetIsWasOff){MINColor = SECColor = 0;}                                                // If HET IS WAS is turned off in menu with Q9
 NoTextInLeds  = true;                                                                        // Flag to control printing of the text IS en WAS in serial
                    #ifdef DE144CLOCK  
 if(Is) {IST;} else {WAR;}                      
                    #endif  //DE144CLOCK                      
 if(Is) {IS;}  else {WAS;} 
 if (Mem.DisplayChoice == RAINBOW ) Displaytime();                                            // Now we have to send the time to the display every second 
 NoTextInLeds  = false;                                                                       // Flag to control printing of the text IS en WAS in serial
 ShowLeds();                                                                                  // Updating IS and WAS with ShowLeds is done here to avoid updating all letters every second with Displaytime function
 
 // Serial.print("   SecColor: ");    Serial.println(SECColor,HEX);  
 // Serial.print("   MinColor: ");    Serial.println(MINColor,HEX);  
 // Serial.print("LetterColor: ");    Serial.println(LetterColor,HEX);         
}

                    #ifdef FOURLANGUAGECLOCK
//--------------------------------------------                                                //
//  CLOCK Set second for 4 language clock
//--------------------------------------------
void SetColours(void)
{  
 static byte kleur = 0;                                                                      //  color increases per second in RAINBOW 
 switch (Mem.DisplayChoice)
  {
   case DEFAULTCOLOUR: LetterColor   = DefaultColor; 
                       UKLetterColor = UKDefaultColor;
                       DELetterColor = DEDefaultColor;
                       FRLetterColor = FRDefaultColor;                      break;            // HET IS WAS changing   
   case HOURHETISWAS :
   case HOURLYCOLOUR : LetterColor   = HourColor[timeinfo.tm_hour];
                       UKLetterColor = HourColor[(timeinfo.tm_hour+1)%24];
                       DELetterColor = HourColor[(timeinfo.tm_hour+2)%24];
                       FRLetterColor = HourColor[(timeinfo.tm_hour+3)%24];  break;            // A colour every hour   
   case WHITECOLOR   : LetterColor   = white; 
                       UKLetterColor = white;
                       DELetterColor = white;
                       FRLetterColor = white;                               break;            // All white
   case OWNCOLOUR    :  
   case OWNHETISWAS  : LetterColor   = Mem.OwnColour; 
                       UKLetterColor = Mem.OwnColour;
                       DELetterColor = Mem.OwnColour;
                       FRLetterColor = Mem.OwnColour;                       break;            // Own colour HET IS WAS changing  
   case WHEELCOLOR   : LetterColor   = Wheel((timeinfo.tm_min*4));       
                       UKLetterColor = Wheel((60 +timeinfo.tm_min*4));
                       DELetterColor = Wheel((120+timeinfo.tm_min*4));
                       FRLetterColor = Wheel((180+timeinfo.tm_min*4));      break;            // Colour of all letters changes per second
   case RAINBOW      : LetterColor   = Wheel((     kleur++));       
                       UKLetterColor = Wheel((60 + kleur));
                       DELetterColor = Wheel((120+ kleur));
                       FRLetterColor = Wheel((180+ kleur));                 break;            // Colour of all letters changes per second                       
   case DIGITAL      :                                                      break;            //LetterColor = Mem.OwnColour;                 break;            // digital display of time. No IS WAS turn color off in display
  }
}
 if (Mem.DisplayChoice == RAINBOW)  Displaytime();                                           // Now we have to send the time to the display every every second
                     #endif  //FOURLANGUAGECLOCK       

//------------------------------------------------------------------------------
// CLOCK Self test sequence
//------------------------------------------------------------------------------
void Selftest(void){Selftest(900);}
void Selftest(int Delayms)
{ 
 GetTijd(true);                                                                                  // Prints time in Serial monitor
 LedsOff(); 
                      #if defined NLM1M2M3M4
 HET;   Laatzien(Delayms); IS;    Laatzien(Delayms); MVIJF; Laatzien(Delayms); MTIEN;   Laatzien(Delayms); MVOOR;  Laatzien(Delayms); OVER;   Laatzien(Delayms);
 KWART; Laatzien(Delayms); HALF;  Laatzien(Delayms); OVER2; Laatzien(Delayms); VOOR;    Laatzien(Delayms); EEN;    Laatzien(Delayms); TWEE;   Laatzien(Delayms);  
 DRIE;  Laatzien(Delayms); VIER;  Laatzien(Delayms); VIJF;  Laatzien(Delayms); ZES;     Laatzien(Delayms); ZEVEN;  Laatzien(Delayms); ACHT;   Laatzien(Delayms); 
 NEGEN; Laatzien(Delayms); TIEN;  Laatzien(Delayms); ELF;   Laatzien(Delayms); TWAALF;  Laatzien(Delayms); UUR;    Laatzien(Delayms); 
 MIN1;  Laatzien(Delayms); MIN2;  Laatzien(Delayms); MIN3;  Laatzien(Delayms); MIN4;    Laatzien(Delayms);
                      #endif  //NLM1M2M3M4
                      #ifdef NL144CLOCK                    
 HET;   Laatzien(Delayms); IS;    Laatzien(Delayms); WAS;    Laatzien(Delayms); PRECIES; Laatzien(Delayms); MTIEN;  Laatzien(Delayms);  MVIJF; Laatzien(Delayms);    
 KWART; Laatzien(Delayms); VOOR;  Laatzien(Delayms); OVER;   Laatzien(Delayms); HALF;    Laatzien(Delayms); MIDDER; Laatzien(Delayms);  VIJF;  Laatzien(Delayms);
 TWEE;  Laatzien(Delayms); EEN;   Laatzien(Delayms); VIER;   Laatzien(Delayms); TIEN;    Laatzien(Delayms); TWAALF; Laatzien(Delayms);  DRIE;  Laatzien(Delayms);
 NEGEN; Laatzien(Delayms); NACHT; Laatzien(Delayms); ACHT;   Laatzien(Delayms); ZES;     Laatzien(Delayms); ZEVEN;  Laatzien(Delayms);  ELF;   Laatzien(Delayms); 
 NOEN;  Laatzien(Delayms); UUR;   Laatzien(Delayms); EDSOFT; Laatzien(Delayms);
                      #endif  //NL144CLOCK
                      #ifdef NL92CLOCK                    
 HET;   Laatzien(Delayms); IS;    Laatzien(Delayms); WAS;    Laatzien(Delayms); PRECIES; Laatzien(Delayms); MTIEN;  Laatzien(Delayms);  MVIJF; Laatzien(Delayms);    
 KWART; Laatzien(Delayms); VOOR;  Laatzien(Delayms); OVER;   Laatzien(Delayms); HALF;    Laatzien(Delayms);  VIJF;  Laatzien(Delayms);
 TWEE;  Laatzien(Delayms); EEN;   Laatzien(Delayms); VIER;   Laatzien(Delayms); TIEN;    Laatzien(Delayms); TWAALF; Laatzien(Delayms);  DRIE;  Laatzien(Delayms);
 NEGEN; Laatzien(Delayms); ACHT;  Laatzien(Delayms); ZES;    Laatzien(Delayms); ZEVEN;   Laatzien(Delayms);  ELF;   Laatzien(Delayms); 
 UUR;   Laatzien(Delayms); //EDSOFT; Laatzien(Delayms);
                      #endif  //NL92CLOCK
                      #ifdef DE144CLOCK                    
 ES;    Laatzien(Delayms);    IST;    Laatzien(Delayms); WAR;    Laatzien(Delayms); GENAU; Laatzien(Delayms); MZEHN; Laatzien(Delayms);  MFUNF; Laatzien(Delayms);    
 VIERTEL; Laatzien(Delayms); ZWANZIG; Laatzien(Delayms); KURZ;   Laatzien(Delayms); VOR;   Laatzien(Delayms); NACH;  Laatzien(Delayms);  HALB;  Laatzien(Delayms);
 FUNF;  Laatzien(Delayms);    EINS;   Laatzien(Delayms); VIERDE; Laatzien(Delayms); ZEHN;  Laatzien(Delayms); ZWOLF; Laatzien(Delayms);  DREI;  Laatzien(Delayms);
 NEUN;  Laatzien(Delayms);    ACHTDE; Laatzien(Delayms); SECHS;  Laatzien(Delayms); SIEBEN;Laatzien(Delayms); ZWEI;  Laatzien(Delayms);  ELFDE; Laatzien(Delayms); 
 UHR;   Laatzien(Delayms); 
                      #endif  //DE144CLOCK                      
 Tekstprintln("*");      
//  Zelftest = false; 
//  Displaytime();                                                                               // Turn on the LEDs with proper time
}
//
//--------------------------------------------                                                //
// CLOCK Say the time and load the LEDs 
// with the proper colour and intensity
//--------------------------------------------
void Displaytime()
{ 
 LedsOff();                                                                                  // Start by clearing the display to a known state   
 if( Mem.DisplayChoice == DIGITAL ) { TimePlaceDigit(timeinfo.tm_hour,timeinfo.tm_min); }
 else                                                                                        // If not a digital display 
   {
                     #if defined NL144CLOCK || defined NL92CLOCK
    Dutch();                                                                                 //     
                     #endif  //NL144CLOCK || defined NL92CLOCK
                     #ifdef DE144CLOCK
    German();                                                                                 //   
                     #endif  //DE144CLOCK
                     #if defined NLM1M2M3M4
    Dutch();                                                                                 //  
                     #endif  //NLM1M2M3M4
                     #ifdef FOURLANGUAGECLOCK
    SetColours();                                                                            // Set the colours for the chosen palette   
    Dutch();
    English();
    German();
    French();
                     #endif  //FOURLANGUAGECLOCK       
   }  
 ShowLeds();                                                                                  // And turn on the LEDs
}
//--------------------------------------------                                                //
// CLOCK Dim the leds measured by the LDR and print values
// LDR reading are between 0 and 255. The Brightness send to the LEDs is between 0 and 255
//--------------------------------------------
void DimLeds(bool print) 
{ 
  int LDRread = ReadLDR();                                                                    // ESP32 analoge read is between 0 - 4096, reduce it to 0-1024                                                                                                   
  int LDRavgread = (4 * Previous_LDR_read + LDRread ) / 5;                                    // Read lightsensor and avoid rapid light intensity changes
  Previous_LDR_read = LDRavgread;                                                             // by using the previous reads
  OutPhotocell = (uint32_t)((Mem.LightReducer * sqrt(255*LDRavgread))/100);                   // Linear --> hyperbolic with sqrt. Result is between 0-255
  MinPhotocell = min(MinPhotocell, LDRavgread);                                               // Lowest LDR measurement
  MaxPhotocell = max(MaxPhotocell, LDRavgread);                                               // Highest LDR measurement
  OutPhotocell = constrain(OutPhotocell, Mem.LowerBrightness, Mem.UpperBrightness);           // Keep result between lower and upper boundery en calc percentage
  SumLDRreadshour += LDRavgread;    NoofLDRreadshour++;                                       // For statistics LDR readings per hour
  if(print)
  {
  // sprintf(sptext,"LDR:%3d Avg:%3d (%3d-%3d) Out:%3d=%2d%% Loop(%ld) ",
  //      LDRread,LDRavgread,MinPhotocell,MaxPhotocell,OutPhotocell,(int)(OutPhotocell/2.55),Loopcounter);    
 if (Mem.UseDS3231)   sprintf(sptext,"LDR:%3d=%2d%% %5d l/s %0.0fC ",
               LDRread,(int)(OutPhotocell*100/255),Loopcounter,RTCklok.getTemperature()); 
 else                 sprintf(sptext,"LDR:%3d=%2d%% %5d l/s ",
               LDRread,(int)(OutPhotocell*100/255),Loopcounter);   
   Tekstprint(sptext);
   PrintTimeHMS();    
  }
 if(LEDsAreOff) OutPhotocell = 0;
 SetBrightnessLeds(OutPhotocell);     // values between 0 and 255
}
//--------------------------------------------                                                //
// CLOCK Turn On en Off the LED's
//--------------------------------------------
void Play_Lights()
{
 SetBrightnessLeds(30);
 RainbowCycle(10);
 for(int i=0; i<NUM_LEDS; i++)  { ColorLeds("",i,i,chromeyellow); } 
 ShowLeds();
 delay(2000);
 LedsOff();
 WhiteOverRainbow(25, 50, 5 ); 
 WhiteOverRainbow(5, 5, 5 );  // wait, whiteSpeed, whiteLength
 LedsOff();
}    
                       #ifdef DE144CLOCK                                        
//--------------------------------------------
// CLOCK Blink UHR
//--------------------------------------------
void BlinkUUR(int NoofBlinks, int Delayms)
{
for (int n=0 ; n<=NoofBlinks; n++) { LedsOff(); Laatzien(Delayms);  UHR; Laatzien(Delayms); } 
}
//--------------------------------------------                                                //
// CLOCK Blink ES IST WAR
//--------------------------------------------
void BlinkHETISWAS (int NoofBlinks, int Delayms)
{ 
for (int n=0 ; n<=NoofBlinks; n++) { LedsOff(); Laatzien(Delayms);  ES; IST; WAR; Laatzien(Delayms); } 
}
//--------------------------------------------                                                //
// CLOCK Blink ZWOLF
//--------------------------------------------
void BlinkTWAALF(int NoofBlinks, int Delayms)
{
for (int n=0 ; n<=NoofBlinks; n++) { LedsOff(); Laatzien(Delayms); ZWOLF; Laatzien(Delayms); } 
}
                     #else
//--------------------------------------------                                                //
// CLOCK Blink UUR
//--------------------------------------------
void BlinkUUR(int NoofBlinks, int Delayms)
{
for (int n=0 ; n<=NoofBlinks; n++) { LedsOff(); Laatzien(Delayms);  UUR; Laatzien(Delayms); } 
}
//--------------------------------------------                                                //
// CLOCK Blink HET IS WAS
//--------------------------------------------
void BlinkHETISWAS (int NoofBlinks, int Delayms)
{
for (int n=0 ; n<=NoofBlinks; n++) { LedsOff(); Laatzien(Delayms);  HET; IS; WAS; Laatzien(Delayms); } 
}
//--------------------------------------------                                                //
// CLOCK Blink TWAALF
//--------------------------------------------
void BlinkTWAALF(int NoofBlinks, int Delayms)
{
for (int n=0 ; n<=NoofBlinks; n++) { LedsOff(); Laatzien(Delayms); TWAALF; Laatzien(Delayms); } 
}

                     #endif //DE144CLOCK

// --------------------Colour Clock Light functions -----------------------------------
//--------------------------------------------                                                //
// LED Set color for LEDs in strip and print tekst
//---------------------------------------------
void ColorLeds(char const *Tekst, int FirstLed, int LastLed, uint32_t RGBWColor)
{ 
 Stripfill(RGBWColor, FirstLed, ++LastLed - FirstLed );                                        //
 if (!NoTextInLeds && strlen(Tekst) > 0 )
     {sprintf(sptext,"%s ",Tekst); Tekstprint(sptext); }                                      // Print the text  
}
//--------------------------------------------
// LED Set color for one LED
//--------------------------------------------
void ColorLed(int Lednr, uint32_t RGBWColor)
{   
 Stripfill(RGBWColor, Lednr, 1 );
}
//--------------------------------------------                                                //
// LED Clear display settings of the LEDs
//--------------------------------------------
void LedsOff(void) 
{ 
 Stripfill(0, 0, NUM_LEDS );                                                                  // 
}
//--------------------------------------------                                                //
// LED Turn On and the LEDs off after Delaymsec milliseconds
//--------------------------------------------
void Laatzien(int Delaymsec) 
{ 
 ShowLeds(); 
 delay(Delaymsec);
 LedsOff(); 
 CheckDevices();                                                                              // Check for input from input devices
}

//--------------------------------------------                                                //
// LED Push data in LED strip to commit the changes
//--------------------------------------------
void ShowLeds(void)
{
//    if (xSemaphoreTake(ledMutex, portMAX_DELAY))                                           // A semaphore is not needed when using Neopixel
    { // Lock the mutex
      LEDstrip.show();
//    xSemaphoreGive(ledMutex); // Release the mutex
    }
}
//--------------------------------------------                                                //
// LED Set brighness of LEDs
//--------------------------------------------
void SetBrightnessLeds(byte Bright)
{
 LEDstrip.setBrightness(Bright);                                                              // Set brightness of LEDs   
 ShowLeds();
}
//--------------------------------------------
// LED Fill the strip array
//--------------------------------------------
void Stripfill(uint32_t RGBWColor, int FirstLed, int NoofLEDs)
{   
 LEDstrip.fill(RGBWColor, FirstLed, NoofLEDs);
}
//--------------------------------------------
// LED Strip Get Pixel Color 
//--------------------------------------------
uint32_t StripGetPixelColor(int Lednr)
{
return(LEDstrip.getPixelColor(Lednr));
}
//--------------------------------------------                                                //
// LED Synchronize the colour of the LEDstrip with the Status LED
// Used during Setup()
//--------------------------------------------
void LEDstartup(uint32_t LEDColour)
{
 static uint32_t ProgressLedNr = 0;
 ColorLed(ProgressLedNr++,LEDColour); 
 ShowLeds();   
 SetStatusLED(Cred(LEDColour),Cgreen(LEDColour),Cblue(LEDColour));  
}

//--------------------------------------------                                                //
// LED convert HSV to RGB  h is from 0-360, s,v values are 0-1
// r,g,b values are 0-255
//--------------------------------------------
/**
 * @brief Convert HSV values to packed RGBW format (white = 0).
 * @param H Hue angle (0–360)
 * @param S Saturation (0–1)
 * @param V Value/Brightness (0–1)
 * @return Packed RGBW uint32_t value
 */
uint32_t HSVToRGB(double H, double S, double V) 
{
 int i;
 double r, g, b, f, p, q, t;
 if (S == 0)  {r = V;  g = V;  b = V; }
 else
  {
   H >= 360 ? H = 0 : H /= 60;
   i = (int) H;
   f = H - i;
   p = V * (1.0 -  S);
   q = V * (1.0 - (S * f));
   t = V * (1.0 - (S * (1.0 - f)));
   switch (i) 
    {
     case 0:  r = V;  g = t;  b = p;  break;
     case 1:  r = q;  g = V;  b = p;  break;
     case 2:  r = p;  g = V;  b = t;  break;
     case 3:  r = p;  g = q;  b = V;  break;
     case 4:  r = t;  g = p;  b = V;  break;
     default: r = V;  g = p;  b = q;  break;
    }
  }
return FuncCRGBW((int)(r*255), (int)(g*255), (int)(b*255), 0 );                                // R, G, B, W 
}
//--------------------------------------------                                                //
// LED function to make RGBW colour
//--------------------------------------------
uint32_t FuncCRGBW( uint32_t Red, uint32_t Green, uint32_t Blue, uint32_t White)
{ 
 return ( (White<<24) + (Red << 16) + (Green << 8) + Blue );
}
//--------------------------------------------                                                //
// LED functions to extract RGBW colours
//--------------------------------------------
 uint8_t Cwhite(uint32_t c) { return (c >> 24);}
 uint8_t Cred(  uint32_t c) { return (c >> 16);}
 uint8_t Cgreen(uint32_t c) { return (c >> 8); }
 uint8_t Cblue( uint32_t c) { return (c);      }


//--------------------------------------------                                                //
//  DISPLAY Clear the display
//--------------------------------------------
void ClearScreen(void)
{
 LedsOff();
}
//--------------------------------------------                                                //
// DISPLAY Wheel The colours are a transition r - g - b - back to r
// Input a value 0 to 255 to get a color value.
//--------------------------------------------
uint32_t Wheel(byte WheelPos) 
{
 WheelPos = 255 - WheelPos;
 if(WheelPos < 85)   { return FuncCRGBW( 255 - WheelPos * 3, 0, WheelPos * 3, 0);  }
 if(WheelPos < 170)  { WheelPos -= 85;  return FuncCRGBW( 0,  WheelPos * 3, 255 - WheelPos * 3, 0); }
 WheelPos -= 170;      
 return FuncCRGBW(WheelPos * 3, 255 - WheelPos * 3, 0, 0);
}

//--------------------------------------------                                                //
// DISPLAY Initialyse the LEDstrip for WS2812 or SK6812 LEDs
//--------------------------------------------
void StartLeds(void) 
{
 switch (Mem.LEDstrip)
    {
      case 0: LEDstrip = LED6812strip; 
              white  = 0xFF000000; 
              lgray  = 0x66000000;  
              gray   = 0x33000000;                                                            // The SK6812 LED has a white LED that is pure white
              dgray  = 0x22000000;
              wgray  = 0xAA000000; 

      break;
      case 1: LEDstrip = LED2812strip; 
              white  = 0xFFFFFF;
              lgray  = 0x666666;                                                              // R, G and B on together gives white light
              gray   = 0x333333;
              dgray  = 0x222222;
              wgray  = 0xAAAAAA;         
      break;
     default: LEDstrip = LED6812strip;
              white  = 0xFF000000; 
              lgray  = 0x66000000;  
              gray   = 0x33000000;                                                            // The SK6812 LED has a white LED that is pure white
              dgray  = 0x22000000; 
              wgray  = 0xAA000000;     
    }
sprintf(sptext,"LED strip is %s", Mem.LEDstrip?"WS2812":"SK6812" ); Tekstprintln(sptext);

//ledMutex = xSemaphoreCreateMutex();

LEDstrip.begin();
LEDstrip.setBrightness(16);  
LedsOff();                                                                // Set initial brightness of LEDs  (0-255)  
ShowLeds();
// for(int i=0; i<10; i++)  ColorLeds("",i,i,Wheel(i * 25));
// ShowLeds();
// int blinktime = 200; 
// BlinkUUR(3, blinktime);                         // Change the hours
// BlinkHETISWAS(3, blinktime);                     // Change the hours        
// BlinkTWAALF(3, blinktime);                      // Turn on TWAALF and change intensity 
}
//--------------------------------------------                                                //
// DISPLAY change display choice
//--------------------------------------------
void ChangeRandomDisplay(void)
{
if (Mem.RandomDisplay)
  {
   Mem.DisplayChoice += 1;
   if (Mem.DisplayChoice == 6)       Mem.DisplayChoice = 7;                                  // Do not display the digital clock
   if (Mem.DisplayChoice > LASTITEM) Mem.DisplayChoice = 0;                                  // 
   PrintDisplayChoice();         
  }
}
//--------------------------------------------                                                //
//  DISPLAY RainbowCycle
//--------------------------------------------
void RainbowCycle(uint8_t wait) 
{
 uint16_t i, j;
 for(j=0; j<256 * 5; j++) 
   {                                                                                          // 5 cycles of all colors on wheel
    for(i=0; i< NUM_LEDS; i++) ColorLeds("",i,i,Wheel(((i * 256 / NUM_LEDS) + j) & 255));
    ShowLeds();
    delay(wait);
  }
}

//--------------------------------------------                                                //
// DISPLAY WhiteOverRainbow
// wait Delay between frames (in ms)
// whiteSpeed Speed at which white segment moves
// whiteLength Length of the white segment
//--------------------------------------------
void WhiteOverRainbow(uint32_t wait, uint8_t whiteSpeed, uint32_t whiteLength ) 
{
 if(whiteLength >= NUM_LEDS) whiteLength = NUM_LEDS - 1;
 uint32_t head = whiteLength - 1;
 uint32_t tail = 0;
 uint32_t loopNum = 0;
 static unsigned long lastTime = 0;
 for(uint32_t j=0; j<256; j++) 
    {
     for(uint32_t i=0; i<=NUM_LEDS; i++) 
       {
        if((i >= tail && i <= head) || (tail > head && i >= tail) || (tail > head && i <= head) )
              ColorLeds("",i,i,0XFFFFFF );
        else  
              ColorLeds("",i,i,Wheel(((i * 256 / NUM_LEDS) + j) & 255));
       }
      if(millis() - lastTime > whiteSpeed) 
       {
        head++;        tail++;
        if(head == NUM_LEDS) loopNum++;
        lastTime = millis();
      }
      head %= NUM_LEDS;
      tail %= NUM_LEDS;
      ShowLeds();
     delay(wait);
    }
}

//--------------------------------------------                                                //
// DISPLAY Place digits 0 - 9 in Matrix display
// First row and column = 0, PosX,PosY is left top position of 3x5 digit
// Calculate position LED #define MATRIX_WIDTH 12 #define MATRIX_HEIGHT 12
//--------------------------------------------
void Zet_Pixel(byte Cijfer,byte Pos_X, byte Pos_Y) 
{ 
 uint32_t LEDnum;
 //uint32_t OrgColor;
 for(int i=0;i<3;i++)
  {  
   for(int j=0;j<5;j++)
   {
    int c = pgm_read_byte_near ( &Getal[Cijfer][i][j]);                                       // Reading a byte of data stored in program memory (PROGMEM area) at a specified address.
    if ( c )                                                                                  // if Digit == 1 then turn that light on
     {                                                                                        // Serial.print(strip.getPixelColor(LEDnum) & 0X00FFFFFF,HEX); Serial.print(" ");
      if((Pos_Y+j)%2) LEDnum =((MATRIX_WIDTH-1) - (Pos_X + i) + (Pos_Y + j) * (MATRIX_HEIGHT));
      else            LEDnum =                    (Pos_X + i) + (Pos_Y + j) * (MATRIX_HEIGHT); 
      ColorLed(LEDnum, (uint32_t)(LetterColor));
     // ColorLed(LEDnum, (uint32_t)(Mem.OwnColour));
     // StripGetPixelColor(LEDnum) && Mem.OwnColour ? OrgColor = LetterColor : OrgColor = 0;
    //  ColorLed(LEDnum, (uint32_t)(OrgColor + Mem.OwnColour));
     }
   }
  }
}
//--------------------------------------------                                                //
// DISPLAY Time in four digits in display
//--------------------------------------------
void TimePlaceDigit(byte uur, byte minuut)
{   
 Zet_Pixel(    uur / 10, 2, 1);  Zet_Pixel(    uur % 10, 7, 1);
 Zet_Pixel( minuut / 10, 2, 7);  Zet_Pixel( minuut % 10, 7, 7);
}

//--------------------------------------------                                                //
// CLOCK In- or decrease light intensity value i.e. Slope
//--------------------------------------------
void WriteLightReducer(int amount)
{
 int value = Mem.LightReducer + amount;                                                       // Prevent byte overflow by making it an integer before adding
 Mem.LightReducer = (byte) constrain(value,5, 250);                                           // Between 5 and 250
 sprintf(sptext,"Max brightness: %3d%%",Mem.LightReducer);
 Tekstprintln(sptext);
}
//--------------------------- Time functions --------------------------
 time_t now;
//--------------------------------------------------------------------
// Initialize time system (DS3231 + NTP + internal RTC)
//--------------------------------------------------------------------
void InitTimeSystem(void)
{
  DS3231Installed = IsDS3231I2Cconnected();
  sprintf(sptext, "External RTC module %s found", DS3231Installed ? "IS" : "NOT");
  Tekstprintln(sptext);
  if (DS3231Installed) RTCklok.begin();
  if (DS3231Installed && Mem.UseDS3231)
  {
    GetDS3231Time(false);
    SetSystemTime(mktime(&timeinfo));                    // Sync ESP32 RTC from DS3231
  }
  else if (Mem.NTPOn && WiFi.isConnected())
  {
    GetNTPtime(false);
    SetSystemTime(mktime(&timeinfo));                    // Sync ESP32 RTC from NTP
    if (DS3231Installed && Mem.UseDS3231) SetDS3231Time(); // Sync DS3231
  }
  else
  {
    time(&now);
    localtime_r(&now, &timeinfo);
  }
}

//--------------------------------------------------------------------
// Get current time from best available source
//--------------------------------------------------------------------
time_t GetTijd(bool printit)
{
  if (Mem.UseDS3231 && DS3231Installed)     { GetDS3231Time(false);  }
  else if (Mem.NTPOn && WiFi.isConnected()) { getLocalTime(&timeinfo);  }
  else  {    time(&now);                      localtime_r(&now, &timeinfo);  }
  time_t t = mktime(&timeinfo);
  if (printit) PrintRTCTime();
  return t;
}

//--------------------------------------------------------------------
// NTP Return local time as RTClib DateTime
//--------------------------------------------------------------------
DateTime GetLocalDateTime()
{
  time_t t = GetTijd(false);
  return DateTime(t);
}

//--------------------------------------------------------------------
// NTP print the NTP time for the timezone set 
//--------------------------------------------------------------------
void GetNTPtime(bool printit)
{
  sntp_set_sync_mode(SNTP_SYNC_MODE_IMMED);  // Immediate sync mode
  sntp_restart();                            // Triggers a fresh sync


//  wait4SNTP();
  getLocalTime(&timeinfo);
  if (printit) PrintNTPtime();
}

//--------------------------------------------------------------------
// NTP Print current local (NTP) time
//--------------------------------------------------------------------
void PrintNTPtime(void)
{
  getLocalTime(&timeinfo);
  PrintRTCTime();
}

//--------------------------------------------------------------------
// NTP print the NTP UTC time 
//--------------------------------------------------------------------
void PrintUTCtime(void)
{
  time(&now);
  struct tm* UTCtime = gmtime(&now);
  sprintf(sptext, "UTC: %02d:%02d:%02d %02d-%02d-%04d",
          UTCtime->tm_hour, UTCtime->tm_min, UTCtime->tm_sec,
          UTCtime->tm_mday, UTCtime->tm_mon + 1, UTCtime->tm_year + 1900);
  Tekstprint(sptext);
}

// //--------------------------------------------                                                //
// // DS3231 Init module
// //--------------------------------------------
// void InitDS3231Mod(void)
// {
//  DS3231Installed = IsDS3231I2Cconnected();                                                    // Check if DS3231 is connected and working   
//  sprintf(sptext,"External RTC module %s found", DS3231Installed?"IS":"NOT");
//  RTCklok.begin();     
//  Tekstprintln(sptext);                                                             
// }
//--------------------------------------------------------------------
// DS3231 check for I2C connection
// DS3231_I2C_ADDRESS (= often 0X68) = DS3231 module
//--------------------------------------------------------------------
bool IsDS3231I2Cconnected(void)
{
  for (byte i = 1; i < 120; i++)
  {
    Wire.beginTransmission(i);
    if (Wire.endTransmission() == 0 && i == DS3231_I2C_ADDRESS)
      return true;
  }
  return false;
}

//--------------------------------------------------------------------
// DS3231 temperature
//--------------------------------------------------------------------
float GetDS3231Temp(void)
{
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0x11);
  Wire.endTransmission();
  Wire.requestFrom(DS3231_I2C_ADDRESS, 2);

  if (Wire.available())
  {
    byte tMSB = Wire.read();
    byte tLSB = Wire.read();
    return (tMSB & 0b01111111) + ((tLSB >> 6) * 0.25);
  }
  return -273.0;
}

//--------------------------------------------------------------------
// DS3231 RTC ESP32 -> DS3231
// Set time in module DS3231
//--------------------------------------------------------------------
void SetDS3231Time(void)
{
  DateTime tNow(timeinfo.tm_year + 1900, timeinfo.tm_mon + 1, timeinfo.tm_mday,
                timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
  RTCklok.adjust(tNow);
  Tekstprintln("Time set in DS3231 RTC module");
  PrintDS3231Time();
  if(!Mem.UseDS3231 && !Mem.NTPOn ) Tekstprintln("*** Not using DS3231");
}

//--------------------------------------------------------------------
// DS3231 reads time in module DS3231
// and store it in Word clock time structure
//--------------------------------------------------------------------
void GetDS3231Time(bool printit)
{
  DateTime Inow = RTCklok.now();
  timeinfo.tm_year = Inow.year() - 1900;
  timeinfo.tm_mon  = Inow.month() - 1;
  timeinfo.tm_mday = Inow.day();
  timeinfo.tm_hour = Inow.hour();
  timeinfo.tm_min  = Inow.minute();
  timeinfo.tm_sec  = Inow.second();
  if (printit) PrintDS3231Time();
}

//--------------------------------------------------------------------
// DS3231 Print DS3231 time
//--------------------------------------------------------------------
void PrintDS3231Time(void)
{
 DateTime Inow = RTCklok.now();
 sprintf(sptext, "%02d/%02d/%04d %02d:%02d:%02d ",
          Inow.day(), Inow.month(), Inow.year(),
          Inow.hour(), Inow.minute(), Inow.second());
 Tekstprintln(sptext);
}

//--------------------------------------------                                                //
// RTC prints the ESP32 internal RTC time to serial
//--------------------------------------------
void PrintRTCTime(void)
{
 sprintf(sptext,"%02d/%02d/%04d %02d:%02d:%02d ", 
     timeinfo.tm_mday,timeinfo.tm_mon+1,timeinfo.tm_year+1900,
     timeinfo.tm_hour,timeinfo.tm_min,timeinfo.tm_sec);
 WTekstprint(sptext,"<!--"," -->");                                                         // Hide the time on the HTML page <!-- This text won't appear at all -->
 Tekstprintln("");
}

//--------------------------------------------                                                //
// RTC Fill sptext with time
//--------------------------------------------
void PrintTimeHMS(){ PrintTimeHMS(2);}                                                        // print with linefeed
void PrintTimeHMS(byte format)
{
 sprintf(sptext,"%02d:%02d:%02d",timeinfo.tm_hour,timeinfo.tm_min,timeinfo.tm_sec);
 switch (format)
 {
  case 0: break;
  case 1: Tekstprint(sptext); break;
  case 2: Tekstprintln(sptext); break;  
 }
}

//--------------------------------------------                                                //
// RTC Set time using global timeinfo struct
//--------------------------------------------
void SetRTCTime(void) 
{ 
 time_t t = mktime(&timeinfo);                                                                // t= unixtime
 SetSystemTime(t);
}

//--------------------------------------------                                                //
// RTC Set RTC time using Unix timestamp
//--------------------------------------------
void SetSystemTime(time_t t)
{ 
  struct timeval tv = { .tv_sec = t, .tv_usec = 0 };
  settimeofday(&tv, nullptr);
  localtime_r(&t, &timeinfo);
  Tekstprint("RTC time set: ");
  PrintTimeHMS();
  Displaytime(); 
}

//--------------------------------------------                                                //
// CLOCK Print all the times available 
//--------------------------------------------
void PrintAllClockTimes(void)
{
 Tekstprint("\n Clock time: ");
 PrintRTCTime();
 if(WiFi.localIP()[0] != 0)                                                                   // If no WIFI then no NTP time printed
   {
    Tekstprint("   NTP time: ");
    PrintNTPtime();
   }
 if(DS3231Installed)
   {
    Tekstprint("DS3231 time: ");
    PrintDS3231Time();
   }
}

//--------------------------- End Time functions --------------------------

//--------------------------------------------                                                //
//  CLOCK Convert Hex to uint32
//--------------------------------------------
uint32_t HexToDec(const String& hexString) {
    uint32_t decValue = 0;

    for (size_t i = 0; i < hexString.length(); i++) 
       {
        char c = hexString.charAt(i);
        uint8_t value = 0;
        if      (c >= '0' && c <= '9') { value = c - '0';      } 
        else if (c >= 'A' && c <= 'F') { value = c - 'A' + 10; } 
        else if (c >= 'a' && c <= 'f') { value = c - 'a' + 10; } 
        else { continue;                                                                      // Skip invalid characters
        }
        decValue = (decValue << 4) | value;                                                   // Multiply by 16 and add value
    }
    return decValue;
}
//------------------------------------------------------------------------------              //
// CLOCK Demo mode
//------------------------------------------------------------------------------
void Demomode(void)
{
 if ( millis() - msTick == 10)  SetNanoLED13(0);                                              // Turn OFF the second on pin 13
 if ( millis() - msTick >= MilliSecondValue)                                                  // Flash the onboard Pin 13 Led so we know something is happening
 {    
  msTick = millis();                                                                          // second++; 
  SetNanoLED13(100);                                                                          // Turn ON the second on pin 13
  if( ++timeinfo.tm_sec >59) { timeinfo.tm_sec = 0; }
  if( ++timeinfo.tm_min >59) { timeinfo.tm_min = 0; timeinfo.tm_sec = 0; timeinfo.tm_hour++;}
  if( timeinfo.tm_hour >24) timeinfo.tm_hour = 0;                                             // If hour is after 12 o'clock 
  DimLeds(false);
  SetSecondColour();                                                                          // Set the colour per second of 'IS' and 'WAS' 
  Displaytime();
  Tekstprintln("");
  SerialCheck();
 }
}
//------------------------------------------------------------------------------              //
// CLOCK Fire display on MATRIX_HEIGHT MATRIX_WIDTH
//------------------------------------------------------------------------------
void FireMode(void)
{
  Fire2023_2D();
  SerialCheck();
}
//--------------------------------------------                                                //
// FIRE
//--------------------------------------------
// ==== FIRE EFFECT SETTINGS ====
#define COOLING     90
#define SPARKING    40
#define FIRE_DELAY  50

uint8_t heat[MATRIX_HEIGHT][MATRIX_WIDTH];  // 2D heat map
//--------------------------------------------                                                //
// FIRE Utils
//--------------------------------------------
uint8_t clamp_sub(uint8_t val, uint8_t sub) { return (val > sub) ? (val - sub) : 0;}
uint8_t clamp_add(uint8_t val, uint8_t add) { return (val + add > 255) ? 255 : (val + add);}
void HeatColor(uint8_t temperature, uint8_t &r, uint8_t &g, uint8_t &b) 
{
 uint8_t t192 = (temperature * 192) / 255;
 uint8_t heatramp = (t192 & 0x3F) << 2;
 if      (t192 > 128) { r = 255;      g = 255;      b = heatramp; } 
 else if (t192 > 64)  { r = 255;      g = heatramp; b = 0;        } 
 else                 { r = heatramp; g = 0;        b = 0;        }
}

uint16_t XY(uint8_t x, uint8_t y) 
{  return (y % 2 == 0) ? (y * MATRIX_WIDTH + x) : (y * MATRIX_WIDTH + (MATRIX_WIDTH-1-x)); }  // Serpentine index mapping

//--------------------------------------------                                                //
// FIRE ==== Fire Logic ====
//--------------------------------------------
void Fire2023_2D() 
{
 for (uint8_t x = 0; x < MATRIX_WIDTH; x++)                                                   // Step 1: Cool down
     for (uint8_t y = 0; y < MATRIX_HEIGHT; y++) 
         heat[y][x] = clamp_sub(heat[y][x], random(0, ((COOLING*10) / MATRIX_HEIGHT) + 2));
 for (uint8_t x = 0; x < MATRIX_WIDTH; x++)                                                   // Step 2: Heat falls down
   {                                                 
    for (int y = 0; y < MATRIX_HEIGHT - 2; y++) 
        heat[y][x] = (heat[y + 1][x] + heat[y + 2][x] + heat[y + 2][x]) / 3;
    heat[MATRIX_HEIGHT - 2][x]=(heat[MATRIX_HEIGHT - 1][x] + heat[MATRIX_HEIGHT - 1][x]) / 2; // Safe heat fall for second-to-top row
   }
 for (uint8_t x = 0; x < MATRIX_WIDTH; x++)                                                   // Step 3: Sparking at the top
    if (random(255) < SPARKING)  
       heat[MATRIX_HEIGHT - 1][x] = clamp_add(heat[MATRIX_HEIGHT - 1][x], random(160, 255));
 for (uint8_t y = 0; y < MATRIX_HEIGHT; y++)                                                  // Step 4: Draw pixels from heat map
   {
    for (uint8_t x = 0; x < MATRIX_WIDTH; x++) 
      {
       uint8_t r, g, b;
       HeatColor(heat[y][x], r, g, b);
       LEDstrip.setPixelColor(XY(x, y), r, g, b, 0);                                          // Add W=0
      }
    }   
 LEDstrip.show();
 delay(FIRE_DELAY);
}
//--------------------------------------------                                                //
// CLOCK Dutch clock display
//--------------------------------------------
                      #if defined(NL) || defined(NL144CLOCK)  || defined (NL92CLOCK)
void Dutch(void)
{
//if(Mem.EdSoftLEDSOn) EDSOFT;                                                                // EDSOFT On or Off
HET; 
                      #if defined(NL) || defined(NL144CLOCK)                                                                                          // HET  is always on
 if (timeinfo.tm_hour == 12 && timeinfo.tm_min == 0 && random(2)==0) { IS; NOEN; return; }
 if (timeinfo.tm_hour == 00 && timeinfo.tm_min == 0 && random(2)==0) { IS; MIDDER; NACHT; return; }
                      #endif //#if defined(NL) || defined(NL144CLOCK)  
switch (timeinfo.tm_min)
 {
  case  0: IS;  PRECIES; break;
  case  1: IS;  break;
  case  2: 
  case  3: WAS; break;
  case  4: 
  case  5: 
  case  6: IS;  MVIJF; OVER; break;
  case  7: 
  case  8: WAS; MVIJF; OVER; break;
  case  9: 
  case 10: 
  case 11: IS;  MTIEN; OVER; break;
  case 12: 
  case 13: WAS; MTIEN; OVER; break;
  case 14: 
  case 15: 
  case 16: IS;  KWART; OVER; break;
  case 17: 
  case 18: WAS; KWART; OVER; break;
  case 19: 
  case 20: 
  case 21: IS;  MTIEN; VOOR; HALF; break;
  case 22: 
  case 23: WAS; MTIEN; VOOR; HALF; break;
  case 24: 
  case 25: 
  case 26: IS;  MVIJF; VOOR; HALF; break;
  case 27: 
  case 28: WAS; MVIJF; VOOR; HALF; break;
  case 29: IS;  HALF; break;
  case 30: IS;  PRECIES; HALF; break;
  case 31: IS;  HALF; break;
  case 32: 
  case 33: WAS; HALF; break;
  case 34: 
  case 35: 
  case 36: IS;  MVIJF; OVER; HALF; break;
  case 37: 
  case 38: WAS; MVIJF; OVER; HALF; break;
  case 39: 
  case 40: 
  case 41: IS;  MTIEN; OVER; HALF; break;
  case 42: 
  case 43: WAS; MTIEN; OVER; HALF; break;
  case 44: 
  case 45: 
  case 46: IS;  KWART; VOOR; break;
  case 47: 
  case 48: WAS; KWART; VOOR; break;
  case 49: 
  case 50: 
  case 51: IS;  MTIEN; VOOR;  break;
  case 52: 
  case 53: WAS; MTIEN; VOOR;  break;
  case 54: 
  case 55: 
  case 56: IS;  MVIJF; VOOR; break;
  case 57: 
  case 58: WAS; MVIJF; VOOR; break;
  case 59: IS;  break;
}
//if (timeinfo.tm_hour >=0 && hour <12) digitalWrite(AMPMpin,0); else digitalWrite(AMPMpin,1);

 sayhour = timeinfo.tm_hour;
 if (timeinfo.tm_min > 18 )  sayhour = timeinfo.tm_hour+1;
 if (sayhour == 24) sayhour = 0;

switch (sayhour)
 {
  case 13:  
  case 1: EEN; break;
  case 14:
  case 2: TWEE; break;
  case 15:
  case 3: DRIE; break;
  case 16:
  case 4: VIER; break;
  case 17:
  case 5: VIJF; break;
  case 18:
  case 6: ZES; break;
  case 19:
  case 7: ZEVEN; break;
  case 20:
  case 8: ACHT; break;
  case 21:
  case 9: NEGEN; break;
  case 22:
  case 10: TIEN; break;
  case 23:
  case 11: ELF; break;
  case 0:
  case 12: TWAALF; break;
 } 
 switch (timeinfo.tm_min)
 {
  case 59: 
  case  0: 
  case  1: 
  case  2: 
  case  3: UUR;  break; 
 }
//                        #if defined(NL) || defined(NL144CLOCK) 
//   switch (timeinfo.tm_min % 5)                                                                // The minutes modulo 5
//  {
//   case  0: break;
//   case  1: MIN1; break;
//   case  2: MIN1; MIN2; break;
//   case  3: MIN1; MIN2; MIN3; break;
//   case  4: MIN1; MIN2; MIN3; MIN4;  
//  }  
//                       #endif // defined(NL) || defined(NL144CLOCK) 
}

                                    #endif //#if defined(NL) || defined(NL144CLOCK)  || defined (NL92CLOCK)

                                    #if defined NLM1M2M3M4
void Dutch(void)
{
if(Mem.EdSoftLEDSOn) EDSOFT;     
HET;                                                                                          // HET  is always on
switch (timeinfo.tm_min)
 {
  case  0: 
  case  1:  
  case  2: 
  case  3: 
  case  4: IS;  break;                                                                        // HET IS .. UUR
  case  5: 
  case  6: 
  case  7: 
  case  8: 
  case  9: IS;  MVIJF; OVER; break;                                                           // HET IS VIJF OVER .. 
  case 10: 
  case 11: 
  case 12: 
  case 13: 
  case 14: IS;  MTIEN; OVER; break;                                                           // Et cetera
  case 15: 
  case 16: 
  case 17: 
  case 18: 
  case 19: IS;  KWART; OVER2; break;
  case 20: 
  case 21: 
  case 22: 
  case 23: 
  case 24: IS;  MTIEN; MVOOR; HALF; break;
  case 25: 
  case 26: 
  case 27: 
  case 28:
  case 29: IS;  MVIJF; MVOOR; HALF; break;
  case 30:
  case 31:
  case 32:
  case 33:
  case 34: IS;  HALF; break;
  case 35: 
  case 36: 
  case 37:
  case 38:
  case 39: IS;  MVIJF; OVER; HALF; break;
  case 40: 
  case 41:
  case 42:
  case 43:
  case 44: IS;  MTIEN; OVER; HALF; break;
  case 45: 
  case 46: 
  case 47:
  case 48:
  case 49: IS;  KWART; VOOR; break;
  case 50:
  case 51:
  case 52:
  case 53:
  case 54: IS;  MTIEN; VOOR;  break;
  case 55:
  case 56: 
  case 57: 
  case 58:
  case 59: IS;  MVIJF; VOOR; break;
}
//if (timeinfo.tm_hour >=0 && hour <12) digitalWrite(AMPMpin,0); else digitalWrite(AMPMpin,1);

 sayhour = timeinfo.tm_hour;
 if (timeinfo.tm_min > 19 )  sayhour = timeinfo.tm_hour+1;
 if (sayhour == 24) sayhour = 0;

switch (sayhour)
 {
  case 13:  
  case 1: EEN; break;
  case 14:
  case 2: TWEE; break;
  case 15:
  case 3: DRIE; break;
  case 16:
  case 4: VIER; break;
  case 17:
  case 5: VIJF; break;
  case 18:
  case 6: ZES; break;
  case 19:
  case 7: ZEVEN; break;
  case 20:
  case 8: ACHT; break;
  case 21:
  case 9: NEGEN; break;
  case 22:
  case 10: TIEN; break;
  case 23:
  case 11: ELF; break;
  case 0:
  case 12: TWAALF; break;
 } 
 switch (timeinfo.tm_min)
 {
//  case 59: 
  case  0: 
  case  1: 
  case  2: 
  case  3: 
  case  4: UUR;  break; 
 }

 switch (timeinfo.tm_min % 5)                                                                // The minutes modulo 5
 {
  case  0: break;
  case  1: MIN1; break;
  case  2: MIN1; MIN2; break;
  case  3: MIN1; MIN2; MIN3; break;
  case  4: MIN1; MIN2; MIN3; MIN4;  
 }
}
                     #endif  //NLM1M2M3M4

                     #ifdef UK
//--------------------------------------------                                                //
//  CLOCK English clock display
//--------------------------------------------
void English(void)
{
 IT;                                                                                          // IT is always on
// if (timeinfo.tm_hour == 00 && timeinfo.tm_min == 0 && random(2)==0) { ISUK; MID; NIGHT; return; } 
 switch (timeinfo.tm_min)
 {
  case  0: ISUK;  EXACTUK; break;
  case  1: ISUK;  break;
  case  2: 
  case  3: WASUK; break;
  case  4: 
  case  5: 
  case  6: ISUK;  MFIVE; PAST; break;
  case  7: 
  case  8: WASUK; MFIVE; PAST; break;
  case  9: 
  case 10: 
  case 11: ISUK;  MTEN; PAST; break;
  case 12: 
  case 13: WASUK; MTEN; PAST; break;
  case 14: 
  case 15: 
  case 16: ISUK;  QUARTER; PAST; break;
  case 17: 
  case 18: WASUK; QUARTER; PAST; break;
  case 19: 
  case 20: 
  case 21: ISUK;  TWENTY; PAST; break;
  case 22: 
  case 23: WASUK; TWENTY; PAST; break;
  case 24: 
  case 25: 
  case 26: ISUK;  TWENTY; MFIVE; PAST; break;
  case 27: 
  case 28: WASUK; TWENTY; MFIVE; PAST; break;
  case 29: ISUK;  HALFUK; PAST; break;
  case 30: ISUK;  EXACTUK; HALFUK; PAST; break;
  case 31: ISUK;  HALFUK; PAST; break;
  case 32: 
  case 33: WASUK; HALFUK; PAST; break;
  case 34: 
  case 35: 
  case 36: ISUK;  TWENTY; MFIVE; TO; break;
  case 37: 
  case 38: WASUK; TWENTY; MFIVE; TO; break;
  case 39: 
  case 40: 
  case 41: ISUK;  TWENTY; TO; break;
  case 42: 
  case 43: WASUK; TWENTY; TO break;
  case 44: 
  case 45: 
  case 46: ISUK;  QUARTER; TO; break;
  case 47: 
  case 48: WASUK; QUARTER; TO; break;
  case 49: 
  case 50: 
  case 51: ISUK;  MTEN; TO;  break;
  case 52: 
  case 53: WASUK; MTEN; TO;  break;
  case 54: 
  case 55: 
  case 56: ISUK;  MFIVE; TO; break;
  case 57: 
  case 58: WASUK; MFIVE; TO; break;
  case 59: ISUK;  break;
}
//if (timeinfo.tm_hour >=0 && hour <12) digitalWrite(AMPMpin,0); else digitalWrite(AMPMpin,1);

 sayhour = timeinfo.tm_hour;
 if (timeinfo.tm_min > 33 ) sayhour = timeinfo.tm_hour+1;
 if (sayhour == 24) sayhour = 0;

switch (sayhour)
 {
  case 13:  
  case 1:  ONE; break;
  case 14:
  case 2:  TWO; break;
  case 15:
  case 3:  THREE; break;
  case 16:
  case 4:  FOUR; break;
  case 17:
  case 5:  FIVE; break;
  case 18:
  case 6:  SIXUK; break;
  case 19:
  case 7:  SEVEN; break;
  case 20:
  case 8:  EIGHT; break;
  case 21:
  case 9:  NINE; break;
  case 22:
  case 10: TEN; break;
  case 23:
  case 11: ELEVEN; break;
  case 0:
  case 12: TWELVE; break;
 } 
 switch (timeinfo.tm_min)
 {
  case 59: 
  case  0: 
  case  1: 
  case  2: 
  case  3: OCLOCK;  break; 
 }
}
                     #endif //UK
                     #if defined DE || defined DE144CLOCK
//--------------------------------------------                                                //
//  CLOCK German clock display
//--------------------------------------------
void German(void)
{
  ES;                                                                                         // ES is always on
// if (timeinfo.tm_hour == 00 && timeinfo.tm_min == 0 && random(2)==0) {IST; MITTER; NACHTDE; return; } 
 switch (timeinfo.tm_min)
 {
  case  0: IST;  GENAU; break;
  case  1: IST; KURZ; NACH; break;
  case  2: 
  case  3: WAR; break;
  case  4: 
  case  5: 
  case  6: IST; MFUNF; NACH; break;
  case  7: 
  case  8: WAR; MFUNF; NACH; break;
  case  9: 
  case 10: 
  case 11: IST; MZEHN; NACH; break;
  case 12: 
  case 13: WAR; MZEHN; NACH; break;
  case 14: 
  case 15: 
  case 16: IST; VIERTEL; NACH; break;
  case 17: 
  case 18: WAR; VIERTEL; NACH; break;
  case 19: 
  case 20: 
  case 21: IST; MZEHN; VOR; HALB; break;
  case 22: 
  case 23: WAR; MZEHN; VOR; HALB; break;
  case 24: 
  case 25: 
  case 26: IST; MFUNF; VOR; HALB; break;
  case 27: 
  case 28: WAR; MFUNF; VOR; HALB; break;
  case 29: IST; KURZ;  VOR; HALB; break;
  case 30: IST; GENAU; HALB; break;
  case 31: IST; KURZ;  NACH; HALB; break;
  case 32: 
  case 33: WAR; HALB; break;
  case 34: 
  case 35: 
  case 36: IST; MFUNF; NACH; HALB; break;
  case 37: 
  case 38: WAR; MFUNF; NACH; HALB; break;
  case 39: 
  case 40: 
  case 41: IST; MZEHN; NACH; HALB; break;
  case 42: 
  case 43: WAR; MZEHN; NACH; HALB; break;
  case 44: 
  case 45: 
  case 46: IST; VIERTEL; VOR; break;
  case 47: 
  case 48: WAR; VIERTEL; VOR; break;
  case 49: 
  case 50: 
  case 51: IST; MZEHN; VOR;  break;
  case 52: 
  case 53: WAR; MZEHN; VOR;  break;
  case 54: 
  case 55: 
  case 56: IST; MFUNF; VOR; break;
  case 57: 
  case 58: WAR; MFUNF; VOR; break;
  case 59: IST;  break;
}
//if (timeinfo.tm_hour >=0 && hour <12) digitalWrite(AMPMpin,0); else digitalWrite(AMPMpin,1);

 sayhour = timeinfo.tm_hour;
 if (timeinfo.tm_min > 18 ) sayhour = timeinfo.tm_hour+1;
 if (sayhour == 24) sayhour = 0;

switch (sayhour)
 {
  case 13:  
  case 1: EINS; break;
  case 14:
  case 2: ZWEI; break;
  case 15:
  case 3: DREI; break;
  case 16:
  case 4: VIERDE; break;
  case 17:
  case 5: FUNF; break;
  case 18:
  case 6: SECHS; break;
  case 19:
  case 7: SIEBEN; break;
  case 20:
  case 8: ACHTDE; break;
  case 21:
  case 9: NEUN; break;
  case 22:
  case 10: ZEHN; break;
  case 23:
  case 11: ELFDE; break;
  case 0:
  case 12: ZWOLF; break;
 } 
 switch (timeinfo.tm_min)
 {
  case 59: 
  case  0: 
  case  1: 
  case  2: 
  case  3: UHR;  break; 
 }
}
                     #endif //DE
                     #ifdef FR
//--------------------------------------------                                                //
//  CLOCK French clock display
//--------------------------------------------
void French(void)
{
 IL;                                                                                          // IL is always on
 switch (timeinfo.tm_min)
 {
  case  0: EST;   EXACT; DITLEHEURE; break;
  case  1: EST;   DITLEHEURE; break;
  case  2: 
  case  3: ETAIT; DITLEHEURE; break;
  case  4: 
  case  5: 
  case  6: EST;   DITLEHEURE; MCINQ; break;
  case  7: 
  case  8: ETAIT; DITLEHEURE; MCINQ; break;
  case  9: 
  case 10: 
  case 11: EST;   DITLEHEURE; MDIX;  break;
  case 12: 
  case 13: ETAIT; DITLEHEURE; MDIX;  break;
  case 14: 
  case 15: 
  case 16: EST;   DITLEHEURE; ET; QUART; break;
  case 17: 
  case 18: ETAIT; DITLEHEURE; ET; QUART; break;
  case 19: 
  case 20: 
  case 21: EST;   DITLEHEURE; VINGT; break;
  case 22: 
  case 23: ETAIT; DITLEHEURE; VINGT; break;
  case 24: 
  case 25: 
  case 26: EST;   DITLEHEURE; VINGT; MCINQ; break;
  case 27: 
  case 28: ETAIT; DITLEHEURE; VINGT; MCINQ; break;
  case 29: EST;   DITLEHEURE; ET; DEMI; break;
  case 30: EST;   EXACT; DITLEHEURE;  ET; DEMI; break;
  case 31: EST;   DITLEHEURE; ET; DEMI; break;
  case 32: 
  case 33: ETAIT; DITLEHEURE; ET; DEMI; break;
  case 34: 
  case 35: 
  case 36: EST;   DITLEHEURE; MOINS; VINGT; MCINQ; break;
  case 37: 
  case 38: ETAIT; DITLEHEURE; MOINS; VINGT; MCINQ; break;
  case 39: 
  case 40: 
  case 41: EST;   DITLEHEURE; MOINS; VINGT;  break;
  case 42: 
  case 43: ETAIT; DITLEHEURE; MOINS; VINGT;  break;
  case 44: 
  case 45: 
  case 46: EST;   DITLEHEURE; MOINS; LE; QUART; break;
  case 47: 
  case 48: ETAIT; DITLEHEURE; MOINS; LE; QUART; break;
  case 49: 
  case 50: 
  case 51: EST;   DITLEHEURE; MOINS; MDIX;   break;
  case 52: 
  case 53: ETAIT; DITLEHEURE; MOINS; MDIX;   break;
  case 54: 
  case 55: 
  case 56: EST;   DITLEHEURE; MOINS; MCINQ;  break;
  case 57: 
  case 58: ETAIT; DITLEHEURE; MOINS; MCINQ;  break;
  case 59: EST;   DITLEHEURE;  break;
 }
}

void DitLeHeure(void)
{
 byte sayhour = timeinfo.tm_hour;
 if (timeinfo.tm_min > 33 ) sayhour = timeinfo.tm_hour+1;
 if (sayhour == 24) sayhour = 0;

switch (sayhour)
 {
  case 13:  
  case 1:  UNE;    HEURE;  break;
  case 14:
  case 2:  DEUX;   HEURES;  break;
  case 15:
  case 3:  TROIS;  HEURES;  break;
  case 16:
  case 4:  QUATRE; HEURES; break;
  case 17:
  case 5:  CINQ;   HEURES;   break;
  case 18:
  case 6:  SIX;    HEURES;   break;
  case 19:
  case 7:  SEPT;   HEURES;  break;
  case 20:
  case 8:  HUIT;   HEURES; break;
  case 21:
  case 9:  NEUF;   HEURES; break;
  case 22:
  case 10: DIX;    HEURES; break;
  case 23:
  case 11: ONZE;   HEURES; break;
  case 0:  MINUIT; break;
  case 12: MIDI;   break;
 } 
}
                     #endif //FR

//--------------------------------------------                                                //
// BLE SendMessage by BLE Slow in packets of 20 chars
// or fast in one long string.
// Fast can be used in IOS app BLESerial Pro
//------------------------------
void SendMessageBLE(std::string Message)
{
 if(deviceConnected) 
   {
    if (Mem.UseBLELongString)                                                                 // If Fast transmission is possible
     {
      pTxCharacteristic->setValue(Message); 
      pTxCharacteristic->notify();
      delay(10);                                                                              // Bluetooth stack will go into congestion, if too many packets are sent
     } 
   else                                                                                       // Packets of max 20 bytes
     {   
      int parts = (Message.length()/20) + 1;
      for(int n=0;n<parts;n++)
        {   
         pTxCharacteristic->setValue(Message.substr(n*20, 20)); 
         pTxCharacteristic->notify();
         delay(10);                                                                           // Bluetooth stack will go into congestion, if too many packets are sent
        }
     }
   } 
}

//--------------------------------------------                                                //
// BLE Start BLE Classes NimBLE Version 2.x.x
//------------------------------
class MyServerCallbacks: public NimBLEServerCallbacks 
{
 void onConnect(NimBLEServer* pServer, NimBLEConnInfo& connInfo) override 
    {deviceConnected = true; Serial.println("BLE Connected"   );}
 void onDisconnect(NimBLEServer* pServer, NimBLEConnInfo& connInfo, int reason) override 
    {deviceConnected = false; Serial.println("BLE NOT Connected" );}
};
  
class MyCallbacks: public NimBLECharacteristicCallbacks 
{
 void onWrite(NimBLECharacteristic *pCharacteristic, NimBLEConnInfo& connInfo) override  
  {
   std::string rxValue = pCharacteristic->getValue();
   ReceivedMessageBLE = rxValue + "\n";
  //  if (rxValue.length() > 0) {for (int i = 0; i < rxValue.length(); i++) printf("%c",rxValue[i]); }
  //  printf("\n");
  }  
};

//--------------------------------------------                                                //
// BLE Start BLE Service
//------------------------------
void StartBLEService(void)
{
 NimBLEDevice::init(Mem.BLEbroadcastName);                                                    // Create the BLE Device
 pServer = NimBLEDevice::createServer();                                                      // Create the BLE Server
 pServer->setCallbacks(new MyServerCallbacks());
 BLEService *pService = pServer->createService(SERVICE_UUID);                                 // Create the BLE Service
 pTxCharacteristic                     =                                                      // Create a BLE Characteristic 
     pService->createCharacteristic(CHARACTERISTIC_UUID_TX, NIMBLE_PROPERTY::NOTIFY);                 
 BLECharacteristic * pRxCharacteristic = 
     pService->createCharacteristic(CHARACTERISTIC_UUID_RX, NIMBLE_PROPERTY::WRITE);
 pRxCharacteristic->setCallbacks(new MyCallbacks());
 pService->start(); 
 BLEAdvertising *pAdvertising = NimBLEDevice::getAdvertising();
 pAdvertising->addServiceUUID(SERVICE_UUID); 
 pAdvertising->setName(Mem.BLEbroadcastName);
 pServer->start();                                                                            // Start the server  Nodig??
 pServer->getAdvertising()->start();                                                          // Start advertising
 TekstSprint("BLE Waiting a client connection to notify ...\n"); 
}
//                                                                                            //
//-----------------------------
// BLE CheckBLE input and rework string
// After one hour the BLE connection is disconnected
//------------------------------
void CheckBLE(void)
{
if (deviceConnected && !oldDeviceConnected) 
  {
   oldDeviceConnected = deviceConnected;
   BLEConnectedSince = millis();                                                              // Mark time of last connection 1 hour after this time the BLE connection will disconnect
  }

if (!deviceConnected && oldDeviceConnected)                                                   // If device is disconnected start advertising
  {
   delay(300);
   pServer->startAdvertising();
   TekstSprint("Start advertising\n");
   oldDeviceConnected = deviceConnected;
   BLEConnectedSince = 0;                                                                     // Reset BLE connection timer
  }

if (deviceConnected && BLEConnectedSince > 0 && (millis() - BLEConnectedSince > 900000))      // Disconnect if connected longer than 15 minutes inactivity 
  {
    auto connHandles = NimBLEDevice::getServer()->getPeerDevices();                           // Vector of uint16_t handles
    if (!connHandles.empty()) 
      {
        uint16_t conn_handle = connHandles[0];
        int rc = ble_gap_terminate(conn_handle, BLE_ERR_REM_USER_CONN_TERM);                  // Reason = 0x13 = user terminated
        if (rc == 0) { TekstSprint("Disconnected BLE client after 1 hour\n"); } 
        else         
           { 
            sprintf(sptext,"BLE disconnect failed, error code: %d\n", rc);
            TekstSprint(sptext);  
           }
        BLEConnectedSince = 0;
      }
  }
 if(ReceivedMessageBLE.length()>0)
   {
    SendMessageBLE(ReceivedMessageBLE);
    String BLEtext = ReceivedMessageBLE.c_str();
    ReceivedMessageBLE = "";
    ReworkInputString(BLEtext); 
    BLEConnectedSince = millis();                                                             // Mark time of last connection 1 hour after thsi time the BLE connection will disconnect
   }
}

//--------------------------------------------                                                //
// WIFI WIFIEvents
//--------------------------------------------
void WiFiEvent(WiFiEvent_t event)
{
  // sprintf(sptext,"[WiFi-event] event: %d  : ", event); 
  // Tekstprint(sptext);
  WiFiEventInfo_t info;
  static bool LostPrinted = false;
    switch (event) 
     {
        case ARDUINO_EVENT_WIFI_READY: 
            Tekstprintln("WiFi interface ready");
            break;
        case ARDUINO_EVENT_WIFI_SCAN_DONE:
            Tekstprintln("Completed scan for access points");
            break;
        case ARDUINO_EVENT_WIFI_STA_START:
            Tekstprintln("WiFi client started");
            break;
        case ARDUINO_EVENT_WIFI_STA_STOP:
            Tekstprintln("WiFi clients stopped");
            break;
        case ARDUINO_EVENT_WIFI_STA_CONNECTED:
            Tekstprintln("Connected to access point");
            LostPrinted = false;
            break;
       case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
            if(!LostPrinted)
             {
              sprintf(sptext,"WiFi lost connection.");                                          // Reason: %d",info.wifi_sta_disconnected.reason); 
              Tekstprintln(sptext);
              LostPrinted = true;
             }
            //WiFi.reconnect(); //is checked in EveryMinuteUpdate()
            break;
        case ARDUINO_EVENT_WIFI_STA_AUTHMODE_CHANGE:
            Tekstprintln("Authentication mode of access point has changed");
            break;
        case ARDUINO_EVENT_WIFI_STA_GOT_IP:
            sprintf(sptext,"Connected to : %s",WiFi.SSID().c_str());
            Tekstprintln(sptext);
            sprintf(sptext, "Obtained IP address: %d.%d.%d.%d", WiFi.localIP()[0], WiFi.localIP()[1], WiFi.localIP()[2], WiFi.localIP()[3] );
            Tekstprintln(sptext);         
            strcpy(Mem.SSID,      WiFi.SSID().c_str());
            strcpy(Mem.Password , WiFi.psk().c_str());                                         // Store SSID and password
            Mem.NTPOn        = 1;                                                              // NTP On
            Mem.WIFIOn       = 1;                                                              // WIFI On  
            StoreStructInFlashMemory();  
            break;
        case ARDUINO_EVENT_WIFI_STA_LOST_IP:
            Tekstprintln("Lost IP address and IP address is reset to 0");
            break;
        case ARDUINO_EVENT_WPS_ER_SUCCESS:
            wpsStop();
            delay(100);
            WiFi.begin();
            delay(200);
             sprintf(sptext, "WPS Successfull, stopping WPS and connecting to: %s: ", WiFi.SSID().c_str());
            Tekstprintln(sptext);       
            break;
        case ARDUINO_EVENT_WPS_ER_FAILED:
            Tekstprintln("WPS Failed, retrying");
            wpsStop();
            wpsStart();
            break;
        case ARDUINO_EVENT_WPS_ER_TIMEOUT:
            Tekstprintln("WPS Timedout, Start WPS again");
            wpsStop();
            // wpsStart();
            break;
        case ARDUINO_EVENT_WPS_ER_PIN:
            sprintf(sptext,"WPS_PIN = %s",wpspin2string(info.wps_er_pin.pin_code).c_str());
            Tekstprintln(sptext);
            break;
        case ARDUINO_EVENT_WIFI_AP_START:
            Tekstprintln("WiFi access point started");
            break;
        case ARDUINO_EVENT_WIFI_AP_STOP:
            Tekstprintln("WiFi access point  stopped");
            break;
        case ARDUINO_EVENT_WIFI_AP_STACONNECTED:
            Tekstprintln("Client connected");
            break;
        case ARDUINO_EVENT_WIFI_AP_STADISCONNECTED:
            sprintf(sptext,"Client disconnected.");                                            // Reason: %d",info.wifi_ap_stadisconnected.reason); 
            Tekstprintln(sptext);
            break;
        case ARDUINO_EVENT_WIFI_AP_STAIPASSIGNED:
            Tekstprintln("Assigned IP address to client");
            break;
        case ARDUINO_EVENT_WIFI_AP_PROBEREQRECVED:
            Tekstprintln("Received probe request");
            break;
        case ARDUINO_EVENT_WIFI_AP_GOT_IP6:
            Tekstprintln("AP IPv6 is preferred");
            break;
        case ARDUINO_EVENT_WIFI_STA_GOT_IP6:
            Tekstprintln("STA IPv6 is preferred");
            break;
        case ARDUINO_EVENT_ETH_GOT_IP6:
            Tekstprintln("Ethernet IPv6 is preferred");
            break;
        case ARDUINO_EVENT_ETH_START:
            Tekstprintln("Ethernet started");
            break;
        case ARDUINO_EVENT_ETH_STOP:
            Tekstprintln("Ethernet stopped");
            break;
        case ARDUINO_EVENT_ETH_CONNECTED:
            Tekstprintln("Ethernet connected");
            break;
        case ARDUINO_EVENT_ETH_DISCONNECTED:
            // WiFi.scanNetworks will return the number of networks found("Ethernet disconnected");
            break;
        case ARDUINO_EVENT_ETH_GOT_IP:
            Tekstprintln("Obtained IP address");
            break;
        default: break;
    }
}
//--------------------------------------------                                                //
// WIFI Check for WIFI Network 
// Check if WIFI network to connect to is available
//--------------------------------------------
 bool CheckforWIFINetwork(void)         { return CheckforWIFINetwork(true);}
 bool CheckforWIFINetwork(bool PrintIt)
 {
  Tekstprintln("Scanning for networks");
  int n = WiFi.scanNetworks();                                                                // WiFi.scanNetworks will return the number of networks found
  if (n == 0)                { Tekstprintln("no networks found"); return false;} 
  if (n == WIFI_SCAN_FAILED) { Tekstprintln("All scan attempts failed - No WIFI connection");  return false;   }
  else 
    { 
     sprintf(sptext,"%d: networks found",n);    Tekstprintln(sptext);
     for (int i = 0; i < n; ++i)                                                              // Print SSID and RSSI for each network found
      {
        sprintf(sptext,"%2d: %25s %3d %1s",i+1,WiFi.SSID(i).c_str(),(int)WiFi.RSSI(i),(WiFi.encryptionType(i) == WIFI_AUTH_OPEN)?" ":"*");
        if (strcmp(WiFi.SSID(i).c_str(), Mem.SSID)==0) { strcat(sptext, " -- Will connect to");  }
        if(PrintIt) Tekstprintln(sptext);
      }
    }
 return true;                                                                                 // If no SSID and web page at 192.168.4.1 will be started to enter the credentials
 }

//--------------------------------------------                                                //
// WIFI Scan for WIFI stations and print a list of networks found
//--------------------------------------------
void ScanWIFI(void)
{
 WiFi.disconnect(); 
 WiFi.mode(WIFI_STA);                                                                         // Set WiFi to station mode and disconnect from an AP if it was previously connected.
 delay(100);
 int n = WiFi.scanNetworks();                                                                 // WiFi.scanNetworks will return the number of networks found.
 if (n == 0)  { Tekstprintln("no networks found");  } 
 else 
   {
    sprintf(sptext,"%d networks found",n);   Tekstprintln(sptext);
    Tekstprintln("Nr | SSID                             | RSSI | CH | Encryption");
    for(int i = 0; i < n; ++i) 
      {
       sprintf(sptext,"%2d | %-32.32s | %4d | %2d | ",i + 1, 
                       WiFi.SSID(i).c_str(), (int)WiFi.RSSI(i), (int)WiFi.channel(i));        // Print SSID and RSSI for each network found
       Tekstprint(sptext);
       switch (WiFi.encryptionType(i))
           {
             case WIFI_AUTH_OPEN:            Tekstprint("open");      break;
             case WIFI_AUTH_WEP:             Tekstprint("WEP");       break;
             case WIFI_AUTH_WPA_PSK:         Tekstprint("WPA");       break;
             case WIFI_AUTH_WPA2_PSK:        Tekstprint("WPA2");      break;
             case WIFI_AUTH_WPA_WPA2_PSK:    Tekstprint("WPA+WPA2");  break;
             case WIFI_AUTH_WPA2_ENTERPRISE: Tekstprint("WPA2-EAP");  break;
             case WIFI_AUTH_WPA3_PSK:        Tekstprint("WPA3");      break;
             case WIFI_AUTH_WPA2_WPA3_PSK:   Tekstprint("WPA2+WPA3"); break;
             case WIFI_AUTH_WAPI_PSK:        Tekstprint("WAPI");      break;
             default:                        Tekstprint("unknown");
            }
        Tekstprintln("");
        delay(10);
        }
   }
Tekstprintln("");
//WiFi.scanDelete();                                                                            // Delete the scan result to free memory for code below.
}
//--------------------------------------------                                                //
// WIFI Check for WIFI router SSID and password 
// If not valid then start webpage to enter the credentials
//--------------------------------------------
void ConnectWIFI(void)
{
if( (strlen(Mem.Password)<5 || strlen(Mem.SSID)<3))   // If WIFI required and no SSID or password
   {
     Tekstprintln("Starting AP mode to enter SSID and password of the WIFI router");
     StartAPMode();
   }  
 else 
  { 
    Tekstprintln("Starting WIFI/NTP");
    StartWIFI_NTP();
  }
 }
//--------------------------------------------                                                //
// WIFI Check if WIFI is sill connected and if not restore it
//-------------------------------------------- 
void CheckRestoreWIFIconnectivity(void)
{
 if(Mem.WIFIOn && WIFIwasConnected)                                                           // If WIFI switch is On and there was a connection.
   {
    if(WiFi.localIP()[0] == 0) 
       {
        if(Mem.WIFIcredentials == SET_AND_OK)  WiFi.reconnect();                              // If connection lost and WIFI is used reconnect
        if(CheckforWIFINetwork(false) && !WIFIwasConnected) StartWIFI_NTP();                  // If there was no WIFI at start up start a WIFI connection 
        if(WiFi.localIP()[0] != 0)                                                            // WIFI connection is established
          {
          sprintf(sptext, "Reconnected to IP address: %d.%d.%d.%d", WiFi.localIP()[0], WiFi.localIP()[1], WiFi.localIP()[2], WiFi.localIP()[3] );
          Tekstprintln(sptext);
          }
       }
    }
 }

//--------------------------------------------                                                //
// WIFI Start WIFI connection and NTP service
//--------------------------------------------
void StartWIFI_NTP(void)
{
 WiFi.disconnect(true,true);                                                                  // Remove all previous settings and entered SSID and password
 delay(100);
 WiFi.setHostname(Mem.BLEbroadcastName);                                                      // Set the host name in the WIFI router instead of a cryptic esp32 name
 WiFi.mode(WIFI_STA);  
 WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE, INADDR_NONE);
 WIFIwasConnected = false;
 WiFi.begin(Mem.SSID, Mem.Password);
 wifiEventHandler = WiFi.onEvent(WiFiEvent);                                                  // Using WiFi.onEvent interrupts and crashes IL9341 screen display while writing the screen
 MDNS.begin(Mem.BLEbroadcastName);                                                            // After reset http://wordclock.local 
 // Task function, Name, Stack size, Parameter, Priority, Task handle, Core 1
 xTaskCreatePinnedToCore(WebServerTask,"WebServerTask", 4096, NULL, 1, NULL, 1 );             // Start handleClient loop on core 1
 int tryDelay = 5000;                                                                         // Will try for about 50 seconds (20x 10,000ms)
 int numberOfTries = 10;
 while (true)                                                                                 // Wait for the WiFi event
  {
    switch(WiFi.status()) 
    {
     case WL_NO_SSID_AVAIL:
          Tekstprintln("[WiFi] SSID not found (Unexpected error)\n Is the router turned off?");
          return;
     case WL_CONNECT_FAILED:
          Tekstprint("[WiFi] Failed - WiFi not connected! Reason:? \n Reset the clock with option R and re-enter SSID and Password.");
          return;
     case WL_CONNECTION_LOST:
          Tekstprintln("[WiFi] Connection was lost");
          break;
     case WL_SCAN_COMPLETED:
          Tekstprintln("[WiFi] Scan is completed");
          break;
     case WL_DISCONNECTED:
          Tekstprintln("[WiFi] WiFi is disconnected, will reconnect");
          WiFi.reconnect();
          break;
     case WL_CONNECTED:
          sprintf(sptext, "IP Address: %d.%d.%d.%d", WiFi.localIP()[0], WiFi.localIP()[1], WiFi.localIP()[2], WiFi.localIP()[3] );
          Tekstprintln(sptext); 
          WIFIwasConnected = true;                                                            // No we know the SSID ans password are correct and we can reconnect
          Mem.WIFIcredentials = SET_AND_OK;
          StoreStructInFlashMemory();
          break;
     default:
          Serial.print("[WiFi] WiFi Status: ");
          Serial.println(WiFi.status());
          break;
    } 
  LEDstartup(orange);                                                                      // orange colour in WS2812 and SK6812 LEDs
  if (WIFIwasConnected) break;       
  if(numberOfTries <= 0)
    {
     Tekstprintln("[WiFi] Failed to connect to WiFi!");
     WiFi.disconnect();                                                                     // Use disconnect function to force stop trying to connect
//     WIFIwasConnected = false;
     switch(Mem.WIFIcredentials)
        {    
         case NOT_SET:
             Tekstprintln("Check SSID and password or turn WIFI in menu off with option W");
             break;               
         case SET:
             Tekstprintln("Check your SSID name and password.\nRe-enter your password with option B in the menu. Password is now deleted");    
             strcpy(Mem.Password,"");                                                               // This will force a restart is AP mode. PWD can not be checked in menu. SSID can be checked
             break;
         case SET_AND_OK:
             Tekstprintln("Check WIFI router. The router is probably turned off");                  
             break;       
         case IN_AP_NOT_SET:    
         default:    
             Tekstprintln("Unknown condition. Re-enter SSID and password. They are deleted.\nOr turn WIFI in the menu with option W off");      
             strcpy(Mem.SSID,"");
             strcpy(Mem.Password,"");  
             break;
        }
    return;
    } 
  else 
      { 
       delay(tryDelay);  
       numberOfTries--;    
      }
  }
if ( !WIFIwasConnected) return;                                                                  // If WIFI connection fails -> return
//  sprintf(sptext, "IP Address: %d.%d.%d.%d", WiFi.localIP()[0], WiFi.localIP()[1], WiFi.localIP()[2], WiFi.localIP()[3] );  Tekstprintln(sptext); 
if(Mem.NTPOn )
  { 
   initSNTP();
   if(wait4SNTP())  Tekstprintln("NTP is On and synced");
   else             Tekstprintln("NTP is On but NOT synced");
   }   
 if(Mem.WIFIOn) WebPage();                                                                    // Show the web page if WIFI is on
 Tekstprint("Web page started\n");
// WiFi.removeEvent(wifiEventHandler);  You can leave it on because it undertakes no actions. Every minute there is a connectivity check  // Remove the WIFI event handler
}
//--------------------------------------------                                                //
// WIFI WebServerTask on Core 1
// instead of 1000 loops/s it is now 210,000 loops/s 
//--------------------------------------------
void WebServerTask(void *pvParameters) 
{
  for (;;) 
  {
    server.handleClient();                                                                    // This is normally blocking
    vTaskDelay(1);                                                                            // Small delay to yield control
  }
}
//--------------------------------------------                                                //
// NTP functions
//--------------------------------------------
void NTPnotify(struct timeval* t) { Tekstprint("NTP time synchronized\n");}
void setTimezone()                { setenv("TZ", Mem.Timezone, 1);  tzset(); }
void initSNTP() 
{  
 sntp_set_sync_interval(1 * 60 * 60 * 1000UL);  // 1 hour
 sntp_set_time_sync_notification_cb(NTPnotify);                                               // Registers a callback function (void NTPnotify(...) that will be called whenever the SNTP client successfully synchronizes the system time with an NTP server.
 esp_sntp_setoperatingmode(ESP_SNTP_OPMODE_POLL);
 esp_sntp_setservername(0, "pool.ntp.org");
 esp_sntp_init();
 setTimezone();
}
//--------------------------------------------                                                //
// NTP Get a NTP time and wait max 5 sec 
//--------------------------------------------
bool wait4SNTP() 
{
 int32_t Tick = millis(); 
 bool SNTPtimeValid = true;
 while (sntp_get_sync_status() != SNTP_SYNC_STATUS_COMPLETED) 
  { if ((millis() - Tick) >5000) {SNTPtimeValid = false; break;}   }                          // Wait max 5 seconds 
return  SNTPtimeValid;
}

//--------------------------------------------                                                //
// WIFI WEBPAGE 
//--------------------------------------------
void WebPage(void)
{
 int i = 0, n;
 static bool updateSuccess = false;                                                          // Track update status
 SWversion();
 for (n = 0; n < strlen(index_html_top); n++) HTML_page[i++] = index_html_top[n];
 if (i > MAXSIZE_HTML_PAGE - 999) 
    { strcat(HTML_page, "<br> *** INCREASE MAXSIZE_HTML_PAGE in Webpage.h ***<br><br><br>"); } 
 else 
    {
     sprintf(sptext, "<head><title>%s</title></head>",Mem.BLEbroadcastName);
     for (n = 0; n < strlen(sptext); n++)            HTML_page[i++] = sptext[n];
     for (n = 0; n < strlen(html_info); n++)         HTML_page[i++] = html_info[n];
     for (n = 0; n < strlen(index_html_footer); n++) HTML_page[i++] = index_html_footer[n];
     HTML_page[i] = 0;
    }
 
 server.on("/", HTTP_GET, []() { server.send(200, "text/html", HTML_page);  });
 server.on("/get", HTTP_GET, []()
   {
    String inputMessage;
    if (server.hasArg(PARAM_INPUT_1)) { inputMessage = server.arg(PARAM_INPUT_1); } 
    else { inputMessage = ""; }
    if (!inputMessage.equalsIgnoreCase("Y") ) ReworkInputString(inputMessage);
    if (inputMessage.equalsIgnoreCase("Y") ) OptionYRainbow = true;
    SWversion();
    int i = 0, n;
    for (n = 0; n < strlen(index_html_top); n++)    HTML_page[i++] = index_html_top[n];
    for (n = 0; n < strlen(html_info); n++)         HTML_page[i++] = html_info[n];
    for (n = 0; n < strlen(index_html_footer); n++) HTML_page[i++] = index_html_footer[n];
    HTML_page[i] = 0;
    server.send(200, "text/html", HTML_page);
    });

 server.on("/update", HTTP_GET, []() { server.send(200, "text/html", OTA_html); });
 server.on("/update", HTTP_POST, 
    [](){                                                                                     // Response handler - called after upload completes
         if (updateSuccess) {
           server.send(200, "text/html",
            "<!DOCTYPE html><html><body>"
            "<h2>Update successful. Rebooting...</h2>"
            "<p>Device will restart in 10 seconds.</p>"
            "<script>setTimeout(()=>{location.href='/'}, 12000);</script>"
            "</body></html>");
           delay(3000);  // Let response send
           shouldReboot = true;
         } else {
           server.send(500, "text/html",
            "<!DOCTYPE html><html><body>"
            "<h2>Update Failed!</h2>"
            "<p>Please try again or check serial output for errors.</p>"
            "<script>setTimeout(()=>{location.href='/update'}, 5000);</script>"
            "</body></html>");
         }
    }, 
    [](){ // Upload handler
      HTTPUpload& upload = server.upload();
      
      if (upload.status == UPLOAD_FILE_START) 
        {
         updateSuccess = false;
         Serial.printf("\n=== OTA Update Started ===\n");
         Serial.printf("Filename: %s\n", upload.filename.c_str());
         Serial.printf("Free Sketch Space: %u bytes\n", ESP.getFreeSketchSpace());
         
         // Validate filename
         if (!upload.filename.endsWith(".bin")) {
           Serial.println("ERROR: Invalid file type. Must be .bin");
           Update.abort();
           return;
         }
         
         // Begin update with maximum available size
         uint32_t maxSketchSpace = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;
         if (!Update.begin(maxSketchSpace)) {
           Serial.println("ERROR: Not enough space for OTA update");
           Update.printError(Serial);
           return;
         }
         
         Serial.println("Update begin successful");
        } 
      else if (upload.status == UPLOAD_FILE_WRITE) 
        {
         // Write firmware chunk
         size_t written = Update.write(upload.buf, upload.currentSize);
         if (written != upload.currentSize) {
           Serial.printf("ERROR: Write failed - written %u != %u bytes\n", 
                        written, upload.currentSize);
           Update.printError(Serial);
           Update.abort();
           return;
         }
         
         // Progress feedback (optional - remove if it causes issues)
         static uint32_t lastPrint = 0;
         if (millis() - lastPrint > 1000) {
           Serial.printf("Progress: %u bytes\n", upload.totalSize + upload.currentSize);
           lastPrint = millis();
         }
        } 
      else if (upload.status == UPLOAD_FILE_END) 
        {
         // Finalize update with MD5 verification
         if (Update.end(true)) {
           Serial.printf("\n=== OTA Update Successful ===\n");
           Serial.printf("Total size: %u bytes\n", upload.totalSize);
           
           // Verify MD5 if available
           if (Update.hasError()) {
             Serial.println("WARNING: Update completed but has errors");
             Update.printError(Serial);
             updateSuccess = false;
           } else {
             Serial.println("Firmware verification passed");
             updateSuccess = true;
           }
         } else {
           Serial.println("\n=== OTA Update Failed ===");
           Serial.printf("Bytes received: %u\n", upload.totalSize);
           Update.printError(Serial);
           updateSuccess = false;
         }
        } 
      else if (upload.status == UPLOAD_FILE_ABORTED) 
        {
         Update.end();
         Serial.println("\n=== OTA Update Aborted ===");
         updateSuccess = false;
        }
    });

 server.onNotFound([]() {  server.send(404, "text/plain", "Not found");  });
 server.begin();
}
//--------------------------------------------                                                //
// WIFI WEBPAGE Login credentials Access Point page with 192.168.4.1
//--------------------------------------------
void StartAPMode(void) 
{
 WiFi.disconnect(true);                                                                      // Disconnect and erase old config
 apMode = true;
 Mem.WIFIcredentials = IN_AP_NOT_SET;
 StoreStructInFlashMemory();
 WiFi.softAP(AP_SSID, AP_PASSWORD);
 delay(100);
 xTaskCreatePinnedToCore(WebServerTask,"WebServerTask", 4096, NULL, 1, NULL, 1 );            // Start handleClient loop on core 1
 dnsServer.start(53, "*", WiFi.softAPIP());
 Tekstprintln("\nConnect to StartWordcock in WIFI on your mobile.\nEnter password: wordclock\nThen in URL: 192.168.4.1 and enter router credentials");   
 server.on("/", HTTP_GET, []() {  server.send(200, "text/html", SoftAP_html); });            // Serve AP HTML form
 server.on("/", HTTP_POST, []()                                                              // Handle AP form POST
  {
    int params = server.args();
    for (int i = 0; i < params; i++) 
    {
      String name = server.argName(i);
      String value = server.arg(i);
      if (name == "ssid") strcpy(Mem.SSID, value.c_str());
      if (name == "pass") strcpy(Mem.Password, value.c_str());
    }
    Mem.WIFIcredentials = SET;
    StoreStructInFlashMemory();
    server.send(200, "text/plain", "Credentials saved. Restarting..."); 
    delay(300);
    ESP.restart();
  } );
 server.begin();
 Serial.println("AP Mode Started");
 Serial.print("AP SSID: ");       Serial.println(AP_SSID);
 Serial.print("AP IP Address: "); Serial.println(WiFi.softAPIP()); 
}

//--------------------------------------------                                                //
// WIFI WPS functions
//--------------------------------------------
void wpsInitConfig()
{
 config.wps_type = ESP_WPS_MODE;
 strcpy(config.factory_info.manufacturer, ESP_MANUFACTURER);
 strcpy(config.factory_info.model_number, ESP_MODEL_NUMBER);
 strcpy(config.factory_info.model_name, ESP_MODEL_NAME);
 strcpy(config.factory_info.device_name, ESP_DEVICE_NAME);
}

void wpsStart()
{
 if(esp_wifi_wps_enable(&config))  Serial.println("WPS Enable Failed");
 else if(esp_wifi_wps_start(0)) 	 Serial.println("WPS Start Failed");
}

void wpsStop()
{
 if(esp_wifi_wps_disable()) 	     Serial.println("WPS Disable Failed");
}

String wpspin2string(uint8_t a[])
{
 char wps_pin[9];
 for(int i=0;i<8;i++){ wps_pin[i] = a[i]; }
 wps_pin[8] = '\0';
 return (String)wps_pin;
}
//--------------------------------------------                                                //
// End WPS
//--------------------------------------------

                                  #ifdef ONEWIREKEYPAD3x4
//--------------------------------------------                                                //
// KEYPAD check for Onewire Keypad input
//--------------------------------------------
void OnewireKeypad3x4Check(void)
{
 int keyvalue=99;
 int Key=0;
 int sensorValue = analogRead(OneWirePin); // read the value from the sensor:
 switch(sensorValue)
  {
    case   0 ... 100:  keyvalue = 13; break;   // noise
    case 101 ... 132:  keyvalue = 12; Key = '*'; break;   // * 
    case 133 ... 154:  keyvalue =  0; Key = '0'; break;   // 0 
    case 155 ... 216:  keyvalue = 11; Key = '#'; break;   // # 
    case 217 ... 281:  keyvalue =  7; Key = '7'; break;   // 7 
    case 282 ... 318:  keyvalue =  4; Key = '4'; break;   // 4 
    case 319 ... 349:  keyvalue =  1; Key = '1'; break;   // 1 
    case 350 ... 390:  keyvalue =  8; Key = '8'; break;   // 8 
    case 391 ... 463:  keyvalue =  5; Key = '5'; break;   // 5 
    case 464 ... 519:  keyvalue =  2; Key = '2'; break;   // 2 
    case 520 ... 619:  keyvalue =  9; Key = '9'; break;   // 9 
    case 620 ... 848:  keyvalue =  6; Key = '6'; break;   // 6 
    case 849 ... 1023: keyvalue =  3; Key = '3'; break;   // 3
  }
 if(keyvalue<13) { Serial.println(Key); delay(300); }
  if (Key == 12)   // *                                                                       // Pressing a * activates the keyboard input. 
   {  
    KeyInputactivated = true;
    KeyLooptime = millis();
    KeypadString ="";
    ColorLeds("",0,NUM_LEDS-1,0x00FF00);                                                      // Turn all LEDs green
    ShowLeds();                                                                               // Push data in LED strip to commit the changes
    Serial.println("Key entry activated");
   }
 if (KeyInputactivated && (Key>=0 && Key<10))
   {
    delay(20); 
    KeypadString += Key;                                                                      // Digit keys 0 - 9
    ColorLeds("",0,Key-48,0xFF0000);                                                          // Turn all LEDs red
    ShowLeds();                                                                               // Push data in LED strip to commit the changes
 //   Serial.println(KeypadString);
   }
 if (KeypadString.length()>5)                                                                 // If six numbers are entered rework this to a time hhmmss
   {       
   if(KeypadString=="999999")
     { 
      KeypadString = "";   
      Reset();
      Serial.println("Settings reset");   
     }
    else 
     {      
      ReworkInputString(KeypadString);                                                        // Rework ReworkInputString();
      KeypadString = "";
      Serial.println("Time changed");
     }    
   }
 if ( KeyInputactivated && ((millis() - KeyLooptime) > 30000) ) 
   {  
    KeyInputactivated = false;                                                                // Stop data entry after 30 seconds. This avoids unintended entry 
    KeypadString ="";
    Serial.println("Keyboard entry stopped");
  }
}
                                  #endif  //ONEWIREKEYPAD3x4  
                                  #ifdef ONEWIREKEYPAD3x1
//--------------------------------------------                                                //
// KEYPAD check for Onewire Keypad input with 5V and 1.1, 4.7, 4.7, 4.7 kOhm resistors
//--------------------------------------------
void OnewireKeypad3x1Check(void)
{
 int8_t keyvalue = 99;
 int8_t Key;
 int16_t sensorValue = analogRead(OneWirePin);                                                    // Read the value from the sensor:
 switch(sensorValue)
   {
    case   0 ... 385:  keyvalue = 99;            break;                                       // Noise
    case 386 ... 635:  keyvalue = -1; Key = 'G'; break;                                       // G 
    case 636 ... 910:  keyvalue =  0; Key = 'Y'; break;                                       // Y 
    case 911 ... 1024: keyvalue =  1; Key = 'R'; break;                                       // R 
   }
 if(keyvalue<2) 
    { 
//     Serial.print(sensorValue); Serial.println(Key); 
     if (Key == 'R') ProcessKeyPressTurn(1);                                                  // Pressing Red increases hour or minute. 
     if (Key == 'G') ProcessKeyPressTurn(-1);                                                 // Pressing Green decreases hour or minute. 
     if (Key == 'Y') ProcessKeyPressTurn(0);                                                  // Pressing Yellow activates the keyboard input. 
     delay(200);     
    }
}
                                  #endif //ONEWIREKEYPAD3x1
//--------------------------------------------                                                //
// KEYPAD 3x1 Init 
//--------------------------------------------
 void InitKeypad3x1(void)
 {
 digitalWrite(COLPIN,LOW);
 sprintf(sptext,"3*1 keypad %s used", Mem.UseRotary==2?"IS":"NOT");
 Tekstprintln(sptext);
 }
//--------------------------------------------
// KEYPAD check for Keypad input
//--------------------------------------------                           
void Keypad3x1Check(void)
{ 
// digitalWrite(COLPIN,LOW);                                                                    // Mimic a key press on pin 6 in order to select the first column
 char Key = keypad.getKey();
 if(Key)
  {
   Serial.println(Key);
   if (Key == 'Y')    ProcessKeyPressTurn(0);                                                 // Pressing Middle button Yellow activates the keyboard input.   
   else if (ChangeTime)    
     { 
      if (Key == 'R') ProcessKeyPressTurn(1);                                                 // Pressing Red increases hour or minute. 
      if (Key == 'G') ProcessKeyPressTurn(-1);                                                // Pressing Green decreases hour or minute. 
     }
   delay(200);
  }
} 
//--------------------------------------------                                                //
// KY-040 ROTARY encoder Init 
//--------------------------------------------
 void InitRotaryMod(void)
 {
 pinMode(encoderPinA,  INPUT_PULLUP);
 pinMode(encoderPinB,  INPUT_PULLUP);  
 pinMode(clearButton,  INPUT_PULLUP); 
 myEnc.write(0);                                                                              // Clear Rotary encoder buffer
 sprintf(sptext,"Rotary %s used", Mem.UseRotary==1?"IS":"NOT");
 Tekstprintln(sptext);   
 } 
//--------------------------------------------                                                //
// KY-040 ROTARY check if the rotary is moving
//--------------------------------------------
void RotaryEncoderCheck(void)
{
 int ActionPress = 999;
 if (digitalRead(clearButton) == LOW )          ProcessKeyPressTurn(0);                       // Set the time by pressing rotary button
 else if (ChangeTime || ChangeLightIntensity)    
  {   
   ActionPress = myEnc.read();                                                                // If the knob is turned store the direction (-1 or 1)
   if (ActionPress == 0) {  ActionPress = 999;  ProcessKeyPressTurn(ActionPress);  }          // Sent 999 = nop (no operation) 
   if (ActionPress == 1 || ActionPress == -1 )  ProcessKeyPressTurn(ActionPress);             // Process the ActionPress
  } 
 myEnc.write(0);                                                                              // Set encoder pos back to 0

if ((unsigned long) (millis() - RotaryPressTimer) > 60000)                                    // After 60 sec after shaft is pressed time of light intensity can not be changed 
   {
    if (ChangeTime || ChangeLightIntensity)                         
      {
        Tekstprintln("<-- Changing time is over -->");
        NoofRotaryPressed = 0;
      }
    ChangeTime            = false;
    ChangeLightIntensity  = false;
   }   
}

//--------------------------------------------                                                //
// CLOCK KY-040 Rotary or Membrane 3x1 processing input
// encoderPos < 1 left minus 
// encoderPos = 0 attention and selection choice
// encoderPos > 1 right plus
//--------------------------------------------
void ProcessKeyPressTurn(int encoderPos)
{
 if (ChangeTime || ChangeLightIntensity)                                                      // If shaft is pressed time of light intensity can be changed
   {
    if ( encoderPos!=999 && ( (millis() - Looptime) > 250))                                   // If rotary turned avoid debounce within 0.25 sec
     {   
     Serial.print(F("----> Index:"));   Serial.println(encoderPos);
     if (encoderPos == 1)                                                                     // Increase  
       {     
        if (ChangeLightIntensity)  { WriteLightReducer(5); }                                  // If time < 60 sec then adjust light intensity factor
        if (ChangeTime) 
          {
           if (NoofRotaryPressed == 1)                                                        // Change hours
              {if( ++timeinfo.tm_hour >23) { timeinfo.tm_hour = 0; } }      
           if (NoofRotaryPressed == 2)                                                        // Change minutes
              {  timeinfo.tm_sec = 0;
               if( ++timeinfo.tm_min  >59) 
                 { timeinfo.tm_min  = 0; if( timeinfo.tm_hour >=23) { timeinfo.tm_hour = 0; } }   
              }
           } 
        }    
      if (encoderPos == -1)                                                                   // Decrease
       {
       if (ChangeLightIntensity)   { WriteLightReducer(-5); }                                 // If time < 60 sec then adjust light intensity factor
       if (ChangeTime)     
          {
           if (NoofRotaryPressed == 1)                                                        // Change hours
             {if( timeinfo.tm_hour-- ==0)  { timeinfo.tm_hour = 23; }  }      
           if (NoofRotaryPressed == 2)                                                        // Change minutes
             { 
              timeinfo.tm_sec = 0;
              if( timeinfo.tm_min-- == 0) 
                { timeinfo.tm_min  = 59; if( timeinfo.tm_hour  == 0) { timeinfo.tm_hour = 23; } }
             } 
           }          
        } 
      SetDS3231Time();  
      Displaytime();
      Looptime = millis();    
     }                                                     
   }
 if (encoderPos == 0 )                                                                        // Set the time by pressing rotary button
   { 
    delay(250);
    ChangeTime            = false;
    ChangeLightIntensity  = false;
    RotaryPressTimer      = millis();                                                         // Record the time the shaft was pressed.
    if(++NoofRotaryPressed > 9) NoofRotaryPressed = 0;
    switch (NoofRotaryPressed)                                                                // No of times the rotary is pressed
      {
       case 1:  ChangeTime = true;           BlinkUUR(3, 300);      break;                    // Change the hours
       case 2:  ChangeTime = true;           BlinkHETISWAS(3, 300); break;                    // Change the hours        
       case 3:  ChangeLightIntensity = true; BlinkTWAALF(3, 300);   break;                    // Turn on TWAALF and change intensity 
       case 4:                                                      break;                    // 
       case 5:                                                      break;                    // 
       case 6:                                                      break;                    // 
       case 7:                                                      break;                    //                                
       case 8:                                                      break;
       case 9:  Reset();                                            break;                    // Reset all settings                                                                  
      default:                                                      break;                     
      }
    Serial.print(F("NoofRotaryPressed: "));   Serial.println(NoofRotaryPressed);   
    Looptime = millis();     
    Displaytime();                                                                            // Turn on the LEDs with proper time
   }
 }

//--------------------------------------------                                                //
// IR-RECEIVER Init
//--------------------------------------------
void Init_IRreceiver(void)
{
 IrReceiver.begin(IRReceiverPin);
 if (Mem.UseRotary == 3)  {  GetIRRemoteFromFlashMemory(); }                                  // Load IR settings (only if IR remote is enabled)
 if (IRMem.remoteIdentified && IRMem.buttons[0].learned)                                      // Check if we already have learned buttons
  {
   learningMode = false;
   PrintAllMappings();
  }
}
//--------------------------------------------                                                //
// IR-RECEIVER Decode received signal,
// routes to learning or recognition mode and return code
//--------------------------------------------
uint16_t IrReceiverDecode(void)
{
  uint16_t command = 0; 
  if (IrReceiver.decode())
  {
                   command = IrReceiver.decodedIRData.command;
    uint16_t address       = IrReceiver.decodedIRData.address;
    decode_type_t protocol = IrReceiver.decodedIRData.protocol;
    if (!(IrReceiver.decodedIRData.flags & IRDATA_FLAGS_IS_REPEAT))                           // Ignore repeat codes
    {
      if (learningMode)  { ProcessLearningMode(protocol, command, address);  }
      else               { ProcessRecognitionMode(protocol, command, address);  }
    }
    delay(100);
    IrReceiver.resume();
  }
  return command;
}

//============================================
// IR-RECEIVER Internal function - Handles learning mode
// Validates remote matches, Saves when complete
//============================================
void ProcessLearningMode(decode_type_t protocol, uint16_t command, uint16_t address)
{
  // First button press - identify the remote control
  if (!IRMem.remoteIdentified)
  {
    IRMem.learnedRemoteAddress = address;
    IRMem.learnedRemoteProtocol = (uint8_t)protocol;
    IRMem.remoteIdentified = true;
    sprintf(sptext, "Remote identified - Protocol: %s, Address: 0x%04X", getProtocolString(protocol), address);
    Tekstprintln(sptext);
  }
  else
  {
    if (address != IRMem.learnedRemoteAddress || protocol != (decode_type_t)IRMem.learnedRemoteProtocol)    // Check if this is from the same remote
    {
      Tekstprintln("⚠ Wrong remote! Please use the same remote control.");
      sprintf(sptext, "Expected Address: 0x%04X, Got: 0x%04X", IRMem.learnedRemoteAddress, address);
      Tekstprintln(sptext);
      return;
    }
  }
  IRMem.buttons[currentLearningIndex].protocol = (uint8_t)protocol;                           // Store the button mapping
  IRMem.buttons[currentLearningIndex].command = command;
  IRMem.buttons[currentLearningIndex].address = address;
  IRMem.buttons[currentLearningIndex].learned = true;
  
  sprintf(sptext, "✓ Learned '%s' - Protocol: %s, Command: 0x%04X, Address: 0x%04X", 
          ButtonNames[currentLearningIndex], getProtocolString(protocol), command, address);
  Tekstprintln(sptext);
  currentLearningIndex++;
  if (currentLearningIndex < MAX_BUTTONS)
  {
    sprintf(sptext, "\nPlease press button: %s", ButtonNames[currentLearningIndex]);
    Tekstprintln(sptext);
  }
  else
  {
    learningMode = false;                                                                     // Learning complete
    StoreIRRemoteInFlashMemory();
    Tekstprintln("\n=== Learning Complete & Saved! ===");
    Tekstprintln("\nStored Button Mappings:");
    PrintAllMappings();
    Tekstprintln("\n=== Now in Recognition Mode ===");
    sprintf(sptext, "Only responding to remote with Address: 0x%04X", IRMem.learnedRemoteAddress);
    Tekstprintln(sptext);
    Tekstprintln("Press any learned button to test...\nPress OK to quit test mode\n");
  }
}

//============================================
// IR-RECEIVER Internal function - handles recognition mode
// Validates remote address Calls RecognizeButton
//============================================
void ProcessRecognitionMode(decode_type_t protocol, uint16_t command, uint16_t address)
{
  if (address != IRMem.learnedRemoteAddress || 
     protocol != (decode_type_t) IRMem.learnedRemoteProtocol)                                 // Check if it's from the learned remote
  {
    sprintf(sptext, "⚠ Ignored - Wrong remote (Address: 0x%04X)", address);
    if (address!=0) Tekstprintln(sptext);                                                                     // Stray input detected
  }
  else {RecognizeButton(protocol, command, address);  }                                       // Correct remote - identify which button was pressed
}

//--------------------------------------------                                                //
// IR-RECEIVER Initializes learning mode
// Resets all button data, Prompts for first button
//--------------------------------------------
void StartIRLearning()
{
  Tekstprintln("\n=== Starting Learning Mode ===");
  currentLearningIndex = 0;
  learningMode = true;
  IRMem.remoteIdentified = false;
  for (int i = 0; i < MAX_BUTTONS; i++) {IRMem.buttons[i].learned = false; }
  Tekstprintln("Please press button: 0");
}

//--------------------------------------------                                                //
// IR-RECEIVER Shows learned buttons
//--------------------------------------------
void PrintAllMappings(void)
{
 if (!IRMem.remoteIdentified) {Tekstprintln("No remote learned yet!");  return; }
 PrintLine(35);
 sprintf(sptext, "Remote Address: 0x%04X", IRMem.learnedRemoteAddress);
 Tekstprintln(sptext);
 sprintf(sptext, "Remote Protocol: %s", getProtocolString( (decode_type_t) IRMem.learnedRemoteProtocol));
 Tekstprintln(sptext);
 PrintLine(35);
  for (int i = 0; i < MAX_BUTTONS; i++)
  {
   if (IRMem.buttons[i].learned)
    {
     sprintf(sptext, "%s\t-> Cmd: 0x%04X", ButtonNames[i], IRMem.buttons[i].command);
     Tekstprintln(sptext);     
    }
  }
 PrintLine(35);
}

//--------------------------------------------                                                //
// IR-RECEIVER Reset all settings
//--------------------------------------------
void ResetAllIRremoteSettings()
{
 Tekstprintln("\n=== Resetting All IR remote Data ===");
 IRMem.remoteIdentified = false;
 IRMem.learnedRemoteAddress = 0;
 IRMem.learnedRemoteProtocol = 0;
 for (int i = 0; i < MAX_BUTTONS; i++)
  {
    IRMem.buttons[i].learned = false;
    IRMem.buttons[i].protocol = 0;
    IRMem.buttons[i].command = 0;
    IRMem.buttons[i].address = 0;
  }
  
  StoreIRRemoteInFlashMemory();
  Tekstprintln("✓ IR remote reset complete.");   
  learningMode = false;
}


//--------------------------------------------                                                //
// IR-RECEIVER Identifies button pressed
//--------------------------------------------
int RecognizeButton(decode_type_t protocol, uint16_t command, uint16_t address)
{
  bool found = false;
  for (int i = 0; i < MAX_BUTTONS; i++)
  {
    if (IRMem.buttons[i].learned && 
        IRMem.buttons[i].protocol == (uint8_t)protocol &&
        IRMem.buttons[i].command == command && 
        IRMem.buttons[i].address == address)
    {
      sprintf(sptext, "Button pressed: %s", ButtonNames[i]);
      Tekstprintln(sptext);
      found = true;
      ReworkIRremoteValue(i);
      return i;
    }
  }
  
  if (!found)
  {
    sprintf(sptext, "Unknown button - Protocol: %s, Command: 0x%04X, Address: 0x%04X", getProtocolString(protocol), command, address);
    Tekstprintln(sptext);
    ReworkIRremoteValue(-1);
    return -1;
  } 
 return -1;
}

//--------------------------------------------                                                //
// IR-RECEIVER ReworkRemoteValue
// String ButtonNames[] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9",
//                        "UP", "DOWN", "LEFT", "RIGHT", "POWER", "OK"};
//--------------------------------------------
void ReworkIRremoteValue(int ButtonNamesNr)
{
 if (ButtonNamesNr == -1)  {Tekstprintln("Unknown button - ignored");  return;  }
 if (IR_PowerOnstate == false && ButtonNamesNr != 14 ) return;                               // Button "POWER"

 String ButtonName = ButtonNames[ButtonNamesNr];
 switch(ButtonNamesNr)
  {
    case 0:                                                                                   // Button "0"
    case 1:                                                                                   // Button "1"
    case 2:                                                                                   // Button "2"
    case 3:                                                                                   // Button "3"
    case 4:                                                                                   // Button "4"
    case 5:                                                                                   // Button "5"
    case 6:                                                                                   // Button "6"
    case 7:                                                                                   // Button "7"
    case 8:                                                                                   // Button "8"
    case 9:                                                                                   // Button "9"
      EnteredDigits += ButtonName;                                                            // Add digit to entered string  
      if (EnteredDigits.length() > 6) {EnteredDigits = EnteredDigits.substring(0, 6); }      // Limit to 6 digits (HHMMSS)
      if (EnteredDigits.length() == 6)
      {
        sprintf(sptext, "Time entered: %c%c:%c%c:%c%c (press OK)", 
                EnteredDigits[0], EnteredDigits[1], EnteredDigits[2], 
                EnteredDigits[3], EnteredDigits[4], EnteredDigits[5]);
      }
      else sprintf(sptext, "Digits: %s (Need 6 for HHMMSS)", EnteredDigits.c_str());
      Tekstprintln(sptext);
      break;
    case 10:                                                                                  // Button "UP"
      AdjustTime(1, 0, 0);                                                                    // +1 hour
      break;
    case 11:                                                                                  // Button "DOWN"
      AdjustTime(-1, 0, 0);                                                                   // -1 hour
      break;
    case 12:                                                                                  // Button "LEFT"
      AdjustTime(0, -1, 0);                                                                   // -1 minute
      break;
    case 13:                                                                                  // Button "RIGHT"
      AdjustTime(0, 1, 0);                                                                    // +1 minute
      break;
    case 14:                                                                                  // Button "POWER"
      ToggleIRpower();
      break;
    case 15: 
      learningMode = false;                                                                  // Button "OK"
//    Serial.println(EnteredDigits.length());   
      if (EnteredDigits.length() == 6) {ReworkInputString(EnteredDigits); EnteredDigits = "";}
      else  { Tekstprintln("⚠ Need 6 digits (HHMMSS) before OK");  }
      break;
    default:
      sprintf(sptext, "Button '%s' not yet assigned", ButtonName.c_str());
      Tekstprintln(sptext);
      break;
  }
}

//--------------------------------------------                                                //
// IR-RECEIVER Adjust current time by hours/minutes/seconds
//--------------------------------------------
void AdjustTime(int DeltaHours, int DeltaMinutes, int DeltaSeconds)
{
 timeinfo.tm_hour += DeltaHours;
 timeinfo.tm_min  += DeltaMinutes;
 timeinfo.tm_sec  += DeltaSeconds;
 time_t t = mktime(&timeinfo);
 if(DS3231Installed)   SetDS3231Time();
 Displaytime();       
}

//--------------------------------------------                                                //
// IR-RECEIVER Turn On or off Remote control
// Turns off after 60 seconds
//--------------------------------------------
void ToggleIRpower()
{
 static bool powerState = false;                                                              //
 powerState = !powerState;
 if (powerState)
  {
   IR_PowerOnstate = true;                                                                    // React on IR-remote input
   Tekstprintln("✓ IR-remote is ON");
   IR_StartTime = millis();
  }
 else
  {
    Tekstprintln("X IR-remote is OFF");
    IR_PowerOnstate = false;                                                                  // Do not react on IR-remote input
  } 
}