/* 

--------------------------------------  
 Author .    : Ed Nieuwenhuys
 Changes V001: Derived from ESP32C3S3_WordClockV026.ino
 Changes V002: 
 Changes V003: Clock and SK6812 LEDstrip working
 Changes V004: Changed the coding pin numbering to Arduino default macro numbering D0, D1, A4 et cetera
 Changes V005: Language #define settings are working. Code cleanup
 Changes V006: Replaced AsyncElegantOTA with newer ElegantOTA
 Changes V007: WPS functionality added (not working yet (see line 2339)
               void SetStatusLED from int to bool. Removed Colors.h
 Changes V008: Code clean up. Minute and second colour of HET IS WAS repaired in NL144CLOCK. Defines of NL144CLOCK reordered
 Changes V009: Rewritten IS WAS changing every second in single language clock
               Corrected SetRTCtime() struct timeval now = { .tv_sec = t , .tv_usec = t}; --> struct timeval now = { .tv_sec = t , .tv_usec = 0};
 Changes V010: Stable version. Added four LEDs to strip for minute 1, 2, 3 and 4
 Changes V011: Added Login page (Not working yet) A page is visible but input is neglected
 Changes V012: Mislukt
 Changes V013: WIFI manager nog verbeteren of verwijderen
 Changes V014: WIFI manager verwijderd.Webpage updated
 Changes V015: ---
 Changes V016: Added RTC <-- stopped. some changed added to V017
 Changes V017: Added DS3231 source. Added again the defines for NL144 word clock Source Derived from Klok Ulirich Vertessen ESP32Arduino_WordClockV014U002.ino
 Changes V018: Added DS3231 code from Character_Colour_Clock_V089.ino. DS3231 Get proper time
 Changes V019: Added DS3231 functionality. ie that the clock get its time from the Ds3231 Module if no NTP or WIFI
 Changes V020: Removed CR LF from the Inputstring
 Changes V021: Clean up code
 Changes V022: Ulrich Vertessen clock. % in menu switches between SK6812 and WS2812 LEDstrip. Selftest added  with # in menu
 Changes V023: Updated HTML page. Reference to manual on Github
 Changes V024: Changed ESP32 board to 3.0.0. Arduino Nano ESP32 still version 2.0.
               
  Changed ESP32 board to 3.0.0
               Changed in Arduino\libraries\ESPAsyncWebServer\src\WebAuthentic
               at line 75,76,77   
               //-----------------              
                #ifdef ESP_ARDUINO_VERSION_MAJOR
                   #if ESP_ARDUINO_VERSION >= ESP_ARDUINO_VERSION_VAL(3, 0, 0)
                     // Code for version 3.x
                   mbedtls_md5_init(&_ctx);
                   mbedtls_md5_starts(&_ctx);
                   mbedtls_md5_update(&_ctx, data, len);
                   mbedtls_md5_finish(&_ctx, _buf);
                   #else
                    // Code for version 2.x
                   #ifdef ESP32
                      mbedtls_md5_init(&_ctx);
                      mbedtls_md5_starts_ret(&_ctx);
                      mbedtls_md5_update_ret(&_ctx, data, len);
                      mbedtls_md5_finish_ret(&_ctx, _buf);
                   #else
                      MD5Init(&_ctx);
                      MD5Update(&_ctx, data, len);
                      MD5Final(_buf, &_ctx);
                    #endif
                 #endif
                #endif
               //-------------------- 
               in AsyncEventSource.cpp changed at line 189 ets_printf --> log_e for V3
               in AsyncWebSocket.cpp changed at line 549 ets_printf --> log_e  for V3
                 #ifdef ESP_ARDUINO_VERSION_MAJOR
                   #if ESP_ARDUINO_VERSION >= ESP_ARDUINO_VERSION_VAL(3, 0, 0)
                     // Code for version 3.x
                    log_e("ERROR: Too many messages queued\n");
                   #else
                    // Code for version 2.x
                    ets_printf("ERROR: Too many messages queued\n");
                 #endif
                #endif  
                


How to compile: 
Install Arduino Nano ESP32 boards
Board: Arduino Nano ESP32
Partition Scheme: With FAT
Pin Numbering: By GPIO number (legacy)
Select below the clock type

*/
// =============================================================================================================================

// ------------------>   Define only one clock type
//#define FOURLANGUAGECLOCK
//#define NL144CLOCK
#define ULRICH

//--------------------------------------------
// ESP32 Includes defines and initialisations
//--------------------------------------------

#include <Preferences.h>
#include <Adafruit_NeoPixel.h> // https://github.com/adafruit/Adafruit_NeoPixel   for LED strip WS2812 or SK6812
#include <NimBLEDevice.h>      // For BLE communication  https://github.com/h2zero/NimBLE-Arduino
#include <ESPNtpClient.h>      // https://github.com/gmag11/ESPNtpClient
#include <WiFi.h>              // Used for web page 
#include "esp_wps.h"           // WPS functionality
#include <AsyncTCP.h>          // Used for webpage   https://github.com/me-no-dev/ESPAsyncWebServer
#include <ESPAsyncWebServer.h> // Used for webpage   https://github.com/me-no-dev/ESPAsyncWebServer
#include <ElegantOTA.h>        // If compile error see here :https://docs.elegantota.pro/async-mode/  Used for OTA new version // #include <AsyncElegantOTA.h>   // Used for OTA old version
 //                    #ifdef  MOD_DS3231
#include <Wire.h>              // Ter zijner tijd Wire functies gaan gebruiken. Staan al klaar in de code 
#include <RTClib.h>            // Used for connected DS3231 RTC // Reference https://adafruit.github.io/RTClib/html/class_r_t_c___d_s3231.html
 //                    #endif  //MOD_DS3231

//------------------------------------------------------------------------------
// ARDUINO Definition of installed language word clock
//------------------------------------------------------------------------------
                     #ifdef FOURLANGUAGECLOCK
#define NL
#define UK
#define DE
#define FR    
                     #endif //FOURLANGUAGECLOCK  

#define MIN1   ColorLeds("M1",    140, 140, LetterColor); 
#define MIN2   ColorLeds("M2",    141, 141, LetterColor);       //MINColor); 
#define MIN3   ColorLeds("M3",    142, 142, LetterColor);
#define MIN4   ColorLeds("M4",    143, 143, LetterColor);                                           
                                      

                     #ifdef NL144CLOCK
#define HET     ColorLeds("Het",     0,   2, MINColor);   
#define IS      ColorLeds("is",      4,   5, SECColor);  ColorLeds("", 8,10, 0); Is = true;
#define WAS     ColorLeds("was",     8,  10, SECColor);  ColorLeds("", 4, 5, 0); Is = false;
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
#define EDSOFT  ColorLeds("EdSoft", 132,132, LetterColor);
#define X_OFF   ColorLeds("",         0,  2, 0);
#define X_ON    ColorLeds("",         0,  2, LetterColor);
                     #endif //NL144CLOCK
                     
                     
                     #ifdef ULRICH
#define HET     ColorLeds("Het",      0,   4, MINColor);   
#define IS      ColorLeds("is",       5,   7, SECColor);     Is = true;    //ColorLeds("", 8,10, 0); 
#define WAS     ColorLeds("was",      5,   7, SECColor);     Is = false;  // ColorLeds("", 4, 5, 0); 
#define MVIJF   ColorLeds("vijf",     8,  13, LetterColor); 
#define MVOOR   ColorLeds("voor",    20,  25, LetterColor);
#define MTIEN   ColorLeds("tien",    14,  19, LetterColor); 
#define OVER    ColorLeds("over",    26,  31, LetterColor);
#define KWART   ColorLeds("kwart",   32,  39, LetterColor);
#define OVER2   ColorLeds("over",    46,  51, LetterColor);
#define HALF    ColorLeds("half",    40,  45, LetterColor);
#define VOOR    ColorLeds("voor",    52,  57, LetterColor);
#define EEN     ColorLeds("een",     58,  62, LetterColor);
#define DRIE    ColorLeds("drie",    69,  74, LetterColor);
#define TWEE    ColorLeds("twee",    63,  68, LetterColor);
#define VIER    ColorLeds("vier",    75,  80, LetterColor);
#define VIJF    ColorLeds("vijf",    81,  86, LetterColor);
#define ZES     ColorLeds("zes",     87,  91, LetterColor);
#define NEGEN   ColorLeds("negen",  100, 107, LetterColor);
#define ZEVEN   ColorLeds("zeven",   92,  99, LetterColor);
#define ACHT    ColorLeds("acht",   108, 113, LetterColor);
#define TIEN    ColorLeds("tien",   114, 119, LetterColor);
#define ELF     ColorLeds("elf",    120, 124, LetterColor);
#define TWAALF  ColorLeds("twaalf", 125, 134, LetterColor);
#define UUR     ColorLeds("uur",    135, 139, LetterColor);
                     #endif //ULRICH
                     
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
#define ES      ColorLeds("\nEs",  334, 335, DELetterColor);   
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
//--------------------------------------------
// SPIFFS storage
//--------------------------------------------
Preferences FLASHSTOR;

 //--------------------------------------------
// COLOURS stored in Mem.DisplayChoice
//--------------------------------------------   

const byte DEFAULTCOLOUR = 0;
const byte HOURLYCOLOUR  = 1;          
const byte WHITECOLOR    = 2;
const byte OWNCOLOUR     = 3;
const byte OWNHETISCLR   = 4;
const byte WHEELCOLOR    = 5;
const byte DIGITAL       = 6;
const byte ANALOOG       = 7;

//------------------------------------------------------------------------------
// PIN Assigments for Arduino Nano ESP32
//------------------------------------------------------------------------------ 
 
enum DigitalPinAssignments {      // Digital hardware constants ATMEGA 328 ----
 SERRX        = D0,               // D1 Connects to Bluetooth TX
 SERTX        = D1,               // D0 Connects to Bluetooth RX
 encoderPinA  = D3,               // D3 right (labeled DT on decoder)on interrupt  pin
 clearButton  = D4,               // D4 switch (labeled SW on decoder)
 LED_PIN      = D5,               // D5 Pin to control colour SK6812/WS2812 LEDs
 encoderPinB  = D8,               // D8 left (labeled CLK on decoder)no interrupt pin  
 PCB_LED_D09  = D9,               // D9
 PCB_LED_D10  = D10,              // D10
 secondsPin   = LED_BUILTIN,      // D13  GPIO48 (#ifdef LED_BUILTIN  #undef LED_BUILTIN #define LED_BUILTIN 48)
 };
 
enum AnaloguePinAssignments {     // Analogue hardware constants ----
 EmptyA0      = A0,               // Empty
 EmptyA1      = A1,               // Empty
 PhotoCellPin = A2,               // LDR pin
 OneWirePin   = A3,               // OneWirePin
 SDA_pin      = A4,               // SDA pin
 SCL_pin      = A5,               // SCL pin
 EmptyA6     =  A6,               // Empty
 EmptyA7     =  A7};              // Empty

byte ResetDisplayChoice  = DEFAULTCOLOUR; 

//------------------------------------------------------------------------------
const uint32_t black    = 0x000000, darkorange    = 0xFF8C00, red    = 0xFF0000, chartreuse = 0x7FFF00;
const uint32_t brown    = 0x503000, cyberyellow   = 0xFFD300, orange = 0xFF8000; 
const uint32_t yellow   = 0xFFFF00, cadmiumyellow = 0xFFF600, dyellow= 0xFFAA00, chromeyellow = 0xFFA700;
const uint32_t green    = 0x00FF00, brightgreen   = 0x66FF00, apple  = 0x80FF00, grass  = 0x00FF80;  
const uint32_t amaranth = 0xE52B50, edamaranth    = 0xFF0050, amber  = 0xFF7E00;
const uint32_t marine   = 0x0080FF, darkviolet    = 0x800080, pink   = 0xFF0080, purple = 0xFF00FF; 
const uint32_t blue     = 0x0000FF, cerulean      = 0x007BA7, sky    = 0x00FFFF, capri  = 0x00BFFF;
const uint32_t edviolet = 0X7500BC, frenchviolet  = 0X8806CE, coquelicot = 0xFF3800;
const uint32_t greenblue= 0x00F2A0, hotmagenta    = 0xFF00BF, dodgerblue = 0x0073FF, screamingreen= 0x70FF70;
 uint32_t white    = 0xFFFFFF, lgray         = 0x666666;               // R, G and B on together gives white light
 uint32_t gray     = 0x333333, dgray         = 0x222222;  
