# Program explanation

The program uses the following standard libraries.

```cpp
//------------------------------------------------------------------------------              //
// ESP32 Includes defines and initialisations
//-------------------------------------------
                      #if ESP_ARDUINO_VERSION >= ESP_ARDUINO_VERSION_VAL (5, 2, 0)
#define USEEDSOFTLED  // EdsoftLED >= V1.7.0 for SK6812. Saves 200-500 bytes vs NeoPixel
                      #endif
                      #ifdef USEEDSOFTLED
#include <EdSoftLED.h>         // https://github.com/ednieuw/EdSoftLED for WS2812 or SK6812
                      #else
#include <Adafruit_NeoPixel.h> // https://github.com/adafruit/Adafruit_NeoPixel (legacy, GPIO numbering)
                      #endif
#include <Preferences.h>       // Permanent storage of the Mem struct
#include <NimBLEDevice.h>      // BLE communication. Use NimBLE 2.x.x  https://github.com/h2zero/NimBLE-Arduino
#include <WiFi.h>              // Web page
#include <esp_sntp.h>          // NTP
#include <esp_wps.h>           // WPS
#include <ESPAsyncWebServer.h> // Web page  https://github.com/ESP32Async
#include <Update.h>            // Over-the-Air (OTA)
#include <ESPmDNS.h>           // Show BLE name in router
#include <DNSServer.h>         // Captive portal for entering SSID/password
#include <Wire.h>              // I2C
#include <RTClib.h>            // DS3231 RTC  https://adafruit.github.io/RTClib/html/class_r_t_c___d_s3231.html
#include <Encoder.h>           // Rotary encoder
#include <Keypad.h>            // 3x1 membrane keypad (Mark Stanley & Alexander Brevig)
#include <IRremote.hpp>        // IR remote control
```

The HTML pages and clock logic are split over separate TABs in the IDE and included as headers:

```cpp
#include "Webpage.h"    // Main clock web page (with live log window + menu link)
#include "SoftAP.h"     // Access-Point page to enter SSID and password
#include "OTAhtml.h"    // OTA firmware update page (/update)
#include "LogViewer.h"  // Full log viewer page (/log)
#include "ColourPick.h" // Colour picker page (/colourpick)
#include "MenuPage.h"   // Touch-friendly menu page (/menu)
#include "ClockLogic.h" // Clock display logic and language functions
```

`Webpage.h`, `MenuPage.h` etc. are included as TABs in the IDE. They contain the web pages shown in the browser. I made the original page in the free *Microsoft Expression Web 4* — no longer maintained but more than enough for this purpose; Notepad++ or an AI assistant are good alternatives. To copy code out of MS-Expression: click **Split** at the bottom of the window — the upper half shows the raw HTML, the lower half the design view. Copy the HTML and paste it in the relevant `.h` TAB between `R"rawliteral(` … `)rawliteral";`.

A long list of definitions and initialisations follows. I am not a fan of passing all variables to and from functions and prefer to keep them global in one program listing. When writing with other people it is good practice to avoid too many globals, but this program is one large listing, kept simple on purpose. I grouped the variables per application so a simple *find* locates them quickly. To print the time as coloured text/LEDs, each word and its position in the LED string is defined as a macro that calls `ColorLeds` with the right parameters. The functions `Dutch()`, `English()` etc. show why these macros are so handy. They live in the TAB **`ClockFaces.h`**.

```cpp
#define PRECIES ColorLeds("precies", 16, 22, LetterColor);
#define MTIEN   ColorLeds("tien", 25, 28, LetterColor);
#define KWART   ColorLeds("kwart", 32, 36, LetterColor);
#define VOOR    ColorLeds("voor", 38, 41, LetterColor);
```

This is the storage area for the `EEPROMstorage` struct. The whole struct is saved in one command to permanent memory via `Preferences FLASHSTOR;`

```cpp
struct EEPROMstorage {           // Kept after power loss
  byte DisplayChoice = 0;
  ...
  char BLEbroadcastName[30];     // Name of the BLE beacon
  char Timezone[50];
  int  Checksum = 0;
} Mem;
```

