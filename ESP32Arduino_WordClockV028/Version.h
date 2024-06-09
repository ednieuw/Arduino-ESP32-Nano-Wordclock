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

  Changes V026: Removed #ifdef  MOD_DS3231. Updated detection and software around DS3231. RTClib.h still in use
                All three clock types work.
  Changes V027: Is V026 with WS2812 and SK6812 as defines. Was used for test
  Changes V028: Bug StatusLEDs On Off corrected. StartLEDs optimized 
  Changes V029:
  Changes V030:


*/