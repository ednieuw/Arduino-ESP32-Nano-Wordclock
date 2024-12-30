/*
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
 Changes V025: 
 Changes V026: Removed #ifdef  MOD_DS3231. Updated detection and software around DS3231. RTClib.h still in use
               All three clock types work.
 Changes V027: Is V026 with WS2812 and SK6812 as defines. Was used for test
 Changes V028: Bug StatusLEDs On Off corrected. StartLEDs optimized 
 Changes V029: Removed SetSecondColour() from DisplayTime() bij NL144 clock). Ulrich's word plate face corrected
 Changes V030: Ulrich's clock
 Changes V031: Added length control in menu options S, U and V  
 Changes V032: Added void Demomode(void) with optien M in the menu. moved Selftest to #-entry in the menu. Updated web page
 Changes V033: Using all available LEDs on board and PCB. 
 Changes V034: Working version
 Changes V035: Correct clock went off at 00:00 for one minute if( (timeinfo.tm_hour == Mem.TurnOffLEDsAtHH) && (Mem.TurnOffLEDsAtHH != Mem.TurnOnLEDsAtHH))  
 Changes V036: Send_P() -> Send(). Leds on? --> Update display  in menu óption 'O en EveryHourUpdate(). Ulrich's clocks >18 --> >19 in SayHour(). 
                Added two clock faces #define NLM1M2M3M4L94          // NL clock with four extra LEDs for the minutes to light up for Ulrich
                                  and #define NLM1M2M3M4L256         // NL clock with four extra LEDs for the minutes to light up for Ulrich 
 Changes V037: Selftest runs once. Added   <meta name="google" content="notranslate" /> in Wedpage.h. Added menu items Selftest RTC                                 
 Changes V038: Added Rotary and One-wire support. Copied from ESP32WordClockUltimatePCB_V015
 Changes V039: Removed unneccesary command WiFi.mode(WIFI_STA);  Changed PCB and ESP32 LEDs to analog control of the intensity
 Changes V040: Menu changed!! 
 Changes V041: 
 Changes V042: WIFI not working. EdSoft_SK6812 compiles with ESP32 V3.0.4, EdSoftLED compiles with ESP32 V3.0.4 .
 Changes V043: WIFI working EdSoftLED compiles with ESP32 V3.0.5. All working. 
 Changes V044: Added ESP32 core 2.0.17 and 3.0.5 functionality to compile both versions 
 Changes V045: CCC in menu to turn on/off BLE. Rotary working again. Added void InitRotaryMod(void)
 Changes V046: Cleanup code. Code synced with ESP32WordClockUltimatePCB_V025 
 Changes V047: Updated menu and webpage
 Changes V048: Removed define NLM1M2M3M4L256 Ulrich's clock design. Scan network added. 
 Changes V049: Rotary usage can be turned on and off in software with menu 
 Changes V050: Only first 10 LEDs light up at startup instead of whole LED-string
 Changes V051: Moved In Setup() InitStorage() just after serial started. Setting were read too late  
 Changes V052: Violet startup Progress LEDs added
  */