//------------------------------------------------------------------------------
// LED
//------------------------------------------------------------------------------
//                                                                  //
                                             #ifdef NL144CLOCK
const uint32_t NUM_LEDS  =  144 + 4;                                    // How many leds in  strip? + 4 for the minutes
const byte MATRIX_WIDTH  =  12;
const byte MATRIX_HEIGHT =  12;
                                             #endif //NL144CLOCK
                                             #ifdef ULRICH
const uint32_t NUM_LEDS  =  364;                                    // How many leds in  strip? + 4 for the minutes
const byte MATRIX_WIDTH  =  36;
const byte MATRIX_HEIGHT =  10;
                                             #endif //ULRICH
                                             
                                             #ifdef FOURLANGUAGECLOCK                                             
const uint32_t  NUM_LEDS = 625;                                     // How many leds in  strip?
const byte MATRIX_WIDTH  =  25;
const byte MATRIX_HEIGHT =  25;
                                             #endif //FOURLANGUAGECLOCK
const byte BRIGHTNESS    =  32;                                    // BRIGHTNESS 0 - 255

Adafruit_NeoPixel LEDstrip;
//                            #ifdef LED6812
Adafruit_NeoPixel LED6812strip = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRBW + NEO_KHZ800);  //NEO_RGBW
//                            #endif  //LED6812
//                            #ifdef LED2812
Adafruit_NeoPixel LED2812strip = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);  //NEO_RGB NEO_GRB
//                            #endif  //LED2812
bool     LEDsAreOff            = false;                             // If true LEDs are off except time display
bool     NoTextInColorLeds     = false;                             // Flag to control printing of the text in function ColorLeds()
int      Previous_LDR_read     = 512;                               // The actual reading from the LDR + 4x this value /5
uint32_t MINColor      = chromeyellow;
uint32_t SECColor      = chromeyellow;  
uint32_t DefaultColor  = chromeyellow;   
uint32_t LetterColor   = chromeyellow;   
                                             #ifdef FOURLANGUAGECLOCK
uint32_t DefaultColor  = white;   
uint32_t LetterColor   = white; 
uint32_t UKLetterColor = green;   
uint32_t DELetterColor = red;
uint32_t FRLetterColor = yellow;
uint32_t UKDefaultColor= UKLetterColor;
uint32_t DEDefaultColor= DELetterColor;
uint32_t FRDefaultColor= FRLetterColor;
                                             #endif //FOURLANGUAGECLOCK
uint32_t OwnColour     = 0X002345DD;        // Blueish
uint32_t WhiteColour   = white;
uint32_t WheelColor    = blue;
uint32_t HourColor[] ={  white,      darkviolet, cyberyellow, capri,         amber,         apple,
                         darkorange, cerulean,   edviolet,    cadmiumyellow, green,         edamaranth,
                         red,        yellow,     coquelicot,  pink,          apple,         hotmagenta,
                         green,      greenblue,  brightgreen, dodgerblue,    screamingreen, blue,
                         white,      darkviolet, chromeyellow};     

// uint32_t HourColor[24] ={white   ,0X800080,0XFFD500,0X00B8FF,0XFF6200,0X80FF00,
//                          0XFF8000,0X00C8FF,0X7500BC,0XFFD000,0X00FF00,0XFF0050,
//                          0XFF0000,0XFFFF00,0XFF4000,0XFF0088,0XF7FF00,0XFF00BF,
//                          0X00FF00,0X00F2A0,0X6FFF00,0X0073FF,0X70ff70,0X0000FF };     

// Definition of the digits 0 - 9, 3 wide, 5 high. 
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
 //                                 #ifdef MOD_DS3231
//--------------------------------------------
// DS3231 CLOCK MODULE
//--------------------------------------------
#define DS3231_I2C_ADDRESS          0x68
#define DS3231_TEMPERATURE_MSB      0x11
#define DS3231_TEMPERATURE_LSB      0x12

DateTime Inow;
RTC_DS3231 RTCklok;
byte DS3231Installed = 0;                                        // True if the DS3231 is detected
//                                  #endif  //MOD_DS3231


//------------------------------------------------------------------------------
// KY-040 ROTARY
//------------------------------------------------------------------------------ 
                          #ifdef ROTARYMOD                         
Encoder myEnc(encoderPinA, encoderPinB);                         // Use digital pin  for encoder
                          #endif  //ROTARYMOD      
long     Looptime          = 0;
byte     RotaryPress       = 0;                                  // Keeps track display choice and how often the rotary is pressed.
uint32_t RotaryPressTimer  = 0;
byte     NoofRotaryPressed = 0;

//------------------------------------------------------------------------------
// LDR PHOTOCELL
//------------------------------------------------------------------------------
//                                                                                            //
const byte SLOPEBRIGHTNESS    = 80;                                                           // Steepness of with luminosity of the LED increases
const int  MAXBRIGHTNESS      = 255;                                                          // Maximun value in bits  for luminosity of the LEDs (1 - 255)
const byte LOWBRIGHTNESS      = 5;                                                            // Lower limit in bits of Brightness ( 0 - 255)   
byte       TestLDR            = 0;                                                            // If true LDR inf0 is printed every second in serial monitor
int        OutPhotocell;                                                                      // stores reading of photocell;
int        MinPhotocell       = 999;                                                          // stores minimum reading of photocell;
int        MaxPhotocell       = 1;                                                            // stores maximum reading of photocell;
uint32_t   SumLDRreadshour    = 0;
uint32_t   NoofLDRreadshour   = 0;

//--------------------------------------------
// CLOCK initialysations
//--------------------------------------------                                 

static uint32_t msTick;                                                                       // Number of millisecond ticks since we last incremented the second counter
byte      lastminute = 0, lasthour = 0, lastday = 0, sayhour = 0;
bool      Zelftest             = false;
bool      Is                   = true;                                                        // toggle of displaying Is or Was
bool      ZegUur               = true;                                                        // Say or not say Uur in NL clock
struct    tm timeinfo;                                                                        // storage of time 

//--------------------------------------------                                                //
// BLE   //#include <NimBLEDevice.h>
//--------------------------------------------
BLEServer *pServer      = NULL;
BLECharacteristic * pTxCharacteristic;
bool deviceConnected    = false;
bool oldDeviceConnected = false;
std::string ReceivedMessageBLE;

#define SERVICE_UUID           "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"                         // UART service UUID
#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"


//----------------------------------------
// WEBSERVER
//----------------------------------------
int WIFIConnected = 0;                                                                        // Is wIFI connected?
AsyncWebServer server(80);                                                                    // For OTA Over the air uploading
#include "Webpage.h"


//--------------------------------------------                                                //
// NTP
//----------------------------------------
boolean syncEventTriggered = false;                                                           // True if a time even has been triggered
NTPEvent_t ntpEvent;                                                                          // Last triggered event
//----------------------------------------
// Common
//----------------------------------------
 uint64_t Loopcounter = 0;
#define MAXTEXT 140
char sptext[MAXTEXT];                                                                        // For common print use 
bool SerialConnected = true;   
struct EEPROMstorage {                                                                       // Data storage in EEPROM to maintain them after power loss
  byte DisplayChoice    = 0;
  byte TurnOffLEDsAtHH  = 0;
  byte TurnOnLEDsAtHH   = 0;
  byte LanguageChoice   = 0;
  byte LightReducer     = 0;
  int  LowerBrightness  = 0;
  int  UpperBrightness  = 0;
  int  NVRAMmem[24];                                                                          // LDR readings
  byte BLEOnOff         = 1;
  byte NTPOnOff         = 1;
  byte WIFIOnOff        = 1;  
  byte StatusLEDOnOff   = 1;
  int  ReconnectWIFI    = 0;                                                                  // No of times WIFI reconnected 
  byte UseSDcard        = 0;
  byte WIFINoOfRestarts = 0;                                                                  // If 1 than resart MCU once
  byte UseDS3231        = 0;                                                                  // Use the DS3231 time module 
  byte LEDstrip         = 0;                                                                  // 0 = SK6812 LED strip. 1 = WS2812 LED strip
  byte ByteFuture3      = 0;                                                                  // For future use
  byte ByteFuture4      = 0;                                                                  // For future use
  int  IntFuture1       = 0;                                                                  // For future use
  int  IntFuture2       = 0;                                                                  // For future use
  int  IntFuture3       = 0;                                                                  // For future use
  int  IntFuture4       = 0;                                                                  // For future use   
  byte UseBLELongString = 0;                                                                  // Send strings longer than 20 bytes per message. Possible in IOS app BLEserial Pro 
  uint32_t OwnColour    = 0;                                                                  // Self defined colour for clock display
  uint32_t DimmedLetter = 0;
  uint32_t BackGround   = 0;
  char Ssid[30];                                                                              // 
  char Password[40];                                                                          // 
  char BLEbroadcastName[30];                                                                  // Name of the BLE beacon
  char Timezone[50];
  int  Checksum        = 0;
}  Mem; 
//--------------------------------------------                                                //
// Menu
//0        1         2         3         4
//1234567890123456789012345678901234567890----  
 char menu[][40] = {
 "A SSID B Password C BLE beacon name",
 "D Date (D15012021) T Time (T132145)",
 "E Timezone  (E<-02>2 or E<+01>-1)",
 "F Own colour  (Hex FRRGGBB)",
 "I To print this Info menu",
 "J Toggle use DS3231 RTC module",
 "K LDR reads/sec toggle On/Off", 
                      #ifdef FOURLANGUAGECLOCK
 "L L0 = NL, L1 = UK, L2 = DE",
 "  L3 = FR, L4 = Wheel",
                      #endif  // FOURLANGUAGECLOCK
 "N Display off between Nhhhh (N2208)",
 "O Display toggle On/Off",
 "P Status LED toggle On/Off", 
 "Q Display colour choice      (Q0-7)",
 "  Q0 Yellow  Q1 hourly",
 "  Q2 White   Q3 All Own",
 "  Q4 Own     Q5 Wheel",
                      #ifdef NL144CLOCK
 "  Q6 Digital",
                      #endif //NL144CLOCK
 "R Reset settings @ = Reset MCU",
 "--Light intensity settings (1-250)--",
 "S=Slope V=Min  U=Max   (S80 V5 U200)",
 "W=WIFI  X=NTP& Y=BLE  Z=Fast BLE",
 "Ed Nieuwenhuys Juni 2024" };
 
//  -------------------------------------   End Definitions  ---------------------------------------