Variables are addressed with the short name `Mem`, e.g. `Mem.DisplayChoice = 3;`. The struct is stored with `StoreStructInFlashMemory()` and retrieved with `GetStructFromFlashMemory()`.

The menu shown in the serial monitor, BLE app and web page is defined here. On the web page the line with **II long menu** also carries a hyperlink to `/menu`.

```cpp
// Menu
//0        1         2         3         4
//1234567890123456789012345678901234567890
const char *menu[] = {
 "A SSID, B Password, C BLE beacon name",
 "D Date (D15012021), T Time (T132145)",
 ...
 "I Info menu, II long menu <a href=/menu>&#9776; Menu page</a>",
 ...
 "Ed Nieuwenhuijs Jun 2026" };
```

The Setup happens here:

```cpp
//--------------------------------------------
// ARDUINO Setup
//--------------------------------------------
void setup()
{
 Serial.begin(115200); Tekstprintln("Serial started");
 ...
 InitLogBuffer();   // Allocate the circular log buffer (PSRAM if available)
 ...
 msTick = millis();
}
```

The loop is kept almost empty; the program runs from `EverySecondCheck`. Nothing here needs to run every millisecond.

```cpp
//--------------------------------------------
// ARDUINO Loop
//--------------------------------------------
void loop()
{
 Loopcounter++;
 if (Demo)            Demomode();
 else if(FireDisplay) FireMode();
 else                 EverySecondCheck();   // Let the second LED tick and run the clock
 CheckDevices();
}
```

The following routines check what must happen every second, minute, hour and day. This keeps the processor ~99% free. The loops-per-second figure is printed in the serial monitor (with `TestLDR` on) and is handy to spot if the program is spending too much time somewhere — currently around 200,000 l/s.

```cpp
//--------------------------------------------                                                //
// COMMON Check connected input devices
//--------------------------------------------
void CheckDevices(void)
{
 CheckBLE();                                  // BLE input?
 SerialCheck();                               // Serial input?
 CheckWIFIcommand();                          // Command from the web page?
 if (Mem.TimeInput==1) RotaryEncoderCheck();
 if (Mem.TimeInput==2) Keypad3x1Check();
 if (Mem.TimeInput==3 || Mem.TimeInput==4) IrReceiverDecode();
 if (ntpJustSynced) CheckandPrintNTPsynced();
                                  #ifdef ONEWIREKEYPAD3x4
 OnewireKeypad3x4Check();
                                  #endif
                                  #ifdef ONEWIREKEYPAD3x1
 OnewireKeypad3x1Check();
                                  #endif
}
```

`EverySecondCheck()` also services the Time Receiver client, the LDR dimming and the OTA reboot, and calls `EveryMinuteUpdate()` → `EveryHourUpdate()` → `EveryDayUpdate()` in cascade.

```cpp
void EverySecondCheck(void)
{
 static bool Toggle = 0;
 uint32_t msLeap = millis() - msTick;
 if (msLeap > 999)                                       // Every second
  {
   ...
   if (Mem.TimeReceiver) CheckTimeReceiverClient();
   DimLeds(TestLDR);
   if (shouldReboot) { delay(2000); ESP.restart(); }     // After OTA update
   if (timeinfo.tm_min != lastminute) EveryMinuteUpdate();
   Loopcounter=0;
  }
}
```

## Status LEDs

```cpp
//--------------------------------------------                                                //
// COMMON Update routine for the status LEDs
//--------------------------------------------
void UpdateStatusLEDs(bool Toggle)
{
 if(Mem.StatusLEDOn)
   {
    SetStatusLED((Toggle && WiFi.localIP()[0]==0) * 20,
                 (Toggle && WiFi.localIP()[0]!=0) * 20,
                 (Toggle &&      deviceConnected) * 20);
    SetPCBLED09( Toggle * 10);                    // Left LED
    SetPCBLED10((!Toggle) * 10);                  // Right LED
    SetNanoLED13((!Toggle) * (!Mem.UseDS3231 * 50));
   }
 else { SetStatusLED(0,0,0); SetPCBLED09(0); SetPCBLED10(0); SetNanoLED13(0); }
}
```

