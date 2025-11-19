# Arduino ESP32-Nano word clock

A clock that displays the time in words in Dutch, English, French and
German in a large 4-language clock or as a single-language clock.

An Arduino ESP32 Nano computer integrated into the clock is used to
control the clock\'s time and establish Wi-Fi and Bluetooth
connections.\
The time is synchronized with the internet\'s Network Time Protocol
(NTP) via Wi-Fi. It\'s also possible to set the time without Wi-Fi or
Bluetooth using a rotary knob/pushbutton.\
Settings can be managed via a web page, a PC, or a Bluetooth Low Energy
(BLE) serial terminal app installed on a phone, PC, or tablet.

  ----------------------------------------------------------------------------------------------------------------
  ![](media/image1.jpeg){width="3.2083333333333335in"        ![](media/image2.jpeg){width="3.2916666666666665in"
  height="3.2305555555555556in"}                             height="3.23125in"}
  ---------------------------------------------------------- -----------------------------------------------------
  4-language clock                                           Dutch language clock

  ![image](media/image3.jpeg){width="3.3319444444444444in"   
  height="3.188888888888889in"}                              

  Corten steel clock                                         
  ----------------------------------------------------------------------------------------------------------------

+--------------------------------------------------------------------+---+
| **Quick start**                                                    |   |
|                                                                    |   |
| Find one of the following apps in the App Store. This app is used  |   |
| to send commands to the clock.                                     |   |
|                                                                    |   |
| If the app cannot be downloaded, please read on to find out how to |   |
| connect to Wi-Fi.                                                  |   |
|                                                                    |   |
| +-------------------+-------------------+---------------------+    |   |
| | iOS               | iOS               | Android             |    |   |
| | iPhone/iPad/Mac   | iPhone/iPad/Mac   |                     |    |   |
| +===================+===================+=====================+    |   |
| | ![](media/image4  | ![](media/image5  | ![](media/im        |    |   |
| | .png){width="0.68 | .png){width="0.61 | age6.png){width="0. |    |   |
| | 69291338582677in" | 82370953630796in" | 6827766841644795in" |    |   |
| | height="0.684     | height="0.629     | height="0.6         |    |   |
| | 0015310586177in"} | 9015748031496in"} | 856321084864392in"} |    |   |
| +-------------------+-------------------+---------------------+    |   |
| | [BLE Serial       | [BLESerial        | [Serial Bluetooth   |    |   |
| | Pro](https:/      | nRF](https://     | T                   |    |   |
| | /apps.apple.com/n | apps.apple.com/nl | erminal](https://pl |    |   |
| | l/app/ble-serial- | /app/ble-serial-t | ay.google.com/store |    |   |
| | pro/id1632245655) | iny/id1607862132) | /apps/details?id=de |    |   |
| |                   |                   | .kai_morich.serial_ |    |   |
| |                   |                   | bluetooth_terminal) |    |   |
| |                   |                   |                     |    |   |
| |                   |                   | (Turn off timestamp |    |   |
| |                   |                   | in the menu)        |    |   |
| +-------------------+-------------------+---------------------+    |   |
+====================================================================+===+
+--------------------------------------------------------------------+---+

![](media/image7.png){width="1.8138888888888889in"
height="2.6430555555555557in"}The clock is set to receive the time via
Wi-Fi. If the Wi-Fi router name and password haven\'t been entered, a
webpage will open asking you to enter the router credentials.

-   StartWordclock " station in the WIFI settings .

-   Enter the password: wordclock

-   Then open the browser on your mobile and type in the URL:
    162.168.4.1 and press enter.

-   Enter the name and password of the Wi-Fi router used at home.\
    **Tip** : You can also use your mobile hotspot for Wi-Fi.

**Bluetooth app**

Setup is easier with the Bluetooth app on your phone or tablet.

-   ![](media/image8.wmf){width="1.8506944444444444in"
    height="2.2888888888888888in"}Open the Bluetooth app.

The iOS app displays a list of Bluetooth stations.\
If it\'s taking too long, tap the wheel in the top left corner (nRF52 is
selected) and tap Done .\
On Android, tap Scan and Connect with the word clock.

-   Send the command: **i** to see the menu

Type the letter **a** followed by the name of the router and press Send
. ( eg : aNAMErouter Do not send extra periods or spaces.)

-   Type the letter **b** followed by the router password and press Send
    . ( e.g .: bGeHeim1234)

-   Send @ to restart the clock.

**\
**

**Or start WPS** (A button on the router with WPS. Read this in the
manual)

-   Press the WPS button and then send the command: **z** in the
    Bluetooth serial app.

-   **If WIFI is not possible,**\
    send the letter J in the menu with the Bluetooth app.

-   Then it appears: Use DS3231 is ON. WIFI ON, NTP OFF.

-   Send a W to turn off Wi-Fi. Send @ to restart the clock.

-   Send the correct time and date via the menu.\
    ( For example, send: T143000 for \'half past two\' and, for example,
    for the date: D090625 )

-   ![](media/image9.jpeg){width="5.4847222222222225in"
    height="2.313888888888889in"}

Small PCB design with Rotary and DS3231 RTC attached

**\
**

**Before starting\
**The clock receives the time from the internet if there is a WIFI
connection.

When a DS3231 time module is mounted on the PCB, an Internet connection
is not required.

If a rotary or push button knob is installed it can be used to set the
time.

An infrared remote control can also be installed to set the time.

To connect to the internet, the Wi-Fi station name and password must be
entered into the clock software to connect to a Wi-Fi router.\
The Wi-Fi station name and password must be entered only once. The login
information is stored in the microprocessor\'s memory.

A serial terminal app on a phone, tablet, or PC allows control without
connecting the clock to a PC with a USB cable to enter Wi-Fi
credentials.

The clock can also be controlled via a browser when connected to WIFI.

  -----------------------------------------------------------------------------------------------------------------------------------------------------
  ![](media/image10.png){width="1.8840277777777779in"   ![](media/image11.png){width="2.1875in"   ![](media/image12.png){width="2.0597222222222222in"
  height="1.9270833333333333in"}                        height="1.9381944444444446in"}            height="1.6756944444444444in"}
  ----------------------------------------------------- ----------------------------------------- -----------------------------------------------------
  BLESerial nRF                                         BLE Serial Pro                            Serial Bluetooth terminal

  -----------------------------------------------------------------------------------------------------------------------------------------------------

Download a Bluetooth UART serial terminal app on a phone, PC, or
tablet.\
For iOS: BLE Serial Pro or BLESerial nRF .\
For Android: Serial Bluetooth terminal.\
For a PC: Install the Arduino IDE on a PC and connect the clock to the
PC using a USB-C cable.

![](media/image13.jpeg){width="3.536111111111111in"
height="2.6534722222222222in"}\
Arduino ESP32 Nano on the printed circuit board in the clock.

**\
**

**\
First use**

\- Connect the clock power.

**If there\'s no internet or Bluetooth connection** , the clock can be
controlled with the rotary knob, three push buttons, or a remote
control, provided it\'s installed and activated. Activation is done in
the menu with option H01 (rotary knob), H02 push buttons, or H03 and H04
(IR remote control).\
Sometimes the knob is mounted inside the cabinet.

\- Press the dial or the center button of the three push buttons.\
The \"HOUR\" indicator will flash three times, and the hours can be set
by turning the dial or pressing the up or down buttons on the
three-button membrane.

\- A second press of the button or the middle button will make\
HETISWAS flash and the minutes can be changed. Seconds are set to 0. So
to be precise, set the last minute to exactly 0 seconds.

\- Pressing a third time will cause TWELVE to flash three times and the
LED light intensity can be adjusted.

Pressing the button a fourth time will cause the NTP and RTC signs to
flash three times. You can switch between RTC and NTP time.

\- Pressing nine times will reset the clock to factory settings.

The IR remote control can be used to operate the clock once it has been
learned. The { option in the menu prompts the clock to press buttons
corresponding to the numbers 0-9 and a few control buttons, such as
POWER.

**\
**

**Via WIFI connection**

When the clock is started for the first time, a WIFI connection will
need to be established.

If there are still old connection data in the clock, enter the capital
letter R 5 times in the menu (RRRRR) to delete it.

The clock settings are then set to:

SSID and password are empty

Timezone:CET-1CEST,M3.5.0,M10.5.0/3

WIFI=On NTP=On BLE=On

**WPS**

WPS is a method to obtain the password and the name of the WIFI station,
the SSID, from the router.

Start WPS on the clock by entering Z in the menu, and then start WPS on
the Wi-Fi router. If all goes well, the clock will restart and the time
will be correct.

**Access point in the phone**

In the WIFI connections on the phone (tablet on PC) there will be a "
StartWordclock " station next to the router you are connected to.

-   Connect to the \" StartWordclock \" station. Use the password:
    **wordclock**

-   Open a browser and type: 192.168.4.1 and press enter.

![](media/image14.jpeg){width="2.911111111111111in"
height="1.5520833333333333in"}

-   In this screen, enter the SSID and password of the WIFI router and
    press Submit

> This information is often found on the bottom of the router.

![](media/image15.png){width="2.497998687664042in"
height="2.7968285214348207in"}

The clock will restart and display the correct time.

**When the Bluetooth app is installed on the phone or tablet**

With the \'Bluetooth serial terminal\' app, the clock can be controlled
via Bluetooth with your phone, tablet or macOS PC.

\- Launch the Bluetooth terminal app on your phone or tablet

\- Click on the connection named \' wordclock \' in the serial terminal
app.

\- Find your Wi-Fi router\'s login information.\
Find the Wi-Fi router\'s SSID and password. These are typically located
on the bottom of the Wi-Fi router.

Type in:

\- aSSID and press Send.\
SSID is the name of the SSID you can find on your phone, for example.
It\'s the name to which Wi-Fi is connected.

bPASSWORD and press send.\
PASSWORD is the password you can find on the bottom of your Wi-Fi
router.

\- cBLENAME (optional, otherwise the clock will be named \' Wordclock
\')\
The BLENAME is the name of the clock in the BLE serial terminal app and
in the connected WIFI router.

If the Router Name (SSID) or Password is incorrect, the clock will
restart the Access Point page ( 192.168.4.1 ) in the phone after a few
restarts .

The correct data can be entered via this access point page, Bluetooth or
with the USB cable.

For a detailed explanation, see the chapter: Clock operation and
settings.

+-------------------------------------------+--------------------------+
| ![](media/image16.png){width="2.425in"    | ![                       |
| height="1.2465277777777777in"}            | ](media/image17.png){wid |
|                                           | th="2.376388888888889in" |
| In the FRITZBox WIFI router, for example, | height                   |
| the clock named "SteelClock-01" is        | ="2.8520833333333333in"} |
| connected to WIFI.                        |                          |
|                                           |                          |
| You can launch the clock\'s webpage in a  |                          |
| browser by tapping:                       |                          |
|                                           |                          |
| [htt                                      |                          |
| p://SteelClock-01](http://SteelClock-01/) |                          |
| or SteelClock-01.local.                   |                          |
|                                           |                          |
| The default name is: wordclock and then   |                          |
| the url to start the menu page becomes:   |                          |
| wordclock.local .                         |                          |
+===========================================+==========================+
+-------------------------------------------+--------------------------+

Option I in the menu selects a short menu for everyday use or a long
menu with option ii (2x i) for an extensive menu with all options.

**When the rotary encoder or a three-button membrane button or an IR
remote control is installed**

A rotary control, push-button control, or remote control, along with the
highly accurate DS3231 RTC clock, are optional and not required. The
time is then synchronized with the internet.\
These components are necessary if a clock cannot receive Wi-Fi.

In the clock menu, the rotary encoder (or three pushbuttons) can be
switched on or off. If no rotary encoders, pushbuttons, or remote
control are installed,\
disable the rotary encoder using the H00 option in the menu. (Open the
longer settings menu by pressing ii). Also disable\
the use of the DS3231 RTC (menu option J) so that the internet time is
used.

**When rotary encoder is installed**

The rotary knob is active for 60 seconds after pressing the button.

\- Press the dial or the center button of the three push buttons.\
The \"HOUR\" indicator will flash three times, and the hours can be set
by turning the dial or pressing the up or down buttons on the
three-button membrane.

\- A second press of the button or the middle button will make\
HETISWAS flash and the minutes can be changed. Seconds are set to 0. So
to be precise, set the last minute to exactly 0 seconds.

\- Pressing a third time will cause TWELVE to flash three times and the
LED light intensity can be adjusted.

Pressing the button a fourth time will cause the NTP and RTC signs to
flash three times.\
You can switch between RTC and NTP time.

\- Pressing nine times will reset the clock to factory settings.

**When push buttons are installed**

The push buttons are activated for 60 seconds after pressing a \*

Then enter the time with 6 digits as hhmmss .

For example, 134500 for a quarter to two.

After entering the 6th number ^the^ time is entered automatically.

**When an IR remote control is installed**

Press POWER to activate clock reception.

Use the large remote control (H03 enabled in the menu) to transmit the
correct time using digits such as hhmmss. For example, a quarter to
three would be 144500 in the afternoon or 024500 in the morning.

Alternatively, use the Up and Down buttons and CH+ and CH- buttons on a
small remote control (H04 in menu activated) to change the minutes
and/or hours.

Press ONOFF to turn the clock light on or off.

**\
**

**Making the clock case**

The construction of the clock case with lighting can be found here:

<https://ednieuw.home.xs4all.nl/Woordklok/Bouwpakket/WoordklokSK6812.htm>

or here: https://github.com/ednieuw/Arduino-ESP32-Nano-Wordclock

Instructions for a four-language word clock with SK6812 LEDs in UK, NL,
DE, and FR with Nano . https://github.com/ednieuw/FourLanguageClock

**Compilation and uploading**

The software runs on an Arduino Nano ESP32.

This software is compiled using the Arduino IDE and uploaded to the Nano
ESP32.

A compiler is available for each board that can be programmed with the
Arduino IDE. This can be selected in the board manager menu.

There are two versions available for the Arduino Nano ESP32.

One version, from Arduino itself, that uses core version 2. This version
works without any problems.

![](media/image18.png){width="5.302083333333333in"
height="4.077083333333333in"}

-   Load the file ESP32Arduino_WordClockV0xx.INO into the IDE

In the sketch, somewhere around line 50, select one of the three word
clocks by removing the two forward slashes //.

//#define FOURLANGUAGECLOCK

#define NL144CLOCK

//#define NLM1M2M3M4L114 // NL clock with four extra LEDs for the
minutes to light up

#define NL144CLOCK -\> a 144 LED single-language clock. The default
language is Dutch. For French, German, and English, copy the
four-language clock code between the NL144CLOCK definitions.

#define NLM1M2M3M4L114 -\> a 110 LED single language clock with 4 extra
LEDs for the minutes and a slightly different design

#define FOURLANGUAGECLOCK-\> a 4-language clock with 625 LEDs in a 25 x
25 LED grid.

The libraries.zip file contains the libraries needed to compile the
software.\
Unzip them into the libraries folder. This folder is usually located
between the folder containing the sketches.

Board: Arduino Nano ESP32

Partition scheme: with FAT

Pin numbering : By GPIO number ( legacy )

**Problems**

If the LEDs do not light up, there is a good chance that the pin
numbering setting\
( By GPIO number ( legacy )) is set to By Arduino pin (default).

More than one clock has been selected in the defines around line 50

Bluetooth not working

After version ESP32Arduino_WordClockV060, NimBLE version 2.0 must be
used.

For version ESP32Arduino_WordClockV060 only NimBLE version 1.4.3 works

![](media/image19.png){width="3.213888888888889in"
height="2.3229166666666665in"}
![](media/image20.png){width="2.3229166666666665in"
height="2.2756944444444445in"}

If this does not work, try the \' Alternative software installation\'
method **.**

**Alternative software installation**

![](media/image21.png){width="5.750694444444444in"
height="3.9277777777777776in"}

As described further on, software can also be installed \'Over the Air\'
on the Nano ESP32.

The advantage is that you don\'t have to install the libraries and in a
few years you won\'t have problems with incompatible libraries .

Compiling is then complete, and the file uploaded to the Nano ESP32 is
saved as a .bin file. Various .bin file versions can be found on GitHub.

<https://github.com/ednieuw/Arduino-ESP32-Nano-Wordclock>

-   Select the Nano ESP32 as the board (Tools-\>Board- ArduinoESPboards
    -\>Arduino Nano ESP32)

-   Examples , search for ArduinoOTA -\> and open the OTAWebUpdater
    program

-   Choose Sketch-\>Upload or press the upload button at the top left

-   Open the serial monitor and see which IP address is printed.\
    (If nothing prints, press the white reset button on the Nano ESP32.)

-   Enter this IP address into the URL of a browser ( eg :
    192.168.0.123)

-   Login with admin and with password admin

-   Find the .bin file and press update\
    (For example \'ESP32Arduino_WordClockV057.inoNL114.bin\' This is
    version V057 with settings for the NL114 clock)

![](media/image22.png){width="2.0833333333333335in"
height="4.473611111111111in"}**Operating the clock** \
\
To connect to a Wi-Fi network, you must enter an SSID (Wi-Fi name) and
password.\
There are a few methods:\
Connect the MCU in the clock to a PC with a micro-USB-serial cable and
use a serial terminal.\
Use a BLE serial terminal app on a phone or tablet to connect.

For a PC, the Serial monitor in the Arduino IDE is recommended.\
For iOS use: **BLE Serial Pro** or **BLESerial nRF** .\
For Android use: **Serial Bluetooth terminal** .\
\
Bluetooth Low Energy (BLE) can use two protocols: CC25nn or nRF52nn,
where nn is a specific number. This clock uses nRF52.\
- Launch the app and initiate a connection with the clock.

![](media/image23.png){width="2.033333333333333in"
height="4.384722222222222in"}Some apps automatically launch with a
connection window, but others require pressing a connection icon. There
will likely be one station, the word clock, to select from.\
- Select the clock from the list.\
The app will display a window and a line where you can enter and send
commands to the clock.\
- Pressing the letter I or i for information displays the menu, followed
by the actual settings for various preferences. A single \"i\" displays
the short menu. Double \"i\" (ii) displays a longer menu with many clock
settings.

![](media/image24.png){width="0.90625in"
height="0.9013888888888889in"}The clock has an LED with a red dot that
lights up when the program is running.\
A green dot lights up when a Wi-Fi connection is established. When a
Bluetooth connection is established, a blue dot lights up in the LED.

In both cases, **press the letter i for Information and the short menu
will appear** .\
**The long menu will appear with ii**

Enter the first letter of the setting you want to change, followed by a
code.\
Some items only toggle between On and Off. For example, enter W to turn
Wi-Fi On or Off.\
\
\
To change the SSID and password:\
Enter **A** or **a** followed by the Wi-Fi station\'s name and **B** or
**b** followed by the password.

+-------------------------------------+--------------------------------+
| **Amy-SSID** and send this command. | \_\_\_\_\_\_                   |
| For example, AFRITZ!Box01 or        | \_\_\_\_\_\_\_\_\_\_\_\_\_\_\_ |
| aFRITZ!Box01. It doesn\'t matter    | \_\_\_\_\_\_\_\_\_\_\_\_\_\_\_ |
| whether it\'s uppercase or          |                                |
| lowercase.                          | F Own colour (Hex FWWRRGGBB)   |
|                                     |                                |
| Then follows the letter B with the  | I Menu, II long menu           |
| password.\                          |                                |
| **Bmypassword** and send the        | N Display off between Nhhhh    |
| password.                           | (N2208)                        |
|                                     |                                |
| **Cbroadcastname** changes to the   | O Display On/Off               |
| name that appears in the Bluetooth  |                                |
| connection list.\                   | Q Display color choice (Q0)    |
| For example, something like:        |                                |
| cWordClock\                         | R Reset settings, @ Restart    |
| \                                   |                                |
| If the length of the SSID and/or    | \_\_\_\_\_                     |
| password is less than 5 characters, | \_\_\_\_\_\_\_\_\_\_\_\_\_\_\_ |
| the WIFI will be automatically      | \_\_\_\_\_\_\_\_\_\_\_\_\_\_\_ |
| turned off.                         |                                |
|                                     | Display off between: 10pm -    |
| Use a minimum of 8 characters for   | 8am                            |
| the SSID and password.\             |                                |
| Check in the menu (third line from  | Display choice: Rainbow        |
| the bottom) that Wi-Fi and NTP are  |                                |
| enabled.\                           | RandomDisplay is OFF           |
| \                                   |                                |
| Enter @ to restart the Nano ESP32.  | Slope: 30 Min: 20 Max: 255     |
|                                     |                                |
| It will restart and connect.\       | BLE name: ESP32Test            |
| Sometimes the clock needs to be     |                                |
| reset a second or third time before | IP address:                    |
| it can connect to Wi-Fi. If the     | 192.168.178.172/update         |
| connection still isn\'t             |                                |
| established, check the password you | WIFI=On NTP=On BLE=On FastBLE  |
| entered.                            | =Off                           |
|                                     |                                |
| When WIFI is connected , the LED on | 15/10/2025 09:08:18            |
| the Nano ESP32 will flash green.    |                                |
|                                     | \_\_\_\_\_                     |
|                                     | \_\_\_\_\_\_\_\_\_\_\_\_\_\_\_ |
|                                     | \_\_\_\_\_\_\_\_\_\_\_\_\_\_\_ |
|                                     |                                |
|                                     | \_\_\_\_                       |
|                                     | \_\_\_\_\_\_\_\_\_\_\_\_\_\_\_ |
|                                     | \_\_\_\_\_\_\_\_\_\_\_\_\_\_\_ |
|                                     |                                |
|                                     | A SSID B Password C BLE beacon |
|                                     | name                           |
|                                     |                                |
|                                     | D Date (D15012021) T Time      |
|                                     | (T132145)                      |
|                                     |                                |
|                                     | E Timezone (E\<-02\>2 or       |
|                                     | E\<+01\>-1)                    |
|                                     |                                |
|                                     | F Own colour (Hex FWWRRGGBB)   |
|                                     |                                |
|                                     | G Scan WIFI networks           |
|                                     |                                |
|                                     | H H001 rotary, H002 membrane   |
|                                     | (H000)                         |
|                                     |                                |
|                                     | I Info menu , II long menu     |
|                                     |                                |
|                                     | J DS3231 RTC module On/Off     |
|                                     |                                |
|                                     | K LDR reads/sec On/Off         |
|                                     |                                |
|                                     | N Display off between Nhhhh    |
|                                     | (N2208)                        |
|                                     |                                |
|                                     | O Display On/Off               |
|                                     |                                |
|                                     | P Status LED On/Off            |
|                                     |                                |
|                                     | Q Display choice (Q0) \~       |
|                                     | Changing                       |
|                                     |                                |
|                                     | R Reset settings, @ Restart    |
|                                     |                                |
|                                     | U Demo ( msec ) (U200) Y LED   |
|                                     | test                           |
|                                     |                                |
|                                     | S Slope , L Min, M Max (S50 L5 |
|                                     | M200)                          |
|                                     |                                |
|                                     | W WIFI X NTP& Z WPS CCC BLE +  |
|                                     | Fast BLE                       |
|                                     |                                |
|                                     | #nnn Selftest , RTC: ! See, &  |
|                                     | Update                         |
|                                     |                                |
|                                     | ) HETISWAS On/Off, ( EdSoft    |
|                                     | On/Off                         |
|                                     |                                |
|                                     | Ed Nieuwenhuys Oct 2025        |
|                                     |                                |
|                                     | \_\_\_\_\_                     |
|                                     | \_\_\_\_\_\_\_\_\_\_\_\_\_\_\_ |
|                                     | \_\_\_\_\_\_\_\_\_\_\_\_\_\_\_ |
|                                     |                                |
|                                     | Display off between: 10pm -    |
|                                     | 8am                            |
|                                     |                                |
|                                     | Display choice: Rainbow        |
|                                     |                                |
|                                     | RandomDisplay is OFF           |
|                                     |                                |
|                                     | Slope: 30 Min: 20 Max: 255     |
|                                     |                                |
|                                     | SSID: FRITZ!BoxEd              |
|                                     |                                |
|                                     | BLE name: ESP32Test            |
|                                     |                                |
|                                     | IP address:                    |
|                                     | 192.168.178.172/update         |
|                                     |                                |
|                                     | Timez                          |
|                                     | one:CET-1CEST,M3.5.0,M10.5.0/3 |
|                                     |                                |
|                                     | WIFI=On NTP=On BLE=On FastBLE  |
|                                     | =Off                           |
|                                     |                                |
|                                     | Rotary=Off Membrane=Off        |
|                                     | DS3231=Off                     |
|                                     |                                |
|                                     | WS2812 strip with 148 LEDs     |
|                                     | (switch %)                     |
|                                     |                                |
|                                     | Software:                      |
|                                     | ESP32_WordClockV116.ino        |
+=====================================+================================+
|                                     | Short (i) and long (ii) menu   |
|                                     | display                        |
+-------------------------------------+--------------------------------+

By default, the time zone is set to Amsterdam time.\
A reset using the R option in the menu will return the time zone to
Amsterdam. To set a different time zone, send the timezone string,
preceded by the character E or e.\
Many time zones are printed at the bottom of this manual.\
For example, if you live in Australia/Sydney, send the string,

eAEST-10AEDT,M10.1.0,M4.1.0/3.

The clock uses Daylight Savings Time (DST) when connected to an NTP
server, but not when the DS3231 time module is used

  ---------------------------------------------------------------------------------------------------------
  ![](media/image25.png){width="2.466666666666667in"   ![](media/image26.png){width="2.294594269466317in"
  height="5.34375in"}                                  height="5.171874453193351in"}
  ---------------------------------------------------- ----------------------------------------------------
  HTML page on iPhone                                  " Serial monitor" screen from a PC

  ---------------------------------------------------------------------------------------------------------

**\
**

**Using the remote control**

The clock can be equipped with an infrared (IR) receiver. Using a
suitable remote control, you can set the time and select one of the many
color schemes the clock can display.

-   In the menu on the PC or BLE app, turn on the IR remote control
    using H03 or H04 for a smaller IR remote control. This has fewer
    codes:\
    MIN-1, MIN+1, HOUR-1, HOUR+1, POWER, ONOFF.

-   Restart the clock with @.

-   Send **{** to teach the clock the remote control key codes.

Not all remotes will work.

-   The software will ask you to press the following keys.

\"0\", \"1\", \"2\", \"3\", \"4\", \"5\", \"6\", \"7\", \"8\", \"9\",
\"UP\", \"DOWN\", \"LEFT\", \"RIGHT\", \"POWER\", \"OK\",\"ONOFF\"

If a specific key isn\'t on your keyboard, choose a similar one.\
For example, choose the Source button when prompted to press ONOFF.

Do not use the same key for different codes.

=== Starting Learning Mode ===

Please press button: 0

Learning IR started

It is five past half past twelve LDR:110= 3% 173860 l/s 12:36:40

=== Starting Learning Mode ===

Please press button: 0

Learning IR started

It is five past half past twelve LDR:110= 3% 173860 l/s 12:36:40

Remote identified - Protocol: NEC, Address: 0xBD00

✓ Learned \'0\' - Protocol: NEC, Command: 0x0041, Address: 0xBD00

Please press button: 1

✓ Learned \'1\' - Protocol: NEC, Command: 0x0015, Address: 0xBD00

Please press button: 2

✓ Learned \'2\' - Protocol: NEC, Command: 0x0016, Address: 0xBD00

Please press button: 3

✓ Learned \'3\' - Protocol: NEC, Command: 0x0017, Address: 0xBD00

\...

Please press button: ONOFF

✓ Learned \'ONOFF\' - Protocol: NEC, Command: 0x0008, Address: 0xBD00

===Learning Complete & Saved! ===

Stored Button Mappings:

\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_

IR Remote Address : 0xBD00

IR Remote Protocol : NEC

\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_

0 -\> Cmd : 0x0041

5 -\> Cmd : 0x001A

6 -\> Cmd : 0x001B

7 -\> Cmd : 0x001D

8 -\> Cmd : 0x001E

9 -\> Cmd : 0x001F

UP -\> Cmd : 0x000B

DOWN -\> Cmd : 0x000F

LEFT -\> Cmd : 0x0049

RIGHT -\> Cmd : 0x004A

POWER -\> Cmd : 0x0001

OK -\> Cmd : 0x000D

ONOFF -\> Cmd : 0x0008

\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_

=== Now in Recognition Mode ===

Only responding to remote with Address: 0xBD00

Press any learned button to test\....

Press POWER to process the key presses at a time. POWER ON

Press POWER again to stop time entry. POWER OFF\
With POWER OFF the digits will change the Display choice

-   Press POWERON to set the time.

-   Enter six numbers as hhmmss .\
    For example, a quarter to four in the afternoon: 154500

-   or use UP and DOWN to adjust the HOUR up or down and LEFT and RIGHT
    to adjust the minutes up or down.

-   Press POWER again to stop time entry.\
    After 5 minutes, the POWER will automatically turn off.

-   With the large remote control, the number keys can be used to change
    the clock\'s\
    color display (option Q in the menu). With both the large and small
    remote controls, the ONOFF key turns the display on or off.

**Clock settings**

![](media/image27.jpeg){width="3.7152777777777777in"
height="3.625in"}The clock\'s settings can be controlled via the Wi-Fi
webpage or the Bluetooth BLE terminal app.\
When the clock is connected to Wi-Fi, it receives an IP address from the
router it\'s connected to.\
The IP address is printed in the menu.

To start the menu as a web page, the IP address of the clock must be
entered into a web browser (for example: 192.168.178.77)

local instead as a URL in the browser. (Default after a reset:
wordclock.local )

Or use a Bluetooth connection:\
- Open the BLE Terminal app. - In the app, find the clock to connect to
and connect.\
\
Each app has its own way of displaying the Bluetooth device to connect
to.\
\
The iPhone/iPad/iMac BLE serial apps are my own and work with their
default settings. Other apps may need to change the settings for sending
and receiving data. Play around with the font size and the CR and LF
settings until you\
can speed up the transfer speed on an iPhone, iPad, or iMac with the BLE
serial app by selecting the \"+ Fast BLE\" option. in the menu

Unfortunately, some apps can\'t read strings longer than 20 characters,
and you\'ll see them truncated or garbled.\
If lines are printed incompletely, enter a \"+\" to select a slower
transmission mode.\
If the transmission is too garbled and it\'s impossible to send the +
sign, try the clock\'s webpage and send a + sign.

If all else fails, you should connect the Arduino Nano ESP32 in the
clock to a PC with a USB-C cable and use the Arduino IDE to send a + or
send the letter R to reset the clock to its default settings.

**\
**

**Adjust the display light intensity**

![](media/image28.png){width="1.2395833333333333in"
height="1.1694444444444445in"}

In the menu the light intensity of the display can be adjusted with
three parameters:

\--Light intensity settings (1-250)\--

S=Slope L=Min M=Max (S80 L5 M200)

Default values are shown in ().

**S** Controls how quickly the light output of the LEDs reaches maximum
brightness.

**L** How bright the screen remains in complete darkness.

**M** the maximum brightness of the screen .

The lower half of the menu displays the saved values

Slope: 80 Min: 5 Max: 255

The clock responds to light with its LDR (light dependent resistor).

When it gets dark, the display doesn\'t turn off completely but remains
dimmed at a minimum value. The\
**L** parameter controls the minimum brightness. This brightness can be
set between 0 and 255.\
L5 is the default value.

The maximum display brightness is controlled by the **M parameter** . It
also has a value between 0 and 255. By default, it is set to a maximum
of 255.\
Parameter **S** controls the slope of how quickly the display reaches
maximum brightness.

![](media/image29.png){width="4.128472222222222in"
height="3.192361111111111in"}These values also vary between 0 and 255.
By default, it is set to 80.

**Settings are set by entering the first character of a command,
followed by parameters if necessary.**

For example:\
To set the color of the words on the clock to white, enter: Q2\
\
To turn the clock off between 10pm and 8am send: n2208\
\
Turn off WIFI by sending a W.

Restart the clock by sending the letter @.

Reset to default setting by sending R.

  -------------------------------------------------------------------------------------------------------------
  ![](media/image30.jpeg){width="2.8541666666666665in"   ![](media/image31.jpeg){width="2.8645833333333335in"
  height="5.850694444444445in"}                          height="5.561111111111111in"}
  ------------------------------------------------------ ------------------------------------------------------
  HTML page                                              BLE menu

  -------------------------------------------------------------------------------------------------------------

**Update the software**

The software can be updated over the air when the clock is connected to
Wi-Fi. The IP address is listed in the menu.

Enter the IP address of the clock directly, without a space, followed by
/update.

![](media/image32.jpeg){width="2.1145833333333335in"
height="2.790277777777778in"}Something like this:
192.168.178.141/update.

Or use: http://BLEbroadcastname.local/update

instead of the IP address.

In this case: redpcbv01.local/update.\
(Sometimes http:// can be omitted.)

![](media/image33.png){width="3.575in" height="2.017361111111111in"}

From the File menu, select the bin file you want to update.

Something like: ESP32Arduino_WordClockV100.ino.bin

where V100 is the software version number.

![](media/image34.png){width="6.268055555555556in"
height="2.09375in"}![](media/image35.png){width="6.268055555555556in"
height="2.373611111111111in"}

**Detailed menu description:**\
\
The menu allows you to set many preferences. These preferences are
stored in the ESP32 storage and are retained during power failures.\
 \
Enter the first character in the menu for the item to be modified,
followed by the parameter.\
For example, N2208 to turn off the clock light between 10:00 PM and 8:00
AM.\
Uppercase and lowercase letters are not distinguished; both are
acceptable.

When the clock is started and connected to a PC using a USC-C serial
cable, the startup log data is printed in the Arduino IDE\'s serial
terminal.

Serial started

Rotary NOT used

Rotary available

Found I2C address: 0X57

Found I2C address: 0X68

External RTC module IS found

DS3231 RTC software started

Mem.Checksum = 25065

Stored settings loaded

LED strip is SK6812

LED strip started

BLE started

10: networks found

1: FRITZ!BoxEd -47 \* \-- Will connect to

2: NETGEAR_EXT -69

3: FRITZ!BoxEd -71 \* \-- Will connect to

4: H369A209CE1 -75 \*

5: H3baas -81 \*

6: H369A209CE1 -81 \*

7: FRITZ!Box -82 \*

8: H369A209CE1 -83 \*

9: FRITZ!Box 5490 IS -83 \*

10: FRITZ!Box 5490 XL -84 \*

\[ WiFi -event\] event: 0 : WiFi interface ready

\[ WiFi -event\] event: 2 : WiFi client started

\[ WiFi -event\] event: 4 : Connected to access point

\[ WiFi -event\] event: 7 : Obtained IP address: 192.168.178.199

IP Address: 192.168.178.199

Web page started

WIFI started

01/01/2000 00:06:42

\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_

\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_

A SSID B Password C BLE beacon name

D Date (D15012021) T Time (T132145)

E Timezone (E\<-02\>2 or E\<+01\>-1)

F Own colour (Hex FWWRRGGBB)

G Scan WIFI networks

H H001 rotary, H002 membrane (H000)

I Info menu , II long menu

J Toggle use DS3231 RTC module

K LDR reads/sec toggle On/Off

N Display off between Nhhhh (N2208)

O Display toggle On/Off

P Status LED toggle On/Off

Q Display color choice (Q0)

R Reset settings, @ = Reset MCU

U Demo (msec) (U200) Y LED test

\--Light intensity settings (1-250)\--

S Slope , L Min, M Max (S50 L5 M200)

W WIFI X NTP& Z WPS CCC BLE + Fast BLE

#nnn Selftest , RTC: ! See, & Update

Ed Nieuwenhuys April 2025

\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_

**A SSID B Password C BLE beacon name**\
Enter the SSID and password of the router the clock should connect to.\
The Bluetooth broadcast name displayed in the Bluetooth Serial Terminal
app can be changed.\
For example: **a** FRITZ!Box or **A** FRITZ!Box .\
Then enter the password. For example: **B** password .

If necessary, enter **c** Bluetooth Name (default after a reset:
wordclock )\
Restart the clock by sending @ .

**CCC** Entering CCC or ccc enables or disables BLE.

Be careful when turning it off. If BLE is disabled, the clock can only
be controlled via Wi-Fi or the USB-serial port.

**D Set Date** and **T Set Time**\
If there\'s no Wi-Fi time connection and an RTC DS3231 is connected, the
date must be set manually.\
For example, enter: D06112022 to set the date to November 6, 2022.

For example, enter T132145 (or 132145, or t132145) to set the time to 45
seconds and 21 minutes past one.\
Changing the date and time only works if Wi-Fi and NTP are disabled.

**E Set time zone E\<-02\>2 or E\<+01\>-1**\
At the bottom of this page are the time zones in use in 2022.\
It\'s a pretty complicated string, so it\'s wise to copy it.\
Let\'s choose one if you live here: Antarctica/ Troll
,\"\<+00\>0\<+02\>-2,M3.5.0/1,M10.5.0/3\"\
Copy the string between the \" \"\'s and send it starting with an \'E\'
or \'e\' in front of it.\
E\<+00\>0\<+02\>-2,M3.5.0/1,M10.5.0/3

**\
**

**F Own color ( Hex WWRRGGBB)**

(In a browser or AI, type 0xff00ff to make colors hexadecimal) **\
**The color of the words displayed in the clock can be customized and
can be selected with option Q3 or Q4 in the menu.

~~\
~~The format to be entered is hexadecimal. 0123456789ABCDEF are the
characters that can be used.

The command consists of 2 numbers for White followed by 2 numbers for
Red followed by 2 for Green and ending with 2 numbers for Blue.

color the letters intense green, enter **F** 00 00 FF 00 , preceded by
the letter F. To color the letters intense red, enter FF0000, preceded
by the character F.

**F** 00 00 00 FF

To set intense blue, enter F000000FF.

For example: F8800FF00 is 50% white and 100% green.

In short: 00 is LED off, 44 is 25%, 88 is 50%, BB is 75%, and FF is
100%.

![](media/image36.png){width="2.078472222222222in"
height="0.8854166666666666in"}

WS2812 LED strips do not have a white LED and the command to control the
white has no effect.

The SK6812 LEDs have an extra white LED (yellow area) next to the three
RGB LEDs.

Which LEDs are mounted in the clock is shown at the bottom of the long
menu (ii)\
WS2812 strip with 148 LEDs (switch %)

**G Scan for nearby WIFI networks**\
Sending \'G\' will print the available nearby networks in the Bluetooth
BLE terminal.

**H H01 rotary H02 buttons H03/04 remote Sending**\
\" Hnn \" selects whether an optional rotary knob, a (membrane) push
button, or a remote control will be used to set the time and date.\
The internal, then mandatory, DS3231 RTC will then be automatically
enabled and the NTP time will be disabled.

H03 is a large remote control with numbers and H04 is for a small
version without numbers.

The software then checks whether an input is being made. Therefore,
disable the H00 option if the input is not being used to prevent
accidental input.

After a Factory Reset, the usage of this setting is left unchanged.

**Learn IR remote**

To use a remote control, the clock must first learn the infrared code a
button sends.\
Not all remote controls will work.

**I or ii to show the info menu,**\
Print the menu.

Option I (i) in the menu selects a short menu for everyday use or option
II (2x i) for a long menu with all options.

**J Switch to use the time from the DS3231 module**\
Sending \'Y\' toggles the use of the optional DS3231 time module ON or
OFF.\
When the time from the DS3231 RTC module is used, the yellow LED near
the USB connector on the Nano ESP32 will stop flashing. If the clock is
not connected to the internet, the time will expire quickly.\
Using the DS3231 time module reduces the deviation to a few seconds per
year.

The time can be entered into the DS3231 time module using option T
(time) and D (date) in the menu.

If the time isn\'t visible, send the ! command to see the internal clock
times.

**Toggle K Reads/sec**\
By entering a K the LDR reading of the measured light intensity per
second can be monitored.\
It also shows how many times per second (l/s) the processor is running
through the program and checking its tasks to keep the clock running.

TestLDR : On

LDR reading, %Out, loops per second and time

LDR: 1= 1% 205413 l/s 16:06:08

LDR: 1= 1% 215535 l/s 16:06:09

LDR: 1= 1% 215451 l/s 16:06:10

LDR: 1= 1% 215350 l/s 16:06:11

TestLDR : Off

**N Display off between Nhhhh (N2208)**\
N2208 turns off the display between 22:00 and 08:00.\
\
**O Switch display on/off**\
O Switches the display off and on.

**Turn**\
**status LEDs on and off** P Turns the status LEDs on the Arduino Nano
on and off.

**\
**

**Q Display color selection (Q0-9 )\
**Q0 displays the time with yellow words

and IT (or THE) will change from green to yellow to red within an hour.

and will change IS or WAS from green to red to yellow within a minute.\
Q1 will display a different color every hour.\
Q2 displays all text in white.\
Q3 uses custom-defined colors.

(The colors can be set with option F in the menu)

Q4 uses its own defined colors. (IT IS/WAS changes color from green to
red.)

Q5 will cycle through the rainbow colors in an hour.\
Q6 is a digital display

Q7 will display a different color every hour. (IT IS/WAS changes color
from green to red.)

Q8 will go through the rainbow colors in 4 minutes.

Q9 will cycle through the rainbow colors in 15 seconds.

**R Reset Settings**\
R resets all preferences to their default settings.

The SSID, password, BLE name, and time zone are not cleared.\
RRR does clear the last four. Wi-Fi, NTP, and BLE are enabled.

RRRRR is a combination of the R and RRR options.\
The LED strip type and the use of a rotary or membrane button remain
unchanged.

Perform a reset with RRRRR, a total reset, when the sketch is first
uploaded to the Arduino Nano ESP32.

**S = Slope L = Min M = Max (S50 L5 M200)**\
S How quickly the brightness reaches maximum brightness.\
L How bright the display is in complete darkness. M The maximum
brightness of the display.\
Values between 0 and 250

**U Demo Mode ( msec ) (U200)**

Enter U followed by the duration of a second in milliseconds.

U200 (200 milliseconds) speeds up the clock 5 times.

Sending only a U will disable demo mode.

**W=WIFI, X=NTP & , CCC=BLE**\
Turn WIFI and NTP on and off.\
Enter the character to enable or disable the option.\
It is indicated at the bottom of the menu.

Sending an & will set the NTP time into the internal Nano ESP32 RTC and
DS3231 time module.

**Y LED test**

All LEDs will go through a rainbow.

**Z WPS**

WPS is a method for obtaining the password and the Wi-Fi access point\'s
name (SSID) from the router.\
If the Wi-Fi router has a WPS button, the clock can be connected to the
internet without having to enter the router\'s name and password.

Start WPS on the clock by entering Z in the menu, and then start WPS on
the Wi-Fi router. If all goes well, the clock will restart and the time
will be correct.

Sometimes you need to hold the WPS button on the router for a few
seconds.

How to start WPS differs per router.

**+ = Fast BLE**\
The BLE UART protocol sends Standard packets are 20 bytes long. There is
a 50 msec delay between each packet .\
The iOS BLEserial app, and perhaps others, can receive packets of 80
bytes or more before missing any characters. This allows the menu to
print faster.\
Option + toggles between long and short packets.

**\~ = Change display mode per minute or hour**

\~ Starts the display mode change option and changes every minute.\
\~1 changes the display mode every minute.

\~2 changes the display mode every hour.

\~0 or \~ disables the toggle option.

**! = Show NTP, RTC, and DS3231 time\
!** will display the NTP, RTC, and DS3231 time as stored in the clock.
The DS3231 time module must be installed for the module to display a
realistic time.\
The same as the & option, but this option will not be updated from the
internet NTP server; it only displays the time.

**#nnn = Selftest\
**Sending a \# will start the clock self-test. This is useful for
checking that all words in the clock are functioning.

Starting in V062, #nnnn is also possible. Nnnn is the millisecond delay
between each word.

The self-test will stop automatically after one cycle.

**% = Switch between SK6812 and WS2812 LED strips\
**. This option allows you to switch the LED strip in use. The clock is
equipped with one of these two types of LED strips.

Resetting all settings by sending an R in the menu does not change the
LED strip selection.

**\$ = Fireplace\
**Displays a fireplace on a 144 LED clock **.** The result will be less
attractive on other types of clocks.**\
**

**( = Turns additional optional LEDs On/Off**

The clock can optionally be equipped with an additional logo text.

**) = Turns IT/IS/WAS On/Off**

IT WAS always remains out

**@ = Restart the clock\
**@ restarts the clock. This is useful if the SSID, etc., changes and
the program needs to be restarted.\
Settings are preserved.

**& = Update with NTP\
**Synchronizes the RTC module with the NTP time **\
**The RTC time module in the clock can control the time. The T and D
menu options allow you to manually enter the time and date.\
This option copies the NTP time to the RTC module.

The computer inside the clock has its own internal clock. This clock
moves forward seconds per day and is regularly updated with the NTP time
or the DS3231 RTC time module. If NTP and the DS3231 RTC time module are
disabled, the clock will still display the time, but after a few days,
it will become out of sync with the correct time.

**123456 Setting the time in the DS3231 RTC time module**

Enter the time as 152300 hhmmss . Same as T152300.\
Changing the date and time only works if NTP is disabled.

**\
**

**Time zones**

Copy the text **between the quotes** and paste it after the E (or e)
character and send it to the clock.

For the Netherlands and Belgium send: eCET-1CEST, M3.5.0, M10.5.0/3

+-------------+--------------+-------------+--------------+----------+
| Africa      | Americ       | America/T   | Asia/Tok     | Indian   |
| /Abidj      | a/Detroit,\" | egucigalpa, | yo,\"JST-9\" | /Antan   |
| an,\'GMT0\' | EST5EDT,M3.2 | \"CST6\"    |              | anarivo, |
|             | .0,M11.1.0\" |             | Asia/ Tomsk  | \        |
| Africa      |              | America     | ,\           | "EAT-3\" |
| /Acc        | America      | /Thule,\"AS | "\<+07\>-7\" |          |
| ra,\'GMT0\' | /Dominica,   | T4ADT,M3.2. |              | Indian   |
|             | \"AST4\"     | 0,M11.1.0\" | Asia/U       | /Chag    |
| Africa      |              | Am          | laanbaatar,\ | os,\"\<+ |
| /Addis_Abab | America      | erica/Thund | "\<+08\>-8\" | 06\>-6\" |
| a,\'EAT-3\' | /Edmonton,\" | er_Bay,\"ES |              |          |
|             | MST7MDT,M3.2 | T5EDT,M3.2. | Asia/ Urumqi | Indian   |
| Africa      | .0,M11.1.0\" | 0,M11.1.0\" | ,\           | /Christm |
| /Algier     |              |             | "\<+06\>-6\" | as,\"\<+ |
| s,\'CET-1\' | America /    | America     |              | 07\>-7\" |
|             | Eirunepe     | /T          | Asia/        |          |
| Africa      | ,            | ijuana,\"PS | Ust-Nera     | Indian   |
| /Asmara,    | \"\<-05\>5\" | T8PDT,M3.2. | ,\"          | /        |
| \"EAT-3\"   |              | 0,M11.1.0\" | \<+10\>-10\" | Cocos,\" |
|             | America      |             |              | \<+0630\ |
| Africa/Bama | /El_Salva    | America/T   | Asia         | >-6:30\" |
| ko,\'GMT0\' | dor,\"CST6\" | oronto,\"ES | /Vientiane,\ |          |
|             |              | T5EDT,M3.2. | "\<+07\>-7\" | Indian / |
| A           | America      | 0,M11.1.0\" |              | Comoro , |
| frica/Bangu | /Fortaleza,  |             | Asia/Vl      | \        |
| i,\"WAT-1\" | \"\<-03\>3\" | Ameri       | adivostok,\" | "EAT-3\" |
|             |              | ca/Tortola, | \<+10\>-10\" |          |
| Africa/Banj | America      | \"AST4\"    |              | Indian   |
| ul,\'GMT0\' | /Fort_Nel    |             | Asia/        | /Kerguel |
|             | son,\"MST7\" | America/Van | Yakutsk      | en,\"\<+ |
| Africa/Biss |              | couver,\"PS | ,\           | 05\>-5\" |
| au,\'GMT0\' | America      | T8PDT,M3.2. | "\<+09\>-9\" |          |
|             | /            | 0,M11.1.0\" |              | Indian   |
| Afr         | Glace_Bay,\" |             | Asia/Y       | /Ma      |
| ica/Blantyr | AST4ADT,M3.2 | Ameri       | angon,\"\<+0 | he,\"\<+ |
| e,\"CAT-2\" | .0,M11.1.0\" | ca/Whitehor | 630\>-6:30\" | 04\>-4\" |
|             |              | se,\"MST7\" |              |          |
| Africa      | America /    |             | Asia/        | Indian   |
| /Brazzavill | Godthab      | America/Wi  | Y            | /Maldiv  |
| e,\"WAT-1\" | ,\"\<        | nnipeg,\"CS | ekaterinburg | es,\"\<+ |
|             | -03\>3\<-02\ | T6CDT,M3.2. | ,\           | 05\>-5\" |
| Afri        | >,M3.5.0/-2, | 0,M11.1.0\" | "\<+05\>-5\" |          |
| ca/Bujumbur | M10.5.0/-1\" |             |              | Indian   |
| a,\'CAT-2\' |              | America/Yak | Asia/        | /Mauriti |
|             | America/     | utat,\"AKST | Yerevan      | us,\"\<+ |
| Af          | Goose_Bay,\" | 9AKDT,M3.2. | ,\           | 04\>-4\" |
| rica/Cairo, | AST4ADT,M3.2 | 0,M11.1.0\" | "\<+04\>-4\" |          |
| \"EET-2\"   | .0,M11.1.0\" |             |              | Indian   |
|             |              | Am          | Atlant       | /M       |
| Africa/Ca   | America/G    | erica/Yello | ic/Azores,\" | ayotte,\ |
| sablanca,\" | rand_Turk,\" | wknife,\"MS | \<-01\>1\<+0 | "EAT-3\" |
| \<+01\>-1\" | EST5EDT,M3.2 | T7MDT,M3.2. | 0\>,M3.5.0/0 |          |
|             | .0,M11.1.0\" | 0,M11.1.0\" | ,M10.5.0/1\" | Indian / |
| Africa/Ceu  |              |             |              | Reunion  |
| ta,\"CET-1C | Amer         | Antarctic   | Atlanti      | ,\"\<+   |
| EST,M3.5.0, | ica/Grenada, | a/Casey,\"\ | c/Bermuda,\" | 04\>-4\" |
| M10.5.0/3\" | \"AST4\"     | <+11\>-11\" | AST4ADT,M3.2 |          |
|             |              |             | .0,M11.1.0\" | Pac      |
| A           | America      | Antarcti    |              | ific/Api |
| frica/Conak | /Guadeloupe, | ca/Davis,\" | Atlantic/    | a,\"\<+1 |
| ry,\"GMT0\" | \"AST4\"     | \<+07\>-7\" | Canary,\"WET | 3\>-13\" |
|             |              |             | 0WEST,M3.5.0 |          |
| Af          | Americ       | Antarctica/ | /1,M10.5.0\" | Pacific/ |
| rica/Dakar, | a/Guatemala, | Dum         |              | Auckland |
| \'GMT0\'    | \"CST6\"     | ontDUrville | Atlantic/    | ,\"NZST- |
|             |              | ,\"\        | Cape_Verde   | 12NZDT,M |
| Africa/D    | Americ       | <+10\>-10\" | ,            | 9.5.0,M4 |
| ar_es_Salaa | a/Guayaquil, |             | \"\<-01\>1\" | .1.0/3\" |
| m,\"EAT-3\" | \"\<-05\>5\" | Antarctic   |              |          |
|             |              | a/Macquarie | Atlantic     | Paci     |
| Afr         | Ame          | ,\"AEST-10A | /Faroe,\"WET | fic/Boug |
| ica/Djibout | rica/Guyana, | EDT,M10.1.0 | 0WEST,M3.5.0 | ainville |
| i,\"EAT-3\" | \"\<-04\>4\" | ,M4.1.0/3\" | /1,M10.5.0\" | a,\"\<+1 |
|             |              |             |              | 1\>-11\" |
| Af          | America      | Antarctica/ | Atlantic/M   |          |
| rica/Douala | /Halifax,\"  | Mawson      | adeira,\"WET | Paci     |
| ,\"WHAT-1\" | AST4ADT,M3.2 | ,\"         | 0WEST,M3.5.0 | fic/Chat |
|             | .0,M11.1.0\" | \<+05\>-5\" | /1,M10.5.0\" | ham,\"\< |
| Africa/     |              |             |              | +1245\>- |
| El_Aaiun    | America      | Antarc      | Atlanti      | 12:45\<+ |
| ,\"         | /H           | tica/McMurd | c/Reykjavik, | 1345\>,M |
| \<+01\>-1\" | avana,\"CST5 | o,\"NZST-12 | \'GMT0\'     | 9.5.0/2: |
|             | CDT,M3.2.0/0 | NZDT,M9.5.0 |              | 45,M4.1. |
| Afric       | ,M11.1.0/1\" | ,M4.1.0/3\" | Atlantic/    | 0/3:45\" |
| a/Freetown, |              |             | S            |          |
| \'GMT0\'    | America      | Antarctica  | outh_Georgia | Paci     |
|             | /Hermosillo, | /Palmer,\   | ,            | fic/Chuu |
| Afr         | \"MST7\"     | "\<-03\>3\" | \"\<-02\>2\" | k,\"\<+1 |
| ica/Gaboron |              |             |              | 0\>-10\" |
| e,\"CAT-2\" | America      | Antarctica  | Atlan        |          |
|             | /Indiana/Ind | / Rothera   | tic/Stanley, | Pacifi   |
| Afr         | ianapolis,\" | ,\          | \"\<-03\>3\" | c/Easter |
| ica/Harare, | EST5EDT,M3.2 | "\<-03\>3\" |              | ,\"\<-06 |
| \"CAT-2\"   | .0,M11.1.0\" |             | Atlantic/St  | \>6\<-05 |
|             |              | Antarctica  | Helena,      | \>,M9.1. |
| Africa/J    | America/Ind  | / Syowa     | \'GMT0\'     | 6/22,M4. |
| ohannesburg | iana/Knox,\" | ,\"         |              | 1.6/22\" |
| ,\'SAST-2\' | CST6CDT,M3.2 | \<+03\>-3\" | Austr        |          |
|             | .0,M11.1.0\" |             | alia/Adelaid | Paci     |
| Africa/Jub  |              | Antarctica  | e,\"ACST-9:3 | fic/Efat |
| a,\"CAT-2\" | Am           | / Troll     | 0ACDT,M10.1. | e,\"\<+1 |
|             | erica/Indian | ,\"\<+00    | 0,M4.1.0/3\" | 1\>-11\" |
| Afri        | a/Marengo,\" | \>0\<+02\>- |              |          |
| ca/Kampala, | EST5EDT,M3.2 | 2,M3.5.0/1, | Austral      | Pacific/ |
| \"EAT-3\"   | .0,M11.1.0\" | M10.5.0/3\" | ia/Brisbane, | E        |
|             |              |             | \'AEST-10\'  | nderbury |
| Afr         | Ameri        | Antarctica  |              | ,\"\<+1  |
| ica/Khartou | ca/Indiana/P | / Vostok    | Australi     | 3\>-13\" |
| m,\"CAT-2\" | etersburg,\" | ,\"         | a/Broken_Hil |          |
|             | EST5EDT,M3.2 | \<+06\>-6\" | l,\"ACST-9:3 | Pacifi   |
| A           | .0,M11.1.0\" |             | 0ACDT,M10.1. | c/Fakaof |
| frica/Kigal |              | Arctic      | 0,M4.1.0/3\" | o,\"\<+1 |
| i,\"CAT-2\" | Amer         | /Longyearby |              | 3\>-13\" |
|             | ica/Indiana/ | en,\"CET-1C | A            |          |
| Afri        | Tell_City,\" | EST,M3.5.0, | ustralia/Cur | Paci     |
| ca/Kinshasa | CST6CDT,M3.2 | M10.5.0/3\" | rie,\"AEST-1 | fic/Fiji |
| ,\'WHAT-1\' | .0,M11.1.0\" |             | 0AEDT,M10.1. | ,\"\<+12 |
|             |              | A           | 0,M4.1.0/3\" | \>-12\<+ |
| Africa/Lago | America/Indi | sia/Aden,\" |              | 13\>,M11 |
| s,\"WAT-1\" | ana/Vevay,\" | \<+03\>-3\" | Austra       | .2.0,M1. |
|             | EST5EDT,M3.2 |             | lia/Darwin,\ | 2.3/99\" |
| Africa      | .0,M11.1.0\" | Asi         | "ACST-9:30\" |          |
| /Libreville |              | a/Almaty,\" |              | Pacific  |
| ,\"WHAT-1\" | Amer         | \<+06\>-6\" | Australia/   | /Funafut |
|             | ica/Indiana/ |             | Eucla        | i,\"\<+1 |
| Africa/Lo   | Vincennes,\" | Asia/Amman, | ,\"\<+0      | 2\>-12\" |
| me,\'GMT0\' | EST5EDT,M3.2 | \"EET-2EEST | 845\>-8:45\" |          |
|             | .0,M11.1.0\" | ,M2.5.4/24, |              | Pacifi   |
| Afr         |              | M10.5.5/1\" | A            | c/Galapa |
| ica/Luanda, | Am           |             | ustralia/Hob | gos,\"\< |
| \"WHAT-1\"  | erica/Indian | Asia        | art,\"AEST-1 | -06\>6\" |
|             | a/Winamac,\" | /Anadyr,\"\ | 0AEDT,M10.1. |          |
| Afric       | EST5EDT,M3.2 | <+12\>-12\" | 0,M4.1.0/3\" | Paci     |
| a/Lubumbash | .0,M11.1.0\" |             |              | fic/Gamb |
| i,\"CAT-2\" |              | Asia/ Aqtau | Austra       | ier,\"\< |
|             | Ameri        | ,\"         | lia/Lindeman | -09\>9\" |
| A           | ca/Inuvik,\" | \<+05\>-5\" | ,\"AEST-10\" |          |
| frica/Lusak | MST7MDT,M3.2 |             |              | Pacific  |
| a,\'CAT-2\' | .0,M11.1.0\" | Asia/       | Australia/   | /Gu      |
|             |              | Aqtobe      | Lord_Howe    | adalcana |
| Afr         | Americ       | ,\"         | ,\"\<+103    | l,\"\<+1 |
| ica/Malabo, | a/Iqaluit,\" | \<+05\>-5\" | 0\>-10:30\<+ | 1\>-11\" |
| \"WHAT-1\"  | EST5EDT,M3.2 |             | 11\>-11,M10. |          |
|             | .0,M11.1.0\" | Asia/       | 1.0,M4.1.0\" | Pacific  |
| A           |              | Ashgabat    |              | /        |
| frica/Maput | America/Jama | ,\"         | Aust         | Guam,\"C |
| o,\"CAT-2\" | ica,\"EST5\" | \<+05\>-5\" | ralia/Melbou | hST-10\" |
|             |              |             | rne,\"AEST-1 |          |
| Af          | America      | Asi         | 0AEDT,M10.1. | Pacific  |
| rica/Maseru | /Juneau,\"AK | a/Atyrau,\" | 0,M4.1.0/3\" | /H       |
| ,\"SAST-2\" | ST9AKDT,M3.2 | \<+05\>-5\" |              | onolulu, |
|             | .0,M11.1.0\" |             | Aust         | \        |
| Afr         |              | Asia        | ralia/Perth, | "HST10\" |
| ica/Mbabane | Americ       | /Baghdad,\" | \"AWST-8\"   |          |
| ,\'SAST-2\' | a/Kentucky/L | \<+03\>-3\" |              | Pacific  |
|             | ouisville,\" |             | A            | /        |
| Africa      | EST5EDT,M3.2 | Asia        | ustralia/Syd | Ki       |
| /Mogadishu, | .0,M11.1.0\" | /Bahrain,\" | ney,\"AEST-1 | ritimati |
| \'EAT-3\'   |              | \<+03\>-3\" | 0AEDT,M10.1. | ,\"\<+1  |
|             | Americ       |             | 0,M4.1.0/3\" | 4\>-14\" |
| Af          | a/Kentucky/M | A           |              |          |
| rica/Monrov | onticello,\" | sia/Baku,\" | Europe/Amst  | Pacific  |
| ia,\'GMT0\' | EST5EDT,M3.2 | \<+04\>-4\" | erdam,\"CET- | / Kosrae |
|             | .0,M11.1.0\" |             | 1CEST,M3.5.0 | ,\"\<+1  |
| Afri        |              | Asia        | ,M10.5.0/3\" | 1\>-11\" |
| ca/Nairobi, | America /    | /Bangkok,\" |              |          |
| \'EAT-3\'   | Kralendijk , | \<+07\>-7\" | Europe/An    | Pacific  |
|             | \"AST4\"     |             | dorra,\"CET- | /        |
| Afr         |              | Asia        | 1CEST,M3.5.0 | K        |
| ica/Ndjamen | America /    | /Barnaul,\" | ,M10.5.0/3\" | wajalein |
| a,\"WAT-1\" | La_Paz       | \<+07\>-7\" |              | ,\"\<+1  |
|             | ,            |             | Europe       | 2\>-12\" |
| A           | \"\<-04\>4\" | Asia/Beirut | /Astrakhan,\ |          |
| frica/Niame |              | ,\"EET-2EES | "\<+04\>-4\" | Pacific  |
| y,\"WAT-1\" | America      | T,M3.5.0/0, |              | / Majuro |
|             | /Lima,       | M10.5.0/0\" | Europe/Ath   | ,\"\<+1  |
| Afri        | \"\<-05\>5\" |             | ens,\"EET-2E | 2\>-12\" |
| ca/Nouakcho |              | Asia        | EST,M3.5.0/3 |          |
| tt,\'GMT0\' | America      | /Bishkek,\" | ,M10.5.0/4\" | Pacific  |
|             | /Lo          | \<+06\>-6\" |              | /Mar     |
| Afric       | s_Angeles,\" |             | Europe/Bel   | quesas,\ |
| a/Ouagadoug | PST8PDT,M3.2 | Asi         | grade,\"CET- | "\<-0930 |
| ou,\'GMT0\' | .0,M11.1.0\" | a/Brunei,\" | 1CEST,M3.5.0 | \>9:30\" |
|             |              | \<+08\>-8\" | ,M10.5.0/3\" |          |
| Africa/     | America      |             |              | Pacific  |
| Porto-Novo, | /Lower_Prin  | Asia/ Chita | Europe/B     | / Midway |
| \"WAT-1\"   | ces,\"AST4\" | ,\"         | erlin,\"CET- | ,        |
|             |              | \<+09\>-9\" | 1CEST,M3.5.0 | \        |
| Af          | America /    |             | ,M10.5.0/3\" | "SST11\" |
| rica/Sao_To | Maceio       | Asia/Ch     |              |          |
| me,\'GMT0\' | ,            | oibalsan,\" | Europe/Brati | Pacific  |
|             | \"\<-03\>3\" | \<+08\>-8\" | slava,\"CET- | /Naur    |
| Afri        |              |             | 1CEST,M3.5.0 | u,\"\<+1 |
| ca/Tripoli, | America      | Asia/Colo   | ,M10.5.0/3\" | 2\>-12\" |
| \"EET-2\"   | /Managua,    | mbo,\"\<+05 |              |          |
|             | \"CST6\"     | 30\>-5:30\" | Europe/Bru   | Pacific  |
| Af          |              |             | ssels,\"CET- | /Ni      |
| rica/Tunis, | America /    | As          | 1CEST,M3.5.0 | ue,\"\<- |
| \'CET-1\'   | Manaus       | ia/Damascus | ,M10.5.0/3\" | 11\>11\" |
|             | ,            | ,\"EET-2EES |              |          |
| Afr         | \"\<-04\>4\" | T,M3.5.5/0, | E            | Pacifi   |
| ica/Windhoe |              | M10.5.5/0\" | urope/Buchar | c/Norfol |
| k,\"CAT-2\" | America /    |             | est,\"EET-2E | k,\"\<+1 |
|             | Marigot ,    | As          | EST,M3.5.0/3 | 1\>-11\< |
| America     | \"AST4\"     | ia/Dhaka,\" | ,M10.5.0/4\" | +12\>,M1 |
| /Adak,\"HST |              | \<+06\>-6\" |              | 0.1.0,M4 |
| 10HDT,M3.2. | America /    |             | Europe/Bud   | .1.0/3\" |
| 0,M11.1.0\" | Martinique , | A           | apest,\"CET- |          |
|             | \"AST4\"     | sia/Dili,\" | 1CEST,M3.5.0 | Pacific/ |
| Am          |              | \<+09\>-9\" | ,M10.5.0/3\" | Noumea   |
| erica/Ancho | America      |             |              | ,\"\<+1  |
| rage,\"AKST | /            | As          | Europe/Bus   | 1\>-11\" |
| 9AKDT,M3.2. | Matamoros,\" | ia/Dubai,\" | ingen,\"CET- |          |
| 0,M11.1.0\" | CST6CDT,M3.2 | \<+04\>-4\" | 1CEST,M3.5.0 | Pac      |
|             | .0,M11.1.0\" |             | ,M10.5.0/3\" | ific/Pag |
| Americ      |              | Asia/       |              | o_Pago,\ |
| a/Anguilla, | America      | Dushanbe    | Europe/Chi   | "SST11\" |
| \"AST4\"    | /Mazatlan,\" | ,\"         | sinau,\"EET- |          |
|             | MST7MDT,M4.1 | \<+05\>-5\" | 2EEST,M3.5.0 | Pac      |
| America /   | .0,M10.5.0\" |             | ,M10.5.0/3\" | ific/Pal |
| Antigua ,   |              | Asi         |              | au,\"\<+ |
| \"AST4\"    | America/     | a/Famagusta | Europe/Copen | 09\>-9\" |
|             | Menominee,\" | ,\"EET-2EES | hagen,\"CET- |          |
| America /   | CST6CDT,M3.2 | T,M3.5.0/3, | 1CEST,M3.5.0 | Pacif    |
| Araguaina   | .0,M11.1.0\" | M10.5.0/4\" | ,M10.5.0/3\" | ic/Pitca |
| ,\          |              |             |              | irn,\"\< |
| "\<-03\>3\" | Ameri        | Asia        | Europe/D     | -08\>8\" |
|             | ca/Merida,\" | /Gaza,      | ublin,\"IST- |          |
| America     | CST6CDT,M4.1 | \'EET-2EEST | 1GMT0,M10.5. | Pacifi   |
| /Argentina/ | .0,M10.5.0\" | ,M3.4.4/48, | 0,M3.5.0/1\" | c/Pohnpe |
| B           |              | M10.5.5/1\' |              | i,\"\<+1 |
| uenos_Aires | America/Met  |             | Europe/Gibr  | 1\>-11\" |
| ,\          | lakatla,\"AK | A           | altar,\"CET- |          |
| "\<-03\>3\" | ST9AKDT,M3.2 | sia/Hebron, | 1CEST,M3.5.0 | Pacific/ |
|             | .0,M11.1.0\" | \"EET-2EEST | ,M10.5.0/3\" | Port     |
| America     |              | ,M3.4.4/48, |              | _Moresby |
| /Argentina/ | America      | M10.5.5/1\" | Europe/G     | ,\"\<+1  |
| Catamarca,\ | /Me          |             | uernsey,\"GM | 0\>-10\" |
| '\<-03\>3\' | xico_City,\" | Asia/       | T0BST,M3.5.0 |          |
|             | CST6CDT,M4.1 | Ho_Chi_Minh | /1,M10.5.0\" | Pacific  |
| America     | .0,M10.5.0\" | ,\"         |              | /Raroton |
| /Argentina/ |              | \<+07\>-7\" | Europe/Helsi | ga,\"\<- |
| Cordoba     | America /    |             | nki,\"EET-2E | 10\>10\" |
| ,\          | Miquelon     | As          | EST,M3.5.0/3 |          |
| "\<-03\>3\" | ,\"\<-03\>3  | ia/Hong_Kon | ,M10.5.0/4\" | Pa       |
|             | \<-02\>,M3.2 | g,\"HKT-8\" |              | cific/Sa |
| America     | .0,M11.1.0\" |             | Europe/Isle  | ipan,\"C |
| /Argent     |              | Asia/ Hovd  | _of_Man,\"GM | hST-10\" |
| ina/Jujuy,\ | America      | ,\"         | T0BST,M3.5.0 |          |
| "\<-03\>3\" | /Moncton,\"  | \<+07\>-7\" | /1,M10.5.0\" | Paci     |
|             | AST4ADT,M3.2 |             |              | fic/Tahi |
| America     | .0,M11.1.0\" | Asia        | Europ        | ti,\"\<- |
| /Argentina/ |              | /Irkutsk,\" | e/Istanbul,\ | 10\>10\" |
| La_Rioja    | America      | \<+08\>-8\" | "\<+03\>-3\" |          |
| ,\          | /            |             |              | Pacif    |
| "\<-03\>3\" | Monterrey,\" | Asia/Jakart | Europe       | ic/Taraw |
|             | CST6CDT,M4.1 | a,\"WIB-7\" | /Jersey,\"GM | a,\"\<+1 |
| America     | .0,M10.5.0\" |             | T0BST,M3.5.0 | 2\>-12\" |
| /Argentin   |              | Asi         | /1,M10.5.0\" |          |
| a/Mendoza,\ | America      | a/Jayapura, |              | Pacific/ |
| "\<-03\>3\" | /Montevideo, | \"WHITE-9\" | Europe/      | T        |
|             | \"\<-03\>3\" |             | Kaliningrad, | ongatapu |
| America     |              | A           | \'EET-2\'    | ,\"\<+1  |
| /Argentina/ | America      | sia/Jerusal |              | 3\>-13\" |
| R           | /Montreal,\" | em,\"IST-2I | Europe/K     |          |
| io_Gallegos | EST5EDT,M3.2 | DT,M3.4.4/2 | iev,\"EET-2E | Pac      |
| ,\          | .0,M11.1.0\" | 6,M10.5.0\" | EST,M3.5.0/3 | ific/Wak |
| "\<-03\>3\" |              |             | ,M10.5.0/4\" | e,\"\<+1 |
|             | America      | Asia/Ka     |              | 2\>-12\" |
| America     | /Montser     | bul,\"\<+04 | Eu           |          |
| /Argent     | rat,\"AST4\" | 30\>-4:30\" | rope/Kirov,\ | Pacif    |
| ina/Salta,\ |              |             | '\<+03\>-3\' | ic/Valai |
| "\<-03\>3\" | America      | Asia/       |              | s,\"\<+1 |
|             | /Nassau,\"   | Kamchatka   | Europe/      | 2\>-12\" |
| America     | EST5EDT,M3.2 | ,\"\        | Lisbon,\"WET |          |
| /Argentina/ | .0,M11.1.0\" | <+12\>-12\" | 0WEST,M3.5.0 | Etc      |
| San_Juan    |              |             | /1,M10.5.0\" | /GMT,    |
| ,\          | America      | As          |              | \"GMT0\" |
| "\<-03\>3\" | /New_York,\" | ia/Karachi, | Europe/Ljub  |          |
|             | EST5EDT,M3.2 | \"PKT-5\"   | ljana,\"CET- | Etc      |
| America     | .0,M11.1.0\" |             | 1CEST,M3.5.0 | /GMT-0,  |
| /Argentina/ |              | Asia/Kathma | ,M10.5.0/3\" | \"GMT0\" |
| San_Luis    | America      | ndu,\"\<+05 |              |          |
| ,\          | /Nipigon,\"  | 45\>-5:45\" | Europe       | Etc      |
| "\<-03\>3\" | EST5EDT,M3.2 |             | /London,\"GM | /GMT     |
|             | .0,M11.1.0\" | Asia/       | T0BST,M3.5.0 | -1,\"\<+ |
| America     |              | Khandyga    | /1,M10.5.0\" | 01\>-1\" |
| /Argentina/ | Ameri        | ,\"         |              |          |
| Tucuman     | ca/Nome,\"AK | \<+09\>-9\" | Europe/Luxem | Etc      |
| ,\          | ST9AKDT,M3.2 |             | bourg,\"CET- | /GMT     |
| "\<-03\>3\" | .0,M11.1.0\" | Asi         | 1CEST,M3.5.0 | -2,\"\<+ |
|             |              | a/Kolkata,\ | ,M10.5.0/3\" | 02\>-2\" |
| America     | Amer         | "IST-5:30\" |              |          |
| /Argentin   | ica/Noronha, |             | Europe/M     | Etc      |
| a/Ushuaia,\ | \"\<-02\>2\" | Asia/       | adrid,\"CET- | /GMT     |
| "\<-03\>3\" |              | Krasnoyarsk | 1CEST,M3.5.0 | -3,\"\<+ |
|             | America/     | ,\"         | ,M10.5.0/3\" | 03\>-3\" |
| America     | North_Dakota | \<+07\>-7\" |              |          |
| /Aruba,     | /Beulah,\"   |             | Europe/      | Etc      |
| \"AST4\"    | CST6CDT,M3.2 | Asia/       | Malta,\"CET- | /GMT     |
|             | .0,M11.1.0\" | K           | 1CEST,M3.5.0 | -4,\"\<+ |
| America /   |              | uala_Lumpur | ,M10.5.0/3\" | 04\>-4\" |
| Asuncion    | America/     | ,\"         |              |          |
| ,\"\<-      | North_Dakota | \<+08\>-8\" | Europe       | Etc      |
| 04\>4\<-03\ | /Center,\"   |             | /Marieh      | /GMT     |
| >,M10.1.0/0 | CST6CDT,M3.2 | Asia/       | amn,\"EET-2E | -5,\"\<+ |
| ,M3.4.0/0\" | .0,M11.1.0\" | Kuching     | EST,M3.5.0/3 | 05\>-5\" |
|             |              | ,\"         | ,M10.5.0/4\" |          |
| America     | America/     | \<+08\>-8\" |              | Etc      |
| /Atikok     | North_Dakota |             | Europe       | /GMT     |
| an,\"EST5\" | /            | Asi         | /Minsk,\     | -6,\"\<+ |
|             | New_Salem,\" | a/Kuwait,\" | '\<+03\>-3\' | 06\>-6\" |
| America /   | CST6CDT,M3.2 | \<+03\>-3\" |              |          |
| Bahia       | .0,M11.1.0\" |             | Europe/M     | Etc      |
| ,\          |              | Asia/ Macau | onaco,\"CET- | /GMT     |
| "\<-03\>3\" | Ameri        | , \"CST-8\" | 1CEST,M3.5.0 | -7,\"\<+ |
|             | ca/Nuuk,\"\< |             | ,M10.5.0/3\" | 07\>-7\" |
| America     | -03\>3\<-02\ | Asia/       |              |          |
| /Bahia_Ba   | >,M3.5.0/-2, | Magadan     | Europe/Mosc  | Etc      |
| nderas,\"CS | M10.5.0/-1\" | ,\"\        | ow,\"MSK-3\" | /GMT     |
| T6CDT,M4.1. |              | <+11\>-11\" |              | -8,\"\<+ |
| 0,M10.5.0\" | Americ       |             | Europe       | 08\>-8\" |
|             | a/Ojinaga,\" | As          | /Oslo,\"CET- |          |
| America     | MST7MDT,M3.2 | ia/Makassar | 1CEST,M3.5.0 | Etc      |
| /Barbados,  | .0,M11.1.0\" | ,\"WITA-8\" | ,M10.5.0/3\" | /GMT     |
| \"AST4\"    |              |             |              | -9,\"\<+ |
|             | Ame          | A           | Europe/      | 09\>-9\" |
| America     | rica/Panama, | sia/Manila, | Paris,\"CET- |          |
| /Belem,\    | \"EST5\"     | \"PST-8\"   | 1CEST,M3.5.0 | Etc      |
| "\<-03\>3\" |              |             | ,M10.5.0/3\" | /GMT-1   |
|             | America/Pa   | Asia/       |              | 0,\"\<+1 |
| America /   | ngnirtung,\" | Muscat      | Europe/Podg  | 0\>-10\" |
| Belize ,    | EST5EDT,M3.2 | ,\"         | orica,\"CET- |          |
| \"CST6\"    | .0,M11.1.0\" | \<+04\>-4\" | 1CEST,M3.5.0 | Etc      |
|             |              |             | ,M10.5.0/3\" | /GMT-1   |
| America     | America      | A           |              | 1,\"\<+1 |
| /Blanc-     | /Paramaribo, | sia/Nicosia | Europe/P     | 1\>-11\" |
| Sablon ,    | \"\<-03\>3\" | ,\"EET-2EES | rague,\"CET- |          |
| \"AST4\"    |              | T,M3.5.0/3, | 1CEST,M3.5.0 | Etc      |
|             | Amer         | M10.5.0/4\" | ,M10.5.0/3\" | /GMT-1   |
| America /   | ica/Phoenix, |             |              | 2,\"\<+1 |
| Boa_Vista   | \"MST7\"     | Asia/Novo   | Europe/R     | 2\>-12\" |
| ,\          |              | kuznetsk,\" | iga,\'EET-2E |          |
| "\<-04\>4\" | A            | \<+07\>-7\" | EST,M3.5.0/3 | Etc      |
|             | merica/Port- |             | ,M10.5.0/4\' | /GMT-1   |
| America /   | au-Prince,\" | Asia/Nov    |              | 3,\"\<+1 |
| Bogota      | EST5EDT,M3.2 | osibirsk,\" | Europe       | 3\>-13\" |
| ,\          | .0,M11.1.0\" | \<+07\>-7\" | /Rome,\"CET- |          |
| "\<-05\>5\" |              |             | 1CEST,M3.5.0 | Etc      |
|             | Americ       | A           | ,M10.5.0/3\" | /GMT-1   |
| America     | a/Port_of_Sp | sia/Omsk,\" |              | 4,\"\<+1 |
| /Boise,\"MS | ain,\"AST4\" | \<+06\>-6\" | Eur          | 4\>-14\" |
| T7MDT,M3.2. |              |             | ope/Samara,\ |          |
| 0,M11.1.0\" | America/     | A           | "\<+04\>-4\" | Etc      |
|             | Porto_Velho  | sia/Oral,\" |              | /GMT0,   |
| Amer        | ,            | \<+05\>-5\" | Europe/San_M | \"GMT0\" |
| ica/Cambrid | \"\<-04\>4\" |             | arino,\"CET- |          |
| ge_Bay,\"MS |              | Asia/       | 1CEST,M3.5.0 | Etc      |
| T7MDT,M3.2. | Amer         | Phnom_Penh  | ,M10.5.0/3\" | /GMT+0,  |
| 0,M11.1.0\" | ica/Puerto_R | ,\"         |              | \"GMT0\" |
|             | ico,\"AST4\" | \<+07\>-7\" | Europe/Sar   |          |
| America/    |              |             | ajevo,\"CET- | Etc      |
| C           | America/     | Asia/       | 1CEST,M3.5.0 | /GM      |
| ampo_Grande | Punta_Arenas | Pontianak , | ,M10.5.0/3\" | T+1,\"\< |
| ,\          | ,            | \"WIB-7\"   |              | -01\>1\" |
| "\<-04\>4\" | \"\<-03\>3\" |             | Euro         |          |
|             |              | As          | pe/Saratov,\ | Etc      |
| Amer        | America/Ra   | ia/Pyongyan | "\<+04\>-4\" | /GM      |
| ica/Cancun, | iny_River,\" | g,\"KST-9\" |              | T+2,\"\< |
| \"EST5\"    | CST6CDT,M3.2 |             | Eur          | -02\>2\" |
|             | .0,M11.1.0\" | As          | ope/Simferop |          |
| Americ      |              | ia/Qatar,\" | ol,\'MSK-3\' | Etc      |
| a/Caracas,\ | America/Ran  | \<+03\>-3\" |              | /GM      |
| "\<-04\>4\" | kin_Inlet,\" |             | Europe/S     | T+3,\"\< |
|             | CST6CDT,M3.2 | Asia/       | kopje,\"CET- | -03\>3\" |
| Americ      | .0,M11.1.0\" | Qyzylorda   | 1CEST,M3.5.0 |          |
| a/Cayenne,\ |              | ,\"         | ,M10.5.0/3\" | Etc      |
| "\<-03\>3\" | Ame          | \<+05\>-5\" |              | /GM      |
|             | rica/Recife, |             | Europe/So    | T+4,\"\< |
| A           | \"\<-03\>3\" | Asia/       | fia,\"EET-2E | -04\>4\" |
| merica/Caym |              | Riyadh      | EST,M3.5.0/3 |          |
| an,\"EST5\" | Ame          | ,\"         | ,M10.5.0/4\" | Etc      |
|             | rica/Regina, | \<+03\>-3\" |              | /GM      |
| America/C   | \"CST6\"     |             | Europe/Stoc  | T+5,\"\< |
| hicago,\"CS |              | Asia/       | kholm,\"CET- | -05\>5\" |
| T6CDT,M3.2. | America      | Sakhalin    | 1CEST,M3.5.0 |          |
| 0,M11.1.0\" | /Resolute,\" | ,\"\        | ,M10.5.0/3\" | Etc      |
|             | CST6CDT,M3.2 | <+11\>-11\" |              | /GM      |
| America/Chi | .0,M11.1.0\" |             | Europe/Tall  | T+6,\"\< |
| huahua,\"MS |              | Asia/S      | inn,\"EET-2E | -06\>6\" |
| T7MDT,M4.1. | America/     | amarkand,\" | EST,M3.5.0/3 |          |
| 0,M10.5.0\" | Rio_Branco   | \<+05\>-5\" | ,M10.5.0/4\" | Etc      |
|             | ,            |             |              | /GM      |
| Ameri       | \"\<-05\>5\" | Asia/Seou   | Europe/T     | T+7,\"\< |
| ca/Costa_Ri |              | l,\"KST-9\" | irane,\"CET- | -07\>7\" |
| ca,\"CST6\" | Ameri        |             | 1CEST,M3.5.0 |          |
|             | ca/Santarem, | Asi         | ,M10.5.0/3\" | Etc      |
| Am          | \"\<-03\>3\" | a/Shanghai, |              | /GM      |
| erica/Crest |              | \"CST-8\"   | Europe/      | T+8,\"\< |
| on,\"MST7\" | America/     |             | Ulyanovsk    | -08\>8\" |
|             | Santiago,\"\ | Asia/S      | ,\           |          |
| America/    | <-04\>4\<-03 | ingapore,\" | "\<+04\>-4\" | Etc      |
| Cuiaba      | \>,M9.1.6/24 | \<+08\>-8\" |              | /GM      |
| ,\          | ,M4.1.6/24\" |             | Europe/Uzhgo | T+9,\"\< |
| "\<-04\>4\" |              | Asia/       | rod,\"EET-2E | -09\>9\" |
|             | Americ       | Sr          | EST,M3.5.0/3 |          |
| Ameri       | a/Santo_Domi | ednekolymsk | ,M10.5.0/4\" | Etc      |
| ca/Curacao, | ngo,\"AST4\" | ,\"\        |              | /GMT+    |
| \"AST4\"    |              | <+11\>-11\" | Europe/      | 10,\"\<- |
|             | America/     |             | Vaduz,\"CET- | 10\>10\" |
| America     | Sao_Paulo    | Asia/Taipe  | 1CEST,M3.5.0 |          |
| /Danmarksha | ,            | i,\"CST-8\" | ,M10.5.0/3\" | Etc      |
| vn,\'GMT0\' | \"\<-03\>3\" |             |              | /GMT+    |
|             |              | Asia/       | Europe/Va    | 11,\"\<- |
| A           | America/     | Tashkent    | tican,\"CET- | 11\>11\" |
| merica/Daws | Scoresbysund | ,\"         | 1CEST,M3.5.0 |          |
| on,\"MST7\" | ,\"          | \<+05\>-5\" | ,M10.5.0/3\" | Etc      |
|             | \<-01\>1\<+0 |             |              | /GMT+    |
| America     | 0\>,M3.5.0/0 | Asia        | Europe/V     | 12,\"\<- |
| /Dawson_Cre | ,M10.5.0/1\" | /Tbilisi,\" | ienna,\"CET- | 12\>12\" |
| ek,\"MST7\" |              | \<+04\>-4\" | 1CEST,M3.5.0 |          |
|             | Americ       |             | ,M10.5.0/3\" | Etc      |
| America/    | a/Sitka,\"AK | Asia/       |              | /UCT,    |
| Denver,\"MS | ST9AKDT,M3.2 | Tehran      | Europe/Viln  | \"UTC0\" |
| T7MDT,M3.2. | .0,M11.1.0\" | ,\"\<+0330  | ius,\"EET-2E |          |
| 0,M11.1.0\" |              | \>-3:30\<+0 | EST,M3.5.0/3 | Etc      |
|             | Americ       | 430\>,J79/2 | ,M10.5.0/4\" | /UTC,    |
|             | a/St_Barthel | 4,J263/24\" |              | \"UTC0\" |
|             | emy,\"AST4\" |             | Europe       |          |
|             |              | Asia        | /Volgograd,\ | Etc      |
|             | America/St   | /Thimphu,\" | "\<+03\>-3\" | /Gr      |
|             | _Johns,\"NST | \<+06\>-6\" |              | eenwich, |
|             | 3:30NDT,M3.2 |             | Europe/W     | \'GMT0\' |
|             | .0,M11.1.0\" |             | arsaw,\"CET- |          |
|             |              |             | 1CEST,M3.5.0 | Etc      |
|             | A            |             | ,M10.5.0/3\" | /Un      |
|             | merica/St_Ki |             |              | iversal, |
|             | tts,\"AST4\" |             | Europe/Z     | \"UTC0\" |
|             |              |             | agreb,\"CET- |          |
|             | America/St.  |             | 1CEST,M3.5.0 | Etc      |
|             | Lu           |             | ,M10.5.0/3\" | /Zulu,   |
|             | cia,\"AST4\" |             |              | \"UTC0\" |
|             |              |             | Eu           |          |
|             | Am           |             | rope/Zaporoz |          |
|             | erica/St_Tho |             | hye,\"EET-2E |          |
|             | mas,\"AST4\" |             | EST,M3.5.0/3 |          |
|             |              |             | ,M10.5.0/4\" |          |
|             | Ame          |             |              |          |
|             | rica/St_Vinc |             | Europe/Z     |          |
|             | ent,\"AST4\" |             | urich,\"CET- |          |
|             |              |             | 1CEST,M3.5.0 |          |
|             | Americ       |             | ,M10.5.0/3\" |          |
|             | a/Swift_Curr |             |              |          |
|             | ent,\"CST6\" |             |              |          |
+=============+==============+=============+==============+==========+
+-------------+--------------+-------------+--------------+----------+
