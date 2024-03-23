# Arduino-ESP32-Nano-Word clock
Wordclock with an Arduino ESP32 Nano

The Arduino Nano word clocks from my older projects, https://github.com/ednieuw/Word-Colour-Clock-SK6812-WS2812, must have a DS3231 RTC and a Bluetooth module attached to get and keep the time.
The Arduino nao ESP32 can receive its time with WIFI from the internet and has an built-in RTC module.

The older Arduino MKR1000, Nano BLE 33 and all its variants with Bluetooth and WIFI has the disadvantage that only WIFI or BLE could be used. The ESP32 has an Espressif BLE/WIFI. BLE is not with the TI CC2541 chip but a Nordic nRF52 chip. <br>
That means you have to use a different BLE service. Not FFE0 but 6e400001-b5a3-... et cetera in your serial terminal app that is used to communicatie with the settings of the clock software.

See here: https://github.com/ednieuw/nRF-ESP32

![Nano-ESP32 Pinout](https://github.com/ednieuw/Arduino-ESP32-Nano-Wordclock/assets/12166816/8d2201ea-e34a-4734-9fc0-5480a702290c)