On the Nano ESP32 `analogWrite(LED_RED, 512)` is OFF and `analogWrite(LED_RED, 0)` is ON. With Espressif ESP32 core 3.x the RMT resolution rose from 8→10 bits, so the off value is `8191` to make sure the LED turns off completely:

```cpp
analogWrite(LED_RED, Red == 0 ? 8191 : (512 - Red));
```

## Common print routines

All output passes through one place, so it is easy to retarget for other boards. Note that `Tekstprint` now also feeds the **ring-buffer log** via `AddLog`:

```cpp
void Tekstprint(char const *tekst)   { if(Serial) Serial.print(tekst); SendMessageTimeReceiver(tekst); SendMessageBLE(tekst); AddLog(tekst); }
void Tekstprintln(char const *tekst) { snprintf(sptext, sizeof(sptext),"%s\n",tekst); Tekstprint(sptext); }
void Tekstprintf(const char* fmt, ...)   { va_list a; va_start(a,fmt); vsnprintf(sptext,sizeof(sptext),fmt,a); va_end(a); Tekstprint(sptext); }
void Tekstprintlnf(const char* fmt, ...) { va_list a; va_start(a,fmt); vsnprintf(sptext,sizeof(sptext),fmt,a); va_end(a); Tekstprintln(sptext); }
```

For the web page there is a parallel set, `WTekstprint` / `WTekstprintln`, that builds the HTML info block. They take an optional prefix/suffix so a line can carry web-only HTML (e.g. the `/update` link) without cluttering the serial/BLE output.

## Serial input

Check for serial input from the serial monitor and pass the command to `ReworkInputString()`.

```cpp
//--------------------------------------------
// COMMON Check for serial input
//--------------------------------------------
void SerialCheck(void)
{
 ...
 ReworkInputString(SerialString);   // Process the input string in the menu
 ...
}
```

## Reset to defaults

Restore all default values. Triggered by the `R` command or when the stored checksum is invalid.

```cpp
//--------------------------------------------
// COMMON Reset to default settings
//--------------------------------------------
void Reset(void)
{
 Mem.Checksum      = 25065;
 Mem.DisplayChoice = DEFAULTCOLOUR;
 Mem.OwnColour     = green;
 Mem.DimmedLetter  = black;
 Mem.BackGround    = black;
 ...
 StoreStructInFlashMemory();
 GetTijd(false);
 SWversion();
 Displaytime(); Tekstprintln("");
}
```

## Storage

`InitStorage()` sets up the storage space and checks the validity of the settings; an invalid checksum triggers a `Reset()` to the defaults. The whole `Mem` struct is then saved or loaded in one command via `Preferences`.

```cpp
//--------------------------------------------
// COMMON Init and check contents of storage
//--------------------------------------------
void InitStorage(void) { ... }

//--------------------------------------------
// COMMON Store Mem struct in flash storage
//--------------------------------------------
void StoreStructInFlashMemory(void) { ... }

//--------------------------------------------
// COMMON Get data from flash storage (Preferences.h)
//--------------------------------------------
void GetStructFromFlashMemory(void) { ... }
```

## Version info and menu

`SWversion()` prints the menu and the current settings; `PrintLine()` draws the horizontal divider lines. `SWversion(bool Small)` selects the short or long menu, and the print loop adds the web-only `/menu` hyperlink to the matching line.

```cpp
//--------------------------------------------
// COMMON Version info
//--------------------------------------------
void SWversion(void)        { SWversion(false); }   // Long menu by default
void SWversion(bool Small)
{
 #define FILENAAM (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
 PrintLine(35);
 if(Small) for (uint8_t i = 0; i < sizeof(menusmall)/sizeof(menusmall[0]); i++) ...
 else      for (uint8_t i = 0; i < sizeof(menu)/sizeof(menu[0]); i++) ...
 ...
 PrintLine(35);
}

void PrintLine(byte Lengte) { ... }
```

## Ring-buffer logging and the log viewer  *(new in V227)*

All text sent through `Tekstprint` is also appended to a **circular log buffer** in PSRAM (falling back to internal RAM). This lets the clock keep a rolling history that survives reconnects and can be viewed in the browser.

