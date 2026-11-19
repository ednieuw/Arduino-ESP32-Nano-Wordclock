# Programmauitleg WordFiboClock V247

Dit document beschrijft hoe de sketch is opgebouwd. Het is bedoeld voor wie de software wil begrijpen, aanpassen of uitbreiden, bijvoorbeeld met een eigen klokvariant of een extra menucommando. Voor gebruik en bediening van de klok: zie de [Handleiding](Handleiding.md).

[English](ProgramExplanation.md) · [README](READMENL.md) · [Handleiding](Handleiding.md)

## Inhoud

1. [Overzicht](#1-overzicht)
2. [Bestanden en klokvarianten](#2-bestanden-en-klokvarianten)
3. [Bibliotheken](#3-bibliotheken)
4. [Globale variabelen en permanente opslag](#4-globale-variabelen-en-permanente-opslag)
5. [Opstarten: `setup()`](#5-opstarten-setup)
6. [Het hoofdprogramma: `loop()` en de tijdstappen](#6-het-hoofdprogramma-loop-en-de-tijdstappen)
7. [Uitvoer: de `Tekstprint`-functies en het logboek](#7-uitvoer-de-tekstprint-functies-en-het-logboek)
8. [Invoer: alle commando's via `ReworkInputString()`](#8-invoer-alle-commandos-via-reworkinputstring)
9. [Tijd](#9-tijd)
10. [LEDs en helderheid](#10-leds-en-helderheid)
11. [Het klokbord](#11-het-klokbord)
12. [Bluetooth, tijdontvanger en tijdzender](#12-bluetooth-tijdontvanger-en-tijdzender)
13. [WIFI, webpagina's en OTA](#13-wifi-webpaginas-en-ota)
14. [MQTT en Home Assistant](#14-mqtt-en-home-assistant)
15. [Bewaking en herstel](#15-bewaking-en-herstel)
16. [Aanpassen en uitbreiden](#16-aanpassen-en-uitbreiden)
17. [Functieoverzicht](#17-functieoverzicht)

---

## 1. Overzicht

De sketch is één groot programma met bewust zoveel mogelijk **globale variabelen**. Variabelen zijn per onderdeel gegroepeerd, zodat met *zoeken* in de IDE snel alles te vinden is. Functies krijgen weinig parameters. Dat is geen aanbeveling voor grote projecten met meerdere ontwikkelaars, maar houdt dit programma eenvoudig te lezen.

Alle functies staan in dezelfde volgorde in de sketch, gegroepeerd met een commentaarregel als `// COMMON`, `// LED`, `// WIFI`, `// BLE`, `// MQTT`, `// CLOCK` voor de functienaam. Zo is zichtbaar of een functie voor alle klokvarianten (`COMMON`) of voor één onderdeel geldt.

Het programma is **cooperatief**: er wordt nergens lang gewacht. `loop()` doet bijna niets zelf en roept routines aan die elke seconde, minuut, uur en dag iets uitvoeren. Daardoor is de processor grotendeels vrij: met `K` (LDR-uitlezing) wordt het aantal doorlopen per seconde getoond, ongeveer 200 kl/s (200.000 per seconde).

## 2. Bestanden en klokvarianten

| Bestand | Inhoud |
|---|---|
| `WordFiboClockV247.ino` | Alles wat voor alle varianten geldt: opstarten, menu, WIFI, BLE, MQTT, tijd, LED-hulpfuncties, opslag |
| `WordClock.h` | Weergave van de woordklok (`Displaytime()`, de taalfuncties `Dutch()`, `English()`...), kleurkiezer en menupagina |
| `ClockFaces.h` | LED-posities van elk woord, per klokvariant |
| `Fibonacci.h` | Weergave van de Fibonacci-klok, kleurkiezer en menupagina |
| `NoClock.h` | Lege functies voor de variant zonder display |
| `Webpage.h`, `SoftAP.h`, `OTAhtml.h`, `LogViewer.h` | HTML-pagina's van de klok |

### De variant kiezen

Bovenin de sketch wordt met `#define` één variant gekozen. Daaruit volgt welk `.h`-bestand wordt ingevoegd, aan het einde van de definities (onder de menutabel):

```cpp
#if   defined(WORDCLOCK)
  #include "WordClock.h"      // Woordklok: Displaytime(), ColourPick_html[], menu_html[]
#elif defined(FIBONACCI)
  #include "Fibonacci.h"      // Fibonacci
#elif defined(NOCLOCK)
  #include "NoClock.h"        // Geen display
#endif
```

`WORDCLOCK` wordt automatisch gedefinieerd zodra geen `FIBONACCI` of `NOCLOCK` is gekozen.

Elk variantbestand levert dezelfde onderdelen, zodat de hoofdsketch er niets van hoeft te weten:

| Onderdeel | Doel |
|---|---|
| `Displaytime()` | Zet de tijd op de LEDs |
| `QchoiceNames[]` en `LASTITEM` | Namen van de kleurschema's voor het commando `Q` (en voor Home Assistant) |
| `CLOCK_VARIANT` | Naam van de variant in het logboek |
| `NUM_LEDS`, `MATRIX_WIDTH`, `MATRIX_HEIGHT`, `LEDNEARLDR` | Afmetingen en LED-posities |
| `ColourPick_html[]`, `menu_html[]` | Pagina's `/colourpick` en `/menu` |
| `PrintDisplayChoice()`, `SetSecondColour()`, `Demomode()`, `FireMode()`, `Selftest()`, `RotPressOne/Two/Three()` | Hulpfuncties. Waar een variant ze niet nodig heeft, zijn het lege functies |

Dit is de reden dat commando's als `$` (haardvuur) of `#` (zelftest) bij de Fibonacci-klok niets doen.

## 3. Bibliotheken

```cpp
#include <EdSoftLED.h>          // LED-strip (core 3.3+); anders <Adafruit_NeoPixel.h>
#include <Preferences.h>        // Permanente opslag
#include <NimBLEDevice.h>       // Bluetooth Low Energy (NimBLE 2.x)
#include <WiFi.h>               // WIFI
#include <esp_sntp.h>           // NTP
#include <esp_wps.h>            // WPS
#include <ESPAsyncWebServer.h>  // Webpagina's
#include <Update.h>             // OTA
#include <ESPmDNS.h>            // naam.local
#include <DNSServer.h>          // Toegangspuntpagina
#include <Wire.h>               // I2C
#include <RTClib.h>             // DS3231
#include <Encoder.h>            // Rotary encoder
#include <Keypad.h>             // Drie drukknoppen
#include <IRremote.hpp>         // IR-afstandsbediening
#include <PubSubClient.h>       // MQTT
```

De LED-bibliotheek wordt automatisch gekozen: bij ESP32-core 3.3.0 of hoger `USEEDSOFTLED` (EdSoftLED), anders NeoPixel. Zie [hoofdstuk 4 van de handleiding](Handleiding.md#4-software-installeren) voor de bijbehorende pinnummering.

## 4. Globale variabelen en permanente opslag

### De struct `Mem`

Alle instellingen staan in één struct, `EEPROMstorage`, met de variabele `Mem`:

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

De velden worden met `Mem.<naam>` aangesproken, bijvoorbeeld `Mem.DisplayChoice = 3;`. De hele struct wordt in één keer met de bibliotheek `Preferences` opgeslagen en teruggelezen:

| Functie | Taak |
|---|---|
| `InitStorage()` | Leest de struct en controleert de waarden. Is `Mem.Checksum` niet 25065, dan volgt `Reset()` |
| `StoreStructInFlashMemory()` | Slaat de struct op. Gebeurt na elk commando en elke dag |
| `GetStructFromFlashMemory()` | Leest de struct uit het geheugen |
| `Reset()` | Zet alle waarden op de standaard (behalve WIFI-gegevens, BLE-naam en tijdzone) |
| `ResetCredentials()` | Wist SSID, wachtwoord, BLE-naam en tijdzone |
| `StoreIRRemoteInFlashMemory()`, `GetIRRemoteFromFlashMemory()` | De geleerde IR-toetsen (aparte opslag) |

De struct bevat reservevelden (`byteFuture…`, `IntFuture…`, `Extra…`, `ExtraSpace`), zodat een nieuwe instelling zonder verandering van de indeling kan worden toegevoegd.

> Wordt de indeling van de struct gewijzigd, dan klopt de opslag in bestaande klokken niet meer en volgt een reset naar de standaardwaarden bij de eerste start. Neem daarom een nieuw veld liever in een reserveveld op.

### Een nieuwe instelling toevoegen

1. Voeg het veld toe in de struct (of gebruik een reserveveld).
2. Zet de standaardwaarde in `Reset()`.
3. Voeg het toe aan `PrintMem()` zodat het met `=` zichtbaar is.
4. Schrijf het commando in `ReworkInputString()` (zie [16](#16-aanpassen-en-uitbreiden)).

### De menutabel

Het menu dat in de seriële monitor, de Bluetooth-app en op de webpagina wordt getoond staat als twee tabellen, `menu[]` (lang) en `menusmall[]` (kort). De regels zijn maximaal 40 tekens lang. `SWversion()` print het menu en de huidige instellingen, `PrintLine()` tekent de scheidingslijnen.

## 5. Opstarten: `setup()`

`setup()` start de onderdelen in deze volgorde. Elke stap toont een eigen kleur op de LED-strip, zodat zichtbaar is waar het opstarten hangt (`LEDstartup()`):

1. Seriële poort (115200 baud) en I2C.
2. `InitStorage()`: opslag lezen en controleren, ringbuffer voor het logboek (`InitLogBuffer()`).
3. `StartLeds()` en `InitStatusLEDs()`.
4. Wachten op de seriële verbinding (maximaal 3 seconden).
5. `++Mem.MCUrestarted`: te vaak herstarten leidt tot een reset (zie [15](#15-bewaking-en-herstel)).
6. Invoerapparaat: rotary (`TimeInput` 1), drukknoppen (2) of IR (3 en 4).
7. BLE (`StartBLEService()`), WIFI (`ConnectWIFI()`), tijdsysteem (`InitTimeSystem()`).
8. Tijdontvanger (`H05`), BLE-tijdzender (`H06`), HC-12 (`H07`), resetknop (`^nn`).
9. Eerste LDR-meting, eerste weergave van de tijd en het menu.

## 6. Het hoofdprogramma: `loop()` en de tijdstappen

```cpp
void loop()
{
 Loopcounter++;
 if (Demo)            Demomode();
 else if(FireDisplay) FireMode();
 else                 EverySecondCheck();   // laat de seconde tikken en draait de klok
 CheckDevices();
}
```

`CheckDevices()` kijkt of er invoer is, elke keer dat `loop()` draait:

```cpp
void CheckDevices(void)
{
 CheckBLE();                 // Bluetooth-invoer?
 SerialCheck();              // Seriële invoer?
 CheckWIFIcommand();         // Commando van de webpagina?
 if(Mem.MQTT) CheckMQTT();   // MQTT onderhouden
 if (Mem.TimeInput==1) RotaryEncoderCheck();
 if (Mem.TimeInput==2) Keypad3x1Check();
 if (Mem.TimeInput==3 || Mem.TimeInput==4) IrReceiverDecode();
 if (ntpJustSynced) CheckandPrintNTPsynced();
}
```

`EverySecondCheck()` doet één keer per seconde (als `millis() - msTick > 999`) het volgende werk:

- tijd lezen met `GetTijd()`, status-LED's bijwerken, kleur van HET IS WAS per seconde (`SetSecondColour()`);
- resetknop controleren, tijdontvanger- en tijdzender-client onderhouden (`CheckTimeReceiverClient()`, `CheckTimeSender()`);
- `DimLeds()`: helderheid op basis van de LDR;
- na een OTA-update of herstartopdracht: MQTT op *offline* zetten en met `ESP.restart()` herstarten;
- is de minuut veranderd: `EveryMinuteUpdate()`.

De tijdstappen lopen in een cascade:

| Routine | Wanneer | Taak |
|---|---|---|
| `EveryMinuteUpdate()` | Elke minuut | Weergave bijwerken, tijdlog (`K1`), HC-12, willekeurig kleurschema (`~1`), `PublishMQTTStates()` |
| `EveryHourUpdate()` | Elk uur | Display aan/uit volgens `N`, `CheckRestoreWIFIconnectivity()`, gemiddelde LDR per uur, `~2` |
| `EveryDayUpdate()` | Elke dag | LDR-minimum en -maximum terugzetten, tijd printen, `StoreStructInFlashMemory()` |

## 7. Uitvoer: de `Tekstprint`-functies en het logboek

Alle uitvoer loopt via één plaats, zodat hij is om te leiden:

```cpp
void Tekstprint(char const *tekst)
{
 if(Serial) Serial.print(tekst);
 SendMessageTimeReceiver(tekst);   // naar de tijdzender (BLE)
 SendMessageBLE(tekst);            // naar de Bluetooth-app
 AddLog(tekst);                    // in de ringbuffer
}
```

Hiervan zijn `Tekstprintln`, `Tekstprintf` en `Tekstprintlnf` varianten met regeleinde en `printf`-opmaak. Een tweede reeks, `WTekstprint…`, bouwt het HTML-blok voor de webpagina op, met optioneel voor- en achtervoegsel zodat een regel alleen op de webpagina een link kan bevatten (bijvoorbeeld `/update`).

### Het logboek (ringbuffer)

`InitLogBuffer()` reserveert bij het opstarten een groot geheugenblok, bij voorkeur in PSRAM. `AddLog()` schrijft de tekst cyclisch in dat blok; aan het einde begint het weer vooraan. Twee routes tonen het logboek:

| Route | Functie | Inhoud |
|---|---|---|
| `/tekstprint` | `HandleTekstPrint()` | De laatste ongeveer 500 regels |
| `/tekstdownload` | `HandleTekstDownload()` | De hele buffer als bestand `<naam>-log.txt` |

Met `}` (`Mem.Ringbufcnt`) wordt achter elke regel de positie in de buffer geprint.

## 8. Invoer: alle commando's via `ReworkInputString()`

Alle invoerbronnen komen samen in één functie, `ReworkInputString(String InputString)`:

| Bron | Waar |
|---|---|
| Seriële monitor | `SerialCheck()` |
| Bluetooth | `CheckBLE()` |
| Webpagina | `CheckWIFIcommand()` (via `PendingCommand`) |
| MQTT | `MQTTCallback()` |
| Tijdzender (BLE) | `TimeReceiverNotifyCB()` |

De functie

- negeert invoer langer dan 40 tekens;
- kijkt naar het eerste teken (`toupper(InputString[0])`) en kiest met een `switch` het commando;
- zet het resultaat in `sptext`, dat aan het einde met `Tekstprintln` wordt getoond;
- slaat `Mem` op met `StoreStructInFlashMemory()` en forceert een nieuwe weergave (`lastminute = 99`).

Hierdoor werkt elk commando overal hetzelfde: een knop in Home Assistant, een tik op de webpagina en een letter in de Bluetooth-app roepen dezelfde code aan.

Hulpfuncties bij de invoer: `SConstrainInt()` (getal uit een tekenreeks met begrenzing), `ParseColourInput()` (kleuren in hex en cijfers, voor `F` en `V`), `HexToDec()`.

## 9. Tijd

`GetTijd()` haalt de tijd uit de beste beschikbare bron en zet hem in de standaard C-structuur `timeinfo`.

| Bron | Functies |
|---|---|
| NTP via WIFI | `StartWIFI_NTP()`, `initSNTP()`, `setTimezone()`, `wait4SNTP()`, `NTPnotify()`, `GetNTPtime()` |
| DS3231 (I2C) | `IsDS3231I2Cconnected()`, `GetDS3231Time()`, `SetDS3231Time()`, `GetDS3231Temp()` |
| Interne RTC van de ESP32 | `SetRTCTime()`, `SetSystemTime()` |
| Handmatig | commando's `D`, `T` en `uummss` |
| Tijdontvanger | commando's `T` en `D` via BLE |

Met `J` (`Mem.UseDS3231`) en `X` (`Mem.NTPOn`) wordt de bron gekozen. `InitTimeSystem()` start de bronnen op. `PrintAllClockTimes()` (commando `!`) toont alle tijden naast elkaar. De zomertijd komt van de tijdzone-string (POSIX-formaat) en geldt alleen voor NTP, niet voor de DS3231.

## 10. LEDs en helderheid

### De LED-strip

Alle aanroepen voor de strip staan in hulpfuncties, zodat de rest van de code los staat van de bibliotheek (EdSoftLED of NeoPixel):

| Functie | Taak |
|---|---|
| `StartLeds()` | Kiest SK6812 (met wit) of WS2812 volgens `Mem.LEDstrip` |
| `ColorLeds(tekst, eerste, laatste, kleur)` | Zet een reeks LEDs aan en toont de tekst in het logboek |
| `ColorLed()`, `LedsOff()`, `ShowLeds()`, `SetBrightnessLeds()`, `Stripfill()`, `ClearScreen()` | Basisbewerkingen |
| `LEDstartup()` | Toont tijdens het opstarten een kleur op de strip |
| `LEDtest()` | Het regenboogprogramma van `Y` (onderbreekbaar met `Y`) |

Een kleur is een 32-bit waarde `0xWWRRGGBB`. Bij een SK6812 bepaalt de hoogste byte de witte LED, bij een WS2812 wordt die genegeerd.

### Lichtgevoeligheid

`ReadLDR()` leest de LDR (0-4095) en schaalt naar 0-255. `DimLeds()` verwerkt de meting tot de helderheid van de LEDs (`OutPhotocell`) met de parameters `Mem.LightReducer` (`S`), `Mem.LowerBrightness` (`L`) en `Mem.UpperBrightness` (`M`). De waarde wordt gekwadrateerd, zodat de curve beter aansluit op de reactie van het oog dan een rechte lijn.

### Status-LED's

`UpdateStatusLEDs()` bestuurt de RGB-LED op de Nano ESP32 en twee LED's op de PCB. De kleur toont de toestand (programma loopt, WIFI, Bluetooth). Op de Nano ESP32 is een lage PWM-waarde *aan*; `InitStatusLEDs()` stelt daarom de resolutie en frequentie van de PWM-pinnen expliciet in (9 bit, 5 kHz), omdat de standaardwaarden tussen core-versies zijn gewijzigd.

## 11. Het klokbord

### Woordklok

Elk woord is een macro die `ColorLeds()` aanroept met de eerste en laatste LED van het woord. Ze staan in `ClockFaces.h`, per variant:

```cpp
#define PRECIES ColorLeds("precies", 16, 22, LetterColor);
#define MTIEN   ColorLeds("tien",    25, 28, LetterColor);
#define KWART   ColorLeds("kwart",   32, 36, LetterColor);
#define VOOR    ColorLeds("voor",    38, 41, LetterColor);
```

De taalfuncties in `WordClock.h` zetten de woorden voor de tijd met alleen die macro's:

```cpp
void Dutch(void)
{
 HET;                                          // HET is altijd aan
 switch (timeinfo.tm_min)
   {
    case  0: IS;  PRECIES; break;
    case  4: case 5: case 6: IS; MVIJF; OVER; break;
    ...
   }
}
```

`Displaytime()` roept per variant de juiste taalfuncties aan (`Dutch()`, `English()`, `German()`, `French()`; bij de 4-talenklok alle vier). Dat een taal soms twee mogelijke lezingen heeft ("half negen" is 8:30 of 9:30) is in de taalfuncties opgelost.

De kleuren van de woorden volgen uit `SetSecondColour()` en het gekozen kleurschema (`Mem.DisplayChoice`, de `Q`-opdracht).

### Fibonacci

`Fibonacci.h` bevat `LEDsetTime()` (Fibonacci-weergave), `MakeChronoList()` (Chrono-weergave), `MakeFibonacciList()` (willekeurige combinatie van cellen die een getal vormt) en `setPixel()`, dat per `NUM_LEDS` de LED-indeling van elke cel bepaalt. Zie [hoofdstuk 10](Handleiding.md#10-fibonacci-klok-en-noclock) van de handleiding.

### Een nieuwe klokvariant toevoegen (woordklok)

1. Voeg in `ClockFaces.h` een blok toe voor de nieuwe `#define`, met `NUM_LEDS`, `LEDNEARLDR`, `MATRIX_WIDTH`, `MATRIX_HEIGHT` en de macro's voor elk woord.
2. Voeg de naam toe aan de lijst `CLOCK_VARIANT` in `WordClock.h`.
3. Voeg de `#define`-regel toe in de lijst bovenin de sketch.

## 12. Bluetooth, tijdontvanger en tijdzender

### Bluetooth

De klok gebruikt de Nordic **nRF UART-service** van NimBLE, niet de TI CC2541-service (`FFE0`) van chips als de HM-10.

| Functie | Taak |
|---|---|
| `StartBLEService()` | Start de UART-service |
| `SendMessageBLE()` | Stuurt tekst in pakketjes van 20 tekens (of langer met snelle BLE, `+`) |
| `CheckBLE()` | Leest invoer en geeft die aan `ReworkInputString()` |
| `DisconnectBLE()` | Verbreekt de verbinding |

### Tijdontvanger (`H05`)

De klok is dan BLE-**client** en zoekt een BLE-apparaat met de naam `BLE-UARTtime`.

| Functie | Taak |
|---|---|
| `StartTimeReceiverScan()` | Start het scannen |
| `Connect_TimeReceiver()` | Maakt verbinding (drie pogingen) en abonneert op notificaties |
| `CheckTimeReceiverClient()` | Hoofdlus: verwerkt opruimwerk na een verbinding die wegvalt, time-outs en periodiek opnieuw scannen |
| `StopTimeReceiver()`, `ReconnectTimeReceiver()` | Stoppen en opnieuw verbinden |
| `TimeReceiverNotifyCB()` | Ontvangt tekst en geeft die aan `ReworkInputString()` |

Alle opruimwerk na een verbroken verbinding wordt in `loop()` uitgevoerd, niet in de callback zelf. Dat voorkomt crashes in NimBLE.

### Tijdzender (`H06`, `H07`)

| Functie | Taak |
|---|---|
| `BLETimeSenderStart()`, `BLETimeSenderStop()` | BLE-**server** met de naam `BLE-UARTtime` |
| `CheckTimeSender()` | Stuurt elke 10 seconden `Thhmmss` en 1 seconde later `Dddmmjjjj` zodra een client is verbonden en notificaties heeft ingeschakeld |
| `InitHC12()`, `SendHC12TimeString()` | HC-12 op D4, 9600 baud, `Thhmmss` per minuut (`K1`) of uur (`K2`) |

## 13. WIFI, webpagina's en OTA

### WIFI en NTP

| Functie | Taak |
|---|---|
| `ConnectWIFI()` | Verbindt met SSID en wachtwoord uit `Mem` |
| `StartWIFI_NTP()` | Start WIFI en NTP |
| `CheckforWIFINetwork()`, `ScanWIFI()` | Netwerken zoeken (`G`) |
| `StartAPMode()` | Toegangspunt `StartWordclock` met de pagina `SoftAP.h` |
| `wpsInitConfig()`, `wpsStart()`, `wpsStop()` | WPS (`Z`) |
| `WiFiEvent()` | Verwerkt WIFI-gebeurtenissen |
| `CheckRestoreWIFIconnectivity()` | Elk uur: herstelt de verbinding (zie [15](#15-bewaking-en-herstel)) |

### De routes van de webserver

`WebPage()` registreert alle routes op de ESPAsyncWebServer:

| Route | Pagina of functie |
|---|---|
| `/` | Hoofdpagina (`Webpage.h`): invoerveld, menu en live logvenster |
| `/log` | Volledig logboek (`LogViewer.h`) |
| `/tekstprint`, `/tekstdownload` | Logboek als tekst, zie [7](#7-uitvoer-de-tekstprint-functies-en-het-logboek) |
| `/menu` | Aanraakbare menupagina (`menu_html[]` uit het variantbestand) |
| `/menustate` | JSON met de stand van de schakelaars voor de menupagina |
| `/colourpick`, `/setcolours` | Kleurkiezer en het verwerken van de gekozen kleuren |
| `/get` | Commando van het invoerveld. Het commando wordt in `PendingCommand` gezet (de webserver is asynchroon) en de browser wordt naar `/wait` gestuurd |
| `/wait` | Toont *Processing command...* en ververst elke seconde tot het commando is verwerkt in `CheckWIFIcommand()`; daarna terug naar `/` |
| `/update` (GET en POST) | OTA-pagina en het uploaden van het `.bin`-bestand |

De pagina's staan als *raw string literals* in de `.h`-bestanden (`R"rawliteral( … )rawliteral"`). Waarden die de klok moet invullen staan als plaatshouder (`%%COLNL%%`, `%%CNAME%%`…) en worden bij het opvragen vervangen. Het hoofdmenu en het menu van `/menu` hebben een klein **live logvenster** dat elke seconde `/tekstprint` ophaalt, de laatste 50 regels toont en alleen automatisch meescrollt als de lezer onderaan staat.

Een HTML-pagina kan in een gewone teksteditor (bijvoorbeeld Notepad++) of met een AI-assistent worden gemaakt en daarna tussen `R"rawliteral(` en `)rawliteral";` in een `.h`-bestand worden geplakt.

### OTA

Het uploaden van een `.bin` verloopt in `/update` (POST) met de functies van `Update.h`. De sketch beschermt tegen twee gelijktijdige uploads: een tweede upload wordt geweigerd zolang een eerste actief is (`OTAupdateInProgress`, V244). Na een geslaagde update wordt `shouldReboot` gezet; `EverySecondCheck()` herstart de klok dan na twee seconden.

## 14. MQTT en Home Assistant

| Functie | Taak |
|---|---|
| `CheckMQTT()` | Houdt de verbinding aan (niet-blokkerend, elke 5 seconden een nieuwe poging) |
| `publishMQTTDiscovery()` | Eén keer na het verbinden: stuurt de aanmeldberichten |
| `publishDiscoverySensor/Button/Switch/Number/Select()` | Hulpfuncties per type |
| `PublishMQTTStates()` | Stuurt alle waarden: elke minuut, na elk commando en direct na het verbinden |
| `MQTTCallback()` | Ontvangt commando's van `<id>/cmnd` en geeft ze aan `ReworkInputString()` |
| `MQTTDeviceId()` | Apparaat-id: Bluetooth-naam in kleine letters, spaties en schuine strepen als `_` |

De MQTT-instellingen (`MQTT_SERVER`, `MQTT_PORT`, `MQTT_USER`, `MQTT_PASSWORD`) staan als constanten bovenin de sketch. Zie [hoofdstuk 9 van de handleiding](Handleiding.md#9-mqtt-en-home-assistant) en [MQTT-HomeAssistant-setup.md](MQTT-HomeAssistant-setup.md).

## 15. Bewaking en herstel

| Mechanisme | Waar | Gedrag |
|---|---|---|
| Geen WIFI | `CheckRestoreWIFIconnectivity()` (elk uur) | Opnieuw verbinden. Na 30 uur zonder IP: herstart (`Mem.WIFInoConnection`) |
| Hoofdprogramma hangt | Zelfde routine | Minder dan 10 doorlopen per seconde met een WIFI-netwerk in de buurt: herstart (`Mem.LoopRebooted`) |
| Te vaak herstarten | `setup()` | Meer dan 5 herstarts kort na het opstarten: `Reset()`. `CheckMCURestartCounter()` wist de teller als de klok 2 minuten stabiel loopt |
| Resetknop | `CheckResetPin()` | 2 seconden ingedrukt: fabrieksinstellingen en herstart |
| Opslag ongeldig | `InitStorage()` | `Mem.Checksum` niet 25065: standaardwaarden |

## 16. Aanpassen en uitbreiden

### Een nieuw menucommando toevoegen

1. Kies een vrij teken en voeg een `case` toe in `ReworkInputString()` (zie bij de bestaande cases `'V'`, `'^'` en `'_'` als voorbeeld).
2. Zet de waarde in `Mem` (zie [4](#een-nieuwe-instelling-toevoegen)) en geef een melding via `snprintf(sptext, …)`.
3. Voeg het commando toe aan `menu[]` (maximaal 40 tekens per regel).
4. Voeg het toe aan de webmenu-pagina als er een schakelaar bij hoort (`menu_html[]` in het variantbestand en `/menustate`).
5. Overweeg een onderdeel in Home Assistant (zie `publishDiscovery…` en `PublishMQTTStates()`).

### Een nieuwe invoerbron toevoegen

Zorg dat de bron een tekenreeks aan `ReworkInputString()` geeft. Meer is niet nodig, het commando werkt dan verder overal.

### Een nieuw kleurschema voor de Fibonacci-klok

Voeg een rij toe aan de tabel `colors[][5]` in `Fibonacci.h`, een naam aan `QchoiceNames[]`, en `LASTITEM` volgt vanzelf.

## 17. Functieoverzicht

In de map met de sketch staat het bestand `FunctionListV247.txt` met alle functies per bestand en een korte omschrijving per functie. De lijst is voor V247 bijgewerkt. Ten opzichte van de lijst voor V237 zijn dit de wijzigingen:

| Functie | Wijziging |
|---|---|
| `ParseColourInput()` | Hernoemd (was `parseColourInput`) |
| `PublishMQTTStates()` | Vervangt `PublishCPUTemperature()`, `PublishLDRreading()` en `PublishDisplayChoiceState()` (V244) |
| `publishDiscoveryNumber()`, `publishDiscoverySwitch()` | Nieuw (schuifregelaars en schakelaar in Home Assistant) |
| `publishDiscoverySensor()`, `publishDiscoverySelect()` | Extra parameters (diagnostiek, lijst met opties) |
| `CheckResetPin()` | Leest nu `Mem.ResetPin` (V246) |
| `InitHC12()`, `SendHC12TimeString()` | Worden nu aangeroepen via `H07` (V247) |
