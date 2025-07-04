



# Arduino-ESP32-Nano-Word clock

![image](https://github.com/ednieuw/Arduino-ESP32-Nano-Wordclock/assets/12166816/d5d450ce-df3c-4274-94ba-184792a7ee3d)

Built your own word clock with one or four languages with an Arduino ESP32 Nano.<br>
[Use the word clock selector page to compare other software/hardware designs to make.](https://github.com/ednieuw/WordclockSelector)<br>

The clock can be controlled with an app on your phone, a web page in a browser or with a serial cable connected to your PC, a rotary encoder or membrane swiches.<br>
<img src="https://github.com/user-attachments/assets/0b566154-2bc8-415f-927c-ea496154f767" style="width:50%;">

To connect the clock to WIFI one can enter the SSID and password of the router or use the WPS function.<br>
Software updates can be uploaded with OTA (Over the Air).<br>

It will keep its time within the second correct with the internet Network Time Protocol (NTP) time with time zone and day light saving corrections.<br>

If no internet is available a DS3231 RTC-module can be attached to the PCB to get an accurate time. With a rotary or membrane pad time can be set.<br>
A LDR (light-dependent resistor) is used to give the LEDs an optimal brightness.<br>

The older Arduino MKR1000, Nano BLE 33 and all its variants with Bluetooth and WIFI has the disadvantage that only WIFI or BLE could be used.

The ESP32 has an Espressif BLE/WIFI module. Bluetooth LE does not use the TI CC2541 chip but a Nordic nRF52 chip.<br>
That means you have to use a different BLE service for the serial communication. 
Not with characteristic FFE0 but 6e400001-b5a3-... et cetera in your serial terminal app that is used to communicate with the settings of the clock software.<br>

See here: [Use BLE on mobile to control the ESP32](https://github.com/ednieuw/nRF-ESP32)

![image](https://github.com/user-attachments/assets/f187786c-b6db-49a8-9e91-708b93361390)
Clock with Corten steel face

# Hardware
For my projects I designed the printed circuit board (PCB) with [Fritzing software](https://fritzing.org/).

This Fritzing program is easy to use and it can export Gerber files that can be send to companies that print PCB's. You can also order a PCB with the Fritzing app itself.<br>
[PCBWay](https://www.pcbway.com/) print 10 PCB's for $5. With +/-$25 shipping and custom charges this is very cheap. The PCB were received within a week after ordering and the quality was excellent. Just upload the Gerber files in the ZIP-file and pay the charges.<br>  

The [PCB design .FZZ-file and Gerber files](PCB) can be found in the folder PCB in this repository or here:  [ESP32 PCB](https://github.com/ednieuw/NanoESP32PCB)

The PCB can connect to a rotary encoder or three button membrane switch, a DS3231 RTC module or other I2C device, a DCF77-receiver module or other device that receives pulses and a LDR to measure light intensity to control the brightness of the LED-strip.

The SK6812 RGBW LED-strip operates at 5V. But the data signal from the Nano is only 3.3V.<br> 
I tried to use optocouplers to amplify the data signal from 3.3V to 5V but failed. see here: 
https://ednieuw.home.xs4all.nl/ElecProj/OptoSK6812/OptocouplerSK6812.html<br>
For level switching the 74HCT125 level shifter is used. It has four ports and I adapted the design of the PCB it can use the other three ports on the IC for other uses. <br>
But the SK6812 RGBW strip, with 14 LEDs, also happily worked when the data line was connected directly to the 3.3V data line of the Nano ESP32. I have not tested strips with more LEDs in the strip.<br>
The lesson of this story is that you can connect the SK6812 RGBW strip with a 470 ohm resistor in the data line and a 200 - 1000 uF capacitor over the 5V and GND directly to the strip without the use of a level shifter.  But success is not guaranteed.

![Nano-ESP32-V04](https://github.com/user-attachments/assets/3ad06452-0dd4-4310-8b51-25c115ec3959)

An alternative universal PCB to drive RGB(W) LEDs and /or white 2835 LED strips with shift registers can be found here: [Nano  ESP32 RGBW BW PCB](https://github.com/ednieuw/NanoESP32-BW-RGBW-clock) <br>
This PCB, with one Arduino source code, will support the colour LED clocks and the white LED clocks to run with a Arduino Nano ES32. 

![V01SmallNanoESP32-3D](https://github.com/user-attachments/assets/fb3863f4-1346-4985-a541-f087be7e8b68)
The design of a [minimum PCB for small spaces](/PCB-small) can also be found in the folder PCB-small in this repository.

Power for the LED-strip can be drawn from the VBUS-pin (5V USB) on the Nano if powered with a USB-C cable.  
VBUS provides 5V whenever powered via USB. It is possible to feed the LED-strip via this connection but keep in mind the power travels through the PCB and USB-C power supplies are able to supply over 20 A. This will probably melt the small copper lines on the PCB.<br> 
If powered via the VIN pin VBUS it is disabled. This means that while powering the board through the VIN pin, you can't get 5V power from the board. 

The PCB can use different power connection options.
A 5V, >1A power supply can be attached at the 5V power connection on the top left of the PCB.<br>
If the red line shortcut (see picture below) is closed then the LED-strip is powered via the Power 6-21V (VIN) connection with 5V.<br> 
This will also power the Nano ESP32. You can connect the Vin on the ESP32 Nano with a 5V USB power supply although the minimum voltage is noted as 6V. the 1N5817 diode even lowers the voltage with 0.5V to 4.5V.<br> 
BUT KEEP IN MIND. This 5V connection is fed to the LED-strip directly. If a higher voltage power supply connected the LEDs will be destroyed but Nano will turn on happily. <br>
If the Nano ESP32 is connected via the USB-C port then the yellow connection must be shortcut. The LEDs will be powered by this power source. This is fine with 20 LEDs but probably not for longer strips.

![5VConnections](https://github.com/ednieuw/Arduino-ESP32-Nano-Wordclock/assets/12166816/39ea30fc-2850-4c1c-9fdb-4b4bec752f9a)

After connections are made the PCB and hardware looks like this:

![image](https://github.com/ednieuw/Arduino-ESP32-Nano-Wordclock/assets/12166816/86c688e0-8006-4042-880a-40064a4d8949)
![NanoESP32-V04-3D](https://github.com/user-attachments/assets/e717d61f-7953-4e35-80c1-278ca05a10df)

![image](https://github.com/user-attachments/assets/77fc651e-cfd6-4681-83e5-53c4f6bcfb55)


# PCB Connections

The large PCB has connections to: (from top right clock wise)
- 6-21V power input to Vin on Nano ESP32
- DCF to pin D2, 3V3 and GND
- LDR to pin A2 and 3V3
- RTC-clock to pin  GND, 3V3, A4 and A5
- Rotary encoder or 3-button switch to pin D8, D3, D4 3V3 and GND
- LEDs to pin D10 and D9
- Connector to pin D12, D11, D10 and D9
- D7 with 5V via 3V3 -> 5V level switcher
- Connector with GND and 3V3
- Connector for AMS1117 5V -> 3V3 1A voltage regulator.
- D6 with 5V via 3V3 -> 5V level switcher
- Connector for LED-strip SK6812/WS2812 to 5V, D5 and GND
- D3 with 3V3 to D3 and GND
- 5V power input to 5V-USB or Vin on ESP32 and to LED-strip  
- 5V and GND connector and connector to Vin and 5V-USB on Arduino Nano ESP32
- Connector to GND, RX and TX 

The small PCB has connections to: (from top left clock wise)

- LDR to pin A2 and 3V3
- 5V power input to to 5V-USB on ESP32 and to LED-strip
- Connector for LED-strip SK6812/WS2812 to 5V, D5 and GND
- Rotary encoder or 3-button switch to pin D8, D3, D4 3V3 and GND
- RTC-clock to pin  GND, 3V3, A4 and A5

![image](https://github.com/user-attachments/assets/f2919fb4-b571-4a1d-8264-4e227d4b104e) 

# The LED ground plate

![IMG_1469](https://github.com/user-attachments/assets/a6a80ac1-4cb6-47b2-a496-f9645e8795c1)

The distance between the LEDs on the strip is suitable for making a clock of 25 x 25 cm (60 LEDs/m) or 50 x 50 cm (30 LEDs/m).<br> 
You can choose to stick the 144 LEDs of the clock in 12 rows of 12 LEDs or only behind the letter that should light up.<br>
The advantage of the latter is that fewer LEDs are needed and that you have to drill fewer holes in the spacer plate.<br>
But there are many more soldering points, each of which can cause malfunctions.<br>  
My experience is that imperfect soldering on the strip may loosens over time causing malfunctions. 

The software has a digital time display option you cannot use if you do not install all 144 LEDs.<br>
You can choose to cut out the words like IT WAS FIVE and so on in a 1 cm thick MDF board with a jigsaw.<br>
Be sure to paint the insides bright white, otherwise the white light will become dingy.<br> 
It is better to use foamed PVC white 10 MM RAL 9003. It cuts easily, is white and does not discolour. (Often paint does discolours)
 
Stick/glue the strips starting from left to right on the odd lines and from right to left on the even lines. <br>
### Follow the arrows on the strip.<br> 
(You have to turn the strip upside down every second row!)<br>
This avoids long data lines going from the right end of the strip to the left end on the next row of a LED strip.<br>
And the software does not have to be corrected for the new postions.

![IMG_5384](https://github.com/user-attachments/assets/945ba89a-6fda-43fe-b16b-52f5babf0584)

I find it useful to use tinned copper wire.<br> 
For example, you solder all 5V connections to the left side of the strip and all GND to the right side of the strip.
To do this, drill a small hole through the plate next to all 5V connections on the left side of the strip and to the GND on the right side

Bend a 90 degree angle, put the wire through the hole and solder the wire to the LED strip.

![IMG_5467](https://github.com/user-attachments/assets/cb3148c9-6efb-4a4c-b04a-4ceb46584de6)

![IMG_1471](https://github.com/user-attachments/assets/fccbe36d-321e-4bd3-af34-782082c0a97e)

Place the spacer plate over de LEDs, then a sheet of white paper and finally the word plate.

# Software

The ESP32Arduino_WordClockVxxx.ino turns the Nano ESP32 and SK6812 or WS2812 LED strips into a Word clock with connection to your WIFI router and receives the time from a NTP server.

The software can be controlled with Bluetooth on your PC or a mobile Android or iPhone/iPad/iMac.
Several designs can be selected before compiling.  (If you have a word plate of course.)
Use a word plate design [from this repository](WordPlateFrontCovers)  or use your own and adapt the position of the LEDs in the software<br>.

![ClockDefines](https://github.com/user-attachments/assets/89a48534-c97a-41fc-94a5-bb8ee35ebb34)

Select one of the three word clocks
```
//#define FOURLANGUAGECLOCK
#define NL144CLOCK
//#define NLM1M2M3M4L94          // NL clock with four extra LEDs for the minutes to light up

```

- #define NL144CLOCK -> a 144 LED single language clock. Default language is Dutch. For French, German and English copy the coding from the four-language clock between the NL144CLOCK defines.<br>
 [Build instruction for the clock in Dutch and English](https://ednieuw.home.xs4all.nl/Woordklok/Bouwpakket/WoordklokSK6812.htm)
or [here in this repository](Manual-Instructions)

- #define NLM1M2M3M4L94 -> a 144 LED single language clock with 4 extra LEDs for the minutes and a slightly different design

- #define FOURLANGUAGECLOCK-> a 4-language clock with 625 LEDs in a 25 x 25 grid.<br>
[Build instruction of 4-language word clock with SK6812 LEDs in UK, NL, DE, FR with Nano Every.](https://github.com/ednieuw/FourLanguageClock). 

In the libraries.zip are the libraries to compile the software. Unpack them in you libraries folder.<br>
Download the program folder and compile for Arduino Nano ESP32.<br>
Libraries update constantly what may result in incompatibility with this source code after a few years. Therefore these libraries are included.

An alternative is to compile the ArduinoOTA sketch that can be found in the Examples of the Arduino IDE.<br>
Upload the OTA-updater in the Nano ESP32 and note the IP-address that is printed in the Serial monitor of the Arduino IDE.
Type this IP-address URL in a browser and enter admin/admin as user and password.<br>
Then upload the .bin file from this repository.<br>
<br>
Or use this one: [Uploads .bin files in the ESP32 by using wifimanager](https://github.com/ednieuw/OTA-webupdater).<br>
<br>
# How to compile: 
At the moment of writing ( june 2025) the Espressif ESP32 board core V3.2. with the Arduino Nano ESP32 selected does compile to a working program when ESP32WordclockV100.ino of a higher version is used.
Select the Nano ESP32 board from Arduino. 
The Arduino ESP32 board with core version 2.0.17/2.0.18, compiles without errors and is advised to use. 
It generates the smallest binary code (V2.0.18 = 1.0 Mb vs V3.20 = 1.3 Mb)

![image](https://github.com/user-attachments/assets/1f4eeec5-ebc5-47fb-b513-2514f5ff6ecd)

There are two compiler pin numbering methods: One method uses the GPIO numbering of the ESP32-S3 or by Arduino pin numbering.<br>
This code must be compiled with GPIO numbering selected!! <br>
If LEDs do not turn on properly check this GPIO numbering setting.<br>
When the Arduino macro numbering D1, D2, ... , D13 is used for digital pins and A0, A1, .. A7 for the analogue pins and LED_BUILTIN, LED_RED LED_GREEN, LED_BLUE for the LEDs on the Nano ESP32 board 'GPIO numbering' must be selected in the compiler.

![Nano-ESP32 Pinout](https://github.com/ednieuw/Arduino-ESP32-Nano-Wordclock/assets/12166816/8d2201ea-e34a-4734-9fc0-5480a702290c)

Install Arduino Nano ESP32 board<br>
Board: Arduino Nano ESP32<br>
Partition Scheme: With FAT<br>
<strong> Pin Numbering: By GPIO number (legacy)  !! change this </strong><br>
The LEDs will probably not turn on when you use the "Arduino default pin settings" due to a bug in the Adafruit Neopixel library.<br>

![SerialOutputs](https://github.com/ednieuw/Arduino-ESP32-Nano-Wordclock/assets/12166816/6b1e48c3-783a-4b4e-bc32-eaa1fe344297)

# Before starting

The clock receives time from the internet if a DS3231 time module is not installed. 

When a DS3231 time module is attached to the circuit board an internet connection is not obliged. Select in the menu that the DS3231 is used instead of the NTP time. (J Toggle use DS3231 RTC module)

To connect to the internet the name of the WIFI station and its password must be entered in the clock software to be able to connect to a WIFI router.

### Connect to WIFI
The name of the WIFI-station and password has to be entered once. These credentials will be stored in memory of the microprocessor.

If the clock software is started without a SSID it will start a WIFI station you can connect to. 
- Open in your phone, tablet or PC the WIFI connections. In the list of WIFI stations there will be one named: StartWordClock. 
- Connect to it and enter the password: 12345678
- Enter the SSID and password of your WIFI router. You can find these credentials ofter at the bottom of your router.
- When Submit is pressed the clock will restart and connect to the internet

### Connect via Bluetooth
To make life easy it is preferred to use a phone or tablet and a Bluetooth communication app to enter the WIFI credentials into the clock.
![image](https://github.com/ednieuw/Arduino-ESP32-Nano-Wordclock/assets/12166816/261800f1-7cd6-4078-8c7e-ad9cd2ba47ec)
 	 	 
BLESerial nRF	BLE Serial Pro	Serial Bluetooth Terminal
- Download a Bluetooth UART serial terminal app on your phone, PC, or tablet.<br>

- For IOS iPhone or iPad: [BLE Serial Pro](https://apps.apple.com/nl/app/ble-serial-pro/id1632245655?l=en). <br>
Or the free less sophisticated app: [BLE serial nRF](https://apps.apple.com/nl/app/bleserial-nrf/id1632235163?l).<br>
Tip: Turn on Fast BLE with option + in the menu for a faster transmission.

- For Android use: [Serial Bluetooth terminal](https://play.google.com/store/apps/details?id=de.kai_morich.serial_bluetooth_terminal). <br>
Do not turn on Fast BLE in the menu. (Off = default)

Enter the SSID name and password of your router prefixed with A for the SSID and with an B for the password.
- aSSIDname and press Send
- bPassword and press send
Optionally
- cClockname and press send. 
Clockname is the name of the clock displayed in the WIFI-router in the the list of bluetooth devices in the BLE serial terminal app


# Control of the clock
If the clock is connected to the internet it will seek contact with a time server. (NTP connection can be turned off in the menu).

The time zone is set to UTC+1 Amsterdam but can be changed in the menu. <br>
To connect to a WIFI network a SSID and password must be entered. 
 
There are a few methods: <br>
1. Connect the MCU with a serial cable to a PC and use a serial terminal.

I use the Arduino IDE as serial terminal. 

Sending the character 'I' for information will display the menu followed with the actual settings of several preferences. 

<table style="width: 51%"><tr>
<td style="width: 342px"><img alt="Menu on iPhone" src="Pics/menuphone.jpg" width="350"  /></td>
<td style="width: 415px"><img alt="Termite terminal" src="Pics/Termite.jpg" width="400"  /></td>
</tr>

<tr>
<td style="width: 342px">HTML page on iPhone.</td>
<td style="width: 415px">Termite terminal on a PC. </td>
</tr>
</table>
<br>

2. USE the BLE nRF connection with an UART serial terminal app to control it with your mobile phone or tablet. 

Use the IOS app for iPhone or iPad: <a href="https://apps.apple.com/nl/app/ble-serial-pro/id1632245655?l=en">BLE Serial Pro</a>. <br>
or the free app BLEserial nRF: https://apps.apple.com/nl/app/bleserial-nrf/id1632235163?l=en-GB
Turn on Fast BLE with option Z in the menu. 

For Android use:<a href="https://play.google.com/store/apps/details?id=de.kai_morich.serial_bluetooth_terminal">Serial Bluetooth terminal</a>. <br>
Turn off (default) Fast BLE in the menu. 

Start the app and find the MCU in the list of devices and connect to it. You can change it's beacon name in the menu with option C. 

In both cases send the character I of Information and the menu shows up.<br>
Enter the first character of the setting you want to change followed with a code. <br>
Some entries just toggle On and Off. Like the W to set WIFI Off or On. 
  
<p> To change the SSID and password: <br>
Amy-ssid and send this command. Eg AFRITZ!Box01 or aFRITZ!Box01. 
Starting with an upper or lower case character is an identical instruction in the command string <br>
Then Bmy-passwordand send the password.(for example: bSecret<br>
 
Cbroadcastname will change to name displayed in the Bluetooth connection list. <br>
If the length of the SSID and/or password is less then 5 characters the WIFI will be turned off automatically. This will speed up startup time if no internet connection is available <br>
Use a length of minimal 8 characters for SSID and password. <br>
Check in the menu (third row from the bottom) if WIFI and NTP are on. <br>
If WIFI is connected the LED on the MCU will pulse green. <br>
<br> Enter @ to reset the MCU. It will restart and connections will be made. <br>
<br>
To set a time zone. Send the time zone string between the quotes prefixed with the character E or e. <br>
See the time zones at the bottom of this page. <br>
For example; if you live in Australia/Sydney send the string: eAEST-10AEDT,M10.1.0,M4.1.0/3 

<pre>
___________________________________
A SSID B Password C BLE beacon name
D Date (D15012021) T Time (T132145)
E Timezone  (E<-02>2 or E<+01>-1)
F Own colour  (Hex FWWRRGGBB)
G Scan WIFI networks
H Toggle use rotary encoder
I To print this Info menu
J Toggle use DS3231 RTC module
K LDR reads/sec toggle On/Off
N Display off between Nhhhh (N2208)
O Display toggle On/Off
P Status LED toggle On/Off
Q Display colour choice
  Q0 Yellow  Q1 Hourly  Q2 White
  Q3 All Own Q4 Own     Q5 Wheel
  Q6 Digital
R Reset settings @ = Reset MCU
U Demo mode (msec) (U200)
--Light intensity settings (1-250)--
S Slope, L Min, M Max  (S80 L5 M200)
W WIFI, X NTP&, CCC BLE, + Fast BLE
# Self test, ! See RTC, & Update RTC
Ed Nieuwenhuys November 2024
___________________________________
Display off between: 23h - 08h
Display choice: Yellow  
Slope: 10     Min: 5     Max: 255 
SSID: FRITZ!Box
BLE name: wordclock
IP-address: 192.168.178.141 (/update)
Timezone:CET-1CEST,M3.5.0,M10.5.0/3
WIFI=On NTP=On BLE=On FastBLE=Off
LED strip: WS2812 (Send % to switch)
Software: ESP32Arduino_WordClockV056.ino
ESP32 Arduino core version: 2.0.17
17/11/2024 16:01:22 
___________________________________
</pre>

Menu shown in serial output.

As mentioned before the clock can be controlled with the WIFI webpage or BLE UART terminal app. <br>
When the clock is connected to WIFI the IP-address is displayed in the menu<br>

Enter this IP-address numbers and dots (for example: 192.168.178.31) in the browser of your mobile or PC where you type 
your internet addresses (URL). 

Or in version V056 or higher the name of the BLE beacon can be used followed with .local

If the name of your BLE beacon is wordclock ( see in the menu -> BLE name: wordclock)
Enter 'wordclock.local' as URL in the browser. 
 if this does not work install host software:
  - For Linux, install Avahi (http://avahi.org/).
  - For Windows, install Bonjour (http://www.apple.com/support/bonjour/).
  - For Mac OSX and iOS support is built in through Bonjour already.

  - Point your browser to http://wordclock.local, you should see a response.

Or 
Open the BLE terminal app. Look for the WordClock to connect to and connect.

for Apple IOS devices BLE connection can be made with my app <a href="https://ednieuw.home.xs4all.nl/BLESerial/BLESerialPRO.html"> BLE Serial pro </a> on the <a href="https://apps.apple.com/nl/app/ble-serial-pro/id1632245655?l=en">app store</a> .<br>

For Android <a href="https://play.google.com/store/apps/details?id=com.nordicsemi.nrfUARTv2&amp;hl=en&amp;gl=US"> 
nRF UART terminal program </a>and <a href="https://play.google.com/store/apps/details?id=de.kai_morich.serial_bluetooth_terminal">
Serial Bluetooth terminal</a>. <br>
Unfortunately these Android apps can not read strings longer than 20 characters. <br>
If you see a garbled menu enter and send the character '+' to select the slower transmission mode. 
 
Settings are set by entering the first character of a command following by parameters if necessary.<br>
For example to set the colours of the characters in the display to white enter: Q2 <br>
<br>
 Set time by entering T130245. (130245 will also work) <br>
<br>
 Turn off WIFI by sending a W. 
<p >Reset the MCU with the character @.<br>
Reset to default setting by send R.<br><br>
 In the BLE connection the SSID and password will be shown.  <br>
	
<table cellspacing="6" style="width: auto">
<tr>
<td style="width: 318px">
<img alt="Phone" src="Pics/IMG_9449.jpg" width="350" class="auto-style4" /></td>
<td>
<img alt="WIFI-BLE" src="Pics/IMG_9448.jpg" width="350" class="auto-style4" /></td>
</tr>
<tr>
<td style="width: 318px" >HTML page</td>
<td >BLE menu</td>
</tr>
</table>

## Detailed description
<pre>
	
</pre>

With the menu many preferences can be set. <br>
These preferences are permanently stored in the Arduino Nano ESP32 storage space. 

Enter the first character in the menu of the item to be changed followed with the parameter. <br>
For most entries upper and lower case are identical. 

## A SSID B Password C BLE beacon name<br>
Change the name of the SSID of the router to be connected to.<br>
aFRITZ!Box or AFRITZ!Box<br>
Then enter the password. For example: BSecret_pass <br>
and cMywordclock as a name of the BLE beacon that will be shown in your phone. (default after a reset:  wordclock )

Restart the MCU by sending @. <br>
## CCC  
Entering CCC or ccc will toggle BLE on or off. Be careful turning it off. When BLE is off the clock can only be controlled with WIFI or the USB serial port.

## D Set Date and T Set Time <br>
If not connected to WIFI time and a RTC DS3231 is attached date must be set by hand.<br>
For example enter: D06112022 to set the date to 6 November 2022.  <br>
Enter for example T132145 (or 132145 , or t132145) to set time to 45 seconds and 21 minutes past one o'clock.

## E Set Timezone E&lt;-02&gt;2 or E&lt;+01&gt;-1<br>
At the bottom of this page you can find the time zones used in 2022. <br>
It is a rather complicated string and it is therefore wise to copy it. <br>
Let's pick one if you happen to live here: Antarctica/Troll,"&lt;+00&gt;0&lt;+02&gt;-2,M3.5.0/1,M10.5.0/3" <br>
Copy the string between the " " 's and send it starting with an 'E' or 'e' in front. <br>
E&lt;+00&gt;0&lt;+02&gt;-2,M3.5.0/1,M10.5.0/3 

## F Own colour  (Hex FWWRRGGBB <br>
With option Q3 and Q4 from the menu you can set your own colours for the clock to display.
The format to be entered is hexadecimal. 0123456789ABCDEF are the characters that can be used. <br>
The command is 2 digits for white followed with two digits for Red followed with two digits for Green and ending with two digits for Blue.<br>
To colour the characters intense red enter FF0000 prefixed with the character F. <br>
To set intense blue enter: F0000FF or FFF.<br>
To set the dimmed character to dark gray enter for example: F191919. <br>
You get gray if red, green and blue has the same intensity.
With SK6812 LEDs the extra white LED can be used besides the three RGB LEDs in the same housing.
For example: F8800FF00 is 50% white with 100% green.

## I To print this Info menu<br>
Print the menu to Bluetooth and the serial monitor when connected with an USB-cable. 

## K LDR reads/sec toggle On/Off<br>
Prints the LDR-readings and the calculated intensity output.

## N Display off between Nhhhh (N2208)<br>
With N2208 the display will be turned off between 22:00 and 08:00. 

## O Display toggle On/Off<br>
 O toggles the display off and on.
 
##  Q Display colour choice (Q0-6)<br>
Q0 Yellow Q1 hourly Q2 White Q3 All Own Q4 Own Q5 Wheel Q6 Digital display <br>
Q0 will show the time with yellow words. <br>
Q1 will show every hour another colour. <br>
Q2 shows all the texts white. <br>
Q3 and Q4 uses you own defined colours. <br>
Q5 will follow rainbow colours every hour. <br>
Q6 is the digital clock if you have used 12x12 = 144 LEDs in the clock <br>
Send an 'I' to display the latest's settings 

## R Reset settings <br>
R will set all preferences to default settings. The SSID and password, timezone, the LED-strip type and use a rotary will be kept.<br>
RRR will clear SSID, password, set Time zone to Amsterdam (CET-1) and turn WIFI, NTP and BLE on.<br>
RRRRR is a total reset

## U Demo mode (msec) (U200)
Enter U followed with the duration of a second in milli seconds. 
M200 (200 milli second) will speed up the clock 5 times.  
Sending an U will turn off the demo mode.

## Y Play lights
To test the functionality of the LEDs

## Light intensity (1-250)
S=Slope L=Min M=Max (S100 L5 M200)

<table style="width: 100%">
<tr>
<td> To control the light intensity three parameters can be set. <br>
The values ranges between 0 and 255. <br>
The minimal light intensity is to avoid that the display turns completely off when it is dark. <br>
It also prevents flickering when the input value are very low, like 2 and 3 bits. <br>
Values ranging between 5 and 20 are suggested. When no LDR is attached the Min value can be set to 255.<br>
The maximum light intensity is 255 bits for 100% intensity.  <br>
The Slope (sensitivity) controls the speed at with the maximum value is reached. <br>
It multiplies the reading as a percentage. So, entering 100 is a multiplication by one. <br>
The placement and sensitivity of your LDR can be adjusted with this parameter Slope. </td>
<td> 
<img alt="Min Max Slope" src="Pics/MinMaxSlope.jpg" /><br>
The effect of the value of the slope on the light intensity of the LEDs 
or display. </td>
</tr>
</table>

<table style="width: 100%">
<tr>
<td> 



# @ = Restart MCU<br>
@ will restart the MCU. This is handy when the SSID, password, et cetera are changed and the program must be restarted.<br>
<br>
W=WIFI, X=NTP, Y=BLE<br>
Toggle WIFI, NTP and BLE on and off.<br>
Sending the character will toggle it on or off. <br>
At the bottom of the menu the state is printed.<br>
 </td>
<td>
<img alt="Bottom menu" class="auto-style4" src="Pics/BottomMenu.gif" /></td>
</tr>
</table>

## + Fast BLE

The BLE UART protocol sends default packets of 20 bytes. Between every packet there is a delay of 50 msec.<br>
The IOS BLEserial app, and maybe others too, is able to receive packets of 80 bytes or more before characters are missed.<br>With most apps you will see these will truncate the long strings of the menu.<br>
Then turn off Fast BLE. <br>
Option Z toggles between the long and short packages. 
Settings are stored in the SPIFFS space from the Arduino Nano ESP32

## ! = Show NTP, RTC and DS3231 time
!  will display the NTP, RTC and DS3231 time as they are stored in the clock  in the clock. The DS3231 time module must be installed and being used to show a realistic time.
Same as & option but this option will not update from the internet NTP server but only shows the time.

## #= Self test
Sending a # will start the clock self test. This is convenient to check if all the words in the display are functioning.<br> 
The time of a minute is reduced to 0.9 seconds (900 milli seconds).<br>
#nnnn were nnnn is the delay between minutes in milli seconds.
  
## % = Fire display
Shows a fire on the display in some clocks with a 12 x12 LED-matrix design installed

## % = Switch between SK6812 and WS2812 LED strip
With this option the used LED strip can be changed. The clock is equipped with on of these to types of LED strips. 
A Reset of all settings by sending a R in the menu will not change this LED strip selection.

## @ = Reset MCU
@ will restart the MCU. This is handy when the SSID, et cetera are changed and the program must be restarted. Settings will not be deleted.

## & = Get and stores NTP time in RTC and DS3231 time
&  will get the NTP time immediately from the internet and stores it in the RTC clocks. This option is convenient to force the clock to get the proper NTP time. 
In other cases the program will check the time running in the clock and on the NTP server so now and then and update the RTC clocks.
The DS3231 time module must be installed and being used to show a realistic time.

## 123456 Set time in RTC module
Enter the time as 152300 hhmmss. Same as T152300
Changing date and time only works when a DS3231 RTC module is attached.

# Compilation and uploading

The settings of the Arduino Nano ESP32 board is as follows.<br>
Remember to install the ESP32 boards as explained above.<br>

<img alt="Board settings" height="497" src="https://github.com/ednieuw/ESP32-C3-Clock-and-more/P/img6.gif" width="488" />
<p>

<p># Program explanation
 The program uses the following standard libraries.   
<br>
<pre>// Includes defines and initialisations 

#include <Preferences.h>
                      #if ESP_ARDUINO_VERSION >= ESP_ARDUINO_VERSION_VAL(3, 0, 0)
#include <EdSoftLED.h>         // https://github.com/ednieuw/EdSoftLED for LED strip WS2812 or SK6812 
                      #else
#include <Adafruit_NeoPixel.h> // https://github.com/adafruit/Adafruit_NeoPixel   for LED strip WS2812 or SK6812
                      #endif
#include <NimBLEDevice.h>      // For BLE communication. !!!Use NimBLE version 2.x.x  https://github.com/h2zero/NimBLE-Arduino
#include <ESPNtpClient.h>      // https://github.com/gmag11/ESPNtpClient
#include <WiFi.h>              // Used for web page 
#include <AsyncTCP.h>          // Used for webpage                                                         old https://github.com/me-no-dev/ESPAsyncWebServer
#include <ESPAsyncWebServer.h> // Used for webpage  https://github.com/mathieucarbou/ESPAsyncWebServer     Old original one https://github.com/me-no-dev/ESPAsyncWebServer
#include <ElegantOTA.h>        // If a large bunch of compile error see here :https://docs.elegantota.pro/async-mode
                               // Locate the ELEGANTOTA_USE_ASYNC_WEBSERVER macro in the ElegantOTA.h file, and set it to 1:
                               // #define ELEGANTOTA_USE_ASYNC_WEBSERVER 1
#include <ESPmDNS.h>           // To show BLEname in router
#include <DNSServer.h>         // For the web page to enter SSID and password of the WIFI router 
#include <Wire.h>              // Ter zijner tijd Wire functies gaan gebruiken. Staan al klaar in de code 
#include <RTClib.h>            // Used for connected DS3231 RTC // Reference https://adafruit.github.io/RTClib/html/class_r_t_c___d_s3231.html
#include <Encoder.h>           // For rotary encoder
#include <Keypad.h>            // For 3x1 membrane keypad instead of rotary encoder by Mark Stanley & Alexander Brevig 

</pre>

Webpage.h is included in the program as a TAB in the IDE. It contains the web page to display in the browser.<br>

I made the web page in the free 'Microsoft Expression Web 4'. It is not maintained anymore but has more than enough functionalities for our purposes.<br>
<br>
To copy the code from the MS-Expression: <br>
At the bottom line of the window of MS-Expression click 'Split'.<br>
In the upper half the raw HTML Code is displayed and in the bottom half the 
Design window.<br>
Copy in the Code window all the HTML code <br>
Go to the Arduino IDE and paste it in the webpage.h TAB between: <br>
R"rawliteral(   ... and ...
 )rawliteral";<br>
<br>
  Or copy the the code from the webpage.h into 
MSexpression Code area and redesign the page as you like 

<img alt="MS expression" class="auto-style4" src="Pics/MSExpression.jpg" /><br>

A long list if definitions and initialisations follows. <br>
I am not a fan of passing all the variables to and from functions and like to keep them global in one program list.<br>
If you write a program with other people it is good practice not to use too many globals but this program is in one large listing, for the same reason to keep it simple.<br>
I grouped all the variables per application to keep track where they are used.<br>
With a simple find it is easy in this one great listing to find the back.<br>
<br>
To print the time as text and colour with the proper LEDs or characters, the words and its position in a string of LEDs or text are defined. <br>
The defines executes the function ColorLeds with its proper parameters.<br>
Further in the program in the function void Dutch(void), void English(void) et cetera it becomes clear why these defines are so useful and handy.  

<pre> ...   
#define PRECIES ColorLeds("precies", 16, 22, LetterColor);
#define MTIEN   ColorLeds("tien", 25, 28, LetterColor); 
#define KWART   ColorLeds("kwart", 32, 36, LetterColor); 
#define VOOR    ColorLeds("voor", 38, 41, LetterColor); 
... 
</pre>
This is the initialisation of the storage area to store the struct EEPROMstorage.<br>
The Struct with all its settings is saved in one command to permanent memory or SD <br>
Preferences FLASHSTOR; <br>
<br>
<pre> ...  
struct EEPROMstorage { // Data storage in EEPROM to maintain them after power loss <br>
byte DisplayChoice = 0; <br>
... <br>
char BLEbroadcastName[30]; // Name of the BLE beacon <br>
char Timezone[50]; <br>
int Checksum = 0; <br>
} Mem; <br>
... 
</pre>
<br>
The variables are addressed with a short name Mem.<br> 
For example Mem.DisplayChoice = 3; <br>
<br>
The Struct EEPROMstorage is stored in the function StoreStructInFlashMemory and retrieved in the function GetStructFromFlashMemory <br>
<br>
<br><br>
The menu displayed in the serial monitor and BLE app is defined here.<br>
Strings may not be longer than 40 characters what can be checked with the 'ruler' string. <br>

<pre>
// Menu 
//0        1         2         3         4 
//1234567890123456789012345678901234567890  
char menu[][40] = { 
"A SSID B Password C BLE beacon name", 
"D Date (D15012021) T Time (T132145)", 
... 
"W=WIFI, X=NTP, Y=BLE, Z=Fast BLE",  
"Nov 2022" }; </pre>
 The Setup happens here: 
<pre>
//-------------------------------------------- 
// ARDUINO Setup
//-------------------------------------------- 
void setup()  
{
Serial.begin(115200); Tekstprintln("Serial started"); // Setup the serial port to 115200 baud // 
...
... 
msTick = millis();  
}
</pre>

The loop is kept almost empty and the program starts in Every SecondCheck.<br>
There is nothing in this program that should be executes every millisecond or less
 
<pre>
//-------------------------------------------- 
// ARDUINO Loop
//-------------------------------------------- 
void loop() 
{
 Loopcounter++;
 if (Demo)         Demomode();
 else              EverySecondCheck();                 // Let the second led tick and run the clock program
 CheckDevices();
} 
</pre>

The following routines check if something must happen every second, minute, hour and day.<br>
This flow handling of the program keeps the processor for 99% free for other uses.<br>
In this program that is almost nothing but for other purposes this can be needed.<br>
In the serial monitor the loops per second are printed. this can be handy to check if the program spends too much time elsewhere in the program. At the moment (V056) of writing it is around 216,000 l/s.

In the Bluetooth and Serial communication functions some short delays are used that are essential here but the program only runs here when there is an actual communication.<br>
(An alternative method could have been the use of an interrupt every second and an empty loop)<br>
<br>

<pre>
//--------------------------------------------                //
// COMMON Check connected input devices
//--------------------------------------------
void CheckDevices(void)
{
 CheckBLE();                                                  // Something with BLE to do?
 SerialCheck();                                               // Check serial port every second 
 NTPsyncEvent();                                              // Check for NTP events
 ElegantOTA.loop();                                           // For Over The Air updates
 if (Mem.UseRotary==1) RotaryEncoderCheck(); 
 if (Mem.UseRotary==2) Keypad3x1Check();
}
//-------------------------------------------- 
// CLOCK Update routine done every second 
//-------------------------------------------- 
void EverySecondCheck(void) 
{
 static int Toggle = 0;
 uint32_t msLeap = millis() - msTick; 
 if (msLeap >999)                                              // Every second enter the loop
 {  
...  
if (timeinfo.tm_min != lastminute) EveryMinuteUpdate();        // Enter the every minute routine after one minute 
... 
}
//-------------------------------------------- 
// CLOCK Update routine done every minute 
//--------------------------------------------  
void EveryMinuteUpdate(void) 
{
... 
if(timeinfo.tm_hour != lasthour) EveryHourUpdate(); 
}
//-------------------------------------------- 
// CLOCK Update routine done every hour 
//-------------------------------------------- 
void EveryHourUpdate(void) 
{
... 
if (timeinfo.tm_mday != lastday) EveryDayUpdate();  
}
// // 
//------------------------------------------------------------------------------ 
// CLOCK Update routine done every day 
//------------------------------------------------------------------------------ 
void EveryDayUpdate(void) 
{
... 
}</pre>
<br>

Update the LEDs on the Nano and the PCB. 
On the Nano analogWrite(LED_RED,   512); is OFF and analogWrite(LED_RED, 0); is ON
```
//--------------------------------------------                                                //
// COMMON Update routine for the status LEDs
//-------------------------------------------- 
void UpdateStatusLEDs(int Toggle)
{
 if(Mem.StatusLEDOn)   
   {
...
void SetStatusLED(int Red, int Green, int Blue)
{
 analogWrite(LED_RED,   512 - Red);                                                                 // !Red (not Red) because 1 or HIGH is LED off
 analogWrite(LED_GREEN, 512 - Green);
 analogWrite(LED_BLUE,  512 - Blue);
}

```
</pre>

Check for serial input from the serial monitor and pass the command to ReworkInputString()&nbsp;  
<pre>
//-------------------------------------------- 
// Common check for serial input 
//-------------------------------------------- 
void SerialCheck(void) 
{
... 
ReworkInputString(SerialString+"\n"); // Rework ReworkInputString(); 
... 
}
</pre>

Restore all the default values. 
<pre>
//------------------------------------------------------------------------------ 
// Common Reset to default settings 
//------------------------------------------------------------------------------ 
void Reset(void) 
{
Mem.Checksum = 25065;
...
 }</pre>

Common print routines.<br>

To keep all the print commands in one places it is easy to change these routines 
for other boards with a different 'slang'. <br>
<pre>

//-------------------------------------------- 
// Common common print routines 
//-------------------------------------------- 
void Tekstprint(char const tekst[]) { if(Serial) Serial.print(tekst); SendMessageBLE(tekst);sptext[0]=0; }  
void Tekstprintln(char const tekst[]) { sprintf(sptext,"%s\n",tekst); Tekstprint(sptext); } 
void TekstSprint(char const tekst[]) { printf(tekst); sptext[0]=0;} // printing for Debugging purposes in serial monitor  
void TekstSprintln(char const tekst[]){ sprintf(sptext,"%s\n",tekst); TekstSprint(sptext); } 
//------------------------------------------------------------------------------ 
// Common Constrain a string with integers 
// The value between the first and last character in a string is returned between the low and up bounderies 
//------------------------------------------------------------------------------ 
int SConstrainInt(String s,byte first,byte last,int low,int up){return constrain(s.substring(first, last).toInt(), low, up);} 
int SConstrainInt(String s,byte first, int low,int up){return constrain(s.substring(first).toInt(), low, up);} 
</pre>

The setup of storage space and control of the validity of the settings.<br>
In the checksum is invalid a reset() will restore the default settings 

<pre>
//-------------------------------------------- 
// Common Init and check contents of EEPROM 
//-------------------------------------------- 
void InitStorage(void) 
</pre>

Store and retrieve the settings from SPIFFS or SD or EEPROM.
The several possibilities are stored here. EEPROM becomes outdated but still works. 

<pre>
-------------------------------------------- 
// COMMON Store mem.struct in FlashStorage or SD 
//-------------------------------------------- 
void StoreStructInFlashMemory(void) 
{
}
//-------------------------------------------- 
// COMMON Get data from FlashStorage Preferences.h 
//-------------------------------------------- 
void GetStructFromFlashMemory(void) 
{
}
</pre>

SWversion() prints the menu and the settings of several preferences.<br>

PrintLine() prints the horizontal lines in the menu.<br>

<pre>
//-------------------------------------------- 
// COMMON Version info
//-------------------------------------------- 
void SWversion(void)  
{  
#define FILENAAM (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__) 
PrintLine(35); 
for (uint8_t i = 0; i &lt; sizeof(menu) / sizeof(menu[0]); Tekstprintln(menu[i++])); 
... 
PrintLine(35); 
}

void PrintLine(byte Lengte) 
{... } 
</pre>


Get the commands from the strings entered in the serial monitor, Bluetooth or the webpage and perform the command in an action.<br>
The menu letters are almost used but it possible to distinguish between lower and uppercase when more commands are needed.<br>
(That is why there is no conversion to UpperCase or LowerCase). 

<pre>
/-------------------------------------------- 
//  COMMON Input from Bluetooth or Serial
//--------------------------------------------
void ReworkInputString(String InputString) 
{
....  
switch (InputString[0]) 
{
case 'A': 
case 'a':  
if (InputString.length() >5 ) 
... 
</pre>

Read the LDR and divide it with 16 to get the values from 0 - 4096 between 0 and 255.<br>
Not all boards has a 12 bit AD converter like.  

<pre>
//-------------------------------------------- 
// LDR reading are between 0 and 255.  
// ESP32 analogue read is between 0 - 4096 --   is: 4096 / 8 
//-------------------------------------------- 
int ReadLDR(void) {  return analogRead(PhotoCellPin)/16;} 
</pre>

Control the color and intensity of the LED on the boards in one command <br>
<pre>//
--------------------------------------------
// CLOCK Control the LEDs on the ESP32 
// 0 Low is LED off. Therefore the value is inversed with the ! Not 
//-------------------------------------------- 
void SetStatusLED(bool Red, bool Green, bool Blue) 
{
</pre>

Here we print and colour the characters in the display or light up to proper LEDs in a String of RGB(W) LEDs.<br>
The #define executes this functions with the proper parameters for every language and prints the texts in the serial connections.<br>
#define QUARTER ColorLeds("quarter", 32, 38, LetterColor);

<pre>
//--------------------------------------------    / 
 / LED Set color for LED. 
//--------------------------------------------   
void ColorLeds(char const *Texkst, int FirstLed, int LastLed, uint32_t RGBColor)  
{  } 
</pre>

Every display or strip uses other commands to regulate the brightness. Therefore for all LED/Display commands a function.
<pre>
//------------------------------------------------------------------------------ 
// LED Set brightness of backlight 
//------------------------------------------------------------------------------ 
void SetBrightnessLeds(byte Bright) 
{ 
 SetBackLight(Bright);                        // Set brightness of LEDs 
} 
</pre>

A place to turn off all LEDs or clear the display<br>
<pre>
/-------------------------------------------- 
// LED Clear the character string 
//-------------------------------------------- 
void LedsOff(void)  
</pre>

Here the colours are set for the characters.<br>
It is also used to change the colours of HET IS WAS used in some display choices

<pre>
//--------------------------------------------  <br>
// LED Set second color <br>
//-------------------------------------------- <br>
void SetSecondColour(void) <br>
{ 
...
 MINColor = FuncCRGBW(15 + timeinfo.tm_min * 4, 255 - timeinfo.tm_min * 4, 0, 0); 
 SECColor = FuncCRGBW(15 + timeinfo.tm_sec * 4, 255 - timeinfo.tm_sec * 4, 0, 0 );   
                                                                                              // Light up  IS or WAS with the proper colour  
switch (Mem.DisplayChoice) 
  {
    case DEFAULTCOLOUR: LetterColor = C_YELLOW;  
<br>
... 
</pre>


Subroutine for the Self test of the clock. Default there is a 900 ms delay.
<pre>
//------------------------------------------------------------------------------
// CLOCK Self test sequence
//------------------------------------------------------------------------------
void Selftest(void){Selftest(900);}
void Selftest(int Delayms)
{ 
 GetTijd(true);                                                                                  // Prints time in Serial monitor
 LedsOff(); 
...
}
</pre>

Displaytime() prints the time to the serial monitor as text and control which language is printed.<br>
It also sends the appropriate sequence of colour and intensities to a RGB(W) LED strip. 
<pre>
//--------------------------------------------  
// CLOCK Say the time and load the LEDs 
// with the proper colour and intensity
//--------------------------------------------
void Displaytime(void) 
{  
LedsOff();                                                                                  // Start by clearing the display to a known state   
 if( Mem.DisplayChoice == DIGITAL ) { TimePlaceDigit(timeinfo.tm_hour,timeinfo.tm_min); }
 else                                                                                        // If not a digital display 
   {
                     #ifdef NL144CLOCK
    Dutch();                                                                                 // If TurnLEDsOff is true a new time must be send to the LEDstrip      
                     #endif  //NL144CLOCK
...
                     #ifdef FOURLANGUAGECLOCK
    SetColours();                                                                            // Set the colours for the chosen palette   
    Dutch();
    English();
    German();
    French();
                     #endif  //FOURLANGUAGECLOCK       
   }  
 ShowLeds();                                                                                  // And turn on the LEDs
}
</pre>

DimLeds() reads the analog port and calculates an output intensity to a display or LED-strip.<br>
The readings are squared to get a hyperbolic curve that resembles you eye correction for dark and light better than a linear range.<br>
It works wonderfully well.<br>

<pre>
//--------------------------------------------  
// LED Dim the LEDs measured by the LDR and print values  
// LDR reading are between 0 and 255. The Brightness send to the LEDs is between 0 and 255  
//--------------------------------------------  
void DimLeds(bool print) { ... } 
</pre>


A list of subroutine follows used for several LED operations follows including the initialisations of the WS2812 or SK6812 LED-strip
The SK6812 LED-strip also has a white LED whicht intensity is controlled with the highest bytes (WW) in the 32-bit integer (0XWWRRGGBB)

<pre>
//--------------------------------------------                                                //
//  LED Start LEDs
//--------------------------------------------
void StartLeds(void) 
{
 switch (Mem.LEDstrip)
    {
      case 0: LEDstrip = LED6812strip; 
              white  = 0xFF000000; 
              lgray  = 0x66000000;  
              gray   = 0x33000000;                                                            // The SK6812 LED has a white LED that is pure white
              dgray  = 0x22000000;
              wgray  = 0xAA000000; 

      break;
      case 1: LEDstrip = LED2812strip; 
              white  = 0xFFFFFF;
              lgray  = 0x666666;                                                              // R, G and B on together gives white light
...
}

</pre>
 

A series of functions to get and store time.<br>
The NTP time server puts the retrieved time in the standard C time structures.<br>
<pre>/--------------------------- Time functions -------------------------- 

void GetTijd(byte printit) 
void Print_RTC_tijd(void) 
void PrintNTP_tijd(void) 
void PrintUTCtijd(void) 
void Print_tijd(void) 
void SetRTCTime(void) 

</pre>

Convert a HEX string to a unsigned 32-bits integer<br>

<pre>
//--------------------------------------------
// CLOCK Convert Hex to uint32 
//-------------------------------------------- 
uint32_t HexToDec(String hexString)  </pre>


<h2>Functions to let the clocks speak the time in four languages</h2>
There is also a lot of slang in languages. <br>
'Half nine' sometimes means 8:30 but can also be 9:30. (-:<br>
<br>

<pre>
//--------------------------------------------                                                //
//  CLOCK Dutch clock display
//--------------------------------------------
                      #if defined(NL) || defined(NL144CLOCK)  
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
...



</pre>

<br>

The Bluetooth Low Energy Nordic nRF.. functions.<br>
They are different from the Texas instrument CC2540/CC2541 that is used in other 
chipsets like the HM-10, HM16, JDY-08 et cetera.<br>
<a href="https://www.instructables.com/Communicate-Using-CC254x-or-NRF-BLE-With-Arduino-a/">More here on Instructables</a><br>
<br>


<pre>/----------------------------- 
// BLE SendMessage by BLE Slow in packets of 20 chars 
//------------------------------ 
void SendMessageBLE(std::string Message) 

/----------------------------- 
// BLE Start BLE Classes 
//------------------------------ 
class MyServerCallbacks: public BLEServerCallbacks  

/----------------------------- 
// BLE Start BLE Service 
//------------------------------ 
void StartBLEService(void) 

/----------------------------- 
// BLE CheckBLE 
//------------------------------ 
void CheckBLE(void) </pre>
<br>
Functions to start a WIFI connection and use the webpage<br>
<br>
<pre>/-------------------------------------------- 
// WIFI WEBPAGE  
//-------------------------------------------- 
void StartWIFI_NTP(void) 

/-------------------------------------------- 
// WIFI WEBPAGE  
//-------------------------------------------- 
void WebPage(void)  

/-------------------------------------------- 
// WIFI WEBPAGE Not found message 
//-------------------------------------------- 
void notFound(AsyncWebServerRequest *request) 
</pre>
<br>



#Time zones
 
<br>
 Copy the text between the quotes and paste them after the character E 
  

<pre> Africa/Abidjan,"GMT0" 
 Africa/Accra,"GMT0" 
 Africa/Addis_Ababa,"EAT-3" 
 Africa/Algiers,"CET-1" 
 Africa/Asmara,"EAT-3" 
 Africa/Bamako,"GMT0" 
 Africa/Bangui,"WAT-1" 
 Africa/Banjul,"GMT0" 
 Africa/Bissau,"GMT0" 
 Africa/Blantyre,"CAT-2" 
 Africa/Brazzaville,"WAT-1" 
 Africa/Bujumbura,"CAT-2" 
 Africa/Cairo,"EET-2" 
 Africa/Casablanca,"&lt;+01&gt;-1" 
 Africa/Ceuta,"CET-1CEST,M3.5.0,M10.5.0/3" 
 Africa/Conakry,"GMT0" 
 Africa/Dakar,"GMT0" 
 Africa/Dar_es_Salaam,"EAT-3" 
 Africa/Djibouti,"EAT-3" 
 Africa/Douala,"WAT-1" 
 Africa/El_Aaiun,"&lt;+01&gt;-1" 
 Africa/Freetown,"GMT0" 
 Africa/Gaborone,"CAT-2" 
 Africa/Harare,"CAT-2" 
 Africa/Johannesburg,"SAST-2" 
 Africa/Juba,"CAT-2" 
 Africa/Kampala,"EAT-3" 
 Africa/Khartoum,"CAT-2" 
 Africa/Kigali,"CAT-2" 
 Africa/Kinshasa,"WAT-1" 
 Africa/Lagos,"WAT-1" 
 Africa/Libreville,"WAT-1" 
 Africa/Lome,"GMT0" 
 Africa/Luanda,"WAT-1" 
 Africa/Lubumbashi,"CAT-2" 
 Africa/Lusaka,"CAT-2" 
 Africa/Malabo,"WAT-1" 
 Africa/Maputo,"CAT-2" 
 Africa/Maseru,"SAST-2" 
 Africa/Mbabane,"SAST-2" 
 Africa/Mogadishu,"EAT-3" 
 Africa/Monrovia,"GMT0" 
 Africa/Nairobi,"EAT-3" 
 Africa/Ndjamena,"WAT-1" 
 Africa/Niamey,"WAT-1" 
 Africa/Nouakchott,"GMT0" 
 Africa/Ouagadougou,"GMT0" 
 Africa/Porto-Novo,"WAT-1" 
 Africa/Sao_Tome,"GMT0" 
 Africa/Tripoli,"EET-2" 
 Africa/Tunis,"CET-1" 
 Africa/Windhoek,"CAT-2" 
 America/Adak,"HST10HDT,M3.2.0,M11.1.0" 
 America/Anchorage,"AKST9AKDT,M3.2.0,M11.1.0" 
 America/Anguilla,"AST4" 
 America/Antigua,"AST4" 
 America/Araguaina,"&lt;-03&gt;3" 
 America/Argentina/Buenos_Aires,"&lt;-03&gt;3" 
 America/Argentina/Catamarca,"&lt;-03&gt;3" 
 America/Argentina/Cordoba,"&lt;-03&gt;3" 
 America/Argentina/Jujuy,"&lt;-03&gt;3" 
 America/Argentina/La_Rioja,"&lt;-03&gt;3" 
 America/Argentina/Mendoza,"&lt;-03&gt;3" 
 America/Argentina/Rio_Gallegos,"&lt;-03&gt;3" 
 America/Argentina/Salta,"&lt;-03&gt;3" 
 America/Argentina/San_Juan,"&lt;-03&gt;3" 
 America/Argentina/San_Luis,"&lt;-03&gt;3" 
 America/Argentina/Tucuman,"&lt;-03&gt;3" 
 America/Argentina/Ushuaia,"&lt;-03&gt;3" 
 America/Aruba,"AST4" 
 America/Asuncion,"&lt;-04&gt;4&lt;-03&gt;,M10.1.0/0,M3.4.0/0" 
 America/Atikokan,"EST5" 
 America/Bahia,"&lt;-03&gt;3" 
 America/Bahia_Banderas,"CST6CDT,M4.1.0,M10.5.0" 
 America/Barbados,"AST4" 
 America/Belem,"&lt;-03&gt;3" 
 America/Belize,"CST6" 
 America/Blanc-Sablon,"AST4" 
 America/Boa_Vista,"&lt;-04&gt;4" 
 America/Bogota,"&lt;-05&gt;5" 
 America/Boise,"MST7MDT,M3.2.0,M11.1.0" 
 America/Cambridge_Bay,"MST7MDT,M3.2.0,M11.1.0" 
 America/Campo_Grande,"&lt;-04&gt;4" 
 America/Cancun,"EST5" 
 America/Caracas,"&lt;-04&gt;4" 
 America/Cayenne,"&lt;-03&gt;3" 
 America/Cayman,"EST5" 
 America/Chicago,"CST6CDT,M3.2.0,M11.1.0" 
 America/Chihuahua,"MST7MDT,M4.1.0,M10.5.0" 
 America/Costa_Rica,"CST6" 
 America/Creston,"MST7" 
 America/Cuiaba,"&lt;-04&gt;4" 
 America/Curacao,"AST4" 
 America/Danmarkshavn,"GMT0" 
 America/Dawson,"MST7" 
 America/Dawson_Creek,"MST7" 
 America/Denver,"MST7MDT,M3.2.0,M11.1.0" 
 America/Detroit,"EST5EDT,M3.2.0,M11.1.0" 
 America/Dominica,"AST4" 
 America/Edmonton,"MST7MDT,M3.2.0,M11.1.0" 
 America/Eirunepe,"&lt;-05&gt;5" 
 America/El_Salvador,"CST6" 
 America/Fortaleza,"&lt;-03&gt;3" 
 America/Fort_Nelson,"MST7" 
 America/Glace_Bay,"AST4ADT,M3.2.0,M11.1.0" 
 America/Godthab,"&lt;-03&gt;3&lt;-02&gt;,M3.5.0/-2,M10.5.0/-1" 
 America/Goose_Bay,"AST4ADT,M3.2.0,M11.1.0" 
 America/Grand_Turk,"EST5EDT,M3.2.0,M11.1.0" 
 America/Grenada,"AST4" 
 America/Guadeloupe,"AST4" 
 America/Guatemala,"CST6" 
 America/Guayaquil,"&lt;-05&gt;5" 
 America/Guyana,"&lt;-04&gt;4" 
 America/Halifax,"AST4ADT,M3.2.0,M11.1.0" 
 America/Havana,"CST5CDT,M3.2.0/0,M11.1.0/1" 
 America/Hermosillo,"MST7" 
 America/Indiana/Indianapolis,"EST5EDT,M3.2.0,M11.1.0" 
 America/Indiana/Knox,"CST6CDT,M3.2.0,M11.1.0" 
 America/Indiana/Marengo,"EST5EDT,M3.2.0,M11.1.0" 
 America/Indiana/Petersburg,"EST5EDT,M3.2.0,M11.1.0" 
 America/Indiana/Tell_City,"CST6CDT,M3.2.0,M11.1.0" 
 America/Indiana/Vevay,"EST5EDT,M3.2.0,M11.1.0" 
 America/Indiana/Vincennes,"EST5EDT,M3.2.0,M11.1.0" 
 America/Indiana/Winamac,"EST5EDT,M3.2.0,M11.1.0" 
 America/Inuvik,"MST7MDT,M3.2.0,M11.1.0" 
 America/Iqaluit,"EST5EDT,M3.2.0,M11.1.0" 
 America/Jamaica,"EST5" 
 America/Juneau,"AKST9AKDT,M3.2.0,M11.1.0" 
 America/Kentucky/Louisville,"EST5EDT,M3.2.0,M11.1.0" 
 America/Kentucky/Monticello,"EST5EDT,M3.2.0,M11.1.0" 
 America/Kralendijk,"AST4" 
 America/La_Paz,"&lt;-04&gt;4" 
 America/Lima,"&lt;-05&gt;5" 
 America/Los_Angeles,"PST8PDT,M3.2.0,M11.1.0" 
 America/Lower_Princes,"AST4" 
 America/Maceio,"&lt;-03&gt;3" 
 America/Managua,"CST6" 
 America/Manaus,"&lt;-04&gt;4" 
 America/Marigot,"AST4" 
 America/Martinique,"AST4" 
 America/Matamoros,"CST6CDT,M3.2.0,M11.1.0" 
 America/Mazatlan,"MST7MDT,M4.1.0,M10.5.0" 
 America/Menominee,"CST6CDT,M3.2.0,M11.1.0" 
 America/Merida,"CST6CDT,M4.1.0,M10.5.0" 
 America/Metlakatla,"AKST9AKDT,M3.2.0,M11.1.0" 
 America/Mexico_City,"CST6CDT,M4.1.0,M10.5.0" 
 America/Miquelon,"&lt;-03&gt;3&lt;-02&gt;,M3.2.0,M11.1.0" 
 America/Moncton,"AST4ADT,M3.2.0,M11.1.0" 
 America/Monterrey,"CST6CDT,M4.1.0,M10.5.0" 
 America/Montevideo,"&lt;-03&gt;3" 
 America/Montreal,"EST5EDT,M3.2.0,M11.1.0" 
 America/Montserrat,"AST4" 
 America/Nassau,"EST5EDT,M3.2.0,M11.1.0" 
 America/New_York,"EST5EDT,M3.2.0,M11.1.0" 
 America/Nipigon,"EST5EDT,M3.2.0,M11.1.0" 
 America/Nome,"AKST9AKDT,M3.2.0,M11.1.0" 
 America/Noronha,"&lt;-02&gt;2" 
 America/North_Dakota/Beulah,"CST6CDT,M3.2.0,M11.1.0" 
 America/North_Dakota/Center,"CST6CDT,M3.2.0,M11.1.0" 
 America/North_Dakota/New_Salem,"CST6CDT,M3.2.0,M11.1.0" 
 America/Nuuk,"&lt;-03&gt;3&lt;-02&gt;,M3.5.0/-2,M10.5.0/-1" 
 America/Ojinaga,"MST7MDT,M3.2.0,M11.1.0" 
 America/Panama,"EST5" 
 America/Pangnirtung,"EST5EDT,M3.2.0,M11.1.0" 
 America/Paramaribo,"&lt;-03&gt;3" 
 America/Phoenix,"MST7" 
 America/Port-au-Prince,"EST5EDT,M3.2.0,M11.1.0" 
 America/Port_of_Spain,"AST4" 
 America/Porto_Velho,"&lt;-04&gt;4" 
 America/Puerto_Rico,"AST4" 
 America/Punta_Arenas,"&lt;-03&gt;3" 
 America/Rainy_River,"CST6CDT,M3.2.0,M11.1.0" 
 America/Rankin_Inlet,"CST6CDT,M3.2.0,M11.1.0" 
 America/Recife,"&lt;-03&gt;3" 
 America/Regina,"CST6" 
 America/Resolute,"CST6CDT,M3.2.0,M11.1.0" 
 America/Rio_Branco,"&lt;-05&gt;5" 
 America/Santarem,"&lt;-03&gt;3" 
 America/Santiago,"&lt;-04&gt;4&lt;-03&gt;,M9.1.6/24,M4.1.6/24" 
 America/Santo_Domingo,"AST4" 
 America/Sao_Paulo,"&lt;-03&gt;3" 
 America/Scoresbysund,"&lt;-01&gt;1&lt;+00&gt;,M3.5.0/0,M10.5.0/1" 
 America/Sitka,"AKST9AKDT,M3.2.0,M11.1.0" 
 America/St_Barthelemy,"AST4" 
 America/St_Johns,"NST3:30NDT,M3.2.0,M11.1.0" 
 America/St_Kitts,"AST4" 
 America/St_Lucia,"AST4" 
 America/St_Thomas,"AST4" 
 America/St_Vincent,"AST4" 
 America/Swift_Current,"CST6" 
 America/Tegucigalpa,"CST6" 
 America/Thule,"AST4ADT,M3.2.0,M11.1.0" 
 America/Thunder_Bay,"EST5EDT,M3.2.0,M11.1.0" 
 America/Tijuana,"PST8PDT,M3.2.0,M11.1.0" 
 America/Toronto,"EST5EDT,M3.2.0,M11.1.0" 
 America/Tortola,"AST4" 
 America/Vancouver,"PST8PDT,M3.2.0,M11.1.0" 
 America/Whitehorse,"MST7" 
 America/Winnipeg,"CST6CDT,M3.2.0,M11.1.0" 
 America/Yakutat,"AKST9AKDT,M3.2.0,M11.1.0" 
 America/Yellowknife,"MST7MDT,M3.2.0,M11.1.0" 
 Antarctica/Casey,"&lt;+11&gt;-11" 
 Antarctica/Davis,"&lt;+07&gt;-7" 
 Antarctica/DumontDUrville,"&lt;+10&gt;-10" 
 Antarctica/Macquarie,"AEST-10AEDT,M10.1.0,M4.1.0/3" 
 Antarctica/Mawson,"&lt;+05&gt;-5" 
 Antarctica/McMurdo,"NZST-12NZDT,M9.5.0,M4.1.0/3" 
 Antarctica/Palmer,"&lt;-03&gt;3" 
 Antarctica/Rothera,"&lt;-03&gt;3" 
 Antarctica/Syowa,"&lt;+03&gt;-3" 
 Antarctica/Troll,"&lt;+00&gt;0&lt;+02&gt;-2,M3.5.0/1,M10.5.0/3" 
 Antarctica/Vostok,"&lt;+06&gt;-6" 
 Arctic/Longyearbyen,"CET-1CEST,M3.5.0,M10.5.0/3" 
 Asia/Aden,"&lt;+03&gt;-3" 
 Asia/Almaty,"&lt;+06&gt;-6" 
 Asia/Amman,"EET-2EEST,M2.5.4/24,M10.5.5/1" 
 Asia/Anadyr,"&lt;+12&gt;-12" 
 Asia/Aqtau,"&lt;+05&gt;-5" 
 Asia/Aqtobe,"&lt;+05&gt;-5" 
 Asia/Ashgabat,"&lt;+05&gt;-5" 
 Asia/Atyrau,"&lt;+05&gt;-5" 
 Asia/Baghdad,"&lt;+03&gt;-3" 
 Asia/Bahrain,"&lt;+03&gt;-3" 
 Asia/Baku,"&lt;+04&gt;-4" 
 Asia/Bangkok,"&lt;+07&gt;-7" 
 Asia/Barnaul,"&lt;+07&gt;-7" 
 Asia/Beirut,"EET-2EEST,M3.5.0/0,M10.5.0/0" 
 Asia/Bishkek,"&lt;+06&gt;-6" 
 Asia/Brunei,"&lt;+08&gt;-8" 
 Asia/Chita,"&lt;+09&gt;-9" 
 Asia/Choibalsan,"&lt;+08&gt;-8" 
 Asia/Colombo,"&lt;+0530&gt;-5:30" 
 Asia/Damascus,"EET-2EEST,M3.5.5/0,M10.5.5/0" 
 Asia/Dhaka,"&lt;+06&gt;-6" 
 Asia/Dili,"&lt;+09&gt;-9" 
 Asia/Dubai,"&lt;+04&gt;-4" 
 Asia/Dushanbe,"&lt;+05&gt;-5" 
 Asia/Famagusta,"EET-2EEST,M3.5.0/3,M10.5.0/4" 
 Asia/Gaza,"EET-2EEST,M3.4.4/48,M10.5.5/1" 
 Asia/Hebron,"EET-2EEST,M3.4.4/48,M10.5.5/1" 
 Asia/Ho_Chi_Minh,"&lt;+07&gt;-7" 
 Asia/Hong_Kong,"HKT-8" 
 Asia/Hovd,"&lt;+07&gt;-7" 
 Asia/Irkutsk,"&lt;+08&gt;-8" 
 Asia/Jakarta,"WIB-7" 
 Asia/Jayapura,"WIT-9" 
 Asia/Jerusalem,"IST-2IDT,M3.4.4/26,M10.5.0" 
 Asia/Kabul,"&lt;+0430&gt;-4:30" 
 Asia/Kamchatka,"&lt;+12&gt;-12" 
 Asia/Karachi,"PKT-5" 
 Asia/Kathmandu,"&lt;+0545&gt;-5:45" 
 Asia/Khandyga,"&lt;+09&gt;-9" 
 Asia/Kolkata,"IST-5:30" 
 Asia/Krasnoyarsk,"&lt;+07&gt;-7" 
 Asia/Kuala_Lumpur,"&lt;+08&gt;-8" 
 Asia/Kuching,"&lt;+08&gt;-8" 
 Asia/Kuwait,"&lt;+03&gt;-3" 
 Asia/Macau,"CST-8" 
 Asia/Magadan,"&lt;+11&gt;-11" 
 Asia/Makassar,"WITA-8" 
 Asia/Manila,"PST-8" 
 Asia/Muscat,"&lt;+04&gt;-4" 
 Asia/Nicosia,"EET-2EEST,M3.5.0/3,M10.5.0/4" 
 Asia/Novokuznetsk,"&lt;+07&gt;-7" 
 Asia/Novosibirsk,"&lt;+07&gt;-7" 
 Asia/Omsk,"&lt;+06&gt;-6" 
 Asia/Oral,"&lt;+05&gt;-5" 
 Asia/Phnom_Penh,"&lt;+07&gt;-7" 
 Asia/Pontianak,"WIB-7" 
 Asia/Pyongyang,"KST-9" 
 Asia/Qatar,"&lt;+03&gt;-3" 
 Asia/Qyzylorda,"&lt;+05&gt;-5" 
 Asia/Riyadh,"&lt;+03&gt;-3" 
 Asia/Sakhalin,"&lt;+11&gt;-11" 
 Asia/Samarkand,"&lt;+05&gt;-5" 
 Asia/Seoul,"KST-9" 
 Asia/Shanghai,"CST-8" 
 Asia/Singapore,"&lt;+08&gt;-8" 
 Asia/Srednekolymsk,"&lt;+11&gt;-11" 
 Asia/Taipei,"CST-8" 
 Asia/Tashkent,"&lt;+05&gt;-5" 
 Asia/Tbilisi,"&lt;+04&gt;-4" 
 Asia/Tehran,"&lt;+0330&gt;-3:30&lt;+0430&gt;,J79/24,J263/24" 
 Asia/Thimphu,"&lt;+06&gt;-6" 
 Asia/Tokyo,"JST-9" 
 Asia/Tomsk,"&lt;+07&gt;-7" 
 Asia/Ulaanbaatar,"&lt;+08&gt;-8" 
 Asia/Urumqi,"&lt;+06&gt;-6" 
 Asia/Ust-Nera,"&lt;+10&gt;-10" 
 Asia/Vientiane,"&lt;+07&gt;-7" 
 Asia/Vladivostok,"&lt;+10&gt;-10" 
 Asia/Yakutsk,"&lt;+09&gt;-9" 
 Asia/Yangon,"&lt;+0630&gt;-6:30" 
 Asia/Yekaterinburg,"&lt;+05&gt;-5" 
 Asia/Yerevan,"&lt;+04&gt;-4" 
 Atlantic/Azores,"&lt;-01&gt;1&lt;+00&gt;,M3.5.0/0,M10.5.0/1" 
 Atlantic/Bermuda,"AST4ADT,M3.2.0,M11.1.0" 
 Atlantic/Canary,"WET0WEST,M3.5.0/1,M10.5.0" 
 Atlantic/Cape_Verde,"&lt;-01&gt;1" 
 Atlantic/Faroe,"WET0WEST,M3.5.0/1,M10.5.0" 
 Atlantic/Madeira,"WET0WEST,M3.5.0/1,M10.5.0" 
 Atlantic/Reykjavik,"GMT0" 
 Atlantic/South_Georgia,"&lt;-02&gt;2" 
 Atlantic/Stanley,"&lt;-03&gt;3" 
 Atlantic/St_Helena,"GMT0" 
 Australia/Adelaide,"ACST-9:30ACDT,M10.1.0,M4.1.0/3" 
 Australia/Brisbane,"AEST-10" 
 Australia/Broken_Hill,"ACST-9:30ACDT,M10.1.0,M4.1.0/3" 
 Australia/Currie,"AEST-10AEDT,M10.1.0,M4.1.0/3" 
 Australia/Darwin,"ACST-9:30" 
 Australia/Eucla,"&lt;+0845&gt;-8:45" 
 Australia/Hobart,"AEST-10AEDT,M10.1.0,M4.1.0/3" 
 Australia/Lindeman,"AEST-10" 
 Australia/Lord_Howe,"&lt;+1030&gt;-10:30&lt;+11&gt;-11,M10.1.0,M4.1.0" 
 Australia/Melbourne,"AEST-10AEDT,M10.1.0,M4.1.0/3" 
 Australia/Perth,"AWST-8" 
 Australia/Sydney,"AEST-10AEDT,M10.1.0,M4.1.0/3" 
 Europe/Amsterdam,"CET-1CEST,M3.5.0,M10.5.0/3" 
 Europe/Andorra,"CET-1CEST,M3.5.0,M10.5.0/3" 
 Europe/Astrakhan,"&lt;+04&gt;-4" 
 Europe/Athens,"EET-2EEST,M3.5.0/3,M10.5.0/4" 
 Europe/Belgrade,"CET-1CEST,M3.5.0,M10.5.0/3" 
 Europe/Berlin,"CET-1CEST,M3.5.0,M10.5.0/3" 
 Europe/Bratislava,"CET-1CEST,M3.5.0,M10.5.0/3" 
 Europe/Brussels,"CET-1CEST,M3.5.0,M10.5.0/3" 
 Europe/Bucharest,"EET-2EEST,M3.5.0/3,M10.5.0/4" 
 Europe/Budapest,"CET-1CEST,M3.5.0,M10.5.0/3" 
 Europe/Busingen,"CET-1CEST,M3.5.0,M10.5.0/3" 
 Europe/Chisinau,"EET-2EEST,M3.5.0,M10.5.0/3" 
 Europe/Copenhagen,"CET-1CEST,M3.5.0,M10.5.0/3" 
 Europe/Dublin,"IST-1GMT0,M10.5.0,M3.5.0/1" 
 Europe/Gibraltar,"CET-1CEST,M3.5.0,M10.5.0/3" 
 Europe/Guernsey,"GMT0BST,M3.5.0/1,M10.5.0" 
 Europe/Helsinki,"EET-2EEST,M3.5.0/3,M10.5.0/4" 
 Europe/Isle_of_Man,"GMT0BST,M3.5.0/1,M10.5.0" 
 Europe/Istanbul,"&lt;+03&gt;-3" 
 Europe/Jersey,"GMT0BST,M3.5.0/1,M10.5.0" 
 Europe/Kaliningrad,"EET-2" 
 Europe/Kiev,"EET-2EEST,M3.5.0/3,M10.5.0/4" 
 Europe/Kirov,"&lt;+03&gt;-3" 
 Europe/Lisbon,"WET0WEST,M3.5.0/1,M10.5.0" 
 Europe/Ljubljana,"CET-1CEST,M3.5.0,M10.5.0/3" 
 Europe/London,"GMT0BST,M3.5.0/1,M10.5.0" 
 Europe/Luxembourg,"CET-1CEST,M3.5.0,M10.5.0/3" 
 Europe/Madrid,"CET-1CEST,M3.5.0,M10.5.0/3" 
 Europe/Malta,"CET-1CEST,M3.5.0,M10.5.0/3" 
 Europe/Mariehamn,"EET-2EEST,M3.5.0/3,M10.5.0/4" 
 Europe/Minsk,"&lt;+03&gt;-3" 
 Europe/Monaco,"CET-1CEST,M3.5.0,M10.5.0/3" 
 Europe/Moscow,"MSK-3" 
 Europe/Oslo,"CET-1CEST,M3.5.0,M10.5.0/3" 
 Europe/Paris,"CET-1CEST,M3.5.0,M10.5.0/3" 
 Europe/Podgorica,"CET-1CEST,M3.5.0,M10.5.0/3" 
 Europe/Prague,"CET-1CEST,M3.5.0,M10.5.0/3" 
 Europe/Riga,"EET-2EEST,M3.5.0/3,M10.5.0/4" 
 Europe/Rome,"CET-1CEST,M3.5.0,M10.5.0/3" 
 Europe/Samara,"&lt;+04&gt;-4" 
 Europe/San_Marino,"CET-1CEST,M3.5.0,M10.5.0/3" 
 Europe/Sarajevo,"CET-1CEST,M3.5.0,M10.5.0/3" 
 Europe/Saratov,"&lt;+04&gt;-4" 
 Europe/Simferopol,"MSK-3" 
 Europe/Skopje,"CET-1CEST,M3.5.0,M10.5.0/3" 
 Europe/Sofia,"EET-2EEST,M3.5.0/3,M10.5.0/4" 
 Europe/Stockholm,"CET-1CEST,M3.5.0,M10.5.0/3" 
 Europe/Tallinn,"EET-2EEST,M3.5.0/3,M10.5.0/4" 
 Europe/Tirane,"CET-1CEST,M3.5.0,M10.5.0/3" 
 Europe/Ulyanovsk,"&lt;+04&gt;-4" 
 Europe/Uzhgorod,"EET-2EEST,M3.5.0/3,M10.5.0/4" 
 Europe/Vaduz,"CET-1CEST,M3.5.0,M10.5.0/3" 
 Europe/Vatican,"CET-1CEST,M3.5.0,M10.5.0/3" 
 Europe/Vienna,"CET-1CEST,M3.5.0,M10.5.0/3" 
 Europe/Vilnius,"EET-2EEST,M3.5.0/3,M10.5.0/4" 
 Europe/Volgograd,"&lt;+03&gt;-3" 
 Europe/Warsaw,"CET-1CEST,M3.5.0,M10.5.0/3" 
 Europe/Zagreb,"CET-1CEST,M3.5.0,M10.5.0/3" 
 Europe/Zaporozhye,"EET-2EEST,M3.5.0/3,M10.5.0/4" 
 Europe/Zurich,"CET-1CEST,M3.5.0,M10.5.0/3" 
 Indian/Antananarivo,"EAT-3" 
 Indian/Chagos,"&lt;+06&gt;-6" 
 Indian/Christmas,"&lt;+07&gt;-7" 
 Indian/Cocos,"&lt;+0630&gt;-6:30" 
 Indian/Comoro,"EAT-3" 
 Indian/Kerguelen,"&lt;+05&gt;-5" 
 Indian/Mahe,"&lt;+04&gt;-4" 
 Indian/Maldives,"&lt;+05&gt;-5" 
 Indian/Mauritius,"&lt;+04&gt;-4" 
 Indian/Mayotte,"EAT-3" 
 Indian/Reunion,"&lt;+04&gt;-4" 
 Pacific/Apia,"&lt;+13&gt;-13" 
 Pacific/Auckland,"NZST-12NZDT,M9.5.0,M4.1.0/3" 
 Pacific/Bougainville,"&lt;+11&gt;-11" 
 Pacific/Chatham,"&lt;+1245&gt;-12:45&lt;+1345&gt;,M9.5.0/2:45,M4.1.0/3:45" 
 Pacific/Chuuk,"&lt;+10&gt;-10" 
 Pacific/Easter,"&lt;-06&gt;6&lt;-05&gt;,M9.1.6/22,M4.1.6/22" 
 Pacific/Efate,"&lt;+11&gt;-11" 
 Pacific/Enderbury,"&lt;+13&gt;-13" 
 Pacific/Fakaofo,"&lt;+13&gt;-13" 
 Pacific/Fiji,"&lt;+12&gt;-12&lt;+13&gt;,M11.2.0,M1.2.3/99" 
 Pacific/Funafuti,"&lt;+12&gt;-12" 
 Pacific/Galapagos,"&lt;-06&gt;6" 
 Pacific/Gambier,"&lt;-09&gt;9" 
 Pacific/Guadalcanal,"&lt;+11&gt;-11" 
 Pacific/Guam,"ChST-10" 
 Pacific/Honolulu,"HST10" 
 Pacific/Kiritimati,"&lt;+14&gt;-14" 
 Pacific/Kosrae,"&lt;+11&gt;-11" 
 Pacific/Kwajalein,"&lt;+12&gt;-12" 
 Pacific/Majuro,"&lt;+12&gt;-12" 
 Pacific/Marquesas,"&lt;-0930&gt;9:30" 
 Pacific/Midway,"SST11" 
 Pacific/Nauru,"&lt;+12&gt;-12" 
 Pacific/Niue,"&lt;-11&gt;11" 
 Pacific/Norfolk,"&lt;+11&gt;-11&lt;+12&gt;,M10.1.0,M4.1.0/3" 
 Pacific/Noumea,"&lt;+11&gt;-11" 
 Pacific/Pago_Pago,"SST11" 
 Pacific/Palau,"&lt;+09&gt;-9" 
 Pacific/Pitcairn,"&lt;-08&gt;8" 
 Pacific/Pohnpei,"&lt;+11&gt;-11" 
 Pacific/Port_Moresby,"&lt;+10&gt;-10" 
 Pacific/Rarotonga,"&lt;-10&gt;10" 
 Pacific/Saipan,"ChST-10" 
 Pacific/Tahiti,"&lt;-10&gt;10" 
 Pacific/Tarawa,"&lt;+12&gt;-12" 
 Pacific/Tongatapu,"&lt;+13&gt;-13" 
 Pacific/Wake,"&lt;+12&gt;-12" 
 Pacific/Wallis,"&lt;+12&gt;-12" 
 Etc/GMT,"GMT0" 
 Etc/GMT-0,"GMT0" 
 Etc/GMT-1,"&lt;+01&gt;-1" 
 Etc/GMT-2,"&lt;+02&gt;-2" 
 Etc/GMT-3,"&lt;+03&gt;-3" 
 Etc/GMT-4,"&lt;+04&gt;-4" 
 Etc/GMT-5,"&lt;+05&gt;-5" 
 Etc/GMT-6,"&lt;+06&gt;-6" 
 Etc/GMT-7,"&lt;+07&gt;-7" 
 Etc/GMT-8,"&lt;+08&gt;-8" 
 Etc/GMT-9,"&lt;+09&gt;-9" 
 Etc/GMT-10,"&lt;+10&gt;-10" 
 Etc/GMT-11,"&lt;+11&gt;-11" 
 Etc/GMT-12,"&lt;+12&gt;-12" 
 Etc/GMT-13,"&lt;+13&gt;-13" 
 Etc/GMT-14,"&lt;+14&gt;-14" 
 Etc/GMT0,"GMT0" 
 Etc/GMT+0,"GMT0" 
 Etc/GMT+1,"&lt;-01&gt;1" 
 Etc/GMT+2,"&lt;-02&gt;2" 
 Etc/GMT+3,"&lt;-03&gt;3" 
 Etc/GMT+4,"&lt;-04&gt;4" 
 Etc/GMT+5,"&lt;-05&gt;5" 
 Etc/GMT+6,"&lt;-06&gt;6" 
 Etc/GMT+7,"&lt;-07&gt;7" 
 Etc/GMT+8,"&lt;-08&gt;8" 
 Etc/GMT+9,"&lt;-09&gt;9" 
 Etc/GMT+10,"&lt;-10&gt;10" 
 Etc/GMT+11,"&lt;-11&gt;11" 
 Etc/GMT+12,"&lt;-12&gt;12" 
 Etc/UCT,"UTC0" 
 Etc/UTC,"UTC0" 
 Etc/Greenwich,"GMT0" 
 Etc/Universal,"UTC0" 
 Etc/Zulu,"UTC0" </pre>