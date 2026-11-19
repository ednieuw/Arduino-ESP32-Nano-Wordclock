
# Arduino-ESP32-Nano-Woordklok

[English](README.md)

![image](https://github.com/ednieuw/Arduino-ESP32-Nano-Wordclock/assets/12166816/d5d450ce-df3c-4274-94ba-184792a7ee3d)

Bouw je eigen woordklok met WS2812 RGB- of SK6812 RGBW-LEDs, met één of [vier talen](https://github.com/ednieuw/FourLanguageClock), op een Arduino ESP32 Nano.<br>
De sketch werkt ook op andere ESP32-borden, met een andere pinnummering.<br>

[Vergelijk andere woordklok software/hardware-ontwerpen op de selectiepagina.](https://github.com/ednieuw/WordclockSelector)<br>

De klok kan worden bediend met:
- [BLE-webpagina](https://github.com/ednieuw/HTML-BLEserial)
- [BLE-terminal in een browser](https://ednieuw.com/BLESerial/BLE_UART_Terminal.html)
- [BLE seriële terminal-app](https://ednieuw.com/BLESerial/IOSappMain.html)
- Seriële kabel verbonden met je pc
- Rotary encoder
- IR-afstandsbediening
- (Membraan) toetsenbord
- MQTT, bijv. via Home Assistant
- [Time sender-app (iOS) of webpagina (Windows/Android)](https://github.com/ednieuw/HTML-TimeSender) —> [open Time sender HTML](https://ednieuw.com/Woordklok/TimeSender/TimeSender.html)
<br>

## Architectuur & gerelateerde projecten

Sinds V230 is de woordklok-sketch gecombineerd met de [Fibonacci-klok-sketch](https://github.com/ednieuw/Fibonacci-Nano-ESP32-clock) om het onderhoud te vereenvoudigen. Er is ook een derde optie; NOCLOCK.

De drie architecturen zijn eenvoudig te splitsen via `#define`'s op enkele plaatsen, en de klokfuncties kunnen volledig worden verwijderd met `#define NOCLOCK`. Functionaliteit kan via het menu aan/uit worden gezet.

Er is ook code voor een [HC-12 timesender](https://github.com/ednieuw/ESP32-HC12) toegevoegd — deze verzendt NTP-tijd via HC-12 naar een andere MCU met een HC-12-module die de uitzending tot 1000 meter verderop ontvangt. 

Als er geen WiFi beschikbaar is, kan de tijd in plaats daarvan naar de klok worden gestuurd via een timesender iOS-app of een timesender-webpagina, met Bluetooth op je telefoon of tablet.

## Schermen
<img alt="image" src="https://github.com/user-attachments/assets/e150fc48-650f-4d71-bbd8-eef1fb64d26b" style="width:90%;"  />
<br>In diverse browsers kan het TAB worden gesplitst om het menu en de respons van de seriële monitor in één TAB te zien. <br><br>

<img alt="image" src="Pics/AllMenusV235.jpg" style="width:90%;"  /><br>
Alle menu's in een browser sinds versie V235.

<img alt="image" src="Pics/Colourpicker.png" style="width:90%;"  /><br>

Om de klok met WIFI te verbinden kunnen de SSID en het wachtwoord van de router worden ingevoerd met de BLE seriële terminal-app, of kan de WPS-functie worden gebruikt.<br>

Software-updates kunnen worden geüpload met OTA (Over the Air).<br>

<img alt="image" src="Pics/MQTT-HA.jpg" style="width:90%;"  /><br>
Met MQTT in Home Assistant.

| | |
|---|---|
| <img alt="image" src="https://github.com/user-attachments/assets/977bcfa9-2a1c-4b1b-8453-d60769866224" width="650px" /><br><strong>Volledig menu</strong> | De klok houdt de tijd tot op de seconde nauwkeurig bij via het internet Network Time Protocol (NTP), met zomer-/wintertijdcorrectie wanneer een tijdzone is ingevoerd.<br><br>Als er geen internet beschikbaar is, kan een DS3231 RTC-module op de PCB worden aangesloten voor een nauwkeurige tijd.<br>Met een rotary encoder, membraantoetsenbord, time sender-app of bijna elke IR-afstandsbediening kan de tijd worden ingesteld.<br><br>Een LDR (lichtgevoelige weerstand) zorgt voor een optimale helderheid van de LEDs.<br><br>De ESP32 heeft een Espressif BLE/WIFI-module. Bluetooth LE gebruikt niet de TI CC2541-chip maar een Nordic nRF52-chip.<br>Dat betekent dat je een andere BLE-service voor de seriële communicatie moet gebruiken — niet characteristic FFE0 maar 6e400001-b5a3-... enz. in je seriële terminal-app die met de instellingen van de klok communiceert.<br><br>Zie hier: <a href="https://github.com/ednieuw/nRF-ESP32">BLE op een mobiel gebruiken om de ESP32 te bedienen</a><br><br>Een handleiding in het Nederlands en Engels is te vinden in deze repository: <a href="https://github.com/ednieuw/Arduino-ESP32-Nano-Wordclock/tree/main/Manual-Instructions">hier</a> |<br><br><br><br>


![image](https://github.com/user-attachments/assets/f187786c-b6db-49a8-9e91-708b93361390)
Klok met frontplaat van Cortenstaal

# Hardware
Voor mijn projecten zijn printplaten (PCB's) ontworpen met [Fritzing-software](https://fritzing.org/).

Dit Fritzing-programma is gemakkelijk te gebruiken en kan Gerber-bestanden exporteren die naar bedrijven kunnen worden gestuurd die PCB's maken. <br>
In Europa gemaakte PCB's kunnen met de Fritzing-app zelf worden besteld.<br>

[PCBWay](https://www.pcbway.com/) print 10 PCB's voor $5. Met +/-$25 verzend- en invoerkosten is dit erg goedkoop. De PCB's werden binnen een week na bestelling ontvangen en de kwaliteit was uitstekend. Upload gewoon de Gerber-bestanden in het ZIP-bestand en betaal de kosten.<br>  

Het [PCB-ontwerp .FZZ-bestand en de Gerber-bestanden](PCB) zijn te vinden in de map PCB in deze repository of hier:  [ESP32 PCB](https://github.com/ednieuw/NanoESP32PCB)

De PCB kan worden aangesloten op een
- rotary encoder  
- membraanschakelaar met drie knoppen 
- DS3231 RTC-module of ander I2C-apparaat 
- DCF77-ontvangermodule of ander apparaat dat pulsen ontvangt en 
- LDR om de lichtintensiteit te meten om de helderheid van de LED-strip te regelen.
- IR-afstandsbedieningsontvanger
- resetknop 

De SK6812 RGBW of WS2812 RGB LED-strip werkt op 5V. Maar het datasignaal van de Nano ESP32 is 3,3V.<br> 

Ik heb geprobeerd optocouplers te gebruiken om het datasignaal van 3,3V naar 5V te versterken, maar dat mislukte. Zie hier: 
https://ednieuw.com/ElecProj/OptoSK6812/OptocouplerSK6812.html<br>

Voor niveauomzetting wordt de 74HCT125 level shifter IC gebruikt. Deze heeft vier poorten. Het PCB-ontwerp kan de andere drie poorten op de IC voor andere doeleinden gebruiken. <br>

Een SK6812 RGBW-strip met 14 LEDs werkte ook prima toen de datalijn direct op de 3,3V-datalijn van de Nano ESP32 was aangesloten zonder de 74HCT125 level shifter IC. Ik heb strips met meer dan 14 LEDs niet getest.<br>

De les van dit verhaal is dat je de SK6812 RGBW-strip met een weerstand van 470 ohm in de datalijn en een condensator van 200 - 1000 uF over de 5V en GND direct op de strip kunt aansluiten zonder level shifter. <br>Maar succes is niet gegarandeerd.
 
![Nano-ESP32-V04](https://github.com/user-attachments/assets/3ad06452-0dd4-4310-8b51-25c115ec3959)

Een alternatieve universele PCB om RGB(W) LEDs en witte 2835 LED-strips aan te sturen met schuifregisters is hier te vinden: [Nano  ESP32 RGBW BW PCB](https://github.com/ednieuw/NanoESP32-BW-RGBW-clock) <br>
Deze PCB, met één Arduino-broncode, ondersteunt de kleuren-LED-klokken en de witte-LED-klokken met een Arduino Nano ESP32. 

![V01SmallNanoESP32-3D](https://github.com/user-attachments/assets/fb3863f4-1346-4985-a541-f087be7e8b68)
Het ontwerp van een [minimale PCB voor kleine ruimtes](/PCB-small) is ook te vinden in de map PCB-small in deze repository.

Stroom voor de LED-strip kan worden afgenomen van de VBUS-pin (5V USB) op de Nano wanneer deze via een USB-C-kabel wordt gevoed.  
VBUS levert 5V zodra er via USB wordt gevoed. 

Het is mogelijk de LED-strip via deze verbinding te voeden, maar bedenk dat de stroom door de PCB loopt en USB-C-voedingen meer dan 20 A kunnen leveren. Dit laat waarschijnlijk de dunne koperbanen op de PCB smelten.

Als er via de VIN-pin wordt gevoed, is VBUS uitgeschakeld. Dit betekent dat je, terwijl het bord via de VIN-pin wordt gevoed, geen 5V van het bord kunt afnemen. 

De PCB kan verschillende voedingsopties gebruiken.
Een 5V-voeding van >1A kan worden aangesloten op de 5V-voedingsaansluiting linksboven op de PCB.

Als de rode lijn (zie afbeelding hieronder) wordt kortgesloten, wordt de LED-strip met 5V gevoed via de aansluiting Power 6-21V (VIN).<br> 
Hierdoor wordt ook de Nano ESP32 gevoed. Je kunt de Vin op de ESP32 Nano aansluiten op een 5V USB-voeding, hoewel de minimale spanning als 6V is vermeld. De 1N5817-diode verlaagt de spanning zelfs met 0,5V naar 4,5V.

LET OP. Deze 5V-aansluiting gaat rechtstreeks naar de LED-strip. Als een voeding met een hogere spanning wordt aangesloten, worden de LEDs vernield, terwijl de Nano vrolijk aangaat.<br>

Als de Nano ESP32 via de USB-C-poort is aangesloten, moet de gele verbinding worden kortgesloten. De LEDs worden dan door deze voeding gevoed. Dit is prima met 20 LEDs maar waarschijnlijk niet voor langere strips.

![5VConnections](https://github.com/ednieuw/Arduino-ESP32-Nano-Wordclock/assets/12166816/39ea30fc-2850-4c1c-9fdb-4b4bec752f9a)

Nadat de aansluitingen zijn gemaakt ziet de PCB met hardware er zo uit:

![image](https://github.com/ednieuw/Arduino-ESP32-Nano-Wordclock/assets/12166816/86c688e0-8006-4042-880a-40064a4d8949)
![NanoESP32-V04-3D](https://github.com/user-attachments/assets/e717d61f-7953-4e35-80c1-278ca05a10df)

![image](https://github.com/user-attachments/assets/77fc651e-cfd6-4681-83e5-53c4f6bcfb55)

<img alt="PCB V04" src="Pics/PCBV04metOnderdelen.jpg" style="width:100%;"  /><br>
Alle onderdelen op PCB V04 inclusief membraantoetsenbord.

# PCB-aansluitingen

De grote PCB heeft aansluitingen voor: (met de klok mee, vanaf rechtsboven)
- 6-21V voedingsingang naar Vin op Nano ESP32
- DCF naar pin D2, 3V3 en GND
- LDR naar pin A2 en 3V3
- RTC-klok naar pin  GND, 3V3, A4 en A5
- Rotary encoder of 3-knops schakelaar naar pin D8, D3, D4 3V3 en GND
- Resetknop op D6 en GND (of D7)
- IR-afstandsbediening D4 3V3 en GND
- LEDs naar pin D10 en D9
- Connector naar pin D12, D11, D10 en D9
- D7 met 5V via 3V3 -> 5V niveauomzetter
- Connector met GND en 3V3
- Connector voor AMS1117 5V -> 3V3 1A spanningsregelaar.
- D6 met 5V via 3V3 -> 5V niveauomzetter
- Connector voor LED-strip SK6812/WS2812 naar 5V, D5 en GND
- D3 met 3V3 naar D3 en GND
- 5V voedingsingang naar 5V-USB of Vin op ESP32 en naar LED-strip  
- 5V- en GND-connector en connector naar Vin en 5V-USB op Arduino Nano ESP32
- Connector naar GND, RX en TX 

De kleine PCB heeft aansluitingen voor: (met de klok mee, vanaf linksboven)

- LDR naar pin A2 en 3V3
- 5V voedingsingang naar 5V-USB op ESP32 en naar LED-strip
- Connector voor LED-strip SK6812/WS2812 naar 5V, D5 en GND
- Rotary encoder of 3-knops schakelaar naar pin D2, D3, D4 3V3 en GND
- Resetknop op D6 en GND (of D7)
- IR-afstandsbediening D4 3V3 en GND
- RTC-klok naar pin  GND, 3V3, A4 en A5

![image](https://github.com/user-attachments/assets/f2919fb4-b571-4a1d-8264-4e227d4b104e) 

# De LED-grondplaat

![IMG_1469](https://github.com/user-attachments/assets/a6a80ac1-4cb6-47b2-a496-f9645e8795c1)

De afstand tussen de LEDs op de strip is geschikt voor een klok van 25 x 25 cm (60 LEDs/m) of 50 x 50 cm (30 LEDs/m).<br> 
Je kunt kiezen om de 144 LEDs van de klok in 12 rijen van 12 LEDs te plakken of alleen achter de letter die moet oplichten.<br>
Het voordeel van het laatste is dat er minder LEDs nodig zijn en dat je minder gaten in de afstandsplaat hoeft te boren.<br>
Maar er zijn veel meer soldeerpunten, die elk storingen kunnen veroorzaken.<br>  
Mijn ervaring is dat een onvolkomen soldering op de strip na verloop van tijd kan loskomen, met storingen als gevolg. 

De software heeft een digitale tijdweergave-optie waarvoor je 144 (12 x 12) LEDs gebruikt.<br>

Je kunt ervoor kiezen om de woorden zoals HET, WAS, VIJF enzovoort uit te zagen in een 1 cm dikke MDF-plaat met een figuurzaag.<br>
Zorg dat je de binnenkant helderwit verft, anders wordt het witte licht dof.<br> 
Beter is het om geschuimd PVC wit 10 MM RAL 9003 te gebruiken. Het is gemakkelijk te snijden, is wit en verkleurt niet. (Witte verf verkleurt vaak)
 
Plak/lijm de strips van links naar rechts op de oneven regels en van rechts naar links op de even regels. <br>

### Volg de pijlen op de strip.<br> 
(Je moet de strip elke tweede rij omdraaien!)<br>
Zo voorkom je lange datalijnen van het rechter uiteinde van de strip naar het linker uiteinde op de volgende rij van een LED-strip.<br>
En de software hoeft niet te worden aangepast voor de nieuwe posities.

![IMG_5384](https://github.com/user-attachments/assets/945ba89a-6fda-43fe-b16b-52f5babf0584)

Ik vind het handig om vertind koperdraad te gebruiken.<br> 
Bijvoorbeeld: je soldeert alle 5V-aansluitingen aan de linkerkant van de strip en alle GND aan de rechterkant van de strip.
Boor hiervoor een klein gat door de plaat naast alle 5V-aansluitingen aan de linkerkant van de strip en naast de GND aan de rechterkant

Buig een hoek van 90 graden, steek de draad door het gat en soldeer de draad aan de LED-strip.

![IMG_5467](https://github.com/user-attachments/assets/cb3148c9-6efb-4a4c-b04a-4ceb46584de6)

![IMG_1471](https://github.com/user-attachments/assets/fccbe36d-321e-4bd3-af34-782082c0a97e)

Plaats de afstandsplaat over de LEDs, dan een vel wit papier en tenslotte de woordplaat.

# Software

De ESP32Arduino_WordClockVxxx.ino maakt van de Nano ESP32 en SK6812- of WS2812-LED-strips een woordklok met verbinding met een WIFI-router die de tijd ontvangt van een NTP-server of een Timesender-app.

Een **handleiding** in het Nederlands en Engels is te vinden in deze repository [hier](https://github.com/ednieuw/Arduino-ESP32-Nano-Wordclock/tree/main/Manual-Instructions)

De software kan worden bediend met Bluetooth op je pc, Android of iPhone/iPad/iMac met een BLE-terminal-app of met [deze HTML BLE-terminal webpagina](https://ednieuw.com/BLESerial/BLE_UART_Terminal.html).

Vóór het compileren kunnen verschillende ontwerpen worden gekozen.  (Als je tenminste een woordklok-frontplaat hebt.)<br>
Gebruik een woordklok-frontplaatontwerp [uit deze repository](WordPlateFrontCovers)  of gebruik je eigen ontwerp en pas de positie van de LEDs in de software aan.<br>

![ClockDefines](https://github.com/user-attachments/assets/89a48534-c97a-41fc-94a5-bb8ee35ebb34)

Kies een van de woordklokontwerpen
```
#define NL144CLOCK              // Dutch display for 12 x 12 Front
//#define FOURLANGUAGECLOCK       // Four-language clock with 625 LEDs 
//#define NL92CLOCK               // Dutch display for one LED behind every word
//#define UK144CLOCK              // English display for 12 x 12 Front.
//#define FR144CLOCK              // French display for 12 x 12 Front.
//#define DE144CLOCK              // German display for 12 x 12 Front.
...
```

- #define NL144CLOCK -> een klok met 144 LEDs voor één taal. Standaardtaal is Nederlands. Voor Frans, Duits en Engels kopieer je de code van de vierstalenklok tussen de NL144CLOCK-defines.<br>
 [Bouwinstructies voor de klok in het Nederlands en Engels](https://ednieuw.com/Woordklok/Bouwpakket/WoordklokSK6812.htm)
of [hier in deze repository](Manual-Instructions)

- #define NLM1M2M3M4L??? -> een klok met ??? LEDs voor één taal met 4 extra LEDs voor de minuten en een iets ander ontwerp. Veel lay-outs zijn gedefinieerd.

- #define FOURLANGUAGECLOCK-> een vierstalenklok met 625 LEDs in een raster van 25 x 25.<br>
[Bouwinstructies van de vierstalen woordklok met SK6812 LEDs in UK, NL, DE, FR met Nano Every.](https://github.com/ednieuw/FourLanguageClock). 

In de libraries.zip staan de bibliotheken om de software te compileren.<br>
Pak ze uit in je libraries-map (de libraries-map in de map waar je sketches standaard worden opgeslagen)<br>
Download de programmamap en compileer voor Arduino Nano ESP32 met de Espressif core 3.<br>
Bibliotheken worden voortdurend bijgewerkt, wat na verloop van tijd tot incompatibiliteit met deze broncode kan leiden. Daarom zijn deze bibliotheken meegeleverd.

Een alternatief is om de ArduinoOTA-sketch te compileren die je vindt in de voorbeelden van de Arduino IDE.<br>
Upload de OTA-updater in de Nano ESP32 en noteer het IP-adres dat in de Serial monitor van de Arduino IDE wordt afgedrukt.
Typ deze IP-adres-URL in een browser en voer admin/admin in als gebruiker en wachtwoord.<br>
Upload daarna het .bin-bestand dat de Arduino IDE maakt met Sketch->Export compiled binary.<br>
<br>
Of gebruik deze: [Upload .bin-bestanden in de ESP32 met wifimanager](https://github.com/ednieuw/OTA-webupdater).<br>
<br>
# Hoe te compileren: 
Op het moment van schrijven (juni 2026) compileert de Espressif ESP32 board core V3.3.9 met het Arduino Nano ESP32-bord geselecteerd tot een werkend programma wanneer ESP32WordclockV220.ino of een hogere versie wordt gebruikt.
Selecteer het Nano ESP32-bord van Arduino in de Arduino IDE. 
Het Arduino ESP32-bord met core-versie 2.0.17/2.0.18 compileert ook zonder fouten bij gebruik van de Adafruit Neopixel-bibliotheek.


![image](https://github.com/user-attachments/assets/1f4eeec5-ebc5-47fb-b513-2514f5ff6ecd)

Er zijn twee methoden voor pinnummering bij het compileren: de ene gebruikt de GPIO-pinnummering van de ESP32-S3 en de andere de Arduino-pinnummering.<br>
Deze code werkt prima met GPIO-pinnummering geselecteerd.<br>
De Neopixel-bibliotheek geeft een foutmelding als dit niet is geselecteerd. 
De EdSoftLED-bibliotheek kan beide pinnummeringen gebruiken en compileert alleen met Espressif core 3.3.n omdat deze de RMT-functionaliteit gebruikt om de LEDs aan te sturen.

![Nano-ESP32 Pinout](https://github.com/ednieuw/Arduino-ESP32-Nano-Wordclock/assets/12166816/8d2201ea-e34a-4734-9fc0-5480a702290c)

Installeer het Arduino Nano ESP32-bord<br>
```
Board: Arduino Nano ESP32
Partition Scheme: With FAT
Pin Numbering: By GPIO number (legacy)  !! use this when using Neopixel 
```
<br>

![SerialOutputs](https://github.com/ednieuw/Arduino-ESP32-Nano-Wordclock/assets/12166816/6b1e48c3-783a-4b4e-bc32-eaa1fe344297)

# Voordat je begint

De klok ontvangt de tijd van internet wanneer deze met een router is verbonden.

Wanneer een DS3231-tijdmodule op de printplaat is aangesloten, is een internetverbinding niet nodig. Selecteer in het menu dat de DS3231 wordt gebruikt in plaats van de NTP-tijd. (J Toggle use DS3231 RTC module)
Houd er rekening mee dat de DS3231-tijd ook wordt gebruikt wanneer er een internetverbinding is. 

Om met internet te verbinden moeten de naam van het WIFI-station en het wachtwoord in de klok-software worden ingevoerd om met een WIFI-router te kunnen verbinden, of gebruik de persoonlijke hotspot van je mobiel.

### Verbinden met WIFI
De naam van het WIFI-station en het wachtwoord moeten eenmalig worden ingevoerd. Deze gegevens worden opgeslagen in het geheugen van de microprocessor.

Als de klok-software wordt gestart zonder SSID-routernaam, start deze een WIFI-station waarmee je kunt verbinden. 
- Open op je telefoon, tablet of pc de WIFI-verbindingen. In de lijst met WIFI-stations staat er één met de naam: StartWordClock. 
- Maak er verbinding mee en voer het wachtwoord in: 12345678
- Voer de SSID en het wachtwoord van je WIFI-router in. Deze gegevens staan vaak onderop je router.
- Wanneer op Submit wordt gedrukt, start de klok opnieuw op en verbindt met internet.

### Verbinden via Bluetooth
Voor het gemak verdient het de voorkeur om een telefoon of tablet en een Bluetooth-communicatie-app te gebruiken om de WIFI-gegevens in de klok in te voeren.<br>

Met sommige browsers kun je ook [deze webgebaseerde BLE HTML van mijn site](https://ednieuw.com/BLESerial/BLE_UART_Terminal.html) gebruiken

| | |
|:---:|:---:|
| <img src="Pics/BLEHTMLUART.png" alt="BLE UART terminal" width="350"> | ![image](https://github.com/ednieuw/Arduino-ESP32-Nano-Wordclock/assets/12166816/261800f1-7cd6-4078-8c7e-ad9cd2ba47ec) |
| [BLE UART HTML van mijn site](https://ednieuw.com/BLESerial/BLE_UART_Terminal.html) | Bluetooth UART seriële terminal-app |   

BLESerial nRF	BLE Serial Pro	Serial Bluetooth Terminal
- Download een Bluetooth UART seriële terminal-app op je telefoon, pc of tablet.<br>

- Voor IOS iPhone of iPad (Koop deze om mij te steunen): [BLE Serial Pro](https://apps.apple.com/nl/app/ble-serial-pro/id1632245655?l=en). <br>
Of de gratis, minder geavanceerde app: [BLE serial nRF](https://apps.apple.com/nl/app/bleserial-nrf/id1632235163?l).<br>
Tip: Zet Fast BLE aan met optie + in het menu voor snellere overdracht.

- Voor Android gebruik: [Serial Bluetooth terminal](https://play.google.com/store/apps/details?id=de.kai_morich.serial_bluetooth_terminal). <br>
Zet Fast BLE niet aan in het menu. (Uit = standaard)

Voer de SSID-naam en het wachtwoord van je router in, voorafgegaan door een A voor de SSID en een B voor het wachtwoord.
- aSSIDnaam en druk op Send
- bWachtwoord en druk op send
Optioneel
- cKlokNaam en druk op send. 
KlokNaam is de naam van de klok zoals weergegeven in de WIFI-router in de lijst met bluetooth-apparaten in de BLE seriële terminal-app


# Bediening van de klok
Als de klok met internet is verbonden, zoekt deze contact met een tijdserver. (De NTP-verbinding kan in het menu worden uitgezet).

De tijdzone is ingesteld op UTC+1 Amsterdam maar kan in het menu worden gewijzigd. <br>
Om met een WIFI-netwerk te verbinden moeten een SSID en wachtwoord worden ingevoerd.

Als de klok is voorzien van een resetknop, herstelt twee seconden ingedrukt houden de fabrieksinstellingen. <br>
Bluetooth en WIFI worden ingeschakeld.
 
Er zijn een paar methoden: <br>
1. Verbind de MCU met een seriële kabel met een pc en gebruik een seriële terminal.

Ik gebruik de Arduino IDE als seriële terminal. 

Door het teken 'I' voor informatie te sturen wordt het menu getoond, gevolgd door de actuele instellingen van diverse voorkeuren. 

<table style="width: 90%"><tr>
<td style="width: 342px"><img alt="Menu on iPhone" src="Pics/menuphone.jpg" width="350"  /></td>
<td style="width: 415px"><img alt="Termite terminal" src="Pics/Termite.jpg" width="400"  /></td>
</tr>

<tr>
HTML-pagina op iPhone.
Termite-terminal op een pc.
</tr>
</table>
<br>

2. Gebruik de BLE nRF-verbinding met een UART seriële terminal-app om de klok met je mobiele telefoon of tablet te bedienen. 

Gebruik de IOS-app voor iPhone of iPad: [BLE Serial Pro](https://apps.apple.com/nl/app/ble-serial-pro/id1632245655?l=en). <br>
of de gratis app BLEserial nRF: https://apps.apple.com/nl/app/bleserial-nrf/id1632235163?l=en-GB
Zet Fast BLE aan met optie Z in het menu. 

Voor Android gebruik:[Serial Bluetooth terminal](https://play.google.com/store/apps/details?id=de.kai_morich.serial_bluetooth_terminal). <br>
Zet Fast BLE uit (standaard) in het menu. 

Start de app, zoek de MCU in de lijst met apparaten en maak er verbinding mee. Je kunt de beaconnaam in het menu wijzigen met optie C. 

In beide gevallen stuur je het teken I van Informatie en verschijnt het menu.<br>
Voer het eerste teken in van de instelling die je wilt wijzigen, gevolgd door een code. <br>
Sommige invoer schakelt alleen aan en uit. Zoals de W om WIFI uit of aan te zetten. 
  
 Om de SSID en het wachtwoord te wijzigen: <br>
Amijn-ssid en verstuur dit commando. Bijv. AFRITZ!Box01 of aFRITZ!Box01. 
Beginnen met een hoofdletter of een kleine letter is een identiek commando in de commandostring <br>
Dan Bmijn-wachtwoord en verstuur het wachtwoord. (bijvoorbeeld: bGeheim)<br>
 
Cbroadcastnaam wijzigt de naam die in de Bluetooth-verbindingslijst wordt getoond. <br>
Als de lengte van de SSID en/of het wachtwoord minder dan 5 tekens is, wordt de WIFI automatisch uitgezet. Dit versnelt de opstarttijd als er geen internetverbinding beschikbaar is <br>
Gebruik een lengte van minimaal 8 tekens voor het wachtwoord en 3 tekens voor de SSID. <br>
Controleer in het menu (derde regel van onderen) of WIFI en NTP aan staan. <br>
Als WIFI verbonden is, pulseert de LED op de MCU groen. <br>
<br> Voer @ in om de MCU te resetten. Deze start opnieuw op en de verbindingen worden gemaakt.<br>
<br>
Om een tijdzone in te stellen, stuur je de tijdzonestring tussen de aanhalingstekens, voorafgegaan door het teken E of e. <br>
Zie de tijdzones onderaan deze pagina. <br>
Bijvoorbeeld, als je in Australië/Sydney woont, stuur dan de string: eAEST-10AEDT,M10.1.0,M4.1.0/3 

<pre>
___________________________________
___________________________________
A SSID, B Password, C BLE beacon name
D Date (D15012021), T Time (T132145)
E Timezone (E<-02>2 or E<+01>-1)
F Own, V BKGD color (Fwwrrggbb Fwrgb)
G Scan WIFI networks
H H01 rotary H02 buttons H03/04 remote
} Learn IR remote, + Fast BLE
I Info menu, II long menu
J DS3231 RTC, H05 Time receiver On/Off
K LDR /s, Time K1/min K2/hr K0/Off
N Display off between Nhhhh (N2208)
O Display On/Off, P StatusLED On/Off
Q Display choice (Q0-9), ~ Changing
R Reset settings,  @ Restart
S Slope, L Min, M Max (S50 L5 M200)
Unnn Demo (U200 msec), Y LED test
) HETISWAS  On/Off, ( EdSoft On/Off
#nnn Selftest,  RTC: ! See, & Update
W WIFI, X NTP, Z WPS, CCC BLE
Ed Nieuwenhuijs Jun 2026
___________________________________
Display off between: 22h - 08h
Display choice  : All Own
RandomDisplay:Off, Timestamp: Off
Slope: 50     Min: 5     Max: 25 
SSID: FRITZ!BoxEd
BLE name: ESPredPCB
IP-address: 192.168.178.172/update
Timezone:CET-1CEST,M3.5.0,M10.5.0/3
WIFI=On NTP=On BLE=On FastBLE=On
H00=On DS3231=Off TimeReceiver=Off
625 WS2812 LEDs (switch %) 
Software: ESP32_WordClockV223.ino
ESP32 Arduino core version: 3.3.8
__________________________________
</pre>
#### Menu getoond in de seriële uitvoer.

Zoals eerder vermeld kan de klok worden bediend met de WIFI-webpagina of de BLE UART terminal-app. <br>
Wanneer de klok met WIFI is verbonden, wordt het IP-adres in het menu getoond<br>

Voer dit IP-adres (cijfers en punten, bijvoorbeeld: 192.168.178.31) in de browser van je mobiel of pc in, waar je 
je internetadressen (URL) typt. 

Als de naam van je BLE-beacon wordclock is (zie in het woordklok-menu -> BLE name: wordclock)

Voer 'wordclock.local' in als URL in de browser. 
 Als dit niet werkt, installeer dan hostsoftware:
  - Voor Linux, installeer Avahi (http://avahi.org/).
  - Voor Windows, installeer Bonjour (http://www.apple.com/support/bonjour/).
  - Voor Mac OSX en iOS is ondersteuning al ingebouwd via Bonjour.

  - Ga met je browser naar http://wordclock.local, je zou een reactie moeten zien.

Het menu, links in de afbeelding hieronder, verschijnt. Met deze menu's kan bijna alle functionaliteit van de klok worden ingesteld, gewijzigd, en nog veel meer.

![alt text](WordFiboClockmenusS.jpg)


Of open de BLE-terminal-app. (in sommige apps moet je op een CONNECT-knop drukken).<br>
Zoek de WordClock om mee te verbinden en maak verbinding.

Voor Apple IOS-apparaten kan een BLE-verbinding worden gemaakt met mijn app [BLE Serial pro](https://ednieuw.com/BLESerial/BLESerialPRO.html) in de [app store](https://apps.apple.com/nl/app/ble-serial-pro/id1632245655?l=en) .<br>

Voor Android <a href="https://play.google.com/store/apps/details?id=com.nordicsemi.nrfUARTv2&hl=en&gl=US"> 
nRF UART terminal program </a>en <a href="https://play.google.com/store/apps/details?id=de.kai_morich.serial_bluetooth_terminal">
Serial Bluetooth terminal</a>. <br>
<br>
Als je een onleesbaar menu ziet en FASTBLE aan staat, stuur dan het teken '+' om FASTBLE uit te zetten en de langzamere overdrachtsmodus te gebruiken. 
 
Instellingen worden gedaan door het eerste teken van een commando in te voeren, gevolgd door parameters indien nodig.<br>
Om bijvoorbeeld de kleuren van de tekens in het display op wit te zetten, voer in: Q2 <br>
<br>
 Stel de tijd in met T130245. (130245 werkt ook) <br>
<br>
 Zet WIFI uit door een W te sturen. 
Reset de MCU met het teken @.<br>
Reset naar standaardinstellingen door R te sturen.<br><br>
 In de BLE-verbinding worden de SSID en het wachtwoord getoond.  <br>
	
<table cellspacing="6" style="width: auto">
<tr>
<td style="width: 280px">
<img alt="Phone" src="Pics/V200HTMLserialMenu.jpg" height="550" class="auto-style4" /></td>
<td>
<img alt="WIFI-BLE" src="Pics/V200BLEserialMenu.jpg" height="550" class="auto-style4" /></td>
</tr>
<tr>
HTML-pagina
BLEserial iPhone-menu
</tr>
</table>

## Gedetailleerde beschrijving

Met het menu kunnen veel voorkeuren worden ingesteld. <br>
Deze voorkeuren worden permanent opgeslagen in de opslagruimte van de Arduino Nano ESP32. 

Voer het eerste teken in het menu in van het item dat moet worden gewijzigd, gevolgd door de parameter. <br>
Voor de meeste invoer zijn hoofdletters en kleine letters identiek. 
Met de HTML-menupagina's kunnen veel van deze invoeren worden gekozen met knoppen, schuifregelaars of kleurenpaletten.

### Software bijwerken<br>
Software kan worden bijgewerkt met een .bin-bestand. Een bin-bestand wordt gemaakt in de Arduino IDE via het menu Sketch -> Export compiled binary. In de map waar de sketch is opgeslagen worden meerdere bestanden geplaatst in de map "build". Het WordFiboBlockVxxx.ino.bin van circa 1,5 Mb is het bestand om te uploaden.  


### A SSID, B Wachtwoord, C BLE-beaconnaam<br>
Wijzig de naam van de SSID van de router waarmee verbonden moet worden.<br>
aFRITZ!Box of AFRITZ!Box<br>
Voer daarna het wachtwoord in. Bijvoorbeeld: BSecret_pass <br>
en cMywordclock als naam van de BLE-beacon die op je telefoon wordt getoond. (standaard na een reset:  wordclock )
Herstart de MCU door @ te sturen.

### CCC<br>
CCC of ccc invoeren schakelt BLE aan of uit. Wees voorzichtig met uitzetten. Als BLE uit staat, kan de klok alleen worden bediend met WIFI of de USB seriële poort.

### D Datum instellen en T Tijd instellen <br>
Als er geen WIFI-verbinding is en een RTC DS3231 is aangesloten, moet de datum met de hand worden ingesteld.<br>
Voer bijvoorbeeld in: D06112022 om de datum op 6 november 2022 te zetten.  <br>
Voer bijvoorbeeld T132145 in (of 132145 , of t132145) om de tijd op 45 seconden en 21 minuten over één uur te zetten.

### E Tijdzone instellen E<-02>2 of E<+01>-1<br>
Onderaan deze pagina vind je de in 2022 gebruikte tijdzones. <br>
Voor Midden-Europa is de string: CET-1CEST,M3.5.0,M10.5.0/3  <br>
Het is een nogal ingewikkelde string en het is daarom verstandig om deze te kopiëren. <br>
Laten we er een kiezen als je hier toevallig woont: Antarctica/Troll,"<+00>0<+02>-2,M3.5.0/1,M10.5.0/3" <br>
Kopieer de string tussen de " " en stuur deze met een 'E' of 'e' ervoor. <br>
E<+00>0<+02>-2,M3.5.0/1,M10.5.0/3 

RRR invoeren zet de tijdzone terug naar Amsterdam CET. 
Je verliest bij deze reset ook je gegevens! en moet je SSID en wachtwoord opnieuw invoeren.

### Eigen kleur (FWWRRGGBB of Fwrgb) <br>
Met optie Q3 en Q4 uit het menu kun je je eigen kleuren voor de klok instellen.

Of druk op de HTML-menupagina op de link "🌈 Set colours".<br> Hier kan de kleur van het display worden gekozen met een kleurenpalet. Wanneer gekozen wordt automatisch de weergavekeuze "Q3 gebruikt je eigen gedefinieerde kleuren" geselecteerd. 

Het in te voeren formaat is hexadecimaal, 0123456789ABCDEF of decimaal, 0123456789. <br>
Het decimale bereik is gemaakt voor gebruikers die niet bekend zijn met hexadecimale rekenkunde.<br>
Het hexadecimale commando is 2 tekens voor wit, gevolgd door twee tekens voor Rood, gevolgd door twee tekens voor Groen en eindigend met twee cijfers voor Blauw.<br>
Het hexadecimale bereik is 00-FF, of 0-256 decimaal en het decimale bereik is beperkt tot 0-9.
Om de tekens intens rood te kleuren, voer je FF0000 in, voorafgegaan door het teken F. <br>
Om intens blauw in te stellen, voer je in: F000000FF of FFF.<br>
Om het gedimde teken donkergrijs in te stellen, voer je bijvoorbeeld in: F00191919. <br>
Je krijgt grijs als rood, groen en blauw dezelfde intensiteit hebben.
Met SK6812 LEDs kan de extra witte LED worden gebruikt naast de drie RGB-LEDs in dezelfde behuizing.
Bijvoorbeeld: F8800FF00 is 50% wit met 100% groen.

### G WIFI-netwerken scannen
Scan beschikbare Wi‑Fi-netwerken (print de gevonden SSID's).
Deze optie werkt niet vanuit de menu-webpagina.

### H H01 rotary encoder, H02 knoppen, H03/04 afstandsbediening, H05 Time sender-app.
Aanzetten zet ook NTP uit en het gebruik van de DS3231 aan. Controleer de DS3231- en NTP-instelling bij het uitzetten (H00)
De opties zijn:
H01 Rotary encoder<br>
H02 Toetsenbord<br>
H03 Grote IR-afstandsbediening met numeriek UP DOWN LEFT RIGHT ON/OFF en POWER<br>
H04 Kleine IR-afstandsbediening met zes knoppen<br>
H05 BLE-tijdontvanger aan/uit. <br>
Indien ingeschakeld luistert het apparaat naar tijd die wordt gestuurd door een BLE time sender-app op een telefoon of pc in de buurt en gebruikt dat om de klok in te stellen. <br>
Compatibele zenders:
- HTML BLE time sender (Chrome/Edge): [HTML-BLEserial](https://github.com/ednieuw/HTML-BLEserial)
- iOS BLE time sender-app: [BLEtimeSender](https://ednieuw.com/BLESerial/BLEtimeSender.html)
De H05-optie kan worden gebruikt wanneer de klok geen WIFI heeft. 

### } IR-afstandsbediening leren
Start het leren van de toetsen van een nieuwe infraroodafstandsbediening.
  
### I korte menu, II lange menu
Print het menu. 
`i` print het korte infomenu.
`ii` print het uitgebreide menu (lange help).

### J DS3231 RTC-module aan/uit
Wanneer de DS3231 is ingeschakeld, kan NTP worden uitgezet. Gebruik optie `X` om NTP indien nodig aan/uit te zetten.<br>
De knipperende oranje status-LED op het bord gaat uit wanneer de DS3231-tijd wordt gebruikt.

### K LDR-metingen/sec aan/uit<br>
Print de LDR-metingen en de berekende intensiteitsuitvoer.
K1 Zet het weergeven van de tijd elke minuut aan en uit.
K2 Zet het weergeven van de tijd elk uur aan en uit.
In het lange menu wordt de K1- of K2-status getoond. 
Als de tijd elke minuut of elk uur wordt geprint, loopt de logbuffer erg snel vol.
Als de tijd niet wordt geprint, blijft er meer relevante info in de logging.

### N Display uit tussen Nhhhh (N2208)<br>
Met N2208 wordt het display uitgezet tussen 22:00 en 08:00. 

### O Display aan/uit schakelen<br>
Schakelt het display uit en aan.

### P Status-LEDs aan/uit schakelen<br>
Schakel de status-LEDs op het bord aan/uit.

###  Q Weergavekeuze (Q0-9)<br>
Q0 toont de tijd met gele woorden., HETISWAS wisselend. <br>
HET verandert in een uur van groen naar rood via geel en IS/WAS verandert in een minuut van groen naar rood.<br>
Q1 toont elk uur een andere kleur. <br>
Q2 toont alle teksten wit. <br>
Q3 gebruikt je eigen gedefinieerde kleuren. 
Q4 gebruikt je eigen gedefinieerde kleuren met HETISWAS wisselend. <br>
Q5 volgt elk uur de regenboogkleuren. <br>
Q6 is de digitale klok als je 12x12 = 144 LEDs in de klok hebt gebruikt. <br>
Q7= Kleur per uur, HETISWAS wisselend.
Q8= Regenboogkleur wisselend in een minuut.
Stuur een 'I' om de laatste instellingen in het menu te tonen. 

### R Instellingen resetten <br>
R zet alle voorkeuren op de standaardinstellingen. De SSID en het wachtwoord, de tijdzone, het type LED-strip en het gebruik van rotary/IR blijven behouden.<br>
RRR wist de SSID en het wachtwoord, zet de tijdzone op Amsterdam (CET-1) en zet WIFI, NTP en BLE aan.<br>
RRRRR is een totale reset. Een combinatie van R en RRR.<br> 
Een totale reset kan ook worden bereikt met een resetknop aangesloten op D6 of D7. Als er geen rotary encoder wordt gebruikt, kunnen de rotary-encoderpinnen D2, D3 en D4 hiervoor ook worden gebruikt.

### Unnn Demo (U200 msec)
Voer U in, gevolgd door de duur van een seconde in milliseconden. 
M200 (200 milliseconde) versnelt de klok 5*60 keer.  
Door een U te sturen wordt de demomodus uitgezet of, als de demomodus niet actief is, gestart met 800 ms/min.

### V Achtergrondkleur (VWWRRGGBB of Vwrgb) <br>

Met deze optie stel je de kleur en intensiteit in van de LEDs die normaal niet branden.
Deze optie is handig wanneer er lichtlekkage tussen de tekens is of om de uitgeschakelde tekens te accentueren.<br>
Wanneer deze optie wordt gebruikt, wordt de minimale lichtintensiteit op 50 bits gezet om te voorkomen dat het achtergrondlicht 's avonds uitgaat.<br>
50 bits is 20% van de maximale intensiteit. Als je een achtergrond van bijvoorbeeld hexadecimaal 10 (=16 decimaal) invoert, is 20% van 16 = 3 bits en dat is bijna uit.<br>
<br>
Het in te voeren formaat is hexadecimaal, 0123456789ABCDEF of decimaal, 0123456789. <br>
Het decimale bereik is gemaakt voor gebruikers die niet bekend zijn met hexadecimale rekenkunde.
Het hexadecimale commando is 2 tekens voor wit, gevolgd door twee tekens voor Rood, gevolgd door twee tekens voor Groen en eindigend met twee cijfers voor Blauw.<br>
Het hexadecimale bereik is 00-FF, dat is een decimaal bereik van 0-255 en het decimale bereik is beperkt tot 0-9.
Om de tekens zwak rood te kleuren, voer je V00110000 of decimaal V0100 in. <br>
Om zwak blauw in te stellen, voer je in: V00000011 of V0001 decimaal.<br>
Met het commando V wordt het achtergrondlicht uitgezet en de minimale lichtintensiteit op 10 gezet

### Y Lichtjes afspelen
Om de werking van de LEDs te testen.

### S Slope, L Min, M Max (S50 L5 M200)

<table style="width: 100%">
<tr>
Om de lichtintensiteit te regelen kunnen drie parameters worden ingesteld. <br>
De waarden liggen tussen 0 en 255. <br>
De minimale lichtintensiteit voorkomt dat het display helemaal uitgaat wanneer het donker is. <br>
Het voorkomt ook flikkeren wanneer de ingangswaarde erg laag is, zoals 2 en 3 bits. <br>
Waarden tussen 5 en 20 worden voorgesteld. Als er geen LDR is aangesloten, kan de Min-waarde op 255 worden gezet.<br>
De maximale lichtintensiteit is 255 bits voor 100% intensiteit.  <br>
De Slope (gevoeligheid) bepaalt de snelheid waarmee de maximale waarde wordt bereikt. <br>
Het vermenigvuldigt de meting als percentage. Dus 100 invoeren is een vermenigvuldiging met één. <br>
De plaatsing en gevoeligheid van je LDR kan met deze parameter Slope worden aangepast.
<td> 
<img alt="Min Max Slope" src="Pics/MinMaxSlope.jpg" /><br>
Het effect van de waarde van de slope op de lichtintensiteit van de LEDs 
of het display.
</tr>
</table>

<table style="width: 100%">
<tr>
<td> 

<br>
### W=WIFI, X=NTP, Y=BLE
WIFI, NTP en BLE aan en uit schakelen.<br>
Door het teken te sturen wordt het aan- of uitgezet. <br>
Onderaan het lange menu (ii) wordt de status afgedrukt.<br>
 </td>
<td>
<img alt="Bottom menu" class="auto-style4" src="Pics/BottomMenu.jpg" /></td>
</tr>
</table>

### + Fast BLE

Het BLE UART-protocol verstuurt standaard pakketten van 20 bytes. Tussen elk pakket zit een vertraging van 50 msec.<br>
De IOS BLEserial-app, en misschien ook andere, kan pakketten van 80 bytes of meer ontvangen voordat tekens verloren gaan.<br>Bij de meeste apps zie je dat de lange regels van het menu worden afgekapt.<br>
Zet dan Fast BLE uit. <br>
Optie Z schakelt tussen de lange en korte pakketten. 

### ~ = weergavemodus wisselen per minuut of uur
~ start de optie voor wisselende weergavemodus. ~ of ~1 wisselt de weergavemodus elke minuut en ~2 elk uur. ~ of ~0 zet de optie uit. 
Het is handig voor testen en demonstraties.

### ! = NTP-, RTC- en DS3231-tijd tonen
!  toont de NTP-, RTC- en DS3231-tijd zoals deze in de klok zijn opgeslagen. De DS3231-tijdmodule moet zijn geïnstalleerd en in gebruik zijn om een realistische tijd te tonen.
Hetzelfde als optie & maar deze optie werkt de tijd niet bij vanaf de internet NTP-server maar toont alleen de tijd.

### #= Zelftest
Door een # te sturen start de zelftest van de klok. Dit is handig om te controleren of alle woorden in het display werken.<br> 
De tijd van een minuut wordt teruggebracht tot 0,9 seconde (900 milliseconden).<br>
#nnnn waarbij nnnn de vertraging tussen minuten in milliseconden is.
  

### % = Wisselen tussen SK6812 en WS2812 LED-strip
Met deze optie kan de gebruikte LED-strip worden gewijzigd. De klok is uitgerust met een van deze twee typen LED-strips. 
Een reset van alle instellingen door een R in het menu te sturen verandert deze LED-stripselectie niet.

### $ = Vuurweergave
Toont een vuur op het display bij sommige klokken met een 12 x 12 LED-matrixontwerp.

### ( = Extra optionele LEDs aan/uit
De klok kan optioneel worden uitgerust met een extra logotekst. 

### ) = HET/IS/WAS aan/uit
HET IS/WAS is altijd uit

### @ = MCU herstarten
@ herstart de MCU. Dit is handig wanneer de SSID enzovoort zijn gewijzigd en het programma opnieuw moet worden gestart. Instellingen worden niet gewist.

### & = NTP-tijd ophalen en opslaan in RTC- en DS3231-tijd
&  haalt de NTP-tijd direct van internet en slaat deze op in de RTC-klokken. Deze optie is handig om de klok te dwingen de juiste NTP-tijd te halen. 
In andere gevallen controleert het programma af en toe de tijd die in de klok loopt en die op de NTP-server en werkt de RTC-klokken bij.
De DS3231-tijdmodule moet zijn geïnstalleerd en in gebruik zijn om een realistische tijd te tonen.

### = = Waarden uit het permanente geheugen tonen in Mem
=  print de waarden die in het permanente geheugen van de Arduino zijn opgeslagen.

### 123456 Tijd instellen in RTC-module
Voer de tijd in als 152300 hhmmss. Hetzelfde als T152300
Het wijzigen van datum en tijd werkt alleen als een DS3231 RTC-module is aangesloten.

# Compileren en uploaden

De instellingen van het Arduino Nano ESP32-bord zijn als volgt.<br>
Denk eraan de ESP32-borden te installeren zoals hierboven uitgelegd.<br>

<img alt="Board settings" height="497" src="Pics/img6.gif" width="488" />


# Programmauitleg
 Het programma gebruikt de volgende standaardbibliotheken.   
<br>
<pre>
//------------------------------------------------------------------------------              //
// ESP32 Includes defines and initialisations
//-------------------------------------------
                      #if ESP_ARDUINO_VERSION >= ESP_ARDUINO_VERSION_VAL (5, 2, 0)            // Use EdSoftLED with ESP32 compiler.
#define USEEDSOFTLED  // Use EdsoftLED >= V1.7.0 for SK6812. Saves 200-500 bytes compared with NeoPixel. NeoPixel is OK
                      #endif
                      #ifdef USEEDSOFTLED
#include <EdSoftLED.h>         // https://github.com/ednieuw/EdSoftLED for LED strip WS2812 or SK6812 
                      #else
#include <Adafruit_NeoPixel.h> // https://github.com/adafruit/Adafruit_NeoPixel  *** USE GPIO NUMBERING and compile By GPIO number (legacy) 
                      #endif
#include <Preferences.h>
#include <NimBLEDevice.h>      // For BLE communication. !!!Use NimBLE version 2.x.x  https://github.com/h2zero/NimBLE-Arduino
#include <WiFi.h>              // Used for web page 
#include <esp_sntp.h>          // for NTP
#include <esp_wps.h>           // For WPS
#include <ESPAsyncWebServer.h> // Used for webpage  https://github.com/ESP32Async 
#include <Update.h>            // For Over-the-Air (OTA)
#include <ESPmDNS.h>           // To show BLEname in router
#include <DNSServer.h>         // For the web page to enter SSID and password of the WIFI router 
#include <Wire.h>              //  
#include <RTClib.h>            // Used for connected DS3231 RTC // Reference https://adafruit.github.io/RTClib/html/class_r_t_c___d_s3231.html
#include <Encoder.h>           // For rotary encoder
#include <Keypad.h>            // For 3x1 membrane keypad instead of rotary encoder by Mark Stanley & Alexander Brevig 
#include <IRremote.hpp>        // IR remote control

</pre>

Webpage.h is als TAB in de IDE in het programma opgenomen. Het bevat de webpagina die in de browser wordt getoond.<br>

Ik heb de webpagina gemaakt in het gratis 'Microsoft Expression Web 4'. Het wordt niet meer onderhouden maar heeft meer dan genoeg functionaliteit voor ons doel.<br>
Notepad++ of een AI zijn ook goede alternatieven<br>
<br>
Om de code uit MS-Expression te kopiëren: <br>
Klik onderaan het venster van MS-Expression op 'Split'.<br>
In de bovenste helft wordt de ruwe HTML-code getoond en in de onderste helft het 
Ontwerpvenster.<br>
Kopieer in het Code-venster alle HTML-code <br>
Ga naar de Arduino IDE en plak deze in de webpage.h TAB tussen: <br>
R"rawliteral(   ... en ...
 )rawliteral";<br>
<br>
  Of kopieer de code uit webpage.h naar het 
MSexpression Code-gebied en ontwerp de pagina naar wens opnieuw 

<img alt="MS expression" class="auto-style4" src="Pics/MSExpression.jpg" /><br>

Er volgt een lange lijst met definities en initialisaties. <br>
Ik ben geen fan van het doorgeven van alle variabelen van en naar functies en houd ze liever globaal in één programmalijst.<br>
Als je een programma met andere mensen schrijft, is het een goede gewoonte om niet te veel globale variabelen te gebruiken, maar dit programma staat om dezelfde reden in één groot listing, om het eenvoudig te houden.<br>
Ik heb alle variabelen per toepassing gegroepeerd om bij te houden waar ze worden gebruikt.<br>
Met een eenvoudige zoekopdracht is het in dit ene grote listing gemakkelijk om ze terug te vinden.<br>
<br>
Om de tijd als tekst en kleur met de juiste LEDs of tekens af te drukken, zijn de woorden en hun positie in een reeks LEDs of tekst gedefinieerd. <br>
De defines voeren de functie ColorLeds uit met de juiste parameters.<br>
Verderop in het programma, in de functies void Dutch(void), void English(void) enzovoort, wordt duidelijk waarom deze defines zo nuttig en handig zijn.  
De definities staan in de TAB "ClockFaces.h"
<pre> ...   
#define PRECIES ColorLeds("precies", 16, 22, LetterColor);
#define MTIEN   ColorLeds("tien", 25, 28, LetterColor); 
#define KWART   ColorLeds("kwart", 32, 36, LetterColor); 
#define VOOR    ColorLeds("voor", 38, 41, LetterColor); 
... 
</pre>
Dit is de initialisatie van het opslaggebied om de struct EEPROMstorage op te slaan.<br>
De Struct met al zijn instellingen wordt met één commando opgeslagen in permanent geheugen of SD <br>
Preferences FLASHSTOR; <br>
<br>
<pre> ...  
struct EEPROMstorage { // Data storage in EEPROM to maintain them after power loss 
byte DisplayChoice = 0; 
... <br>
char BLEbroadcastName[30]; // Name of the BLE beacon 
char Timezone[50];
int Checksum = 0;
} Mem;
... 
</pre>
<br>
De variabelen worden aangesproken met een korte naam Mem.<br> 
Bijvoorbeeld Mem.DisplayChoice = 3; <br>
<br>
De Struct EEPROMstorage wordt opgeslagen met de functie StoreStructInFlashMemory en opgehaald met de functie GetStructFromFlashMemory <br>
<br>

Het menu dat in de seriële monitor en BLE-app wordt getoond, wordt hier gedefinieerd.<br>

```cpp
// Menu 
//0        1         2         3         4 
//1234567890123456789012345678901234567890  
const char *menu[] = { 
"A SSID B Password C BLE beacon name", 
"D Date (D15012021) T Time (T132145)", 
... 
"W=WIFI, X=NTP, Y=BLE, Z=Fast BLE",  
"Apr 2026" };
```
 De Setup gebeurt hier: 
```cpp
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
```

De loop wordt vrijwel leeg gehouden en het programma start in Every SecondCheck.<br>
Er is niets in dit programma dat elke milliseconde of vaker moet worden uitgevoerd
 
```cpp
//-------------------------------------------- 
// ARDUINO Loop
//-------------------------------------------- 
void loop() 
{
 Loopcounter++;
 if (Demo)            Demomode();  
 else if(FireDisplay) FireMode(); 
 else                 EverySecondCheck();  
 CheckDevices();
}
```

De volgende routines controleren of er iets moet gebeuren elke seconde, minuut, uur en dag.<br>
Deze afhandeling houdt de processor voor 99% vrij voor ander gebruik.<br>
In dit programma is dat bijna niets, maar voor andere doeleinden kan dat nodig zijn.<br>
In de seriële monitor wordt het aantal loops per seconde afgedrukt. Dit kan handig zijn om te controleren of het programma te veel tijd elders in het programma doorbrengt. Op het moment van schrijven (V056) is dat ongeveer 200.000 l/s.
<br>

```cpp
//--------------------------------------------                //
// COMMON Check connected input devices
//--------------------------------------------
void CheckDevices(void)
{
 CheckBLE();
 SerialCheck();
 CheckWIFIcommand();
 if (Mem.TimeReceiver) CheckTimeReceiverClient();
 if (Mem.UseRotary==1) RotaryEncoderCheck();
 if (Mem.UseRotary==2) Keypad3x1Check();
 if (Mem.UseRotary==3 || Mem.UseRotary==4) IrReceiverDecode();
 if (ntpJustSynced) CheckandPrintNTPsynced();
                                  #ifdef ONEWIREKEYPAD3x4   
 OnewireKeypad3x4Check(); 
                                  #endif  //ONEWIREKEYPAD3x4
                                  #ifdef ONEWIREKEYPAD3x1   
 OnewireKeypad3x1Check(); 
                                  #endif  //ONEWIREKEYPAD3x1
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

//-------------------------------------------- 
// CLOCK Update routine done every day 
//-------------------------------------------- 
void EveryDayUpdate(void) 
{
... 
}
```
<br>

```cpp
//--------------------------------------------                                                //
// COMMON Update routine for the status LEDs
//-------------------------------------------- 
void UpdateStatusLEDs(int Toggle)
{
 if(Mem.StatusLEDOn)   
   {
    SetStatusLED((Toggle && WiFi.localIP()[0]==0) * 20, 
                 (Toggle && WiFi.localIP()[0]!=0) * 20 , 
                 (Toggle &&      deviceConnected) * 20);
    SetPCBLED09( Toggle * 10);                                                                // Left LED
    SetPCBLED10((!Toggle) * 10);                                                             // Right LED
    SetNanoLED13((!Toggle) * (!Mem.UseDS3231 * 50));                                         // LED on ESP32 board. IF DS3231 in use then it is off
   }
   else
   {
    SetStatusLED(0, 0, 0); 
    SetPCBLED09(0);                                                                           //
    SetPCBLED10(0);                                                                           //
    SetNanoLED13(0);      
   }
}
```
Werk de LEDs op de Nano ESP32 en de PCB bij. 
Op de Nano is analogWrite(LED_RED,   512); UIT en analogWrite(LED_RED, 0); is AAN<br>
Bij compileren met Espressif ESP32 core 3.x.x wordt de resolutie van het RMT-kanaal verhoogd van 8 -> 10 bits. <br>
Dit voorkomt dat de LED niet helemaal uitgaat bij compileren met core 3. ( 8191 = 10^2)

```cpp
analogWrite(LED_RED,   Red   == 0 ? 8191 : (512 - Red)); 
```

```cpp
//--------------------------------------------
// COMMON Control the RGB LEDs on the Nano ESP32
//--------------------------------------------
void SetStatusLED(int Red, int Green, int Blue) 
{
 analogWrite(LED_RED,   Red   == 0 ? 8191 : (512 - Red));
 analogWrite(LED_GREEN, Green == 0 ? 8191 : (512 - Green));
 analogWrite(LED_BLUE,  Blue  == 0 ? 8191 : (512 - Blue));
}
```

```cpp
//--------------------------------------------
// COMMON Control orange LED D13 on the Arduino 
//--------------------------------------------
void SetNanoLED13(int intensity) {analogWrite(secondsPin, intensity);}
```

```cpp
//--------------------------------------------
// COMMON Control the RGB LED on the PCB
//--------------------------------------------
void SetPCBLED09(int intensity) {analogWrite(PCB_LED_D09, intensity);}
void SetPCBLED10(int intensity) {analogWrite(PCB_LED_D10, intensity);}

```
Regel de kleur en intensiteit van de LED op de borden met één commando <br>
```cpp
//
--------------------------------------------
// CLOCK Control the LEDs on the ESP32 
// 0 Low is LED off. Therefore the value is inversed with the ! Not 
//-------------------------------------------- 
void SetStatusLED(bool Red, bool Green, bool Blue) 
{
```


Controleer op seriële invoer vanuit de seriële monitor en geef het commando door aan ReworkInputString()&nbsp;  
```cpp
//-------------------------------------------- 
// Common check for serial input 
//-------------------------------------------- 
void SerialCheck(void) 
{
... 
ReworkInputString(SerialString); // Processes the inputstring in the menu 
... 
}
```

Herstel alle standaardwaarden. 
```cpp
//-------------------------------------
// Common Reset to default settings 
//--------------------------------------
void Reset(void)
{
 Mem.Checksum         = 25065;
 Mem.DisplayChoice    = DEFAULTCOLOUR; 
 Mem.OwnColour        = green; 
 Mem.DimmedLetter     = black;
 Mem.BackGround       = black; 
...
 StoreStructInFlashMemory();   
 GetTijd(false); 
 SWversion();
 Displaytime();Tekstprintln("");
}

 }
 ```

Algemene printroutines.<br>

Om alle printcommando's op één plek te houden, is het eenvoudig deze routines aan te passen voor andere borden met een ander 'jargon'. <br>
```cpp
//-------------------------------------------- 
// Common common print routines 
//-------------------------------------------- 
void Tekstprint(char const *tekst)    { if(Serial) Serial.print(tekst); SendMessageTimeReceiver(tekst); SendMessageBLE(tekst); AddLog(tekst);}  
void Tekstprintln(char const *tekst)  { snprintf(sptext, sizeof(sptext),"%s\n",tekst); Tekstprint(sptext); }
void Tekstprintf(const char* fmt, ...) 
{ va_list args; va_start(args, fmt); vsnprintf(sptext, sizeof(sptext), fmt, args); va_end(args); Tekstprint(sptext);}

void Tekstprintlnf(const char* fmt, ...) 
{ va_list args; va_start(args, fmt); vsnprintf(sptext, sizeof(sptext), fmt, args); va_end(args);  Tekstprintln(sptext);}


```

De opzet van de opslagruimte en controle op de geldigheid van de instellingen.<br>
Als de checksum ongeldig is, herstelt een reset() de standaardinstellingen 

```cpp
//-------------------------------------------- 
// Common Init and check contents of EEPROM 
//-------------------------------------------- 
void InitStorage(void) 
```

Sla de instellingen op in en haal ze op uit SPIFFS of SD of EEPROM.
De verschillende mogelijkheden staan hier. EEPROM raakt verouderd maar werkt nog steeds. 

```cpp
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
```

SWversion() print het menu en de instellingen van diverse voorkeuren.<br>

PrintLine() print de horizontale lijnen in het menu.<br>

```cpp
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
```


Haal de commando's uit de strings die in de seriële monitor, via Bluetooth of de webpagina zijn ingevoerd en voer het commando uit in een actie.<br>
De menuletters zijn bijna op, maar het is mogelijk onderscheid te maken tussen kleine letters en hoofdletters als er meer commando's nodig zijn.<br>
(Daarom is er geen conversie naar hoofdletters of kleine letters). 

```cpp
/-------------------------------------------- 
//  COMMON Input from Bluetooth or Serial
//--------------------------------------------
void ReworkInputString(String InputString) 
{
....  
  int len = InputString.length();
  switch(toupper(InputString[0])) 
   {
    case 'A':
      if(len > 4 && len < 30)
.... 
```

Lees de LDR en deel deze door 16 om de waarden van 0 - 4096 om te rekenen naar 0 - 255.<br>
Niet alle borden hebben een 12-bits AD-converter.  

```cpp
//-------------------------------------------- 
// LDR reading are between 0 and 255.  
// ESP32 analogue read is between 0 - 4096 --   is: 4096 / 8 
//-------------------------------------------- 
int ReadLDR(void) {  return analogRead(PhotoCellPin)/16;} 
```


Hier printen en kleuren we de tekens in het display of laten we de juiste LEDs oplichten in een reeks RGB(W) LEDs.<br>
De #define voert deze functies uit met de juiste parameters voor elke taal en print de teksten in de seriële verbindingen.<br>
`#define QUARTER ColorLeds("quarter", 32, 38, LetterColor);`

```cpp
//--------------------------------------------    / 
 / LED Set color for LED. 
//--------------------------------------------   
void ColorLeds(char const *Texkst, int FirstLed, int LastLed, uint32_t RGBColor)  
{  } 
```

Elk display of elke strip gebruikt andere commando's om de helderheid te regelen. Daarom is er voor alle LED/Display-commando's één functie.
```cpp
//----------------------------------------------
// LED Set brightness of backlight 
//------------------------------------------------
void SetBrightnessLeds(byte Bright) 
{ 
 SetBackLight(Bright);                        // Set brightness of LEDs 
} 
```

Een plek om alle LEDs uit te zetten of het display te wissen<br>
```cpp
/-------------------------------------------- 
// LED Clear the character string 
//-------------------------------------------- 
void LedsOff(void)  
```

Hier worden de kleuren voor de tekens ingesteld.<br>
Het wordt ook gebruikt om de kleuren van HET IS WAS te wijzigen die bij sommige weergavekeuzes worden gebruikt

```cpp
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
```


Subroutine voor de zelftest van de klok. Standaard is er een vertraging van 900 ms.
```cpp
//------------------------------------
// CLOCK Self test sequence
//--------------------------------------
void Selftest(void){Selftest(900);}
void Selftest(int Delayms)
{ 
 GetTijd(true);                                                                                  // Prints time in Serial monitor
 LedsOff(); 
...
}
```

Displaytime() print de tijd als tekst naar de seriële monitor en bepaalt welke taal wordt geprint.<br>
Het stuurt ook de juiste reeks kleuren en intensiteiten naar een RGB(W) LED-strip. 
```cpp
//--------------------------------------------  
// CLOCK Say the time and load the LEDs 
// with the proper colour and intensity
//--------------------------------------------
void Displaytime(void) 
{  
LedsOff();   
 if( Mem.DisplayChoice == DIGITAL ) { TimePlaceDigit(timeinfo.tm_hour,timeinfo.tm_min); }
 else
   {
                     #ifdef NL144CLOCK
    Dutch();  
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
```

DimLeds() leest de analoge poort en berekent een uitgangsintensiteit voor een display of LED-strip.<br>
De metingen worden gekwadrateerd om een hyperbolische curve te krijgen die de oogcorrectie voor donker en licht beter benadert dan een lineair bereik.<br>
Het werkt buitengewoon goed.<br>

```cpp
//--------------------------------------------  
// LED Dim the LEDs measured by the LDR and print values  
// LDR reading are between 0 and 255. The Brightness send to the LEDs is between 0 and 255  
//--------------------------------------------  
void DimLeds(bool print) { ... } 
```

Hierna volgt een lijst met subroutines voor diverse LED-bewerkingen, inclusief de initialisaties van de WS2812- of SK6812-LED-strip
De SK6812 LED-strip heeft ook een witte LED waarvan de intensiteit wordt geregeld met de hoogste bytes (WW) in het 32-bits integer (0XWWRRGGBB)

```cpp
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
              gray   = 0x33000000; 
              dgray  = 0x22000000;
              wgray  = 0xAA000000; 

      break;
      case 1: LEDstrip = LED2812strip; 
              white  = 0xFFFFFF;
              lgray  = 0x666666; // R, G and B on together gives white light
...
}

```
 

Een reeks functies om tijd op te halen en op te slaan.<br>
De NTP-tijdserver zet de opgehaalde tijd in de standaard C-tijdstructuren.<br>

```cpp
/--------------------------- Time functions -------------------------- 

void GetTijd(byte printit) 
void Print_RTC_tijd(void) 
void PrintNTP_tijd(void) 
void PrintUTCtijd(void) 
void Print_tijd(void) 
void SetRTCTime(void) 

```

Zet een HEX-string om in een unsigned 32-bits integer<br>

```cpp
//--------------------------------------------
// CLOCK Convert Hex to uint32 
//-------------------------------------------- 
uint32_t HexToDec(String hexString) 
```


Functies waarmee de klokken de tijd in vier talen uitspreken
Er is ook veel spreektaal in talen. <br>
'Half negen' betekent soms 8:30 maar kan ook 9:30 zijn. (-:<br>
<br>

Deze definities staan in clockfaces.h 
```cpp
//--------------------------------------------
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



```

<br>

De Bluetooth Low Energy Nordic nRF.. functies.<br>
Ze verschillen van de Texas Instruments CC2540/CC2541 die in andere 
chipsets zoals de HM-10, HM16, JDY-08 enzovoort wordt gebruikt.<br>
[Meer hier op Instructables](https://www.instructables.com/Communicate-Using-CC254x-or-NRF-BLE-With-Arduino-a/)<br>
<br>


```cpp
/----------------------------- 
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
void CheckBLE(void) 
```

<br>
Functies om een WIFI-verbinding te starten en de webpagina te gebruiken<br>
<br>

```cpp
/-------------------------------------------- 
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
```
<br>



# Tijdzones
 
<br>
 Kopieer de tekst tussen de aanhalingstekens en plak deze na het teken E 
  

<pre> 
 Africa/Abidjan,"GMT0" 
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