//--------------------------------------------                                                //
// ARDUINO Setup
//--------------------------------------------
void setup() 
{
 Serial.begin(115200);                                                                        // Setup the serial port to 115200 baud //
 pinMode(secondsPin, OUTPUT);                                                                 // turn On seconds LED-pin
 pinMode(PCB_LED_D09, OUTPUT);                                                                // LED on PCB board  09
 pinMode(PCB_LED_D10, OUTPUT);                                                                // LED on PCB board  10
 Wire.begin();
 int32_t Tick = millis();  
 while (!Serial)  {if ((millis() - Tick) >2000) break;}                                       // Wait until serial port is started   
 Tekstprintln("Serial started");         
 //                                #ifdef  MOD_DS3231
 RTCklok.begin();  Tekstprintln("DS3231 RTC started");                                        // Start the DS3231 RTC-module
 //                                 #endif //MOD_DS3231
 InitStorage();    Tekstprintln("Settings loaded");                                           // Load settings from storage and check validity 
 SetStatusLED(1,0,0);
 switch (Mem.LEDstrip)
    {
      case 0: LEDstrip = LED6812strip; 
              white  = 0xFF000000; 
              lgray  = 0x66000000;  
              gray   = 0x33000000;         // The SK6812 LED has a white LED that is pure white
              dgray  = 0x22000000; 

      break;
      case 1: LEDstrip = LED2812strip; 
              white  = 0xFFFFFF;
              lgray  = 0x666666;               // R, G and B on together gives white light
              gray   = 0x333333;
              dgray  = 0x222222;         
      break;
     default: LEDstrip = LED6812strip;
              white  = 0xFF000000; 
              lgray  = 0x66000000;  
              gray   = 0x33000000;         // The SK6812 LED has a white LED that is pure white
              dgray  = 0x22000000;      
    }
 sprintf(sptext,"LED strip is %s", Mem.LEDstrip?"WS2812":"SK6812" ); Tekstprintln(sptext);
                                                                     // Set the status LED to red
 StartLeds();   Tekstprintln("LED strip started");                                            // LED RainbowCycle
 Previous_LDR_read = ReadLDR();
 
 if (Mem.BLEOnOff) StartBLEService();     Tekstprintln("BLE started");                        // Start BLE service
 if(Mem.WIFIOnOff) {Tekstprintln("Starting WIFI");  StartWIFI_NTP();   }                      // Start WIFI and optional NTP if Mem.WIFIOnOff = 1 
 SWversion();                                                                                 // Print the menu + version 
 GetTijd(0);                                                                                  // 
 Print_RTC_tijd();
 Displaytime();                                                                               // Print the tekst time in the display 
 Tekstprintln(""); 
 msTick = millis(); 
}
//--------------------------------------------                                                //
// ARDUINO Loop
//--------------------------------------------
void loop() 
{
 Loopcounter++;
 EverySecondCheck();  
 CheckDevices();
}
//--------------------------------------------                                                //
// COMMON Check connected input devices
//--------------------------------------------
void CheckDevices(void)
{
 CheckBLE();                                                                                  // Something with BLE to do?
 SerialCheck();                                                                               // Check serial port every second 
 ElegantOTA.loop();                                                                           // For Over The Air updates This loop block is necessary for ElegantOTA to handle reboot after OTA update.
// ButtonsCheck();                                                                            // Check if buttons pressed
}
//--------------------------------------------                                                //
// COMMON Update routine done every second
//--------------------------------------------
void EverySecondCheck(void)
{
 static bool Toggle = 1;
 uint32_t msLeap = millis() - msTick;                                                         // 
 if (msLeap >999)                                                                             // Every second enter the loop
 {
  GetTijd(0);                                                                                 // get the time for the seconds 
  msTick = millis();
  Toggle = !Toggle;
  if (Mem.StatusLEDOnOff)  
     {
      SetStatusLED(0,WIFIConnected,deviceConnected); 
      digitalWrite(LED_BUILTIN, Toggle);                                                      // turn the LED off by making the voltage LOW
      digitalWrite(PCB_LED_D09, Toggle);                                                      //
      digitalWrite(PCB_LED_D10, 1-Toggle);                                                    //
     }
  SetSecondColour();                                                                          // Set the colour per second of 'IS' and 'WAS' 
  DimLeds(TestLDR);                                                                           // Every second an intensitiy check and update from LDR reading 
  if (timeinfo.tm_min != lastminute) EveryMinuteUpdate();                                     // Enter the every minute routine after one minute; 
//if (Loopcounter < 10) ESP.restart();
  Loopcounter=0;
 }  
}
//--------------------------------------------                                                //
// COMMON Update routine done every minute
//-------------------------------------------- 
void EveryMinuteUpdate(void)
{   
 lastminute = timeinfo.tm_min;  
 GetTijd(0);
 Displaytime();
 Print_RTC_tijd();
 DimLeds(true);   
// Print_tijd();                                                                              // sprintf(sptext,"NTP:%s", NTP.getTimeDateString());   Tekstprintln(sptext);  
 //  if(WiFi.localIP()[0]>0) WIFIConnected = true; else WIFIConnected = false;                // To be sure connection is still there
 if(timeinfo.tm_hour != lasthour) EveryHourUpdate();
}
//--------------------------------------------                                                //
// COMMON Update routine done every hour
//--------------------------------------------
void EveryHourUpdate(void)
{
  if(Mem.WIFIOnOff)
    {
     if( WiFi.localIP()[0]==0 && strlen(Mem.Password)>4)                                         // if lost connection and a password entered
       {
        sprintf(sptext, "Disconnected from station, attempting reconnection.");
        Tekstprintln(sptext);
        WiFi.reconnect();
       }
    }
 lasthour = timeinfo.tm_hour;
 if (!Mem.StatusLEDOnOff) SetStatusLED(0,0,0);                                                // If for some reason the LEDs are ON and after a MCU restart turn them off.  
 if(timeinfo.tm_hour == Mem.TurnOffLEDsAtHH){ LEDsAreOff = true;  ClearScreen(); }            // Is it time to turn off the LEDs?
 if(timeinfo.tm_hour == Mem.TurnOnLEDsAtHH)   LEDsAreOff = false;                             // 
 Mem.NVRAMmem[lasthour] =(byte)((SumLDRreadshour / NoofLDRreadshour?NoofLDRreadshour:1));     // Update the average LDR readings per hour
 SumLDRreadshour  = 0;
 NoofLDRreadshour = 0;
 if (timeinfo.tm_mday != lastday) EveryDayUpdate();  
}
//--------------------------------------------                                                //
// COMMON Update routine done every day
//--------------------------------------------
void EveryDayUpdate(void)
{
 if(timeinfo.tm_mday != lastday) 
   {
    lastday           = timeinfo.tm_mday; 
    Previous_LDR_read = ReadLDR();                                                            // to have a start value
    MinPhotocell      = Previous_LDR_read;                                                    // Stores minimum reading of photocell;
    MaxPhotocell      = Previous_LDR_read;                                                    // Stores maximum reading of photocell;
//    StoreStructInFlashMemory();                                                             // 
    }
}

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
     ReworkInputString(SerialString);                                                    // Rework ReworkInputString();
     SerialString = "";
    }
}

