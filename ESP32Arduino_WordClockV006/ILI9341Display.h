/*
ILI9341 connections
MISO  to GPIO 10
LED   to 3V3
SCK   to GPIO 09
MOSI  to GPIO 08  
DC/RS to GPIO 06
RESET to GPIO 07
CS    to GND
GND   to GND
3V3   to 3V3

//--------------------------------------------                                                //
// Avaiable functions for the ILI9341 display
// -------------------------------------------
 void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
 void fillScreen(uint16_t color);
 void setTextSize(uint8_t s);
 void setTextSize(uint8_t sx, uint8_t sy);
 void setFont(const GFXfont *f = NULL);
 void drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size);
 void drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size_x, uint8_t size_y);
 virtual void setRotation(uint8_t r);
 virtual void invertDisplay(bool i);

 void setCursor(int16_t x, int16_t y)
 void setTextColor(uint16_t c, uint16_t bg)
 void setTextWrap(bool w) { wrap = w; }
 int16_t width(void) const { return _width; };
 int16_t height(void) const { return _height; }
 uint8_t getRotation(void) const { return rotation; }
 int16_t getCursorX(void) const { return cursor_x; }
 int16_t getCursorY(void) const { return cursor_y; };

*/ 
 
  
#include <Adafruit_ILI9341.h>  // https://github.com/adafruit/Adafruit_ILI9341
//#include <Adafruit_GFX.h>      // Core graphics library, with extra fonts.

extern void SendMessageBLE(std::string Message);
extern void Tekstprint(char const *tekst)    { if(Serial) Serial.print(tekst);  SendMessageBLE(tekst);sptext[0]=0;   } 
extern void Tekstprintln(char const *tekst)  { sprintf(sptext,"%s\n",tekst); Tekstprint(sptext);  }
extern void to_upper(char* string);
extern void SetBackLight(byte intensity);

//--------------------------------------------
// SCREEN ILI9341
//--------------------------------------------
#define TFT_MISO   -1 //10  not used 
#define TFT_SCK    9
#define TFT_MOSI   8
#define TFT_DC     6
#define TFT_RESET  7
#define TFT_CS     -1 //0  not used
#define TFT_LED    1 // the LED backlight display
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCK, TFT_RESET, TFT_MISO);
uint8_t ScreenMiddleX = 160;
uint8_t ScreenMiddleY = 120;




//--------------------------------------------                                                //
// Common common print routines to display
//--------------------------------------------
void Displayprint(const char *tekst)  { tft.print(tekst);   }                                 // Print to an attached display
void Displayprintln(const char *tekst){ tft.println(tekst); }


//--------------------------------------------
//  LED Set color for LED. 
// Fill the struct Strippos with the proper character and its colour
//--------------------------------------------
void ColorLeds(char const *Texkst, int FirstLed, int LastLed, uint32_t RGBColor)
{ 
 int n, i=0;
 char Tekst[135];
  if (!NoTextInColorLeds && (strlen(Texkst) > 0 && strlen(Texkst) <10) )
     {sprintf(sptext,"%s ",Texkst); Tekstprint(sptext); }                                     // Print the text  
                                                                                              // sprintf(sptext," %s, F:%d, L:%d F-L:%d ",Texkst, FirstLed,LastLed,1+LastLed-FirstLed );  Tekstprint(sptext);
 strcpy(Tekst,Texkst);
 to_upper(Tekst);   
 for (n=FirstLed; n<=FirstLed + (LastLed-FirstLed); n++)
 {
  Strippos[n].Character = Tekst[i++];
  Strippos[n].RGBColor = RGBColor;                                                            // Every character has its color stored here
//  sprintf(sptext,"-Strippos[%d].Character=:%c",n, Strippos[n].Character); Tekstprint(sptext);   
 }
}
//                                                                                            //
//--------------------------------------------                                                //
//  LED Set brightness of backlight
//------------------------------------------------------------------------------  
void SetBrightnessLeds(byte Bright)
{
 SetBackLight(Bright);                                                                        // Set brightness of LEDs   
}
//--------------------------------------------
//  LED Clear the character string
//--------------------------------------------
void LedsOff(void) 
{ 
 for (int n=0; n<NUM_LEDS; n++) 
     Strippos[n].Character = Strippos[n].RGBColor = 0;                                        // Erase the struct Strippos
}

