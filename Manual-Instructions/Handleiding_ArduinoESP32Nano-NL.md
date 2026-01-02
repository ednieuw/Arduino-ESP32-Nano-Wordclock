# Arduino ESP32-Nano woordklok

Een klok die de tijd in woorden weergeeft in de talen Nederlands, Engels, Frans en Duits in een grote 4-talenklok of als een ééntalige klok.

Een Arduino ESP32 Nano computer in de klok wordt gebruikt om de tijd in de klok aan te sturen en verbinding met WIFI en Bluetooth op te zetten.\
De tijd wordt gesynchroniseerd met het Network Time Protocol (NTP) van het internet via een WIFI-verbinding. Daarnaast bestaat de optie om de tijd zonder WIFI of Bluetooth met draai/drukknop gelijk te zetten.\
Instellingen kunnen worden beheerd met een webpagina, een PC of een Bluetooth Low Energy (BLE) seriële terminal-app die is geïnstalleerd op een telefoon, pc of tablet.

  ------------------------------------------------------------------------------------------------------------------------------------------------------------------
  ![](media/image1.jpeg){width="3.2083333333333335in" height="3.2305555555555556in"}       ![](media/image2.jpeg){width="3.2916666666666665in" height="3.23125in"}
  ---------------------------------------------------------------------------------------- -------------------------------------------------------------------------
  4-talenklok                                                                              Nederlandse taalklok

  ![image](media/image3.jpeg){width="3.3319444444444444in" height="3.188888888888889in"}   

  Cortenstaalklok                                                                          
  ------------------------------------------------------------------------------------------------------------------------------------------------------------------