//--------------------------------------------                                                //
// COMMON Reset to default settings
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
 Mem.BLEOnOff         = 1;                                                                    // default BLE On
 Mem.UseBLELongString = 0;                                                                    // Default off. works only with iPhone/iPad with BLEserial app
 Mem.NTPOnOff         = 0;                                                                    // NTP default off
 Mem.WIFIOnOff        = 0;                                                                    // WIFI default off
 Mem.ReconnectWIFI    = 0;                                                                    // Correct time if necessary in seconds
 Mem.WIFINoOfRestarts = 0;                                                                    //  
 Mem.UseSDcard        = 0;                                                                    // default off
 Mem.UseDS3231        = 0;                                                                    // Default off
 Mem.LEDstrip         = 0;                                                                    // 0 = SK6812, 1=WS2812
 Previous_LDR_read    = ReadLDR();                                                            // Read LDR to have a start value. max = 4096/8 = 255
 MinPhotocell         = Previous_LDR_read;                                                    // Stores minimum reading of photocell;
 MaxPhotocell         = Previous_LDR_read;                                                    // Stores maximum reading of photocell;                                            
 TestLDR              = 0;                                                                    // If true LDR display is printed every second
 
 strcpy(Mem.Ssid,"");                                                                         // Default SSID
 strcpy(Mem.Password,"");                                                                     // Default password
 strcpy(Mem.BLEbroadcastName,"NanoESP32Clock");
 strcpy(Mem.Timezone,"CET-1CEST,M3.5.0,M10.5.0/3");                                           // Central Europe, Amsterdam, Berlin etc.                                                         // WIFI On  

 Tekstprintln("**** Reset of preferences ****"); 
 StoreStructInFlashMemory();                                                                  // Update Mem struct       
 GetTijd(0);                                                                                  // Get the time and store it in the proper variables
 SWversion();                                                                                 // Display the version number of the software
 //SetSecondColour();            Doet display time al                                                               // Set the colour per second of 'IS' and 'WAS'  
 Displaytime();
}
//--------------------------------------------                                                //
// COMMON common print routines
//--------------------------------------------
void Tekstprint(char const *tekst)    { if(Serial) Serial.print(tekst);  SendMessageBLE(tekst);sptext[0]=0;   } 
void Tekstprintln(char const *tekst)  { sprintf(sptext,"%s\n",tekst); Tekstprint(sptext);  }
void TekstSprint(char const *tekst)   { printf(tekst); sptext[0]=0;}                          // printing for Debugging purposes in serial monitor 
void TekstSprintln(char const *tekst) { sprintf(sptext,"%s\n",tekst); TekstSprint(sptext); }
//--------------------------------------------                                                //
//  COMMON String upper
//--------------------------------------------
void to_upper(char* string)
{
 const char OFFSET = 'a' - 'A';
 while (*string) {(*string >= 'a' && *string <= 'z') ? *string -= OFFSET : *string;   string++;  }
}
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
 Mem.DisplayChoice   = _min(Mem.DisplayChoice, ANALOOG);                                      // Constrain the value to valid ranges 
 if(Mem.OwnColour == 0) Mem.OwnColour = green;                                                // If memory is empty cq black colour then store default value, green  
 Mem.LightReducer    = constrain(Mem.LightReducer,1,250);                                     // 
 Mem.LowerBrightness = constrain(Mem.LowerBrightness, 1, 250);                                // 
 Mem.UpperBrightness = _min(Mem.UpperBrightness, 255); 
 if(strlen(Mem.Password)<5 || strlen(Mem.Ssid)<3)     Mem.WIFIOnOff = Mem.NTPOnOff = 0;       // If ssid or password invalid turn WIFI/NTP off
 
 StoreStructInFlashMemory();
}
//--------------------------------------------                                                //
// COMMON Store mem.struct in FlashStorage or SD
// Preferences.h  
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
// COMMON Get data from FlashStorage
// Preferences.h
//--------------------------------------------
void GetStructFromFlashMemory(void)
{
 FLASHSTOR.begin("Mem", false);
 FLASHSTOR.getBytes("Mem", &Mem, sizeof(Mem) );
 FLASHSTOR.end(); 

// Can be used as alternative if no SD card
//  File myFile = SPIFFS.open("/MemStore.txt");  FILE_WRITE); myFile.read((byte *)&Mem, sizeof(Mem));  myFile.close();

 sprintf(sptext,"Mem.Checksum = %d",Mem.Checksum);Tekstprintln(sptext); 
}
//--------------------------------------------                                                //
//  COMMON Input from Bluetooth or Serial
//--------------------------------------------
void ReworkInputString(String InputString)
{
//  char ff[50];  
//  InputString.toCharArray(ff,InputString.length());                                            // Convert a String to char array
//  sprintf(sptext,"Inputstring: %s  Lengte : %d\n", ff,InputString.length());                 // Tekstprint(sptext);
 if(InputString.length()> 40){Serial.printf("Input string too long (max40)\n"); return;}      // If garbage return
 for (int n=0; n<InputString.length()+1; n++)                                                 // remove CR and LF
    if (InputString[n] == 10 || InputString[n]==13) InputString.remove(n,1);
   
 sptext[0] = 0;                                                                               // Empty the sptext string
 
 if(InputString[0] > 31 && InputString[0] <127)                                               // Does the string start with a letter?
  { 
  switch (InputString[0])
   {
    case 'A':
    case 'a': 
            if (InputString.length() >4 )
            {
             InputString.substring(1).toCharArray(Mem.Ssid,InputString.length());
             sprintf(sptext,"SSID set: %s", Mem.Ssid);  
            }
            else sprintf(sptext,"**** Length fault. Use between 4 and 30 characters ****");
            break;
    case 'B':
    case 'b': 
           if (InputString.length() >4 )
            {  
             InputString.substring(1).toCharArray(Mem.Password,InputString.length());
             sprintf(sptext,"Password set: %s\n Enter @ to reset ESP32 and connect to WIFI and NTP\n WIFI and NTP are turned ON", Mem.Password); 
             Mem.NTPOnOff        = 1;                                                         // NTP On
             Mem.WIFIOnOff       = 1;                                                         // WIFI On  
            }
            else sprintf(sptext,"%s,**** Length fault. Use between 4 and 40 characters ****",Mem.Password);
            break;   
    case 'C':
    case 'c': 
           if (InputString.length() >4 )
            {  
             InputString.substring(1).toCharArray(Mem.BLEbroadcastName,InputString.length());
             sprintf(sptext,"BLE broadcast name set: %s", Mem.BLEbroadcastName); 
             Mem.BLEOnOff        = 1;                                                         // BLE On
            }
            else sprintf(sptext,"**** Length fault. Use between 4 and 30 characters ****");
            break;      
    case 'D':
    case 'd':  
            if (InputString.length() == 9 )
              {
               timeinfo.tm_mday = (int) SConstrainInt(InputString,1,3,0,31);
               timeinfo.tm_mon  = (int) SConstrainInt(InputString,3,5,0,12) - 1; 
               timeinfo.tm_year = (int) SConstrainInt(InputString,5,9,2000,9999) - 1900;
               SetRTCTime();
               Print_tijd();  //Tekstprintln(sptext);                                         // Print_Tijd() fills sptext with time string
              }
            else sprintf(sptext,"****\nLength fault. Enter Dddmmyyyy\n****");
            break;
    case 'E':
    case 'e':  
           if (InputString.length() >2 )
            {  
             InputString.substring(1).toCharArray(Mem.Timezone,InputString.length());
             sprintf(sptext,"Timezone set: %s", Mem.Timezone); 
            }
            else sprintf(sptext,"**** Length fault. Use more than 2 characters ****");
            break;  
    case 'F':
    case 'f':  
             if (InputString.length() == 7 )
               {
                LetterColor = Mem.OwnColour = HexToDec(InputString.substring(1,7));           // Display letter color 
                sprintf(sptext,"Font colour stored: 0X%06X", Mem.OwnColour);
                Tekstprintln("**** Own colour changed ****");    
                LedsOff(); 
                Displaytime();
               }
             else sprintf(sptext,"****Length fault. Enter Frrggbb hexadecimal (0 - F)****\nStored: 0X%06X", Mem.OwnColour);              
             break;
    case 'G':
    case 'g':  
             if (InputString.length() == 7 )
               {
                Mem.DimmedLetter = HexToDec(InputString.substring(1,7));                      // Display letter color 
                sprintf(sptext,"Dimmed colour stored; 0X%06X", Mem.DimmedLetter);
                Tekstprintln("**** Dimmed font colour changed ****");    
                Displaytime();
               }
             else sprintf(sptext,"****Length fault. Enter Grrggbb hexadecimal (0 - F)****\nStored: 0X%06X", Mem.DimmedLetter);            
             break;
    case 'H':
    case 'h':  
             if (InputString.length() == 7 )
               {
                Mem.BackGround = HexToDec(InputString.substring(1,7));                        // Display letter color 
                sprintf(sptext,"Own colour stored: 0X%06X", Mem.BackGround);
                Tekstprintln("**** BackGround colour changed ****");    
                Displaytime();
               }
             else sprintf(sptext,"****Length fault. Enter Hrrggbb hexadecimal (0 - F)****\nStored: 0X%06X", Mem.BackGround);           
             break;            
    case 'I':
    case 'i': 
            SWversion();
            break;
    case 'J':
    case 'j':
             if (InputString.length() == 1)
               {   
                Mem.UseDS3231 = 1 - Mem.UseDS3231; 
                sprintf(sptext,"Use DS3231 is %s", Mem.UseDS3231?"ON":"OFF" );
               }                                
             else sprintf(sptext,"**** Length fault. Enter J ****");
             break; 
    case 'K':
    case 'k':
             TestLDR = 1 - TestLDR;                                                           // If TestLDR = 1 LDR reading is printed every second instead every 30s
             sprintf(sptext,"TestLDR: %s \nLDR reading, min and max of one day, %%Out, loops per second and time",TestLDR? "On" : "Off");
             break;      
    case 'L':                                                                                 // Language to choose
    case 'l':
             if (InputString.length() == 2 )
               {
                byte res = (byte) InputString.substring(1,2).toInt();   
                Mem.LanguageChoice = res%5;                                                   // Result between 0 and 4
                byte ch = Mem.LanguageChoice;                
                sprintf(sptext,"Language choice:%s",ch==0?"NL":ch==1?"UK":ch==2?"DE":ch==3?"FR":ch==4?"Rotate language":"NOP"); 
//                Tekstprintln(sptext);
                lastminute = 99;                                                              // Force a minute update
               }
             else sprintf(sptext,"****\nDisplay choice length fault. Enter L0 - L4\n****"); 
             break;     
    case 'M':
    case 'm':
            if (InputString.length() == 1)
               {   
                Zelftest = 1 - Zelftest; 
                sprintf(sptext,"Zelftest: %d",Zelftest);
                Selftest();
               }
             break; 
    case 'N':
    case 'n':
             if (InputString.length() == 1 )         Mem.TurnOffLEDsAtHH = Mem.TurnOnLEDsAtHH = 0;
             if (InputString.length() == 5 )
              {
               Mem.TurnOffLEDsAtHH =(byte) InputString.substring(1,3).toInt(); 
               Mem.TurnOnLEDsAtHH  =(byte) InputString.substring(3,5).toInt(); 
              }
             Mem.TurnOffLEDsAtHH = _min(Mem.TurnOffLEDsAtHH, 23);
             Mem.TurnOnLEDsAtHH  = _min(Mem.TurnOnLEDsAtHH, 23); 
             sprintf(sptext,"Display is OFF between %2d:00 and %2d:00", Mem.TurnOffLEDsAtHH,Mem.TurnOnLEDsAtHH );
 //            Tekstprintln(sptext); 
             break;
    case 'O':
    case 'o':
             if(InputString.length() == 1)
               {
                LEDsAreOff = !LEDsAreOff;
                sprintf(sptext,"Display is %s", LEDsAreOff?"OFF":"ON" );
                if(LEDsAreOff) { ClearScreen();}                                              // Turn the display off
                else {
                  Tekstprintln(sptext); 
                  lastminute = 99;
                  Displaytime();                                                              // Turn the display on   

                }
               }
             break;                                                                   
    case 'P':
    case 'p':  
             if(InputString.length() == 1)
               {
                Mem.StatusLEDOnOff = !Mem.StatusLEDOnOff;
                if (Mem.StatusLEDOnOff )SetStatusLED(0,0,0); 
                sprintf(sptext,"StatusLEDs are %s", Mem.StatusLEDOnOff?"ON":"OFF" );               
               }
             break;        

    case 'q':
    case 'Q':   
             if (InputString.length() == 1 )
               {
             Tekstprintln("  Q0= Default colour");
             Tekstprintln("  Q1= Hourly colour");
             Tekstprintln("  Q2= All white");
             Tekstprintln("  Q3= All Own colour");
             Tekstprintln("  Q4= Own colour, HETISWAS changing");
             Tekstprint(  "  Q5= Wheel colour");
             Tekstprint(  "  Q6= Digital display");
             sptext[0]=0;
               }
             if (InputString.length() == 2 )
               {
                Mem.DisplayChoice = (byte) InputString.substring(1,2).toInt(); 
                if (Mem.DisplayChoice>6) Mem.DisplayChoice = 0;
                sprintf(sptext,"Display choice: Q%d", Mem.DisplayChoice);
                lastminute = 99;                                                              // Force a minute update
               }    
             Displaytime();   
             break;
    case 'R':
    case 'r':
             if (InputString.length() == 1)
               {   
                Reset();
                sprintf(sptext,"\nReset to default values: Done");
                Displaytime();                                                                // Turn on the display with proper time
               }                                
             else sprintf(sptext,"**** Length fault. Enter R ****");
             break;      
    case 'S':                                                                                 // Slope. factor ( 0 - 1) to multiply brighness (0 - 255) with 
    case 's':
            if (InputString.length() < 5)
               {    
                Mem.LightReducer = (byte) SConstrainInt(InputString,1,1,255);
                sprintf(sptext,"Slope brightness changed to: %d%%",Mem.LightReducer);
               }
              break;                    
    case 'T':
    case 't':
//                                                                                            //
             if(InputString.length() == 7)  // T125500
               {
                timeinfo.tm_hour = (int) SConstrainInt(InputString,1,3,0,23);
                timeinfo.tm_min  = (int) SConstrainInt(InputString,3,5,0,59); 
                timeinfo.tm_sec  = (int) SConstrainInt(InputString,5,7,0,59);
                SetRTCTime();
                Print_tijd(); 
               }
             else sprintf(sptext,"**** Length fault. Enter Thhmmss ****");
             break;            
    case 'U':                                                                                 // factor to multiply brighness (0 - 255) with 
    case 'u':
            if (InputString.length() < 5)
               {    
                Mem.UpperBrightness = SConstrainInt(InputString,1,1,255);
                sprintf(sptext,"Upper brightness changed to: %d bits",Mem.UpperBrightness);
               }
              break;  
    case 'V':
    case 'v':  
             if (InputString.length() < 5)
               {      
                Mem.LowerBrightness = (byte) SConstrainInt(InputString,1,0,255);
                sprintf(sptext,"Lower brightness: %d bits",Mem.LowerBrightness);
               }
             break;      
    case 'W':
    case 'w':
             if (InputString.length() == 1)
               {   
                Mem.WIFIOnOff = 1 - Mem.WIFIOnOff; 
                Mem.ReconnectWIFI = 0;                                                       // Reset WIFI reconnection counter 
                Mem.NTPOnOff = Mem.WIFIOnOff;                                                // If WIFI is off turn NTP also off
                sprintf(sptext,"WIFI is %s after restart", Mem.WIFIOnOff?"ON":"OFF" );
               }                                
             else sprintf(sptext,"**** Length fault. Enter W ****");
             break; 
    case 'X':
    case 'x':
             if (InputString.length() == 1)
               {   
                Mem.NTPOnOff = 1 - Mem.NTPOnOff; 
                sprintf(sptext,"NTP is %s after restart", Mem.NTPOnOff?"ON":"OFF" );
               }                                
             else sprintf(sptext,"**** Length fault. Enter X ****");
             break; 
    case 'Y':
    case 'y':
             if (InputString.length() == 1)
               {   
                Mem.BLEOnOff = 1 - Mem.BLEOnOff; 
                sprintf(sptext,"BLE is %s after restart", Mem.BLEOnOff?"ON":"OFF" );
               }                                
             else sprintf(sptext,"**** Length fault. Enter Y ****");
             break; 
    case 'Z':
    case 'z':
             if (InputString.length() == 1)
               {   
                Mem.UseBLELongString = 1 - Mem.UseBLELongString; 
                sprintf(sptext,"Fast BLE is %s", Mem.UseBLELongString?"ON":"OFF" );
               }                                
             else sprintf(sptext,"**** Length fault. Enter U ****");
             break; 
//--------------------------------------------                                                //        
     case '!':                                                                                // Print the NTP, RTC and DS3231 time
             if (InputString.length() == 1)  PrintAllClockTimes();
             break;       
    case '@':
             if (InputString.length() == 1)
               {   
               Tekstprintln("\n*********\n ESP restarting\n*********\n");
                ESP.restart();   
               }                                
             else sprintf(sptext,"**** Length fault. Enter @ ****");
             break;     
    case '#':
             if (InputString.length() == 1)
               {   
                Zelftest = 1 - Zelftest; 
                sprintf(sptext,"Zelftest: %d",Zelftest);
                Selftest();
                Displaytime();                                                                // Turn on the display with proper time
               }                                
             else sprintf(sptext,"**** Length fault. Enter S ****");
             break; 
    case '$':
             if (InputString.length() == 1)
               {   
                Mem.UseSDcard = 1 - Mem.UseSDcard; 
                sprintf(sptext,"SD is %s after restart", Mem.UseSDcard?"used":"NOT used" );
                sprintf(sptext,"This function is not working, .... yet" );
               }                                
             else sprintf(sptext,"**** Length fault. Enter Z ****");
             break; 
     case '%':
             if (InputString.length() == 1)
               {   
                Mem.LEDstrip = 1 - Mem.LEDstrip; 
                sprintf(sptext,"LED strip is %s after restart", Mem.LEDstrip?"WS2812":"SK6812" );
               }                                
             else sprintf(sptext,"**** Length fault. ****");
             break; 
    case '&':                                                                                // Forced get NTP time and update the DS32RTC module
             if (InputString.length() == 1)
              {
               NTP.getTime();                                                                // Force a NTP time update  
               SetDS3231RTCTijd();
               SetRTCTime();    
               PrintAllClockTimes();
               } 
             break;
                      
    case '0':
    case '1':
    case '2':        
             if (InputString.length() == 6)                                                  // For compatibility input with only the time digits
              {
               timeinfo.tm_hour = (int) SConstrainInt(InputString,0,2,0,23);
               timeinfo.tm_min  = (int) SConstrainInt(InputString,2,4,0,59); 
               timeinfo.tm_sec  = (int) SConstrainInt(InputString,4,6,0,59);
               sprintf(sptext,"Time set");  
               SetRTCTime();
               Print_RTC_tijd(); 
               if(Mem.UseDS3231)  SetDS3231RTCTijd();
               } 
    default: break;
    }
  }  
 Tekstprintln(sptext); 
 StoreStructInFlashMemory();                                                                  // Update EEPROM                                     
 InputString = "";
}


