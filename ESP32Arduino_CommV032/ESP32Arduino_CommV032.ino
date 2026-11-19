/* 
 Author      : Ed Nieuwenhuijs ednieuw.nl
 Changes V001: Derived from ESP32Arduino_WordClockV098.ino
 Changes V002:  
 Changes V003: LED crashes with some rmt error. Runs on ESP32-S3-WROOM-DevKitC-1
 Changes V004: WIFI not connecting. Added in StartAPmode()  xTaskCreatePinnedToCore(WebServerTask,"WebServerTask", 4096, NULL, 1, NULL, 1 ); 
 Changes V005: WIFI not connecting. Added WS2812 and SK6812 LED-strip functionality to GPIO-pin 8 
 Changes V006: Equal to ESP32Arduino_WordClockV105.ino
 Changes V007: crashes when using EDSoftLED
 Changes V008: Equal to ESP32Arduino_WordClockV112.ino
 Changes V009: Added Title in WebPage() 
 Changes V010: Time functions updated. You can compile now By Arduino pin (default)
 Changes V011: Equal to ESP32Arduino_WordClockV132.ino
 Changes V012: Equal to ESP32_WordClockV133. Added Failsafe NoConnectionCounter that restart after 30 minutes no IP when WIFI is On 
 Changes V013: Equal to ESP32_WordClockV150.
 Changes V014: Equal to ESP32_WordClockV183.
 Changes V015: Equal to ESP32_WordClockV184. Long log buffer as download txt file
 Changes V016: Changed name of TimeSender -> TimeReceiver. Added TimeSender that sends time every minute. Sketch compiles without errors
 Changes V017: Added CheckWIFIcommand() to avoid WIFI crashes when a command take several seconds to end, like LEDtest()
 Changes V018: Added in menu option Q; TimeSender On/Off 
 Changes V019: Added DisconnectBLE() as function. When @ in webpage inputfield cleared from sticking @.
 Changes V020: Variables etc. renamed from Timesender -> Timereceiver in Timereceiver functions  
 Changes V021: Comparable with ESP32_WordClockV195
 Changes V022: Corrected nasty bug ->  Mem.NVRAMmem[lasthour] = NoofLDRreadshour ? (byte)(SumLDRreadshour / NoofLDRreadshour) : 0;   // Update the average LDR readings per hour. Avoids dividing by zero
 Changes V023: Comparable with ESP32_WordClockV199
               Changes V195 V199: Cleanup code. Comparable with ESP32Arduino_CommV022.ino. Corrected bug ->  Mem.NVRAMmem[lasthour] = NoofLDRreadshour ? (byte)(SumLDRreadshour / NoofLDRreadshour) : 0;
               To avoid translation lang="en" added -> <html lang="en"><head>. Changes in void PrintMem(void) .
               All Rework in Webpage to CheckWIFIcommand(). Corrected double Menu i and ii on webpage when BLE open. Updated HTML and logging behaviour. 
               lastminute = 99; -> end ReworkInputString
 Changes V024: Comparable with ESP32_WordClockV200
 Changes V025:  Comparable with ESP32_WordClockV200 
 Changes V026: Bug resolved do not print GetTijd when No logging. OTA logic improved   
 Changes V027: Comparable with ESP32_WordClockV210
 Changes V028: Added HC-12 Time sender option   
 Changes V029: Special HC-12 version. Added HC-12 Time sender in menu
 Changes V030: SSID length >3 for P.Andries design 
 Changes V031: Send commands T, K, L, M, N and O to Character_Clock_V117_PA03
 Changes V032  Added InitStatusLEDs(). When checkcode 25065 is wrong also Reset credentials   
 
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

ESP32-S3-WROOM-DevKitC-1
Option	    Value
Board:      ESP32-S3 DEV Module on ESP32 core 3.3.3 
USB         CDC on Boot	Enabled
USB Mode	  Hardware CDC and JTAG (or just Hardware CDC)
Upload Mode	UART0 / Hardware Serial
Flash Mode	QIO 80MHz (default, or leave unchanged)
Port	      Select the UART COM port (the one with a USB-to-Serial chip)

*/
// =============================================================================================================================

//------------------------------------------------------------------------------              //
// ESP32 Includes defines and initialisations
//--------------------------------------------
                      #if ESP_ARDUINO_VERSION >= ESP_ARDUINO_VERSION_VAL (3, 3, 0)            // Use EdSoftLED with ESP32 compiler.
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
 #ifdef LED_BUILTIN            // to suspress warning in IR-remote
 #undef LED_BUILTIN 
 #define LED_BUILTIN 48 
 #endif
#include <IRremote.hpp>        // IR remote control

//------------------------------------------------------------------------------              //
// SPIFFS storage
//--------------------------------------------
Preferences FLASHSTOR;
                           #if defined(ARDUINO_NANO_ESP32)
//------------------------------------------------------------------------------              //
// PIN Assigments for Arduino Nano ESP32 (S3)
//------------------------------------------------------------------------------
 
enum DigitalPinAssignments {      // Digital hardware constants ATMEGA 328 ----
 SERRX        = D0,               // Connects to Bluetooth TX
 SERTX        = D1,               // Connects to Bluetooth RX
 encoderPinB  = D2, //5,          // D2 left (labeled CLK on decoder) *** D8 on large PCB !!! no interrupt pin (Use GPIO pin numbering for rotary encoder lib)  
 encoderPinA  = D3, //6,          // D3 right (labeled DT on decoder) on interrupt pin
 clearButton  = D4,               //7,  // D4 switch (labeled SW on decoder)
 IRReceiverPin= D4,               // D4 Infrared receiver pin instead of rotary encoder
 HC12_TX      = D4,               // D4 Send time to HC-12 serial UART 433MHz transnitter/receiver
 LED_PIN      = D5,               //8,  // D5 / GPIO 8 Pin to control colour SK6812/WS2812 LEDs (replace D5 with 8 for NeoPixel lib)
 EmptyD6      = D6,               // D6 Empty
 EmptyD7      = D7,               // D7 Empty
 encoderPinBL = D8,               // D8 *** If large PCB is used change pin D2 to D8 !!
 PCB_LED_D09  = D9,               // D9
 PCB_LED_D10  = D10,              // D10
 secondsPin   = D13               //48, // D13  GPIO48 (#ifdef LED_BUILTIN  #undef LED_BUILTIN #define LED_BUILTIN 48 #endif)
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
 
                                  #else

//------------------------------------------------------------------------------              //
// PIN Assigments for ESP32 S3 DevKitC 1
//------------------------------------------------------------------------------  
enum DigitalPinAssignments {      // Digital hardware constants ATMEGA 328 ----
 SERRX        = 44,               // D1 Connects to Bluetooth TX
 SERTX        = 43,               // D0 Connects to Bluetooth RX
 encoderPinB  = 4,                // D2 left (labeled CLK on decoder)no interrupt pin (Use GPIO pin numbering for rotary encoder lib)  
 encoderPinA  = 5,                // D3 right (labeled DT on decoder)on interrupt pin
 clearButton  = 6,                // D4 switch (labeled SW on decoder)
 IRReceiverPin= 6,                // D4 Infrared receiver pin instead of rotary encoder
 LED_PIN      = 48,               // D5 / GPIO 8 Pin to control colour SK6812/WS2812 LEDs
 PCB_LED_D09  = 10,               // D9
 PCB_LED_D10  = 11,               // D10
 secondsPin   = 13,               // D13  GPIO48 (#ifdef LED_BUILTIN  #undef LED_BUILTIN #define LED_BUILTIN 48 #endif)
 };
 
enum AnaloguePinAssignments {     // Analogue hardware constants ----
 EmptyA0      = 10,               // Empty
 EmptyA1      = 11,               // Empty
 PhotoCellPin = 12,               // LDR pin
 OneWirePin   = 13,               // OneWirePin
 SDA_pin      = 8,                // SDA pin
 SCL_pin      = 9,                // SCL pin
 EmptyA6      = 16,               // Empty
 EmptyA7     =  17};              // Empty
                          #endif //defined(ARDUINO_NANO_ESP32)
//------------------------------------------------------------------------------              //
// LED
//------------------------------------------------------------------------------
const uint32_t  NUM_LEDS = 1;                                                                // The no of LEDs when a SK6812 or WS2812 LED-strip is attached to LED_PIN
#define CLEARLEDSTARTUP 9999
                     #ifdef USEEDSOFTLED
EdSoftLED LEDstrip;                                                                          // Use EdSoftLED 
EdSoftLED LED6812strip = EdSoftLED(NUM_LEDS, LED_PIN, SK6812WGRB);
EdSoftLED LED2812strip = EdSoftLED(NUM_LEDS, LED_PIN, WS2812GRB);
bool UsedEDSOFTLED = true;
                      #else
Adafruit_NeoPixel LEDstrip;
Adafruit_NeoPixel LED6812strip = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRBW + NEO_KHZ800); // NEO_RGBW
Adafruit_NeoPixel LED2812strip = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRB  + NEO_KHZ800); // NEO_RGB NEO_GRB
bool UsedEDSOFTLED = false;
                      #endif
// #define MCULEDPIN 9
// Adafruit_NeoPixel MCUled = Adafruit_NeoPixel(1, MCULEDPIN, NEO_GRB + NEO_KHZ800);  //NEO_RGB NEO_GRB

//------------------------------------------------------------------------------              //
const uint32_t black    = 0x000000, darkorange    = 0xFF8C00, red        = 0xFF0000, chartreuse   = 0x7FFF00;
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
int        LDRread            = 0;
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
struct    tm timeinfo;                                                                        // storage of time 

//--------------------------------------------                                                //
// BLE  RX-TX //#include <NimBLEDevice.h>
//--------------------------------------------
static NimBLEServer *pServer      = NULL;
static NimBLECharacteristic * pTxCharacteristic;
static bool BLEConnected    = false;
static bool oldBLEConnected = false;
std::string ReceivedMessageBLE = "";
unsigned long BLEConnectedSince   = 0;
static unsigned long TRConnectedSince = 0;                                                      // Separate timer for TimeReceiver activity (avoid sharing with server timeout)

#define SERVICE_UUID           "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"                         // UART service UUID
#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"

//--------------------------------------------                                                //
// BLE TimeReceiver  //#include <NimBLEDevice.h>
//--------------------------------------------
 #define TIMESENDER_DEVICE_NAME "BLE-UARTtime"
 #define TIMERECEIVER_MIN_RSSI    (-90)

static NimBLEClient*               pClient        = nullptr;
static NimBLERemoteCharacteristic* pRemoteTX      = nullptr;
static NimBLERemoteCharacteristic* pRemoteRX      = nullptr;

// Found address and flags
static NimBLEAddress TRfoundAddr;
static bool          TRhaveFoundAddr      = false;
static bool          TRconnectRequested   = false;
static bool          TRConnecting         = false;
static bool          TRConnected          = false;
static unsigned long TRconnectStartms     = 0;
static unsigned long LastTimeReceiverScan = 0;
static bool          TimeReceiverRunning  = false; 
static const unsigned long CONNECT_TIMEOUT_MS = 10000;                         // 10s


/**************************************************************************************************
   TIME SENDER  (BLE SERVER MODE)
**************************************************************************************************/

static NimBLEServer*           TSpServer           = nullptr;                                   // Comment line ...
static NimBLEService*          TSpService          = nullptr;                                   // Comment line ...
static NimBLECharacteristic*   TSpTX               = nullptr;                                   // Comment line ...
static NimBLECharacteristic*   TSpRX               = nullptr;                                   // Comment line ...
static NimBLEAdvertising*      TSpAdvertising      = nullptr;                                   // Comment line ...

static bool                    TSClientConnected   = false;
static bool                    TSClientSubscribed  = false;                                    // True once client has enabled notifications (CCCD written)
static bool                    TimeSenderServerRunning = false;                                // Comment line ...
static unsigned long           TSsendDelayms       = 0;
static bool                    TSdatePending       = false;                                     // Comment line ...
static uint16_t                TSconnHandle        = 0;
#define UART_SERVICE_UUID        "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"                         // Comment line ...
#define UART_CHARACTERISTIC_RX   "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"                         // Comment line ...
#define UART_CHARACTERISTIC_TX   "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"                         // Comment line ...

/**************************************************************************************************/

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
const char* AP_SSID         = "StartESP32Comm";
const char* AP_PASSWORD     = "esp32comm";
AsyncWebServer server(80);
DNSServer dnsServer;
bool shouldReboot           = false;
bool OptionYRainbow         = false;
bool DoNotLog               = false;                                                          // Use this flag to suspress Logging to Logviewer 
byte NoConnectionCounter    = 0;                                                              // Count times minutes with no WIFI connection
String PendingCommand = "";                                                                   // 


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
// circular Log buffer variables
//----------------------------------------
char*  LogBuffer      = nullptr;                                                              // Circular buffer
size_t LogBufferSize  = 0;                                                                    // Total buffer size
size_t LogWritePos    = 0;                                                                    // Write pointer
size_t LogStartPos    = 0;                                                                    // Startpos Write pointer
bool   LogWrapped     = false;                                                                // Did we wrap?