```cpp
void InitLogBuffer(void)                 // Allocate the buffer at startup
void AddLog(const char* msg)             // Append to the circular buffer (wraps at the end)
void HandleTekstPrint(...)               // Serve the last ~500 lines  -> /tekstprint
void HandleTekstDownload(...)            // Stream the full buffer      -> /tekstdownload
```



## Get the commands

Input from serial, BLE or the web page is processed in `ReworkInputString()`. The menu letters are case-insensitive (upper/lower conversion) so upper- and lowercase can carry the same commands.

```cpp
//--------------------------------------------
// COMMON Input from Bluetooth, Serial or web
//--------------------------------------------
void ReworkInputString(String InputString)
{
  int len = InputString.length();
  switch(toupper(InputString[0]))
   {
    case 'A':
      if(len > 4 && len < 30) ...
```

## LDR reading

The LDR (analog 0–4096 on the ESP32) is scaled to 0–255:

```cpp
int ReadLDR(void) { return analogRead(PhotoCellPin)/16; }
```

`DimLeds()` reads the LDR and **squares** the value to get a hyperbolic curve that matches the eye's response to dark/light far better than a linear range — it works wonderfully well.

## LED helpers and the interruptible LED test 

A series of LED helpers (`ColorLeds`, `ColorLed`, `LedsOff`, `ShowLeds`, `SetBrightnessLeds`, `Stripfill`, `StartLeds`, …) wrap every strip-specific command so the rest of the code stays generic. The SK6812 strip also has a white LED, controlled by the highest byte of the 32-bit colour value (`0xWWRRGGBB`).


```cpp
//--------------------------------------------
// LED Set color for a range of LEDs and print label
//--------------------------------------------
void ColorLeds(char const *Tekst, int FirstLed, int LastLed, uint32_t RGBWColor) { ... }
```

Every display/strip uses different commands to set brightness, so each LED operation has its own wrapper function:

```cpp
//--------------------------------------------
// LED Set brightness of LED strip
//--------------------------------------------
void SetBrightnessLeds(byte Bright) { LEDstrip.setBrightness(Bright); ShowLeds(); }
```

A place to turn off all LEDs or clear the display:

```cpp
//--------------------------------------------
// LED Clear the LED display
//--------------------------------------------
void LedsOff(void) { LedsOff(false); }
void LedsOff(bool ClearOff) { ... }
```

## Colours of the words

`SetSecondColour()` sets the colours of the characters and is also used to recolour `HET IS WAS` in some display choices (`ClockLogic.h`).

```cpp
//--------------------------------------------
// LED Set second color
//--------------------------------------------
void SetSecondColour(void)
{
 ...
 MINColor = FuncCRGBW(15 + timeinfo.tm_min * 4, 255 - timeinfo.tm_min * 4, 0, 0);
 SECColor = FuncCRGBW(15 + timeinfo.tm_sec * 4, 255 - timeinfo.tm_sec * 4, 0, 0);
 switch (Mem.DisplayChoice)
   {
    case DEFAULTCOLOUR: LetterColor = C_YELLOW;
    ...
   }
}
```

## Self test

Self-test sequence of the clock; default delay is 900 ms per word (`ClockLogic.h`).

```cpp
//--------------------------------------------
// CLOCK Self test sequence
//--------------------------------------------
void Selftest(void) { Selftest(900); }
void Selftest(int Delayms)
{
 GetTijd(true);     // Print time in the serial monitor
 LedsOff();
 ...
}
```

## Display the time

`Displaytime()` prints the time as text to the serial monitor, selects which language is shown, and sends the right colour/intensity sequence to the RGB(W) LED strip (`ClockLogic.h`).

```cpp
//--------------------------------------------
// CLOCK Say the time and load the LEDs
//--------------------------------------------
void Displaytime(void)
{
 LedsOff();
 if( Mem.DisplayChoice == DIGITAL ) { TimePlaceDigit(timeinfo.tm_hour,timeinfo.tm_min); }
 else
   {
                     #ifdef NL144CLOCK
    Dutch();
                     #endif
                     ...
                     #ifdef FOURLANGUAGECLOCK
    SetColours();          // Set the colours for the chosen palette
    Dutch(); English(); German(); French();
                     #endif
   }
 ShowLeds();               // Turn on the LEDs
}
```