//--------------------------------------------                                                //
// LDR reading are between 0 and 255. 
// ESP32 analogue read is between 0 - 4096 --   is: 4096 / 8
//--------------------------------------------
int ReadLDR(void)
{
 return analogRead(PhotoCellPin)/16;
}
//--------------------------------------------
// LED CLOCK Control the LEDs on the Nano ESP32
// 0 Low is LED off. Therefore the value is inversed with the ! Not 
//--------------------------------------------
void SetStatusLED(bool Red, bool Green, bool Blue)
{
 digitalWrite(LED_RED, !Red);                                                                 // !Red (not Red) because 1 or HIGH is LED off
 digitalWrite(LED_GREEN, !Green);
 digitalWrite(LED_BLUE, !Blue);
}
// --------------------Colour Clock Light functions -----------------------------------
//--------------------------------------------                                                //
//  LED Set color for LEDs in strip and print tekst
//---------------------------------------------
void ColorLeds(char const *Tekst, int FirstLed, int LastLed, uint32_t RGBWColor)
{ 
 Stripfill(RGBWColor, FirstLed, ++LastLed - FirstLed );                                        //
 if (!NoTextInColorLeds && strlen(Tekst) > 0 )
     {sprintf(sptext,"%s ",Tekst); Tekstprint(sptext); }                                      // Print the text  
}
//--------------------------------------------
//  LED Set color for one LED
//--------------------------------------------
void ColorLed(int Lednr, uint32_t RGBWColor)
{   
 Stripfill(RGBWColor, Lednr, 1 );
}
//--------------------------------------------                                                //
//  LED Clear display settings of the LED's
//--------------------------------------------
void LedsOff(void) 
{ 
 Stripfill(0, 0, NUM_LEDS );                                                                  // 
}
//--------------------------------------------                                                //
// LED Turn On and Off the LED's after 200 milliseconds
//--------------------------------------------
void Laatzien()
{ 
 ShowLeds();
 delay(300);
 LedsOff(); 
 CheckDevices();                                                                              // Check for input from input devices
}

//--------------------------------------------                                                //
//  LED Push data in LED strip to commit the changes
//--------------------------------------------
void ShowLeds(void)
{
 LEDstrip.show();
}
//--------------------------------------------                                                //
//  LED Set brighness of LEDs
//--------------------------------------------
void SetBrightnessLeds(byte Bright)
{
 LEDstrip.setBrightness(Bright);                                                              // Set brightness of LEDs   
 ShowLeds();
}
//--------------------------------------------
//  LED Fill the strip array for LEDFAB library
//--------------------------------------------
void Stripfill(uint32_t RGBWColor, int FirstLed, int NoofLEDs)
{   
 LEDstrip.fill(RGBWColor, FirstLed, NoofLEDs);
}
//--------------------------------------------
//  LED Strip Get Pixel Color 
//--------------------------------------------
uint32_t StripGetPixelColor(int Lednr)
{
return(LEDstrip.getPixelColor(Lednr));
}
//--------------------------------------------                                                //
//  LED convert HSV to RGB  h is from 0-360, s,v values are 0-1
//  r,g,b values are 0-255
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
//  LED function to make RGBW color
//--------------------------------------------
uint32_t FuncCRGBW( uint32_t Red, uint32_t Green, uint32_t Blue, uint32_t White)
{ 
 return ( (White<<24) + (Red << 16) + (Green << 8) + Blue );
}
//--------------------------------------------                                                //
//  LED functions to extract RGBW colors
//--------------------------------------------
 uint8_t Cwhite(uint32_t c) { return (c >> 24);}
 uint8_t Cred(  uint32_t c) { return (c >> 16);}
 uint8_t Cgreen(uint32_t c) { return (c >> 8); }
 uint8_t Cblue( uint32_t c) { return (c);      }



//--------------------------------------------                                                //
//  CLOCK Set second color
//  Set the colour per second of 'IS' and 'WAS'
//---------------------------------------------
void SetSecondColour(void)
{
                             #ifdef FOURLANGUAGECLOCK
  return;                                                                                     // 4-language clock uses SetColours() once per minute
                             #endif  //FOURLANGUAGECLOCK    

 MINColor = FuncCRGBW(15 + timeinfo.tm_min * 4, 255 - timeinfo.tm_min * 4,0,0); 
 SECColor = FuncCRGBW(15 + timeinfo.tm_sec * 4, 255 - timeinfo.tm_sec * 4,0,0 );   
                                                                                              // Light up  IS or WAS with the proper colour  
 switch (Mem.DisplayChoice)
  {
   case DEFAULTCOLOUR: LetterColor = DefaultColor;                                     break; // Yellow text with changing MIN and SEC  
   case HOURLYCOLOUR : LetterColor = HourColor[timeinfo.tm_hour];                      break; // A colour every hour
   case WHITECOLOR   : LetterColor = MINColor = SECColor = WhiteColour;                break; // all white
   case OWNCOLOUR    : LetterColor = Mem.OwnColour;                                    break; // own colour
   case OWNHETISCLR  : LetterColor = Mem.OwnColour; MINColor = SECColor = LetterColor; break; // own colour except HET IS WAS  
   case WHEELCOLOR   : LetterColor = MINColor = SECColor = Wheel((timeinfo.tm_min*4)); break; // Colour of all letters changes per second
   case DIGITAL      : LetterColor = white; MINColor = SECColor = 0;                   break; // digital display of time. No IS WAS turn color off in display
  }
 NoTextInColorLeds  = true;                                                                   // Flag to control printing of the text IS en WAS in serial
 if(Is) {IS;} 
 else {WAS;} 
 NoTextInColorLeds  = false;                                                                  // Flag to control printing of the text IS en WAS in serial
 ShowLeds();                                                                                  // Updating IS and WAS with ShowLeds is done here to avoid updating all letters every second with Displaytime function
//    Serial.print("SecColor: ");    Serial.println(SECColor,HEX);  
}

                    #ifdef FOURLANGUAGECLOCK
//--------------------------------------------                                                //
//  CLOCK Set second for 4 language clock
//--------------------------------------------
void SetColours(void)
{  
 switch (Mem.DisplayChoice)
  {
   case DEFAULTCOLOUR: LetterColor   = DefaultColor; 
                       UKLetterColor = UKDefaultColor;
                       DELetterColor = DEDefaultColor;
                       FRLetterColor = FRDefaultColor;           break;                       // HET IS WAS changing   
   case HOURLYCOLOUR : LetterColor   = HourColor[timeinfo.tm_hour];
                       UKLetterColor = HourColor[(timeinfo.tm_hour+1)%24];
                       DELetterColor = HourColor[(timeinfo.tm_hour+2)%24];
                       FRLetterColor = HourColor[(timeinfo.tm_hour+3)%24];  break;            // A colour every hour   
   case WHITECOLOR   : LetterColor   = WhiteColour; 
                       UKLetterColor = WhiteColour;
                       DELetterColor = WhiteColour;
                       FRLetterColor = WhiteColour;              break;                       // All white
   case OWNCOLOUR    : LetterColor   = Mem.OwnColour; 
                       UKLetterColor = Mem.OwnColour;
                       DELetterColor = Mem.OwnColour;
                       FRLetterColor = Mem.OwnColour;            break;
   case OWNHETISCLR  : LetterColor   = Mem.OwnColour; 
                       UKLetterColor = Mem.OwnColour;
                       DELetterColor = Mem.OwnColour;
                       FRLetterColor = Mem.OwnColour;            break;                       // Own colour HET IS WAS changing  
   case WHEELCOLOR   : LetterColor   = Wheel((timeinfo.tm_min*4));       
                       UKLetterColor = Wheel((60+timeinfo.tm_min*4));
                       DELetterColor = Wheel((120+timeinfo.tm_min*4));
                       FRLetterColor = Wheel((180+timeinfo.tm_min*4));   break;               // Colour of all letters changes per second
   case DIGITAL      : LetterColor = white;                      break;                       // digital display of time. No IS WAS turn color off in display
  }
}
                     #endif  //FOURLANGUAGECLOCK       

//------------------------------------------------------------------------------
// CLOCK Self test sequence
//------------------------------------------------------------------------------
void Selftest(void)
{ 
 GetTijd(1);                                                                                  // Prints time in Serial monitor
 LedsOff(); 
                      #ifdef ULRICH
 HET;   Laatzien(); IS;    Laatzien(); MVIJF; Laatzien(); MTIEN;   Laatzien(); MVOOR;  Laatzien(); OVER;   Laatzien();
 KWART; Laatzien(); HALF;  Laatzien(); OVER2; Laatzien(); VOOR;    Laatzien(); EEN;    Laatzien(); TWEE;   Laatzien();  
 DRIE;  Laatzien(); VIER;  Laatzien(); VIJF;  Laatzien(); ZES;     Laatzien(); ZEVEN;  Laatzien(); ACHT;   Laatzien(); 
 NEGEN; Laatzien(); TIEN;  Laatzien(); ELF;   Laatzien(); TWAALF;  Laatzien(); UUR;    Laatzien(); 
                      #endif  //ULRICH
                      #ifdef NL144CLOCK                    
 HET;   Laatzien(); IS;    Laatzien(); WAS;    Laatzien(); PRECIES; Laatzien(); MTIEN;  Laatzien();  MVIJF; Laatzien();    
 KWART; Laatzien(); VOOR;  Laatzien(); OVER;   Laatzien(); HALF;    Laatzien(); MIDDER; Laatzien();  VIJF;  Laatzien();
 TWEE;  Laatzien(); EEN;   Laatzien(); VIER;   Laatzien(); TIEN;    Laatzien(); TWAALF; Laatzien();  DRIE;  Laatzien();
 NEGEN; Laatzien(); NACHT; Laatzien(); ACHT;   Laatzien(); ZES;     Laatzien(); ZEVEN;  Laatzien();  ELF;   Laatzien(); 
 NOEN;  Laatzien(); UUR;   Laatzien(); EDSOFT; Laatzien();
                      #endif  //NL144CLOCK
 Tekstprintln("*");      
 Zelftest = false; 
 Displaytime();                                                                               // Turn on the LEDs with proper time
}
//
//--------------------------------------------                                                //
// CLOCK Say the time and load the LEDs 
// with the proper colour and intensity
// If ClearLEDstrip is true a new minute starts
//--------------------------------------------
void Displaytime()
{ 
 LedsOff();                                                                                  // Start by clearing the display to a known state   
 if( Mem.DisplayChoice == DIGITAL ) { TimePlaceDigit(timeinfo.tm_hour,timeinfo.tm_min); }
 else                                                                                        // If not a difital display 
   {
                     #ifdef NL144CLOCK
    SetSecondColour();                                                                       // Set the colour per second of 'IS' and 'WAS' 
    Dutch();                                                                                 // If TurnLEDsOff is true a new time must be send to the LEDstrip      
 
                     #endif  //NL144CLOCK
                     #ifdef ULRICH
    SetSecondColour();                                                                       // Set the colour per second of 'IS' and 'WAS' 
    Dutch();                                                                                 // If TurnLEDsOff is true a new time must be send to the LEDstrip      
 
                     #endif  //ULRICH
                     #ifdef FOURLANGUAGECLOCK
    SetColours();                                                                            // Set the colours for the chosen palette   
    Dutch();
    English();
    German();
    French();

                     #endif  //FOURLANGUAGECLOCK       
   }  
 ShowLeds();                                                                                  // and turn on the LEDs
}