//--------------------------------------------
//  LED Set second color
//  Set the colour per second of 'IS' and 'WAS'
//--------------------------------------------
void SetSecondColour(void)
{
 switch (Mem.DisplayChoice)
  {
   case DEFAULTCOLOUR: LetterColor = C_YELLOW;
                       MINColor =    C_YELLOW;   // C_GREEN + ((timeinfo.tm_min/2)<<19);
                       SECColor=     C_YELLOW;   //C_GREEN + ((timeinfo.tm_hour/2)<<19);      // (30 << 19 = 0XF00000         
                                                                                    break;
   case HOURLYCOLOUR : LetterColor = MINColor = SECColor = HourColor[timeinfo.tm_hour];       break;    // A colour every hour
   case WHITECOLOR   : LetterColor = MINColor = SECColor = C_WHITE;                 break;    // All white
   case OWNCOLOUR    : LetterColor = MINColor = SECColor = Mem.OwnColour;           break;    // Own colour
   case OWNHETISCLR  : LetterColor = Mem.OwnColour; 
                       MINColor = C_YELLOW;
                       SECColor = C_YELLOW;                                         break;    // Own colour except HET IS WAS  
   case WHEELCOLOR   : LetterColor = MINColor = SECColor = (17*(timeinfo.tm_min*60));      break;    // Colour of all letters changes per minute
   case DIGITAL      : LetterColor = C_WHITE; MINColor = SECColor = C_BLACK;        break;    // Digital display of time. No IS WAS turn color off in display
   case ANALOOG      : LetterColor = C_YELLOW; MINColor = SECColor = C_BLACK;        break;    //
  }
}

//--------------------------------------------                                                //
// CLOCK Say the time and load the LEDs 
// with the proper colour and intensity
//--------------------------------------------
extern void Dutch(void);
extern void English(void);
extern void German(void);
extern void French(void);
extern void Print_tijd(void);
extern void ShowChars(void);

void Displaytime(void)
{ 
 if (Mem.DisplayChoice == DIGITAL) return;                                                    // PrintDigital) 
 if (Mem.DisplayChoice == ANALOOG) return;                                                    // PrintAnalog)  

 byte Language;                                                                               // Start by clearing the display to a known state 
 SetSecondColour();
 if(Mem.LanguageChoice == 4)  Language = random(4);
   else                       Language = Mem.LanguageChoice;
 switch(Language)                                                                             //
     {
      case 0: 
      strncpy(Template,"HETVISOWASOVIJFQPRECIESZSTIENKPFKWARTSVOORSOVERAHALFSMIDDERTVIJFATWEESOEENOXVIERELFQTIENKTWAALFBHDRIECNEGENACHTFZESVZEVENOENVUUR",129);  
              ColorLeds(Template,0,127, Mem.DimmedLetter);
              Dutch(); Print_tijd();   break;
      case 1: 
      strncpy(Template,"HITVISOWASOEXACTHALFITWENTYEFIVEQUARTERTENZPASTATOLFSMIDKSIXVTWOFIVEETWELVEXTENRELEVENENFOURAONETEIGHTHREENIGHTFNINESEVENROCLOCK",129);  
              ColorLeds(Template,0,127, Mem.DimmedLetter);
              English(); Print_tijd(); break;
      case 2: 
      strncpy(Template,"ESTISTWARSGENAUTZEHNFUNFVIERTELQZWANZIGTKURZAVORNACHYHALBKFUNFBOEINSEVIERZWOLFARMITTERNACHTNDREISECHHNSIEBENEUNBZWEIZEHNELFMKUHR",129);
              ColorLeds(Template,0,127, Mem.DimmedLetter); 
              German(); Print_tijd();  break;
      case 3:
      strncpy(Template,"ILWESTETAITEXACTSIXDEUXTROISONZEQUATRERMINUITDIXCINQNEUFKMIDISWOHUITESEPTUNEDOSRHEURESYETOMOINSELETDEMIENEQUARTNDIXEVINGTOCINQKR",129); 
              ColorLeds(Template,0,127, Mem.DimmedLetter);
              French(); Print_tijd();  break;
     }
      ShowChars(); 
}