## LED strip start-up

A list of LED helper functions follows, including initialisation of the WS2812 or SK6812 strip. The SK6812 also has a white LED whose intensity sits in the highest byte (`WW`) of the 32-bit value `0xWWRRGGBB`.

```cpp
//--------------------------------------------                                                //
// LED Start LEDs
//--------------------------------------------
void StartLeds(void)
{
 switch (Mem.LEDstrip)
   {
    case 0: LEDstrip = LED6812strip;          // SK6812 with white
            white = 0xFF000000; lgray = 0x66000000; gray = 0x33000000; ...
            break;
    case 1: LEDstrip = LED2812strip;          // WS2812 RGB
            white = 0xFFFFFF;   lgray = 0x666666; ...
            break;
   }
}
```

## Time functions

A series of functions get and store time. The NTP server places the retrieved time in the standard C time structures.

```cpp
//--------------------------- Time functions --------------------------
time_t      GetTijd(bool printit)         // Get time from the best available source
DateTime    GetLocalDateTime(void)        // Local time as an RTClib DateTime
bool        GetNTPtime(bool printit)      // Fetch NTP time for the configured timezone
const char* PrintRTCTime(void)            // Time string dd-mmm-yy hh:mm:ss
void        PrintTimeHMS(void)            // Print hh:mm:ss with linefeed
void        SetRTCTime(void)              // Set internal RTC from timeinfo
void        PrintAllClockTimes(void)      // Print all available time sources
void        InitTimeSystem(void)          // Initialise time system (DS3231 + NTP + internal RTC).
void SetSystemTimeToCompileTime(void)     // Set system time to compile time.
bool        IsDS3231I2Cconnected(void)    // DS3231 Check for I2C connection.
float       GetDS3231Temp(void)           // DS3231 Read temperature from DS3231.
void        SetDS3231Time(void)           // DS3231 Write ESP32 time to DS3231.
void        GetDS3231Time(bool printit)   // DS3231 Read time from DS3231 module.
void        SetSystemTime(time_t t)       // RTC Set RTC using Unix timestamp.
void        PrintAllClockTimes(void)      // Print all the clock times available 
```

Convert a HEX string to an unsigned 32-bit integer:

```cpp
//--------------------------------------------
// COMMON Convert Hex to uint32
//--------------------------------------------
uint32_t HexToDec(const String& hexString) { ... }
```

## Speaking the time in four languages

Functions let the clocks speak the time in four languages. Languages also carry a lot of slang — *"half nine"* can mean 8:30 or 9:30. These definitions live in `ClockFaces.h` / `ClockLogic.h`.

```cpp
//--------------------------------------------
// CLOCK Dutch clock display
//--------------------------------------------
                      #if defined(NL) || defined(NL144CLOCK)
void Dutch(void)
{
 HET;                                                     // HET is always on
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
    ...
   }
}
```

## Bluetooth Low Energy

