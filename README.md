# Arduino-ESP32-Nano-Word Clock

![image](https://github.com/ednieuw/Arduino-ESP32-Nano-Wordclock/assets/12166816/d5d450ce-df3c-4274-94ba-184792a7ee3d)

Build your own word clock with one or four languages using an Arduino ESP32 Nano.<br>
[Use the word clock selector page to compare other software/hardware designs.](https://github.com/ednieuw/WordclockSelector)<br>

The clock can be controlled via a phone app, a web page in a browser, a serial cable connected to your PC, a rotary encoder, an IR remote controller, or membrane keyboards.<br>

<img src="https://github.com/user-attachments/assets/0b566154-2bc8-415f-927c-ea496154f767" style="width:50%;">

To connect the clock to Wi‑Fi, enter the SSID and password of your router or use the WPS function.<br>
Software updates can be uploaded with OTA (Over the Air).<br>

It keeps its time accurate to within a second by synchronizing with the Network Time Protocol (NTP), including time zone and daylight‑saving corrections.<br>

If no internet is available, a DS3231 RTC module can be attached to the PCB for accurate timekeeping. With a rotary encoder or membrane pad, the time can be set manually.<br>
An LDR (light‑dependent resistor) is used to adjust the LED brightness for optimal visibility.<br>

The older Arduino MKR1000, Nano BLE 33 and similar variants with both Bluetooth and Wi‑Fi have the disadvantage that only Wi‑Fi or BLE can be used — not both simultaneously.

The ESP32 features an Espressif BLE/Wi‑Fi module. Bluetooth LE uses a Nordic nRF52 chip (not the TI CC2541).  
This means you must use a different BLE service for serial communication: not characteristic FFE0, but 6e400001‑b5a3‑... etc., in your serial terminal app to communicate with the clock's settings.<br>

See here: [Use BLE on mobile to control the ESP32](https://github.com/ednieuw/nRF-ESP32)

![image](https://github.com/user-attachments/assets/f187786c-b6db-49a8-9e91-708b93361390)
Clock with Corten steel face

---

# Hardware

For my projects I designed the printed circuit board (PCB) using [Fritzing software](https://fritzing.org/).

Fritzing is easy to use and can export Gerber files for PCB manufacturers. You can also order PCBs from within the Fritzing app.<br>
[PCBWay](https://www.pcbway.com/) prints 10 PCBs for $5; with roughly $25 shipping and customs charges this is very inexpensive. The PCBs arrived within a week and the quality was excellent.

The [PCB design .FZZ file and Gerber files](PCB) are in the PCB folder in this repository or here: [ESP32 PCB](https://github.com/ednieuw/NanoESP32PCB)

The PCB connects to a rotary encoder or three‑button membrane switch, a DS3231 RTC module or other I2C device, a DCF77 receiver or other pulse input device, and an LDR to measure light.

The SK6812 RGBW LED strip operates at 5V, but the data signal from the Nano is only 3.3V.  
I tried using optocouplers to shift the data signal from 3.3V to 5V but that approach failed. See:  
https://ednieuw.home.xs4all.nl/ElecProj/OptoSK6812/OptocouplerSK6812.html  
For level shifting, the 74HCT125 is used. It has four buffers, and I adapted the PCB to use the other three for other signals.  
However, the SK6812 RGBW strip (with 14 LEDs) also worked when the data line was connected directly to the 3.3V data output of the Nano ESP32. I have not tested longer strips.  
Lesson: you can often connect the SK6812 RGBW strip with a 470 Ω resistor in the data line and a 200–1000 µF capacitor between 5V and GND directly to the strip without a level shifter.

![Nano-ESP32-V04](https://github.com/user-attachments/assets/3ad06452-0dd4-4310-8b51-25c115ec3959)

An alternative universal PCB to drive RGB(W) LEDs and/or white 2835 LED strips with shift registers can be found here: [Nano ESP32 RGBW BW PCB](https://github.com/ednieuw/NanoESP32-BW-RGBW-clock).<br>
That PCB (with one Arduino source) supports both color LED clocks and white LED clocks using an Arduino Nano ESP32.

![V01SmallNanoESP32-3D](https://github.com/user-attachments/assets/fb3863f4-1346-4985-a541-f087be7e8b68)
The design for a [minimum PCB for small spaces](/PCB-small) is in the PCB-small folder.

Power for the LED strip can be drawn from the VBUS pin (5V USB) on the Nano when powered via USB‑C.  
VBUS provides 5V whenever the board is powered by USB. You can feed the LED strip from this connection, but be mindful that current travels through the PCB. USB‑C power supplies can provide over 2A.  
If the board is powered via the VIN pin, VBUS is disabled — VIN powering does not provide 5V on VBUS.

The PCB supports multiple power options.  
A 5V, >1A supply can be attached at the 5V power connection on the top left of the PCB.<br>
If the red‑line shortcut (see picture) is closed, the LED strip is powered from the 6–21V VIN connection (via the onboard regulator) with 5V output.<br>
This will also power the Nano ESP32. You can connect VIN using a 5V USB power supply, although the minimum VIN is noted as 6V; the 1N5817 diode drops about 0.5V.  
IMPORTANT: this 5V connection goes directly to the LED strip. If a higher voltage is applied, the LEDs will be destroyed while the Nano may still power on.<br>
If the Nano ESP32 is powered via USB‑C, short the yellow jumper so LEDs are powered by USB. This is acceptable for ~20 LEDs but not recommended for long strips.

![5VConnections](https://github.com/ednieuw/Arduino-ESP32-Nano-Wordclock/assets/12166816/39ea30fc-2850-4c1c-9fdb-4b4bec752f9a)

After connections are made, the PCB and hardware look like this:

![image](https://github.com/ednieuw/Arduino-ESP32-Nano-Wordclock/assets/12166816/86c688e0-8006-4042-880a-40064a4d8949)
![NanoESP32-V04-3D](https://github.com/user-attachments/assets/e717d61f-7953-4e35-80c1-278ca05a10df)
![image](https://github.com/ednieuw/Arduino-ESP32-Nano-Wordclock/assets/12166816/77fc651e-cfd6-4681-83e5-53c4f6bcfb55)

---

# PCB Connections

The large PCB has connections for (from top right clockwise):

- 6–21V power input to VIN on Nano ESP32
- DCF to pin D2, 3V3, and GND
- LDR to pin A2 and 3V3
- RTC clock to GND, 3V3, A4, and A5
- Rotary encoder or 3‑button switch to D8, D3, D4, 3V3, and GND
- LEDs to D10 and D9
- Connector to D12, D11, D10, and D9
- D7 with 5V via 3V3 → 5V level shifter
- Connector with GND and 3V3
- Connector for AMS1117 5V → 3V3 (1A) regulator
- D6 with 5V via 3V3 → 5V level shifter
- Connector for LED strip SK6812/WS2812 to 5V, D5, and GND
- D3 referenced to 3V3 and GND
- 5V power input to 5V‑USB or VIN on ESP32 and to LED strip
- 5V and GND connector and connector to VIN and 5V‑USB on Arduino Nano ESP32
- Connector to GND, RX, and TX

The small PCB has connections for (from top left clockwise):

- LDR to A2 and 3V3
- 5V power input to 5V‑USB on ESP32 and LED strip
- Connector for LED strip SK6812/WS2812 to 5V, D5, and GND
- Rotary encoder or 3‑button switch to D8, D3, D4, 3V3, and GND
- RTC clock to GND, 3V3, A4, and A5

![image](https://github.com/user-attachments/assets/f2919fb4-b571-4a1d-8264-4e227d4b104e)

---

# The LED Ground Plate

![IMG_1469](https://github.com/user-attachments/assets/a6a80ac1-4cb6-47b2-a496-f9645e8795c1)

The LED spacing suits a clock of 25×25 cm (60 LEDs/m) or 50×50 cm (30 LEDs/m).  
You can place 144 LEDs in 12 rows of 12, or position LEDs only behind the letters that should light.  
The latter reduces LED count and drilling, but increases solder points — each solder joint is a potential failure. I found imperfect soldering can loosen over time and cause malfunctions.

The software includes a digital time display option that requires all 144 LEDs.  
For a word plate, you can cut out words (e.g., IT WAS FIVE) from 1 cm MDF with a jigsaw. Paint the inside surfaces bright white so the light looks clean.  
Foamed PVC (white, 10 mm, RAL 9003) is preferable: it cuts easily and resists discoloration (painted MDF can yellow).

Stick/glue the strips left‑to‑right on odd rows and right‑to‑left on even rows.  
### Follow the arrows on the strip.  
(Flip the strip every second row.)  
This avoids long data runs from one end of a row to the next and means the software does not need custom remapping.

![IMG_5384](https://github.com/user-attachments/assets/945ba89a-6fda-43fe-b16b-52f5babf0584)

I recommend using tinned copper wire. Solder all 5V connections on the left side of the board and all GND on the right. Drill small holes through the spacer plate next to the 5V and GND pads, bend the wire 90°, pass it through, and solder to the strip.

![IMG_5467](https://github.com/user-attachments/assets/cb3148c9-6efb-4a4c-b04a-4ceb46584de6)
![IMG_1471](https://github.com/user-attachments/assets/fccbe36d-321e-4bd3-af34-782082c0a97e)

Place the spacer plate over the LEDs, then a sheet of white paper, and finally the word plate.

---

# Software

The `ESP32Arduino_WordClockVxxx.ino` turns the Nano ESP32 and SK6812 or WS2812 LED strips into a Word clock, connects to your Wi‑Fi router, and receives time from an NTP server.

The software can be controlled via Bluetooth on your PC or a mobile Android or iPhone/iPad/iMac.  
Several designs can be selected before compiling (if you have a word plate available).  
Use a word plate design [from this repository](WordPlateFrontCovers), or use your own and adapt the LED positions in the software.

![ClockDefines](https://github.com/user-attachments/assets/89a48534-c97a-41fc-94a5-bb8ee35ebb34)

Select one of the three word clock builds:
```cpp
//#define FOURLANGUAGECLOCK
#define NL144CLOCK
//#define NLM1M2M3M4L94          // NL clock with four extra LEDs for the minutes to light up
```

- `#define NL144CLOCK` → a 144 LED single-language clock. Default language is Dutch. For French, German, and English, copy the language blocks from the four-language clock and adapt them between the NL144CLOCK defines.  
  [Build instructions (NL/EN)](https://ednieuw.home.xs4all.nl/Woordklok/Bouwpakket/WoordklokSK6812.htm) or [Manual-Instructions](Manual-Instructions)

- `#define NLM1M2M3M4L94` → a 144 LED single-language clock with 4 extra minute LEDs and a slightly different layout.

- `#define FOURLANGUAGECLOCK` → a 4-language clock with 625 LEDs in a 25×25 grid.  
  [Four-language clock instructions](https://github.com/ednieuw/FourLanguageClock)

The `libraries.zip` archive includes the libraries needed to compile the software. Unpack them into your Arduino libraries folder. Libraries evolve over time, which may cause incompatibilities; for reproducibility the required versions are provided.

Alternatively, you can use the ArduinoOTA sketch (Examples → ESP32 Arduino).  
Upload the OTA updater to the Nano ESP32 and note the IP address printed in the Serial Monitor. Type that IP in a browser, log in with admin/admin, and upload the `.bin` from this repository.  
Or use this OTA web-updater: [Upload `.bin` files to the ESP32 using wifimanager](https://github.com/ednieuw/OTA-webupdater).

---

# How to Compile

At the time of writing (June 2025), the Espressif ESP32 Arduino core v3.2 compiles to a working program when the Arduino Nano ESP32 board is selected and ESP32WordclockV100.ino (or newer) is used.  
The Arduino ESP32 core versions 2.0.17 / 2.0.18 compile without errors and are recommended — they generate smaller binaries (v2.0.18 ≈ 1.0 MB vs v3.2 ≈ 1.3 MB).

There are two compiler pin-numbering options: one uses GPIO numbering (recommended/legacy for this code), the other uses Arduino macro pin numbering.  
This code must be compiled with GPIO numbering selected!! If LEDs do not light correctly, verify the pin-numbering option.

Install the Arduino Nano ESP32 board in the Arduino IDE and configure:
- Board: Arduino Nano ESP32  
- Partition Scheme: With FAT  
- Pin Numbering: By GPIO number (legacy) — CHANGE THIS setting if different

If you use the Arduino-style pin numbering, the LED library (Adafruit NeoPixel or EdSoftLED) can behave incorrectly on this board — causing the LEDs not to light.

![image](https://github.com/user-attachments/assets/1f4eeec5-ebc5-47fb-b513-2514f5ff6ecd)

---

# Before Starting

If a DS3231 RTC module is not installed, the clock synchronizes time via NTP. If you attach a DS3231 module, the clock can keep accurate time without internet; in the menu toggle use of the DS3231 (J: Toggle DS3231 RTC module).

To connect to the internet you must provide your Wi‑Fi SSID and password to the clock software.

## Connect to Wi‑Fi

Enter the Wi‑Fi SSID and password once; the credentials are stored in the microcontroller's memory.

If the software starts without an SSID it will start an access point:
- On your phone/tablet/PC open Wi‑Fi networks and connect to `StartWordClock`.
- Password: `12345678`
- Enter your router SSID and password on the web page and Submit.
- The clock will restart and attempt to connect to your router.

## Connect via Bluetooth

To simplify setup, use a Bluetooth UART terminal app to enter Wi‑Fi credentials into the clock.

![image](https://github.com/ednieuw/Arduino-ESP32-Nano-Wordclock/assets/12166816/261800f1-7cd6-4078-8c7e-ad9cd2ba47ec)

Recommended apps: BLESerial nRF, BLE Serial Pro, Serial Bluetooth Terminal
- iOS: [BLE Serial Pro](https://apps.apple.com/nl/app/ble-serial-pro/id1632245655?l=en) or [BLEserial nRF](https://apps.apple.com/nl/app/bleserial-nrf/id1632235163?l=en).
  Tip: enable Fast BLE (`+`) for faster transfers.
- Android: [Serial Bluetooth Terminal](https://play.google.com/store/apps/details?id=de.kai_morich.serial_bluetooth_terminal). Keep Fast BLE off (default).

Send Wi‑Fi credentials prefixed with `a` and `b`:
- `aSSIDname` → Send
- `bPassword` → Send
Optionally:
- `cClockname` → Send (name shown in router / BLE device list)

---

# Control of the Clock

If connected to the internet the clock will sync with an NTP server (toggle NTP in the menu).

Default time zone is UTC+1 (Amsterdam) but can be changed in the menu. To join a Wi‑Fi network, an SSID and password must be entered.

You can control the clock via:
1. USB serial (Serial Monitor / other terminal)
   - I use the Arduino IDE Serial Monitor.
   - Sending `I` prints the info menu and current settings.
2. BLE UART serial terminal app on a mobile device
3. Web interface at the clock's IP address (or `wordclock.local` if mDNS/Bonjour/Avahi is available)
4. Rotary encoder or membrane keypad (if installed)

Send commands by the first character followed by parameters. Many commands toggle options — send the letter to toggle.

Example commands:
- `aMySSID` — set SSID
- `bMyPassword` — set Wi‑Fi password
- `cClockName` — set BLE/web name
- `T132145` — set time (13:21:45)
- `D06112022` — set date (6 Nov 2022)
- `@` — restart MCU
- `R` — reset preferences (RRR clears SSID/password/timezone to defaults)

---

# Control & Menu (Detailed)

If the clock is connected to the internet it will synchronize with an NTP server (you can toggle NTP in the menu).  
Default time zone is UTC+1 (Amsterdam) but you can change it in the menu.

Interact via USB serial, BLE, web page, rotary encoder, or membrane keypad. Send the first character of a command followed by parameters when required. Most single-letter commands toggle the setting.

Send `I` to print the info menu and current settings.

Menu summary (examples and formats follow):
```
A SSID        B Password       C BLE beacon name
D Date        T Time
E Timezone    F Own colour
G Scan WIFI   H Toggle rotary/keypad
I Info        J Toggle DS3231 RTC
K LDR reads   N Display off interval
O Display On/Off P Status LED
Q Colour choice R Reset
S Intensity    W WIFI
X NTP         Z Fast BLE
# Self test   ! Show RTC/NTP
& Update RTC  U Demo
= Display memory % LED strip select
$ Fire display ( Extra LEDs
) HET/IS/WAS off
@ Reset MCU
```

Example info output (serial/BLE):
- Display off between: 23h - 08h  
- Display choice: Yellow  
- Slope: 10   Min: 5   Max: 255  
- SSID: FRITZ!Box  
- BLE name: wordclock  
- IP-address: 192.168.178.141 (/update)  
- Timezone: CET-1CEST,M3.5.0,M10.5.0/3  
- WIFI=On NTP=On BLE=On FastBLE=Off  
- LED strip: WS2812 (Send % to switch)  
- Software: ESP32Arduino_WordClockV056.ino  
- ESP32 Arduino core version: 2.0.17

---

## Common Commands (detailed)

- A / a + SSID
  - Example: `aMySSID` — set Wi‑Fi SSID to "MySSID"
  - Case-insensitive for the letter; content preserves case for SSID.

- B / b + Password
  - Example: `bMySecret` — set Wi‑Fi password
  - If SSID or password length < 5 the software may disable Wi‑Fi to speed startup.

- C / c + BLE beacon name
  - Example: `cClock123` — change the BLE/web device name shown in router and BLE lists.

- CCC
  - Toggle BLE on/off. Be careful — if BLE is off you can only control the clock via Wi‑Fi (web) or USB serial.

- D + Date
  - Format: `Dddmmyyyy`
  - Example: `D06112022` sets date to 6 Nov 2022.

- T + Time
  - Format: `Thhmmss` or `hhmmss`
  - Example: `T132145` sets time to 13:21:45.

- E + Timezone string
  - Timezone strings are exact (POSIX-like). Copy the timezone token from the list (bottom of README) and send prefixed with `E`.
  - Example: `eAEST-10AEDT,M10.1.0,M4.1.0/3`

- F + Own colour (LED character colour)
  - Hex WRGB: `FWWRRGGBB` (two hex digits per channel) or short decimal `FWRGB` (digits 0–9).
  - Examples:
    - `F00FF000` → intense red (no white)
    - `F0900` → decimal shorthand (white 0, red 9, green 0, blue 0)
    - `F8800FF00` → 50% white + 100% green (SK6812 WRGB format)

- G
  - Scan available Wi‑Fi networks (prints SSIDs found).

- H
  - Toggle use of rotary encoder (or 3×1 membrane keypad).

- I / II
  - `I` prints the short info menu.
  - `II` prints the extended menu (long help).

- J
  - Toggle use of the DS3231 RTC module.
  - When DS3231 is enabled, NTP may be turned off. Use option `X` to toggle NTP if needed.

- K
  - Toggle LDR readouts (prints LDR readings and calculated intensity).

- N + hhmm (display off interval)
  - Example: `N2208` — turn display off from 22:00 to 08:00.

- O
  - Toggle display On/Off.

- P
  - Toggle status LED on the board.

- Q (Display colour choice)
  - `Q0` Yellow (default)
  - `Q1` Hourly changing
  - `Q2` All white
  - `Q3` Use own defined colours
  - `Q4` Own defined + HET/IS/WAS changing
  - `Q5` Rainbow hourly
  - `Q6` Digital (requires 12×12 = 144 LEDs)
  - `Q7` Hourly colour + HET/IS/WAS changing
  - `Q8` Rainbow changes every minute
  - `Q9` Fast rainbow (4 seconds)

- R
  - Reset preferences to defaults. `RRR` clears SSID/password and sets timezone to Amsterdam with WIFI/NTP/BLE on. `RRRRR` is a deeper reset.

- S (Light intensity parameters)
  - `S <slope> L <min> M <max>` or compact `S80 L5 M200`
  - Values 0–255. Suggested min values 5–20 to avoid flicker.

- U
  - Demo mode timing. `U200` runs demo with 200 ms per minute.
  - Send `U` to toggle demo off.

- Y
  - Play lights test (all LEDs show flowing rainbow).

- W
  - Toggle Wi‑Fi On/Off.

- X
  - Toggle NTP On/Off.

- + / Z (Fast BLE)
  - `+` toggles fast BLE mode in some apps.
  - `Z` toggles between long and short BLE packet modes.

- #
  - Self test, e.g., `#900`.

- !
  - Show NTP, RTC, and DS3231 times.

- &
  - Get NTP time immediately and store in RTC/DS3231.

- %
  - Switch between LED strip types (SK6812 vs WS2812).

- $
  - Fire display (supported 12×12 displays).

- (
  - Toggle optional extra LEDs (logo/ornaments).

- )
  - Toggle HET/IS/WAS behavior.

- @
  - Restart the MCU (soft reset).

- =
  - Show permanent memory (Mem struct) values.

- Set RTC directly (when DS3231 attached)
  - `HHMMSS` (six digits) stores time in attached RTC.
  - Example: `152300` sets time to 15:23:00 (same as `T152300`).

---

## Wi‑Fi / Web interface

When connected to Wi‑Fi the clock prints its IP address in the menu. Open a browser and navigate to the IP (or `http://wordclock.local` if mDNS/Bonjour/Avahi is available).  
- Mac/iOS: Bonjour is built-in.  
- Linux: install Avahi (http://avahi.org/).  
- Windows: install Bonjour (Apple support).

The web interface provides the same settings as BLE/serial and includes an upload page for OTA if enabled.

---

## BLE specifics

- Preferred mobile apps:
  - iOS: BLE Serial Pro, BLEserial nRF
  - Android: Serial Bluetooth Terminal, nRF UART
- For iOS, enabling Fast BLE (`+`) speeds up transfers. For Android, keep Fast BLE off unless your app supports it.
- BLE service/characteristics: use the Nordic UART service UUID (6e400001‑b5a3‑...), not the TI FFE0 characteristic.

---

## Light intensity explained

Three parameters control light behaviour:
- Min: minimum brightness to avoid flicker
- Max: maximum brightness (0–255)
- Slope: sensitivity (percentage multiplier)

Suggested values: Min 5–20, Max 200–255. When no LDR is attached set Min = 255.

The LDR reading is scaled to 0–255: read = analogRead(PhotoCellPin) / 16 (ESP32 analog is 0–4096).

---

## Example Serial/BLE Session

1. Connect via BLE or USB serial.
2. Send `I` — view current settings.
3. If no SSID set:
   - Connect to `StartWordClock` with password `12345678`.
   - Enter SSID: `aMySSID`
   - Enter password: `bMyPassword`
   - Optionally set name: `cMyClock`
4. Restart: `@`
5. Verify IP printed in menu and open web UI.

---

# Program explanation, code snippets, advanced options & Timezones

This final section describes the program flow, key routines, helper functions, storage handling, and includes the timezone strings you can copy/paste into the menu with the `E` command.

---

# Program overview

The main program (ESP32Arduino_WordClockVxxx.ino) is structured to do minimal work in loop() and drive the clock from timed routines:

- setup()
  - Initialize Serial, BLE, Wi‑Fi, NTP, RTC (if present), storage, LED strip, web server and OTA.
- loop()
  - Very lightweight: increments a loop counter and calls EverySecondCheck() (or Demo mode) and CheckDevices().

Timing architecture:
- EverySecondCheck() — performed roughly once per second; updates time, toggles status LED and calls EveryMinuteUpdate() when minute changes.
- EveryMinuteUpdate() — updates displayed words, checks alarms and calls EveryHourUpdate() when hour changes.
- EveryHourUpdate() — performs hourly tasks and calls EveryDayUpdate() when the date changes.
- EveryDayUpdate() — daily tasks (e.g., DST checks, schedules).

This keeps the CPU mostly idle between events and makes the routine timing predictable.

---

# Important code snippets (high‑level)

These are illustrative excerpts — the full code is in the repository with many comments.

Initial defines for words (language-specific):
```cpp
#define PRECIES ColorLeds("precies", 16, 22, LetterColor);
#define MTIEN   ColorLeds("tien", 25, 28, LetterColor);
#define KWART   ColorLeds("kwart", 32, 36, LetterColor);
#define VOOR    ColorLeds("voor", 38, 41, LetterColor);
```

Main timed loop:
```cpp
void loop() {
  Loopcounter++;
  if (Demo) Demomode();
  else EverySecondCheck();
  CheckDevices();
}
```

Every second:
```cpp
void EverySecondCheck(void) {
  static int Toggle = 0;
  uint32_t msLeap = millis() - msTick;
  if (msLeap > 999) {
    msTick = millis();
    Toggle = !Toggle;
    UpdateStatusLEDs(Toggle);
    GetTijd(false); // update timeinfo from RTC/NTP
    if (timeinfo.tm_min != lastminute) EveryMinuteUpdate();
  }
}
```

Every minute:
```cpp
void EveryMinuteUpdate(void) {
  lastminute = timeinfo.tm_min;
  DisplayTime(); // update LEDs / text strings
  if (timeinfo.tm_hour != lasthour) EveryHourUpdate();
}
```

Rework input string (serial/BLE/web):
```cpp
void ReworkInputString(String InputString) {
  switch (InputString[0]) {
    case 'A':
    case 'a':
      if (InputString.length() > 5) {
        // store SSID
      }
      break;
    case 'B':
    case 'b':
      // store password
      break;
    case 'T':
    case 't':
      // set time
      break;
    // many other cases...
  }
}
```

LED helpers:
```cpp
void ColorLeds(const char *Text, int FirstLed, int LastLed, uint32_t RGBColor) {
  // Map letters or LED indices to strip and set colors/intensity
}
```

Status LED:
```cpp
void SetStatusLED(int Red, int Green, int Blue) {
  analogWrite(LED_RED,   512 - Red); // inverted wiring on Nano ESP32 board
  analogWrite(LED_GREEN, 512 - Green);
  analogWrite(LED_BLUE,  512 - Blue);
}
```

Self-test:
```cpp
void Selftest(int Delayms) {
  GetTijd(true); // print time
  LedsOff();
  // cycle words/LEDs with Delayms between updates
}
```

---

# Storage and preferences

Settings are stored in a single struct (EEPROMstorage / Preferences). The program saves and restores this struct from SPIFFS/Preferences to persist preferences across power cycles.

Functions:
- InitStorage() — validate and load settings (or reset to defaults)
- StoreStructInFlashMemory() — write settings to storage
- GetStructFromFlashMemory() — read settings and validate checksum

When checksum fails, Reset() restores safe default settings.

---

# Web page & OTA

- The web UI (webpage.h) is embedded as a raw literal string in a TAB. It provides:
  - Wi‑Fi setup page (AP mode)
  - Status page and menu replicating serial/BLE options
  - OTA upload form (if OTA enabled)

- Use ArduinoOTA or the included OTA-webupdater for remote firmware updates.

---

# BLE specifics

- Uses NimBLE, exposing a Nordic UART‑like service (6e400001‑b5a3‑...).
- For iOS use BLE Serial Pro (supports Fast BLE).
- For Android use Serial Bluetooth Terminal or similar; be careful with >20 bytes fragmentation — use slow or Fast BLE modes depending on app behavior.

---

# Troubleshooting hints

- LEDs not lighting:
  - Ensure correct pin-numbering (GPIO mode in Board options).
  - Verify you chose the correct LED type (SK6812 vs WS2812).
  - Check power supply and common GND between MCU and LED strip.
  - Add 470 Ω in data and 200–1000 µF capacitor across 5V/GND near strip.

- Wi‑Fi not connecting:
  - Ensure SSID/password are correct and of sufficient length (≥ 8 chars recommended).
  - In AP mode connect to StartWordClock and re-enter credentials.

- BLE not seen:
  - Check BLE is enabled (CCC toggle).
  - Confirm BLE beacon name from menu.

- Serial shows garbled text:
  - Try sending `+` to switch to slower BLE mode if using mobile BLE apps.

---

# Advanced options

- Multiple display modes: change per minute or per hour with `~` (`~1` = per minute, `~2` = per hour).
- Demo mode: `U<ms>` runs a fast demo for testing (e.g., `U200`).
- Extra LEDs and logo support: optional pins and menu toggles for custom hardware.
- Additional word plates/languages: add language functions (Dutch, English, German, French) by copying patterns in the code and defining LED indexes.

---

# Full Timezone list (copy-and-paste exact token after E)

These POSIX-like timezone tokens are used by the clock. Copy the full string (including commas and slashes) and send it prefixed with `E`. Example: `eCET-1CEST,M3.5.0,M10.5.0/3`

Common timezone examples (exact strings):

- UTC: `UTC0`
- CET / Amsterdam (Central Europe): `CET-1CEST,M3.5.0/2,M10.5.0/3`
- Europe/Amsterdam: `CET-1CEST,M3.5.0/2,M10.5.0/3`
- Europe/Paris: `CET-1CEST,M3.5.0/2,M10.5.0/3`
- Europe/Berlin: `CET-1CEST,M3.5.0/2,M10.5.0/3`
- UK / BST: `GMT0BST,M3.5.0/1,M10.5.0`
- US Eastern (New York): `EST5EDT,M3.2.0/2,M11.1.0/2`
- US Central (Chicago): `CST6CDT,M3.2.0/2,M11.1.0/2`
- US Mountain (Denver): `MST7MDT,M3.2.0/2,M11.1.0/2`
- US Pacific (Los Angeles): `PST8PDT,M3.2.0/2,M11.1.0/2`
- Australia/Sydney: `AEST-10AEDT,M10.1.0,M4.1.0/3`
- Australia/Perth: `AWST-8`
- Asia/Tokyo: `JST-9`
- Asia/Shanghai: `CST-8`
- Asia/Kolkata: `IST-5:30`
- America/Anchorage (Alaska): `AKST9AKDT,M3.2.0/3,M11.1.0/2`
- Atlantic/Reykjavik (Iceland): `GMT0`
- New Zealand (Auckland): `NZST-12NZDT,M9.5.0/2,M4.1.0/3`

---