//------------------------------------------------------------------------------              //
// Common
//----------------------------------------
char      sptext[255];                                                                        // For common print use 
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
  byte LightReducer     = 50;
  int  LowerBrightness  = 0;
  int  UpperBrightness  = 255;
  int  NVRAMmem[24];                                                                          // LDR or other readings
  byte BLEOn            = 1;                                                                  // BLE On/Off
  byte NTPOn            = 1;                                                                  // NTP On/Off
  byte WIFIOn           = 1;                                                                  // WIFI  On/Off  
  byte StatusLEDOn      = 1;                                                                  // Status LEDs on board  On/Off
  byte MCUrestarted     = 0;                                                                  // No of times WIFI reconnected 
  byte LoopRebooted     = 0;                                                                  // No of times WIFI rebooted
  byte TimeReceiver     = 0;                                                                  // Use Time Sender app to set time 
  byte TimeLogPrint     = 1;                                                                  // Print time per minute (1) or hour (2)  
  byte DCF77On          = 0;
  byte TimeInput        = 0;                                                                  // Use coding for Rotary encoder ==1 or 3x1 membrane keypad ==2
  byte UseDS3231        = 0;                                                                  // Use the DS3231 time module 
  byte LEDstrip         = 0;                                                                  // 0 = SK6812 LED strip. 1 = WS2812 LED strip
  byte FiboChrono       = 0;                                                                  // true = Fibonacci, false = chrono clock display
  byte NoExUl           = 0;                                                                  // 0 = Normal, 1 = Extreme, 2 = Ultimate display of colours
  int  WIFIcredentials  = 0;                               // Can changed to byte             // Status of the WIFI connection. SSID&PWD set or in AP mode
  int  IntFuture2       = 0;                                                                  // For future use
  byte TimeSender       = 0;                                                                  // 
  byte Ringbufcnt       = 0;                                                                  // Ringbuffer counter ON or OFF
  byte HC12Time         = 0;                                                                  // Use HC12 module 
  byte byteFuture4      = 0;                                                                  // For future use
  byte HetIsWasOff      = 0;                                                                  // Turn On or Off HET IS WAS   
  byte EdSoftLEDSOn     = 0;                                                                  // EdSoft text on/off   
  byte RandomDisplay    = 0;                                                                  // For future use 
  byte WIFInoConnection = 0;                                                                  // Will store the no of reboots   
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
bool ShortMenu = true;                                                                        // Small=true of full=false menu
const char *menu[] = {
 "A SSID B Password C BLE beacon name",
 "D Date (D15012021) T Time (T132145)",
 "E Timezone  (E<-02>2 or E<+01>-1)",
 "G Scan WIFI networks",
 "H H01 rotary H02 buttons H03/04 remote",
 "Q Time sender, H05 time receiver On/Off",
 "U HC12 timesender On/Off",
 "} Learn IR remote, + Fast BLE",
 "I Info menu, II long menu ",
 "J DS3231 RTC module On/Off",
 "K LDR /s, Time K1/min K2/hr K0/Off", 
 "N Display off between Nhhhh (N2208)",
 "O Display On/Off, P StatusLED On/Off",
 "R Reset settings, @ Reset MCU",
 "--Light intensity settings (1-250)--",
 "S Slope, L Min, M Max  (S50 L5 M200)",
 "W WIFI X NTP& Z WPS CCC BLE",
 "RTC: ! See, & Update",
 "Ed Nieuwenhuijs May 2026" };
 
 const char *menusmall[] = {
 "I Menu, II long menu",
 "K LDR reads/sec toggle On/Off", 
 "N Display off between Nhhhh (N2208)",
 "R Reset settings",
 "U HC12 timesender On/Off",
 "@ Restart" };
//  -------------------------------------   End Definitions  ---------------------------------------

//--------------------------------------------                                                //
// ARDUINO Setup
//--------------------------------------------
void setup() 
{
 Serial.begin(115200);                                                                        // Setup the serial port to 115200 baud
 Wire.begin();                                                                                // Maybe not necessary
 int32_t Tick = millis(); 
 InitStorage();                                                                               // Load settings from storage and check validity   
 Mem.MCUrestarted++;                                                                          // MCU Restart counter     
 if(Mem.MCUrestarted>10) { Reset(); }                                                         // If the MCU restarts during Setup() so often -> Reset() but no ResetCredentials(); 
 StoreStructInFlashMemory();                                                                  // Store the Mem.MCUrestarted   
 StartLeds();                                                                                 // Select the SK6812 or WS2812 LED strip and initialysed
 InitStatusLEDs();                                                                            // Init status LEDs 
 SetStatusLED(10,0,0);                                                                        // Set the status LED to red
 while (!Serial && ( (millis()-Tick) < 2002)) { LEDstartup(green);delay(500); }               // Wait max 1.5 sec to establish serial connection
 SetStatusLED(10,0,10);                                                                       // Set the status LED to red
 while (!Serial && ( (millis()-Tick) < 3000)) { LEDstartup(purple); delay(250); }             // Wait max 3 sec to establish serial connection
 PrintHeaps(); 
 LEDstartup(capri); Tekstprintln("Serial started\nStored settings loaded\nLED strip started");// InitStorage and StartLEDs must be called first                                                               // 
 Tekstprintlnf( "Using %s LED library", UsedEDSOFTLED?"EDSOFTLED":"NEOPIXEL");                // 
 Tekstprintlnf("LED strip is %s", Mem.LEDstrip?"WS2812":"SK6812" );
 if(Mem.TimeInput==1) {LEDstartup(pink);  InitRotaryMod();     Tekstprintln("Rotary available"); }      // Start the Rotary encoder
 if(Mem.TimeInput==2) {LEDstartup(grass); InitKeypad3x1();     Tekstprintln("Keypad available"); }      // Start the Keypad 3x1 
 if(Mem.TimeInput >2) {LEDstartup(white); Start_IRreceiver();  Tekstprintln("IRremote available");}     // Start IR remote Mem.TimeInput = 3 or 4                                                                                     
 if(Mem.BLEOn)        {LEDstartup(blue);  StartBLEService();   Tekstprintln("BLE started"); }           // Start BLE service 
 if(Mem.WIFIOn)       {LEDstartup(purple);ConnectWIFI();       Tekstprintln("WIFI started");}           // Start WIFI and optional NTP if Mem.WIFIOn = 1 
 InitTimeSystem();    LEDstartup(yellow);                      Tekstprintln("Time system started");     // Initialize RTC + DS3231 sync
 if(Mem.TimeReceiver) {LEDstartup(blue); StartTimeReceiverScan();Tekstprintln("Time Receiver started"); } // Time Sender app on iPhone can be used
 if(Mem.TimeSender)   {LEDstartup(sky);  BLETimeSenderStart(); Tekstprintln("Time Sender started"); }   // Start the TimeSender   
 if(Mem.HC12Time)   {LEDstartup(amber);  InitHC12();           Tekstprintln("HC12 Time started"); }     // Start the HC12 time sender   
 Previous_LDR_read = ReadLDR();                                                                         // Set the initial LDR reading 
 GetTijd(true);                                                                               // Get the time and do not print it
 LEDstartup(green);                                                                           // Set the status LED to green
 SWversion();                                                                                 // Print the menu + version 
 StoreStructInFlashMemory();                                                                  // 
 delay(50);
 LEDstartup(CLEARLEDSTARTUP);                                                                 // Erase the startup LEDs from the clock display
 Tekstprintln("Setup finished");
    #ifdef ARDUINO_BOARD
 Tekstprint("Compiled on: "); Tekstprintln(ARDUINO_BOARD);
    #endif 
 Tekstprintlnf("Software version: %s", SoftwareName());
 Displaytime(); Tekstprintln("");
 msTick = millis();                                                                           // Start the seconds loop counter
}


//--------------------------------------------                                                //
// ARDUINO Loop
//--------------------------------------------
void loop() 
{
 Loopcounter++;                                                                               // a counter to check the speed of the loop
 CheckDevices();                                                                              // Check input from devices
 EverySecondCheck();                                                                          // Enter the command structure
}

//--------------------------------------------                                                //
// COMMON Check connected input devices
//--------------------------------------------
void CheckDevices(void)
{
 CheckBLE();                                                                                  // Something with BLE to do?
 SerialCheck();                                                                               // Check serial port every second
 CheckWIFIcommand();                                                                          // Check if there is a command given from the web page
 if (Mem.TimeReceiver) CheckTimeReceiverClient();                                             // Check if Timesender app is in the air
 if (Mem.TimeInput==1) RotaryEncoderCheck();
 if (Mem.TimeInput==2) Keypad3x1Check();                                                      // 
 if (Mem.TimeInput==3 || Mem.TimeInput==4) IrReceiverDecode();
 if (ntpJustSynced) CheckandPrintNTPsynced();
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
 static bool Toggle = 0;
 uint32_t msLeap = millis() - msTick;                                                         // 
 if (msLeap >999)                                                                             // Every second enter the loop
 {
  msTick = millis();
  GetTijd(false);                                                                             // Get the time for the seconds 
  Toggle = !Toggle;                                                                          // Used to turn On or Off Leds
  UpdateStatusLEDs(Toggle);
  DimLeds(TestLDR);                                                                           // Every second an intensity check and update from LDR reading 
  if (shouldReboot) { delay(2000);   ESP.restart(); }                                         // After finish OTA update restart
  if (timeinfo.tm_min != lastminute) EveryMinuteUpdate();                                     // Enter the every minute routine after one minute; 
  Loopcounter=0;
 }  
}

//--------------------------------------------                                                //
// COMMON Update routine done every minute
//-------------------------------------------- 
void EveryMinuteUpdate(void)
{   
 lastminute = timeinfo.tm_min;  
 if (IR_PowerOnstate && ((millis() - IR_StartTime) > 290000) ) ToggleIRpower();               // Turn off Power after 300 seconds 
 //GetTijd(false);
 bool ff = NoTextInLeds;                                                                      // flag that controls printing og text in ColorLeds()
 if (Mem.TimeLogPrint == 0 || Mem.TimeLogPrint == 2)                                          // Do not print time but do update the display                                                              // do not Print the time string every minute
    { NoTextInLeds = true; DimLeds(false); Displaytime(); GetTijd(false); }
 if (Mem.TimeLogPrint == 1)                                                                   // Print the time string every minute
    { NoTextInLeds = false; DimLeds(true); Displaytime(); GetTijd(false); }
 DimLeds(false);                                                                              // and DimLEDs are used to print the time every minute
 NoTextInLeds = ff;                                                                           // If Mem.TimeLogPrint == 0 the time (controlled in ColorLeds()
 if(timeinfo.tm_hour != lasthour) EveryHourUpdate(); 
}
//--------------------------------------------                                                //
// COMMON Update routine done every hour
//--------------------------------------------
void EveryHourUpdate(void)
{
 static uint32_t HourCount = 0;
 if (HourCount++ == 2)                                                                        // If the clock restarts between 1-2 hours after restart
    {
     Mem.MCUrestarted = 0;                                                                    // Startup went well; Set MCUrestart counter to 0    
     Tekstprintlnf("Mem.MCUrestarted reset to 0, No restarts in the first hour");
     StoreStructInFlashMemory();                                                              // Store the Mem.MCUrestarted    
    } 
 lasthour = timeinfo.tm_hour;
 if (!Mem.StatusLEDOn) SetStatusLED(0,0,0);                                                   // If for some reason the LEDs are ON and after a MCU restart turn them off.  
 if (Mem.TimeLogPrint == 0 )                                                                  // Do not print time but do update the display                                                              // do not Print the time string every minute
    { NoTextInLeds = true; DimLeds(false);  Displaytime();  GetTijd(false);   }
 if (Mem.TimeLogPrint == 2)                                                                   // Print the time string every hour
    { NoTextInLeds = false; DimLeds(true);  Displaytime();  GetTijd(true);  }           
 if ((timeinfo.tm_hour == Mem.TurnOffLEDsAtHH) && (Mem.TurnOffLEDsAtHH != Mem.TurnOnLEDsAtHH))
    { LEDsAreOff = true;  ClearScreen(); }                                                    // Is it time to turn off the LEDs?
 if (timeinfo.tm_hour == Mem.TurnOnLEDsAtHH)
    { LEDsAreOff = false;   lastminute = 99;     Displaytime(); }                             // Force a minute update
 CheckRestoreWIFIconnectivity();                                                              // Check if WIFI is still connected and if not restore it
 Mem.NVRAMmem[lasthour] = NoofLDRreadshour ? (byte)(SumLDRreadshour / NoofLDRreadshour) : 0;  // Update the average LDR readings per hour. Avoids dividing by zero
 SumLDRreadshour  = 0;
 NoofLDRreadshour = 0;
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
 Tekstprintln(PrintRTCTime());                                                                // Print time as dd-mmm-yy hh:mm:ss
 StoreStructInFlashMemory();                                                                  // Update Mem struct once a day to store Mem.NVRAM measurementd
}

//--------------------------------------------                                                //
// COMMON Init status LEDs
//-------------------------------------------- 
void InitStatusLEDs(void)
{
 analogWriteResolution(LED_RED,   9);                                                         // Pin resolution/frequency explicitly, before the first analogWrite() on these pins -
 analogWriteResolution(LED_GREEN, 9);                                                         // 9-bit (max 511) matches SetStatusLED()'s own 0-512 intensity scale directly, and the
 analogWriteResolution(LED_BLUE,  9);                                                         // core's implicit default for this has already silently changed across ESP32 core versions once before
 analogWriteFrequency(LED_RED,   5000);
 analogWriteFrequency(LED_GREEN, 5000);
 analogWriteFrequency(LED_BLUE,  5000);
 SetStatusLED(0,10,0);                                                                        // Set to green
 }