//--------------------------------------------
//  SCREEN ILI9341 Init screen ILI9341
//--------------------------------------------
void InitScreenILI9341(void)
{
 tft.begin();
 SetBackLight(0XA0);   // Turn on the backlight 0 - 0xFF (0-255)
 tft.setRotation(3);   // upside down
// tft.fillScreen(ConvertRGB32toRGB16(C_ORANGE));                                             // Convert colors to RGB16, except C_BLACK (=0x000000)
 tft.setTextColor(C_BLACK, ConvertRGB32toRGB16(C_ORANGE));                                    // Convert colors to RGB16, except C_BLACK (=0x000000)
 tft.setCursor(0, tft.height()/3);
 tft.setTextSize(2);
 tft.println("Starting");
}
//--------------------------------------------                                                //
//  SCREEN ILI9341 Convert 32 bit RGBW to 16 bit RGB
//--------------------------------------------
unsigned short ConvertRGB32toRGB16(uint32_t sourceColor)
{
  uint32_t red   = (sourceColor & 0x00FF0000) >> 16;
  uint32_t green = (sourceColor & 0x0000FF00) >> 8;
  uint32_t blue  =  sourceColor & 0x000000FF;
  return (red >> 3 << 11) + (green >> 2 << 5) + (blue >> 3);
}

//--------------------------------------------
//  SCREEN ILI9341 Make the the screen black
//--------------------------------------------
void ClearScreenILI9341(void) 
{ 
  tft.fillScreen(Mem.BackGround);
}

//--------------------------------------------                                                //
//  SCREEN ILI9341
//  Print characters on the display with black background
//--------------------------------------------
void ShowCharsILI9341(void)
{
 int LEDnr = 0;
 for(int y = 0; y < MATRIX_HEIGHT; y++)
    for(int x = 0; x < MATRIX_WIDTH; x++, LEDnr++)
       {
        tft.drawChar(20 * x, 20 + 27 * y, Strippos[LEDnr].Character, 
                      ConvertRGB32toRGB16(Strippos[LEDnr].RGBColor), 
                            ConvertRGB32toRGB16(Mem.BackGround), 2); 
        loop();                                                                       // printing is slow, we need a check here too
       }     
}


//--------------------------------------------                                                //
//  SCREEN ILI9341 Print the digital time in screen
//--------------------------------------------
void PrintTimeInScreen(void) 
{
 tft.setTextColor(ConvertRGB32toRGB16(LetterColor), C_BLACK);                                 // Convert colors to RGB16, except C_BLACK (=0x000000)
 GetTijd(0); 
// if(Mem.WIFIOnOff && !IP_Printed)                                                             // If WIFI is on print the IP-address
 {
  tft.setTextSize(1);
  tft.setCursor(20, 20); 
  PrintIPaddressInScreen(); 
  IP_Printed = true; 
  }
 sprintf(sptext,"%02d:%02d:%02d",timeinfo.tm_hour,timeinfo.tm_min,timeinfo.tm_sec);
 tft.setTextSize(6); 
 tft.setCursor(20, 90);
 Displayprint(sptext); 
 if (Date_Printed)  return;
 sprintf(sptext,"%02d-%02d-%04d",timeinfo.tm_mday,timeinfo.tm_mon+1,timeinfo.tm_year+1900);
 tft.setCursor(20, 200);
 tft.setTextSize(2);
 Displayprint(sptext);
 Date_Printed = true;
}
//--------------------------------------------                                                //
//  SCREEN ILI9341
// Print Web server IP address in screen
//--------------------------------------------
void PrintIPaddressInScreen()
{
 sprintf(sptext, "%d.%d.%d.%d", WiFi.localIP()[0], WiFi.localIP()[1], WiFi.localIP()[2], WiFi.localIP()[3] );
 Displayprintln(sptext);
}

//--------------------------------------------
// ANALOG CLOCK setup
//--------------------------------------------

