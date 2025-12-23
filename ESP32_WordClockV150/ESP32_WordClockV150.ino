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
 Changes V122: Optimized SetDS3231Time()  if(DS3231Installed)   SetDS3231Time(); Removed stray input from IRremote and added ONOFF
 Changes V123: For colour input, option F in menu, decimal input 0-9 is possible. F0900 = 0x00FF0000 = red 
 Changes V124: Failed to Improved WIFI reconnection after power failure
 Changes V125: Improved WIFI reconnection after power failure. Removed in EveryMinuteUpdate(void) CheckRestoreWIFIconnectivity();  Not neccesarry. Using configTime(); for NTP 
 Changes V126: Placed setTimezone(); after configTime(..);. If DS3231 on --> BUILTIN LED13 Off.  Cosmetic changes
 Changes V127: Replaced  ESP.getFreeSketchSpace() with UPDATE_SIZE_UNKNOWN Added disableCore0WDT() and disableCore1WDT() during upload
               Added WiFi.setSleep(WIFI_PS_NONE) before starting server. Re-enabled watchdogs after upload completes or fails
 Changes V128: Using <ESPAsyncWebServer.h>. Better to use when uploading bin files > 1 Mb
 Changes V129: Using Neopixel. Optimized code so option Y LEDtest runs again from the web page if (inputMessage.equalsIgnoreCase("Y") ) OptionYRainbow = true; etc.
 Changes V130: Removed old coding and use NEOPIXEL and use ESPAsyncWebServer. 203222 l/s again instead of 140000
 Changes V131: Added H04 tiny IR-remote
 Changes V132: Cleanup code
 Changes V133: Added in CheckRestoreWIFIconnectivity() Failsafe NoConnectionCounter that restart after 30 minutes no IP when WIFI is On 
 Changes V134: Added in CheckRestoreWIFIconnectivity() Loopcounter<10 and network available. --> Will reboot. Minor typos and cleanups
 Changes V135: Optimized used of CheckRestoreWIFIconnectivity() and check every hour instead of every mniute
 Changes V136: Added Log viewer webpage url/log 
 Changes V137: No use
 Changes V138: Solve restart ESP32 sprintf --> snprintf
 Changes V139: Logviewer -> 500 lines
 Changes V140: Changed wifi case WL_CONNECTED:
 Changes V141: Ringbuffer with String and Free heap is working OK
 Changes V142: Ringbuffer to char** 
 Changes V143: Ringbuffer to char** optimized
 Changes V144: Keeps all logs in PSRAM  Limits /tekstprint to 500 lines for safe browser display  Allows /tekstdownload for full log retrieval
 Changes V145: no log file anymore --> solved while (!Serial && ( (millis()-Tick) < 500)) { delay(50); }
 Changes V146: Works OK
 Changes V147: Line numbers removed and code cleaning in Logviewer.h 
 Changes V148: Replace snprintf(sptext, sizeof(sptext) with Tekstprintf. Added Option Y { and G from web page menu not possible
 Changes V149: Flashing warning text in web page menu. Optimized bool GetNTPtime(bool printit) . Mem.TimeMinute used to turn off time printing every minute
               Major cleanup and bugfixes
 Changes V150: Stable version


*********************
How to compile: 
Install ESP32 boards
Board: ESP32 core version >3.2.0 
Partition Scheme: With FAT
Pin Numbering: By Arduino pin (default)     -- > with EdsoftLED both can be used
               By GPIO number (legacy).     -- > When using NEOpixel
               Adafruit_NeoPixel/esp.c:78:(.text.espShow+0x55): undefined reference to `digitalPinToGPIONumber' when using By Arduino pin
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
                      #if ESP_ARDUINO_VERSION >= ESP_ARDUINO_VERSION_VAL (5, 2, 0)            // Use EdSoftLED with ESP32 compiler.
#define USEEDSOFTLED  // Use EdsoftLED >= V1.7.0 for SK6812. Saves 200-500 bytes compared with NeoPixel. NeoPixel is OK
                      #endif
                      #ifdef USEEDSOFTLED
#include <EdSoftLED.h>         // https://github.com/ednieuw/EdSoftLED for LED strip WS2812 or SK6812 
                      #else
#include <Adafruit_NeoPixel.h> // https://github.com/adafruit/Adafruit_NeoPixel  *** USE GPIO NUMBERING and compile By GPIO number (legacy) 
                      #endif
#include <Preferences.h>
#include <NimBLEDevice.h>      // For BLE communication. !!!Use NimBLE version 2.x.x  https://github.com/h2zero/NimBLE-Arduino
#include <WiFi.h>              // Used for web page 
#include <esp_sntp.h>          // for NTP
#include <esp_wps.h>           // For WPS
#include <ESPAsyncWebServer.h> // Used for webpage  https://github.com/ESP32Async 
#include <Update.h>            // For Over-the-Air (OTA)
#include <ESPmDNS.h>           // To show BLEname in router
#include <DNSServer.h>         // For the web page to enter SSID and password of the WIFI router 
#include <Wire.h>              // Ter zijner tijd Wire functies gaan gebruiken. Staan al klaar in de code 
#include <RTClib.h>            // Used for connected DS3231 RTC // Reference https://adafruit.github.io/RTClib/html/class_r_t_c___d_s3231.html
#include <Encoder.h>           // For rotary encoder
#include <Keypad.h>            // For 3x1 membrane keypad instead of rotary encoder by Mark Stanley & Alexander Brevig 
#include <IRremote.hpp>        // IR remote control

#include "ClockFaces.h"
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
 encoderPinB  = D2, //5,          // D2 left (labeled CLK on decoder) *** D8 on large PCB !!! no interrupt pin (Use GPIO pin numbering for rotary encoder lib)  
 encoderPinA  = D3, //6,          // D3 right (labeled DT on decoder) on interrupt pin
 clearButton  = D4, //7,          // D4 switch (labeled SW on decoder)
 IRReceiverPin= D4,               // D4 Infrared receiver pin instead of rotary encoder
 LED_PIN      = D5, //8,          // D5 / GPIO 8 Pin to control colour SK6812/WS2812 LEDs (replace D5 with 8 for NeoPixel lib)
 EmptyD6      = D6,               // D6 Empty
 EmptyD7      = D7,               // D7 Empty
 encoderPinBL = D8,               // D8 *** If large PCB is used change pin D2 to D8 !!
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
                     #ifdef USEEDSOFTLED
EdSoftLED LEDstrip ;                                                                          // Use EdSoftLED 
EdSoftLED LED6812strip = EdSoftLED(NUM_LEDS, LED_PIN, SK6812WGRB);
EdSoftLED LED2812strip = EdSoftLED(NUM_LEDS, LED_PIN, WS2812GRB);
bool UsedEDSOFTLED = true;
                      #else
// LED_PIN = 8;  // bug in Neopixel library. Does not translate D5 to GPIO 8
Adafruit_NeoPixel LEDstrip;
Adafruit_NeoPixel LED6812strip = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRBW + NEO_KHZ800); // NEO_RGBW
Adafruit_NeoPixel LED2812strip = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRB  + NEO_KHZ800); // NEO_RGB NEO_GRB
bool UsedEDSOFTLED = false;
                      #endif

#define CLEARLEDSTARTUP 9999
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
String   KeypadString         = "";

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
//#define NO_LED_SEND_FEEDBACK_CODE                                                             // Do not flash the BUILTIN LED on Arduino
String* ButtonNames;
byte NOOFBUTTONS;
String EnteredDigits     = "";                                                                // Store typed digits InputString
String ButtonLNames[]    = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9",                 // Large IR-remote (H03)
                           "UP", "DOWN", "LEFT", "RIGHT", "POWER", "OK","ONOFF"};             // Runtime button info                                     
String ButtonTNames[]    = {"MIN-1", "MIN+1","UUR-1","UUR+1","POWER","ONOFF"};                // Small IR-remote (H04)
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

struct IRRemoteStorage
{
  uint16_t learnedRemoteAddress  = 0;
  byte     learnedRemoteProtocol = 0;
  bool     remoteIdentified      = false;
  StoredButtonMapping buttons[30];                                                            // NOOFBUTTONS = 15 but space for 15 more
  int      Checksum              = 0;
} IRMem;
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
#include "LogViewer.h"
WiFiEventId_t wifiEventHandler;                                                               // To stop the interrupts or callbacks triggered by WiFi.onEvent(WiFiEvent);, you need to deregister the event handler.
bool WIFIwasConnected       = false;                                                          // Is WIFI connected?
bool InApMode               = false;
volatile bool ntpJustSynced = false;
const char* AP_SSID         = "StartWordclock";
const char* AP_PASSWORD     = "wordclock";
AsyncWebServer server(80);
DNSServer dnsServer;
bool shouldReboot           = false;
bool OptionYRainbow         = false;
byte NoConnectionCounter    = 0;                                                              // Count times minutes with no WIFI connection
//----------------------------------------                                                    //
// WPS
//----------------------------------------
#define ESP_WPS_MODE      WPS_TYPE_PBC
#define ESP_MANUFACTURER  "ESPRESSIF"
#define ESP_MODEL_NUMBER  "ESP32"
#define ESP_MODEL_NAME    "ESPRESSIF IOT"
#define ESP_DEVICE_NAME   "ESP STATION"
static esp_wps_config_t config;

//----------------------------------------                                                    //
// Ring Buffer Logger
// circular Log buffer variables                                                                                               //
//--------------------------------------------------------------------------------------------------------------------
char*  LogBuffer      = nullptr;                                                                                       // Circular buffer
size_t LogBufferSize  = 0;                                                                                            // Total buffer size
size_t LogWritePos    = 0;                                                                                            // Write pointer
bool   LogWrapped     = false;                                                                                        // Did we wrap?

//------------------------------------------------------------------------------              //
// Common
//----------------------------------------
#define   MAXTEXT 255
char      sptext[MAXTEXT];                                                                    // For common print use 
bool      LEDsAreOff        = false;                                                          // If true LEDs are off except time display
bool      NoTextInLeds      = false;                                                          // Flag to control printing of the text in function ColorLeds()
int       Previous_LDR_read = 512;                                                            // The actual reading from the LDR + 4x this value /5
uint16_t  MilliSecondValue  = 10;                                                             // The duration of a second  minus 1 ms. Used in Demo mode
uint32_t  Loopcounter       = 100;                                                            // ESP will restart if <10. so start with a larger start value
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
  byte MCUrestarted     = 0;                                                                  // No of times WIFI reconnected 
  byte LoopRebooted     = 0;                                                                  // No of times WIFI rebooted
  byte ByteFuture1      = 0;                                                                  // For future use 
  byte TimeMinute       = 0;                                                                  // For future use  
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
  byte WIFInoConnection = 0;                                                                  //    
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

//--------------------------------------------                                                //
// Menu
//0        1         2         3         4
//1234567890123456789012345678901234567890----  
bool LastMenuformat = true;                                                                   // Small=true of full=false menu
const char *menu[] = {
 "A SSID, B Password, C BLE beacon name",
 "D Date (D15012021), T Time (T132145)",
 "E Timezone (E<-02>2 or E<+01>-1)",
 "F Own colour (FWWRRGGBB or Fwrgb)",
 "G Scan WIFI networks",
 "H H01 rotary H02 buttons H03/04 remote",
 "} Learn IR remote, + Fast BLE",  
 "I Info menu, II long menu",
 "J DS3231 RTC module On/Off",
 "K LDR reads/s, K1 Time/min On/Off", 
 "N Display off between Nhhhh (N2208)",
 "O Display On/Off, P StatusLED On/Off", 
 "Q Display choice (Q0-9), ~ Changing",
 "R Reset settings,  @ Restart",
 "S Slope, L Min, M Max (S50 L5 M200)",
 "Unnn Demo (U200 msec), Y LED test",
 ") HETISWAS  On/Off, ( EdSoft On/Off",
 "#nnn Selftest,  RTC: ! See, & Update",
 "W WIFI, X NTP, Z WPS, CCC BLE",
 "Ed Nieuwenhuijs Dec 2025" };
 
 const char *menusmall[] = {
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
 while (!Serial && ( (millis()-Tick) < 1502)) { delay(250); }                                 // Wait max 1.5 sec to establish serial connection
 InitStorage();                                                                               // Load settings from storage and check validity   
 StartLeds();
 printHeaps();                                                                                // LED RainbowCycle  
 while (!Serial && ( (millis()-Tick) < 3000) ) { LEDstartup(green); delay(499); }             // Wait max 3 sec to establish serial connection
 LEDstartup(capri); Tekstprintln("Serial started\nStored settings loaded\nLED strip started");// InitStorage and StartLEDs must be called first
    #ifdef ARDUINO_BOARD
 Tekstprint("Compiled on: "); Tekstprintln(ARDUINO_BOARD);
    #endif
 Mem.MCUrestarted++;                                                                          // MCU Restart counter     
 if(Mem.MCUrestarted>5) { Reset();  ResetCredentials(); }                                     // If the MCU restarts during Setup() so often Reset all 
 StoreStructInFlashMemory();                                                                  // 
 Tekstprintlnf( "Using %s LED library", UsedEDSOFTLED?"EDSOFTLED":"NEOPIXEL");                                                           // 
 if(Mem.UseRotary==1) {LEDstartup(pink);  InitRotaryMod(); Tekstprintln("Rotary available"); }// Start the Rotary encoder
 if(Mem.UseRotary==2) {LEDstartup(grass); InitKeypad3x1(); Tekstprintln("Keypad available"); }// Start the Keypad 3x1 
 if(Mem.UseRotary >2) {LEDstartup(white);Start_IRreceiver();Tekstprintln("IRremote available");}// Start IR remote Mem.UseRotary = 3 or 4                                                                                     
 if(Mem.BLEOn)        {LEDstartup(blue);StartBLEService(); Tekstprintln("BLE started"); }     // Start BLE service 
 if(Mem.WIFIOn)       {LEDstartup(purple); ConnectWIFI();  Tekstprintln("WIFI started");}     // Start WIFI and optional NTP if Mem.WIFIOn = 1 
 InitTimeSystem();     LEDstartup(yellow); Tekstprintln("Time system started");               // Initialize RTC + DS3231 sync
 Previous_LDR_read = ReadLDR();                                                               // Set the initial LDR reading 
 GetTijd(true);                                                                               // Get the time and do not print it
 LEDstartup(green);                                                                           // Set the status LED to green
 SWversion();                                                                                 // Print the menu + version 
 Mem.MCUrestarted = 0;                                                                        // Startup went well; Set MCUrestart counter to 0    
 StoreStructInFlashMemory();                                                                  // 
 delay(250);
 LEDstartup(CLEARLEDSTARTUP);                                                                 // Erase the startup LEDs
 Tekstprintln("Setup finished");
 msTick = millis();                                                                           // Start the seconds loop counter
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
 if (OptionYRainbow ) { ReworkInputString("Y"); OptionYRainbow=false;}                        // The LED library’s timing can get interrupted by background WiFi interrupts, causing flicker in the strip.
 if (Mem.UseRotary==1) RotaryEncoderCheck();
 if (Mem.UseRotary==2) Keypad3x1Check();                                                      // --> Every100mSecCheck
                                  #ifdef ONEWIREKEYPAD3x4   
 OnewireKeypad3x4Check(); 
                                  #endif  //ONEWIREKEYPAD3x4
                                  #ifdef ONEWIREKEYPAD3x1   
 OnewireKeypad3x1Check(); 
                                  #endif  //ONEWIREKEYPAD3x1
 if (ntpJustSynced) CheckandPrintNTPsynced();                                 
}
//--------------------------------------------                                                //
// COMMON Update routine 
// Performs tasks every second
//--------------------------------------------
void EverySecondCheck(void)
{
 static bool Toggle = 0;
 uint32_t msLeap = millis() - msTick;                                                         // 
 if (msLeap % 100) Every100mSecCheck();
 if (msLeap > 999)                                                                            // Every second enter the loop
  {
   msTick = millis();
   GetTijd(false);                                                                            // Get the time for the seconds 
   Toggle = !Toggle;                                                                          // Used to turn On or Off Leds
   UpdateStatusLEDs(Toggle);
   SetSecondColour();                                                                         // Set the colour per second of 'IS' and 'WAS' 
   DimLeds(TestLDR);                                                                          // Every second an intensity check and update from LDR reading 
   if (shouldReboot) { delay(3000);   ESP.restart(); }                                        // After finish OTA update restarts after 3 seconds. 1 second is too fast
   if (timeinfo.tm_min != lastminute) EveryMinuteUpdate();                                    // Enter the every minute routine after one minute; 
   Loopcounter=0;
 }  
}
//--------------------------------------------                                                //
// COMMON Update routine 
  // Performs tasks every 0.1 second
//--------------------------------------------
void Every100mSecCheck(void)
{
 if (Mem.UseRotary==3 || Mem.UseRotary==4) IrReceiverDecode();
}
//--------------------------------------------                                                //
// COMMON Update routine done every minute
//-------------------------------------------- 
void EveryMinuteUpdate(void)
{   
static byte NoConnectionCounter = 0;  
lastminute = timeinfo.tm_min;  
 CheckRestoreWIFIconnectivity();                                                              // Check if WIFI is still connected and if not restore it
 if (IR_PowerOnstate && ((millis() - IR_StartTime) > 290000) ) ToggleIRpower();               // Turn off Power after 300 seconds 
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
 CheckRestoreWIFIconnectivity();                                                              // Check if WIFI is still connected and if not restore it
 Mem.NVRAMmem[lasthour] =(byte)((SumLDRreadshour / NoofLDRreadshour?NoofLDRreadshour:1));     // Update the average LDR readings per hour. Avoids dividing by zero
 SumLDRreadshour  = 0;
 NoofLDRreadshour = 0;                                                                         // Check if WIFI is still connected and if not restore it
 if (Mem.RandomDisplay == 2) { ChangeRandomDisplay(); SetSecondColour();}                     // If RandomDisplay is ON change the display choice
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
 StoreStructInFlashMemory();                                                                  // Update Mem struct once a day to store Mem.NVRAM measurements
}

//--------------------------------------------                                                //
// COMMON Update routine for the status LEDs
//-------------------------------------------- 
void UpdateStatusLEDs(bool Toggle)
{
 if(Mem.StatusLEDOn)   
   {
    SetStatusLED((Toggle && WiFi.localIP()[0]==0) * 20, 
                 (Toggle && WiFi.localIP()[0]!=0) * 20 , 
                 (Toggle && deviceConnected) * 20);
    SetPCBLED09(   Toggle * 10);                                                              // Left LED
    SetPCBLED10((!Toggle) * 10);                                                              // Right LED
    SetNanoLED13((!Toggle) * (!Mem.UseDS3231 * 50));                                          // LED on ESP32 board. IF DS3231 in use then it is off
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
//  rgbLedWrite(512-Red,512-Green,512-Blue);
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
 Mem.WIFInoConnection = 0;                                                                    // Restarts after 30 minutes no connection 
 Mem.BLEOn            = 1;                                                                    // default BLE On
 Mem.UseBLELongString = 0;                                                                    // Default off. works only with iPhone/iPad with BLEserial app
 Mem.NTPOn            = 0;                                                                    // NTP default off
 Mem.WIFIOn           = 0;                                                                    // WIFI default off
 Mem.MCUrestarted     = 0;                                                                    // MCU Restarts during Setup() counter caused by unknown errors during startup
 Mem.LoopRebooted     = 0;                                                                    // Reboots cause by less than 10 loops /second. Program hangs or severly delayed
 Mem.WIFIcredentials  = NOT_SET;                                                              // Status of the WIFI connection
 //Mem.UseRotary      = 0;    // Do not erase this setting with a reset                       // Use the rotary coding
 Mem.DCF77On          = 0;                                                                    // Default off
 Mem.UseDS3231        = 0;                                                                    // Default off
 //Mem.LEDstrip       = 0;    // Do not erase this setting with a reset                       // 0 = SK6812, 1=WS2812
 Previous_LDR_read    = ReadLDR();                                                            // Read LDR to have a start value. max = 4096/8 = 255
 MinPhotocell         = Previous_LDR_read;                                                    // Stores minimum reading of photocell;
 MaxPhotocell         = Previous_LDR_read;                                                    // Stores maximum reading of photocell;                                            
 TestLDR              = 0;                                                                    // If true LDR display is printed every second
 for (int i = 0; i < NOOFBUTTONS; i++)  IRMem.buttons[i].learned = 0;
 Tekstprintln("**** Reset of preferences ****"); 
 //ResetCredentials();
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
void Tekstprint(char const *tekst)    { if(Serial) Serial.print(tekst);  SendMessageBLE(tekst); AddLog(tekst);} //sptext[0]=0; } 
void Tekstprintln(char const *tekst)  { snprintf(sptext, sizeof(sptext),"%s\n",tekst); Tekstprint(sptext); }
void TekstSprint(char const *tekst)   { printf(tekst); } //sptext[0]=0;}                      // printing for Debugging purposes in serial monitor 
void TekstSprintln(char const *tekst) { snprintf(sptext, sizeof(sptext),"%s\n",tekst); TekstSprint(sptext); }

//--------------------------------------------                                                //
// Usage:
// Tekstprintlnf("Log buffer allocated: %u bytes", (unsigned) LogBufferSize);
// Tekstprintf("Log buffer allocated: %u bytes\n", (unsigned) LogBufferSize);
//--------------------------------------------
void Tekstprintf(const char* fmt, ...) 
 {
  va_list args;
  va_start(args, fmt);
  vsnprintf(sptext, sizeof(sptext), fmt, args);
  va_end(args);
  Tekstprint(sptext);
 }

void Tekstprintlnf(const char* fmt, ...) 
 {
  va_list args;
  va_start(args, fmt);
  vsnprintf(sptext, sizeof(sptext), fmt, args);
  va_end(args);
  Tekstprintln(sptext);
}

//--------------------------------------------                                                //
// COMMON Print web menu page and BLE menu
// 0 = text to print, 1 = header of web page with menu, 2 = footer of web page
// html_info must be empty before starting: --> html_info[0] = 0; 
//--------------------------------------------
void WTekstappend(char const *tekst, char const *prefixtekst, char const *suffixtekst, bool newline) 
{
 if (newline) { snprintf(sptext, sizeof(sptext), "%s\n", tekst); } 
 else {         snprintf(sptext, sizeof(sptext), "%s", tekst);   }
 if(Mem.MCUrestarted == 0) Tekstprint(sptext);                                                // During a normal startup the menu is printed during startup proces.
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
 InitLogBuffer();     Tekstprintln("Ringbuffer initialysed");                                 // For Log lines Ringbuffer
 if( Mem.Checksum != 25065)
   {
    snprintf(sptext, sizeof(sptext),"Checksum (25065) invalid: %d\n Resetting to default values",Mem.Checksum); 
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
 snprintf(sptext, sizeof(sptext),"Mem.Checksum = %d",Mem.Checksum); 
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
 if(Small) {for (uint8_t i = 0; i < sizeof(menusmall) / sizeof(menusmall[0]);                   WTekstprintln(menusmall[i++]) ); }
 else      {for (uint8_t i = 0; i < sizeof(menu) / sizeof(menu[0]);                             WTekstprintln(menu[i++]) ); }                                     
 PrintLine(35);
 snprintf(sptext, sizeof(sptext),"Display off between: %02dh - %02dh",Mem.TurnOffLEDsAtHH, Mem.TurnOnLEDsAtHH);  WTekstprintln(sptext);
 PrintDisplayChoice(false);                                                                     WTekstprintln(sptext);
 snprintf(sptext, sizeof(sptext), "RandomDisplay is %s", Mem.RandomDisplay==1 ? "On/min" : 
                                        Mem.RandomDisplay==2 ? "On/hour" : "Off");              WTekstprintln(sptext);//
 snprintf(sptext, sizeof(sptext),"Slope: %d     Min: %d     Max: %d ",
                 Mem.LightReducer, Mem.LowerBrightness,Mem.UpperBrightness);                    WTekstprintln(sptext);
 if(!Small) {snprintf(sptext, sizeof(sptext),"SSID: %s", Mem.SSID);                             WTekstprintln(sptext); }
// snprintf(sptext, sizeof(sptext),"Password: %s", Mem.Password);                               WTekstprintln(sptext);
 snprintf(sptext, sizeof(sptext),"BLE name: %s", Mem.BLEbroadcastName);                         WTekstprintln(sptext,"<span class=\"verdana-red\">","</span>");
 snprintf(sptext, sizeof(sptext),"IP-address: %d.%d.%d.%d", WiFi.localIP()[0], WiFi.localIP()[1], 
                                           WiFi.localIP()[2], WiFi.localIP()[3] );              WTekstprint(sptext);
 snprintf(sptext, sizeof(sptext),"/update");                                                    WTekstprintln(sptext," <a href=" , "> /update</a>");                                            
 if(!Small) {snprintf(sptext, sizeof(sptext),"Timezone:%s", Mem.Timezone);                      WTekstprintln(sptext); }
 snprintf(sptext, sizeof(sptext),"%s %s %s %s", Mem.WIFIOn?"WIFI=On":"WIFI=Off", 
                               Mem.NTPOn? "NTP=On":"NTP=Off",
                               Mem.BLEOn? "BLE=On":"BLE=Off",
                               Mem.UseBLELongString? "FastBLE=On":"FastBLE=Off" );              WTekstprintln(sptext);
 char fftext[20];              
 if(!Small) {sprintf(fftext,"%s", Mem.UseDS3231?" DS3231=On":" DS3231=Off"); }
 if(!Small) {snprintf(sptext, sizeof(sptext),"%s %s",Mem.UseRotary==0 ?"No Rotary, ":
                        Mem.UseRotary==1 ?"Rotary = ON":
                        Mem.UseRotary==2 ?"Membrane = ON":
                        Mem.UseRotary==3 ?"IR-remote = ON":
                        Mem.UseRotary==4 ?"Ir-remote = ON":"NOP",fftext);                       WTekstprintln(sptext); }                           
 if(!Small) { snprintf(sptext, sizeof(sptext),"%s strip with %d LEDs (switch %%)", 
                 Mem.LEDstrip==0?"SK6812":Mem.LEDstrip==1?"WS2812":"NOP",(int) NUM_LEDS);       WTekstprintln(sptext); }
  if(!Small) {snprintf(sptext, sizeof(sptext),"Software: %s",FILENAAM);                         WTekstprintln(sptext);}  // VERSION);
  if(!Small) {snprintf(sptext, sizeof(sptext),"ESP32 Arduino core version: %d.%d.%d", 
          ESP_ARDUINO_VERSION_MAJOR,ESP_ARDUINO_VERSION_MINOR,ESP_ARDUINO_VERSION_PATCH);       WTekstprintln(sptext); }
 GetTijd(false);                                                                              // Get the time and store it in the proper variables
//  PrintRTCTime(); Tekstprintln("");                                                                               
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
 if(InputString.length()> 40){Tekstprintln("Input string too long (max40)\n"); return;}       // If garbage return
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
        snprintf(sptext, sizeof(sptext), "SSID set: %s", Mem.SSID);
        Mem.WIFIcredentials = NOT_SET;    
        WIFIwasConnected = false;    
        } 
      else snprintf(sptext, sizeof(sptext), "**** Length fault. Use between 4 and 30 characters ****");
      break;
      
    case 'B':                                                                                 // Password setting
      if(InputString.equals("BBBB")) 
      {
        snprintf(sptext, sizeof(sptext), "%s,**** Length fault. Use between 5 and 40 characters ****", Mem.Password);
        break;
      }
      validLength = (len > 4 && len < 40);
      if(validLength) 
        {
        InputString.substring(1).toCharArray(Mem.Password, len);
        snprintf(sptext, sizeof(sptext), "Password set: %s\n Enter @ to reset ESP32 and connect to WIFI and NTP\n WIFI and NTP are turned ON", Mem.Password);
        Mem.NTPOn = Mem.WIFIOn = 1;                                                           // Turn both on
        Mem.WIFIcredentials = NOT_SET;
        WIFIwasConnected = false;
        } 
      else snprintf(sptext, sizeof(sptext), "**** Length fault. Use between 5 and 40 characters ****");
      break;

    case 'C':                                                                                 // BLE settings
      if(InputString.equals("CCC")) 
       {
        Mem.BLEOn = 1 - Mem.BLEOn;
        snprintf(sptext, sizeof(sptext), "BLE is %s after restart", Mem.BLEOn ? "ON" : "OFF");
        break;
       }
      validLength = (len > 4 && len < 30);
      if(validLength) 
       {
        InputString.substring(1).toCharArray(Mem.BLEbroadcastName, len);
        snprintf(sptext, sizeof(sptext), "BLE broadcast name set: %s", Mem.BLEbroadcastName);
        Mem.BLEOn = 1;
       } 
      else snprintf(sptext, sizeof(sptext), "**** Length fault. Use between 4 and 30 characters ****");
      break;
      
    case 'D':                                                                                 // Date entry
      if(len == 9) 
       {
        timeinfo.tm_mday = (int)SConstrainInt(InputString, 1, 3, 0, 31);
        timeinfo.tm_mon  = (int)SConstrainInt(InputString, 3, 5, 0, 12) - 1;
        timeinfo.tm_year = (int)SConstrainInt(InputString, 5, 9, 2000, 9999) - 1900;
        if(DS3231Installed)   SetDS3231Time();
        else snprintf(sptext, sizeof(sptext), "No external RTC module detected");
       } 
      else snprintf(sptext, sizeof(sptext), "****\nLength fault. Enter Dddmmyyyy\n****");
      break;
      
    case 'E':                                                                                 // Time zone setting
      validLength = (len > 2);
      if(validLength) 
       {
        InputString.substring(1).toCharArray(Mem.Timezone, len);
        snprintf(sptext, sizeof(sptext), "Timezone set: %s", Mem.Timezone);
       } 
      else snprintf(sptext, sizeof(sptext), "**** Length fault. Use more than 2 characters ****");
      break;
      
    case 'F': 
      snprintf(sptext, sizeof(sptext),"Stored colour: 0X%08" PRIX32, Mem.OwnColour);      
      if (len==5)                                                                             // Decimal number input (0–9 per digit) 
         {
          String Input = InputString.substring(1, 5);                                         // and map each digit to a corresponding byte intensity from 0x00 to 0xFF
          uint32_t Color = 0;                                                                 // 
          for (int i = 0; i < Input.length(); i++) 
             {
              int Digit = Input[i] - '0';                                                     // convert char to int 0–9
              uint8_t level = map(Digit, 0, 9, 0, 255);                                       // scale 0–9 → 0–255
              Color = (Color << 8) | level;                                                   // pack into 32-bit value
             }   
          LetterColor = Mem.OwnColour = Color;
         }
      else if (len == 9) 
         {                                                                                    // Hexadecimal number input (00 - FF per colour) 
          String HexPart = InputString.substring(1, 9);
          LetterColor = Mem.OwnColour = strtoul(HexPart.c_str(), nullptr, 16);
         }
       else 
         {
          snprintf(sptext, sizeof(sptext),"Enter Fwwrrggbb hexadecimal (0 - F)\nOr Fwrgb decimal 0 - 9\nStored: 0X%08" PRIX32, Mem.OwnColour);
          break;
         }
      snprintf(sptext, sizeof(sptext), "**** Own colour changed ****\nFont colour now: 0X%08" PRIX32, Mem.OwnColour);
      Tekstprintln(sptext);
      LedsOff();                                                                              // Clear the display
      SetSecondColour();                                                                      // Set the colour per second of 'IS' and 'WAS' 
      Displaytime();
      sptext[0]=0;                                                                            // Suppress a second print of sptext
      break;

    case 'G':                                                                                 // Scan WIFI stations
      if(len == 1) 
       {
        //ScanWIFI();
        CheckforWIFINetwork(true);
        if(WIFIwasConnected) WiFi.reconnect();
       } 
      else snprintf(sptext, sizeof(sptext), "**** Length fault. Enter G ****");
      break;

    case 'H':                                                                                 // Use rotary encoder
      if(len == 3) 
       {
        Mem.UseRotary = (byte)SConstrainInt(InputString, 2, 0, 4);                           // keep the range between 0 and 4; H00 - H04
        if(Mem.UseRotary > 4) Mem.UseRotary = 0;
        Tekstprintln("*** Restart with @ to activate Rotary, keyboard or IR remote ***");    // 
        if(Mem.UseRotary > 0)  {Mem.NTPOn = 0;              Mem.UseDS3231 = 1;}              // Configure related settings based on rotary use 
        else                   {Mem.WIFIOn = Mem.NTPOn = 1; Mem.UseDS3231 = 0;}
        } 
      else Tekstprintln("Error **** Enter H00 (none), H01=Rotary, H02=Membrane, H03 large IR-remote, H04 tiny IR-remote ****");

      sprintf(sptext, "\n Use of rotary encoder is %s\n"
                        "         Use of keypad is %s\n"
                        "Use of large IR-remote is %s\n"
                        " Use of tiny IR-remote is %s", 
                        Mem.UseRotary == 1 ? "ON" : "OFF", Mem.UseRotary == 2 ? "ON" : "OFF",
                        Mem.UseRotary == 3 ? "ON" : "OFF", Mem.UseRotary == 4 ? "ON" : "OFF");
      Tekstprintln(sptext);      
      sprintf(sptext, "Use DS3231 is %s, WIFI is %s, NTP is %s\n *** Restart clock with @ ***", 
                       Mem.UseDS3231 ? "ON" : "OFF", Mem.WIFIOn ? "ON" : "OFF", Mem.NTPOn ? "ON" : "OFF");     
      if(Mem.UseRotary >2) { while(IrReceiver.decode())  IrReceiver.resume();  }              // Clear all pending commands    
      break;

    case 'I':                                                                                 // Menu
      SWversion(len == 1);                                                                    // true for small menu, false for full menu
      break;
      
    case 'J':                                                                                 // Use DS3231 RTC module
      if(len == 1) 
      {
        Mem.UseDS3231 =  1 - Mem.UseDS3231;
        Mem.NTPOn     = (1 - Mem.UseDS3231);
        if(Mem.WIFIOn == 0) Mem.NTPOn = 0;                                                    // If WIFI is Off then No NTP
        snprintf(sptext, sizeof(sptext), "Use DS3231 is %s, WIFI is %s, NTP is %s", 
                Mem.UseDS3231 ? "ON" : "OFF", Mem.WIFIOn ? "ON" : "OFF", Mem.NTPOn ? "ON" : "OFF");
       } 
      else snprintf(sptext, sizeof(sptext), "**** Length fault. Enter J ****");
      break;
      
    case 'K':  
      if (len == 1)
        {                                                                               // Test LDR
         TestLDR = 1 - TestLDR;
         snprintf(sptext, sizeof(sptext), "TestLDR: %s", TestLDR ? "On\n   Bits, Out, loops per second and time" : "Off\n");
        }
       if(InputString.equalsIgnoreCase("K1"))                                                           // Delete WIFI settings only
        {
         Mem.TimeMinute = 1 - Mem.TimeMinute;
         snprintf(sptext, sizeof(sptext), "Time display per minute is: %s", Mem.TimeMinute ? "Off\n" : "On\n");  
        } 
      break;
      
    case 'L':                                                                                 // Lower brightness
      validLength = (len > 1 && len < 5);
      if(validLength) 
       {
        Mem.LowerBrightness = (byte)SConstrainInt(InputString, 1, 0, 255);
        snprintf(sptext, sizeof(sptext), "Lower brightness: %d bits", Mem.LowerBrightness);
       } 
      else snprintf(sptext, sizeof(sptext), "**** Input fault. \nEnter Lnnn where n between 1 and 255");
      break;
      
    case 'M':                                                                                 // Max brightness
      validLength = (len > 1 && len < 5);
      if(validLength) 
       {
        Mem.UpperBrightness = SConstrainInt(InputString, 1, 1, 255);
        snprintf(sptext, sizeof(sptext), "Upper brightness changed to: %d bits", Mem.UpperBrightness);
       } 
      else snprintf(sptext, sizeof(sptext), "**** Input fault. \nEnter Mnnn where n between 1 and 255");
      break;
      
    case 'N':                                                                                 // Turn off display between hours
      snprintf(sptext, sizeof(sptext), "**** Length fault N. ****");
      if(len == 1) { Mem.TurnOffLEDsAtHH = Mem.TurnOnLEDsAtHH = 0;  } 
      else if(len == 5) 
       {
        Mem.TurnOffLEDsAtHH = (byte)InputString.substring(1, 3).toInt();
        Mem.TurnOnLEDsAtHH  = (byte)InputString.substring(3, 5).toInt();
       }
      Mem.TurnOffLEDsAtHH = _min(Mem.TurnOffLEDsAtHH, 23);
      Mem.TurnOnLEDsAtHH  = _min(Mem.TurnOnLEDsAtHH, 23);
      snprintf(sptext, sizeof(sptext), "Display is OFF between %2d:00 and %2d:00", Mem.TurnOffLEDsAtHH, Mem.TurnOnLEDsAtHH);
      break;
      
    case 'O':                                                                                 // Turn On/Off Display
      if(len == 1) 
       {
        LEDsAreOff = !LEDsAreOff;
        snprintf(sptext, sizeof(sptext), "Display is %s", LEDsAreOff ? "OFF" : "ON");
        if(LEDsAreOff) { ClearScreen(); }
        else 
         {
          Tekstprintln(sptext);
          lastminute = 99;                                                                    // Force display update
          sptext[0]=0;                                                                        // Suppress a second print of sptext
         }
       } 
      else snprintf(sptext, sizeof(sptext), "**** Length fault O. ****");
      break;
      
    case 'P':                                                                                 // Status LEDs On/Off
      if(len == 1) {
        Mem.StatusLEDOn = !Mem.StatusLEDOn;
        UpdateStatusLEDs(0);
        snprintf(sptext, sizeof(sptext), "StatusLEDs are %s", Mem.StatusLEDOn ? "ON" : "OFF");
      } else snprintf(sptext, sizeof(sptext), "**** Length fault P. ****");
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
        snprintf(sptext, sizeof(sptext), "\nSSID and password deleted. \nWIFI, NTP and BLE is On\n Enter @ to restart");
        break;
       }
      if(len == 1)                                                                            // Set to default settings
        {
        Reset();
        snprintf(sptext, sizeof(sptext), "\nReset to default values: Done");
        lastminute = 99;                                                                      // Force a minute update
        Displaytime();
       } 
      else snprintf(sptext, sizeof(sptext), "**** Length fault R. ****");
      break;
      
    case 'S':                                                                                 // Slope factor for brightness
      validLength = (len > 1 && len < 5);
      if(validLength) 
       {
        Mem.LightReducer = (byte)SConstrainInt(InputString, 1, 1, 255);
        snprintf(sptext, sizeof(sptext), "Slope brightness changed to: %d%%", Mem.LightReducer);
       } 
      else snprintf(sptext, sizeof(sptext), "**** Input fault. \nEnter Snnn where n between 1 and 255");
      break;
      
    case 'T':                                                                                 // Time setting
      if(len == 7) 
       {
        timeinfo.tm_hour = (int)SConstrainInt(InputString, 1, 3, 0, 23);
        timeinfo.tm_min  = (int)SConstrainInt(InputString, 3, 5, 0, 59);
        timeinfo.tm_sec  = (int)SConstrainInt(InputString, 5, 7, 0, 59);
        if(DS3231Installed)   SetDS3231Time();
        else snprintf(sptext, sizeof(sptext), "No external RTC module detected");
       } 
      else snprintf(sptext, sizeof(sptext), "**** Length fault. Enter Thhmmss ****");
      break;
      
    case 'U':                                                                                 // Demo mode
      if(len == 1) 
      { 
        if(Demo)  {   lastminute = 99;  Demo = false; }                                       // Force a minute update and turn off demo mode
        else { MilliSecondValue = 200;  Demo = true;  }                                       // If Demomode was off turn it on with 200 msec delay 
        snprintf(sptext, sizeof(sptext), "Demo mode: %s, %d msec/min", Demo ? "ON" : "OFF", MilliSecondValue);    
       } 
      else if(len > 1 && len < 6) 
       {
        MilliSecondValue = InputString.substring(1, 5).toInt();
        Demo = true;
        snprintf(sptext, sizeof(sptext), "Demo mode: %s, %d msec/min", Demo ? "ON" : "OFF", MilliSecondValue);
       } 
      else snprintf(sptext, sizeof(sptext), "**** Length fault U. Demo mode (Unnn or U) ****");
      break;
      
    case 'W':                                                                                 // WIFI toggle
      if(len == 1) 
       {
        Mem.WIFIOn = 1 - Mem.WIFIOn;
        Mem.NTPOn = Mem.WIFIOn;                                                               // If WIFI is off turn NTP also off
        snprintf(sptext, sizeof(sptext), "WIFI is %s after restart", Mem.WIFIOn ? "ON" : "OFF");
       } 
      else snprintf(sptext, sizeof(sptext), "**** Length fault. Enter W ****");
      break;
      
    case 'X':                                                                                 // NTP toggle
      if(len == 1) 
       {
        Mem.NTPOn = 1 - Mem.NTPOn;
        snprintf(sptext, sizeof(sptext), "NTP is %s after restart", Mem.NTPOn ? "ON" : "OFF");
       } 
      else snprintf(sptext, sizeof(sptext), "**** Length fault. Enter X ****");
      break;
      
    case 'Y':                                                                                 // Play lights
      Play_Lights();
      lastminute = 99;                                                                        // Force a minute update
      snprintf(sptext, sizeof(sptext), "**** Play Lights");
      break;
      
    case 'Z':                                                                                 // Start WPS
      snprintf(sptext, sizeof(sptext), "**** Start WPS on your router");
      WiFi.onEvent(WiFiEvent);
      WiFi.mode(WIFI_STA);
      Tekstprintln("Starting WPS");
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
        snprintf(sptext, sizeof(sptext), "Zelftest: %s", Zelftest ? "ON" : "OFF");
        Selftest();
        Displaytime();
       } 
      else if(len > 1 && len < 6) 
       {
        MilliSecondValue = InputString.substring(1, 5).toInt();
        Zelftest = 1 - Zelftest;
        snprintf(sptext, sizeof(sptext), "Zelftest: %s", Zelftest ? "ON" : "OFF");
        Selftest(MilliSecondValue);
        Displaytime();
       } 
      else snprintf(sptext, sizeof(sptext), "**** Length fault #. Demo mode (#nnnn or #) ****");
      break;
      
    case '$':                                                                                 // Fire display
      FireDisplay = 1 - FireDisplay;
      snprintf(sptext, sizeof(sptext), "FireDisplay: %s", FireDisplay ? "On" : "Off");
      ClearScreen();
      Displaytime();
      break;
      
    case '%':                                                                                 // LED strip type
      if(len == 1) 
       {
        Mem.LEDstrip = 1 - Mem.LEDstrip;
        snprintf(sptext, sizeof(sptext), "LED strip is %s after restart", Mem.LEDstrip ? "WS2812" : "SK6812");
       } 
      else snprintf(sptext, sizeof(sptext), "**** Length fault . ****");
      break;

     case '^':                                                                                 // *** Empty ***
      if(len == 1) 
       {
       // snprintf(sptext, sizeof(sptext), "--- %s ---", Mem.ByteFutureX ? "OFF" : "ON");
       snprintf(sptext, sizeof(sptext), "**** No Use option . ****");
       } 
      else snprintf(sptext, sizeof(sptext), "**** Length fault . ****");
      break;

    case '&':                                                                                 // Force NTP update
      if(len == 1) 
       {
        if(GetNTPtime(false))  SetDS3231Time();                                               // Only set time in DS3231 if time is valid
        SetRTCTime();
        PrintAllClockTimes();
        sptext[0] =0 ;
       } 
      else snprintf(sptext, sizeof(sptext), "**** Length fault &. ****");
      break;
      
    case '(':  
      Mem.EdSoftLEDSOn = !Mem.EdSoftLEDSOn;
      snprintf(sptext, sizeof(sptext), "EDSOFT is %s", Mem.EdSoftLEDSOn ? "ON" : "OFF");
      lastminute = 99;                                                                        // Force a minute update
      break;

    case ')':
      Mem.HetIsWasOff = !Mem.HetIsWasOff;
      snprintf(sptext, sizeof(sptext), "HET IS WAS is %s", Mem.HetIsWasOff ? "OFF" : "ON");
      lastminute = 99;                                                                        // Force a minute update
      break;

    case '{':  
      if (Mem.UseRotary==3 || Mem.UseRotary==4)
       {
         StartIRLearning();                                                                   // Initializes learning mode,Resets all button data, Prompts for first button
         snprintf(sptext, sizeof(sptext), "Learning IR finished");
       }
      else snprintf(sptext, sizeof(sptext), "IR remote is OFF (Turn it on with H03 or H04 )\nOr no IR remote is installed");
      break;

    case '}':                                                                                 // *** Empty ***
      if(len == 1) 
       {
       // snprintf(sptext, sizeof(sptext), "--- %s ---", Mem.ByteFutureX ? "OFF" : "ON");
       snprintf(sptext, sizeof(sptext), "**** No Use option . ****");
       } 
      else snprintf(sptext, sizeof(sptext), "**** Length fault . ****");
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
      snprintf(sptext, sizeof(sptext), "Random display is %s", Mem.RandomDisplay==1 ? "ON/min" : Mem.RandomDisplay==2 ? "ON/hour" : "OFF");
      break;

    case '+':                                                                                 // BLE string toggle
      if(len == 1) 
       {
        Mem.UseBLELongString = 1 - Mem.UseBLELongString;
        snprintf(sptext, sizeof(sptext), "Fast BLE is %s", Mem.UseBLELongString ? "ON" : "OFF");
       } 
      else snprintf(sptext, sizeof(sptext), "**** Length fault. Enter + ****");
      break;
      
    case '_':                                                                                 // Store in flash
      if(len > 1 && len < 4) 
       {
        byte ff = (byte)InputString.substring(1, 3).toInt();
        snprintf(sptext, sizeof(sptext), "No use: %d", ff);
        Tekstprintln(sptext);
       }
      break;

    case '=':                                                                                 // Print permanent Mem memory
      if(len == 1)  PrintMem(); 
      else snprintf(sptext, sizeof(sptext), "**** Length fault. Enter = ****");
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
        else snprintf(sptext, sizeof(sptext), "No external RTC module detected");
       }   
      else snprintf(sptext, sizeof(sptext), "**** Length fault. Enter Thhmmss ****");
      break;
  }
  Tekstprintln(sptext);
  StoreStructInFlashMemory();                                                                 // Update EEPROM
 }
InputString = "";
lastminute = 99;                                                                              // Force a minute update
}
//--------------------------------------------                                                //
// COMMON Print Display choice
// Default print the text
//--------------------------------------------
void PrintDisplayChoice() {PrintDisplayChoice(true);}
void PrintDisplayChoice(bool PrintIt)
{
 byte dp = Mem.DisplayChoice;
 if (dp>9) dp=9;
 snprintf(sptext, sizeof(sptext),"Display choice: %s",
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
  char Method[20];
  switch(Im)
  {
    case 0: strcpy(Method, "None");            break;
    case 1: strcpy(Method, "Rotary");          break;
    case 2: strcpy(Method, "Membrane");        break;
    case 3: strcpy(Method, "Large IR-remote"); break;
    case 4: strcpy(Method, "Tiny Ir-remote");  break;     
   default: strcpy(Method, "Error");
  }
  snprintf(sptext, sizeof(sptext), "Time inputMethod: %s", Method);
  Tekstprintln(sptext);
}
//--------------------------------------------                                                //
// COMMON Print permanent Mem memory
//--------------------------------------------
void PrintMem(void)
{
 PrintLine(35); 
 printHeaps(); 
 PrintAllMappings();                                                                          // Print the IR remote keys 
 PrintDisplayChoice();
 Tekstprintlnf(" TurnOffLEDsAtHH: %d",Mem.TurnOffLEDsAtHH);                 
 Tekstprintlnf("  TurnOnLEDsAtHH: %d",Mem.TurnOnLEDsAtHH);                  
 Tekstprintlnf("  LanguageChoice: %d",Mem.LanguageChoice);                  
 Tekstprintlnf("    LightReducer: %d",Mem.LightReducer);                    
 Tekstprintlnf(" LowerBrightness: %d",Mem.LowerBrightness);                 
 Tekstprintlnf(" UpperBrightness: %d",Mem.UpperBrightness);                 
 for (int i=0;i<12;i++) { Tekstprintf("%03d ",Mem.NVRAMmem[i]); }   Tekstprintln("");
 for (int i=12;i<24;i++){ Tekstprintf("%03d ",Mem.NVRAMmem[i]); }   Tekstprintln("");
 Tekstprintlnf("   DisplayChoice: %d",Mem.DisplayChoice);                                                                                      
 Tekstprintlnf("           BLEOn: %s",Mem.BLEOn ? "ON" : "OFF");            
 Tekstprintlnf("           NTPOn: %s",Mem.NTPOn ? "ON" : "OFF");            
 Tekstprintlnf("          WIFIOn: %s",Mem.WIFIOn ? "ON" : "OFF");           
 Tekstprintlnf("     StatusLEDOn: %s",Mem.StatusLEDOn ? "ON" : "OFF");      
 Tekstprintlnf("    MCUrestarted: %d",Mem.MCUrestarted );                   
 Tekstprintlnf("    LoopRebooted: %d",Mem.LoopRebooted );                   
 Tekstprintlnf("     ByteFuture1: %d",Mem.ByteFuture1 );                    
 Tekstprintlnf(" Time per minute: %s",Mem.TimeMinute ? "OFF" : "ON");        
 Tekstprintlnf("         DCF77On: %s",Mem.DCF77On  ? "ON" : "OFF");         
 Tekstprintlnf("       UseRotary: %d",Mem.UseRotary);                       
 PrintTimeInputMethod(Mem.UseRotary);
 Tekstprintlnf("       UseDS3231: %s",Mem.UseDS3231 ? "ON" : "OFF");        
 Tekstprintlnf("       LED strip: %s", Mem.LEDstrip?"WS2812":"SK6812" );    
 Tekstprintlnf("      FiboChrono: %s",Mem.FiboChrono ? "FIBO" : "CHRONO");  
 Tekstprintlnf("          NoExUl: %s", Mem.NoExUl ? "ON" : "OFF");          
 byte wc = Mem.WIFIcredentials; 
 Tekstprintlnf(" WIFIcredentials: %s", wc==0? "Not SET" : wc==1? "SET" : wc==2? "SET&OK": 
                                   wc==3? "in AP not SET":"Unknown code");   
 Tekstprintlnf("      IntFuture2: %d",Mem.IntFuture2 );                     
 Tekstprintlnf("      IntFuture3: %d",Mem.IntFuture3 );                     
 Tekstprintlnf("      HET IS WAS: %s", Mem.HetIsWasOff ? "OFF" : "ON");     
 Tekstprintlnf("       EDSOFT is: %s", Mem.EdSoftLEDSOn ? "ON" : "OFF");    
 Tekstprintlnf("   RandomDisplay: %s", Mem.RandomDisplay? "ON" : "OFF");    
 Tekstprintlnf("WIFInoConnection: %d", Mem.WIFInoConnection);               
 Tekstprintlnf("UseBLELongString: %s",Mem.UseBLELongString?"ON":"OFF");     
 Tekstprintlnf("     Font colour: 0X%08" PRIX32, Mem.OwnColour);            
 Tekstprintlnf("    DimmedLetter: 0X%08" PRIX32,Mem.DimmedLetter);          
 Tekstprintlnf("      BackGround: 0X%08" PRIX32,Mem.BackGround);            
 Tekstprintlnf("            SSID: %s",Mem.SSID);                            
 // Tekstprintlnf("BackGround: %s",Mem.Password);                           
 Tekstprintlnf("BLEbroadcastName: %s",Mem.BLEbroadcastName);                
 Tekstprintlnf("        Timezone: %s",Mem.Timezone);                        
 Tekstprintlnf("        Checksum: %d",Mem.Checksum);                        
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
 ES;    Laatzien(Delayms);   IST;     Laatzien(Delayms); WAR;    Laatzien(Delayms); GENAU; Laatzien(Delayms); MZEHN; Laatzien(Delayms);  MFUNF; Laatzien(Delayms);    
 VIERTEL; Laatzien(Delayms); ZWANZIG; Laatzien(Delayms); KURZ;   Laatzien(Delayms); VOR;   Laatzien(Delayms); NACH;  Laatzien(Delayms);  HALB;  Laatzien(Delayms);
 FUNF;  Laatzien(Delayms);   EINS;    Laatzien(Delayms); VIERDE; Laatzien(Delayms); ZEHN;  Laatzien(Delayms); ZWOLF; Laatzien(Delayms);  DREI;  Laatzien(Delayms);
 NEUN;  Laatzien(Delayms);   ACHTDE;  Laatzien(Delayms); SECHS;  Laatzien(Delayms); SIEBEN;Laatzien(Delayms); ZWEI;  Laatzien(Delayms);  ELFDE; Laatzien(Delayms); 
 UHR;   Laatzien(Delayms); 
                      #endif  //DE144CLOCK                      
 Tekstprintln("*");      
}
//
//--------------------------------------------                                                //
// CLOCK Say the time and load the LEDs 
// with the proper colour and intensity
//--------------------------------------------
void Displaytime(void)
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
  // snprintf(sptext, sizeof(sptext),"LDR:%3d Avg:%3d (%3d-%3d) Out:%3d=%2d%% Loop(%ld) ",
  //      LDRread,LDRavgread,MinPhotocell,MaxPhotocell,OutPhotocell,(int)(OutPhotocell/2.55),Loopcounter);    
 if (Mem.UseDS3231)   snprintf(sptext, sizeof(sptext),"LDR:%3d->%2d%% %5lu l/s %0.0fC ",
               LDRread,(int)(OutPhotocell*100/255),Loopcounter,RTCklok.getTemperature()); 
 else                 snprintf(sptext, sizeof(sptext),"LDR:%3d->%2d%% %5lu l/s M %0.2f kB ",
               LDRread,(int)(OutPhotocell*100/255),Loopcounter, (float)ESP.getFreeHeap()/1024);   
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
 if (!NoTextInLeds && !Mem.TimeMinute && strlen(Tekst) > 0 )
     { snprintf(sptext, sizeof(sptext),"%s ",Tekst); Tekstprint(sptext); }                                      // Print the text  
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
void ShowLeds(void) { LEDstrip.show(); }
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
// Used during Setup(). if LEDcolour == 9999 -> clear the display
//--------------------------------------------
void LEDstartup(uint32_t LEDColour)
{
 static uint32_t ProgressLedNr = 0;
 if (LEDColour == CLEARLEDSTARTUP)  
    for(int n = ProgressLedNr-1; n>=0; n--)
      {
       ColorLed(n,0); 
       ShowLeds(); 
       delay(200);
      }
 else
   {
    ColorLed(ProgressLedNr++,LEDColour); 
    ShowLeds();   
    SetStatusLED(Cred(LEDColour),Cgreen(LEDColour),Cblue(LEDColour));  
   }
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
snprintf(sptext, sizeof(sptext),"LED strip is %s", Mem.LEDstrip?"WS2812":"SK6812" ); Tekstprintln(sptext);
LEDstrip.begin();
LEDstrip.setBrightness(16);  
LedsOff();                                                                                    // Set initial brightness of LEDs  (0-255)  
ShowLeds();
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
 snprintf(sptext, sizeof(sptext),"Max brightness: %3d%%",Mem.LightReducer);
 Tekstprintln(sptext);
}
//--------------------------- Time functions --------------------------
 time_t now;
 const char* monthNames[] = { "Jan","Feb","Mar","Apr","May","Jun",
                              "Jul","Aug","Sep","Oct","Nov","Dec"};
//--------------------------------------------                                                //
// TIME Initialize time system (DS3231 + NTP + internal RTC)
// First set system time to compile time, then to DS3231, then to NTP if available
//--------------------------------------------------------------------
void InitTimeSystem(void)
{
  SetSystemTimeToCompileTime();                                                               //   CompileTime = DateTime(__DATE__, __TIME__);    
  DS3231Installed = IsDS3231I2Cconnected();
  snprintf(sptext, sizeof(sptext), "External RTC module %s found", DS3231Installed ? "IS" : "NOT");
  Tekstprintln(sptext);
  if (DS3231Installed) RTCklok.begin();
  if (DS3231Installed ) //&& Mem.UseDS3231)
   {
    GetDS3231Time(true);
    SetSystemTime(mktime(&timeinfo));                                                         // Sync ESP32 RTC from DS3231 to have a better time
   }
  if (Mem.NTPOn && WiFi.isConnected())
   {
   GetNTPtime(false);
   SetSystemTime(mktime(&timeinfo));                                                          // Sync ESP32 RTC from NTP
   if (DS3231Installed)
     {
      DateTime Inow = RTCklok.now();
      if (RTCklok.lostPower()) SetDS3231Time();
      if (timeinfo.tm_year > (2024 - 1900) && Inow.year() < 2025) SetDS3231Time();            // Sync DS3231 if time is years too old
     }
  }
  else
  {
    time(&now);
    localtime_r(&now, &timeinfo);
  }
}

//--------------------------------------------                                                //                                               
// TIME Set system time to compile time using basic functions
//--------------------------------------------
void SetSystemTimeToCompileTime(void)
{
 struct tm Tm;
 time_t Now;
 
 const char* MonthNames = "JanFebMarAprMayJunJulAugSepOctNovDec";                             // Parse __DATE__ and __TIME__ macros
 char MonthStr[4];
 sscanf(__DATE__, "%s %d %d", MonthStr, &Tm.tm_mday, &Tm.tm_year);                            // Parse "Nov 08 2025"
 sscanf(__TIME__, "%d:%d:%d", &Tm.tm_hour, &Tm.tm_min, &Tm.tm_sec);                           // Parse "14:30:45"
 Tm.tm_year -= 1900;                                                                          // Years since 1900
 Tm.tm_mon = (strstr(MonthNames, MonthStr) - MonthNames) / 3;                                 // Month 0-11
 Tm.tm_isdst = -1;                                                                            // Auto-determine DST
 Now = mktime(&Tm);                                                                           // Convert to time_t
 struct timeval Tv = { .tv_sec = Now, .tv_usec = 0 };
 settimeofday(&Tv, NULL);                                                                      // settimeofday(&tv, nullptr) This is the actual system clock update
 localtime_r(&Now, &timeinfo);                                                                 // This is independent of setting the system time. It converts the UNIX timestamp into the timeinfo struct so your program can use it.
 snprintf(sptext, sizeof(sptext), "System time set to compile time: %s %s", __DATE__, __TIME__);
 Tekstprintln(sptext);
 PrintRTCTime();  Tekstprintln(sptext);
}

//--------------------------------------------                                                //
// TIME Get current time from best available source
//--------------------------------------------------------------------
time_t GetTijd(bool printit)
{
  if (Mem.UseDS3231 && DS3231Installed)     { GetDS3231Time(false);  }
  else if (Mem.NTPOn && WiFi.isConnected()) { getLocalTime(&timeinfo);  }
  else  {    time(&now);                      localtime_r(&now, &timeinfo);  }
  time_t t = mktime(&timeinfo);
  if (printit) {PrintRTCTime();  Tekstprintln(sptext);}
  return t;
}

//--------------------------------------------                                                //
// NTP Return local time as RTClib DateTime
//--------------------------------------------------------------------
DateTime GetLocalDateTime(){ return DateTime(GetTijd(false));}

//--------------------------------------------                                                //
// NTP print the NTP time for the timezone set 
// return true if time is valid
//--------------------------------------------------------------------
bool GetNTPtime(bool printit)
{
 sntp_set_sync_mode(SNTP_SYNC_MODE_IMMED);
 sntp_restart();
 if (getLocalTime(&timeinfo, 1000))                                                           // wait up to 1 seconds
    {  
      if (timeinfo.tm_year >= (2020 - 1900)) 
        {
         if (printit) PrintNTPtime();
         return true;
        }
    }
 return false;
}

//--------------------------------------------                                                //
// NTP Print current local (NTP) time
//--------------------------------------------------------------------
void PrintNTPtime(void)
{
  getLocalTime(&timeinfo);
  PrintRTCTime();
}

//--------------------------------------------                                                //
// NTP print the NTP UTC time 
//--------------------------------------------------------------------
void PrintUTCtime(void)
{
  time_t now;
  time(&now);

  struct tm* UTCtime = gmtime(&now);
  if (!UTCtime) return;   // safety guard

  snprintf(sptext, sizeof(sptext),  "%02d-%s-%04d %02d:%02d:%02d ",
           UTCtime->tm_mday, monthNames[UTCtime->tm_mon],   // tm_mon = 0–11
           UTCtime->tm_year + 1900, UTCtime->tm_hour,
           UTCtime->tm_min, UTCtime->tm_sec);

  Tekstprint(sptext);
}


//--------------------------------------------                                                //
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

//--------------------------------------------                                                //
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

//--------------------------------------------                                                //
// DS3231 RTC ESP32 -> DS3231
// Set time in module DS3231
//--------------------------------------------------------------------
void SetDS3231Time(void)
{
  DateTime tNow(timeinfo.tm_year + 1900, timeinfo.tm_mon + 1, timeinfo.tm_mday,
                timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
  RTCklok.adjust(tNow);
  Tekstprint("Time set in DS3231 RTC module: ");
  PrintDS3231Time();
  if(!Mem.UseDS3231 && !Mem.NTPOn ) Tekstprintln("*** Not using DS3231");
}

//--------------------------------------------                                                //
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

//--------------------------------------------                                                //
// DS3231 Print DS3231 time
//--------------------------------------------------------------------
void PrintDS3231Time(void)
{
 DateTime Inow = RTCklok.now();
 snprintf(sptext, sizeof(sptext), "%02d-%s-%04d %02d:%02d:%02d ",
          Inow.day() , monthNames[Inow.month()-1] , Inow.year(),
          Inow.hour(), Inow.minute(), Inow.second());
 Tekstprintln(sptext);
}

//--------------------------------------------                                                //
// RTC prints the ESP32 internal RTC time to serial
//--------------------------------------------
void PrintRTCTime(void)
{
 snprintf(sptext, sizeof(sptext),"%02d-%s-%04d %02d:%02d:%02d ",
           timeinfo.tm_mday,  monthNames[timeinfo.tm_mon],   // tm_mon is 0–11
           timeinfo.tm_year + 1900, timeinfo.tm_hour,
           timeinfo.tm_min,          timeinfo.tm_sec);
  WTekstprint(sptext, "<!--", " -->");   // Hidden in HTML
}

//--------------------------------------------                                                //
// RTC prints the ESP32 internal RTC time to serial
//--------------------------------------------
void PrintRTCdecTime(void)
{
 snprintf(sptext, sizeof(sptext),"%02d/%02d/%04d %02d:%02d:%02d ", 
     timeinfo.tm_mday,timeinfo.tm_mon+1,timeinfo.tm_year+1900,
     timeinfo.tm_hour,timeinfo.tm_min,timeinfo.tm_sec);
 WTekstprint(sptext,"<!--"," -->");                                                         // Hide the time on the HTML page <!-- This text won't appear at all -->
}

//--------------------------------------------                                                //
// RTC Fill sptext with time
//--------------------------------------------
void PrintTimeHMS(){ PrintTimeHMS(2);}                                                        // print with linefeed
void PrintTimeHMS(byte format)
{
 snprintf(sptext, sizeof(sptext),"%02d:%02d:%02d",timeinfo.tm_hour,timeinfo.tm_min,timeinfo.tm_sec);
 switch (format)
 {
  case 0: break;
  case 1: Tekstprint(sptext); break;
  case 2: Tekstprintln(sptext); break;  
 }
}

//--------------------------------------------                                                //
// RTC Set time in ESP32 using global timeinfo struct
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
  Tekstprint("ESP32 RTC time set: ");
  PrintTimeHMS();
//  Displaytime(); 
}

//--------------------------------------------                                                //
// CLOCK Print all the times available 
//--------------------------------------------
void PrintAllClockTimes(void)
{
 Tekstprint("\n Clock time: ");    PrintRTCTime();
 if(WiFi.localIP()[0] != 0)                                                                   // If no WIFI then no NTP time printed
   {
    Tekstprint("\n   NTP time: ");  PrintNTPtime();
    Tekstprint("\n   UTC time: ");  PrintUTCtime();
   }
 if(DS3231Installed)
   {
    Tekstprint("\nDS3231 time: ");  PrintDS3231Time();
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
       LEDstrip.setPixelColor(XY(x, y), g, r, b, 0);                                          // Add W=0
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
    {deviceConnected = true; Tekstprintln("BLE Connected"   );}
 void onDisconnect(NimBLEServer* pServer, NimBLEConnInfo& connInfo, int reason) override 
    {deviceConnected = false; Tekstprintln("BLE Disconnected" );}
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
// After 1/4 hour the BLE connection is disconnected
//------------------------------
void CheckBLE(void)
{

if (deviceConnected && !oldDeviceConnected) 
  {
   oldDeviceConnected = deviceConnected;
   BLEConnectedSince = millis();                                                              // Mark time of last connection 1/4 hour after this time the BLE connection will disconnect
  }

if (!deviceConnected && oldDeviceConnected)                                                   // If device is disconnected start advertising
  {
   delay(300);
   pServer->startAdvertising();
   TekstSprint("Start advertising\n");
   oldDeviceConnected = deviceConnected;
   BLEConnectedSince = 0;                                                                     // Reset BLE connection timer
  }

if ((deviceConnected && BLEConnectedSince > 0) && (millis() - BLEConnectedSince > 900000))    // Disconnect if connected longer than 15 minutes inactivity 
  {
   auto connHandles = NimBLEDevice::getServer()->getPeerDevices();                            // Vector of uint16_t handles
   if (!connHandles.empty()) 
     {
      uint16_t conn_handle = connHandles[0];
      int rc = ble_gap_terminate(conn_handle, BLE_ERR_REM_USER_CONN_TERM);                    // Reason = 0x13 = user terminated
      if (rc == 0) { TekstSprint("Disconnected BLE client after 1/4 hour\n"); } 
      else {snprintf(sptext, sizeof(sptext),"BLE disconnect failed, error: %d\n", rc); TekstSprint(sptext);}
       BLEConnectedSince = 0;
      }
  }
 if(ReceivedMessageBLE.length()>0)
   {
    SendMessageBLE(ReceivedMessageBLE);
    String BLEtext = ReceivedMessageBLE.c_str();
    ReceivedMessageBLE = "";
    ReworkInputString(BLEtext); 
    BLEConnectedSince = millis();                                                             // Mark time of last connection 1/4 hour after this time the BLE connection will disconnect
   }
}

//--------------------------------------------                                                //
// WIFI WIFIEvents
//Yes — using WiFi.onEvent() is definitely the better and more reliable way to handle Wi-Fi connection state on the ESP32.
// ✅ Why WiFi.onEvent() is better
// 1. It’s fully asynchronous
//--------------------------------------------
void WiFiEvent(WiFiEvent_t event)
{
 // snprintf(sptext, sizeof(sptext),"[WiFi-event] event: %d  : ", event);  Tekstprint(sptext);
 WiFiEventInfo_t info;
 static bool LostPrinted = false;
 switch (event) 
     {
 /*
 ARDUINO_EVENT_WIFI_STA_CONNECTED → Connected to AP (no IP yet)
ARDUINO_EVENT_WIFI_STA_GOT_IP → DHCP success (ready for servers, OTA)
ARDUINO_EVENT_WIFI_STA_DISCONNECTED → Lost connection
ARDUINO_EVENT_WIFI_STA_STOP → WiFi off
 */     
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
              snprintf(sptext, sizeof(sptext),"WiFi lost connection.");                                          // Reason: %d",info.wifi_sta_disconnected.reason); 
              Tekstprintln(sptext);
              LostPrinted = true;
             }
            //WiFi.reconnect(); //is checked in EveryMinuteUpdate()
            break;
        case ARDUINO_EVENT_WIFI_STA_AUTHMODE_CHANGE:
            Tekstprintln("Authentication mode of access point has changed");
            break;
        case ARDUINO_EVENT_WIFI_STA_GOT_IP:
        /*
✔ Wi-Fi connected
✔ IP address assigned
✔ Network parameters available (IP, subnet, gateway, DNS)
✔ Triggered exactly when it happens
        */
            snprintf(sptext, sizeof(sptext),"Connected to : %s",WiFi.SSID().c_str());
            Tekstprintln(sptext);
            snprintf(sptext, sizeof(sptext), "Obtained IP address: %d.%d.%d.%d", WiFi.localIP()[0], WiFi.localIP()[1], WiFi.localIP()[2], WiFi.localIP()[3] );
            Tekstprintln(sptext);         
            strcpy(Mem.SSID,      WiFi.SSID().c_str());
            strcpy(Mem.Password , WiFi.psk().c_str());                                         // Store SSID and password
            Mem.NTPOn        = 1;                                                              // NTP On
            Mem.WIFIOn       = 1;                                                              // WIFI On  
            delay(100);
            WIFIwasConnected = true;                                                            // Now we know the SSID ans password are correct and we can reconnect
            Mem.WIFIcredentials = SET_AND_OK;
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
            snprintf(sptext, sizeof(sptext), "WPS Successfull, stopping WPS and connecting to: %s: ", WiFi.SSID().c_str());
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
            snprintf(sptext, sizeof(sptext),"WPS_PIN = %s",wpspin2string(info.wps_er_pin.pin_code).c_str());
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
            snprintf(sptext, sizeof(sptext),"Client disconnected.");                                            // Reason: %d",info.wifi_ap_stadisconnected.reason); 
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
    sptext[0] = 0;                                                                            // Clear sptext buffer
}
//--------------------------------------------                                                //
// WIFI Check for WIFI Network 
// Check if WIFI network to connect to is available
// Return true if available
//--------------------------------------------
 bool CheckforWIFINetwork(void)         { return CheckforWIFINetwork(true);}
 bool CheckforWIFINetwork(bool PrintIt)
 {
  if(PrintIt)  Tekstprintln("Scanning for networks");
  int n = WiFi.scanNetworks();                                                                // WiFi.scanNetworks will return the number of networks found
  if (n == 0)                { if(PrintIt) Tekstprintln("No networks found"); return false;} 
  if (n == WIFI_SCAN_FAILED) { if(PrintIt) Tekstprintln("All scan attempts failed - No WIFI connection");  return false;   }
  else 
    { 
     snprintf(sptext, sizeof(sptext),"%d: networks found",n); 
     if(PrintIt) Tekstprintln(sptext);
     for (int i = 0; i < n; ++i)                                                              // Print SSID and RSSI for each network found
      {
        snprintf(sptext, sizeof(sptext),"%2d: %25s %3d %1s",i+1,WiFi.SSID(i).c_str(),(int)WiFi.RSSI(i),(WiFi.encryptionType(i) == WIFI_AUTH_OPEN)?" ":"*");
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
    snprintf(sptext, sizeof(sptext),"%d networks found",n);   Tekstprintln(sptext);
    Tekstprintln("Nr | SSID                             | RSSI | CH | Encryption");
    for(int i = 0; i < n; ++i) 
      {
       snprintf(sptext, sizeof(sptext),"%2d | %-32.32s | %4d | %2d | ",i + 1, 
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
// WIFI Check if WIFI is still connected and if not restore it
//-------------------------------------------- 
void CheckRestoreWIFIconnectivity(void)
{
 if(!Mem.WIFIOn || InApMode) return;                                                          // If in WIFI APmode or there is nothing to check
 if(!WIFIwasConnected)
        { if(CheckforWIFINetwork(false) ) StartWIFI_NTP(); }                                  // If there was no WIFI at start up start a WIFI connection       
 if(Mem.WIFIOn && WIFIwasConnected)                                                           // If WIFI switch is On and there was a connection.
   {
    if(WiFi.localIP()[0] == 0) 
       {
        if(Mem.WIFIcredentials == SET_AND_OK)  WiFi.reconnect();                              // If connection lost and WIFI is used reconnect
        if(WiFi.localIP()[0] != 0)                                                            // WIFI connection is established
          {
          snprintf(sptext, sizeof(sptext), "Reconnected to IP address: %d.%d.%d.%d", WiFi.localIP()[0], WiFi.localIP()[1], WiFi.localIP()[2], WiFi.localIP()[3] );
          Tekstprintln(sptext);
          }
       }
    }
    
 static uint16_t RebootTime = 30; //hours
 if(WiFi.localIP()[0] == 0 && Mem.WIFIcredentials == SET_AND_OK )                                                                                               // Extra failsafe.If for some reason there is no IP address after 30 sec. Then restart
   { 
     NoConnectionCounter++; 
     snprintf(sptext, sizeof(sptext),"No connection for %d hour, will restart in %d hours",NoConnectionCounter, RebootTime  - NoConnectionCounter); 
     Tekstprintln(sptext);
    }
 else NoConnectionCounter = 0;

 if  (NoConnectionCounter > RebootTime ) 
     { 
      Tekstprintln(" ---> Will reboot");
      shouldReboot = true;                                                                    // ESP will restart
      Mem.WIFInoConnection++;                                                                 // Will store the no of reboots
      StoreStructInFlashMemory();
     }   

if(Loopcounter<10)                                                                            // Then something is really wrong
  {
  if(CheckforWIFINetwork(true)) 
    { 
     Tekstprintln("Loopcounter<10 and network available. --> Will reboot"); 
     shouldReboot = true;                                                                     // ESP will restart
     Mem.LoopRebooted++;                                                                      // will store the no of reboots. 
     StoreStructInFlashMemory();
    }
  }
}

//--------------------------------------------                                                //
// WIFI Start WIFI connection and NTP service
//--------------------------------------------
bool StartWIFI_NTP(void)
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

 int tryDelay = 5000;                                                                         // Will try for about 50 seconds (10 x 5sec)
 int numberOfTries = 10;
 while (true)                                                                                 // Wait for the WiFi event
  {
    switch(WiFi.status()) 
    {
     case WL_NO_SSID_AVAIL:
          Tekstprintln("[WiFi] SSID not found (Unexpected error)\n Is the router turned off?");
          if(Mem.WIFIcredentials == SET_AND_OK)
            {
             Tekstprintln("[WiFi] Waiting 20 seconds for router to start");
             for( int n=0; n<10; n++)  { delay(2000); LEDstartup(red); }
            }
          if (WiFi.status() == WL_CONNECTED)  
            {
             WIFIwasConnected = true;                                                         // Now we know the SSID ans password are correct and we can reconnect
             CheckRestoreWIFIconnectivity();
            break;
            } 
          else return false;         
     case WL_CONNECT_FAILED:
          Tekstprint("[WiFi] Failed - WiFi not connected! Reason:? \n Reset the clock with option R and re-enter SSID and Password.");
          return false;
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
          WIFIwasConnected = true;                                                            // Now we know the SSID ans password are correct and we can reconnect
          break;
     default:
     Tekstprintlnf("[WiFi] WiFi Status: %d", WiFi.status());
          break;
    } 
  LEDstartup(orange);                                                                         // Orange colour in WS2812 and SK6812 LEDs
  if (WIFIwasConnected) break;       
  if(numberOfTries <= 0)
    {
     Tekstprintln("[WiFi] Failed to connect to WiFi!");
     WiFi.disconnect();                                                                       // Use disconnect function to force stop trying to connect
     switch(Mem.WIFIcredentials)
        {    
         case NOT_SET:
             Tekstprintln("Check SSID and password or turn WIFI in menu off with option W");
             break;               
         case SET:
             Tekstprintln("Check your SSID name and password.\nRe-enter your password with option B in the menu. Password is now deleted");    
             strcpy(Mem.Password,"");                                                         // This will force a restart is AP mode. PWD can not be checked in menu. SSID can be checked
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
        return false;
     } 
  else 
      { 
       delay(tryDelay);  
       numberOfTries--;    
      }
  }
if (!WIFIwasConnected) return false;                                                               // If WIFI connection fails -> return
//  snprintf(sptext, sizeof(sptext), "IP Address: %d.%d.%d.%d", WiFi.localIP()[0], WiFi.localIP()[1], WiFi.localIP()[2], WiFi.localIP()[3] );  Tekstprintln(sptext); 
if(Mem.NTPOn )
  { 
   initSNTP();
   if(wait4SNTP())  Tekstprintln("NTP is On and synced");
   else             Tekstprintln("NTP is On but NOT synced");
   }   
 if(Mem.WIFIOn) WebPage();                                                                    // Show the web page if WIFI is on
 Tekstprintln("Web page started");
 return true;
// WiFi.removeEvent(wifiEventHandler);  You can leave it on because it undertakes no actions. Every minute there is a connectivity check  // Remove the WIFI event handler
}

//--------------------------------------------                                                //
// NTP functions
//--------------------------------------------
void NTPnotify(struct timeval* tv) { ntpJustSynced = true; }                                  // --- NTP time synchronized --- is printed later
void CheckandPrintNTPsynced(void)
{
 ntpJustSynced = false;
 struct tm tm;
 time_t now = time(nullptr);
 localtime_r(&now, &tm);
 Tekstprintlnf("--- NTP time synchronized --- %02d:%02d:%02d", tm.tm_hour, tm.tm_min, tm.tm_sec);
}

void setTimezone() { setenv("TZ", Mem.Timezone, 1);  tzset(); }
void initSNTP() 
{  
 sntp_set_time_sync_notification_cb(NTPnotify);                                               // Optional: callback when time syncs
 configTime(0, 0, "pool.ntp.org", "time.nist.gov");                                           // 1 hour sync interval = default
 setTimezone();                                                                               // Must be set after configTime
}
//--------------------------------------------                                                //
// NTP Get a NTP time and wait max 2.5 sec 
//--------------------------------------------
bool wait4SNTP() 
{
 int32_t Tick = millis(); 
 bool SNTPtimeValid = true;
 while (sntp_get_sync_status() != SNTP_SYNC_STATUS_COMPLETED) 
  { if ((millis() - Tick) >2500) {SNTPtimeValid = false; break;}   }                          // Wait max 2.5 seconds 
return  SNTPtimeValid;
}

//--------------------------------------------                                                //
// WIFI WEBPAGE 
//--------------------------------------------
void WebPage(void) 
{
 int i=0,n;
 SWversion();                                                                                 // Print the menu to the web page and BLE
 for (n=0;n<strlen(index_html_top);n++)       HTML_page[i++] = (char) index_html_top[n];
 if( i > MAXSIZE_HTML_PAGE - 999) 
   {  
    strcat(HTML_page, "<br> Send I for menu\n*** INCREASE MAXSIZE_HTML_PAGE in Webpage.h ***<br><br><br>");
   }
 else
   {
    for (n=0;n<strlen(html_info);n++)         HTML_page[i++] = (char) html_info[n];
    for (n=0;n<strlen(index_html_footer);n++) HTML_page[i++] = (char) index_html_footer[n];
    HTML_page[i]=0;
   }
 server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)                                  // Send web page with input fields to client
   {  request->send(200, "text/html",(const char*) HTML_page  );  }    ); 
                                                                                             // ringbuffer Web server endpoint for log data
 server.on("/log", HTTP_GET, [](AsyncWebServerRequest *request)                              // Serve the log viewer HTML page
   {  request->send(200, "text/html", logData);    });                                       // Serve the HTML page from LogViewer.h
 server.on("/tekstprint",  HTTP_GET, HandleTekstPrint);
 server.on("/tekstdownload", HTTP_GET, HandleTekstDownload);
 
 server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request)                              // Send a GET request to <ESP_IP>/get?input1=<inputMessage>
   { 
    String inputMessage;    String inputParam;
    if (request->hasParam(PARAM_INPUT_1))                                                     // GET input1 value on <ESP_IP>/get?input1=<inputMessage>
       {
        inputMessage = request->getParam(PARAM_INPUT_1)->value();
        inputParam = PARAM_INPUT_1;
       }
    else 
       {
        inputMessage = "";                                                                    //inputMessage = "No message sent";
        inputParam = "none";
       }
    bool NoInput = false;
    if (!inputMessage.equalsIgnoreCase("Y")   && !inputMessage.equalsIgnoreCase("G") )                  // Do not execute command via ReworkInputMessage && !inputMessage.equals("{")
        ReworkInputString(inputMessage);  
    else NoInput = true;                                                                 // Option Y LEDtest This is a common issue when using AsyncWebServer with NeoPixels/RMT - it's caused by interrupt conflicts and timing issues.
//    if (inputMessage.equalsIgnoreCase("Y") ) OptionYRainbow = true;                         // Interferes with WIFI connection and crasheWe run it outside the Webpage communication and crashes MCU   
    SWversion();                                                                              // Print the menu to the web page and BLE
    int i=0,n;
    for (n=0;n<strlen(index_html_top);n++)    HTML_page[i++] = (char) index_html_top[n];
    for (n=0;n<strlen(html_info);n++)         HTML_page[i++] = (char) html_info[n];
    if (NoInput)                                                                              // Add a flashing text
      {
      // const char* msg = "<p class=\"verdana-warning\"> Option Y or G not possible from browser<br></p>";
       const char* msg =
        "<p id='warn' class='verdana-warningbg'>Option Y or G not possible from browser</p>"
        "<script>"
        "setTimeout(()=>{ let w=document.getElementById('warn');"
        "if(w){w.style.animation='none';}"
        "},5000);"
        "</script>";
       for (int n = 0; n < strlen(msg); n++) {   HTML_page[i++] = msg[n]; }
       NoInput=false;
      }

    for (n=0;n<strlen(index_html_footer);n++) HTML_page[i++] = (char) index_html_footer[n];
    HTML_page[i]=0;        
    request->send(200, "text/html", HTML_page );  
   }   ); 

// Serve the OTA upload page
server.on("/update", HTTP_GET, [](AsyncWebServerRequest *request)
{  request->send(200, "text/html", OTA_html);});

server.on("/update", HTTP_POST, [](AsyncWebServerRequest *request) {                           // Handle the actual OTA upload
    shouldReboot = true;
    request->send(200, "text/html", 
      "<!DOCTYPE html><html><body>"
      "<h2>Update successful. Rebooting...</h2>"
      "<p>You will be redirected shortly.</p>"
      "<script>setTimeout(()=>{location.href='/'}, 7000);</script>"
      "</body></html>");
  }, 
  [](AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final) {
    if (!index) 
      {
       Tekstprintf("OTA Start: %s\n", filename.c_str());
       if (!Update.begin(UPDATE_SIZE_UNKNOWN)) { Update.printError(Serial);  }
      }
    if (Update.write(data, len) != len) { Update.printError(Serial);   }
    if (final) 
      {
       if (Update.end(true)) { Tekstprintf("OTA Success: %u bytes\n", index + len);  } 
       else {Update.printError(Serial); }
      }
  });
 server.onNotFound(notFound);
 server.begin();
}

//--------------------------------------------                                                //
// WIFI WEBPAGE 
//--------------------------------------------
void notFound(AsyncWebServerRequest *request) 
{
  request->send(404, "text/plain", "Not found");
}

//--------------------------------------------                                                //
// WIFI WEBPAGE Login credentials Access Point page with 192.168.4.1
//--------------------------------------------
void StartAPMode(void) 
{
 InApMode = true;
 Mem.WIFIcredentials = IN_AP_NOT_SET;
 WiFi.softAP(AP_SSID, AP_PASSWORD);
 dnsServer.start(53, "*", WiFi.softAPIP());
 Tekstprintln("\nConnect to StartWordcock in WIFI on your mobile.\nEnter password: wordclock\nThen in URL: 192.168.4.1 and enter router credentials");   
 server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
  { request->send(200,"text/html",SoftAP_html);  });
 server.on("/", HTTP_POST, [](AsyncWebServerRequest *request)
   {
    int params = request->params();
    for (int i = 0; i < params; i++) 
      {
       const AsyncWebParameter* p = request->getParam(i);
       if (p->name() == "ssid") { strcpy(Mem.SSID,p->value().c_str());       }
       if (p->name() == "pass") { strcpy(Mem.Password , p->value().c_str()); }
      }
    StoreStructInFlashMemory();
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)                                  // Send web page with input fields to client
      { request->send(200, "text/plain", "Credentials saved. Restarting...");  } );  
    Mem.WIFIcredentials = SET;
    StoreStructInFlashMemory();  
    delay(300);
    InApMode = false;
    ESP.restart();
   });
 server.begin();
 Tekstprintln("AP Mode Started");
 Tekstprintlnf("AP SSID: %s", AP_SSID);
 IPAddress ip = WiFi.softAPIP();
 Tekstprintlnf("AP IP Address: %u.%u.%u.%u", ip[0], ip[1], ip[2], ip[3]);
    
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
 if(esp_wifi_wps_enable(&config))  Tekstprintln("WPS Enable Failed");
 else if(esp_wifi_wps_start(0)) 	 Tekstprintln("WPS Start Failed");
}

void wpsStop()
{
 if(esp_wifi_wps_disable()) 	     Tekstprintln("WPS Disable Failed");
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
 if(keyvalue<13) { Tekstprintlnf("Key: %s",Key); delay(300); }
  if (Key == 12)   // *                                                                       // Pressing a * activates the keyboard input. 
   {  
    KeyInputactivated = true;
    KeyLooptime = millis();
    KeypadString ="";
    ColorLeds("",0,NUM_LEDS-1,0x00FF00);                                                      // Turn all LEDs green
    ShowLeds();                                                                               // Push data in LED strip to commit the changes
    Tekstprintln("Key entry activated");
   }
 if (KeyInputactivated && (Key>=0 && Key<10))
   {
    delay(20); 
    KeypadString += Key;                                                                      // Digit keys 0 - 9
    ColorLeds("",0,Key-48,0xFF0000);                                                          // Turn all LEDs red
    ShowLeds();                                                                               // Push data in LED strip to commit the changes
 //   Tekstprintln(KeypadString);
   }
 if (KeypadString.length()>5)                                                                 // If six numbers are entered rework this to a time hhmmss
   {       
   if(KeypadString=="999999")
     { 
      KeypadString = "";   
      Reset();
      Tekstprintln("Settings reset");   
     }
    else 
     {      
      ReworkInputString(KeypadString);                                                        // Rework ReworkInputString();
      KeypadString = "";
      Tekstprintln("Time changed");
     }    
   }
 if ( KeyInputactivated && ((millis() - KeyLooptime) > 30000) ) 
   {  
    KeyInputactivated = false;                                                                // Stop data entry after 30 seconds. This avoids unintended entry 
    KeypadString ="";
    Tekstprintln("Keyboard entry stopped");
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
 snprintf(sptext, sizeof(sptext),"3*1 keypad %s used", Mem.UseRotary==2?"IS":"NOT");

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
   Tekstprintlnf("Key: %s", Key);
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
 snprintf(sptext, sizeof(sptext),"Rotary %s used", Mem.UseRotary==1?"IS":"NOT");
 
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
     Tekstprintlnf("----> Index: %d", encoderPos);
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
    Tekstprintlnf("NoofRotaryPressed: %d",NoofRotaryPressed);   
    Looptime = millis();     
    Displaytime();                                                                            // Turn on the LEDs with proper time
   }
 }

//--------------------------------------------                                                //
// IR-RECEIVER Start alarge or tiny IR-RECEIVER 
// Copy the to be used ButtonNames for a small or large IR-receiver
//--------------------------------------------
void Start_IRreceiver(void)
{
 if (Mem.UseRotary==3) Init_IRreceiver(ButtonLNames, sizeof(ButtonLNames) / sizeof(ButtonLNames[0]));
 if (Mem.UseRotary==4) Init_IRreceiver(ButtonTNames, sizeof(ButtonTNames) / sizeof(ButtonTNames[0]));
}

//--------------------------------------------                                                //
// IR-RECEIVER Init
//--------------------------------------------
void Init_IRreceiver(String* ButtonTempNames, byte numButtons)
{
 ButtonNames = ButtonTempNames;  // Store pointer
 NOOFBUTTONS = numButtons;
 IrReceiver.begin(IRReceiverPin);

 if (Mem.UseRotary == 3 || Mem.UseRotary == 4)  {  GetIRRemoteFromFlashMemory(); }            // Load IR settings (only if IR remote is enabled)
 if (IRMem.remoteIdentified && IRMem.buttons[0].learned)                                      // Check if we already have learned buttons
  {
   learningMode = false;
   //PrintAllMappings();
   snprintf(sptext, sizeof(sptext), "Remote identified - Protocol: %s", getProtocolString((decode_type_t) IRMem.learnedRemoteProtocol));
   Tekstprintln(sptext);
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
      if (!(IrReceiver.decodedIRData.flags & IRDATA_FLAGS_IS_REPEAT))                         // Ignore repeat codes
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
    snprintf(sptext, sizeof(sptext), "Remote identified - Protocol: %s, Address: 0x%04X", getProtocolString(protocol), address);
    Tekstprintln(sptext);
  }
  else
  {
    if (address != IRMem.learnedRemoteAddress || protocol != (decode_type_t)IRMem.learnedRemoteProtocol)    // Check if this is from the same remote
    {
      Tekstprintln("⚠ Wrong remote! Please use the same remote control.");
      snprintf(sptext, sizeof(sptext), "Expected Address: 0x%04X, Got: 0x%04X", IRMem.learnedRemoteAddress, address);
      Tekstprintln(sptext);
      return;
    }
  }
  IRMem.buttons[currentLearningIndex].protocol = (uint8_t)protocol;                           // Store the button mapping
  IRMem.buttons[currentLearningIndex].command = command;
  IRMem.buttons[currentLearningIndex].address = address;
  IRMem.buttons[currentLearningIndex].learned = true;
  
  snprintf(sptext, sizeof(sptext), "✓ Learned '%s' - Protocol: %s, Command: 0x%04X, Address: 0x%04X", 
          ButtonNames[currentLearningIndex].c_str(), getProtocolString(protocol), command, address);
  Tekstprintln(sptext);
  currentLearningIndex++;
  if (currentLearningIndex < NOOFBUTTONS)
  {
    snprintf(sptext, sizeof(sptext), "\nPlease press button: %s", ButtonNames[currentLearningIndex].c_str());
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
    snprintf(sptext, sizeof(sptext), "Only responding to remote with Address: 0x%04X", IRMem.learnedRemoteAddress);
    Tekstprintln(sptext);
    Tekstprintln("Press POWER to activate the remote.      = POWER ON");
    Tekstprintln("After 5 minutes remote auto powers down. = POWER OFF");         
    Tekstprintln("With POWER OFF the digits will change the Display choice");     
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
    snprintf(sptext, sizeof(sptext), "⚠ Ignored - Wrong remote (Address: 0x%04X)", address);
    if (address!=0) Tekstprintln(sptext);                                                     // Stray input detected
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
 for (int i = 0; i < NOOFBUTTONS; i++) {IRMem.buttons[i].learned = false; }
 snprintf(sptext, sizeof(sptext), "Please press button: %s", ButtonNames[0].c_str());
 Tekstprintln(sptext);
}

//--------------------------------------------                                                //
// IR-RECEIVER Shows learned buttons
//--------------------------------------------
void PrintAllMappings(void)
{
 if (!IRMem.remoteIdentified) {Tekstprintln("No remote learned yet!");  return; }
 PrintLine(35);
 Tekstprintlnf( "IR-Remote Address: 0x%04X", IRMem.learnedRemoteAddress);
 Tekstprintlnf( "IR-Remote Protocol: %s", getProtocolString( (decode_type_t) IRMem.learnedRemoteProtocol));
 PrintLine(35);
 for (int i = 0; i < NOOFBUTTONS; i++)
    if (IRMem.buttons[i].learned)
      Tekstprintlnf( "%s\t-> Cmd: 0x%04X", ButtonNames[i].c_str(), IRMem.buttons[i].command);    
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
 for (int i = 0; i < NOOFBUTTONS; i++)
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
  for (int i = 0; i < NOOFBUTTONS; i++)
  {
    if (IRMem.buttons[i].learned && 
        IRMem.buttons[i].protocol == (uint8_t)protocol &&
        IRMem.buttons[i].command == command && 
        IRMem.buttons[i].address == address)
    {
      snprintf(sptext, sizeof(sptext), "Button pressed: %s", ButtonNames[i].c_str());
      Tekstprintln(sptext);
      found = true;
      ReworkIRremoteValue(i);
      return i;
    }
  }
  
  if (!found)
  {
    snprintf(sptext, sizeof(sptext), "Unknown button - Protocol: %s, Command: 0x%04X, Address: 0x%04X", getProtocolString(protocol), command, address);
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
//  Button "POWER" and "ONOFF" can work without POWER ON
//--------------------------------------------
void ReworkIRremoteValue(int ButtonNamesNr)
{
 String ButtonName = ButtonNames[ButtonNamesNr]; 
 String DisplayOption = "Q0";
 if (ButtonNamesNr == -1)  {Tekstprintln("Unknown button - ignored");  return;  }

if (Mem.UseRotary == 3)
 {
 if (IR_PowerOnstate == false) // && ButtonNamesNr < 14 ) 
 {
 switch(ButtonNamesNr)
  {
    case 0: ReworkInputString("Q0");  break;                                                  // Button "0"
    case 1: ReworkInputString("Q1");  break;                                                  // Button "1"
    case 2: ReworkInputString("Q2");  break;                                                  // Button "2"
    case 3: ReworkInputString("Q3");  break;                                                  // Button "3"
    case 4: ReworkInputString("Q4");  break;                                                  // Button "4"
    case 5: ReworkInputString("Q5");  break;                                                  // Button "5"
    case 6: ReworkInputString("Q6");  break;                                                  // Button "6"
    case 7: ReworkInputString("Q7");  break;                                                  // Button "7"
    case 8: ReworkInputString("Q8");  break;                                                  // Button "8"
    case 9: ReworkInputString("Q9");  break;                                                  // Button "9"                                               // Turn displaychoice on
    case 10:                                                                                  // Button "UP"
    case 11:                                                                                  // Button "DOWN"
    case 12:                                                                                  // Button "LEFT"
    case 13:                                                                                  // Button "RIGHT"
            break;
    case 14:                                                                                  // Button "POWER"
            ToggleIRpower();
            break;
    case 15:                                                                                  // Button "OK"
            break;
    case 16:                                                                                  // Button "ONOFF"
            ReworkInputString("O");                                                           // Turn On OFF display
            break;
    default:
            snprintf(sptext, sizeof(sptext), "Button '%s' not yet assigned", ButtonName.c_str());

            break;
  }
 }
 else
 {
 switch(ButtonNamesNr)
  {
    case 0 ... 9:                                                                                   // Button "0 - 9"
            EnteredDigits += ButtonName;                                                      // Add digit to entered string  
            if (EnteredDigits.length() > 6) {EnteredDigits = EnteredDigits.substring(0, 6); } // Limit to 6 digits (HHMMSS)
            if (EnteredDigits.length() == 6)
              {
               snprintf(sptext, sizeof(sptext), "Time entered: %c%c:%c%c:%c%c (press OK)", 
                 EnteredDigits[0], EnteredDigits[1], EnteredDigits[2], 
                 EnteredDigits[3], EnteredDigits[4], EnteredDigits[5]);
              }
            else snprintf(sptext, sizeof(sptext), "Digits: %s (Need 6 for HHMMSS)", EnteredDigits.c_str());
            Tekstprintln(sptext);
            break;
    case 10:                                                                                  // Button "UP"
            AdjustTime(1, 0, 0);                                                              // +1 hour
            break;
    case 11:                                                                                  // Button "DOWN"
            AdjustTime(-1, 0, 0);                                                             // -1 hour
            break;
    case 12:                                                                                  // Button "LEFT"
            AdjustTime(0, -1, 0);                                                             // -1 minute
            break;
    case 13:                                                                                  // Button "RIGHT"
            AdjustTime(0, 1, 0);                                                              // +1 minute
            break;
    case 14:                                                                                  // Button "POWER"
            ToggleIRpower();
            break;
    case 15: 
            learningMode = false;                                                             // Button "OK"
//          Serial.println(EnteredDigits.length());   
            if (EnteredDigits.length() == 6) {ReworkInputString(EnteredDigits); EnteredDigits = "";}
            else  { Tekstprintln("⚠ Need 6 digits (HHMMSS) before OK");  }
            break;
    case 16:                                                                                  // Button "ONOFF"
            ReworkInputString("O");                                                           // Turn On OFF display
            break;
    default:
            snprintf(sptext, sizeof(sptext), "Button '%s' not yet assigned", ButtonName.c_str());
            Tekstprintln(sptext);
            break;
  }
 }
 }  // end if ==3

if (Mem.UseRotary == 4)                                                                      // {"MIN-1", "MIN+1","UUR-1","UUR+1","POWER","ONOFF"};
 {
 if (IR_PowerOnstate == false) // && ButtonNamesNr < 14 ) 
 {
 switch(ButtonNamesNr)
    {
    case 4:                                                                                   // Button "POWER"
            ToggleIRpower();
            break;
    case 5:                                                                                   // Button "ONOFF"
            ReworkInputString("O");                                                           // Turn On OFF display
            break;
    default:
            Tekstprintln("Turn On remote with POWER");
            break;
    }
  }
 else
   {
    switch(ButtonNamesNr)
     {
    case 0:                                                                                   // Button "MIN-1"
            AdjustTime(0, -1, 0);                                                             // -1 minute
            break;
    case 1:                                                                                   // Button "MIN+1"
            AdjustTime(0, 1, 0);                                                              // +1 minute
            break;
    case 2:                                                                                   // Button "UUR-1"
            AdjustTime(-1, 0, 0);                                                             // -1 hour
            break;
    case 3:                                                                                   // Button "UUR+1"
            AdjustTime(1, 0, 0);                                                              // +1 hour
            break;
    case 4:                                                                                   // Button "POWER"
            ToggleIRpower();
            break;
    case 5:                                                                                   // Button "ONOFF"
            ReworkInputString("O");                                                           // Turn On OFF display
            break;
    default:
            snprintf(sptext, sizeof(sptext), "Button '%s' not yet assigned", ButtonName.c_str());
            Tekstprintln(sptext);
            break;
      }
   }
 } // end if ==4
}

//--------------------------------------------                                                //
// IR-RECEIVER Adjust current time by hours/minutes/seconds
//--------------------------------------------
void AdjustTime(int DeltaHours, int DeltaMinutes, int DeltaSeconds)
{
 timeinfo.tm_hour += DeltaHours;
 timeinfo.tm_min  += DeltaMinutes;
 timeinfo.tm_sec  += DeltaSeconds;
 //time_t t = 
 mktime(&timeinfo);
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
   Tekstprintln("IR-remote is ON");
   IR_StartTime = millis();
  }
 else
  {
    Tekstprintln("IR-remote is OFF");
    IR_PowerOnstate = false;                                                                  // Do not react on IR-remote input
  } 
}

//--------------------------------------------                                                //
// LOGBUFFER Initialize circular logging buffer      
//--------------------------------------------
void InitLogBuffer()
{
 size_t heap = ESP.getFreeHeap();                                                                                   // Current free heap
 if (heap > 20000) LogBufferSize = heap - 10000;                                                                    // Keep 10k reserved
 else LogBufferSize = heap / 2;                                                                                     // Fallback
 LogBuffer = (char*)malloc(LogBufferSize);                                                                          // Allocate single buffer
 if (!LogBuffer)
   {
    Tekstprintln("ERROR: Could not allocate log buffer");                                                        //
    return;                                                                                                        //
   }
 memset(LogBuffer, 0, LogBufferSize);                                                                               // Clear buffer
 LogWritePos = 0;                                                                                                   //
 LogWrapped  = false;       
 snprintf(sptext, sizeof(sptext)," Log buffer allocated: %u bytes", (unsigned) LogBufferSize);                                                                                     //
 Tekstprintln(sptext);                                                                          //
}

//--------------------------------------------                                                //
// LOGBUFFER Add a log line (raw C-string). Always ends with '\n'.
//--------------------------------------------
void AddLog(const char* msg)
{
 if (!LogBuffer) return;
 size_t len = strlen(msg);
 if (len >= LogBufferSize)
    {
     msg += (len - LogBufferSize);  // keep only last part
     len = strlen(msg);
    }
    for (size_t i = 0; i < len; i++)
    {
     LogBuffer[LogWritePos++] = msg[i];
     if (LogWritePos >= LogBufferSize)
        {
         LogWritePos = 0;
         LogWrapped  = true;
        }
    }
}

//--------------------------------------------                                                //
// LOGBUFFER Stream the last 500 lines circular log buffer directly to the client                                 //
//--------------------------------------------
void HandleTekstPrint(AsyncWebServerRequest *request)
{
 if (!LogBuffer)
   {
     request->send(200, "text/plain", "Log buffer uninitialized\n");
     return;
   }
 AsyncResponseStream *response = request->beginResponseStream("text/plain");
 size_t lineCount = 0;
 ssize_t pos = LogWritePos;
 for (size_t scanned = 0; scanned < LogBufferSize && lineCount < 500; scanned++)              // Count lines backwards to find last 500 '\n's
    {
     pos = (pos == 0) ? LogBufferSize - 1 : pos - 1;
     if (LogBuffer[pos] == '\n') lineCount++;
     if (!LogWrapped && pos == 0) break;                                                      // Stop if buffer not wrapped
    }
   size_t start = (pos + 1) % LogBufferSize;                                                  // Now pos points to the start of the 500th-last line (or beginning)
 if (!LogWrapped || start < LogWritePos)                                                      // Stream lines from start → end of buffer
   {
    response->write(LogBuffer + start, LogWritePos - start);
   }
 else
   {
    response->write(LogBuffer + start, LogBufferSize - start);                               // Wrap: first part from start → end of buffer
    response->write(LogBuffer, LogWritePos);                                                 // second part from 0 → write pos
   }
 request->send(response);
}

//--------------------------------------------                                                //
// LOGBUFFER Stream all the circular log buffer directly to the client                                      //
//--------------------------------------------
void HandleTekstDownload(AsyncWebServerRequest *request)
{
 if (!LogBuffer)
   {
    request->send(200, "text/plain", "Log buffer uninitialized\n");
    return;
   }
 AsyncResponseStream *response = request->beginResponseStream("text/plain");
 if (!LogWrapped)     response->write(LogBuffer, LogWritePos);
 else
   {
    size_t tail = LogBufferSize - LogWritePos;
    response->write(LogBuffer + LogWritePos, tail);
    response->write(LogBuffer, LogWritePos);
   }
    request->send(response);
}

/// ringbuffer
//--------------------------------------------                                                //
// Print memory space
//--------------------------------------------
void printHeaps(void)
{
 Tekstprintlnf("8-bit capable heap: %7.3f Mbytes", heap_caps_get_free_size(MALLOC_CAP_8BIT)   / (1024.0 * 1024.0));
 Tekstprintlnf("  DMA capable heap: %7.3f Kbytes", heap_caps_get_free_size(MALLOC_CAP_DMA)    / 1024.0);
 Tekstprintlnf(" SPRAM/SPIRAM heap: %7.3f Mbytes", heap_caps_get_free_size(MALLOC_CAP_SPIRAM) / (1024.0 * 1024.0));
}