//--------------------------------------------                                                //
//  LED Dim the leds measured by the LDR and print values
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
 if (Mem.UseDS3231)   sprintf(sptext,"LDR:%3d (%3d-%3d) %2d%% %5lld l/s %0.0f°C ",
               LDRread,MinPhotocell,MaxPhotocell,(int)(OutPhotocell*100/255),Loopcounter,RTCklok.getTemperature()); 
  else
                  sprintf(sptext,"LDR:%3d (%3d-%3d) %2d%% %5lld l/s ",
               LDRread,MinPhotocell,MaxPhotocell,(int)(OutPhotocell*100/255),Loopcounter);   
   Tekstprint(sptext);
   Print_tijd();  
  }
 if(LEDsAreOff) OutPhotocell = 0;
 SetBrightnessLeds(OutPhotocell);     // values between 0 and 255
}
//--------------------------------------------                                                //
//  LED Turn On en Off the LED's
//--------------------------------------------
void Play_Lights()
{
 for(int i=0; i<NUM_LEDS; i++) { ColorLeds("",i,i,chromeyellow); ShowLeds(); }
 WhiteOverRainbow(0, 0, 5 );
 WhiteOverRainbow(0,0, 5 );  // wait, whiteSpeed, whiteLength
 LedsOff();
}

//--------------------------------------------                                                //
//  DISPLAY
//  Clear the display
//--------------------------------------------
void ClearScreen(void)
{
 LedsOff();
}
//--------------------------------------------                                                //
//  LED Wheel
//  Input a value 0 to 255 to get a color value.
//  The colours are a transition r - g - b - back to r.
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
//  LED RainbowCycle
//--------------------------------------------
void StartLeds(void) 
{
 uint16_t i, j;
 LEDstrip.setBrightness(BRIGHTNESS);                                                         // Set initial brightness of LEDs   
 for(j=0; j<256 ; j++) 
  {                                                                                          // One cycle of all colors on wheel
   for(i=0; i< 32; i++)  ColorLeds("",i,i,Wheel(((i * 256 / 32) + j) & 255));
   ShowLeds();
  }
}

//--------------------------------------------                                                //
//  LED RainbowCycle
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
//  LED WhiteOverRainbow
//--------------------------------------------
void WhiteOverRainbow(uint32_t wait, uint8_t whiteSpeed, uint32_t whiteLength ) 
{
 if(whiteLength >= NUM_LEDS) whiteLength = NUM_LEDS - 1;
 uint32_t head = whiteLength - 1;
 uint32_t tail = 0;
 uint32_t loops = 1;
 uint32_t loopNum = 0;
 static unsigned long lastTime = 0;
 while(true)
  {
    for(uint32_t j=0; j<256; j++) 
     {
      for(uint32_t i=0; i<NUM_LEDS; i++) 
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
      if(loopNum == loops) return;
      head %= NUM_LEDS;
      tail %= NUM_LEDS;
      ShowLeds();
      delay(wait);
    }
  }  // end while
}
//--------------------------------------------                                                //
//  LED Place digits 0 - 9 in Matrix display
// First row and column = 0, PosX,PosY is left top position of 3x5 digit
// Calculate position LED #define MATRIX_WIDTH 12 #define MATRIX_HEIGHT 12
//--------------------------------------------
void Zet_Pixel(byte Cijfer,byte Pos_X, byte Pos_Y) 
{ 
 uint32_t LEDnum;
 uint32_t OrgColor;
 for(int i=0;i<3;i++)
  {  
   for(int j=0;j<5;j++)
   {
    int c = pgm_read_byte_near ( &Getal[Cijfer][i][j]);                                     // Reading a byte of data stored in program memory (PROGMEM area) at a specified address.
    if ( c )                                                                                // if Digit == 1 then turn that light on
     {                                                                                      // Serial.print(strip.getPixelColor(LEDnum) & 0X00FFFFFF,HEX); Serial.print(" ");
      if((Pos_Y+j)%2) LEDnum = ((MATRIX_WIDTH -1) - (Pos_X + i) + (Pos_Y + j) * (MATRIX_HEIGHT));
      else            LEDnum =                      (Pos_X + i) + (Pos_Y + j) * (MATRIX_HEIGHT); 
      StripGetPixelColor(LEDnum) && white ? OrgColor = LetterColor : OrgColor = 0;
      ColorLeds("",  LEDnum, LEDnum, (uint32_t)(OrgColor + white));
     }
   }
  }
}
//--------------------------------------------                                                //
//  LED Time in four digits in display
//--------------------------------------------
void TimePlaceDigit(byte uur, byte minuut)
{   
 Zet_Pixel(    uur / 10, 2, 1);  Zet_Pixel(    uur % 10, 7, 1);
 Zet_Pixel( minuut / 10, 2, 7);  Zet_Pixel( minuut % 10, 7, 7);
}

//--------------------------------------------                                                //
//  LED In- or decrease light intensity value i.e. Slope
//--------------------------------------------
void WriteLightReducer(int amount)
{
 int value = Mem.LightReducer + amount;                                                       // Prevent byte overflow by making it an integer before adding
 Mem.LightReducer = (byte) min(value,  255);                                                         // May not be larger than 255
 sprintf(sptext,"Max brightness: %3d%%",Mem.LightReducer);
 Tekstprintln(sptext);
}

//--------------------------------------------                                                //
// CLOCK Version info
//--------------------------------------------
void SWversion(void) 
{ 
 #define FILENAAM (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
 PrintLine(35);
 for (uint8_t i = 0; i < sizeof(menu) / sizeof(menu[0]); Tekstprintln(menu[i++]));
 PrintLine(35);
 byte ch = Mem.LanguageChoice;
 byte dp = Mem.DisplayChoice;
 sprintf(sptext,"Display off between: %02dh - %02dh",Mem.TurnOffLEDsAtHH, Mem.TurnOnLEDsAtHH);  Tekstprintln(sptext);
 sprintf(sptext,"Display choice: %s",dp==0?"Yellow":dp==1?"Hourly":dp==2?"White":
                      dp==3?"All Own":dp==4?"Own":dp==5?"Wheel":dp==6?"Digital":dp==7?"Analog":"NOP");      
                                                                                              Tekstprintln(sptext);
 sprintf(sptext,"Slope: %d     Min: %d     Max: %d ",Mem.LightReducer, Mem.LowerBrightness,Mem.UpperBrightness);  
                                                                                             Tekstprintln(sptext);
 sprintf(sptext,"SSID: %s", Mem.Ssid);                                                       Tekstprintln(sptext); 
// sprintf(sptext,"Password: %s", Mem.Password);                                             Tekstprintln(sptext);
 sprintf(sptext,"BLE name: %s", Mem.BLEbroadcastName);                                       Tekstprintln(sptext);
 sprintf(sptext,"IP-address: %d.%d.%d.%d (/update)", WiFi.localIP()[0], WiFi.localIP()[1], WiFi.localIP()[2], WiFi.localIP()[3] );  Tekstprintln(sptext);
 sprintf(sptext,"Timezone:%s", Mem.Timezone);                                                Tekstprintln(sptext); 
 sprintf(sptext,"%s %s %s %s", Mem.WIFIOnOff?"WIFI=On":"WIFI=Off", 
                               Mem.NTPOnOff? "NTP=On":"NTP=Off",
                               Mem.BLEOnOff? "BLE=On":"BLE=Off",
                               Mem.UseBLELongString? "FastBLE=On":"FastBLE=Off" );           Tekstprintln(sptext);
 sprintf(sptext,"Language choice: %s",
         ch==0?"NL":ch==1?"UK":ch==2?"DE":ch==3?"FR":ch==4?"Rotate language":"NOP");         Tekstprintln(sptext);
 sprintf(sptext,"LED strip: %s (Send %% to switch)", Mem.LEDstrip==0?"SK6812":Mem.LEDstrip==1?"WS2812":"NOP");   
                                                                                             Tekstprintln(sptext);        
 sprintf(sptext,"Software: %s",FILENAAM);                                                    Tekstprintln(sptext);  //VERSION); 
 //Print_RTC_tijd(); Tekstprintln(""); 
 PrintLine(35);
}
//--------------------------------------------                                                //
// CLOCK PrintLine
//--------------------------------------------
void PrintLine(byte Lengte)
{
 for(int n=0; n<Lengte; n++) sptext[n]='_';
 sptext[Lengte] = 0;
 Tekstprintln(sptext);
}

//--------------------------- Time functions --------------------------
//--------------------------------------------                                                //
// RTC Get time from NTP cq RTC 
// and store it in timeinfo struct
//--------------------------------------------
/*void GetTijd(byte printit)
{
 if(Mem.NTPOnOff)  getLocalTime(&timeinfo);                                                    // if NTP is running get the loacal time
 else 
  { 
   time_t now;
   time(&now);
   localtime_r(&now, &timeinfo);
  } 
 if (printit)  Print_RTC_tijd();                                                              // otherwise time in OK struct timeonfo
}
*/

//--------------------------------------------                                                //
// RTC Get time from NTP cq internal ESP32 RTC 
// and store it in timeinfo struct
// return local time in unix time format
//--------------------------------------------
time_t GetTijd(byte printit)
{
 time_t now;
 
 if (Mem.UseDS3231) GetDS3231RTCTijd(0);                                                 // if the DS3231 is attached and used get its time in timeinfo struct
 else
    { 
     if(Mem.NTPOnOff)  getLocalTime(&timeinfo);                                                   // if NTP is running get the local time
     else { time(&now); localtime_r(&now, &timeinfo);}                                            // else get the time from the internal RTC 
    }
 if (printit)  Print_RTC_tijd();                                                              // 
 localtime(&now);
 return now;                                                                                  // return the unixtime in seconds
}
//--------------------------------------------                                                //
// RTC prints time to serial
//--------------------------------------------
void Print_RTC_tijd(void)
{
 sprintf(sptext,"%02d/%02d/%04d %02d:%02d:%02d ", 
     timeinfo.tm_mday,timeinfo.tm_mon+1,timeinfo.tm_year+1900,
     timeinfo.tm_hour,timeinfo.tm_min,timeinfo.tm_sec);
 Tekstprint(sptext);
}
//                 #ifdef MOD_DS3231
byte   Isecond, Iminute, Ihour , Iday, Imonth, Iyear; 
//--------------------------------------------                                                //
// DS3231 prints time to serial
// reference https://adafruit.github.io/RTClib/html/class_r_t_c___d_s3231.html
//--------------------------------------------
void PrintDS3231RTCtijd(void)
{
 DateTime RTCclock = RTCklok.now();
 sprintf(sptext,"%02d/%02d/%04d %02d:%02d:%02d ", 
      RTCclock.day(),RTCclock.month(),RTCclock.year(), RTCclock.hour(),RTCclock.minute(),RTCclock.second());
 Tekstprint(sptext);
}
//--------------------------------------------                                                //
// DS3231 Get time from DS3231 and stores it in 
// timeinfo struct the clock uses for displaying the time
//--------------------------------------------
void GetDS3231RTCTijd(byte printit)
{
 Inow = RTCklok.now();
 timeinfo.tm_year = Inow.year() - 1900;
 timeinfo.tm_mon  = Inow.month() - 1;
 timeinfo.tm_mday = Inow.day();
 timeinfo.tm_hour = Inow.hour();
 timeinfo.tm_min  = Inow.minute();
 timeinfo.tm_sec  = Inow.second();
 time_t unixtime = mktime(&timeinfo);
 sprintf(sptext,"unixtime = %lld", (long long) unixtime); 
 //Tekstprintln(sptext);
// if (Ihour > 24) { Ihour = random(12)+1; Iminute = random(60)+1; Isecond = 30;}  // set a time if time module is absent or defect
 if (printit)  Print_RTC_tijd(); 
}

