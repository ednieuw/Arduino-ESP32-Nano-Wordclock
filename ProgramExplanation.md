# Program explanation WordFiboClock V247

This document describes how the sketch is built up. It is meant for anyone who wants to understand, modify or extend the software, for example with a clock variant of their own or an extra menu command. For the use and operation of the clock: see the [Manual](Manual.md).

[Nederlands](Programmauitleg.md) · [README](README.md) · [Manual](Manual.md)

## Contents

1. [Overview](#1-overview)
2. [Files and clock variants](#2-files-and-clock-variants)
3. [Libraries](#3-libraries)
4. [Global variables and permanent storage](#4-global-variables-and-permanent-storage)
5. [Starting up: `setup()`](#5-starting-up-setup)
6. [The main program: `loop()` and the time steps](#6-the-main-program-loop-and-the-time-steps)
7. [Output: the `Tekstprint` functions and the log](#7-output-the-tekstprint-functions-and-the-log)
8. [Input: all commands through `ReworkInputString()`](#8-input-all-commands-through-reworkinputstring)
9. [Time](#9-time)
10. [LEDs and brightness](#10-leds-and-brightness)
11. [The clock face](#11-the-clock-face)
12. [Bluetooth, time receiver and time sender](#12-bluetooth-time-receiver-and-time-sender)
13. [WIFI, web pages and OTA](#13-wifi-web-pages-and-ota)
14. [MQTT and Home Assistant](#14-mqtt-and-home-assistant)
15. [Monitoring and recovery](#15-monitoring-and-recovery)
16. [Modifying and extending](#16-modifying-and-extending)
17. [Function overview](#17-function-overview)

---

## 1. Overview

The sketch is one large program with, on purpose, as many **global variables** as possible. Variables are grouped per part, so that *find* in the IDE quickly locates everything. Functions take few parameters. That is not a recommendation for large projects with several developers, but it keeps this program easy to read.

All functions are in the same order in the sketch, grouped by a comment line such as `// COMMON`, `// LED`, `// WIFI`, `// BLE`, `// MQTT`, `// CLOCK` in front of the function name. In this way it is visible whether a function applies to all clock variants (`COMMON`) or to one part.

The program is **cooperative**: nowhere is there a long wait. `loop()` hardly does anything itself and calls routines that do something every second, minute, hour and day. As a result the processor is mostly free: with `K` (LDR read-out) the number of loops per second is shown, about 200 kl/s (200,000 per second).

## 2. Files and clock variants

| File | Contents |
|---|---|
| `WordFiboClockV247.ino` | Everything that applies to all variants: start-up, menu, WIFI, BLE, MQTT, time, LED helper functions, storage |
| `WordClock.h` | Display of the word clock (`Displaytime()`, the language functions `Dutch()`, `English()`...), colour picker and menu page |
| `ClockFaces.h` | LED positions of every word, per clock variant |
| `Fibonacci.h` | Display of the Fibonacci clock, colour picker and menu page |
| `NoClock.h` | Empty functions for the variant without a display |
| `Webpage.h`, `SoftAP.h`, `OTAhtml.h`, `LogViewer.h` | HTML pages of the clock |

### Choosing the variant

At the top of the sketch one variant is chosen with `#define`. That determines which `.h` file is included, at the end of the definitions (below the menu table):

```cpp
#if   defined(WORDCLOCK)
  #include "WordClock.h"      // Word clock: Displaytime(), ColourPick_html[], menu_html[]
#elif defined(FIBONACCI)
  #include "Fibonacci.h"      // Fibonacci
#elif defined(NOCLOCK)
  #include "NoClock.h"        // No display
#endif
```

`WORDCLOCK` is defined automatically as soon as neither `FIBONACCI` nor `NOCLOCK` is chosen.

Every variant file supplies the same parts, so that the main sketch does not need to know anything about them:

| Part | Purpose |
|---|---|
| `Displaytime()` | Puts the time on the LEDs |
| `QchoiceNames[]` and `LASTITEM` | Names of the colour schemes for the command `Q` (and for Home Assistant) |
| `CLOCK_VARIANT` | Name of the variant in the log |
| `NUM_LEDS`, `MATRIX_WIDTH`, `MATRIX_HEIGHT`, `LEDNEARLDR` | Dimensions and LED positions |
| `ColourPick_html[]`, `menu_html[]` | Pages `/colourpick` and `/menu` |
| `PrintDisplayChoice()`, `SetSecondColour()`, `Demomode()`, `FireMode()`, `Selftest()`, `RotPressOne/Two/Three()` | Helper functions. Where a variant does not need them, they are empty functions |

This is the reason why commands such as `$` (fireplace) or `#` (self test) do nothing on the Fibonacci clock.

## 3. Libraries

```cpp
#include <EdSoftLED.h>          // LED strip (core 3.3+); otherwise <Adafruit_NeoPixel.h>
#include <Preferences.h>        // Permanent storage
#include <NimBLEDevice.h>       // Bluetooth Low Energy (NimBLE 2.x)
#include <WiFi.h>               // WIFI
#include <esp_sntp.h>           // NTP
#include <esp_wps.h>            // WPS
#include <ESPAsyncWebServer.h>  // Web pages
#include <Update.h>             // OTA
#include <ESPmDNS.h>            // name.local
#include <DNSServer.h>          // Access point page
#include <Wire.h>               // I2C
#include <RTClib.h>             // DS3231
#include <Encoder.h>            // Rotary encoder
#include <Keypad.h>             // Three push buttons
#include <IRremote.hpp>         // IR remote control
#include <PubSubClient.h>       // MQTT
```

The LED library is chosen automatically: with ESP32 core 3.3.0 or higher `USEEDSOFTLED` (EdSoftLED), otherwise NeoPixel. See [chapter 4 of the manual](Manual.md#4-installing-the-software) for the corresponding pin numbering.

## 4. Global variables and permanent storage

### The struct `Mem`

All settings are in one struct, `EEPROMstorage`, with the variable `Mem`:

```cpp
struct EEPROMstorage {
  byte DisplayChoice = 0;
  byte TurnOffLEDsAtHH = 0;
  ...
  char SSID[30];
  char Password[40];
  char BLEbroadcastName[30];
  char Timezone[50];
  int  Checksum = 0;
  ...
} Mem;
```

The fields are addressed with `Mem.<name>`, for example `Mem.DisplayChoice = 3;`. The whole struct is stored and read back in one go with the library `Preferences`:

| Function | Task |
|---|---|
| `InitStorage()` | Reads the struct and checks the values. If `Mem.Checksum` is not 25065, `Reset()` follows |
| `StoreStructInFlashMemory()` | Stores the struct. Happens after every command and every day |
| `GetStructFromFlashMemory()` | Reads the struct from memory |
| `Reset()` | Sets all values to the default (except WIFI data, BLE name and time zone) |
| `ResetCredentials()` | Erases SSID, password, BLE name and time zone |
| `StoreIRRemoteInFlashMemory()`, `GetIRRemoteFromFlashMemory()` | The learned IR keys (separate storage) |

The struct contains spare fields (`byteFuture…`, `IntFuture…`, `Extra…`, `ExtraSpace`), so that a new setting can be added without changing the layout.

> If the layout of the struct is changed, the storage in existing clocks no longer matches and a reset to the default values follows at the first start. So preferably put a new field in a spare field.

### Adding a new setting

1. Add the field in the struct (or use a spare field).
2. Put the default value in `Reset()`.
3. Add it to `PrintMem()` so that it is visible with `=`.
4. Write the command in `ReworkInputString()` (see [16](#16-modifying-and-extending)).

### The menu table

The menu that is shown in the serial monitor, the Bluetooth app and on the web page is held in two tables, `menu[]` (long) and `menusmall[]` (short). The lines are at most 40 characters long. `SWversion()` prints the menu and the current settings, `PrintLine()` draws the divider lines.

## 5. Starting up: `setup()`

`setup()` starts the parts in this order. Each step shows its own colour on the LED strip, so that it is visible where the start-up hangs (`LEDstartup()`):

1. Serial port (115200 baud) and I2C.
2. `InitStorage()`: read and check the storage, ring buffer for the log (`InitLogBuffer()`).
3. `StartLeds()` and `InitStatusLEDs()`.
4. Waiting for the serial connection (at most 3 seconds).
5. `++Mem.MCUrestarted`: restarting too often leads to a reset (see [15](#15-monitoring-and-recovery)).
6. Input device: rotary (`TimeInput` 1), push buttons (2) or IR (3 and 4).
7. BLE (`StartBLEService()`), WIFI (`ConnectWIFI()`), time system (`InitTimeSystem()`).
8. Time receiver (`H05`), BLE time sender (`H06`), HC-12 (`H07`), reset button (`^nn`).
9. First LDR measurement, first display of the time and the menu.

## 6. The main program: `loop()` and the time steps

```cpp
void loop()
{
 Loopcounter++;
 if (Demo)            Demomode();
 else if(FireDisplay) FireMode();
 else                 EverySecondCheck();   // let the second tick and run the clock
 CheckDevices();
}
```

`CheckDevices()` checks whether there is input, every time `loop()` runs:

```cpp
void CheckDevices(void)
{
 CheckBLE();                 // Bluetooth input?
 SerialCheck();              // Serial input?
 CheckWIFIcommand();         // Command from the web page?
 if(Mem.MQTT) CheckMQTT();   // Maintain MQTT
 if (Mem.TimeInput==1) RotaryEncoderCheck();
 if (Mem.TimeInput==2) Keypad3x1Check();
 if (Mem.TimeInput==3 || Mem.TimeInput==4) IrReceiverDecode();
 if (ntpJustSynced) CheckandPrintNTPsynced();
}
```

`EverySecondCheck()` does the following work once per second (when `millis() - msTick > 999`):

- reading the time with `GetTijd()`, updating the status LEDs, colour of HET IS WAS per second (`SetSecondColour()`);
- checking the reset button, maintaining the time receiver and time sender client (`CheckTimeReceiverClient()`, `CheckTimeSender()`);
- `DimLeds()`: brightness based on the LDR;
- after an OTA update or restart command: setting MQTT to *offline* and restarting with `ESP.restart()`;
- if the minute has changed: `EveryMinuteUpdate()`.

The time steps run in a cascade:

| Routine | When | Task |
|---|---|---|
| `EveryMinuteUpdate()` | Every minute | Update the display, time log (`K1`), HC-12, random colour scheme (`~1`), `PublishMQTTStates()` |
| `EveryHourUpdate()` | Every hour | Display on/off according to `N`, `CheckRestoreWIFIconnectivity()`, average LDR per hour, `~2` |
| `EveryDayUpdate()` | Every day | Reset LDR minimum and maximum, print the time, `StoreStructInFlashMemory()` |

## 7. Output: the `Tekstprint` functions and the log

All output goes through one place, so that it can be redirected:

```cpp
void Tekstprint(char const *tekst)
{
 if(Serial) Serial.print(tekst);
 SendMessageTimeReceiver(tekst);   // to the time sender (BLE)
 SendMessageBLE(tekst);            // to the Bluetooth app
 AddLog(tekst);                    // into the ring buffer
}
```

`Tekstprintln`, `Tekstprintf` and `Tekstprintlnf` are variants of this with a line ending and `printf` formatting. A second series, `WTekstprint…`, builds the HTML block for the web page, with an optional prefix and suffix so that a line can contain a link only on the web page (for example `/update`).

### The log (ring buffer)

`InitLogBuffer()` reserves a large block of memory at start-up, preferably in PSRAM. `AddLog()` writes the text cyclically into that block; at the end it starts again at the beginning. Two routes show the log:

| Route | Function | Contents |
|---|---|---|
| `/tekstprint` | `HandleTekstPrint()` | The last roughly 500 lines |
| `/tekstdownload` | `HandleTekstDownload()` | The whole buffer as a file `<name>-log.txt` |

With `}` (`Mem.Ringbufcnt`) the position in the buffer is printed behind every line.

## 8. Input: all commands through `ReworkInputString()`

All input sources come together in one function, `ReworkInputString(String InputString)`:

| Source | Where |
|---|---|
| Serial monitor | `SerialCheck()` |
| Bluetooth | `CheckBLE()` |
| Web page | `CheckWIFIcommand()` (through `PendingCommand`) |
| MQTT | `MQTTCallback()` |
| Time sender (BLE) | `TimeReceiverNotifyCB()` |

The function

- ignores input longer than 40 characters;
- looks at the first character (`toupper(InputString[0])`) and chooses the command with a `switch`;
- puts the result in `sptext`, which is shown at the end with `Tekstprintln`;
- stores `Mem` with `StoreStructInFlashMemory()` and forces a new display (`lastminute = 99`).

As a result every command works the same everywhere: a button in Home Assistant, a tap on the web page and a letter in the Bluetooth app all call the same code.

Helper functions for the input: `SConstrainInt()` (number from a string with limits), `ParseColourInput()` (colours in hex and digits, for `F` and `V`), `HexToDec()`.

## 9. Time

`GetTijd()` gets the time from the best available source and puts it in the standard C structure `timeinfo`.

| Source | Functions |
|---|---|
| NTP through WIFI | `StartWIFI_NTP()`, `initSNTP()`, `setTimezone()`, `wait4SNTP()`, `NTPnotify()`, `GetNTPtime()` |
| DS3231 (I2C) | `IsDS3231I2Cconnected()`, `GetDS3231Time()`, `SetDS3231Time()`, `GetDS3231Temp()` |
| Internal RTC of the ESP32 | `SetRTCTime()`, `SetSystemTime()` |
| Manual | commands `D`, `T` and `hhmmss` |
| Time receiver | commands `T` and `D` through BLE |

With `J` (`Mem.UseDS3231`) and `X` (`Mem.NTPOn`) the source is chosen. `InitTimeSystem()` starts the sources. `PrintAllClockTimes()` (command `!`) shows all times side by side. Summer time comes from the time zone string (POSIX format) and only applies to NTP, not to the DS3231.

## 10. LEDs and brightness

### The LED strip

All calls for the strip are in helper functions, so that the rest of the code is independent of the library (EdSoftLED or NeoPixel):

| Function | Task |
|---|---|
| `StartLeds()` | Chooses SK6812 (with white) or WS2812 according to `Mem.LEDstrip` |
| `ColorLeds(text, first, last, colour)` | Switches on a range of LEDs and shows the text in the log |
| `ColorLed()`, `LedsOff()`, `ShowLeds()`, `SetBrightnessLeds()`, `Stripfill()`, `ClearScreen()` | Basic operations |
| `LEDstartup()` | Shows a colour on the strip during start-up |
| `LEDtest()` | The rainbow program of `Y` (can be interrupted with `Y`) |

A colour is a 32-bit value `0xWWRRGGBB`. With an SK6812 the highest byte determines the white LED; with a WS2812 it is ignored.

### Light sensitivity

`ReadLDR()` reads the LDR (0-4095) and scales it to 0-255. `DimLeds()` turns the measurement into the brightness of the LEDs (`OutPhotocell`) with the parameters `Mem.LightReducer` (`S`), `Mem.LowerBrightness` (`L`) and `Mem.UpperBrightness` (`M`). The value is squared, so that the curve matches the response of the eye better than a straight line.

### Status LEDs

`UpdateStatusLEDs()` controls the RGB LED on the Nano ESP32 and two LEDs on the PCB. The colour shows the state (program running, WIFI, Bluetooth). On the Nano ESP32 a low PWM value means *on*; `InitStatusLEDs()` therefore sets the resolution and frequency of the PWM pins explicitly (9 bit, 5 kHz), because the default values have changed between core versions.

## 11. The clock face

### Word clock

Every word is a macro that calls `ColorLeds()` with the first and last LED of the word. They are in `ClockFaces.h`, per variant:

```cpp
#define PRECIES ColorLeds("precies", 16, 22, LetterColor);
#define MTIEN   ColorLeds("tien",    25, 28, LetterColor);
#define KWART   ColorLeds("kwart",   32, 36, LetterColor);
#define VOOR    ColorLeds("voor",    38, 41, LetterColor);
```

The language functions in `WordClock.h` set the words for the time with only those macros:

```cpp
void Dutch(void)
{
 HET;                                          // HET is always on
 switch (timeinfo.tm_min)
   {
    case  0: IS;  PRECIES; break;
    case  4: case 5: case 6: IS; MVIJF; OVER; break;
    ...
   }
}
```

`Displaytime()` calls the right language functions per variant (`Dutch()`, `English()`, `German()`, `French()`; for the four-language clock all four). That a language sometimes has two possible readings ("half nine" is 8:30 or 9:30) is solved in the language functions.

The colours of the words follow from `SetSecondColour()` and the chosen colour scheme (`Mem.DisplayChoice`, the `Q` command).

### Fibonacci

`Fibonacci.h` contains `LEDsetTime()` (Fibonacci display), `MakeChronoList()` (Chrono display), `MakeFibonacciList()` (random combination of cells that forms a number) and `setPixel()`, which determines the LED layout of each cell per `NUM_LEDS`. See [chapter 10](Manual.md#10-fibonacci-clock-and-noclock) of the manual.

### Adding a new clock variant (word clock)

1. In `ClockFaces.h`, add a block for the new `#define`, with `NUM_LEDS`, `LEDNEARLDR`, `MATRIX_WIDTH`, `MATRIX_HEIGHT` and the macros for each word.
2. Add the name to the list `CLOCK_VARIANT` in `WordClock.h`.
3. Add the `#define` line in the list at the top of the sketch.

## 12. Bluetooth, time receiver and time sender

### Bluetooth

The clock uses the Nordic **nRF UART service** of NimBLE, not the TI CC2541 service (`FFE0`) of chips such as the HM-10.

| Function | Task |
|---|---|
| `StartBLEService()` | Starts the UART service |
| `SendMessageBLE()` | Sends text in packets of 20 characters (or longer with fast BLE, `+`) |
| `CheckBLE()` | Reads input and hands it to `ReworkInputString()` |
| `DisconnectBLE()` | Breaks the connection |

### Time receiver (`H05`)

The clock is then a BLE **client** and looks for a BLE device with the name `BLE-UARTtime`.

| Function | Task |
|---|---|
| `StartTimeReceiverScan()` | Starts scanning |
| `Connect_TimeReceiver()` | Makes the connection (three attempts) and subscribes to notifications |
| `CheckTimeReceiverClient()` | Main loop: handles clean-up after a connection that drops, time-outs and periodic re-scanning |
| `StopTimeReceiver()`, `ReconnectTimeReceiver()` | Stopping and reconnecting |
| `TimeReceiverNotifyCB()` | Receives text and hands it to `ReworkInputString()` |

All clean-up work after a broken connection is done in `loop()`, not in the callback itself. That prevents crashes in NimBLE.

### Time sender (`H06`, `H07`)

| Function | Task |
|---|---|
| `BLETimeSenderStart()`, `BLETimeSenderStop()` | BLE **server** with the name `BLE-UARTtime` |
| `CheckTimeSender()` | Sends `Thhmmss` every 10 seconds and `Dddmmyyyy` 1 second later, as soon as a client is connected and has enabled notifications |
| `InitHC12()`, `SendHC12TimeString()` | HC-12 on D4, 9600 baud, `Thhmmss` per minute (`K1`) or hour (`K2`) |

## 13. WIFI, web pages and OTA

### WIFI and NTP

| Function | Task |
|---|---|
| `ConnectWIFI()` | Connects with the SSID and password from `Mem` |
| `StartWIFI_NTP()` | Starts WIFI and NTP |
| `CheckforWIFINetwork()`, `ScanWIFI()` | Searching for networks (`G`) |
| `StartAPMode()` | Access point `StartWordclock` with the page `SoftAP.h` |
| `wpsInitConfig()`, `wpsStart()`, `wpsStop()` | WPS (`Z`) |
| `WiFiEvent()` | Handles WIFI events |
| `CheckRestoreWIFIconnectivity()` | Every hour: restores the connection (see [15](#15-monitoring-and-recovery)) |

### The routes of the web server

`WebPage()` registers all routes on the ESPAsyncWebServer:

| Route | Page or function |
|---|---|
| `/` | Main page (`Webpage.h`): input field, menu and live log window |
| `/log` | Complete log (`LogViewer.h`) |
| `/tekstprint`, `/tekstdownload` | Log as text, see [7](#7-output-the-tekstprint-functions-and-the-log) |
| `/menu` | Touch-friendly menu page (`menu_html[]` from the variant file) |
| `/menustate` | JSON with the state of the switches for the menu page |
| `/colourpick`, `/setcolours` | Colour picker and the processing of the chosen colours |
| `/get` | Command from the input field. The command is put in `PendingCommand` (the web server is asynchronous) and the browser is redirected to `/wait` |
| `/wait` | Shows *Processing command...* and refreshes every second until the command has been processed in `CheckWIFIcommand()`; then back to `/` |
| `/update` (GET and POST) | OTA page and the uploading of the `.bin` file |

The pages are held as *raw string literals* in the `.h` files (`R"rawliteral( … )rawliteral"`). Values that the clock has to fill in are placeholders (`%%COLNL%%`, `%%CNAME%%`…) and are replaced when the page is requested. The main page and the menu page of `/menu` have a small **live log window** that fetches `/tekstprint` every second, shows the last 50 lines and only scrolls along automatically if the reader is at the bottom.

An HTML page can be made in an ordinary text editor (for example Notepad++) or with an AI assistant and then pasted between `R"rawliteral(` and `)rawliteral";` in a `.h` file.

### OTA

The uploading of a `.bin` runs in `/update` (POST) with the functions of `Update.h`. The sketch protects against two simultaneous uploads: a second upload is refused as long as a first one is active (`OTAupdateInProgress`, V244). After a successful update `shouldReboot` is set; `EverySecondCheck()` then restarts the clock after two seconds.

## 14. MQTT and Home Assistant

| Function | Task |
|---|---|
| `CheckMQTT()` | Maintains the connection (non-blocking, a new attempt every 5 seconds) |
| `publishMQTTDiscovery()` | Once after connecting: sends the registration messages |
| `publishDiscoverySensor/Button/Switch/Number/Select()` | Helper functions per type |
| `PublishMQTTStates()` | Sends all values: every minute, after every command and directly after connecting |
| `MQTTCallback()` | Receives commands from `<id>/cmnd` and hands them to `ReworkInputString()` |
| `MQTTDeviceId()` | Device id: Bluetooth name in lower case, spaces and slashes as `_` |

The MQTT settings (`MQTT_SERVER`, `MQTT_PORT`, `MQTT_USER`, `MQTT_PASSWORD`) are constants at the top of the sketch. See [chapter 9 of the manual](Manual.md#9-mqtt-and-home-assistant) and [MQTT-HomeAssistant-setup.md](MQTT-HomeAssistant-setup.md).

## 15. Monitoring and recovery

| Mechanism | Where | Behaviour |
|---|---|---|
| No WIFI | `CheckRestoreWIFIconnectivity()` (every hour) | Reconnect. After 30 hours without an IP: restart (`Mem.WIFInoConnection`) |
| Main program hangs | Same routine | Fewer than 10 loops per second with a WIFI network nearby: restart (`Mem.LoopRebooted`) |
| Restarting too often | `setup()` | More than 5 restarts shortly after start-up: `Reset()`. `CheckMCURestartCounter()` clears the counter when the clock has run stably for 2 minutes |
| Reset button | `CheckResetPin()` | Held down for 2 seconds: factory settings and restart |
| Invalid storage | `InitStorage()` | `Mem.Checksum` is not 25065: default values |

## 16. Modifying and extending

### Adding a new menu command

1. Choose a free character and add a `case` in `ReworkInputString()` (look at the existing cases `'V'`, `'^'` and `'_'` as an example).
2. Put the value in `Mem` (see [4](#adding-a-new-setting)) and give a message through `snprintf(sptext, …)`.
3. Add the command to `menu[]` (at most 40 characters per line).
4. Add it to the web menu page if a switch belongs with it (`menu_html[]` in the variant file and `/menustate`).
5. Consider an item in Home Assistant (see `publishDiscovery…` and `PublishMQTTStates()`).

### Adding a new input source

Make sure that the source gives a string to `ReworkInputString()`. Nothing more is needed; the command then works everywhere.

### A new colour scheme for the Fibonacci clock

Add a row to the table `colors[][5]` in `Fibonacci.h`, a name to `QchoiceNames[]`, and `LASTITEM` follows by itself.

## 17. Function overview

In the folder with the sketch is the file `FunctionListV247.txt` with all functions per file and a short description of each function. The list has been updated for V247. Compared with the list for V237 these are the changes:

| Function | Change |
|---|---|
| `ParseColourInput()` | Renamed (was `parseColourInput`) |
| `PublishMQTTStates()` | Replaces `PublishCPUTemperature()`, `PublishLDRreading()` and `PublishDisplayChoiceState()` (V244) |
| `publishDiscoveryNumber()`, `publishDiscoverySwitch()` | New (sliders and switch in Home Assistant) |
| `publishDiscoverySensor()`, `publishDiscoverySelect()` | Extra parameters (diagnostics, list of options) |
| `CheckResetPin()` | Now reads `Mem.ResetPin` (V246) |
| `InitHC12()`, `SendHC12TimeString()` | Are now called through `H07` (V247) |