The BLE functions use the Nordic nRF (NimBLE) stack, which differs from the Texas Instruments CC2540/CC2541 used in chipsets like the HM-10, HM-16, JDY-08 etc. ([More on Instructables](https://www.instructables.com/Communicate-Using-CC254x-or-NRF-BLE-With-Arduino-a/))

```cpp
void SendMessageBLE(std::string Message)   // BLE Send message in packets of 20 chars
class MyServerCallbacks : public NimBLEServerCallbacks   // BLE connect/disconnect callbacks
void StartBLEService(void)                 // BLE Start the UART service
void DisconnectBLE(void)                   // BLE Disconnect BLE Service
void CheckBLE(void)                        // BLE Check input and process the string
```
```cpp
//--------------------------- BLE TimeReceiver functions --------------------------
void        TimeReceiverNotifyCB(...)              // Notify callback: copies incoming BLE data into a buffer and passes it to ReworkInputString(); resets the connection-alive timestamp
void        SendMessageTimeReceiver(const char*)   // Writes a message to the remote RX characteristic (no-op if not connected)
void        StartTimeReceiverScan(void)            // Starts a continuous BLE scan using the static scan callback; guards against double-starts and resets stale flags
void        StopTimeReceiver(void)                 // Stops the scan, disconnects and deletes the NimBLE client, and resets all TR state flags
static void Connect_TimeReceiver(void)             // Attempts up to 3 times to connect to the found address, discover the service/characteristics, and subscribe to TX notifications; falls back to a fresh scan on total failure
void        ReconnectTimeReceiver(void)            // Tears down any existing client, reuses the previously found address, and immediately calls Connect_TimeReceiver()
void        CheckTimeReceiverClient(void)          // Main loop-context driver: processes deferred disconnect cleanup, honours scan-callback connect requests, times out stalled connection attempts, and triggers periodic rescans when idle
```
These functions start a connection to a time sender that will set time in this sketch  when the BLE connection is succesfull.
Several time senders are available; from IOS app to web pages or ESP32 sketches.
Just open the application, wait a minute or so and you will see the clock sets time correctly.

- [iPhone BLE TimeSender app (search for Timesender in the Apple app store)](https://ednieuw.nl/BLESerial/BLEtimeSender.html)
- [Arduino Nano ESP32 sketch that Auto connects to BLE-UARTtime BLE client and sends time and date](https://github.com/ednieuw/BLE-time-Sender)
- [HTML page that connects to a BLE UART client and send time and date to it](https://github.com/ednieuw/HTML-TimeSender)
- [web page Windows/IOS/Android.](https://github.com/ednieuw/HTML-BLEserial)
- [BLE serial monitor app on your phone or with a web page Windows/Android](https://github.com/ednieuw/HTML-BLEserial)



## WIFI and the web page

Functions to start a WIFI connection and serve the web pages.

 `WebPage()` registers all the routes listed in the table below.

| Route | Page |
|-------|------|
| `/`             | Main clock page (Webpage.h) |
| `/menu`         | Touch menu page (MenuPage.h) |
| `/log`          | Full log viewer (LogViewer.h) |
| `/tekstprint`   | Last ~500 log lines (used by the live log windows) |
| `/tekstdownload`| Full log buffer download |
| `/colourpick`   | Colour picker (ColourPick.h) |
| `/update`       | OTA firmware update (OTAhtml.h) |

The main page and the menu page each embed a small **live log window** — a read-only text area that fetches `/tekstprint` every second, shows the last 50 lines and auto-scrolls to the bottom *only* when you are already at the bottom (so you can scroll up to read history without being yanked back). A checkbox on the main page toggles the window on/off.

```cpp

void WiFiEvent(WiFiEvent_t event)              // WIFI Handle WIFI events.
bool CheckforWIFINetwork(void)                 // WIFI Scan for configured WIFI network (print=true).
bool CheckforWIFINetwork(bool PrintIt)         // WIFI Scan for configured WIFI network, optionally silent.
void ScanWIFI(void)                            // WIFI Scan and print list of available WIFI networks.
void ConnectWIFI(void)                         // WIFI Connect to WIFI router using stored SSID and password.
void CheckRestoreWIFIconnectivity(void)        // WIFI Check WIFI connection and restore if lost.
bool StartWIFI_NTP(void)                       // WIFI Start WIFI connection and NTP service.
void NTPnotify(struct timeval* tv)             // NTP Callback: set ntpJustSynced flag on NTP sync.
void CheckandPrintNTPsynced(void)              // NTP Check and print NTP sync confirmation.
void setTimezone(void)                         // NTP Apply stored timezone to system.
void initSNTP(void)                            // NTP Initialise SNTP client.
bool wait4SNTP(void)                           // NTP Wait up to 2.5 s for SNTP synchronisation.
void CheckWIFIcommand(void)                    // WIFI Process pending command received from web page.
void WebPage(void)                             // WIFI Register all web server routes and start server.
void notFound(AsyncWebServerRequest *request)  // WIFI Handle unknown web page requests.
void StartAPMode(void)                         // WIFI Start Access Point for entering WIFI credentials.
void wpsInitConfig(void)                       // WIFI Initialise WPS configuration.
void wpsStart(void)                            // WIFI Start WPS push-button connection.
void wpsStop(void)                             // WIFI Stop WPS.
String wpspin2string(uint8_t a[])              // WIFI Convert WPS PIN byte array to string.

```