//--------------------------------------------                                                //
// DS3231 Set time in module and print it
//--------------------------------------------
void SetDS3231RTCTijd(void)
{ 
 RTCklok.adjust(DateTime(timeinfo.tm_year+1900,timeinfo.tm_mon+1,timeinfo.tm_mday,timeinfo.tm_hour,timeinfo.tm_min,timeinfo.tm_sec));
}


// Below function to be used without using RTCLIB. 
//--------------------------------------------                                                //
// DS3231 Get temperature from module
//--------------------------------------------
int GetDS3231Temp(void)
{
 byte tMSB, tLSB;
 int  temp3231;
 
  Wire.beginTransmission(DS3231_I2C_ADDRESS);                                                // Temp registers (11h-12h) get updated automatically every 64s
  Wire.write(0x11);
  Wire.endTransmission();
  Wire.requestFrom(DS3231_I2C_ADDRESS, 2);
 
  if(Wire.available()) 
  {
    tMSB = Wire.read();                                                                       // 2's complement int portion
    tLSB = Wire.read();                                                                       // fraction portion 
    temp3231 = (tMSB & 0b01111111);                                                           // do 2's math on Tmsb
    temp3231 += ( (tLSB >> 6) * 0.25 ) + 0.5;                                                 // only care about bits 7 & 8 and add 0.5 to round off to integer   
  }
  else   {temp3231 = -273; }  
  return (temp3231);
}

//--------------------------------------------                                                //
// DS3231  Convert normal decimal numbers to binary coded decimal and vise versa
//--------------------------------------------
byte decToBcd(byte val){ return( (val/10*16) + (val%10) );}

byte bcdToDec(byte val){ return( (val/16*10) + (val%16) );}

//--------------------------------------------                                                //
// DS3231 Set time in module DS3231
//--------------------------------------------
void setDS3231time(byte second, byte minute, byte hour, byte dayOfWeek, byte
dayOfMonth, byte month, byte year)
{
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0x0E); // select register
  Wire.write(0b00011100); // write register bitmap, bit 7 is /EOS
  Wire.write(decToBcd(second)); // set seconds
  Wire.write(decToBcd(minute)); // set minutes
  Wire.write(decToBcd(hour)); // set hours
  Wire.write(decToBcd(dayOfWeek)); // set day of week (1=Sunday, 7=Saturday)
  Wire.write(decToBcd(dayOfMonth)); // set date (1 to 31)
  Wire.write(decToBcd(month)); // set month
  Wire.write(decToBcd(year)); // set year (0 to 99)
  Wire.endTransmission();
}

//--------------------------------------------                                                //
// DS3231 reads time in module DS3231
//--------------------------------------------
void readDS3231time(byte *second,byte *minute,byte *hour,byte *dayOfWeek,byte *dayOfMonth,byte *month,byte *year)
{
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // set DS3231 register pointer to 00h
  Wire.endTransmission();
  Wire.requestFrom(DS3231_I2C_ADDRESS, 7);
  // request seven bytes of data from DS3231 starting from register 00h
  *second     = bcdToDec(Wire.read() & 0x7f);
  *minute     = bcdToDec(Wire.read());
  *hour       = bcdToDec(Wire.read() & 0x3f);
  *dayOfWeek  = bcdToDec(Wire.read());
  *dayOfMonth = bcdToDec(Wire.read());
  *month      = bcdToDec(Wire.read());
  *year       = bcdToDec(Wire.read());
}

void displayTime()
{
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);     // retrieve data from DS3231
  Serial.print(hour, DEC);
}
 //                #endif// MOD_DS3231
//--------------------------------------------                                                //
// NTP print the NTP time for the timezone set 
//--------------------------------------------
void PrintNTP_tijd(void)
{
 sprintf(sptext,"%s  ", NTP.getTimeDateString());  
 Tekstprint(sptext);              // 17/10/2022 16:08:15

// int dd,mo,yy,hh,mm,ss=0;      // nice method to extract the values from a string into variabeles
// if (sscanf(sptext, "%2d/%2d/%4d %2d:%2d:%2d", &dd, &mo, &yy, &hh, &mm, &ss) == 6) 
//     {      sprintf(sptext,"%02d:%02d:%02d %02d-%02d-%04d", hh,mm,ss,dd,mo,yy);      Tekstprintln(sptext); }
}

//--------------------------------------------                                                //
// NTP print the NTP UTC time 
//--------------------------------------------
void PrintUTCtijd(void)
{
 time_t tmi;
 struct tm* UTCtime;
 time(&tmi);
 UTCtime = gmtime(&tmi);
 sprintf(sptext,"UTC: %02d:%02d:%02d %02d-%02d-%04d  ", 
     UTCtime->tm_hour,UTCtime->tm_min,UTCtime->tm_sec,
     UTCtime->tm_mday,UTCtime->tm_mon+1,UTCtime->tm_year+1900);
 Tekstprint(sptext);   
}

//--------------------------------------------                                                //
// NTP processSyncEvent 
//--------------------------------------------
void processSyncEvent (NTPEvent_t ntpEvent) 
{
 switch (ntpEvent.event) 
    {
        case timeSyncd:
        case partlySync:
        case syncNotNeeded:
        case accuracyError:
            sprintf(sptext,"[NTP-event] %s\n", NTP.ntpEvent2str (ntpEvent));
            Tekstprint(sptext);
            break;
        default:
            break;
    }
}
//--------------------------------------------                                                //
// RTC Fill sptext with time
/*
 * Option to try
void timePrint() {
  struct tm now;
  getLocalTime(&now,0);
  if (now.tm_year >= 117) Serial.println(&now, "%B %d %Y %H:%M:%S (%A)");
}
*/


//--------------------------------------------
void Print_tijd(){ Print_tijd(2);}                                                            // print with linefeed
void Print_tijd(byte format)
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
// RTC Set time from global timeinfo struct
// Check if values are within range

// Option to try
//  
//  This function sets the internal time. The timeval struct is seconds, milliseconds. settimeofday parameters are
//   epoch_time, timezone. 
//
//int setUnixtime(int32_t unixtime) {
//  timeval epoch = {unixtime, 0};
//  return settimeofday((const timeval*)&epoch, 0);
//  }

//--------------------------------------------
void SetRTCTime(void)
{ 
 time_t t = mktime(&timeinfo);  // t= unixtime
 sprintf(sptext, "Setting time: %s", asctime(&timeinfo));    Tekstprintln(sptext);
 struct timeval now = { .tv_sec = t , .tv_usec = 0};
 settimeofday(&now, NULL);
 GetTijd(0);                                                                                // Synchronize time with RTC clock
 Displaytime();
 Print_tijd();
}

//--------------------------------------------                                                //
// Print all the times available 
//--------------------------------------------
void PrintAllClockTimes(void)
{
 Tekstprint(" Clock time: ");
 GetTijd(1);
 Tekstprint("\n   NTP time: ");
 PrintNTP_tijd();
 //                              #ifdef MOD_DS3231
 Tekstprint("\nDS3231 time: ");
 PrintDS3231RTCtijd();
 //                             #endif //MOD_DS3231
 Tekstprintln(""); 
}
//                                                                                            //
// ------------------- End  Time functions 

//--------------------------------------------                                                //
//  CLOCK Convert Hex to uint32
//--------------------------------------------
uint32_t HexToDec(String hexString) 
{
 uint32_t decValue = 0;
 int nextInt;
 for (uint8_t i = 0; i < hexString.length(); i++) 
  {
   nextInt = int(hexString.charAt(i));
   if (nextInt >= 48 && nextInt <= 57)  nextInt = map(nextInt, 48, 57, 0, 9);
   if (nextInt >= 65 && nextInt <= 70)  nextInt = map(nextInt, 65, 70, 10, 15);
   if (nextInt >= 97 && nextInt <= 102) nextInt = map(nextInt, 97, 102, 10, 15);
   nextInt = constrain(nextInt, 0, 15);
   decValue = (decValue * 16) + nextInt;
  }
 return decValue;
}


//--------------------------------------------                                                //
//  CLOCK Dutch clock display
//--------------------------------------------
                     #ifdef NL144CLOCK
void Dutch(void)
{
HET;                                                                                          // HET  is always on
 if (timeinfo.tm_hour == 12 && timeinfo.tm_min == 0 && random(2)==0) { IS; NOEN; return; }
 if (timeinfo.tm_hour == 00 && timeinfo.tm_min == 0 && random(2)==0) { IS; MIDDER; NACHT; return; } 
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

 switch (timeinfo.tm_min % 5)                                                                // The minutes modulo 5
 {
  case  0: break;
  case  1: MIN1; break;
  case  2: MIN1; MIN2; break;
  case  3: MIN1; MIN2; MIN3; break;
  case  4: MIN1; MIN2; MIN3; MIN4;  
 }
}
                                    #endif //NL144CLOCK
                                    #ifdef ULRICH
void Dutch(void)
{
HET;                                                                                          // HET  is always on
switch (timeinfo.tm_min)
 {
  case  0: IS;  break;
  case  1: IS;  break; 
  case  2: IS;  break; 
  case  3: IS;  break; 
  case  4: IS;  break; 
  case  5: IS;  MVIJF; OVER; break;
  case  6: IS;  MVIJF; OVER; break;
  case  7: IS;  MVIJF; OVER; break;
  case  8: IS;  MVIJF; OVER; break;
  case  9: IS;  MVIJF; OVER; break;
  case 10: IS;  MTIEN; OVER; break;
  case 11: IS;  MTIEN; OVER; break;
  case 12: IS;  MTIEN; OVER; break;
  case 13: IS;  MTIEN; OVER; break;
  case 14: IS;  MTIEN; OVER; break;
  case 15: IS;  KWART; OVER2; break;
  case 16: IS;  KWART; OVER2; break;
  case 17: IS;  KWART; OVER2; break;
  case 18: IS;  KWART; OVER2; break;
  case 19: IS;  KWART; OVER2; break;
  case 20: IS;  MTIEN; MVOOR; HALF; break;
  case 21: IS;  MTIEN; MVOOR; HALF; break;
  case 22: IS;  MTIEN; MVOOR; HALF; break;
  case 23: IS;  MTIEN; MVOOR; HALF; break;
  case 24: IS;  MTIEN; MVOOR; HALF; break;
  case 25: IS;  MVIJF; MVOOR; HALF; break;
  case 26: IS;  MVIJF; MVOOR; HALF; break;
  case 27: IS;  MVIJF; MVOOR; HALF; break;
  case 28: IS;  MVIJF; MVOOR; HALF; break;
  case 29: IS;  MVIJF; MVOOR; HALF; break;
  case 30: IS;  HALF; break;
  case 31: IS;  HALF; break;
  case 32: IS;  HALF; break;
  case 33: IS;  HALF; break;
  case 34: IS;  HALF; break;
  case 35: IS;  MVIJF; OVER; HALF; break;
  case 36: IS;  MVIJF; OVER; HALF; break;
  case 37: IS;  MVIJF; OVER; HALF; break;
  case 38: IS;  MVIJF; OVER; HALF; break;
  case 39: IS;  MVIJF; OVER; HALF; break;
  case 40: IS;  MTIEN; OVER; HALF; break;
  case 41: IS;  MTIEN; OVER; HALF; break;
  case 42: IS;  MTIEN; OVER; HALF; break;
  case 43: IS;  MTIEN; OVER; HALF; break;
  case 44: IS;  MTIEN; OVER; HALF; break;
  case 45: IS;  KWART; VOOR; break;
  case 46: IS;  KWART; VOOR; break;
  case 47: IS;  KWART; VOOR; break;
  case 48: IS;  KWART; VOOR; break;
  case 49: IS;  KWART; VOOR; break;
  case 50: IS;  MTIEN; VOOR;  break;
  case 51: IS;  MTIEN; VOOR;  break;
  case 52: IS;  MTIEN; VOOR;  break;
  case 53: IS;  MTIEN; VOOR;  break;
  case 54: IS;  MTIEN; VOOR;  break;
  case 55: IS;  MVIJF; VOOR; break;
  case 56: IS;  MVIJF; VOOR; break;
  case 57: IS;  MVIJF; VOOR; break;
  case 58: IS;  MVIJF; VOOR; break;
  case 59: IS;  MVIJF; VOOR; break;
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
                     #endif  //ULRICH

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
                     #ifdef DE
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
// BLE 
// SendMessage by BLE Slow in packets of 20 chars
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
         delay(40);                                                                           // Bluetooth stack will go into congestion, if too many packets are sent
        }
     }
   } 
}
//-----------------------------
// BLE Start BLE Classes
//------------------------------
class MyServerCallbacks: public BLEServerCallbacks 
{
 void onConnect(BLEServer* pServer) {deviceConnected = true; };
 void onDisconnect(BLEServer* pServer) {deviceConnected = false;}
};

