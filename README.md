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

if you choose 'by GPIO number' you can use the GPIO numbers as an integer value as used for an ESP32-S3 or as Arduino macro definitions like the numbers printed on the Nano ESP32 board. Like: D1,D2,A2,A5. 

![Nano-ESP32 Pinout](https://github.com/ednieuw/Arduino-ESP32-Nano-Wordclock/assets/12166816/8d2201ea-e34a-4734-9fc0-5480a702290c)

For my clocks I use a in Fritzing designed PCB. The SK6812 RGBW LED-strips operates with 5V. De data signal from the Nano is 3.3V 
I tried to use optocouplers to amplify the data signal to 5V but failed. see here:
https://ednieuw.home.xs4all.nl/ElecProj/OptoSK6812/OptocouplerSK6812.html
You can also read about the use of the 74HCT125 level shifter with good results. It has four port and I designed the PCB you can use the other three port for other uses. But in the end the SK6812 RGBW strip, with 14 LEDs, also happily worked when the data line was connected to the 3.3V data line. I have not tested strips with more LEDs. 
The lesson of this story is that you can connect de SK6812 RBW with a 470 ohm resistor and a 200 - 1000 uF capacitor to the strip. 
But you need a separate 5V power supply for the LEDs. You can draw the power from the VBUS-pin on the Nano if the Nano is powered with a USB-C cable.  
VBUS provides 5V whenever powered via USB. If powered via the VIN pin, it is disabled. This means that while powering the board through the VIN pin, you can't get 5V from the board.

![Nano_ESP32-PCB_V02](https://github.com/ednieuw/Arduino-ESP32-Nano-Wordclock/assets/12166816/9b5c5c5a-e908-4ae9-8d86-66cb5d0ebd43)