//--------------------------------------------                                                //
// COMMON Update routine for the status LEDs
//-------------------------------------------- 
void UpdateStatusLEDs(bool Toggle)
{
 if(Mem.StatusLEDOn)   
   {
    if (Toggle) SetStatusLED(( WiFi.localIP()[0]==0) * 20,                                    // no WIFI on 
                              (WiFi.localIP()[0]!=0) * 20,                                    // WIFI on
                              (BLEConnected)         * 20);                                   // BLE on
    else        SetStatusLED((TRConnected) * 20, (TRConnected) * 20, 0);                      // If TimeRreceiver Connected then Yellow = Red+Green                               
    SetPCBLED09(  Toggle * 10);                                                               // Left LED
    SetPCBLED10((!Toggle) * 10);                                                              // Right LED
    SetNanoLED13((!Toggle) * (!Mem.UseDS3231 * 50));                                          // LED on ESP32 board. IF DS3231 in use then it is off
   }
else                                                                                          // Turn off all LEDs
   {
    SetStatusLED(0,0,0);                  
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
void SetStatusLED(int Red, int Green, int Blue)
{
 analogWrite(LED_RED,   511 - Red);
 analogWrite(LED_GREEN, 511 - Green);
 analogWrite(LED_BLUE,  511 - Blue);
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
 Mem.DisplayChoice    = 0;                                                                    // Default colour scheme 
 Mem.OwnColour        = green;                                                                // Own designed colour.
 Mem.DimmedLetter     = dgray;
 Mem.BackGround       = black; 
 Mem.LanguageChoice   = 0;                                                                    // 0 = NL, 1 = UK, 2 = DE, 3 = FR, 4 = Wheel
 Mem.LightReducer     = SLOPEBRIGHTNESS;                                                      // Factor to dim ledintensity with. Between 0.1 and 1 in steps of 0.05
 Mem.UpperBrightness  = MAXBRIGHTNESS;                                                        // Upper limit of Brightness in bits ( 1 - 1023)
 Mem.LowerBrightness  = LOWBRIGHTNESS;                                                        // Lower limit of Brightness in bits ( 0 - 255)
 Mem.TurnOffLEDsAtHH  = 0;                                                                    // Display Off at nn hour
 Mem.TurnOnLEDsAtHH   = 0;                                                                    // Display On at nn hour Not Used
 Mem.TimeLogPrint     = 1;                                                                    // Print time per minute (1) or hour (2)
 Mem.HetIsWasOff      = 0;                                                                    // Turn On or Off HET IS WAS   
 Mem.EdSoftLEDSOn     = 0;                                                                    // Turn On or Off EDSOFT LEDs
 Mem.RandomDisplay    = 0;                                                                    // Choose every day another display
 Mem.WIFInoConnection = 0;                                                                    // Restarts after 30 minutes no connection 
 Mem.TimeReceiver     = 0;                                                                    // Use Time Sender app to set time 
 Mem.TimeSender       = 0;                                                                    // 
 Mem.Ringbufcnt       = 0;                                                                    // 
 Mem.HC12Time         = 1;                                                                    // Use HC12 module 
 Mem.byteFuture4      = 0;                                                                    // For future use
 Mem.BLEOn            = 1;                                                                    // default BLE On
 Mem.UseBLELongString = 0;                                                                    // Default off. works only with iPhone/iPad with BLEserial app
 Mem.NTPOn            = 1;                                                                    // NTP default off
 Mem.WIFIOn           = 1;                                                                    // WIFI default off
 Mem.MCUrestarted     = 0;                                                                    // MCU Restarts during Setup() counter caused by unknown errors during startup
 Mem.LoopRebooted     = 0;                                                                    // Reboots cause by less than 10 loops /second. Program hangs or severly delayed
 Mem.WIFIcredentials  = NOT_SET;                                                              // Status of the WIFI connection
 //Mem.TimeInput      = 0;    // Do not erase this setting with a reset                       // Use the rotary coding
 Mem.DCF77On          = 0;                                                                    // Default off
 Mem.UseDS3231        = 0;                                                                    // Default off
 //Mem.LEDstrip       = 0;    // Do not erase this setting with a reset                       // 0 = SK6812, 1=WS2812
 Previous_LDR_read    = ReadLDR();                                                            // Read LDR to have a start value. max = 4096/8 = 255
 MinPhotocell         = Previous_LDR_read;                                                    // Stores minimum reading of photocell;
 MaxPhotocell         = Previous_LDR_read;                                                    // Stores maximum reading of photocell;                                            
 TestLDR              = 0;                                                                    // If true LDR display is printed every second
 for (int i = 0; i < NOOFBUTTONS; i++)  IRMem.buttons[i].learned = 0;
 for (int i = 0; i < 24         ; i++)           Mem.NVRAMmem[i] = 0;
 Tekstprintln("**** Reset of preferences ****"); 
 //ResetCredentials();
 StoreStructInFlashMemory();                                                                  // Update Mem struct       
 GetTijd(false);                                                                              // Get the time and store it in the proper variables
 SWversion();                                                                                 // Display the version number of the software
 Displaytime(); Tekstprintln("");
}
//--------------------------------------------                                                //
// COMMON Reset to empty credential settings WIFI, NTP, BLE ON
//--------------------------------------------
void ResetCredentials(void)
{
 strcpy(Mem.SSID,"");                                                                         // Default SSID
 strcpy(Mem.Password,"");                                                                     // Default password
 strcpy(Mem.BLEbroadcastName,"ESP32Nano");
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
void Tekstprint(char const *tekst)    { if(Serial) Serial.print(tekst); SendMessageTimeReceiver(tekst); SendMessageBLE(tekst); AddLog(tekst);}
void Tekstprintln(char const *tekst)  { snprintf(sptext, sizeof(sptext),"%s\n",tekst); Tekstprint(sptext); }
void TekstSprint(char const *tekst)   { printf(tekst); }                                     // printing for Debugging purposes in serial monitor 
void TekstSprintln(char const *tekst) { snprintf(sptext, sizeof(sptext),"%s\n",tekst); TekstSprint(sptext); }

//--------------------------------------------                                                //
// COMMON Formatted Tekst print
// Usage: Tekstprintlnf("Log buffer allocated: %u bytes", (unsigned) LogBufferSize);
//--------------------------------------------
void Tekstprintf(const char* fmt, ...) 
{
 va_list args; va_start(args, fmt); vsnprintf(sptext, sizeof(sptext), fmt, args); va_end(args);
 Tekstprint(sptext);
}
//--------------------------------------------                                                //
// COMMON Formatted Tekstprintln
// Usage: Tekstprintf("Log buffer allocated: %u bytes", (unsigned) LogBufferSize);
//--------------------------------------------
void Tekstprintlnf(const char* fmt, ...) 
{
 va_list args; va_start(args, fmt); vsnprintf(sptext, sizeof(sptext), fmt, args); va_end(args);
 Tekstprintln(sptext);
}

//--------------------------------------------                                                //
// COMMON Print web menu page and BLE menu
// 0 = text to print, 1 = header of web page with menu, 2 = footer of web page
// html_info must be empty before starting: --> html_info[0] = 0; 
//--------------------------------------------
void WTekstappend(char const *tekst, char const *prefix, char const *suffix, bool newline) 
{
 if (newline) { snprintf(sptext, sizeof(sptext), "%s\n", tekst); } 
 else {         snprintf(sptext, sizeof(sptext), "%s",   tekst); }
 Tekstprint(sptext);     
 size_t needed = strlen(prefix) + strlen(tekst) + strlen(suffix) + strlen("<br>");            // Estimate how much space will be added
 if (strlen(html_info) + needed > MAXSIZE_HTML_INFO - 1) 
     { strcat(html_info, "<br> *** Increase MAXSIZE_HTML_INFO ***<br>");  return;  }
 strcat(html_info, prefix);                                             
 strcat(html_info, tekst);
 strcat(html_info, suffix);
 if (newline) { strcat(html_info, "<br>"); }   // Append to html_info
}

void WTekstprintln(char const *tekst) { WTekstappend(tekst, "", "", true);}
void WTekstprintln(char const *tekst, char const *prefix, char const *suffix) 
                                      { WTekstappend(tekst, prefix, suffix, true); }
void WTekstprint(char const *tekst)   { WTekstappend(tekst, "", "", false);}
void WTekstprint(char const *tekst, char const *prefix, char const *suffix) 
                                      { WTekstappend(tekst, prefix, suffix, false);}

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
 GetStructFromFlashMemory();
 InitLogBuffer();     Tekstprintln("Ringbuffer initialysed");                                 // For Log lines Ringbuffer
 if( Mem.Checksum != 25065)
   {
    Tekstprintlnf("Checksum (25065) invalid: %d\n Resetting to default values",Mem.Checksum); 
    Reset();                                                                                  // If the checksum is NOK the Settings were not set
    ResetCredentials(); 
   }

 Mem.LightReducer    = constrain(Mem.LightReducer,1,250);                                     // 
 Mem.LowerBrightness = constrain(Mem.LowerBrightness, 1, 250);                                // 
 Mem.UpperBrightness = constrain(Mem.UpperBrightness, 1, 255); 
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
 Tekstprintlnf("Get Mem Struct done: Mem.Checksum = %d",Mem.Checksum);
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
// COMMON Print Software naam without path
/* for MacOS/Linux / and Windows \. */
//--------------------------------------------
const char* SoftwareName(void)
{
 const char* path = __FILE__;
 const char* end  = strstr(path, ".ino");
 if (!end) return path;
 const char* p = end;
 while (p > path && *(p - 1) != '/' && *(p - 1) != '\\')   p--;
 return p;
}
//--------------------------------------------                                                //
// COMMON Version info
//--------------------------------------------
void SWversion(void) {SWversion(ShortMenu);}                                                  // ShortMenu is default after startup true (= small)   
void SWversion(bool Small) 
{ 
 html_info[0] = 0;                                                                           // Empty the info web page to be used in void WTekstprintln() 
 ShortMenu = Small;
 PrintLine(35);
 if(Small) {for (uint8_t i = 0; i < sizeof(menusmall) / sizeof(menusmall[0]);                   WTekstprintln(menusmall[i++]) ); }
 else      {for (uint8_t i = 0; i < sizeof(menu) / sizeof(menu[0]);                             WTekstprintln(menu[i++]) ); }                                     
 PrintLine(35);
 snprintf(sptext, sizeof(sptext),"Display off between: %02dh - %02dh",Mem.TurnOffLEDsAtHH, Mem.TurnOnLEDsAtHH); WTekstprintln(sptext);
 if(!Small) { snprintf(sptext, sizeof(sptext), "RandomDisplay:%s, Timestamp:%s", 
               Mem.RandomDisplay==1 ? "On/min" :  Mem.RandomDisplay==2 ? "On/hour" : "Off",
               Mem.TimeLogPrint==1 ? "/min" : Mem.TimeLogPrint==2 ? "/hour" : " Off" );         WTekstprintln(sptext); } 
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
 char fftext[20], ffftext[20];              
 if(!Small) {snprintf(fftext, sizeof(fftext),"%s", Mem.UseDS3231?"DS3231=On":"DS3231=Off"); }
 if(!Small) {snprintf(ffftext,sizeof(ffftext),"%s", Mem.HC12Time?"HC-12Time=On":"HC-12Time=Off");} 
 if(!Small) {snprintf(sptext, sizeof(sptext),"%s %s %s",Mem.TimeInput==0 ?"H00=On":
                        Mem.TimeInput==1 ?"Rotary=On":
                        Mem.TimeInput==2 ?"Membrane=On":
                        Mem.TimeInput==3 ?"IR-remote=On":
                        Mem.TimeInput==4 ?"Ir-remote=On":"NOP",fftext,ffftext);                 WTekstprintln(sptext); }    
  if(!Small) {snprintf(sptext, sizeof(sptext),"%s %s",
                               Mem.TimeInput==5?"TimeReceiver=On":"TimeReceiver=Off",
                               Mem.TimeReceiver?"TimeSender=On":"TimeSender=Off");              WTekstprintln(sptext); }                                
 if(!Small) {snprintf(sptext, sizeof(sptext),"%s strip with %d LEDs (switch %%)", 
                 Mem.LEDstrip==0?"SK6812":Mem.LEDstrip==1?"WS2812":"NOP",(int) NUM_LEDS);       WTekstprintln(sptext); }
 if(!Small) {snprintf(sptext, sizeof(sptext),"Software: %s", SoftwareName());                   WTekstprintln(sptext);}
 if(!Small) {snprintf(sptext, sizeof(sptext),"ESP32 Arduino core version: %d.%d.%d", 
          ESP_ARDUINO_VERSION_MAJOR,ESP_ARDUINO_VERSION_MINOR,ESP_ARDUINO_VERSION_PATCH);       WTekstprintln(sptext); }
 GetTijd(false);                                                                              // Get the time and store it in the proper variables
 PrintLine(35);                                                                               //
}

//--------------------------------------------                                                //
// COMMON PrintLine Print a _ line of n length
//--------------------------------------------
void PrintLine(byte Lengte)
{
 for(int n=0; n<Lengte; n++) sptext[n]='_';
 sptext[Lengte] = 0;
 WTekstprintln(sptext);
 sptext[0] = 0;
}

//--------------------------------------------                                                //
// COMMON Input from Bluetooth, Serial or HTML page
//--------------------------------------------
void ReworkInputString(String InputString)
{
 if(InputString.length()> 40){Tekstprintln("Input string too long (max40)\n"); return;}       // If garbage return
 InputString.trim();                                                                          // Remove CR, LF etc.
 sptext[0] = 0;                                                                               // Suppress a second print of sptext 
 PendingCommand = "";                                                                         // Clear the global PendingCommand
 if(InputString[0] > 31 && InputString[0] < 127)                                              // Does the string start with a letter?
  {
  int len = InputString.length();
  switch(toupper(InputString[0])) 
   {
    case 'A':                                                                                 // SSID setting
      if(len > 3 && len < 30) 
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
      if(len > 4 && len < 40) 
        {
        InputString.substring(1).toCharArray(Mem.Password, len);
        snprintf(sptext, sizeof(sptext), "Password set: %s\n Enter @ to reset ESP32 and connect to WIFI and NTP\n WIFI and NTP are turned ON", Mem.Password);
        Mem.NTPOn = Mem.WIFIOn = 1;                                                           // Turn both on
        Mem.WIFIcredentials = NOT_SET;
        WIFIwasConnected = false;
        } 
      else snprintf(sptext, sizeof(sptext), "**** Length fault. Use between 5 and 40 characters ****");
      break;

    case 'C':                                                                                // BLE settings
      if(InputString.equals("CCC")) 
       {
        Mem.BLEOn = 1 - Mem.BLEOn;
        snprintf(sptext, sizeof(sptext), "BLE is %s after restart", Mem.BLEOn ? "ON" : "OFF");
        break;
       }
      if(len > 4 && len < 30) 
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
      if(len > 2) 
       {
        InputString.substring(1).toCharArray(Mem.Timezone, len);
        snprintf(sptext, sizeof(sptext), "Timezone set: %s", Mem.Timezone);
       } 
      else snprintf(sptext, sizeof(sptext), "**** Length fault. Use more than 2 characters ****");
      break;

    case 'F':                                                                                 // *** Empty *** 
       snprintf(sptext, sizeof(sptext), "**** No Use option . ****");
       break; 

    case 'G':                                                                                 // Scan WIFI stations
      if(len == 1) 
       {
        CheckforWIFINetwork(true);
        if(WIFIwasConnected) WiFi.reconnect();
       } 
      else snprintf(sptext, sizeof(sptext), "**** Length fault. Enter G ****");
      break;

    case 'H':                                                                                 // Use rotary encoder
      if(len == 3) 
       {
        byte Choice = (byte)SConstrainInt(InputString, 2, 0, 5);                                 // keep the range between 0 and 5; H00 - H05
        if (Choice <5) Mem.TimeInput = Choice;
        else
            {
            if (Choice ==5) Mem.TimeReceiver = 1 - Mem.TimeReceiver;
            }
        if(Mem.TimeInput > 4) Mem.TimeInput = 0;
        Tekstprintln("*** Restart with @ to activate Rotary, key pad, Time sender app or IR remote ***");    // 
        if(Mem.TimeInput > 0)  {Mem.NTPOn = 0;              Mem.UseDS3231 = 1;}              // Configure related settings based on rotary use 
        else                   {Mem.WIFIOn = Mem.NTPOn = 1; Mem.UseDS3231 = 0;}
        } 
      else Tekstprintln("Error **** Enter H00 (none), H01=Rotary, H02=Membrane, H03 large IR-remote, H04 tiny IR-remote, H05 Time sender app ****");
      sprintf(sptext, "\n Use of rotary encoder is %s\n"
                        "         Use of keypad is %s\n"
                        "Use of large IR-remote is %s\n"
                        " Use of tiny IR-remote is %s\n"
                        "Use of Time sender app is %s",                        
                        Mem.TimeInput == 1 ? "ON" : "OFF", Mem.TimeInput == 2 ? "ON" : "OFF",
                        Mem.TimeInput == 3 ? "ON" : "OFF", Mem.TimeInput == 4 ? "ON" : "OFF",
                        Mem.TimeReceiver   ? "ON" : "OFF");
      Tekstprintln(sptext);      
      sprintf(sptext, "Use DS3231 is %s, WIFI is %s, NTP is %s\n *** Restart clock with @ ***", 
                       Mem.UseDS3231 ? "ON" : "OFF", Mem.WIFIOn ? "ON" : "OFF", Mem.NTPOn ? "ON" : "OFF");     
      if(Mem.TimeInput >2) { while(IrReceiver.decode())  IrReceiver.resume();  }              // Clear all pending commands    
      break;

    case 'I':                                                                                 // Menu
      if(InputString.equalsIgnoreCase("I"))  ShortMenu = true;
      if(InputString.equalsIgnoreCase("II")) ShortMenu = false;
      SWversion(ShortMenu);                                                                   // ShortMenu is true for small menu, false for full menu
      break;
      
    case 'J':                                                                                 // Use DS3231 RTC module
      if(len == 1)
      {
        if(Mem.UseDS3231)                                                                      // Turning DS3231 OFF
         {
          if(Mem.WIFIcredentials == SET_AND_OK)
           {
            Mem.UseDS3231 = 0;
            Mem.WIFIOn    = 1;
            Mem.NTPOn     = 1;
            StoreStructInFlashMemory();
            snprintf(sptext, sizeof(sptext), "DS3231=Off, WIFI=On, NTP=On (restart required)");
           }
          else
           {
            snprintf(sptext, sizeof(sptext), "*** No valid WIFI credentials. DS3231 remains ON ***");
           }
         }
        else                                                                                   // Turning DS3231 ON
         {
          Mem.UseDS3231 = 1;
          Mem.NTPOn     = 0;
          StoreStructInFlashMemory();
          snprintf(sptext, sizeof(sptext), "DS3231=On, NTP=Off, WIFI=%s", Mem.WIFIOn ? "ON" : "OFF");
         }
       }
      else snprintf(sptext, sizeof(sptext), "**** Length fault. Enter J ****");
      break;
      
    case 'K':  
      snprintf(sptext, sizeof(sptext), "K Test LDR, Print timestamp. K0=Off, K1=per min, K2 is per hour");
      if (len == 1)
        {                                                                                     // Test LDR
         TestLDR = 1 - TestLDR;
         snprintf(sptext, sizeof(sptext), "TestLDR: %s", TestLDR ? "On\n   Bits, Out, loops per second and time" : "Off\n");
         if(Mem.HC12Time)  Serial1.print("W"); //        if(Mem.HC12Time)  Serial1.print(InputString);
        }
       if(InputString.equalsIgnoreCase("K0"))                                                 // do not log
        {
         Mem.TimeLogPrint = 0;
         snprintf(sptext, sizeof(sptext), "Time display per minute and hour is: %s", Mem.TimeLogPrint ? "ON\n" : "OFF\n");  
        } 
       if(InputString.equalsIgnoreCase("K1"))                                                 // Log per minute
        {
         Mem.TimeLogPrint = 1;
         snprintf(sptext, sizeof(sptext), "Time display per minute is: %s", Mem.TimeLogPrint ? "ON\n" : "OFF\n");  
        } 
       if(InputString.equalsIgnoreCase("K2"))                                                 // Log per hour
        {
         Mem.TimeLogPrint = 2;
         snprintf(sptext, sizeof(sptext), "Time display per hour is: %s", Mem.TimeLogPrint ? "ON\n" : "OFF\n");  
        }       
      break;

    case 'L':                                                                                 // Lower brightness
      if(len > 1 && len < 5) 
       {
        Mem.LowerBrightness = (byte)SConstrainInt(InputString, 1, 0, 250);
        snprintf(sptext, sizeof(sptext), "Lower brightness: %d bits", Mem.LowerBrightness);
        if(Mem.HC12Time)  Serial1.println(InputString);
       } 
      else snprintf(sptext, sizeof(sptext), "**** Input fault. \nEnter Lnnn where n between 1 and 250");
      break;
      
    case 'M':                                                                                 // Max brightness
      if(len > 1 && len < 5) 
       {
        Mem.UpperBrightness = SConstrainInt(InputString, 1, 1, 255);
        snprintf(sptext, sizeof(sptext), "Upper brightness changed to: %d bits", Mem.UpperBrightness);
        if(Mem.HC12Time)  Serial1.println(InputString);
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
        if(Mem.HC12Time)  Serial1.println(InputString);
      break;
      
case 'O':                                                                                 // Turn On/Off Display
      if(len == 1) 
       {
        LEDsAreOff = !LEDsAreOff;
        if(Mem.HC12Time) {Serial1.println(InputString);}                                      // Send raw command to HC-12
        if(LEDsAreOff) { ClearScreen(); }
        snprintf(sptext, sizeof(sptext), "Display is %s", LEDsAreOff ? "OFF" : "ON");
        Tekstprintln(sptext);
        lastminute = 99;                                                                    // Force display update
        sptext[0]=0;                                                                        // Suppress a second print of sptext
       } 
      else snprintf(sptext, sizeof(sptext), "**** Length fault O. ****");
      break;
      
    case 'P':                                                                                 // Status LEDs On/Off
      if(len == 1) 
        {
         Mem.StatusLEDOn = !Mem.StatusLEDOn;
         UpdateStatusLEDs(0);
         snprintf(sptext, sizeof(sptext), "StatusLEDs are %s", Mem.StatusLEDOn ? "ON" : "OFF");
        } 
      else snprintf(sptext, sizeof(sptext), "**** Length fault P. ****");
      break;

    case 'Q':                                                                                 // *** Empty *** 
      if(len == 1) 
        {
         Mem.TimeSender = 1 - Mem.TimeSender;
         if (Mem.TimeSender)  Mem.TimeReceiver = 0;                                           // turn off TimeReceiver                                                                                                    
         snprintf(sptext, sizeof(sptext), "TimeSender is %s after restart (@)", Mem.TimeSender ? "ON" : "OFF");
        } 
      else snprintf(sptext, sizeof(sptext), "**** Length fault Q. ****");
      break;

    case 'R':                                                                                 // Reset to default settings
      if(InputString.equals("RRRRR"))                                                         // Delete WIFI settings and set defaults
      {
        Reset();
        ResetCredentials();
        shouldReboot = true; // ESP.restart();
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
        snprintf(sptext, sizeof(sptext), "\nReset to default values: Done");                                                                  // Force a minute update
        Displaytime(); Tekstprintln("");
       } 
      else snprintf(sptext, sizeof(sptext), "**** Length fault R. ****");
      break;
      
    case 'S':                                                                                 // Slope factor for brightness
      if(len > 1 && len < 5) 
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
        SetRTCTime();                                                                         // Always sync ESP32 internal RTC
        if(Mem.HC12Time)  Serial1.println(InputString.c_str());
        if(DS3231Installed)
         {
          SetDS3231Time();                                                                    // Write time to DS3231
          Mem.UseDS3231 = 1;                                                                 // Time source = DS3231
          Mem.NTPOn     = 0;                                                                 // NTP not needed
          Mem.WIFIOn    = 0;                                                                 // WIFI not needed
          StoreStructInFlashMemory();                                                        // Persist settings
          snprintf(sptext, sizeof(sptext), "Time set. DS3231=On, WIFI=Off, NTP=Off");
         }
        else
         {
          Mem.UseDS3231 = 0;
          snprintf(sptext, sizeof(sptext), "Time set (no DS3231). WIFI/NTP unchanged.");
         }
       } 
      else snprintf(sptext, sizeof(sptext), "**** Length fault. Enter Thhmmss ****");
      break;

    case 'U':                                                                                 // *** Empty *** 
      if(len == 1) 
        {
         Mem.HC12Time = 1 - Mem.HC12Time;                                  // turn off TimeReceiver                                                                                                    
         snprintf(sptext, sizeof(sptext), "HC12 TimeSender is %s after restart (@)", Mem.HC12Time ? "ON" : "OFF");
        } 
      else snprintf(sptext, sizeof(sptext), "**** Length fault U. ****");
      break;
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

    case 'Y':       
                 // function that will run > 1sec can be placed here                                                                                  // *** Empty *** 
       snprintf(sptext, sizeof(sptext), "**** No Use option . ****");
      break;

    case 'Z':                                                                                 // Start WPS
      if(len == 1) 
       {
        snprintf(sptext, sizeof(sptext), "**** Start WPS on your router");
        WiFi.onEvent(WiFiEvent);
        WiFi.mode(WIFI_STA);
        Tekstprintln("Starting WPS");
        wpsInitConfig();
        wpsStart();
       }
      break;
      
    case '!':                                                                                 // Print times
      if(len == 1) PrintAllClockTimes();
      sprintf(sptext,"\n");
      break;
      
    case '@':                                                                                 // Reset ESP
      if(len == 1) 
       {
        Mem.MCUrestarted = 0;                                                                 // Reset MCUrestart counter to 0    
        Tekstprintlnf("Mem.MCUrestarted reset to 0");
        StoreStructInFlashMemory();
        Tekstprintln("\n*********\n ESP restarting\n*********\n");            
        shouldReboot = true; // ESP.restart();
       } 
      else sprintf(sptext, "**** Length fault. Enter @ ****");
      break;
 
    case '#':                                                                                 // *** Empty *** 
        snprintf(sptext, sizeof(sptext), "**** No Use option . ****");
      break;

    case '$':                                                                                 // *** Empty *** 
       snprintf(sptext, sizeof(sptext), "**** No Use option . ****");
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
       snprintf(sptext, sizeof(sptext), "**** No Use option . ****");
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
      snprintf(sptext, sizeof(sptext), "**** No Use option . ****");     
      break;

    case ')':
      snprintf(sptext, sizeof(sptext), "**** No Use option . ****");     
      break;

    case '{':  
      if (Mem.TimeInput==3 || Mem.TimeInput==4)
       {
         StartIRLearning();                                                                   // Initializes learning mode,Resets all button data, Prompts for first button
         snprintf(sptext, sizeof(sptext), "Learning IR finished");
       }
      else snprintf(sptext, sizeof(sptext), "IR remote is OFF (Turn it on with H03 or H04 )\nOr no IR remote is installed");
      break;

    case '}':                                                                                 // *** Empty ***
      if(len == 1) 
        {
         Mem.Ringbufcnt = !Mem.Ringbufcnt;
         snprintf(sptext, sizeof(sptext), "Ring buffer counter is %s", Mem.Ringbufcnt ? "ON" : "OFF");
        } 
      else snprintf(sptext, sizeof(sptext), "**** Length fault . ****");
      break;


   case '+':                                                                                 // BLE string toggle
      if(len == 1) 
       {
        Mem.UseBLELongString = 1 - Mem.UseBLELongString;
        snprintf(sptext, sizeof(sptext), "Fast BLE is %s", Mem.UseBLELongString ? "ON" : "OFF");
       } 
      else snprintf(sptext, sizeof(sptext), "**** Length fault. Enter + ****");
      break;

    case '_':                                                                                 // NO USE
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
    case '2':                                                                                 // Time entry compatibility mode
      if(len == 6)                                                                            // The first digit of InputString is 0, 1 or 2
       {
        timeinfo.tm_hour = (int)SConstrainInt(InputString, 0, 2, 0, 23);
        timeinfo.tm_min  = (int)SConstrainInt(InputString, 2, 4, 0, 59);
        timeinfo.tm_sec  = (int)SConstrainInt(InputString, 4, 6, 0, 59);
        SetRTCTime();                                                                         // Always sync ESP32 internal RTC
        if(DS3231Installed)
         {
          SetDS3231Time();                                                                    // Write time to DS3231
          Mem.UseDS3231 = 1;                                                                  // Time source = DS3231
          Mem.NTPOn     = 0;                                                                  // NTP not needed
          Mem.WIFIOn    = 0;                                                                  // WIFI not needed
          StoreStructInFlashMemory();                                                         // Persist settings
          snprintf(sptext, sizeof(sptext), "Time set. DS3231=On, WIFI=Off, NTP=Off");
         }
        else
         {
          Mem.UseDS3231 = 0;
          snprintf(sptext, sizeof(sptext), "Time set (no DS3231). WIFI/NTP unchanged.");
         }
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
  Tekstprintlnf("Time inputMethod: %s", Method);
}
//--------------------------------------------                                                //
// COMMON Print permanent Mem memory
//--------------------------------------------
void PrintMem(void)
{
 PrintLine(35); 
 PrintHeaps(); 
 PrintLine(35); 
 for (int i=0;i<12;i++) { Tekstprintf("%03d ",Mem.NVRAMmem[i]); }   Tekstprintln("");
 for (int i=12;i<24;i++){ Tekstprintf("%03d ",Mem.NVRAMmem[i]); }   Tekstprintln(""); 
 PrintAllMappings();                                                                          // Print the IR remote keys 
 Tekstprintlnf("Q  DisplayChoice: %d",Mem.DisplayChoice);  
 Tekstprintlnf(" TurnOffLEDsAtHH: %d",Mem.TurnOffLEDsAtHH);                 
 Tekstprintlnf("  TurnOnLEDsAtHH: %d",Mem.TurnOnLEDsAtHH); 
 Tekstprintlnf("F    Font colour: 0X%08" PRIX32, Mem.OwnColour);            
 Tekstprintlnf("    DimmedLetter: 0X%08" PRIX32,Mem.DimmedLetter);          
 Tekstprintlnf("      BackGround: 0X%08" PRIX32,Mem.BackGround);                   
 Tekstprintlnf("  LanguageChoice: %d",Mem.LanguageChoice);                  
 Tekstprintlnf("S   LightReducer: %d",Mem.LightReducer);                    
 Tekstprintlnf("L  MinBrightness: %d",Mem.LowerBrightness);                 
 Tekstprintlnf("M  MaxBrightness: %d",Mem.UpperBrightness);                                                                                                     
 Tekstprintlnf("CCC        BLEOn: %s",Mem.BLEOn ? "ON" : "OFF");            
 Tekstprintlnf("X          NTPOn: %s",Mem.NTPOn ? "ON" : "OFF");            
 Tekstprintlnf("W         WIFIOn: %s",Mem.WIFIOn ? "ON" : "OFF");           
 Tekstprintlnf("P    StatusLEDOn: %s",Mem.StatusLEDOn ? "ON" : "OFF");      
 Tekstprintlnf("    MCUrestarted: %d",Mem.MCUrestarted );                   
 Tekstprintlnf("    LoopRebooted: %d",Mem.LoopRebooted );                   
 Tekstprintlnf("K Time log print: %s",Mem.TimeLogPrint == 0 ? "OFF" : 
                                      Mem.TimeLogPrint == 1 ? "Per minute" : 
                                      Mem.TimeLogPrint == 2 ? "Per hour" : "Undefined");         
 Tekstprintlnf("         DCF77On: %s",Mem.DCF77On  ? "ON" : "OFF");         
 Tekstprintlnf("H05 TimeReceiver: %s",Mem.TimeReceiver ? "ON" : "OFF"); 
 Tekstprintlnf("      TimeSender: %s",Mem.TimeSender ? "ON" : "OFF");   
 Tekstprintlnf("H      TimeInput: %d",Mem.TimeInput);                       
 PrintTimeInputMethod(Mem.TimeInput);
 Tekstprintlnf("J      UseDS3231: %s",Mem.UseDS3231 ? "ON" : "OFF");        
Tekstprintlnf("%%      LED strip: %s",Mem.LEDstrip?"WS2812":"SK6812" );    
 Tekstprintlnf("      FiboChrono: %s",Mem.FiboChrono ? "FIBO" : "CHRONO");  
 Tekstprintlnf("          NoExUl: %s",Mem.NoExUl ? "ON" : "OFF");          
 byte wc = Mem.WIFIcredentials; 
 Tekstprintlnf(" WIFIcredentials: %s", wc==0? "Not SET" : wc==1? "SET" : wc==2? "SET&OK": 
                                       wc==3? "in AP not SET":"Unknown code");   
 Tekstprintlnf("      IntFuture2: %d",Mem.IntFuture2 );  
 Tekstprintlnf("}     Ringbufcnt: %s",Mem.Ringbufcnt ? "ON" : "OFF");                     
 Tekstprintlnf("        HC12Time: %s",Mem.HC12Time ? "ON" : "OFF");  
 Tekstprintlnf("     byteFuture4: %d",Mem.byteFuture4 );                                         
 Tekstprintlnf(")     HET IS WAS: %s",Mem.HetIsWasOff ? "OFF" : "ON");     
 Tekstprintlnf("(      EDSOFT is: %s",Mem.EdSoftLEDSOn ? "ON" : "OFF");    
 Tekstprintlnf("~  RandomDisplay: %s",Mem.RandomDisplay? "ON" : "OFF");    
 Tekstprintlnf("WIFInoConnection: %d",Mem.WIFInoConnection);               
 Tekstprintlnf("+       Fast BLE: %s",Mem.UseBLELongString?"ON":"OFF");               
 Tekstprintlnf("A           SSID: %s",Mem.SSID);                            
 // Tekstprintlnf("BackGround: %s",Mem.Password);                           
 Tekstprintlnf("C  BroadcastName: %s",Mem.BLEbroadcastName);                
 Tekstprintlnf("E       Timezone: %s",Mem.Timezone);                        
 Tekstprintlnf("        Checksum: %d",Mem.Checksum);                        
 sptext[0]=0;                                                                                 // Suppress a second print of sptext
}

//--------------------------------------------                                                //
// COMMON Print memory space
//--------------------------------------------
void StoredStartHeaps(){ StoredStartHeaps(false); }
void StoredStartHeaps(bool Store)
{
 static uint32_t a,b,c,d,e,f;
 if(Store)
   {
    a = ESP.getFreeHeap();
    b = heap_caps_get_largest_free_block(MALLOC_CAP_8BIT);
    c = esp_get_minimum_free_heap_size();
    d = heap_caps_get_free_size(MALLOC_CAP_8BIT);
    e = heap_caps_get_free_size(MALLOC_CAP_DMA);
    f = heap_caps_get_free_size(MALLOC_CAP_SPIRAM);
   }
  else
  {
  Tekstprintln("Heaps before allocation of the ringbuffer");
  Tekstprintlnf("         Free heap: %7.1f Kbytes", a / 1024.0);
  Tekstprintlnf("     Largest block: %7.1f Kbytes", b / 1024.0);
  Tekstprintlnf("     Min free heap: %7.1f Kbytes", c / 1024.0);
  Tekstprintlnf("8-bit capable heap: %7.1f Kbytes", d / 1024.0);
  Tekstprintlnf("  DMA capable heap: %7.1f Kbytes", e / 1024.0);
  Tekstprintlnf(" SPRAM/SPIRAM heap: %7.1f Kbytes", f / 1024.0);
  PrintLine(35);
  }
}
//--------------------------------------------                                                //
// COMMON Print memory space
//--------------------------------------------
void PrintHeaps(void)
{
 StoredStartHeaps();  
 Tekstprintln("Actual heap sizes");                                                           // Print the heaps before allocation
 Tekstprintlnf("         Free heap: %7.1f Kbytes", ESP.getFreeHeap() / 1024.0);
 Tekstprintlnf("     Largest block: %7.1f Kbytes", heap_caps_get_largest_free_block(MALLOC_CAP_8BIT) / 1024.0);
 Tekstprintlnf("     Min free heap: %7.1f Kbytes", esp_get_minimum_free_heap_size()           / 1024.0);
 Tekstprintlnf("8-bit capable heap: %7.1f Kbytes", heap_caps_get_free_size(MALLOC_CAP_8BIT)   / 1024.0);
 Tekstprintlnf("  DMA capable heap: %7.1f Kbytes", heap_caps_get_free_size(MALLOC_CAP_DMA)    / 1024.0);
 Tekstprintlnf(" SPRAM/SPIRAM heap: %7.1f Kbytes", heap_caps_get_free_size(MALLOC_CAP_SPIRAM) / 1024.0);
}

//--------------------------------------------                                                //
// COMMON LDR reading are between 0 and 255. 
// ESP32 analogue read is between 0 - 4096 --   is: 4096 / 8
//--------------------------------------------
int ReadLDR(void)
{
 return analogRead(PhotoCellPin) / 16;
}

//--------------------------------------------                                                //
// CLOCK Display the time 
// This function is controlled by Mem.TimeLogPrint and called when time is changed by some input
//--------------------------------------------
void Displaytime(void)
{ 
 if(Mem.HC12Time) SendHC12TimeString();                                                       // HC12 Send time via HC-12 on D4
}

//--------------------------------------------                                                //
// CLOCK Read LDR and update brightness variables
//--------------------------------------------
void ReadAndAverageLDR(void) {ReadAndAverageLDR(false);}
void ReadAndAverageLDR(bool clean)
{
 LDRread           = ReadLDR();                                                               // Raw LDR value (also used for print in DimLeds)
 int LDRavgread    = clean ? LDRread                                                          // Clean read: bypass smoothing, reset average directly
                           : (4 * Previous_LDR_read + LDRread) / 5;                          // Normal read: smooth rapid light intensity changes
 Previous_LDR_read = LDRavgread;
 OutPhotocell      = (uint32_t)((Mem.LightReducer * sqrt(255 * LDRavgread)) / 100);          // Linear --> hyperbolic with sqrt. Result is between 0-255
 OutPhotocell      = constrain(OutPhotocell, Mem.LowerBrightness, Mem.UpperBrightness);
 MinPhotocell      = min(MinPhotocell, LDRavgread);
 MaxPhotocell      = max(MaxPhotocell, LDRavgread);
 SumLDRreadshour  += LDRavgread;   NoofLDRreadshour++;
}

//--------------------------------------------                                                //
// CLOCK Dim the leds measured by the LDR and print values
// LDR reading are between 0 and 255. The Brightness send to the LEDs is between 0 and 255
//--------------------------------------------
void DimLeds(bool print)
{
 ReadAndAverageLDR();
 if(print)
   {
    PrintTimeHMS(1);                                                                          //
    char tempStr[10] = "";
    char posStr[30]  = "";
    if (Mem.UseDS3231)  snprintf(tempStr, sizeof(tempStr), "%0.0fC", RTCklok.getTemperature());
    if (Mem.Ringbufcnt) snprintf(posStr, sizeof(posStr), "@%u ->%u", LogWritePos, LogBufferSize-LogWritePos);
    snprintf(sptext, sizeof(sptext), "LDR:%3d->%2d%% %3lu kl/s %s%s ",
            LDRread, (int)(OutPhotocell * 100 / 255), Loopcounter / 1000, posStr, tempStr);               
    if(TestLDR) Tekstprintln(sptext);
    else Tekstprint(sptext); 
    sptext[0] = 0;
   }
 if(LEDsAreOff) OutPhotocell = 0;
 SetBrightnessLeds(OutPhotocell);     // values between 0 and 255
}

// --------------------Colour Clock Light functions -----------------------------------
//--------------------------------------------                                                //
// LED Set color for LEDs in strip and print tekst
//---------------------------------------------
void ColorLeds(char const *Tekst, int FirstLed, int LastLed, uint32_t RGBWColor)
{ 
 Stripfill(RGBWColor, FirstLed, ++LastLed - FirstLed );                                        //
 if (!NoTextInLeds && strlen(Tekst) > 0 )
     { snprintf(sptext, sizeof(sptext),"%s ",Tekst); Tekstprint(sptext); }                     // Print the text  
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
// brief Convert HSV values to packed RGBW format (white = 0).
// param H Hue angle (0–360) param S Saturation (0–1) param V Value/Brightness (0–1)
// return Packed RGBW uint32_t value
//--------------------------------------------
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
LEDstrip.begin();
LEDstrip.setBrightness(16);  
LedsOff();                                                                                    // Set initial brightness of LEDs  (0-255)  
ShowLeds();
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
 if (DS3231Installed )
   {
    GetDS3231Time(true);
    Tekstprint("DS3231 -> ");  
    SetSystemTime(mktime(&timeinfo));                                                         // Sync ESP32 RTC from DS3231 to have a better time
   }
 if (Mem.NTPOn && WiFi.isConnected())
    {
     GetNTPtime(false);
     Tekstprint("   NTP -> ");
     SetSystemTime(mktime(&timeinfo));                                                        // Sync ESP32 RTC from NTP
     if (DS3231Installed)
        {
         DateTime Inow = RTCklok.now();
         if (RTCklok.lostPower()) SetDS3231Time();
         if (timeinfo.tm_year > (2024 - 1900) && Inow.year() < 2025) SetDS3231Time();         // Sync DS3231 if time is years too old
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
 settimeofday(&Tv, NULL);                                                                     // settimeofday(&tv, nullptr) This is the actual system clock update
 localtime_r(&Now, &timeinfo);                                                                // This is independent of setting the system time. It converts the UNIX timestamp into the timeinfo struct so your program can use it.
 snprintf(sptext, sizeof(sptext),"System time set to compile time: %s %s", __DATE__, __TIME__);
 Tekstprintln(sptext);
 Tekstprintln(PrintRTCTime());
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
 if (printit) {Tekstprintln(PrintRTCTime());}
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
const char* PrintNTPtime(void)
{
 getLocalTime(&timeinfo);
 return(PrintRTCTime()); 
}

//--------------------------------------------                                                //
// NTP print the NTP UTC time 
//--------------------------------------------------------------------
const char* PrintUTCtime(void)
{
 static char buf[40];   // persistent storage inside the function
 time_t now;
 time(&now);
 struct tm* UTCtime = gmtime(&now);
 if (!UTCtime) return(0);                                                                     // safety guard
 snprintf(buf, sizeof(buf),  "%02d-%s-%04d %02d:%02d:%02d ",
           UTCtime->tm_mday, monthNames[UTCtime->tm_mon],   // tm_mon = 0–11
           UTCtime->tm_year + 1900, UTCtime->tm_hour,
           UTCtime->tm_min, UTCtime->tm_sec);
 return buf;
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
 Tekstprintlnf("Time set in DS3231 RTC module: %s" ,PrintDS3231Time());
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
 if (printit) Tekstprintln(PrintDS3231Time());
}

//--------------------------------------------                                                //
// DS3231 Print DS3231 time
//--------------------------------------------------------------------
const char* PrintDS3231Time(void)
{
 static char buf[40];
 DateTime Inow = RTCklok.now();
 snprintf(buf, sizeof(buf), "%02d-%s-%04d %02d:%02d:%02d ",
          Inow.day() , monthNames[Inow.month()-1] , Inow.year(),
          Inow.hour(), Inow.minute(), Inow.second());
 return buf;
}
//--------------------------------------------                                                //
// RTC prints the ESP32 internal RTC time to sptext and web page
// usage: Tekstprintln(PrintRTCTime());
//--------------------------------------------
const char* PrintRTCTime()
{
 static char buf[40];
 snprintf(buf, sizeof(buf), "%02d-%s-%04d %02d:%02d:%02d",
             timeinfo.tm_mday, monthNames[timeinfo.tm_mon], timeinfo.tm_year + 1900,
             timeinfo.tm_hour, timeinfo.tm_min,  timeinfo.tm_sec);
 return buf;
}

//--------------------------------------------                                                //
// RTC Fill sptext with time
// Empty and 2 is with LF, 1 is without LF
//--------------------------------------------
void PrintTimeHMS(){ PrintTimeHMS(2);}                                                        // print with linefeed
void PrintTimeHMS(byte format)
{
 snprintf(sptext, sizeof(sptext),"%02d:%02d:%02d ",timeinfo.tm_hour,timeinfo.tm_min,timeinfo.tm_sec);
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
}

//--------------------------------------------                                                //
// CLOCK Print all the times available 
//--------------------------------------------
void PrintAllClockTimes(void)
{
 Tekstprintf("\n Clock time: %s", PrintRTCTime());
 if(WiFi.localIP()[0] != 0)                                                                   // If no WIFI then no NTP time printed
   {
    Tekstprintf("\n   NTP time: %s", PrintNTPtime());
    Tekstprintf("\n   UTC time: %s", PrintUTCtime());
   }
 if(DS3231Installed)
    Tekstprintf("\nDS3231 time: %s", PrintDS3231Time());
}
//--------------------------- End Time functions --------------------------


//------------------ BLE

//--------------------------------------------                                                //
// BLE SendMessage by BLE Slow in packets of 20 chars
// or fast in one long string.
// Fast can be used in IOS app BLESerial Pro
//--------------------------------------------
void SendMessageBLE(std::string Message)
{
 if(BLEConnected) 
   {
    if (Mem.UseBLELongString)                                                                 // If Fast transmission is possible
     {
      pTxCharacteristic->setValue(Message); 
      pTxCharacteristic->notify();
      delay(10);                                                                              // Bluetooth stack will go into congestion, if too many packets are sent
     } 
   else                                                                                       // Packets of max 20 bytes
     {   
      int parts = (Message.length() + 19) / 20;
      for(int n=0;n<parts;n++)
        {   
         pTxCharacteristic->setValue(Message.substr(n*20, 20)); 
         pTxCharacteristic->notify();
         delay(30);                                                                           // Bluetooth stack will go into congestion, if too many packets are sent too fast
        }
     }
   } 
}

//--------------------------------------------                                                //
// BLE Start BLE Classes NimBLE Version 2.x.x
//--------------------------------------------
class MyServerCallbacks: public NimBLEServerCallbacks 
{
 void onConnect(NimBLEServer* pServer, NimBLEConnInfo& connInfo) override 
    {BLEConnected = true; Tekstprintln("BLE Connected"   );}
 void onDisconnect(NimBLEServer* pServer, NimBLEConnInfo& connInfo, int reason) override 
    {BLEConnected = false; Tekstprintln("BLE Disconnected" );}
};
  
class MyCallbacks: public NimBLECharacteristicCallbacks 
{
 void onWrite(NimBLECharacteristic *pCharacteristic, NimBLEConnInfo& connInfo) override  
  {
   std::string rxValue = pCharacteristic->getValue();
   ReceivedMessageBLE = rxValue + "\n";
  }  
};

//--------------------------------------------                                                //
// BLE Start BLE Service
//--------------------------------------------
void StartBLEService(void)
{
 NimBLEDevice::init(Mem.BLEbroadcastName);
 pServer = NimBLEDevice::createServer();
 pServer->setCallbacks(new MyServerCallbacks());
 BLEService *pService = pServer->createService(SERVICE_UUID);
 pTxCharacteristic = pService->createCharacteristic(
        CHARACTERISTIC_UUID_TX, NIMBLE_PROPERTY::NOTIFY);
 BLECharacteristic *pRxCharacteristic = pService->createCharacteristic(
        CHARACTERISTIC_UUID_RX, NIMBLE_PROPERTY::WRITE);
 pRxCharacteristic->setCallbacks(new MyCallbacks());
 pServer->start();
 NimBLEAdvertising *pAdvertising = NimBLEDevice::getAdvertising();                            // Configure advertising AFTER server start
 pAdvertising->addServiceUUID(SERVICE_UUID);
 NimBLEAdvertisementData scanResponse;                                                         // Scan response needed for Android (Samsung) to find device in BLE scan
 scanResponse.setName(Mem.BLEbroadcastName);
 pAdvertising->setScanResponseData(scanResponse);
 pAdvertising->start();
 TekstSprint("BLE Waiting a client connection to notify ...\n");
}

//--------------------------------------------                                                //
// BLE Disconnect BLE Service
// NimBLE's built-in disconnect (higher level)
//--------------------------------------------
void DisconnectBLE(void)
{
 auto* server = NimBLEDevice::getServer();
 if (server)
   {
    auto connHandles = server->getPeerDevices();                                              // Get actual connection handles (not index)
    if (!connHandles.empty())
      {
       pServer->disconnect(connHandles[0]);
      }
    BLEConnectedSince = 0;
   }
}
//--------------------------------------------                                                //
// BLE CheckBLE input and rework string
// After 1/4 hour the BLE connection is disconnected
//--------------------------------------------
void CheckBLE(void)
{
 if (BLEConnected && !oldBLEConnected) 
   {
    oldBLEConnected = BLEConnected;
    BLEConnectedSince = millis();                                                             // Mark time of last connection 1/4 hour after this time the BLE connection will disconnect
   }
 if (!BLEConnected && oldBLEConnected)                                                        // If device is disconnected start advertising
   {
    delay(300);
    pServer->startAdvertising();
    TekstSprint("Start advertising\n");
    oldBLEConnected = BLEConnected;
    BLEConnectedSince = 0;                                                                    // Reset BLE connection timer
   }
 if ((BLEConnected && BLEConnectedSince > 0) && (millis() - BLEConnectedSince > 900000))      // Disconnect if connected longer than 15 minutes inactivity 
   {
    auto connHandles = NimBLEDevice::getServer()->getPeerDevices();                           // Vector of uint16_t handles
    if (!connHandles.empty()) 
      {
       pServer->disconnect(connHandles[0]);
       TekstSprint("Disconnected BLE client after 1/4 hour\n");
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

// --------- Time Receiver. Connect to TimeSender TIMESENDER_DEVICE_NAME "BLE-UARTtime"
//--------------------------------------------                                                //
// BLE TimeReceiver client
//--------------------------------------------
void SendMessageTimeReceiver(const char* msg)
{
 if (!TRConnected || !msg || !pRemoteRX || !pClient) return;
 if (!pClient->isConnected()) { TRConnected = false; return; }
 size_t len = strlen(msg);
 pRemoteRX->writeValue((uint8_t*)msg, len, false);
}

//--------------------------------------------                                                //
// BLE TimeReceiver Reconnect time receiver
//--------------------------------------------
void ReconnectTimeReceiver()
{
 if (pClient)                                                                                 // Clean up old client before reconnecting to avoid race condition with onDisconnect callback
   {
    if (pClient->isConnected()) pClient->disconnect();
    NimBLEDevice::deleteClient(pClient);
    pClient = nullptr;
   }
 TRConnected        = false;
 TRConnecting       = false;
 TRhaveFoundAddr    = true;                                                                   // Keep the address
 TRconnectRequested = true;
 delay(200);
 Connect_TimeReceiver();
}
//--------------------------------------------                                                //
// BLE TimeReceiver RX notify callback
// -------------------------------------------
static void TimeReceiverNotifyCB( NimBLERemoteCharacteristic*,  uint8_t* data, size_t len, bool notify)
{
 static char buf[128];
 size_t n = (len < sizeof(buf) - 1) ? len : sizeof(buf) - 1;
 memcpy(buf, data, n);
 buf[n] = 0;
 ReworkInputString(String(buf));
 TRConnectedSince = millis();
}

//--------------------------------------------                                                //
// BLE TimeReceiver Scan callbacks
// Client callbacks
// -------------------------------------------
class TimeReceiverClientCB : public NimBLEClientCallbacks
{
 void onConnect(NimBLEClient* client) override
   {
    TRConnected = true;
    TRConnecting = false;
    TRConnectedSince = millis();
    Tekstprintlnf("TimeReceiver connected to: %s", TIMESENDER_DEVICE_NAME);
   }

 void onDisconnect(NimBLEClient* client, int reason) override
   {
    Tekstprintlnf("TimeReceiver disconnected (reason: %d)", reason); 
    TRConnected        = false;
    TRConnecting       = false;
    TRhaveFoundAddr    = false;
    TRconnectRequested = false;
    TimeReceiverRunning  = false;                                                            // Reset flag
    if (pClient)                                                                             // Clean up client
      {
       NimBLEDevice::deleteClient(pClient);
       pClient = nullptr;
      }
    pRemoteTX = nullptr;
    pRemoteRX = nullptr;
    NimBLEScan* scan = NimBLEDevice::getScan();                                               // Stop any existing scan first
    if (scan->isScanning())  {  scan->stop();   delay(100);  }
    delay(200);
    StartTimeReceiverScan();                                                                  // Start new scan
   }
};

//--------------------------------------------                                                //
// Scan callbacks
//--------------------------------------------
class TimeReceiverScanCB : public NimBLEScanCallbacks
{
 void onResult(const NimBLEAdvertisedDevice* dev) override
   {
    if (TRConnecting) return;
    if (!dev->haveName()) return;
    std::string rawName = dev->getName();
    if (rawName.find(TIMESENDER_DEVICE_NAME) == std::string::npos) return;
    if (dev->getRSSI() <= TIMERECEIVER_MIN_RSSI) return;
    if (!dev->isConnectable()) return;
    if (!dev->isAdvertisingService(NimBLEUUID(SERVICE_UUID))) return;
    TRfoundAddr        = dev->getAddress();
    TRhaveFoundAddr    = true;
    TRconnectRequested = true;
    NimBLEDevice::getScan()->stop();
   }
};

//--------------------------------------------                                                //
// BLE TimeReceiver Start Time Receiver scan
//--------------------------------------------
void StartTimeReceiverScan(void)
{
 NimBLEScan* scan = NimBLEDevice::getScan();
 if (TimeReceiverRunning && !scan->isScanning())                                              // Reset flag if scan isn't actually running
   {
    TimeReceiverRunning = false;
    Tekstprintln("Flag reset - scan not running");
   }
 if (TimeReceiverRunning)  { Tekstprintln("Scan already running - skipping");   return;}     // Only start if not already running
 if (scan->isScanning()) { scan->stop();  delay(100);}                                        // Stop any existing scan 
 TimeReceiverRunning = true; 
 scan->setActiveScan(true);
 scan->setInterval(160);
 scan->setWindow(30);
 scan->setScanCallbacks(new TimeReceiverScanCB(), true);  // true = delete old callback 
 if (!scan->start(0, false, false))
   {
    Tekstprintln("ERROR: Failed to start TimeReceiver scan");
    TimeReceiverRunning = false;
   }
 else
   {
    LastTimeReceiverScan = millis();
    Tekstprintln("TimeReceiver scan started");
   }
}
//--------------------------------------------                                                //
// BLE TimeReceiver Stop Time Receiver
//--------------------------------------------
void StopTimeReceiver()
{
 NimBLEDevice::getScan()->stop();
 if (pClient)
   {
    if (pClient->isConnected()) pClient->disconnect();
    NimBLEDevice::deleteClient(pClient);                                                      // Always delete to avoid memory leak
    pClient = nullptr;
   }
 TRConnected           = false;
 TRConnecting          = false;
 pRemoteTX             = nullptr;
 pRemoteRX             = nullptr;
 TRhaveFoundAddr       = false;
 TRconnectRequested    = false;
}

//--------------------------------------------                                                //
// BLE TimeReceiver Connect TimeReceiver
// -------------------------------------------
static void Connect_TimeReceiver(void)
{
 if (!TRhaveFoundAddr || TRConnected || TRConnecting) return;
 const int MAX_CONNECT_ATTEMPTS = 3;
 const unsigned long POST_DISCONNECT_DELAY_MS = 150;
 
 for (int attempt = 1; attempt <= MAX_CONNECT_ATTEMPTS; ++attempt)
    {
     NimBLEDevice::getScan()->stop();
     delay(100);
     pClient = NimBLEDevice::createClient();
     pClient->setClientCallbacks(new TimeReceiverClientCB(), false);
     TRConnecting      = true;
     TRconnectStartms  = millis();
     
     if (!pClient->connect(TRfoundAddr))
        {
         NimBLEDevice::deleteClient(pClient);
         pClient = nullptr;
         TRConnecting = false;
         delay(POST_DISCONNECT_DELAY_MS);
         continue;
        }
       
      NimBLERemoteService* svc = pClient->getService(SERVICE_UUID);
      if (!svc)
        {
         pClient->disconnect();
         NimBLEDevice::deleteClient(pClient);
         pClient = nullptr;
         TRConnecting = false;
         delay(POST_DISCONNECT_DELAY_MS);
         continue;
        }
      
      pRemoteTX = svc->getCharacteristic(CHARACTERISTIC_UUID_TX);
      pRemoteRX = svc->getCharacteristic(CHARACTERISTIC_UUID_RX);
      if (!pRemoteTX || !pRemoteRX)
        {
         pClient->disconnect();
         NimBLEDevice::deleteClient(pClient);
         pClient = nullptr;
         TRConnecting = false;
         delay(POST_DISCONNECT_DELAY_MS);
         continue;
        }
      
      if (!pRemoteTX->subscribe(true, TimeReceiverNotifyCB, true))
        {
         pClient->disconnect();
         NimBLEDevice::deleteClient(pClient);
         pClient = nullptr;
         TRConnecting = false;
         delay(POST_DISCONNECT_DELAY_MS);
         continue;
        }
      
      TRConnectedSince    = millis();
      TRhaveFoundAddr     = false;
      TRconnectRequested  = false;
      TRConnecting        = false;
 //     Tekstprintlnf("TimeReceiver connected to: %s", Mem.BLEbroadcastName);
      return;
    }

 if (pClient)
   {
    NimBLEDevice::deleteClient(pClient);
    pClient = nullptr;
   }
 TRhaveFoundAddr    = false;
 TRconnectRequested = false;
 Tekstprintln("TimeReceiver connection failed - restarting scan");
 StartTimeReceiverScan();
}

//--------------------------------------------                                                //
// BLE TimeReceiver Check TimeReceiver Client
//--------------------------------------------
void CheckTimeReceiverClient(void)
{
 unsigned long now = millis();
 NimBLEScan* scan = NimBLEDevice::getScan();
 if (!scan->isScanning() && TimeReceiverRunning)                                              // If scanning is not active and flag is still true, reset it to allow rescanning
   {
    TimeReceiverRunning = false;                                                              // Allow rescanning
    Tekstprintln("TimeReceiverRunning flag reset - ready for rescan");
   }
 if (TRconnectRequested && TRhaveFoundAddr && !TRConnected && !TRConnecting)                  // Handle connect request
   {
    TRconnectRequested = false;
    Connect_TimeReceiver();
   }
 if (TRConnecting && (now - TRconnectStartms > CONNECT_TIMEOUT_MS))                           // Handle connection timeout
   {
    if (pClient)
      {
       if (pClient->isConnected()) pClient->disconnect();
       NimBLEDevice::deleteClient(pClient);
       pClient = nullptr;
      }
    TRConnecting = false;
    TRhaveFoundAddr = false;
    TRconnectRequested = false;
    TimeReceiverRunning = false;                                                              // Reset flag to allow new scan
    Tekstprintln("Connect timeout - resetting for new scan");
    scan->start(0, false, false);
  }

 if (!TRConnected && !scan->isScanning())
   {
    unsigned long timeSinceLastScan = now - LastTimeReceiverScan;
    if (timeSinceLastScan > 3000)                                                             // Reset the flag if it's been long enough (allows rescanning)
      {
       TimeReceiverRunning = false;                                                           // Reset flag for new attempt
       StartTimeReceiverScan();                                                               // This will restart the scan
       Tekstprintlnf("Periodic rescan initiated (%.1f seconds since last scan)",timeSinceLastScan / 1000.0);
      }
    }
}
// End Time Receiver
//--------------------------------------------                                                //
// WIFI WIFIEvents
// Yes — using WiFi.onEvent() is definitely the better and more reliable way to handle Wi-Fi connection state on the ESP32.
// Why WiFi.onEvent() is better
// 1. It’s fully asynchronous
//--------------------------------------------
void WiFiEvent(WiFiEvent_t event)
{
 // snprintf(sptext, sizeof(sptext),"[WiFi-event] event: %d  : ", event);  Tekstprint(sptext);
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
//WiFi.scanDelete();                                                                          // Delete the scan result to free memory for code below.
}
//--------------------------------------------                                                //
// WIFI Check for WIFI router SSID and password 
// If not valid then start webpage to enter the credentials
//--------------------------------------------
void ConnectWIFI(void)
{
if( (strlen(Mem.Password)<5 || strlen(Mem.SSID)<3))                                           // If WIFI required and no SSID or password
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
 static uint16_t RebootTime = 30;                                                             // Hours
 if(!Mem.WIFIOn || InApMode) return;                                                          // If in WIFI APmode or there is nothing to check
 if(!WIFIwasConnected) { if(CheckforWIFINetwork(false) ) StartWIFI_NTP(); }                   // If there was no WIFI at start up start a WIFI connection       
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
     Mem.LoopRebooted++;                                                                      // Will store the no of reboots. 
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
   else { delay(tryDelay);  numberOfTries--; }
  }
if (!WIFIwasConnected) return false;                                                          // If WIFI connection fails -> return
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
// NTP Notify
//--------------------------------------------
void NTPnotify(struct timeval* tv) { ntpJustSynced = true; }                                  // --- NTP time synchronized --- is printed later

//--------------------------------------------                                                //
// NTP Check and Print NTP if synced
//--------------------------------------------
void CheckandPrintNTPsynced(void)
{
 struct tm tm;
 time_t now = time(nullptr);
 localtime_r(&now, &tm); 
 if (Mem.TimeLogPrint != 0 )                                              // Do not print time but do update the display                                                              // do not Print the time string every minute
   { 
    Tekstprintlnf("-- NTP time synchronized --  %02d-%s-%04d %02d:%02d:%02d ",
           timeinfo.tm_mday,  monthNames[timeinfo.tm_mon],   // tm_mon is 0–11
           timeinfo.tm_year + 1900, timeinfo.tm_hour,
           timeinfo.tm_min,          timeinfo.tm_sec);
   }       
 ntpJustSynced = false;
}
//--------------------------------------------                                                //
// NTP Set time zone
//--------------------------------------------
void setTimezone(void) { setenv("TZ", Mem.Timezone, 1);  tzset(); }
//--------------------------------------------                                                //
// NTP Init NTP
//--------------------------------------------
void initSNTP(void) 
{  
 sntp_set_time_sync_notification_cb(NTPnotify);                                               // Optional: callback when time syncs
 configTime(0, 0, "pool.ntp.org", "time.nist.gov");                                           // 1 hour sync interval = default
 setTimezone();                                                                               // Must be set after configTime
}
//--------------------------------------------                                                //
// NTP Get a NTP time and wait max 2.5 sec 
//--------------------------------------------
bool wait4SNTP(void) 
{
 int32_t Tick = millis(); 
 bool SNTPtimeValid = true;
 while (sntp_get_sync_status() != SNTP_SYNC_STATUS_COMPLETED) 
  { if ((millis() - Tick) >2500) {SNTPtimeValid = false; break;}   }                          // Wait max 2.5 seconds 
return  SNTPtimeValid;
}

//--------------------------------------------                                                //
// WIFI Check for a Pending command received from the web page 
// Log SWversion to web page when Log = true
//--------------------------------------------
void BuildHTMLPage(bool Log)                                                                   // Build HTML_page from current state. log=true: menu also goes to log (use for i/ii)
{
 int i = 0, n;
 html_info[0] = 0;
 DoNotLog = !Log;                                                                              // Suppress log output unless explicitly requested (i/ii)
 SWversion();
 DoNotLog = false;
 for (n=0;n<strlen(index_html_top);n++)       HTML_page[i++] = (char) index_html_top[n];
 if (i <= MAXSIZE_HTML_PAGE - 999)
  {
   for (n=0;n<strlen(html_info);n++)          HTML_page[i++] = (char) html_info[n];
   for (n=0;n<strlen(index_html_footer);n++)  HTML_page[i++] = (char) index_html_footer[n];
  }
 else strcat(HTML_page, "<br> Send I for menu\n*** INCREASE MAXSIZE_HTML_PAGE in Webpage.h ***<br><br><br>");
 HTML_page[i] = 0;
}
//--------------------------------------------                                                //
// WIFI CheckWIFIcommand
//--------------------------------------------
void CheckWIFIcommand(void)
{
 if (PendingCommand.length() > 0)
  {
   ReworkInputString(PendingCommand);
   BuildHTMLPage(false);                                                                      // Rebuild page after command; no logging (only i/ii should log the menu) 
  }
}
//--------------------------------------------                                                //
// WIFI WEBPAGE
//--------------------------------------------
void WebPage(void)
{
 BuildHTMLPage(true);                                                                         // Build initial page at startup with logging enabled
 server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)                                  // Send web page with input fields to client
   {  request->send(200, "text/html",(const char*) HTML_page  );  }    ); 
 server.on("/log", HTTP_GET, [](AsyncWebServerRequest *request)                               // Serve the log viewer HTML page
   {  request->send(200, "text/html", logData);    });                                        // Serve the HTML page from LogViewer.h
 server.on("/tekstprint",  HTTP_GET, HandleTekstPrint);                                       // LOGBUFFER Printlast 500 lines
 server.on("/tekstdownload", HTTP_GET, HandleTekstDownload);                                  // LOGBUFFER Stream all the circular log buffer directly to the client
 server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request)
   {
    String inputMessage;
    if (request->hasParam(PARAM_INPUT_1)) {inputMessage = request->getParam(PARAM_INPUT_1)->value();}
    PendingCommand = inputMessage;                                                            //  Command → queue it and redirect
    request->redirect("/wait");
});
 server.on("/wait", HTTP_GET, [](AsyncWebServerRequest *request)
  {
   if (PendingCommand.length() == 0) { request->redirect("/"); return; }                      // Command finished → show updated page
    request->send(200, "text/html",
        "<html><body>"
        "<h3>Processing command...</h3>"
        "<meta http-equiv='refresh' content='1'>"
        "</body></html>");
  });
 server.on("/update", HTTP_GET, [](AsyncWebServerRequest *request)                            // Serve the OTA upload page
    {  request->send(200, "text/html", OTA_html);});
 server.on("/update", HTTP_POST, [](AsyncWebServerRequest *request)                           // Handle the actual OTA upload
   {
    shouldReboot = !Update.hasError();
    AsyncWebServerResponse *response = request->beginResponse(
        200, "text/plain", shouldReboot ? "OK" : "FAIL");
    response->addHeader("Connection", "close");
    request->send(response);
   },  
  [](AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final) 
   {
    if (!index) 
      {
       Tekstprintf("OTA Start: %s\n", filename.c_str());
       if (!Update.begin(UPDATE_SIZE_UNKNOWN)) { Update.printError(Serial); }
      }
    if (Update.write(data, len) != len) { Update.printError(Serial); }
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
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)                               // Send web page with input fields to client
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
 int16_t sensorValue = analogRead(OneWirePin);                                                // Read the value from the sensor:
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
 snprintf(sptext, sizeof(sptext),"3*1 keypad %s used", Mem.TimeInput==2?"IS":"NOT");

 }
//--------------------------------------------
// KEYPAD check for Keypad input
//--------------------------------------------                           
void Keypad3x1Check(void)
{ 
// digitalWrite(COLPIN,LOW);                                                                  // Mimic a key press on pin 6 in order to select the first column
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
 snprintf(sptext, sizeof(sptext),"Rotary %s used", Mem.TimeInput==1?"IS":"NOT");
 
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
      Displaytime();Tekstprintln("");
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
       case 1:  ChangeTime = true;                  break;  
       case 2:  ChangeTime = true;                  break;    
       case 3:  ChangeLightIntensity = true;        break;   
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
    Displaytime();  Tekstprintln("");                                                                          // Turn on the LEDs with proper time
   }
 }

//--------------------------------------------                                                //
// IR-RECEIVER Start alarge or tiny IR-RECEIVER 
// Copy the to be used ButtonNames for a small or large IR-receiver
//--------------------------------------------
void Start_IRreceiver(void)
{
 if (Mem.TimeInput==3) Init_IRreceiver(ButtonLNames, sizeof(ButtonLNames) / sizeof(ButtonLNames[0]));
 if (Mem.TimeInput==4) Init_IRreceiver(ButtonTNames, sizeof(ButtonTNames) / sizeof(ButtonTNames[0]));
}

//--------------------------------------------                                                //
// IR-RECEIVER Init
//--------------------------------------------
void Init_IRreceiver(String* ButtonTempNames, byte numButtons)
{
 ButtonNames = ButtonTempNames;  // Store pointer
 NOOFBUTTONS = numButtons;
 IrReceiver.begin(IRReceiverPin);

 if (Mem.TimeInput == 3 || Mem.TimeInput == 4)  {  GetIRRemoteFromFlashMemory(); }            // Load IR settings (only if IR remote is enabled)
 if (IRMem.remoteIdentified && IRMem.buttons[0].learned)                                      // Check if we already have learned buttons
  {
   learningMode = false;
   //PrintAllMappings();
   snprintf(sptext, sizeof(sptext), "Remote identified - Protocol: %s", getProtocolString((decode_type_t) IRMem.learnedRemoteProtocol));
   Tekstprintlnf("Remote identified - Protocol: %s", getProtocolString((decode_type_t) IRMem.learnedRemoteProtocol));
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
    Tekstprintlnf("Remote identified - Protocol: %s, Address: 0x%04X", getProtocolString(protocol), address);
  }
  else
  {
    if (address != IRMem.learnedRemoteAddress || protocol != (decode_type_t)IRMem.learnedRemoteProtocol)    // Check if this is from the same remote
    {
      Tekstprintln("⚠ Wrong remote! Please use the same remote control.");
      Tekstprintlnf("Expected Address: 0x%04X, Got: 0x%04X", IRMem.learnedRemoteAddress, address);
      return;
    }
  }
  IRMem.buttons[currentLearningIndex].protocol = (uint8_t)protocol;                           // Store the button mapping
  IRMem.buttons[currentLearningIndex].command = command;
  IRMem.buttons[currentLearningIndex].address = address;
  IRMem.buttons[currentLearningIndex].learned = true;
  
  Tekstprintlnf("✓ Learned '%s' - Protocol: %s, Command: 0x%04X, Address: 0x%04X", 
          ButtonNames[currentLearningIndex].c_str(), getProtocolString(protocol), command, address);
  currentLearningIndex++;
  if (currentLearningIndex < NOOFBUTTONS)
  {
    Tekstprintlnf("\nPlease press button: %s", ButtonNames[currentLearningIndex].c_str());
  }
  else
  {
    learningMode = false;                                                                     // Learning complete
    StoreIRRemoteInFlashMemory();
    Tekstprintln("\n=== Learning Complete & Saved! ===");
    Tekstprintln("\nStored Button Mappings:");
    PrintAllMappings();
    Tekstprintln("\n=== Now in Recognition Mode ===");
    Tekstprintlnf("Only responding to remote with Address: 0x%04X", IRMem.learnedRemoteAddress);
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
void StartIRLearning(void)
{
 Tekstprintln("\n=== Starting Learning Mode ===");
 currentLearningIndex = 0;
 learningMode = true;
 IRMem.remoteIdentified = false;
 for (int i = 0; i < NOOFBUTTONS; i++) {IRMem.buttons[i].learned = false; }
 Tekstprintlnf("Please press button: %s", ButtonNames[0].c_str());
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
void ResetAllIRremoteSettings(void)
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
      Tekstprintlnf("Button pressed: %s", ButtonNames[i].c_str());
      found = true;
      ReworkIRremoteValue(i);
      return i;
    }
  }
  
  if (!found)
  {
    Tekstprintlnf("Unknown button - Protocol: %s, Command: 0x%04X, Address: 0x%04X", getProtocolString(protocol), command, address);
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

if (Mem.TimeInput == 3)
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
    case 0 ... 9:                                                                             // Button "0 - 9"
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
            Tekstprintlnf("Button '%s' not yet assigned", ButtonName.c_str());
            break;
  }
 }
 }  // end if ==3

if (Mem.TimeInput == 4)                                                                       // {"MIN-1", "MIN+1","UUR-1","UUR+1","POWER","ONOFF"};
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
            Tekstprintlnf("Button '%s' not yet assigned", ButtonName.c_str());
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
 Displaytime();  Tekstprintln("");     
}

//--------------------------------------------                                                //
// IR-RECEIVER Turn On or off Remote control
// Turns off after 60 seconds
//--------------------------------------------
void ToggleIRpower(void)
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


//   Ringbuffer

//--------------------------------------------                                                //
// LOGBUFFER Initialize circular logging buffer
//--------------------------------------------
void InitLogBuffer() 
{
 size_t largest = heap_caps_get_largest_free_block(MALLOC_CAP_SPIRAM);
 size_t reserve = 5000000;                                                                      // much smaller reserve needed — PSRAM is separate from internal RAM
 StoredStartHeaps(true);                                                                      // Store the values before allocation                                                                               //                                     // Keep a safety margin tune this based on your system
 if (largest > reserve) LogBufferSize = largest - reserve;
 else LogBufferSize = largest / 2;
 LogBuffer = (char*)heap_caps_malloc(LogBufferSize, MALLOC_CAP_SPIRAM);
 if (!LogBuffer)                                                                              // Fallback to internal RAM if no PSRAM available
    {
     largest = heap_caps_get_largest_free_block(MALLOC_CAP_8BIT);
     reserve = 120000;
     LogBufferSize = (largest > reserve) ? largest - reserve : largest / 2;
     LogBuffer = (char*) heap_caps_malloc(LogBufferSize, MALLOC_CAP_8BIT);
    }
 if (!LogBuffer) { Tekstprintln("ERROR: Could not allocate log buffer");     return;  }
 memset(LogBuffer, 0, LogBufferSize);                                                         // Fill the buffer with 0
 LogWritePos = 0;
 LogWrapped = false;
 Tekstprintlnf("Log buffer allocated: %u bytes (largest block was %u)", (unsigned)LogBufferSize, (unsigned)largest);
}

//--------------------------------------------                                                //
// LOGBUFFER Add a log line (raw C-string). Always ends with '\n'.
//--------------------------------------------
void AddLog(const char* msg) 
{
  if (DoNotLog) return;
  if (!LogBuffer) return;
  size_t len = strlen(msg);
  if (len >= LogBufferSize) {msg += (len - LogBufferSize);  len = strlen(msg); }              // Keep only last part
  for (size_t i = 0; i < len; i++) 
    {
     LogBuffer[LogWritePos++] = msg[i];
     if (LogWritePos >= LogBufferSize)  { LogWritePos = 0;  LogWrapped = true; }              // If we are at the end then wrap. Essential to find the last 500 lines
    }
}

//--------------------------------------------                                                //
// LOGBUFFER Printlast 500 lines
//--------------------------------------------
void HandleTekstPrint(AsyncWebServerRequest* request) 
{
  if (!LogBuffer) {request->send(200, "text/plain", "Log buffer uninitialized\n"); return; }
  AsyncResponseStream* response = request->beginResponseStream("text/plain");
  ssize_t lastNewline = -1;                                                                   // Find the actual end of the last complete line (last \n before LogWritePos)
  ssize_t searchPos = (LogWritePos == 0) ? LogBufferSize - 1 : LogWritePos - 1;
  for (size_t i = 0; i < LogBufferSize; i++) {
    if (LogBuffer[searchPos] == '\n') {lastNewline = searchPos;   break; }
    if (!LogWrapped && searchPos == 0)                            break;
    searchPos = (searchPos == 0) ? LogBufferSize - 1 : searchPos - 1;
  }
  if (lastNewline == -1) 
     { request->send(200, "text/plain", "No complete lines in buffer\n");   return; }         // No complete lines yet
  size_t lineCount = 0;                                                                       // Now count back 100 for testing) (500) lines from lastNewline
  ssize_t pos = lastNewline;
  for (size_t scanned = 0; scanned < LogBufferSize && lineCount < 499; scanned++)             //  499 because we already found one \n
  {
    pos = (pos == 0) ? LogBufferSize - 1 : pos - 1;
    if (LogBuffer[pos] == '\n') lineCount++;
    if (!LogWrapped && pos == 0) break;
  }
  size_t start = pos % LogBufferSize;                                                         // Start is the position right after the newline we stopped at
  size_t end   = (lastNewline + 1) % LogBufferSize;                                           // Include the last newline
  if (!LogWrapped || start <= end) {
    response->write(LogBuffer + start, end - start);
  } else {
    response->write(LogBuffer + start, LogBufferSize - start);
    response->write(LogBuffer, end);
  }
  request->send(response);
}

//--------------------------------------------                                                //
// LOGBUFFER Stream all the circular log buffer directly to the client
//--------------------------------------------
void HandleTekstDownload(AsyncWebServerRequest *request)
{
 if (!LogBuffer) {request->send(200,"text/plain","Log buffer uninitialized\n"); return;  }
 size_t start   = 0;                                                                          // Calculate start and length of valid data
 size_t dataLen = 0;
 if (!LogWrapped)   { start = 0; dataLen = LogWritePos; }
 else
   {
    ssize_t pos = LogWritePos;                                                                // Find first complete line after LogWritePos (skip partial overwritten line)
    size_t  scanned = 0;
    while (scanned < LogBufferSize && LogBuffer[pos % LogBufferSize] != '\n')
      {
       pos++;
       scanned++;
      }
    pos++;                                                                                    // move past the '\n'
    start   = pos % LogBufferSize;
    dataLen = LogBufferSize - 1;                                                              // almost full buffer, minus the partial line we skipped
   }

 AsyncWebServerResponse *response = request->beginResponse( "text/plain", dataLen,
        [start, dataLen](uint8_t *buffer, size_t maxLen, size_t alreadySent) -> size_t
   {                                                                                          // Use a chunked response — ESPAsyncWebServer streams this without one giant allocation
    if (alreadySent >= dataLen) return 0; // done
    size_t remaining  = dataLen - alreadySent;
    size_t toSend     = min(remaining, maxLen);
    size_t physPos    = (start + alreadySent) % LogBufferSize;                                // Physical position in the ring buffer
    size_t tillEnd    = LogBufferSize - physPos;                                              // How much fits before wrap?
    if (toSend <= tillEnd) { memcpy(buffer, LogBuffer + physPos, toSend); }
    else
       {
        memcpy(buffer,           LogBuffer + physPos, tillEnd);                               // Two-part copy over the wrap boundary
        memcpy(buffer + tillEnd, LogBuffer,           toSend - tillEnd);
        }
    return toSend;
    }
   );
 char filename[80];
 snprintf(filename, sizeof(filename), "attachment; filename=\"%s-log.txt\"", Mem.BLEbroadcastName);
 response->addHeader("Content-Disposition", filename);
 request->send(response);
}

/// ringbuffer

//--------------------------------------------                                                //
// TIMESENDER classes connect disconnect
//--------------------------------------------
// TIMESENDER Characteristic callbacks — fires when client subscribes/unsubscribes
//--------------------------------------------
class TSCharCallbacks : public NimBLECharacteristicCallbacks
{
 void onSubscribe(NimBLECharacteristic* pChar, NimBLEConnInfo& connInfo, uint16_t subValue) override
  {
   TSClientSubscribed = (subValue > 0);                                                       // subValue 1=notify, 2=indicate, 0=unsubscribed
  }
};
//--------------------------------------------
class TSServerCallbacks : public NimBLEServerCallbacks
{
void onConnect(NimBLEServer* pServer, NimBLEConnInfo& connInfo) override
  {
   TSClientConnected = true;
   TSconnHandle      = connInfo.getConnHandle();
   TSsendDelayms     = millis() - 10001;                                                       // Trigger first send immediately after delay
   TSdatePending     = false;
   Tekstprintln("TimeSender client connected");
  }
 void onDisconnect(NimBLEServer* pServer, NimBLEConnInfo& connInfo, int reason) override
   {
    TSClientConnected  = false;
    TSClientSubscribed = false;
    TSconnHandle = 0;
    Tekstprintln("TimeSender client disconnected");
    NimBLEDevice::startAdvertising();
   }
};
//--------------------------------------------                                                //
// TIMESENDER Time Sender Start
//--------------------------------------------
void BLETimeSenderStart(void)
{
  if(TimeSenderServerRunning) return;                          
  NimBLEDevice::init(TIMESENDER_DEVICE_NAME);            
  TSpServer = NimBLEDevice::createServer();              
  TSpServer->setCallbacks(new TSServerCallbacks());      
  TSpService = TSpServer->createService(UART_SERVICE_UUID);                                  // 
  TSpTX = TSpService->createCharacteristic(UART_CHARACTERISTIC_TX, NIMBLE_PROPERTY::NOTIFY);
  TSpTX->setCallbacks(new TSCharCallbacks());                                                  // Track when client subscribes to notifications              
  TSpRX = TSpService->createCharacteristic(UART_CHARACTERISTIC_RX, NIMBLE_PROPERTY::WRITE);                                                                          
  TSpAdvertising = NimBLEDevice::getAdvertising();       
  TSpAdvertising->addServiceUUID(UART_SERVICE_UUID);     
  TSpAdvertising->start();
  TimeSenderServerRunning = true;                                                                   // 
}
//--------------------------------------------                                                //
// TIMESENDER Time Sender Stop
//--------------------------------------------
void BLETimeSenderStop(void)
{
 if(!TimeSenderServerRunning) return;                                                               // 
 if(TSpAdvertising) TSpAdvertising->stop();               
 if(TSpServer && TSClientConnected) { TSpServer->disconnect(TSconnHandle);  }
 NimBLEDevice::deinit(true);                              
 TimeSenderServerRunning = false;                               
 TSClientConnected = false;                               
 TSconnHandle      = 0;                                   
}
//--------------------------------------------                                                //
// TIMESENDER Time Sender Send Time and Date
//--------------------------------------------
void CheckTimeSender(void)
{
 if(!TimeSenderServerRunning) return;
 if(!TSClientConnected) return;
 if(!TSClientSubscribed) return;                                                               // Wait until client has subscribed to notifications (CCCD enabled)
 GetTijd(false);
 int h  = timeinfo.tm_hour;
 int m  = timeinfo.tm_min;
 int s  = timeinfo.tm_sec;
 int d  = timeinfo.tm_mday;
 int mo = timeinfo.tm_mon + 1;
 int y  = timeinfo.tm_year + 1900;

 bool sendNow = (millis() - TSsendDelayms >= 10000);

 if(sendNow)
   {
    char buffer[16];
    snprintf(buffer, sizeof(buffer),"T%02d%02d%02d", h, m, s);
    Tekstprintlnf("Sending TIME: %s", buffer);
    TSpTX->setValue((uint8_t*)buffer, strlen(buffer));   
    TSpTX->notify();
    TSsendDelayms = millis();
    TSdatePending = true;                                
   }

 if(TSdatePending && (millis() - TSsendDelayms > 1000))
   {
    char buffer[20];
    snprintf(buffer, sizeof(buffer), "D%02d%02d%04d", d, mo, (uint16_t)y);
    Tekstprintlnf("Sending DATE: D%02d%02d%04d", d, mo, y);
    TSpTX->setValue((uint8_t*)buffer, strlen(buffer));   
    TSpTX->notify();
    TSdatePending = false;                               
   }
}

//--------------------------------------------                                                //
// HC12 Initialyse HC12 serial connection
//--------------------------------------------
void InitHC12(void) 
{
  Serial1.begin(9600, SERIAL_8N1, -1, HC12_TX);                                               // -1 = no RX
}

//--------------------------------------------                                                //
// HC12 Send time via HC-12 half-duplex wireless serial communication module
//--------------------------------------------
void SendHC12TimeString(void) 
{
  char TimeStr[20];
  sprintf(TimeStr, "T%02d%02d%02d\n", timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
  Serial1.print(TimeStr);
  Tekstprintf("HC-12 time sent: %s",TimeStr); 
}

/**************************************************************************************************/