class MyCallbacks: public BLECharacteristicCallbacks 
{
 void onWrite(BLECharacteristic *pCharacteristic) 
  {
   std::string rxValue = pCharacteristic->getValue();
   ReceivedMessageBLE = rxValue + "\n";
//   if (rxValue.length() > 0) {for (int i = 0; i < rxValue.length(); i++) printf("%c",rxValue[i]); }
//   printf("\n");
  }  
};
//--------------------------------------------                                                //
// BLE Start BLE Service
//------------------------------
void StartBLEService(void)
{
 BLEDevice::init(Mem.BLEbroadcastName);                                                       // Create the BLE Device
 pServer = BLEDevice::createServer();                                                         // Create the BLE Server
 pServer->setCallbacks(new MyServerCallbacks());
 BLEService *pService = pServer->createService(SERVICE_UUID);                                 // Create the BLE Service
 pTxCharacteristic                     =                                                      // Create a BLE Characteristic 
     pService->createCharacteristic(CHARACTERISTIC_UUID_TX, NIMBLE_PROPERTY::NOTIFY);                 
 BLECharacteristic * pRxCharacteristic = 
     pService->createCharacteristic(CHARACTERISTIC_UUID_RX, NIMBLE_PROPERTY::WRITE);
 pRxCharacteristic->setCallbacks(new MyCallbacks());
 pService->start(); 
 BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
 pAdvertising->addServiceUUID(SERVICE_UUID); 
 pServer->start();                                                                            // Start the server  Nodig??
 pServer->getAdvertising()->start();                                                          // Start advertising
 TekstSprint("BLE Waiting a client connection to notify ...\n"); 
}
//                                                                                            //
//-----------------------------
// BLE  CheckBLE
//------------------------------
void CheckBLE(void)
{
 if(!deviceConnected && oldDeviceConnected)                                                   // Disconnecting
   {
    delay(300);                                                                               // Give the bluetooth stack the chance to get things ready
    pServer->startAdvertising();                                                              // Restart advertising
    TekstSprint("Start advertising\n");
    oldDeviceConnected = deviceConnected;
   }
 if(deviceConnected && !oldDeviceConnected)                                                   // Connecting
   { 
    oldDeviceConnected = deviceConnected;
    SWversion();
   }
 if(ReceivedMessageBLE.length()>0)
   {
    SendMessageBLE(ReceivedMessageBLE);
    String BLEtext = ReceivedMessageBLE.c_str();
    ReceivedMessageBLE = "";
    ReworkInputString(BLEtext); 
   }
}

//--------------------------------------------                                                //
// WIFI WIFIEvents
//--------------------------------------------
void WiFiEvent(WiFiEvent_t event)
{
  const char* txtstr;
  sprintf(sptext,"[WiFi-event] event: %d", event); 
  Tekstprintln(sptext);
  WiFiEventInfo_t info;
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
            break;
       case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
            Mem.ReconnectWIFI++;
            sprintf(sptext, "Disconnected from station, attempting reconnection for the %d time", Mem.ReconnectWIFI);
            Tekstprintln(sptext);
            sprintf(sptext,"WiFi lost connection."); // Reason: %d",info.wifi_sta_disconnected.reason); 
            Tekstprintln(sptext);
            WiFi.reconnect();
            break;
        case ARDUINO_EVENT_WIFI_STA_AUTHMODE_CHANGE:
            Tekstprintln("Authentication mode of access point has changed");
            break;
        case ARDUINO_EVENT_WIFI_STA_GOT_IP:
            sprintf(sptext, "Obtained IP address: %d.%d.%d.%d", WiFi.localIP()[0], WiFi.localIP()[1], WiFi.localIP()[2], WiFi.localIP()[3] );
            Tekstprintln(sptext);
            WiFiGotIP(event,info);
            break;
        case ARDUINO_EVENT_WIFI_STA_LOST_IP:
            Tekstprintln("Lost IP address and IP address is reset to 0");
            break;
        case ARDUINO_EVENT_WPS_ER_SUCCESS:
            txtstr = WiFi.SSID().c_str();
            sprintf(sptext, "WPS Successfull, stopping WPS and connecting to: %s: ", txtstr);
            Tekstprintln(sptext);
//            wpsStop();
//            delay(10);
//            WiFi.begin();
            break;
        case ARDUINO_EVENT_WPS_ER_FAILED:
            Tekstprintln("WPS Failed, retrying");
//            wpsStop();
//            wpsStart();
            break;
        case ARDUINO_EVENT_WPS_ER_TIMEOUT:
            Tekstprintln("WPS Timedout, retrying");
//            wpsStop();
//            wpsStart();
            break;
        case ARDUINO_EVENT_WPS_ER_PIN:
 //           txtstr = wpspin2string(info.wps_er_pin.pin_code).c_str();
//            sprintf(sptext,"WPS_PIN = %s",txtstr);
//            Tekstprintln(sptext);
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

//          Serial.print("WiFi lost connection. Reason: ");
//          Tekstprintln(info.wifi_sta_disconnected.reason);
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
            Tekstprintln("Ethernet disconnected");
            break;
        case ARDUINO_EVENT_ETH_GOT_IP:
            Tekstprintln("Obtained IP address");
            WiFiGotIP(event,info);
            break;
        default: break;
    }
}
//--------------------------------------------                                                //
// WIFI GOT IP address 
//--------------------------------------------
void WiFiGotIP(WiFiEvent_t event, WiFiEventInfo_t info)
{
  WIFIConnected = 1;
//       Displayprintln("WIFI is On"); 
 if(Mem.WIFIOnOff) WebPage();                                                                 // Show the web page if WIFI is on
 if(Mem.NTPOnOff)
   {
    NTP.setTimeZone(Mem.Timezone);                                                            // TZ_Europe_Amsterdam); //\TZ_Etc_GMTp1); // TZ_Etc_UTC 
    NTP.begin();                                                                              // https://raw.githubusercontent.com/nayarsystems/posix_tz_db/master/zones.csv
    Tekstprintln("NTP is On"); 
    // Displayprintln("NTP is On");                                                           // Print the text on the display
   } 
}

//--------------------------------------------                                                //
// WIFI WEBPAGE 
//--------------------------------------------
void StartWIFI_NTP(void)
{
// WiFi.disconnect(true);
// WiFi.onEvent(WiFiEvent);   // Using WiFi.onEvent interrupts and crashes screen display while writing the screen
// Examples of different ways to register wifi events
                         //  WiFi.onEvent(WiFiGotIP, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_GOT_IP);
                         //  WiFiEventId_t eventID = WiFi.onEvent([](WiFiEvent_t event, WiFiEventInfo_t info)
                         //    {Serial.print("WiFi lost connection. Reason: ");  Serial.println(info.wifi_sta_disconnected.reason); }, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_DISCONNECTED);
// if(strlen (Mem.Ssid) <5 || strlen(Mem.Password) <6) { AskSSIDPW(); return;}

 
 WiFi.mode(WIFI_STA);
 WiFi.begin(Mem.Ssid, Mem.Password);
 if (WiFi.waitForConnectResult() != WL_CONNECTED) 
      { 
       if(Mem.WIFINoOfRestarts == 0)                                                          // Try once to restart the ESP and make a new WIFI connection
       {
          Mem.WIFINoOfRestarts = 1;
          StoreStructInFlashMemory();                                                         // Update Mem struct   
          ESP.restart(); 
       }
       else
       {
         Mem.WIFINoOfRestarts = 0;
         StoreStructInFlashMemory();                                                          // Update Mem struct   
         // Displayprintln("WiFi Failed!");                                                   // Print the text on the display
         Tekstprintln("WiFi Failed! Enter @ to retry"); 
         WIFIConnected = 0;       
         return;
       }
      }
 else 
      {
       Tekstprint("Web page started\n");
       sprintf(sptext, "IP Address: %d.%d.%d.%d", WiFi.localIP()[0], WiFi.localIP()[1], WiFi.localIP()[2], WiFi.localIP()[3] );
       // Displayprintln(sptext);
       Tekstprintln(sptext); 
       WIFIConnected = 1;
       Mem.WIFINoOfRestarts = 0;
       StoreStructInFlashMemory();                                                            // Update Mem struct   
       // Displayprintln("WIFI is On"); 
       // AsyncElegantOTA.begin(&server);                                                     // Start ElegantOTA  old version
       ElegantOTA.begin(&server);                                                             // Start ElegantOTA  new version in 2023  
       // if compile error see here :https://docs.elegantota.pro/async-mode/
                                                                                              // Open ElegantOTA folder and then open src folder
                                                                                              // Locate the ELEGANTOTA_USE_ASYNC_WEBSERVER macro in the ElegantOTA.h file, and set it to 1:
                                                                                              // #define ELEGANTOTA_USE_ASYNC_WEBSERVER 1
                                                                                              // Save the changes to the ElegantOTA.h file.   
       if(Mem.NTPOnOff)
          {
           NTP.setTimeZone(Mem.Timezone);                                                     // TZ_Europe_Amsterdam); //\TZ_Etc_GMTp1); // TZ_Etc_UTC 
           NTP.begin();                                                                       // https://raw.githubusercontent.com/nayarsystems/posix_tz_db/master/zones.csv
           Tekstprintln("NTP is On"); 
           // Displayprintln("NTP is On");                                                    // Print the text on the display
          }
//       PrintIPaddressInScreen();
      }
 if(Mem.WIFIOnOff) WebPage();                                                                 // Show the web page if WIFI is on
}


//--------------------------------------------                                                //
// WIFI WEBPAGE 
//--------------------------------------------
void WebPage(void) 
{
 server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)                                  // Send web page with input fields to client
          { request->send_P(200, "text/html", index_html);  }    );
 server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request)                              // Send a GET request to <ESP_IP>/get?input1=<inputMessage>
       { 
        String inputMessage;    String inputParam;
        if (request->hasParam(PARAM_INPUT_1))                                                 // GET input1 value on <ESP_IP>/get?input1=<inputMessage>
           {
            inputMessage = request->getParam(PARAM_INPUT_1)->value();
            inputParam = PARAM_INPUT_1;
           }
        else 
           {
            inputMessage = "";    //inputMessage = "No message sent";
            inputParam = "none";
           }  
  //  error oplossen       sprintf(sptext,"%s",inputMessage);    Tekstprintln(sptext); //format '%s' expects argument of type 'char*', but argument 3 has type 'String' [-Werror=format=]

        ReworkInputString(inputMessage);
        request->send_P(200, "text/html", index_html);
       }   );
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





//                                                                                            //