+------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+---+
| **Snelle start**                                                                                                                                                                                                                                                                               |   |
|                                                                                                                                                                                                                                                                                                |   |
| Zoek één van de volgende apps in de app store. De app wordt gebruikt om commando's naar de klok te sturen.                                                                                                                                                                                     |   |
|                                                                                                                                                                                                                                                                                                |   |
| Mocht de app niet te downloaden zijn lees dan verder hoe de WIFI aan te sluiten.                                                                                                                                                                                                               |   |
|                                                                                                                                                                                                                                                                                                |   |
| +-----------------------------------------------------------------------------------+-----------------------------------------------------------------------------------+--------------------------------------------------------------------------------------------------------------------+ |   |
| | IOS iPhone/iPad/Mac                                                               | IOS iPhone/iPad/Mac                                                               | Android                                                                                                            | |   |
| +===================================================================================+===================================================================================+====================================================================================================================+ |   |
| | ![](media/image4.png){width="0.6869291338582677in" height="0.6840015310586177in"} | ![](media/image5.png){width="0.6182370953630796in" height="0.6299015748031496in"} | ![](media/image6.png){width="0.6827766841644795in" height="0.6856321084864392in"}                                  | |   |
| +-----------------------------------------------------------------------------------+-----------------------------------------------------------------------------------+--------------------------------------------------------------------------------------------------------------------+ |   |
| | [BLE Serial Pro](https://apps.apple.com/nl/app/ble-serial-pro/id1632245655)       | [BLESerial nRF](https://apps.apple.com/nl/app/ble-serial-tiny/id1607862132)       | [Serial Bluetooth Terminal](https://play.google.com/store/apps/details?id=de.kai_morich.serial_bluetooth_terminal) | |   |
| |                                                                                   |                                                                                   |                                                                                                                    | |   |
| |                                                                                   |                                                                                   | (Zet timestamp uit in het menu)                                                                                    | |   |
| +-----------------------------------------------------------------------------------+-----------------------------------------------------------------------------------+--------------------------------------------------------------------------------------------------------------------+ |   |
+================================================================================================================================================================================================================================================================================================+===+
+------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+---+

![](media/image7.png){width="1.8138888888888889in" height="2.6430555555555557in"}De klok is ingesteld om met WIFI de tijd te ontvangen. Als de naam van de WIFI-router en wachtwoord niet zijn ingevoerd wordt een webpagina gestart om de routergegevens in te voeren.

-   Open in een telefoon of tablet bij de WIFI-instellingen het station "StartWordclock".

-   Tik als wachtwoord in: wordclock

-   Open daarna de browser op het mobiel en tik in de URL: 162.168.4.1 en druk enter.

-   Voer de naam en het wachtwoord van de WIFI-router die in huis gebruikt wordt.\
    **Tip**: Je kan ook de hotspot van je mobiel voor een WIFI-verbinding gebruiken.

**Bluetooth app**

Instellen is makkelijker met de Bluetooth app op de telefoon of tablet.

-   ![](media/image8.wmf){width="1.8506944444444444in" height="2.2888888888888888in"}Open de Bluetooth app.

De IOS app toont een lijst met Bluetooth stations.\
Als het te lang duurt. Druk wieltje linksboven (nRF52 is geselecteerd) en druk Done.\
Bij Android druk Scan en Connect met de woordklok.

-   Stuur het commando: **i** om het menu te zien

Tik in de letter **a** gevolgd door de naam van de router en druk Send. (bijv: aNAAMrouter Stuur geen extra punten of spaties.)

-   Tik in de letter **b** gevolgd door het wachtwoord van de router en druk Send. (bijv: bGeHeim1234)

-   Stuur @ om de klok te herstarten.

**\
**

**Of start WPS** (Een knop op de router met WPS. Lees dit na in de handleiding)

-   Druk de WPS-knop en stuur daarna het commando: **z** in de Bluetooth serial app.

-   **Als geen WIFI mogelijk is**\
    Stuur de letter J in het menu met de Bluetooth app.

-   Er verschijnt dan: Use DS3231 is ON. WIFI ON, NTP OFF.

-   Stuur een Letter W om WIFI uit te zetten. Stuur @ om de klok te herstarten.

-   Stuur de juiste tijd en datum via het menu.\
    (bijv stuur: T143000 voor 'half drie' en bijv als datum: D090625)

-   ![](media/image9.jpeg){width="5.4847222222222225in" height="2.313888888888889in"}

Klein PCB-ontwerp met Rotary en DS3231 RTC bevestigd

**\
**

**Voor het starten\
**De klok ontvangt de tijd van het internet als er een WIFI-verbinding is.

Wanneer een DS3231-tijdmodule op de printplaat is bevestigd, is een internetverbinding niet vereist.

Als een draai- of drukknopknop geïnstalleerd is kan deze worden gebruikt om de tijd in te stellen.

Ook kan een infrarood afstandsbediening geïnstalleerd zijn om de tijd in te stellen.

Om verbinding te maken met het internet moet de naam van het WIFI-station en het wachtwoord in de klok-software worden ingevoerd om verbinding te kunnen maken met een WIFI-router.\
De naam van het WIFI-station en het wachtwoord moeten eenmalig worden ingevoerd.\
De inloggegevens worden opgeslagen in het geheugen van de microprocessor.

Een seriële terminal app op een telefoon, tablet of PC maakt bediening mogelijk zonder de klok met een USB kabel aan een PC te verbinden om de WIFI inlog-gegevens in te voeren.

De klok kan ook via een browser worden aangestuurd als er verbinding is gemaakt met WIFI.

  --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  ![](media/image10.png){width="1.8840277777777779in" height="1.9270833333333333in"}   ![](media/image11.png){width="2.1875in" height="1.9381944444444446in"}   ![](media/image12.png){width="2.0597222222222222in" height="1.6756944444444444in"}
  ------------------------------------------------------------------------------------ ------------------------------------------------------------------------ ------------------------------------------------------------------------------------
  BLESerial nRF                                                                        BLE Serial Pro                                                           Serial Bluetooth-terminal

  --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

\- Download een Bluetooth UART seriële terminal app op een telefoon, PC of tablet.\
Voor IOS: BLE Serial Pro of BLESerial nRF.\
Voor Android: Serial Bluetooth-terminal.\
Voor een PC: installeer de Arduino IDE op een PC en verbind de klok via een USB-C kabel met de PC.

![](media/image13.jpeg){width="3.536111111111111in" height="2.6534722222222222in"}\
Arduino ESP32 Nano op de printplaat in de klok.

**Eerste bediening**

\- Sluit de voeding van de klok aan.

**Als er geen internetverbinding of Bluetooth-verbinding is**, kan de klok worden bediend met de draaiknop, met drie drukknoppen, of een afstandsbediening, mits deze geïnstalleerd en geactiveerd is. Activeren gebeurd in het menu met optie H01 (draaiknop, H02 drukknoppen of H03 en H04 een IR-afstandsbediening.\
Soms zit de knop in de kast gemonteerd.

\- Druk op de draaiknop of de middelste knop van de drie drukknoppen.\
UUR licht 3 keer op en de uren kunnen worden ingesteld door de knop te draaien of op de omhoog- of omlaag-knop te drukken op de drieknopmembraan.

\- Een tweede keer drukken op de knop of de middelste knop laat\
HETISWAS knipperen en de minuten kunnen worden gewijzigd.\
Seconden worden ingesteld op 0. Dus om precies te zijn, zet de laatste minuut precies op 0 seconden.

\- Met een derde keer drukken knippert TWAALF drie keer en kan de lichtintensiteit van de LED\'s worden ingesteld.

\- Met een vierde keer drukken knipperen de tekens NTP en RTC drie keer. Men kan overschakelen van RTC- naar NTP-tijd.

\- Negen keer drukken reset de klok naar de fabrieksinstellingen.

Met de IR-afstandsbediening kan de klok bedient worden als deze geleerd is met de klok. Met de { optie in het menu vraagt de klok om knoppen te druken die overeenkomen met de getallen 0-9 en een paar bedieningsknopen zoals POWER.

**\
**

**Via WIFI-verbinding**

Als de klok voor de eerste keer gestart wordt zal een WIFI-verbinding moeten worden gemaakt.

Als er nog oude verbindingsgegevens in de klok staan voer dan 5 maal hoofdletter R in het menu (RRRRR) in om deze te wissen.

De klokinstellingen staan dan op:

SSID en password zijn leeg

Timezone:CET-1CEST,M3.5.0,M10.5.0/3

WIFI=On NTP=On BLE=On

**WPS**

WPS is een methode om het wachtwoord en de naam van het WIFI-station, de SSID, uit de router te krijgen..

Start WPS in de klok door Z in het menu in te voeren en start WPS op de WIFI-router. Als alles goed gaat herstart de klok en zal de tijd gelijk staan.

**Accespunt in de telefoon**

Bij de WIFI-verbindingen in de telefoon (tablet op PC) zal bij de router waarmee je verbonden bent een "StartWordclock" station staan.

-   Verbind met het "StartWordclock" station. Gebruik als wachtwoord: **wordclock**

-   Open een browser en tik in: 192.168.4.1 en druk enter.

![](media/image14.jpeg){width="2.911111111111111in" height="1.5520833333333333in"}

-   Tik in dit scherm de SSID en het password van de WIFI router in en druk Submit

> Deze gegevens staan vaak onderop de router.

![](media/image15.png){width="2.497998687664042in" height="2.7968285214348207in"}

De klok zal herstarten en de juiste tijd weergeven.

**Wanneer de Bluetooth-app op de telefoon of tablet is geïnstalleerd**

Met de 'Bluetooth serial terminal' app kan de klok via bluetooth worden bedient met je telefoon, tablet of MacOS PC.

\- Start de Bluetooth-terminal-app op de telefoon of tablet

\- Klik op de verbinding met de naam \'wordclock\' in de seriële terminal-app.

\- Zoek de inloggegevens van uw WIFI-router.\
Zoek de SSID van de router WIFI en het wachtwoord. Deze staan ​​normaal gesproken aan de onderkant van de WIFI-router

Typ in:

\- aSSID en druk op Verzenden.\
SSID is de naam van de SSID die u bijvoorbeeld op uw telefoon kunt vinden. Het is de naam waarmee WIFI is verbonden.

\- bPASSWORD en druk op verzenden.\
PASSWORD is het wachtwoord dat u onderop de WIFI-router kunt vinden.

\- cBLENAME (optioneel, anders heeft de klok de naam 'Wordclock')\
De BLENAME is de naam van de klok in de BLE seriële terminal-app en in de verbonden WIFI-router.

Als de Routernaam (SSID) of het wachtwoord niet juist is zal de klok na enkele herstarts weer de Accesspunt pagina (192.168.4.1 ) in de telefoon starten.

Via deze accespuntpagina, Bluetooth of met de USB kabel kan de juiste gegevens worden ingevoerd.

Zie voor uitgebreide uitleg hoofdstuk: Bediening en instellingen van de klok.

+-----------------------------------------------------------------------------------------------------+------------------------------------------------------------------------------------+
| ![](media/image16.png){width="2.425in" height="1.2465277777777777in"}                               | ![](media/image17.png){width="2.4090277777777778in" height="2.9159722222222224in"} |
|                                                                                                     |                                                                                    |
| In de FRITZBox WIFI router is als voorbeeld de klok met de naam "SteelClock-01" met WIFI verbonden. |                                                                                    |
|                                                                                                     |                                                                                    |
| Je kan de webpagina van de klok in een browser starten door te tikken:                              |                                                                                    |
|                                                                                                     |                                                                                    |
| [http://SteelClock-01](http://SteelClock-01/) or SteelClock-01.local.                               |                                                                                    |
|                                                                                                     |                                                                                    |
| De standaardnaam is: wordclock en dan wordt de url om de menupagina te starten: wordclock.local.    |                                                                                    |
+=====================================================================================================+====================================================================================+
+-----------------------------------------------------------------------------------------------------+------------------------------------------------------------------------------------+

Met de optie I in het menu wordt gekozen voor een kort menu voor alledaags gebruik of voor een lang menu met optie ii (2x i) voor een uitgebreid menu met alle opties.

![](media/image18.png){width="6.268055555555556in" height="6.961111111111111in"}

Door 'Log view' boven in de web pagina te klikken wordt de Logviewer gestart. Deze toon de laatste 500 regels die naar de seriële poort of de bluetooth app zijn geprint.\
In Chrome kan (december 2025) de TAB gesplitst worden (klik met rechtermuisknop op de TAB en kies 'split TAB' o.i.d.) en kan zowel de webpagina als de logviewer getoond worden.\
'All log files' toont de laatste vele duizenden regels.

**Wanneer de draaiknop/rotary encoder of een drieknop membraanknop of een IR-afstandsbediening is geïnstalleerd**

Een draai- , drukknoppen of afstandsbediening en de zeer nauwkeurig lopende klok DS3231 RTC zijn optioneel en niet persé nodig. De tijd wordt dan namelijk met het internet gesynchroniseerd.\
Deze componenten zijn wel nodig als een klok geen WIFI kan ontvangen.

In het klokmenu kan de rotary encoder (of drie drukknoppen) aan of uit worden gezet.\
Zet de rotary met de optie H00 in het menu uit als er geen draai- , drukknoppen of afstandsbediening is geïnstalleerd. (Open het langere instelmenu door ii te sturen).\
Zet het gebruik van de DS3231 RTC (menuoptie J) ook uit zodat de tijd van internet wordt gebruikt.

**Wanneer draaiknop/rotary encoder is geïnstalleerd**

De draaiknop is 60 seconden actief na het drukken op de knop.

\- Druk op de draaiknop of de middelste knop van de drie drukknoppen.\
UUR licht 3 keer op en de uren kunnen worden ingesteld door de knop te draaien of op de omhoog- of omlaag-knop te drukken op de drieknopmembraan.

\- Een tweede keer drukken op de knop of de middelste knop laat\
HETISWAS knipperen en de minuten kunnen worden gewijzigd.\
Seconden worden ingesteld op 0. Dus om precies te zijn, zet de laatste minuut precies op 0 seconden.

\- Met een derde keer drukken knippert TWAALF drie keer en kan de lichtintensiteit van de LED\'s worden ingesteld.

\- Met een vierde keer drukken knipperen de tekens NTP en RTC drie keer.\
Men kan overschakelen van RTC- naar NTP-tijd.

\- Negen keer drukken reset de klok naar de fabrieksinstellingen.

**Wanneer drukknoppen zijn geïnstalleerd**

De drukknopen zijn 60 seconden geactiveerd na het drukken van een \*

Tik daarna de tijd in met 6 getallen als uummss.

Bijvoorbeeld 134500 voor kwart voor twee.

Na invoer van het 6^de^ getal wordt de tijd vanzelf ingevoerd.

**Wanneer een IR-afstandsbediening is geïnstalleerd**

Zet de ontvangst met de klok aan met POWER.\
Verzend met de grote afstandsbediening (H03 geactiveerd in het menu) de juiste tijd met cijfers als uummss. Dus kwart voor drie als 144500 in de middag of als 024500 in de ochtend.\
Of gebruik met een kleine afstandsbediening (H04 geactiveerd in het menu) de knoppen Up en Down en CH+ en CH- om de minuten en of uren te veranderen.

Met ONOFF kan de verlichting van de klok worden uit- of aangezet.

**\
**

**De klokkast maken**

De bouw van de klokkast met verlichting is hier te vinden:

<https://ednieuw.home.xs4all.nl/Woordklok/Bouwpakket/WoordklokSK6812.htm>

of hier: https://github.com/ednieuw/Arduino-ESP32-Nano-Wordclock

Bouwinstructie van een 4-talige woordklok met SK6812 LEDs in het UK, NL, DE, FR met Nano. https://github.com/ednieuw/FourLanguageClock

**Compilatie en uploaden**

De software draait op een Arduino Nano ESP32.

Deze software wordt met de Arduino IDE gecompileerd en naar de Nano ESP32 geüpload.

Voor elk board dat met de Arduino IDE geprogrammeerd kan worden is een compiler beschikbaar. Deze kan geselecteerd worden in het boards manager menu

Er zijn twee versies beschikbaar voor de Arduino Nano ESP32.

Eén versie, van Arduino zelf, die gebruik maakt van core versie 2. Deze versie is probleemloos in gebruik.

![](media/image19.png){width="5.302083333333333in" height="4.077083333333333in"}

-   Laad het bestand ESP32Arduino_WordClockV0xx.INO in de IDE

Selecteer in de sketch , ergens rond regel 50, een van de drie woordklokken door de twee schuine strepen // weg te halen.

//#define FOURLANGUAGECLOCK

#define NL144CLOCK

//#define NLM1M2M3M4L114 // NL clock with four extra LEDs for the minutes to light up

#define NL144CLOCK -\> een 144 LED enkeltalige klok. Standaardtaal is Nederlands. Voor Frans, Duits en Engels kopieer de codering van de viertalige klok tussen de NL144CLOCK-definities.

#define NLM1M2M3M4L114 -\> een 110 LED enkeltalige klok met 4 extra LEDs voor de minuten en een iets ander ontwerp

#define FOURLANGUAGECLOCK-\> een 4-talenklok met 625 LEDs in een raster van 25 x 25 LEDs.

In de libraries.zip staan de bibliotheken om de software te compileren.\
Unzip ze in de libraries-map. Deze folder staat meestal tussen folder met de sketches.

Board: Arduino Nano ESP32

Partitieschema: met FAT

Pinnummering: By GPIO-nummer (legacy)

**Problemen**

Als de LEDs niet gaan branden, is de kans groot dat de pinnummeringinstelling\
(By GPIO-number (legacy) op By Arduino pin (default) staat.

Er is meer dan één klok geselecteerd bij de defines rond regel 50

Bluetooth werkt niet

Na versie ESP32Arduino_WordClockV060 moet NimBLE versie 2.0 worden gebruikt.

Voor versie ESP32Arduino_WordClockV060 werkt alleen NimBLE versie 1.4.3

![](media/image20.png){width="3.213888888888889in" height="2.3229166666666665in"} ![](media/image21.png){width="2.3229166666666665in" height="2.2756944444444445in"}

Lukt het niet probeer dan de methode 'Alternatieve softwareinstallatie'**.**

**Alternatieve softwareinstallatie**

![](media/image22.png){width="5.750694444444444in" height="3.9277777777777776in"}

Zoals verderop is beschreven kan software ook 'Over the Air' op de Nano ESP32 worden gezet.

Voordeel is dat je niet de libraries hoeft te installeren en over een paar jaar problemen hebt met niet meer compatibele libraries.

Het compileren is dan al gedaan en het bestand dat naar de Nano ESP32 geüpload wordt is dan als .bin bestand opgeslagen. Op GitHub kunnen diverse .bin-file versies worden gevonden.

<https://github.com/ednieuw/Arduino-ESP32-Nano-Wordclock>

-   Selecteer als board de Nano ESP32 (Tools-\>Board-ArduinoESPboards-\>Arduino Nano ESP32)

-   Zoek in Examples naar ArduinoOTA -\> en open het programma OTAWebUpdater

-   Kies Sketch-\>Upload of druk de upload knop links boven

-   Open de seriële monitor en kijk welk IP-adres geprint wordt.\
    (Druk op de witte resetknop op de Nano ESP32 als er niets geprint wordt)

-   Tik dit IP-adres in de URL van een browser (bijv: 192.168.0.123)

-   Login met admin en met password admin

-   Zoek het .bin bestand en druk update\
    (Bijvoorbeeld 'ESP32Arduino_WordClockV057.inoNL114.bin' Dit is versie V057 met instellingen voor de NL114 klok)

![](media/image23.png){width="2.0833333333333335in" height="4.473611111111111in"}**Bediening van de klok** \
\
Om verbinding te maken met een WIFI-netwerk moet een SSID (WIFI-naam) en wachtwoord worden ingevoerd.\
Er zijn een paar methoden:\
Sluit de MCU in de klok met een micro-USB- seriële kabel aan op een pc en gebruik een seriële terminal.\
Gebruik​ een BLE- seriële terminal-app op een telefoon of tablet voor verbinding.

Voor een PC is de Serial monitor in de Arduino IDE aan te raden.\
Voor IOS gebruik : **BLE Serial Pro** of **BLESerial nRF**.\
Voor Android gebruik: **Serial Bluetooth terminal**.\
\
Bluetooth Low Energy (BLE) kan twee soorten protocol gebruiken CC25nn of nRF52nn waarbij nn een specifiek nummer is. Deze klok gebruikt nRF52.\
- Start de app en start een verbinding met de klok.

![](media/image24.png){width="2.033333333333333in" height="4.384722222222222in"}Sommige apps starten automatisch met een verbindingsvenster, maar voor sommige moet een verbindingssymbool worden ingedrukt. Er is waarschijnlijk één station, de woordklok, om uit te selecteren.\
- Selecteer de klok in de lijst.\
De app toont een venster en een regel waar opdrachten kunnen worden ingevoerd en naar de klok kunnen worden verzonden.\
- Door de letter I of i voor informatie te verzenden, wordt het menu weergegeven, gevolgd door de werkelijke instellingen van verschillende voorkeuren.\
Met één i wordt het korte menu getoond. Met dubbel i (ii) verschijnt een langer menu met vele instelmogelijkheden van de klok.

![](media/image25.png){width="0.90625in" height="0.9013888888888889in"}In de klok zit een LED die een rode stip heeft die oplicht als het programma draait.\
Een groene stip gaat branden als er een WIFI-verbinding is. Als er een Bluetooth-verbinding is, gaat er een blauwe stip in de LED branden.

In beide gevallen **stuur de letter i van Informatie en het korte menu verschijnt** .\
**Het lange menu verschijnt met ii**

Voer de eerste letter in van de instelling die je wilt wijzigen, gevolgd door een code.\
Sommige items schakelen alleen Aan en Uit. Zoals de W om WIFI Uit of Aan te zetten.\
\
\
Om de SSID en het wachtwoord te wijzigen:\
Verstuur de letter **A** of **a** gevolgd door de naam van het wifi -station en **B** of **b** gevolgd door het wachtwoord.

+---------------------------------------------------------------------------------------------------------------------------------+--------------------------------------------------------------------------+
| **Amy-SSID** en verstuur deze opdracht. Bijvoorbeeld AFRITZ!Box01 of aFRITZ!Box01. Hoofdletter of kleine letter maakt niet uit. | \_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_ |
|                                                                                                                                 |                                                                          |
| Daarna volgt de letter B met het wachtwoord.\                                                                                   | F Own colour (Hex FWWRRGGBB)                                             |
| **Bmypassword** en verstuur het wachtwoord.                                                                                     |                                                                          |
|                                                                                                                                 | I Menu, II long menu                                                     |
| **Cbroadcastname** verandert in de naam die wordt weergegeven in de Bluetooth-verbindingslijst.\                                |                                                                          |
| Bijvoorbeeld iets als: cWoordklok\                                                                                              | N Display off between Nhhhh (N2208)                                      |
| \                                                                                                                               |                                                                          |
| Als de lengte van de SSID en/of het wachtwoord minder dan 5 tekens is, wordt de WIFI automatisch uitgeschakeld.                 | O Display On/Off                                                         |
|                                                                                                                                 |                                                                          |
| Gebruik een lengte van minimaal 8 tekens voor de SSID en het wachtwoord.\                                                       | Q Display colour choice (Q0)                                             |
| Controleer in het menu (derde regel van onderen) of WIFI en NTP aan staan.\                                                     |                                                                          |
| \                                                                                                                               | R Reset settings, @ Restart                                              |
| Voer @ in om de Nano ESP32 opnieuw te starten.                                                                                  |                                                                          |
|                                                                                                                                 | \_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_   |
| Deze wordt opnieuw opgestart en er wordt verbindingen gemaakt.\                                                                 |                                                                          |
| Soms moet de klok een tweede of derde keer worden gereset voordat hij verbinding kan maken met WIFI.\                           | Display off between: 22h - 08h                                           |
| Als​ verbinding​​​​​​​​​ nog steeds niet tot stand komt controleer het ingevoerde wachtwoord.                                             |                                                                          |
|                                                                                                                                 | Display choice: Rainbow                                                  |
| Als WIFI is verbonden, zal de LED op de Nano ESP32 groen knipperen.                                                             |                                                                          |
|                                                                                                                                 | RandomDisplay is OFF                                                     |
|                                                                                                                                 |                                                                          |
|                                                                                                                                 | Slope: 30 Min: 20 Max: 255                                               |
|                                                                                                                                 |                                                                          |
|                                                                                                                                 | BLE name: ESP32Test                                                      |
|                                                                                                                                 |                                                                          |
|                                                                                                                                 | IP-address: 192.168.178.172/update                                       |
|                                                                                                                                 |                                                                          |
|                                                                                                                                 | WIFI=On NTP=On BLE=On FastBLE=Off                                        |
|                                                                                                                                 |                                                                          |
|                                                                                                                                 | 15/10/2025 09:08:18                                                      |
|                                                                                                                                 |                                                                          |
|                                                                                                                                 | \_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_   |
|                                                                                                                                 |                                                                          |
|                                                                                                                                 | \_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_     |
|                                                                                                                                 |                                                                          |
|                                                                                                                                 | A SSID B Password C BLE beacon name                                      |
|                                                                                                                                 |                                                                          |
|                                                                                                                                 | D Date (D15012021) T Time (T132145)                                      |
|                                                                                                                                 |                                                                          |
|                                                                                                                                 | E Timezone (E\<-02\>2 or E\<+01\>-1)                                     |
|                                                                                                                                 |                                                                          |
|                                                                                                                                 | F Own colour (Hex FWWRRGGBB)                                             |
|                                                                                                                                 |                                                                          |
|                                                                                                                                 | G Scan WIFI networks                                                     |
|                                                                                                                                 |                                                                          |
|                                                                                                                                 | H H001 rotary, H002 membrane (H000)                                      |
|                                                                                                                                 |                                                                          |
|                                                                                                                                 | I Info menu, II long menu                                                |
|                                                                                                                                 |                                                                          |
|                                                                                                                                 | J DS3231 RTC module On/Off                                               |
|                                                                                                                                 |                                                                          |
|                                                                                                                                 | K LDR reads/sec On/Off                                                   |
|                                                                                                                                 |                                                                          |
|                                                                                                                                 | N Display off between Nhhhh (N2208)                                      |
|                                                                                                                                 |                                                                          |
|                                                                                                                                 | O Display On/Off                                                         |
|                                                                                                                                 |                                                                          |
|                                                                                                                                 | P Status LED On/Off                                                      |
|                                                                                                                                 |                                                                          |
|                                                                                                                                 | Q Display choice (Q0) \~ Changing                                        |
|                                                                                                                                 |                                                                          |
|                                                                                                                                 | R Reset settings, @ Restart                                              |
|                                                                                                                                 |                                                                          |
|                                                                                                                                 | U Demo (msec) (U200) Y LED test                                          |
|                                                                                                                                 |                                                                          |
|                                                                                                                                 | S Slope, L Min, M Max (S50 L5 M200)                                      |
|                                                                                                                                 |                                                                          |
|                                                                                                                                 | W WIFI X NTP& Z WPS CCC BLE + Fast BLE                                   |
|                                                                                                                                 |                                                                          |
|                                                                                                                                 | #nnn Selftest, RTC: ! See, & Update                                      |
|                                                                                                                                 |                                                                          |
|                                                                                                                                 | ) HETISWAS On/Off, ( EdSoft On/Off                                       |
|                                                                                                                                 |                                                                          |
|                                                                                                                                 | Ed Nieuwenhuys Oct 2025                                                  |
|                                                                                                                                 |                                                                          |
|                                                                                                                                 | \_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_   |
|                                                                                                                                 |                                                                          |
|                                                                                                                                 | Display off between: 22h - 08h                                           |
|                                                                                                                                 |                                                                          |
|                                                                                                                                 | Display choice: Rainbow                                                  |
|                                                                                                                                 |                                                                          |
|                                                                                                                                 | RandomDisplay is OFF                                                     |
|                                                                                                                                 |                                                                          |
|                                                                                                                                 | Slope: 30 Min: 20 Max: 255                                               |
|                                                                                                                                 |                                                                          |
|                                                                                                                                 | SSID: FRITZ!BoxEd                                                        |
|                                                                                                                                 |                                                                          |
|                                                                                                                                 | BLE name: ESP32Test                                                      |
|                                                                                                                                 |                                                                          |
|                                                                                                                                 | IP-address: 192.168.178.172/update                                       |
|                                                                                                                                 |                                                                          |
|                                                                                                                                 | Timezone:CET-1CEST,M3.5.0,M10.5.0/3                                      |
|                                                                                                                                 |                                                                          |
|                                                                                                                                 | WIFI=On NTP=On BLE=On FastBLE=Off                                        |
|                                                                                                                                 |                                                                          |
|                                                                                                                                 | Rotary=Off Membrane=Off DS3231=Off                                       |
|                                                                                                                                 |                                                                          |
|                                                                                                                                 | WS2812 strip with 148 LEDs (switch %)                                    |
|                                                                                                                                 |                                                                          |
|                                                                                                                                 | Software: ESP32_WordClockV116.ino                                        |
+=================================================================================================================================+==========================================================================+
|                                                                                                                                 | Korte (i) en lange (ii) menuweergave                                     |
+---------------------------------------------------------------------------------------------------------------------------------+--------------------------------------------------------------------------+

Standaard staat de tijdzone ingesteld op Amsterdamse tijd.\
Een reset met optie R in het menu zal de tijdzone weer terugzetten naar Amsterdam.\
Om een andere tijdzone in te stellen, stuurt je de tijdzone-string, voorafgegaan door het teken E of e.\
Onderaan deze handleiding zijn veel tijdzones afgedrukt.\
Bijvoorbeeld: als u in Australië/Sydney woont, stuurt u de string,

eAEST-10AEDT,M10.1.0,M4.1.0/3.

De klok gebruikt de zomertijd (DST) wanneer deze is verbonden met een NTP-server, maar niet wanneer de DS3231-tijdmodule wordt gebruikt

  ---------------------------------------------------------------------------------------------------------------------------------------------------------------------
  ![](media/image26.png){width="2.832638888888889in" height="6.551388888888889in"}   ![](media/image27.png){width="2.294594269466317in" height="5.171874453193351in"}
  ---------------------------------------------------------------------------------- ----------------------------------------------------------------------------------
  HTML-pagina                                                                        "Serial monitor"-scherm vanaf een pc

  ---------------------------------------------------------------------------------------------------------------------------------------------------------------------

**\
**

**Afstandsbediening gebruiken**

De klok kan zijn uitgevoerd met een infrarood (IR) ontvanger. Met een geschikte afstandsbediening kan daarmee de tijd van de klok ingesteld worden en een van de vele kleurschema's die de klok kan tonen geselecteerd worden .

-   Zet in het menu met de PC of BLE app de IR-afstandsbediening aan met H03 of H04 voor een kleine IR-afstandsbediening. Deze heeft minder codes\
    MIN-1, MIN+1, UUR-1, UUR+1, POWER, ONOFF

-   Herstart de klok met @.

-   Zend **{** om de klok de toetscodes van de afstandsbediening te leren.

Niet alle afstandsbedieningen zullen werken.

-   De software zal vragen om de volgende toetsen te drukken.

\"0\", \"1\", \"2\", \"3\", \"4\", \"5\", \"6\", \"7\", \"8\", \"9\", \"UP\", \"DOWN\", \"LEFT\", \"RIGHT\", \"POWER\", \"OK\",\"ONOFF\"  

Zit een bepaalde toets niet op je toetsenbord kies er dan een die er op lijkt.\
Kies bijvoorbeeld de knop Source als ONOFF wordt gevraagd.

Gebruikt niet dezelfde toets voor verschillende codes.

=== Starting Learning Mode ===

Please press button: 0

Learning IR started

Het is vijf over half een LDR:110= 3% 173860 l/s 12:36:40

=== Starting Learning Mode ===

Please press button: 0

Learning IR started

Het is vijf over half een LDR:110= 3% 173860 l/s 12:36:40

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

=== Learning Complete & Saved! ===

Stored Button Mappings:

\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_

IR-Remote Address: 0xBD00

IR-Remote Protocol: NEC

\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_

0 -\> Cmd: 0x0041

5 -\> Cmd: 0x001A

6 -\> Cmd: 0x001B

7 -\> Cmd: 0x001D

8 -\> Cmd: 0x001E

9 -\> Cmd: 0x001F

UP -\> Cmd: 0x000B

DOWN -\> Cmd: 0x000F

LEFT -\> Cmd: 0x0049

RIGHT -\> Cmd: 0x004A

POWER -\> Cmd: 0x0001

OK -\> Cmd: 0x000D

ONOFF -\> Cmd: 0x0008

\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_

=== Now in Recognition Mode ===

Only responding to remote with Address: 0xBD00

Press any learned button to test\....

Press POWER to process the key presses to a time. POWER ON

Press POWER again to stop time entry. POWER OFF\
With POWER OFF the digits will change the Display choice

-   Druk POWERON om de tijd in te stellen.

-   Toets 6 nummers in als uummss.\
    Bijvoorbeeld Kwart voor vier in de middag: 154500

-   of gebruik UP en DOWN om een UUR hoger of lager te zetten en LEFT en RIGHT om de minuten lager of hoger te zetten.

-   Druk weer op POWER om de tijdinvoer te stoppen.\
    Na 5 minuten gaat de POWER automatisch uit.

-   Met de grote afstandsbediening kunnen de cijfertoetsen gebruikt worden om kleurscherma's van de klok (optie Q in het menu) te veranderen.\
    Met de grote en kleine afstandsbediening kan met ONOFF de display worden aan- of uitgezet

**Instellingen van de klok**

![](media/image28.jpeg){width="3.7152777777777777in" height="3.625in"}Instellingen van de klok kunnen bediend worden met de WIFI webpagina of Bluetooth BLE terminal app.\
Wanneer de klok is verbonden met WIFI heeft deze een IP -adres ontvangen van de router waarmee deze is verbonden.\
Het IP-adres staat afgedrukt in het menu.

Om het menu als een webpagina te starten moet het IP-adres van de klok in een webbrowser worden ingevoerd (bijvoorbeeld: 192.168.178.77)

of gebruik in plaats daarvan de BLE-stationsnaam gevolgd door .local als URL in de browser. (Standaard na een reset: wordclock.local)

Of gebruik een Bluetooth-verbinding:\
- Open de BLE-terminal-app.\
- Zoek in de app naar de klok om verbinding mee te maken en maak verbinding.\
\
Elke app heeft zijn eigen manier om het Bluetooth-apparaat weer te geven waarmee verbinding moet worden gemaakt.\
\
De iPhone / iPad / iMac BLE-seriële apps zijn door mij gemaakt en werken met hun standaardinstellingen.\
Bij andere apps moeten de instellingen voor het verzenden en ontvangen van gegevens mogelijk worden gewijzigd. Speel met de lettergrootte en de CR- en LF-instelling totdat u\
op een iPhone, iPad of iMac met de BLE-seriële app de transmissiesnelheid kunt versnellen door de optie \'+ Fast BLE\' te selecteren in het menu

Helaas kunnen sommige apps geen strings lezen die langer zijn dan 20 tekens en ziet u de strings afgekapt of vervormd .\
Als er regels onvolledig worden geprint stuur dan \'+\' in om een langzamere transmissiemodus te selecteren.\
Als de transmissie te vervormd is en het onmogelijk is om het teken + te verzenden, probeer dan de webpagina van de klok en verstuur een + teken.

Als alles mislukt, moet u de Arduino Nano ESP32 in de klok met een USB-C-kabel aansluiten op een pc en met de Arduino IDE een + te verzenden of stuur de letter R om de klok naar de standaardinstellingen te resetten.

**\
**

**De lichtintensiteit van het display aanpassen**

![](media/image29.png){width="1.2395833333333333in" height="1.1694444444444445in"}

In het menu kan de lichtintensiteit van het display met drie parameters worden geregeld:

\--Instellingen voor lichtintensiteit (1-250)\--

S=Helling L=Min M=Max (S80 L5 M200)

Standaardwaarden worden tussen () weergegeven.

**S** Regelt hoe snel de lichtsterkte van de LEDs de maximale helderheid bereikt.

**L** Hoe helder het scherm blijft in volledige duisternis.

**M** de maximale helderheid van het scherm .

In de onderste helft van het menu worden de opgeslagen waarden weergegeven

Helling: 80 Min: 5 Max: 255

De klok reageert op licht met zijn LDR (lichtafhankelijke weerstand).

Wanneer het donker wordt schakelt de display niet helemaal uit maar blijft gedimd op een minimale waarde.\
Met de parameter **L** kan de minimale helderheid worden geregeld. Met een waarde tussen 0 en 255 kan deze helderheid worden ingesteld.\
L5 is de standaardwaarde.

De maximale helderheid van het display wordt geregeld met de parameter **M**. Ook een waarde tussen 0 en 255. Standaard staat deze maximaal op 255.\
Met parameter **S** kan de helling worden geregeld hoe snel de maximale helderheid wordt bereikt.

![](media/image30.png){width="4.128472222222222in" height="3.192361111111111in"}Ook deze waarden variëren tussen de 0 en 255. Standaard staat deze op 80.

**Instellingen worden ingesteld door het eerste teken van een opdracht in te voeren, gevolgd door parameters indien nodig.**

Bijvoorbeeld:\
Om de kleur van de woorden in de klok op wit in te stellen, voer in: Q2\
\
Om de klok tussen 22 uur en 8 uur uit te schakelen stuur: n2208\
\
Schakel WIFI uit door een W te sturen.

Start de klok opnieuw door de letter @ te verzenden.

Reset naar standaardinstelling door R te sturen.

  -------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  ![](media/image31.jpeg){width="2.8541666666666665in" height="5.850694444444445in"}   ![](media/image32.jpeg){width="2.8645833333333335in" height="5.561111111111111in"}
  ------------------------------------------------------------------------------------ ------------------------------------------------------------------------------------
  HTML-pagina                                                                          BLE-menu

  -------------------------------------------------------------------------------------------------------------------------------------------------------------------------

**De software updaten**

De software kan \'Over The Air\' worden bijgewerkt wanneer de klok is verbonden met WIFI. Het IP-adres staat in het menu.

Voer het IP-adres van de klok in, direct, zonder spatie, gevolgd door /update.

![](media/image33.jpeg){width="2.1145833333333335in" height="2.790277777777778in"}Zoiets als dit: 192.168.178.141/update.

Of gebruik: http://BLEbroadcastname.local/update

in plaats van het IP-adres.

In dit geval: redpcbv01.local/update.\
(Soms kan http:// weggelaten worden)

![](media/image34.png){width="3.575in" height="2.017361111111111in"}

\'Kies bestand\' in het menu en selecteer het bin-bestand dat u wilt bijwerken.

Iets als: ESP32Arduino_WordClockV100.ino.bin

waarbij V100 het versienummer van de software is.

![](media/image35.png){width="6.268055555555556in" height="2.09375in"}![](media/image36.png){width="6.268055555555556in" height="2.373611111111111in"}

**Gedetailleerde menubeschrijving**\
\
Met het menu kunnen veel voorkeuren worden ingesteld. Deze voorkeuren worden opgeslagen in de ESP32-opslagruimte en blijven bewaard bij stroomuitval.\
 \
Voer het eerste teken in het menu van het item in dat moet worden gewijzigd, gevolgd door de parameter.\
Bijvoorbeeld N2208 om de klokverlichting tussen 22:00 en 8:00 uit te zetten.\
Er is geen verschil tussen hoofdletters of kleine letters. Beide zijn OK.

Als de klok gestart wordt en verbonden is met een USC-C seriële kabel aan een PC dan wordt in de 'serial terminal' van de Arduino IDE de opstart loggegevens geprint.

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

\[WiFi-event\] event: 0 : WiFi interface ready

\[WiFi-event\] event: 2 : WiFi client started

\[WiFi-event\] event: 4 : Connected to access point

\[WiFi-event\] event: 7 : Obtained IP address: 192.168.178.199

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

I Info menu, II long menu

J Toggle use DS3231 RTC module

K LDR reads/sec toggle On/Off

N Display off between Nhhhh (N2208)

O Display toggle On/Off

P Status LED toggle On/Off

Q Display colour choice (Q0)

R Reset settings, @ = Reset MCU

U Demo (msec) (U200) Y LED test

\--Light intensity settings (1-250)\--

S Slope, L Min, M Max (S50 L5 M200)

W WIFI X NTP& Z WPS CCC BLE + Fast BLE

#nnn Selftest, RTC: ! See, & Update

Ed Nieuwenhuys April 2025

\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_

**A SSID B Wachtwoord C BLE beacon naam**\
Voer de naam van de SSID en wachtwoord van de router in waarmee de klok verbonden moet worden.\
De Bluetooth broadcast-naam die weergegeven wordt in de Bluetooth seriële terminal app kan gewijzigd worden.\
Bijvoorbeeld: **a**FRITZ!Box of **A**FRITZ!Box .\
Voer vervolgens het wachtwoord in. Bijvoorbeeld: **B**wachtwoord.

Voer eventueel **c**BluetoothNaam in (standaard na een reset: wordclock )\
Start de klok opnieuw door @ te verzenden .

**CCC** Door CCC of ccc in te voeren, schakelt u BLE in of uit.

Wees voorzichtig bij het uitschakelen. Als BLE uit staat, kan de klok alleen worden bediend met WIFI of de USB-seriële poort.

**D Datum instellen** en **T Tijd instellen**\
Als er geen verbinding is met WIFI-tijd en er een RTC DS3231 is aangesloten, moet de datum handmatig worden ingesteld.\
Voer bijvoorbeeld in: D06112022 om de datum in te stellen op 6 november 2022.

Voer bijvoorbeeld T132145 (of 132145, of t132145) in om de tijd in te stellen op 45 seconden en 21 minuten na één uur.\
Datum en tijd wijzigen werkt alleen als WIFI en NTP uitgeschakeld zijn.

**E Stel tijdzone E\<-02\>2 of E\<+01\>-1**\
Onderaan deze pagina staan de tijdzones die in 2022 worden gebruikt.\
Het is een vrij ingewikkelde string en het is daarom verstandig om deze te kopiëren.\
Laten we er een kiezen als u hier woont: Antarctica/Troll,\"\<+00\>0\<+02\>-2,M3.5.0/1,M10.5.0/3\"\
Kopieer de string tussen de \" \"\'s en verstuur deze beginnend met een \'E\' of \'e\' ervoor.\
E\<+00\>0\<+02\>-2,M3.5.0/1,M10.5.0/3

**\
**

**F Eigen kleur (Hex WWRRGGBB of Fwrgb)**

(Tik in een browser 0xff00ff in om op een dan te kiezen website kleuren hexadecimaal te maken)

*Of als je niet met hexadecimaal kan omgaan tik na de F een decimaal getal tussen 0 en 9 voor respectievelijk Wit, Rood, Groen en Blauw.\
0 is uit en 9 is 100% aan. F0900 is rood en F0009 is blauw en oranjerood F0920*

*Met de hexadecimale kleuren is er keuze uit 255 gradaties ipv de 10 nu.*

De kleur van de woorden die in de klok worden weergegeven kunnen aangepast worden en zijn te selecteren met optie Q3 of Q4 in het menu.

~~\
~~Het in te voeren formaat is hexadecimaal. 0123456789ABCDEF zijn de tekens die kunnen worden gebruikt.

Het commando bestaat uit 2 cijfers voor wit gevolgd 2 cijfers voor Rood gevolgd door 2 voor Groen en eindigend met 2 cijfers voor Blauw.

Om de letters intens Groen te kleuren, voer **F**0000FF00 in, voorafgegaan door de letter F. Om de letters intens Rood te kleuren, voer FF0000 in, voorafgegaan door het karakter F.

**F**000000FF

Om intens blauw in te stellen, voer je F000000FF in.

Bijvoorbeeld: F8800FF00 is 50% wit en 100% groen.

In het kort: 00 is LED uit, 44 is 25%, 88 is 50%, BB is 75% en FF is 100%.

![](media/image37.png){width="2.078472222222222in" height="0.8854166666666666in"}

WS2812 LED-strips hebben geen witte LED en het commando om de wit aan te sturen geen effect.

Bij SK6812 LEDs heeft een extra witte LED (geel vlakje) naast de drie RGB LEDs.

Welke LEDs in de klok zijn gemonteerd staat onderin het lange menu (ii)\
WS2812 strip with 148 LEDs (switch %)

**G Scannen naar WIFI netwerken in de buurt**\
Door \'G\' te versturen worden de beschikbare netwerken in de buurt in de Bluetooth BLE-terminal afgedrukt.

**H H01 rotary H02 buttons H03/04 remote**\
Door \'Hnn\' te verzenden wordt gekozen of een optionele draaiknop (**H01**), een (membraan) drukknop (**H02**) of een afstandsbediening wordt gebruikt om de tijd en datum in te kunnen stellen.\
De interne, dan verplichte, DS3231 RTC wordt dan automatisch aangezet en de NTP tijd uit.

**H03** is een grote afstandsbediening met cijfers en **H04** is voor een kleine uitvoering zonder cijfers.

De software controleert dan of er een invoer plaatsvindt. Zet daarom de optie met **H00** uit als de invoer niet gebruikt wordt om onbedoelde invoer te voorkomen.

Na een Reset naar fabrieksinstelling wordt het gebruik van deze instelling ongewijzigd gelaten.

**} Learn IR remote**

Om een afstandsbediening te kunnen gebruiken moet de klok eerst leren wat de infrarood-code is die een knop verzendt.\
*Niet alle afstandsbedieningen zullen te programmeren zijn.*

Bij optie **H03**, de grote afstandsbediening, worden de volgende toetsen geprogrammeerd:

"0\", \"1\", \"2\", \"3\", \"4\", \"5\", \"6\", \"7\", \"8\", \"9, \"UP\", \"DOWN\", \"LEFT\", \"RIGHT\", \"POWER\", \"OK\",\"ONOFF\".

Als de POWER knop niet gedrukt wordt dan selecteren de toetsen; "0\", \"1\", \"2\", \"3\", \"4\", \"5\", \"6\", \"7\", \"8\", \"9 de display modes. De Q codes in het menu. Q2 is bijvoorbeeld wit.

Met de \"ONOFF\" toets gaat de verlichting van de klok uit of aan

Als de POWER knop wordt gedrukt dan kan met de cijfertoetsen vijf minuten lang de tijd worden ingevoerd. Na vijf minuten gaat de POWER vanzelf uit om te voorkomen dat ongewenst de tijd veranderd wordt.

Voer zes getallen in + \"OK\".

Druk 153612 + \"OK\" voor 15 uur, 36 minuten en 12 seconden

Of eenvoudiger:

Met \"UP\", \"DOWN\" worden de uren verlaagd of verhoogd en

Met \"LEFT\", \"RIGHT\" worden minuten verlaagd of verhoogd.

Bij een kleine afstandsbediening (**H04**) werkt als volgt :

Met de \"ONOFF\" toets gaat de verlichting van de klok uit of aan

Druk \"POWER\" en daarna de toetsen MIN-1\", \"MIN+1\", \"UUR-1\", \"UUR+1" om de tijd aan te passen.\
\
**I of ii om het infomenu te tonen,**\
Print het menu.

Met de optie I (i) in het menu wordt gekozen voor een kort menu voor alledaags gebruik of voor een lang menu met optie II (2x i) voor een uitgebreid menu met alle opties.

**J Schakelen om de tijd uit de DS3231-module te gebruiken**\
Door \'J\' te verzenden, wordt het gebruik van de optionele DS3231-tijdmodule AAN of UIT gezet.\
Als de tijd van de DS3231 RTC-module wordt gebruikt zal de gele LED bij de USB-stekker op de Nano ESP32 niet meer knipperen.\
Als de klok geen internetverbinding heeft, zal de tijd snel verlopen.\
Door gebruik van de DS3231-tijdmodule wordt de afwijking teruggebracht tot een paar seconden per jaar.

De tijd kan in de DS3231-tijdmodule worden ingevoerd met optie T (time) en D (date) in het menu.

Mocht de tijd niet zichtbaar worden stuur dan het commando ! om de interne kloktijden te zien.

**K Reads/sec aan/uit zetten**

Door een K in te voeren kan het afdrukken van de LDR-uitlezing van de gemeten lichtintensiteit per seconde gevolgd worden\
Het toont ook hoe vaak de processor per seconde (l/s) door het programma loopt en zijn taken controleert om de klok te laten lopen.

TestLDR: On

LDR reading, %Out, loops per second and time

LDR: 1= 1% 205413 l/s 16:06:08

LDR: 1= 1% 215535 l/s 16:06:09

LDR: 1= 1% 215451 l/s 16:06:10

LDR: 1= 1% 215350 l/s 16:06:11

TestLDR: Off

**K0, K1, K2 Logging uit, per min, per uur**

De tijd wordt standaard elke minuut in de logs geprint. Met de webpagina kunnen duizenden logregels getoond worden. Met K1 kan de log per minuut, en met K2 per uur, worden aan of uitgezet gezet.\
In het lange menu (ii) staat dan Timestamp/min, Timestamp/hour of Timestamp off.

**N Display uit tussen Nhhhh (N2208)**\
Met N2208 wordt het display uitgeschakeld tussen 22:00 en 8:00.\
\
**O Display aan/uit schakelen**\
O Schakelt het display uit en aan.

**P Status-LED's uit- en aanschakelen**\
P Schakelt de status-LED\'s op de Arduino Nano uit en aan.

**Q Keuze van de weergavekleur (Q0-9)\
**Q0 geeft de tijd weer met gele woorden

en zal HET (of IT) binnen een uur van groen via geel naar rood veranderen.

en zal IS of WAS van groen naar rood via geel veranderen in een minuut.\
Q1 zal elk uur een andere kleur tonen.\
Q2 toont alle teksten wit.\
Q3 gebruikt eigen gedefinieerde kleuren.

(Met optie F in het menu zijn de kleuren in te stellen)

Q4 gebruikt eigen gedefinieerde kleuren. (HET IS/WAS verkleurt van groen naar rood)

Q5 zal in een uur de regenboogkleuren doorlopen.\
Q6 is een digitale display

Q7 zal elk uur een andere kleur tonen. (HET IS/WAS verkleurt van groen naar rood)

Q8 zal in 4 minuten de regenboogkleuren doorlopen.

Q9 zal in 15 seconden de regenboogkleuren doorlopen.

**R Instellingen resetten**\
R zet alle voorkeuren terug naar de standaardinstellingen.

De SSID, het wachtwoord, de BLE-naam en de tijdzone worden niet gewist.\
RRR wist deze laatste vier wel. WIFI, NTP en BLE worden aangezet

RRRRR is de combinatie van de optie R en RRR.\
Het type LED-strip en het gebruik van een rotary of membraanknop worden ongewijzigd gelaten.

Voer een reset met RRRRR uit , een totale reset, wanneer de sketch voor de eerste keer wordt geüpload naar de Arduino Nano ESP32.

**S=Slope L=Min M=Max (S50 L5 M200)**\
S Hoe snel de helderheid de maximale helderheid bereikt.\
L Hoe helder het display is in volledige duisternis.\
M de maximale helderheid van het display.\
Waarden tussen 0 en 250\'

**U Demo-modus (msec) (U200)**

Voer U in, gevolgd door de duur van een seconde in milliseconden.

U200 (200 milliseconde) versnelt de klok 5 keer.

Met alleen versturen van een U wordt de demomodus uitgeschakeld.

**W=WIFI, X=NTP & , CCC=BLE**\
Schakel WIFI, NTP in en uit.\
Voer het teken in om de optie in of uit te schakelen.\
Onderaan het menu staat het aangegeven.

Door een & te verzenden wordt de NTP-tijd in de interne Nano ESP32 RTC- en DS3231-tijdmodule gezet.

**Y LED-test**

Alle LEDs gaan een regenboog doorlopen.

**Z WPS**

WPS is een methode om het wachtwoord en de naam van het WIFI-station, de SSID, uit de router te krijgen.\
Als er een WPS-knop op de WIFI-router aanwezig is kan de klok zonder de naam en het wachtwoord van de router in te hoeven vullen verbonden worden met het internet.

Start WPS in de klok door Z in het menu in te voeren en start WPS op de WIFI-router. Als alles goed gaat herstart de klok en zal de tijd gelijk staan.

Soms moet je de WPS-knop op de router een paar seconden ingedrukt houden.

Hoe je WPS moet starten verschilt per router.

**\
**

**+ = Fast BLE**\
Het BLE UART-protocol verzendt standaardpakketten van 20 bytes lang. Tussen elk pakket zit een vertraging van 50 msec .\
De IOS BLEserial- app, en misschien ook andere, kan pakketten van 80 bytes of meer ontvangen voordat tekens worden gemist . Hierdoor wordt het menu sneller afgedrukt.\
Optie + schakelt tussen de lange en korte pakketten.

**\~ = Verander display mode per minuut of uur**

\~ start de optie voor het wijzigen van de weergavemodus en wisselt per minuut.\
\~1 verandert de display mode elke minuut

\~2 wisselt de display mode elk uur.

\~0 of \~ schakelt de wisseloptie uit.

In het lange menu (ii) staat dan RandomDisplay: On/min, On/hour of Off

**! = Toon NTP, RTC en DS3231 tijd\
!** zal de NTP, RTC en DS3231 tijd weergeven zoals ze zijn opgeslagen in de klok in de klok . De DS3231-tijdmodule moet geïnstalleerd zijn om een realistische tijd voor de module te tonen.\
Hetzelfde als de & optie maar deze optie zal niet worden bijgewerkt vanaf de internet NTP server maar toont alleen de tijd.

**#nnn = Selftest\
**Door een \# te sturen start de klokzelftest. Dit is handig om te controleren of alle woorden in de klok functioneren.

Vanaf V062 is #nnnn is ook mogelijk. Nnnn zijn de milliseconden vertraging tussen elk woord.

De zelftest stopt vanzelf na een cyclus.

**% = Wisselen tussen SK6812 en WS2812 LED strip\
**Met deze optie kan de gebruikte LED strip worden gewisseld. De klok is uitgerust met een van deze twee typen LED strips.

Een reset van alle instellingen door een R in het menu te sturen, verandert de selectie van de LED-strip niet.

**\$ = Haardvuur\
**Toont een haardvuur op en 144 LED-klok**.** Op andere type klokken zal het resultaat minder mooi zijn.**\
**

**( = Schakelt extra optionele LEDs Aan/Uit**

De klok kan optioneel uitgerust zijn met een extra logo tekst.

**) = Schakelt HET/IS/WAS Aan/Uit**

HET IS WAS blijven altijd uit

**\
**

**= = Toont de permanente instelling**\
Met de optie = worden alle instellingen geprint

**@ = Herstart de klok\
**@ start de klok opnieuw op. Dit is handig als de SSID, et cetera worden gewijzigd en het programma opnieuw moet worden opgestart.\
Instellingen blijven behouden.

**& = Update met NTP\
**Synchroniseert de RTC-module met de NTP tijd **\
**De RTC-tijdmodule in de klok kan de tijd in de klok aansturen. Met de menu-opties T en D kan de tijd en datum handmatig worden ingevoerd.\
Met deze optie wordt de NTP-tijd naar de RTC-module gekopieerd.

De computer die in de klok zit heeft een eigen interne klok. Deze verloopt seconden per dag en wordt regelmatig met of de NTP-tijd om de DS3231 RTC-tijdmodule bijgewerkt. Als NTP en de DS3231 RTC-tijdmodule zijn uitgeschakeld zal de klok wel een tijd weergeven maar deze zal na een paar dagen uit de pas met de juiste tijd gaan lopen.

**123456 Tijd instellen in DS3231 RTC-tijdmodule**

Voer de tijd in als 152300 hhmmss. Hetzelfde als T152300.\
Datum en tijd wijzigen werkt alleen als NTP uitgeschakeld is.

**\
**

**Tijdzones**

Kopieer de tekst **tussen de aanhalingstekens** en plak deze na het teken E (of e) en stuur deze naar de klok.

Voor Nederland en België zend: eCET-1CEST,M3.5.0,M10.5.0/3

+---------------------------------------------------------+-------------------------------------------------------------+-----------------------------------------------------------------------------------------+------------------------------------------------------------------+----------------------------------------------------------------------+
| Africa/Abidjan,\'GMT0\'                                 | America/Detroit,\"EST5EDT,M3.2.0,M11.1.0\"                  | America/Tegucigalpa, \"CST6\"                                                           | Asia/Tokyo,\"JST-9\"                                             | Indian/Antananarivo, \"EAT-3\"                                       |
|                                                         |                                                             |                                                                                         |                                                                  |                                                                      |
| Africa/Accra,\'GMT0\'                                   | America/Dominica, \"AST4\"                                  | America/Thule,\"AST4ADT,M3.2.0,M11.1.0\" America/Thunder_Bay,\"EST5EDT,M3.2.0,M11.1.0\" | Asia/Tomsk,\"\<+07\>-7\"                                         | Indian/Chagos,\"\<+06\>-6\"                                          |
|                                                         |                                                             |                                                                                         |                                                                  |                                                                      |
| Africa/Addis_Ababa,\'EAT-3\'                            | America/Edmonton,\"MST7MDT,M3.2.0,M11.1.0\"                 | America/Tijuana,\"PST8PDT,M3.2.0,M11.1.0\"                                              | Asia/Ulaanbaatar,\"\<+08\>-8\"                                   | Indian/Christmas,\"\<+07\>-7\"                                       |
|                                                         |                                                             |                                                                                         |                                                                  |                                                                      |
| Africa/Algiers,\'CET-1\'                                | America/Eirunepe,\"\<-05\>5\"                               | America/Toronto,\"EST5EDT,M3.2.0,M11.1.0\"                                              | Asia/Urumqi,\"\<+06\>-6\"                                        | Indian/Cocos,\"\<+0630\>-6:30\"                                      |
|                                                         |                                                             |                                                                                         |                                                                  |                                                                      |
| Africa/Asmara, \"EAT-3\"                                | America/El_Salvador,\"CST6\"                                | America/Tortola, \"AST4\"                                                               | Asia/Ust-Nera,\"\<+10\>-10\"                                     | Indian/Comoro, \"EAT-3\"                                             |
|                                                         |                                                             |                                                                                         |                                                                  |                                                                      |
| Africa/Bamako,\'GMT0\'                                  | America/Fortaleza,\"\<-03\>3\"                              | America/Vancouver,\"PST8PDT,M3.2.0,M11.1.0\"                                            | Asia/Vientiane,\"\<+07\>-7\"                                     | Indian/Kerguelen,\"\<+05\>-5\"                                       |
|                                                         |                                                             |                                                                                         |                                                                  |                                                                      |
| Africa/Bangui,\"WAT-1\"                                 | America/Fort_Nelson,\"MST7\"                                | America/Whitehorse,\"MST7\"                                                             | Asia/Vladivostok,\"\<+10\>-10\"                                  | Indian/Mahe,\"\<+04\>-4\"                                            |
|                                                         |                                                             |                                                                                         |                                                                  |                                                                      |
| Africa/Banjul,\'GMT0\'                                  | America/Glace_Bay,\"AST4ADT,M3.2.0,M11.1.0\"                | America/Winnipeg,\"CST6CDT,M3.2.0,M11.1.0\"                                             | Asia/Yakutsk,\"\<+09\>-9\"                                       | Indian/Maldives,\"\<+05\>-5\"                                        |
|                                                         |                                                             |                                                                                         |                                                                  |                                                                      |
| Africa/Bissau,\'GMT0\'                                  | America/Godthab,\"\<-03\>3\<-02\>,M3.5.0/-2,M10.5.0/-1\"    | America/Yakutat,\"AKST9AKDT,M3.2.0,M11.1.0\"                                            | Asia/Yangon,\"\<+0630\>-6:30\"                                   | Indian/Mauritius,\"\<+04\>-4\"                                       |
|                                                         |                                                             |                                                                                         |                                                                  |                                                                      |
| Africa/Blantyre,\"CAT-2\"                               | America/Goose_Bay,\"AST4ADT,M3.2.0,M11.1.0\"                | America/Yellowknife,\"MST7MDT,M3.2.0,M11.1.0\"                                          | Asia/Yekaterinburg,\"\<+05\>-5\"                                 | Indian/Mayotte,\"EAT-3\"                                             |
|                                                         |                                                             |                                                                                         |                                                                  |                                                                      |
| Africa/Brazzaville,\"WAT-1\"                            | America/Grand_Turk,\"EST5EDT,M3.2.0,M11.1.0\"               | Antarctica/Casey,\"\<+11\>-11\"                                                         | Asia/Yerevan,\"\<+04\>-4\"                                       | Indian/Reunion,\"\<+04\>-4\"                                         |
|                                                         |                                                             |                                                                                         |                                                                  |                                                                      |
| Africa/Bujumbura,\'CAT-2\'                              | America/Grenada, \"AST4\"                                   | Antarctica/Davis,\"\<+07\>-7\"                                                          | Atlantic/Azores,\"\<-01\>1\<+00\>,M3.5.0/0,M10.5.0/1\"           | Pacific/Apia,\"\<+13\>-13\"                                          |
|                                                         |                                                             |                                                                                         |                                                                  |                                                                      |
| Africa/Cairo, \"EET-2\"                                 | America/Guadeloupe, \"AST4\"                                | Antarctica/DumontDUrville,\"\<+10\>-10\"                                                | Atlantic/Bermuda,\"AST4ADT,M3.2.0,M11.1.0\"                      | Pacific/Auckland,\"NZST-12NZDT,M9.5.0,M4.1.0/3\"                     |
|                                                         |                                                             |                                                                                         |                                                                  |                                                                      |
| Africa/Casablanca,\"\<+01\>-1\"                         | America/Guatemala, \"CST6\"                                 | Antarctica/Macquarie,\"AEST-10AEDT,M10.1.0,M4.1.0/3\"                                   | Atlantic/Canary,\"WET0WEST,M3.5.0/1,M10.5.0\"                    | Pacific/Bougainvillea,\"\<+11\>-11\"                                 |
|                                                         |                                                             |                                                                                         |                                                                  |                                                                      |
| Africa/Ceuta,\"CET-1CEST,M3.5.0,M10.5.0/3\"             | America/Guayaquil,\"\<-05\>5\"                              | Antarctica/Mawson,\"\<+05\>-5\"                                                         | Atlantic/Cape_Verde,\"\<-01\>1\"                                 | Pacific/Chatham,\"\<+1245\>-12:45\<+1345\>,M9.5.0/2:45,M4.1.0/3:45\" |
|                                                         |                                                             |                                                                                         |                                                                  |                                                                      |
| Africa/Conakry,\"GMT0\"                                 | America/Guyana,\"\<-04\>4\"                                 | Antarctica/McMurdo,\"NZST-12NZDT,M9.5.0,M4.1.0/3\"                                      | Atlantic/Faroe,\"WET0WEST,M3.5.0/1,M10.5.0\"                     | Pacific/Chuuk,\"\<+10\>-10\"                                         |
|                                                         |                                                             |                                                                                         |                                                                  |                                                                      |
| Africa/Dakar,\'GMT0\'                                   | America/Halifax,\"AST4ADT,M3.2.0,M11.1.0\"                  | Antarctica/Palmer,\"\<-03\>3\"                                                          | Atlantic/Madeira,\"WET0WEST,M3.5.0/1,M10.5.0\"                   | Pacific/Easter,\"\<-06\>6\<-05\>,M9.1.6/22,M4.1.6/22\"               |
|                                                         |                                                             |                                                                                         |                                                                  |                                                                      |
| Africa/Dar_es_Salaam,\"EAT-3\"                          | America/Havana,\"CST5CDT,M3.2.0/0,M11.1.0/1\"               | Antarctica/Rothera,\"\<-03\>3\"                                                         | Atlantic/Reykjavik,\'GMT0\'                                      | Pacific/Efate,\"\<+11\>-11\"                                         |
|                                                         |                                                             |                                                                                         |                                                                  |                                                                      |
| Africa/Djibouti,\"EAT-3\"                               | America/Hermosillo, \"MST7\"                                | Antarctica/Syowa,\"\<+03\>-3\"                                                          | Atlantic/South_Georgia,\"\<-02\>2\"                              | Pacific/Enderbury,\"\<+13\>-13\"                                     |
|                                                         |                                                             |                                                                                         |                                                                  |                                                                      |
| Africa/Douala,\"WHAT-1\"                                | America/Indiana/Indianapolis,\"EST5EDT,M3.2.0,M11.1.0\"     | Antarctica/Troll,\"\<+00\>0\<+02\>-2,M3.5.0/1,M10.5.0/3\"                               | Atlantic/Stanley,\"\<-03\>3\"                                    | Pacific/Fakaofo,\"\<+13\>-13\"                                       |
|                                                         |                                                             |                                                                                         |                                                                  |                                                                      |
| Africa/El_Aaiun,\"\<+01\>-1\"                           | America/Indiana/Knox,\"CST6CDT,M3.2.0,M11.1.0\"             | Antarctica/Vostok,\"\<+06\>-6\"                                                         | Atlantic/St_Helena,\'GMT0\'                                      | Pacific/Fiji,\"\<+12\>-12\<+13\>,M11.2.0,M1.2.3/99\"                 |
|                                                         |                                                             |                                                                                         |                                                                  |                                                                      |
| Africa/Freetown,\'GMT0\'                                | America/Indiana/Marengo,\"EST5EDT,M3.2.0,M11.1.0\"          | Arctic/Longyearbyen,\"CET-1CEST,M3.5.0,M10.5.0/3\"                                      | Australia/Adelaide,\"ACST-9:30ACDT,M10.1.0,M4.1.0/3\"            | Pacific/Funafuti,\"\<+12\>-12\"                                      |
|                                                         |                                                             |                                                                                         |                                                                  |                                                                      |
| Africa/Gaborone,\"CAT-2\"                               | America/Indiana/Petersburg,\"EST5EDT,M3.2.0,M11.1.0\"       | Asia/Aden,\"\<+03\>-3\"                                                                 | Australia/Brisbane,\'AEST-10\'                                   | Pacific/Galapagos,\"\<-06\>6\"                                       |
|                                                         |                                                             |                                                                                         |                                                                  |                                                                      |
| Africa/Harare, \"CAT-2\"                                | America/Indiana/Tell_City,\"CST6CDT,M3.2.0,M11.1.0\"        | Asia/Almaty,\"\<+06\>-6\"                                                               | Australia/Broken_Hill,\"ACST-9:30ACDT,M10.1.0,M4.1.0/3\"         | Pacific/Gambier,\"\<-09\>9\"                                         |
|                                                         |                                                             |                                                                                         |                                                                  |                                                                      |
| Africa/Johannesburg,\'SAST-2\'                          | America/Indiana/Vevay,\"EST5EDT,M3.2.0,M11.1.0\"            | Asia/Amman,\"EET-2EEST,M2.5.4/24,M10.5.5/1\"                                            | Australia/Currie,\"AEST-10AEDT,M10.1.0,M4.1.0/3\"                | Pacific/Guadalcanal,\"\<+11\>-11\"                                   |
|                                                         |                                                             |                                                                                         |                                                                  |                                                                      |
| Africa/Juba,\"CAT-2\"                                   | America/Indiana/Vincennes,\"EST5EDT,M3.2.0,M11.1.0\"        | Asia/Anadyr,\"\<+12\>-12\"                                                              | Australia/Darwin,\"ACST-9:30\"                                   | Pacific/Guam,\"ChST-10\"                                             |
|                                                         |                                                             |                                                                                         |                                                                  |                                                                      |
| Africa/Kampala, \"EAT-3\"                               | America/Indiana/Winamac,\"EST5EDT,M3.2.0,M11.1.0\"          | Asia/Aqtau,\"\<+05\>-5\"                                                                | Australia/Eucla,\"\<+0845\>-8:45\"                               | Pacific/Honolulu, \"HST10\"                                          |
|                                                         |                                                             |                                                                                         |                                                                  |                                                                      |
| Africa/Khartoum,\"CAT-2\"                               | America/Inuvik,\"MST7MDT,M3.2.0,M11.1.0\"                   | Asia/Aqtobe,\"\<+05\>-5\"                                                               | Australia/Hobart,\"AEST-10AEDT,M10.1.0,M4.1.0/3\"                | Pacific/Kiritimati,\"\<+14\>-14\"                                    |
|                                                         |                                                             |                                                                                         |                                                                  |                                                                      |
| Africa/Kigali,\"CAT-2\"                                 | America/Iqaluit,\"EST5EDT,M3.2.0,M11.1.0\"                  | Asia/Ashgabat,\"\<+05\>-5\"                                                             | Australia/Lindeman,\"AEST-10\"                                   | Pacific/Kosrae,\"\<+11\>-11\"                                        |
|                                                         |                                                             |                                                                                         |                                                                  |                                                                      |
| Africa/Kinshasa,\'WHAT-1\'                              | America/Jamaica,\"EST5\"                                    | Asia/Atyrau,\"\<+05\>-5\"                                                               | Australia/Lord_Howe,\"\<+1030\>-10:30\<+11\>-11,M10.1.0,M4.1.0\" | Pacific/Kwajalein,\"\<+12\>-12\"                                     |
|                                                         |                                                             |                                                                                         |                                                                  |                                                                      |
| Africa/Lagos,\"WAT-1\"                                  | America/Juneau,\"AKST9AKDT,M3.2.0,M11.1.0\"                 | Asia/Baghdad,\"\<+03\>-3\"                                                              | Australia/Melbourne,\"AEST-10AEDT,M10.1.0,M4.1.0/3\"             | Pacific/Majuro,\"\<+12\>-12\"                                        |
|                                                         |                                                             |                                                                                         |                                                                  |                                                                      |
| Africa/Libreville,\"WHAT-1\"                            | America/Kentucky/Louisville,\"EST5EDT,M3.2.0,M11.1.0\"      | Asia/Bahrain,\"\<+03\>-3\"                                                              | Australia/Perth, \"AWST-8\"                                      | Pacific/Marquesas,\"\<-0930\>9:30\"                                  |
|                                                         |                                                             |                                                                                         |                                                                  |                                                                      |
| Africa/Lome,\'GMT0\'                                    | America/Kentucky/Monticello,\"EST5EDT,M3.2.0,M11.1.0\"      | Asia/Baku,\"\<+04\>-4\"                                                                 | Australia/Sydney,\"AEST-10AEDT,M10.1.0,M4.1.0/3\"                | Pacific/Midway, \"SST11\"                                            |
|                                                         |                                                             |                                                                                         |                                                                  |                                                                      |
| Africa/Luanda, \"WHAT-1\"                               | America/Kralendijk, \"AST4\"                                | Asia/Bangkok,\"\<+07\>-7\"                                                              | Europe/Amsterdam,\"CET-1CEST,M3.5.0,M10.5.0/3\"                  | Pacific/Nauru,\"\<+12\>-12\"                                         |
|                                                         |                                                             |                                                                                         |                                                                  |                                                                      |
| Africa/Lubumbashi,\"CAT-2\"                             | America/La_Paz,\"\<-04\>4\"                                 | Asia/Barnaul,\"\<+07\>-7\"                                                              | Europe/Andorra,\"CET-1CEST,M3.5.0,M10.5.0/3\"                    | Pacific/Niue,\"\<-11\>11\"                                           |
|                                                         |                                                             |                                                                                         |                                                                  |                                                                      |
| Africa/Lusaka,\'CAT-2\'                                 | America/Lima,\"\<-05\>5\"                                   | Asia/Beirut,\"EET-2EEST,M3.5.0/0,M10.5.0/0\"                                            | Europe/Astrakhan,\"\<+04\>-4\"                                   | Pacific/Norfolk,\"\<+11\>-11\<+12\>,M10.1.0,M4.1.0/3\"               |
|                                                         |                                                             |                                                                                         |                                                                  |                                                                      |
| Africa/Malabo, \"WHAT-1\"                               | America/Los_Angeles,\"PST8PDT,M3.2.0,M11.1.0\"              | Asia/Bishkek,\"\<+06\>-6\"                                                              | Europe/Athens,\"EET-2EEST,M3.5.0/3,M10.5.0/4\"                   | Pacific/Noumea,\"\<+11\>-11\"                                        |
|                                                         |                                                             |                                                                                         |                                                                  |                                                                      |
| Africa/Maputo,\"CAT-2\"                                 | America/Lower_Princes,\"AST4\"                              | Asia/Brunei,\"\<+08\>-8\"                                                               | Europe/Belgrade,\"CET-1CEST,M3.5.0,M10.5.0/3\"                   | Pacific/Pago_Pago,\"SST11\"                                          |
|                                                         |                                                             |                                                                                         |                                                                  |                                                                      |
| Africa/Maseru,\"SAST-2\"                                | America/Maceio,\"\<-03\>3\"                                 | Asia/Chita,\"\<+09\>-9\"                                                                | Europe/Berlin,\"CET-1CEST,M3.5.0,M10.5.0/3\"                     | Pacific/Palau,\"\<+09\>-9\"                                          |
|                                                         |                                                             |                                                                                         |                                                                  |                                                                      |
| Africa/Mbabane,\'SAST-2\'                               | America/Managua, \"CST6\"                                   | Asia/Choibalsan,\"\<+08\>-8\"                                                           | Europe/Bratislava,\"CET-1CEST,M3.5.0,M10.5.0/3\"                 | Pacific/Pitcairn,\"\<-08\>8\"                                        |
|                                                         |                                                             |                                                                                         |                                                                  |                                                                      |
| Africa/Mogadishu,\'EAT-3\'                              | America/Manaus,\"\<-04\>4\"                                 | Asia/Colombo,\"\<+0530\>-5:30\"                                                         | Europe/Brussels,\"CET-1CEST,M3.5.0,M10.5.0/3\"                   | Pacific/Pohnpei,\"\<+11\>-11\"                                       |
|                                                         |                                                             |                                                                                         |                                                                  |                                                                      |
| Africa/Monrovia,\'GMT0\'                                | America/Marigot, \"AST4\"                                   | Asia/Damascus,\"EET-2EEST,M3.5.5/0,M10.5.5/0\"                                          | Europe/Bucharest,\"EET-2EEST,M3.5.0/3,M10.5.0/4\"                | Pacific/Port_Moresby,\"\<+10\>-10\"                                  |
|                                                         |                                                             |                                                                                         |                                                                  |                                                                      |
| Africa/Nairobi,\'EAT-3\'                                | America/Martinique, \"AST4\"                                | Asia/Dhaka,\"\<+06\>-6\"                                                                | Europe/Budapest,\"CET-1CEST,M3.5.0,M10.5.0/3\"                   | Pacific/Rarotonga,\"\<-10\>10\"                                      |
|                                                         |                                                             |                                                                                         |                                                                  |                                                                      |
| Africa/Ndjamena,\"WAT-1\"                               | America/Matamoros,\"CST6CDT,M3.2.0,M11.1.0\"                | Asia/Dili,\"\<+09\>-9\"                                                                 | Europe/Busingen,\"CET-1CEST,M3.5.0,M10.5.0/3\"                   | Pacific/Saipan,\"ChST-10\"                                           |
|                                                         |                                                             |                                                                                         |                                                                  |                                                                      |
| Africa/Niamey,\"WAT-1\"                                 | America/Mazatlan,\"MST7MDT,M4.1.0,M10.5.0\"                 | Asia/Dubai,\"\<+04\>-4\"                                                                | Europe/Chisinau,\"EET-2EEST,M3.5.0,M10.5.0/3\"                   | Pacific/Tahiti,\"\<-10\>10\"                                         |
|                                                         |                                                             |                                                                                         |                                                                  |                                                                      |
| Africa/Nouakchott,\'GMT0\'                              | America/Menominee,\"CST6CDT,M3.2.0,M11.1.0\"                | Asia/Dushanbe,\"\<+05\>-5\"                                                             | Europe/Copenhagen,\"CET-1CEST,M3.5.0,M10.5.0/3\"                 | Pacific/Tarawa,\"\<+12\>-12\"                                        |
|                                                         |                                                             |                                                                                         |                                                                  |                                                                      |
| Africa/Ouagadougou,\'GMT0\'                             | America/Merida,\"CST6CDT,M4.1.0,M10.5.0\"                   | Asia/Famagusta,\"EET-2EEST,M3.5.0/3,M10.5.0/4\"                                         | Europe/Dublin,\"IST-1GMT0,M10.5.0,M3.5.0/1\"                     | Pacific/Tongatapu,\"\<+13\>-13\"                                     |
|                                                         |                                                             |                                                                                         |                                                                  |                                                                      |
| Africa/Porto-Novo, \"WAT-1\"                            | America/Metlakatla,\"AKST9AKDT,M3.2.0,M11.1.0\"             | Asia/Gaza,\'EET-2EEST,M3.4.4/48,M10.5.5/1\'                                             | Europe/Gibraltar,\"CET-1CEST,M3.5.0,M10.5.0/3\"                  | Pacific/Wake,\"\<+12\>-12\"                                          |
|                                                         |                                                             |                                                                                         |                                                                  |                                                                      |
| Africa/Sao_Tome,\'GMT0\'                                | America/Mexico_City,\"CST6CDT,M4.1.0,M10.5.0\"              | Asia/Hebron,\"EET-2EEST,M3.4.4/48,M10.5.5/1\"                                           | Europe/Guernsey,\"GMT0BST,M3.5.0/1,M10.5.0\"                     | Pacific/Valais,\"\<+12\>-12\"                                        |
|                                                         |                                                             |                                                                                         |                                                                  |                                                                      |
| Africa/Tripoli, \"EET-2\"                               | America/Miquelon,\"\<-03\>3\<-02\>,M3.2.0,M11.1.0\"         | Asia/Ho_Chi_Minh,\"\<+07\>-7\"                                                          | Europe/Helsinki,\"EET-2EEST,M3.5.0/3,M10.5.0/4\"                 | Etc/GMT,\"GMT0\"                                                     |
|                                                         |                                                             |                                                                                         |                                                                  |                                                                      |
| Africa/Tunis, \'CET-1\'                                 | America/Moncton,\"AST4ADT,M3.2.0,M11.1.0\"                  | Asia/Hong_Kong,\"HKT-8\"                                                                | Europe/Isle_of_Man,\"GMT0BST,M3.5.0/1,M10.5.0\"                  | Etc/GMT-0,\"GMT0\"                                                   |
|                                                         |                                                             |                                                                                         |                                                                  |                                                                      |
| Africa/Windhoek,\"CAT-2\"                               | America/Monterrey,\"CST6CDT,M4.1.0,M10.5.0\"                | Asia/Hovd,\"\<+07\>-7\"                                                                 | Europe/Istanbul,\"\<+03\>-3\"                                    | Etc/GMT-1,\"\<+01\>-1\"                                              |
|                                                         |                                                             |                                                                                         |                                                                  |                                                                      |
| America/Adak,\"HST10HDT,M3.2.0,M11.1.0\"                | America/Montevideo,\"\<-03\>3\"                             | Asia/Irkutsk,\"\<+08\>-8\"                                                              | Europe/Jersey,\"GMT0BST,M3.5.0/1,M10.5.0\"                       | Etc/GMT-2,\"\<+02\>-2\"                                              |
|                                                         |                                                             |                                                                                         |                                                                  |                                                                      |
| America/Anchorage,\"AKST9AKDT,M3.2.0,M11.1.0\"          | America/Montreal,\"EST5EDT,M3.2.0,M11.1.0\"                 | Asia/Jakarta,\"WIB-7\"                                                                  | Europe/Kaliningrad, \'EET-2\'                                    | Etc/GMT-3,\"\<+03\>-3\"                                              |
|                                                         |                                                             |                                                                                         |                                                                  |                                                                      |
| America/Anguilla, \"AST4\"                              | America/Montserrat,\"AST4\"                                 | Asia/Jayapura,\"WHITE-9\"                                                               | Europe/Kiev,\"EET-2EEST,M3.5.0/3,M10.5.0/4\"                     | Etc/GMT-4,\"\<+04\>-4\"                                              |
|                                                         |                                                             |                                                                                         |                                                                  |                                                                      |
| America/Antigua, \"AST4\"                               | America/Nassau,\"EST5EDT,M3.2.0,M11.1.0\"                   | Asia/Jerusalem,\"IST-2IDT,M3.4.4/26,M10.5.0\"                                           | Europe/Kirov,\'\<+03\>-3\'                                       | Etc/GMT-5,\"\<+05\>-5\"                                              |
|                                                         |                                                             |                                                                                         |                                                                  |                                                                      |
| America/Araguaina,\"\<-03\>3\"                          | America/New_York,\"EST5EDT,M3.2.0,M11.1.0\"                 | Asia/Kabul,\"\<+0430\>-4:30\"                                                           | Europe/Lisbon,\"WET0WEST,M3.5.0/1,M10.5.0\"                      | Etc/GMT-6,\"\<+06\>-6\"                                              |
|                                                         |                                                             |                                                                                         |                                                                  |                                                                      |
| America/Argentina/Buenos_Aires,\"\<-03\>3\"             | America/Nipigon,\"EST5EDT,M3.2.0,M11.1.0\"                  | Asia/Kamchatka,\"\<+12\>-12\"                                                           | Europe/Ljubljana,\"CET-1CEST,M3.5.0,M10.5.0/3\"                  | Etc/GMT-7,\"\<+07\>-7\"                                              |
|                                                         |                                                             |                                                                                         |                                                                  |                                                                      |
| America/Argentina/Catamarca,\'\<-03\>3\'                | America/Nome,\"AKST9AKDT,M3.2.0,M11.1.0\"                   | Asia/Karachi, \"PKT-5\"                                                                 | Europe/London,\"GMT0BST,M3.5.0/1,M10.5.0\"                       | Etc/GMT-8,\"\<+08\>-8\"                                              |
|                                                         |                                                             |                                                                                         |                                                                  |                                                                      |
| America/Argentina/Cordoba,\"\<-03\>3\"                  | America/Noronha,\"\<-02\>2\"                                | Asia/Kathmandu,\"\<+0545\>-5:45\"                                                       | Europe/Luxembourg,\"CET-1CEST,M3.5.0,M10.5.0/3\"                 | Etc/GMT-9,\"\<+09\>-9\"                                              |
|                                                         |                                                             |                                                                                         |                                                                  |                                                                      |
| America/Argentina/Jujuy,\"\<-03\>3\"                    | America/North_Dakota/Beulah,\"CST6CDT,M3.2.0,M11.1.0\"      | Asia/Khandyga,\"\<+09\>-9\"                                                             | Europe/Madrid,\"CET-1CEST,M3.5.0,M10.5.0/3\"                     | Etc/GMT-10,\"\<+10\>-10\"                                            |
|                                                         |                                                             |                                                                                         |                                                                  |                                                                      |
| America/Argentina/La_Rioja,\"\<-03\>3\"                 | America/North_Dakota/Center,\"CST6CDT,M3.2.0,M11.1.0\"      | Asia/Kolkata,\"IST-5:30\"                                                               | Europe/Malta,\"CET-1CEST,M3.5.0,M10.5.0/3\"                      | Etc/GMT-11,\"\<+11\>-11\"                                            |
|                                                         |                                                             |                                                                                         |                                                                  |                                                                      |
| America/Argentina/Mendoza,\"\<-03\>3\"                  | America/North_Dakota/New_Salem,\"CST6CDT,M3.2.0,M11.1.0\"   | Asia/Krasnoyarsk,\"\<+07\>-7\"                                                          | Europe/Mariehamn,\"EET-2EEST,M3.5.0/3,M10.5.0/4\"                | Etc/GMT-12,\"\<+12\>-12\"                                            |
|                                                         |                                                             |                                                                                         |                                                                  |                                                                      |
| America/Argentina/Rio_Gallegos,\"\<-03\>3\"             | America/Nuuk,\"\<-03\>3\<-02\>,M3.5.0/-2,M10.5.0/-1\"       | Asia/Kuala_Lumpur,\"\<+08\>-8\"                                                         | Europe/Minsk,\'\<+03\>-3\'                                       | Etc/GMT-13,\"\<+13\>-13\"                                            |
|                                                         |                                                             |                                                                                         |                                                                  |                                                                      |
| America/Argentina/Salta,\"\<-03\>3\"                    | America/Ojinaga,\"MST7MDT,M3.2.0,M11.1.0\"                  | Asia/Kuching,\"\<+08\>-8\"                                                              | Europe/Monaco,\"CET-1CEST,M3.5.0,M10.5.0/3\"                     | Etc/GMT-14,\"\<+14\>-14\"                                            |
|                                                         |                                                             |                                                                                         |                                                                  |                                                                      |
| America/Argentina/San_Juan,\"\<-03\>3\"                 | America/Panama, \"EST5\"                                    | Asia/Kuwait,\"\<+03\>-3\"                                                               | Europe/Moscow,\"MSK-3\"                                          | Etc/GMT0,\"GMT0\"                                                    |
|                                                         |                                                             |                                                                                         |                                                                  |                                                                      |
| America/Argentina/San_Luis,\"\<-03\>3\"                 | America/Pangnirtung,\"EST5EDT,M3.2.0,M11.1.0\"              | Asia/Macau, \"CST-8\"                                                                   | Europe/Oslo,\"CET-1CEST,M3.5.0,M10.5.0/3\"                       | Etc/GMT+0,\"GMT0\"                                                   |
|                                                         |                                                             |                                                                                         |                                                                  |                                                                      |
| America/Argentina/Tucuman,\"\<-03\>3\"                  | America/Paramaribo,\"\<-03\>3\"                             | Asia/Magadan,\"\<+11\>-11\"                                                             | Europe/Paris,\"CET-1CEST,M3.5.0,M10.5.0/3\"                      | Etc/GMT+1,\"\<-01\>1\"                                               |
|                                                         |                                                             |                                                                                         |                                                                  |                                                                      |
| America/Argentina/Ushuaia,\"\<-03\>3\"                  | America/Phoenix, \"MST7\"                                   | Asia/Makassar,\"WITA-8\"                                                                | Europe/Podgorica,\"CET-1CEST,M3.5.0,M10.5.0/3\"                  | Etc/GMT+2,\"\<-02\>2\"                                               |
|                                                         |                                                             |                                                                                         |                                                                  |                                                                      |
| America/Aruba, \"AST4\"                                 | America/Port-au-Prince,\"EST5EDT,M3.2.0,M11.1.0\"           | Asia/Manila, \"PST-8\"                                                                  | Europe/Prague,\"CET-1CEST,M3.5.0,M10.5.0/3\"                     | Etc/GMT+3,\"\<-03\>3\"                                               |
|                                                         |                                                             |                                                                                         |                                                                  |                                                                      |
| America/Asuncion,\"\<-04\>4\<-03\>,M10.1.0/0,M3.4.0/0\" | America/Port_of_Spain,\"AST4\"                              | Asia/Muscat,\"\<+04\>-4\"                                                               | Europe/Riga,\'EET-2EEST,M3.5.0/3,M10.5.0/4\'                     | Etc/GMT+4,\"\<-04\>4\"                                               |
|                                                         |                                                             |                                                                                         |                                                                  |                                                                      |
| America/Atikokan,\"EST5\"                               | America/Porto_Velho,\"\<-04\>4\"                            | Asia/Nicosia,\"EET-2EEST,M3.5.0/3,M10.5.0/4\"                                           | Europe/Rome,\"CET-1CEST,M3.5.0,M10.5.0/3\"                       | Etc/GMT+5,\"\<-05\>5\"                                               |
|                                                         |                                                             |                                                                                         |                                                                  |                                                                      |
| America/Bahia,\"\<-03\>3\"                              | America/Puerto_Rico,\"AST4\"                                | Asia/Novokuznetsk,\"\<+07\>-7\"                                                         | Europe/Samara,\"\<+04\>-4\"                                      | Etc/GMT+6,\"\<-06\>6\"                                               |
|                                                         |                                                             |                                                                                         |                                                                  |                                                                      |
| America/Bahia_Banderas,\"CST6CDT,M4.1.0,M10.5.0\"       | America/Punta_Arenas,\"\<-03\>3\"                           | Asia/Novosibirsk,\"\<+07\>-7\"                                                          | Europe/San_Marino,\"CET-1CEST,M3.5.0,M10.5.0/3\"                 | Etc/GMT+7,\"\<-07\>7\"                                               |
|                                                         |                                                             |                                                                                         |                                                                  |                                                                      |
| America/Barbados, \"AST4\"                              | America/Rainy_River,\"CST6CDT,M3.2.0,M11.1.0\"              | Asia/Omsk,\"\<+06\>-6\"                                                                 | Europe/Sarajevo,\"CET-1CEST,M3.5.0,M10.5.0/3\"                   | Etc/GMT+8,\"\<-08\>8\"                                               |
|                                                         |                                                             |                                                                                         |                                                                  |                                                                      |
| America/Belem,\"\<-03\>3\"                              | America/Rankin_Inlet,\"CST6CDT,M3.2.0,M11.1.0\"             | Asia/Oral,\"\<+05\>-5\"                                                                 | Europe/Saratov,\"\<+04\>-4\"                                     | Etc/GMT+9,\"\<-09\>9\"                                               |
|                                                         |                                                             |                                                                                         |                                                                  |                                                                      |
| America/Belize, \"CST6\"                                | America/Recife,\"\<-03\>3\"                                 | Asia/Phnom_Penh,\"\<+07\>-7\"                                                           | Europe/Simferopol,\'MSK-3\'                                      | Etc/GMT+10,\"\<-10\>10\"                                             |
|                                                         |                                                             |                                                                                         |                                                                  |                                                                      |
| America/Blanc-Sablon, \"AST4\"                          | America/Regina, \"CST6\"                                    | Asia/Pontianak, \"WIB-7\"                                                               | Europe/Skopje,\"CET-1CEST,M3.5.0,M10.5.0/3\"                     | Etc/GMT+11,\"\<-11\>11\"                                             |
|                                                         |                                                             |                                                                                         |                                                                  |                                                                      |
| America/Boa_Vista,\"\<-04\>4\"                          | America/Resolute,\"CST6CDT,M3.2.0,M11.1.0\"                 | Asia/Pyongyang,\"KST-9\"                                                                | Europe/Sofia,\"EET-2EEST,M3.5.0/3,M10.5.0/4\"                    | Etc/GMT+12,\"\<-12\>12\"                                             |
|                                                         |                                                             |                                                                                         |                                                                  |                                                                      |
| America/Bogota,\"\<-05\>5\"                             | America/Rio_Branco,\"\<-05\>5\"                             | Asia/Qatar,\"\<+03\>-3\"                                                                | Europe/Stockholm,\"CET-1CEST,M3.5.0,M10.5.0/3\"                  | Etc/UCT,\"UTC0\"                                                     |
|                                                         |                                                             |                                                                                         |                                                                  |                                                                      |
| America/Boise,\"MST7MDT,M3.2.0,M11.1.0\"                | America/Santarem,\"\<-03\>3\"                               | Asia/Qyzylorda,\"\<+05\>-5\"                                                            | Europe/Tallinn,\"EET-2EEST,M3.5.0/3,M10.5.0/4\"                  | Etc/UTC,\"UTC0\"                                                     |
|                                                         |                                                             |                                                                                         |                                                                  |                                                                      |
| America/Cambridge_Bay,\"MST7MDT,M3.2.0,M11.1.0\"        | America/Santiago,\"\<-04\>4\<-03\>,M9.1.6/24,M4.1.6/24\"    | Asia/Riyadh,\"\<+03\>-3\"                                                               | Europe/Tirane,\"CET-1CEST,M3.5.0,M10.5.0/3\"                     | Etc/Greenwich,\'GMT0\'                                               |
|                                                         |                                                             |                                                                                         |                                                                  |                                                                      |
| America/Campo_Grande,\"\<-04\>4\"                       | America/Santo_Domingo,\"AST4\"                              | Asia/Sakhalin,\"\<+11\>-11\"                                                            | Europe/Ulyanovsk,\"\<+04\>-4\"                                   | Etc/Universal,\"UTC0\"                                               |
|                                                         |                                                             |                                                                                         |                                                                  |                                                                      |
| America/Cancun, \"EST5\"                                | America/Sao_Paulo,\"\<-03\>3\"                              | Asia/Samarkand,\"\<+05\>-5\"                                                            | Europe/Uzhgorod,\"EET-2EEST,M3.5.0/3,M10.5.0/4\"                 | Etc/Zulu,\"UTC0\"                                                    |
|                                                         |                                                             |                                                                                         |                                                                  |                                                                      |
| America/Caracas,\"\<-04\>4\"                            | America/Scoresbysund,\"\<-01\>1\<+00\>,M3.5.0/0,M10.5.0/1\" | Asia/Seoul,\"KST-9\"                                                                    | Europe/Vaduz,\"CET-1CEST,M3.5.0,M10.5.0/3\"                      |                                                                      |
|                                                         |                                                             |                                                                                         |                                                                  |                                                                      |
| America/Cayenne,\"\<-03\>3\"                            | America/Sitka,\"AKST9AKDT,M3.2.0,M11.1.0\"                  | Asia/Shanghai, \"CST-8\"                                                                | Europe/Vatican,\"CET-1CEST,M3.5.0,M10.5.0/3\"                    |                                                                      |
|                                                         |                                                             |                                                                                         |                                                                  |                                                                      |
| America/Cayman,\"EST5\"                                 | America/St_Barthelemy,\"AST4\"                              | Asia/Singapore,\"\<+08\>-8\"                                                            | Europe/Vienna,\"CET-1CEST,M3.5.0,M10.5.0/3\"                     |                                                                      |
|                                                         |                                                             |                                                                                         |                                                                  |                                                                      |
| America/Chicago,\"CST6CDT,M3.2.0,M11.1.0\"              | America/St_Johns,\"NST3:30NDT,M3.2.0,M11.1.0\"              | Asia/Srednekolymsk,\"\<+11\>-11\"                                                       | Europe/Vilnius,\"EET-2EEST,M3.5.0/3,M10.5.0/4\"                  |                                                                      |
|                                                         |                                                             |                                                                                         |                                                                  |                                                                      |
| America/Chihuahua,\"MST7MDT,M4.1.0,M10.5.0\"            | America/St_Kitts,\"AST4\"                                   | Asia/Taipei,\"CST-8\"                                                                   | Europe/Volgograd,\"\<+03\>-3\"                                   |                                                                      |
|                                                         |                                                             |                                                                                         |                                                                  |                                                                      |
| America/Costa_Rica,\"CST6\"                             | America/St_Lucia,\"AST4\"                                   | Asia/Tashkent,\"\<+05\>-5\"                                                             | Europe/Warsaw,\"CET-1CEST,M3.5.0,M10.5.0/3\"                     |                                                                      |
|                                                         |                                                             |                                                                                         |                                                                  |                                                                      |
| America/Creston,\"MST7\"                                | America/St_Thomas,\"AST4\"                                  | Asia/Tbilisi,\"\<+04\>-4\"                                                              | Europe/Zagreb,\"CET-1CEST,M3.5.0,M10.5.0/3\"                     |                                                                      |
|                                                         |                                                             |                                                                                         |                                                                  |                                                                      |
| America/Cuiaba,\"\<-04\>4\"                             | America/St_Vincent,\"AST4\"                                 | Asia/Tehran,\"\<+0330\>-3:30\<+0430\>,J79/24,J263/24\"                                  | Europe/Zaporozhye,\"EET-2EEST,M3.5.0/3,M10.5.0/4\"               |                                                                      |
|                                                         |                                                             |                                                                                         |                                                                  |                                                                      |
| America/Curacao, \"AST4\"                               | America/Swift_Current,\"CST6\"                              | Asia/Thimphu,\"\<+06\>-6\"                                                              | Europe/Zurich,\"CET-1CEST,M3.5.0,M10.5.0/3\"                     |                                                                      |
|                                                         |                                                             |                                                                                         |                                                                  |                                                                      |
| America/Danmarkshavn,\'GMT0\'                           |                                                             |                                                                                         |                                                                  |                                                                      |
|                                                         |                                                             |                                                                                         |                                                                  |                                                                      |
| America/Dawson,\"MST7\"                                 |                                                             |                                                                                         |                                                                  |                                                                      |
|                                                         |                                                             |                                                                                         |                                                                  |                                                                      |
| America/Dawson_Creek,\"MST7\"                           |                                                             |                                                                                         |                                                                  |                                                                      |
|                                                         |                                                             |                                                                                         |                                                                  |                                                                      |
| America/Denver,\"MST7MDT,M3.2.0,M11.1.0\"               |                                                             |                                                                                         |                                                                  |                                                                      |
+=========================================================+=============================================================+=========================================================================================+==================================================================+======================================================================+
+---------------------------------------------------------+-------------------------------------------------------------+-----------------------------------------------------------------------------------------+------------------------------------------------------------------+----------------------------------------------------------------------+