void  AnalogClockSetupILI9341(void) 
{
 float sx, sy; 
 uint8_t smx = ScreenMiddleX;
 uint8_t smy = ScreenMiddleY;
 uint16_t x00=0, x11=0, y00=0, y11=0;
// tft.fillScreen(ConvertRGB32toRGB16(C_GRAY)); 
 tft.setTextColor(ConvertRGB32toRGB16(C_WHITE), ConvertRGB32toRGB16(C_GRAY));                // Adding a background colour erases previous text automatically
 tft.fillCircle(smx, smy, 118, ConvertRGB32toRGB16(C_GREEN));                                // Draw clock face
 tft.fillCircle(smx, smy, 110, ConvertRGB32toRGB16(C_BLACK));
 for(int i = 0; i<360; i+= 30)   // Draw 12 lines
   {
    sx = cos((i-90)*0.0174532925);                                                           // 0.0174532925 rad/s in 1 deg/s
    sy = sin((i-90)*0.0174532925);                                                           // 1 rad = 57.29577951 degrees
    x00 = sx*114+smx;
    y00 = sy*114+smy;
    x11 = sx*100+smx;
    y11 = sy*100+smy;
    tft.drawLine(x00, y00, x11, y11, ConvertRGB32toRGB16(C_GREEN));
   }
 for(int i = 0; i<360; i+= 6)                                                                // Draw 60 dots
   {
    sx = cos((i-90)*0.0174532925);
    sy = sin((i-90)*0.0174532925);
    x00 = sx*102+smx;
    y00 = sy*102+smy;
    tft.drawPixel(x00, y00, ConvertRGB32toRGB16(C_WHITE));                                   // Draw minute markers
    if(i==0  || i==180) tft.fillCircle(x00, y00, 2, ConvertRGB32toRGB16(C_WHITE));           // Draw main quadrant dots
    if(i==90 || i==270) tft.fillCircle(x00, y00, 2, ConvertRGB32toRGB16(C_WHITE));
   }
//  tft.fillCircle(160, 121, 3, ConvertRGB32toRGB16(C_WHITE));

}
//                                                                                            //
//--------------------------------------------
// ANALOG CLOCK Draw hands
//--------------------------------------------
void DrawAnalogeClockHands(void)
{
 //float sdeg=0, mdeg=0, hdeg=0; 
 uint8_t smx = ScreenMiddleX;
 uint8_t smy = ScreenMiddleY;
 //static float sx = 0, sy = 1, mx = 1, my = 0, hx = -1, hy = 0;                                // Saved H, M, S x & y multipliers
 //static uint16_t osx=smx, osy=smy, omx=smx, omy=smy, ohx=smx, ohy=smy;                        // Saved H, M, S x & y coords
 static int ss=0,mm=0,hh=0;
                                                                                              // Pre-compute hand degrees, x & y coords for a fast screen update
 if (mm !=timeinfo.tm_min || ss==59) 
   {
    DrawMinuteHand(mm, 0, smx, smy, C_BLACK);                                                 // Erase old minute and hour hand
    DrawHourHand  (hh, mm, smx, smy, C_BLACK); 
    }
 DrawSecondHand(ss,   smx, smy, C_BLACK);                                                     // Redraw new hand positions, hour and minute hands not erased here to avoid flicker
 ss = (int) timeinfo.tm_sec;                                                                  // Retrieve the actual time
 mm = (int) timeinfo.tm_min;
 hh = (int) timeinfo.tm_hour;
 sprintf(sptext,"%02d:%02d:%02d",timeinfo.tm_hour,timeinfo.tm_min,timeinfo.tm_sec);
 tft.setTextColor(ConvertRGB32toRGB16(C_YELLOW), C_BLACK);
 tft.setTextSize(2); 
 tft.setCursor(smx-50, smy/2);
 Displayprint(sptext); 
 DrawSecondHand(ss,     smx, smy, 0xF800);   // (C_RED));
 DrawMinuteHand(mm, 0,  smx, smy, 0xFFFF);    // (C_WHITE)
 DrawHourHand(  hh, mm, smx, smy, 0xFFFF); 
 DrawSecondHand(ss,     smx, smy, 0xF800);
}
//--------------------------------------------
// ANALOG CLOCK Draw second hands
//--------------------------------------------
void DrawSecondHand(int ss , uint8_t smx, uint8_t smy, uint32_t Kleur)
{  
 if(ss <0 ) ss = 59;                                    
 uint16_t osx = cos((ss * 6 - 90) * 0.0174532925) * 90 + smx;                                 // 0-59 ss -> 0-354 degrees
 uint16_t osy = sin((ss * 6 - 90) * 0.0174532925) * 90 + smy; 
 tft.drawLine(osx,   osy,   smx, smy, Kleur);    
 tft.fillCircle(smx, smy, 3, 0xF800);
}
//--------------------------------------------
// ANALOG CLOCK Draw minute hands
//--------------------------------------------
void DrawMinuteHand(int mm, int ss, uint8_t smx, uint8_t smy, uint32_t Kleur)
{
 if(mm < 0) mm = 59;
 if(ss <0 ) ss = 59;  
 uint8_t ycorr = 0, xcorr = 0;
 float mdeg = mm * 6; //+ ss * 6 * 0.01666667;                                                // 0-59 -> 0-360 - includes not seconds
 uint16_t omx = cos((mdeg-90)*0.0174532925) * 84 + smx;    
 uint16_t omy = sin((mdeg-90)*0.0174532925) * 84 + smy;
// sprintf(sptext,"MINUUT: mm:%d ss:%d ohx:%d ohy:%d ",mm,ss,omx,omy ); Tekstprintln(sptext);

 if(mm>7  && mm <22) {xcorr = 0; ycorr = 2;} 
 if(mm>21 && mm <38) {xcorr = 2; ycorr = 0;}    
 if(mm>37 && mm <52) {xcorr = 0; ycorr = 2;} 
 if(mm>51 || mm <8)  {xcorr = 2; ycorr = 0;}                                 
 tft.drawLine(omx - xcorr, omy - ycorr, smx - xcorr, smy - ycorr, Kleur);  
 tft.drawLine(omx,         omy,         smx,         smy,         Kleur); 
 tft.drawLine(omx + xcorr, omy + ycorr, smx + xcorr, smy + ycorr, Kleur);
  
}
//--------------------------------------------
// ANALOG CLOCK Draw hour hands
//--------------------------------------------
void DrawHourHand(uint8_t hh, int mm, uint8_t smx, uint8_t smy, uint32_t Kleur)
{
 uint8_t ycorr = 0, xcorr = 0;
 float hdeg = hh%12 * 30 + mm * 0.5;                                                          // 0-11 -> 0-360 - includes minutes
 uint16_t  ohx = cos((hdeg-90)*0.0174532925) * 62 + smx;    
 uint16_t  ohy = sin((hdeg-90)*0.0174532925) * 62 + smy;
// sprintf(sptext,"   UUR: hh:%d mm:%d ohx:%d ohy:%d ",hh,mm,ohx,ohy ); Tekstprintln(sptext);
 
 if(hh>1  && hh <5) {xcorr = 2; ycorr = 0;} 
 if(hh>4  && hh <8) {xcorr = 0; ycorr = 2;}    
 if(hh>7  && hh <11) {xcorr = 2; ycorr = 0;} 
 if(hh>10 || hh <2)  {xcorr = 0; ycorr = 2;}       
 tft.drawLine(ohx-xcorr, ohy-ycorr, smx-ycorr, smy-ycorr, Kleur); 
 tft.drawLine(ohx,       ohy,       smx, smy, Kleur);
 tft.drawLine(ohx+xcorr, ohy+ycorr, smx+xcorr, smy+xcorr, Kleur);     
}
//--------------------------------------------                                                //
//  DISPLAY Init display
//--------------------------------------------
void InitDisplay(void)
{
 InitScreenILI9341();
}

