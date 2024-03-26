# Arduino-ESP32-Nano-Word clock

![image](https://github.com/ednieuw/Arduino-ESP32-Nano-Wordclock/assets/12166816/d5d450ce-df3c-4274-94ba-184792a7ee3d)

Wordclock with one or four languages with an Arduino ESP32 Nano.

The Arduino Nano word clocks from my older projects, https://github.com/ednieuw/Word-Colour-Clock-SK6812-WS2812, must have a DS3231 RTC and a Bluetooth module attached to get and keep the time.
The Arduino nao ESP32 can receive its time with WIFI from the internet and has an built-in RTC module.

The older Arduino MKR1000, Nano BLE 33 and all its variants with Bluetooth and WIFI has the disadvantage that only WIFI or BLE could be used. The ESP32 has an Espressif BLE/WIFI. BLE is not with the TI CC2541 chip but a Nordic nRF52 chip. <br>
That means you have to use a different BLE service. Not FFE0 but 6e400001-b5a3-... et cetera in your serial terminal app that is used to communicatie with the settings of the clock software.

See here: [Use BLE on mobile to control the ESP32](https://github.com/ednieuw/nRF-ESP32)

Because the ESP32-S3 chip of the Nano has may more ports and other port numbering as the ATMEGA chips Arduino had to change the pin numbering in the software. 

There are two compiler pin numbering methods: One method uses the GPIO numbering of the ESP32-S3 or by Arduino pin numbering.
IMy choice was to use the Arduino pin numbering. But ... Then you have to use the Arduino macro numbering D1, D2, ... , D13  for digital pins and A0, A1, .. A7 for the analog pins. In the compiler settings the Arduino numbering is the default setting

if you choose 'by GPIO number' you can use the GPIO numbers as an integer value as used for an ESP32-S3 or as Arduino macro definitions like the numbers printed on the Nano ESP32 board. Like: D1,D2,A2,A5. 

![Nano-ESP32 Pinout](https://github.com/ednieuw/Arduino-ESP32-Nano-Wordclock/assets/12166816/8d2201ea-e34a-4734-9fc0-5480a702290c)

For my clocks I use an in Fritzing designed PCB. 
The PCB can connect to a rotary encoder, a DS3231 RTC module or other I2C device, a DCF77-receiver module or other device that receives pulses and a LDR to measure light intensity to control the brightness of the LED-strip.

The SK6812 RGBW LED-strip operates at 5V. De data signal from the Nano is 3.3V. 

I tried to use optocouplers to amplify the data signal from 3.3V to 5V but failed. see here:
https://ednieuw.home.xs4all.nl/ElecProj/OptoSK6812/OptocouplerSK6812.html

You can also read about the use of the 74HCT125 level shifter with good results. It has four ports and I designed the PCB it can also use the other three ports on the IC for other uses. 

But in the end the SK6812 RGBW strip, with 14 LEDs, also happily worked when the data line was connected to the 3.3V data line. I have not tested strips with more LEDs in the strip. 

The lesson of this story is that you can connect the SK6812 RGBW strip with a 470 ohm resistor in the data line and a 200 - 1000 uF capacitor over the 5V and GND to the strip. 

![Nano_ESP32-PCB_V02](https://github.com/ednieuw/Arduino-ESP32-Nano-Wordclock/assets/12166816/9b5c5c5a-e908-4ae9-8d86-66cb5d0ebd43)

But you need a powerfull 5V power supply for the LEDs. Power can be drawn from the VBUS-pin (5V USB) on the Nano if the Nano is powered with a USB-C cable.  
VBUS provides 5V whenever powered via USB. If powered via the VIN pin, it is disabled. This means that while powering the board through the VIN pin, you can't get 5V from the board.

The PCB can use different power connection options.

If the blue line shortcut is closed then the LED-strip is powered via the Power 6-21V connection. This will also power the Nano ESP32. You can connect it to a 5V USB power supply allthought the minimum voltage is noted as 6V. 

BUT KEEP IN MIND. this connection is fed to the LED-strip. If a higher voltage power supply us connected the LED will be destroyed but Nano will turn on happily.

If the Nano ESP32 is connected via the USB-C port then the yellow connection must be shortcutted. The LEDs will be powered by thus power source and must be 1A or more with longer strips cq more LEDs.

A 5V >1A power supply can also be attached at the 5V power connection on the top left of the PCB and the Vin connection (red line) connected to 5V. 

![5VConnections](https://github.com/ednieuw/Arduino-ESP32-Nano-Wordclock/assets/12166816/39ea30fc-2850-4c1c-9fdb-4b4bec752f9a)

After connections are made the PCB and hardware looks like this

![image](https://github.com/ednieuw/Arduino-ESP32-Nano-Wordclock/assets/12166816/86c688e0-8006-4042-880a-40064a4d8949)


# Software

The ESP32Arduino_WordClockV006.ino or higher version turns the Nano ESP32 and SK6812 or WS2812 LED strips into a Wordclock with connection to your WIFI router and receives the time from a NTP server. You can operate the software via Bluetooth on your Android ot iPhone/iPad/iMac

You can choose between four languages or make a four-language clock that displays four languages at once with it 625 LEDs. If you have a word plate ofcourse. [Word clock with SK6812 LEDs in NL, DE, FR.](https://github.com/ednieuw/FourLanguageClock)
![Clockoptions](https://github.com/ednieuw/Arduino-ESP32-Nano-Wordclock/assets/12166816/f094e08d-e4f9-4a94-b4c3-6fea4be256ee)
With defines the software compiles for SK6812 or WS2812 LED-strips and you can choose between a Dutch or 4-languages clock.<br>
It is simple to change the clock to a single French, German or English design.

Or a ![single language clock](https://github.com/ednieuw/Woordklok-witte-LEDs)

In the libraries.zip I packed the libraries I used for V006. Unpack them in you libraries folder.<br>
Download the V006 folder and compile for ESP32<br>

How to compile: Install Arduino Nano ESP32 boards<br>
Board: Arduino Nano ESP32<br>
Partition Scheme: With FAT<br>
Pin Numbering: By GPIO number (legacy)<br>

![SerialOutputs](https://github.com/ednieuw/Arduino-ESP32-Nano-Wordclock/assets/12166816/6b1e48c3-783a-4b4e-bc32-eaa1fe344297)





