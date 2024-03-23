# Arduino-ESP32-Nano-Word clock
# Under construction

Wordclock with an Arduino ESP32 Nano

The Arduino Nano word clocks from my older projects, https://github.com/ednieuw/Word-Colour-Clock-SK6812-WS2812, must have a DS3231 RTC and a Bluetooth module attached to get and keep the time.
The Arduino nao ESP32 can receive its time with WIFI from the internet and has an built-in RTC module.

The older Arduino MKR1000, Nano BLE 33 and all its variants with Bluetooth and WIFI has the disadvantage that only WIFI or BLE could be used. The ESP32 has an Espressif BLE/WIFI. BLE is not with the TI CC2541 chip but a Nordic nRF52 chip. <br>
That means you have to use a different BLE service. Not FFE0 but 6e400001-b5a3-... et cetera in your serial terminal app that is used to communicatie with the settings of the clock software.

See here: https://github.com/ednieuw/nRF-ESP32

Because the ESP32-S3 chip of the Nano has may more ports and other port numbering as the ATMEGA chips Arduino had to change the pin numbering in the software. 

There are two compiler pin numbering methods: One method uses the GPIO numbering of the ESP32-S3 or by Arduino pin numbering.
IMy choice was to use the Arduino pin numbering. But ... Then you have to use the Arduino macro numbering D1, D2, ... , D13  for digital pins and A0, A1, .. A7 for the analog pins. In the compiler settings the Arduino numbering is the default setting

if you choose 'by GPIO number' you can use the GPIO numbers as an integer value. 

![Nano-ESP32 Pinout](https://github.com/ednieuw/Arduino-ESP32-Nano-Wordclock/assets/12166816/8d2201ea-e34a-4734-9fc0-5480a702290c)