//--------------------------------------------                                                //
//  DISPLAY
//  Print characters on the display with black background
//--------------------------------------------
void ClearScreen()
{
 ClearScreenILI9341(void);
}

//--------------------------------------------                                                //
//  DISPLAY
//  Print characters on the display with black background
//--------------------------------------------
void ShowChars(void)
{
 ShowCharsILI9341();
}
//--------------------------------------------                                                //
//  DISPLAY
//  Do screen update called every second
//--------------------------------------------
void ScreenSecondProcess()
{
 if (Mem.DisplayChoice == DIGITAL)  PrintTimeInScreen();                                    // Show the digital time in the screen
 if (Mem.DisplayChoice == ANALOOG)  DrawAnalogeClockHands();                                // Show the analogue time in the screen
}
//--------------------------------------------                                                //
//  DISPLAY
//  Setup analogue clock display
//--------------------------------------------
void AnalogClockSsetup()
{
 AnalogClockSetupILI9341();
}
//--------------------------------------------                                                //
//  DISPLAY Init display
//--------------------------------------------
void InitDisplay()
{
InitScreenILI9341();
}

//--------------------------------------------                                                //
//  DISPLAY
//  Clear the display
//--------------------------------------------
void ClearScreen()
{
 ClearScreenILI9341();
}
//--------------------------------------------
//  DISPLAY Set intensity backlight (0 -255)
//--------------------------------------------
void SetBackLight(byte intensity)
{ 
// analogWrite(TFT_LED, intensity); 
 ledcWrite(0, intensity);   //ledChannel, dutyCycle);
}
//--------------------------------------------                                                //
//  DISPLAY
//  Print characters on the display with black background
//--------------------------------------------
void ShowChars(void)
{
 ShowCharsILI9341();
}
//--------------------------------------------                                                //
//  DISPLAY
//  Do screen update called every second
//--------------------------------------------
void ScreenSecondProcess()
{
                                      #ifdef ILI9341
   if (Mem.DisplayChoice == DIGITAL)  PrintTimeInScreen();                                    // Show the digital time in the screen
   if (Mem.DisplayChoice == ANALOOG)  DrawAnalogeClockHands();                                // Show the analogue time in the screen
                                      #endif // ILI9341 
}
//--------------------------------------------                                                //
//  DISPLAY
//  Setup analogue clock display
//--------------------------------------------
void AnalogClockSsetup()
{
 AnalogClockSetupILI9341();
}
