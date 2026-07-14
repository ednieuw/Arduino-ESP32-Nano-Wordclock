//
// WordClock.h - all WORDCLOCK-specific code in one file.
// ---------------------------------------------------------------------------
// Included once from the .ino when WORDCLOCK is selected. Contains, in order:
//   1. Display layer  (was Display_WordClock.h, -> #includes ClockFaces.h)
//   2. Colour picker  HTML page  ColourPick_html[]   (was ColourPick.h)
//   3. Settings menu  HTML page  menu_html[]         (was MenuPageW.h)
// ClockFaces.h is kept separate (large static LED-layout data tables).
// ---------------------------------------------------------------------------

// ===== 1. DISPLAY LAYER =====================================================
//
// Display_WordClock.h - WordClock variant display layer.
// ---------------------------------------------------------------------------
// Included from the .ino only when WORDCLOCK is selected (any non-FIBONACCI
// display type at the top of the sketch). Contains the full word-clock display
// logic (formerly ClockLogic.h) and #includes ClockFaces.h for the LED face
// tables. The variant-agnostic .ino only ever calls Displaytime().
// ---------------------------------------------------------------------------

#include "ClockFaces.h"

extern struct EEPROMstorage Mem;
extern void Displaytime(void);

// WordClock colour schemes stored in Mem.DisplayChoice (Q1..Q9).
// Q0 = DEFAULTCOLOUR (in the .ino, shared); LASTITEM is derived below from QchoiceNames.
const byte HOURLYCOLOUR  = 1;
const byte WHITECOLOR    = 2;
const byte OWNCOLOUR     = 3;
const byte OWNHETISWAS   = 4;
const byte WHEELCOLOR    = 5;
const byte DIGITAL       = 6;
const byte HOURHETISWAS  = 7;
const byte RAINBOW       = 8;
const byte FASTRAINBOW   = 9;          // Like RAINBOW but colour steps 4x faster per second

// Build name string (shown in version/log). String literals sit on their own
// line - keeping them off the #if line avoids "token not valid in preprocessor".
constexpr const char* CLOCK_VARIANT =
  #if defined(NL144CLOCK)
    "NL144CLOCK"
  #elif defined(FOURLANGUAGECLOCK)
    "FOURLANGUAGECLOCK"
  #elif defined(NL92CLOCK)
    "NL92CLOCK"
  #elif defined(NLM1M2M3M4L161)
    "NLM1M2M3M4L161"
  #elif defined(DE144CLOCK)
    "DE144CLOCK"
  #elif defined(NLM1M2M3M4L94)
    "NLM1M2M3M4L94"
  #elif defined(NLM1M2M3M4L114)
    "NLM1M2M3M4L114"
  #elif defined(NLM1M2M3M4L144)
    "NLM1M2M3M4L144"
  #elif defined(NLM1M2M3M4L256)
    "NLM1M2M3M4L256"
  #elif defined(NLM1M2M3M4L294)
    "NLM1M2M3M4L294"
  #elif defined(VIERTALENKLOK)
    "VIERTALENKLOK"
  #else
    "UNKNOWN"
  #endif
;

// === WordClock display colour state + digital-display digits (moved from the .ino) ===
uint32_t MINColor      = chromeyellow;
uint32_t SECColor      = chromeyellow;
  
                                             #if defined FOURLANGUAGECLOCK || defined VIERTALENKLOK
uint32_t DefaultColor  = white;
uint32_t LetterColor   = white;
                                             #else
uint32_t DefaultColor  = chromeyellow;
uint32_t LetterColor   = chromeyellow;
                                             #endif //FOURLANGUAGECLOCK
uint32_t UKLetterColor = green;
uint32_t UKMINColor    = UKLetterColor;
uint32_t UKSECColor    = UKLetterColor;
uint32_t DELetterColor = red;
uint32_t DEMINColor    = DELetterColor;
uint32_t DESECColor    = DELetterColor;
uint32_t FRLetterColor = yellow;
uint32_t FRMINColor    = FRLetterColor;
uint32_t FRSECColor    = FRLetterColor;
uint32_t UKDefaultColor= UKLetterColor;
uint32_t DEDefaultColor= DELetterColor;
uint32_t FRDefaultColor= FRLetterColor;

uint32_t OwnColour     = 0X002345DD;        // Blueish
uint32_t WheelColor    = blue;
uint32_t HourColor[] ={  white,     darkviolet, cyberyellow, capri,         orange,        apple,
                         greenblue, cerulean,   edviolet,    cadmiumyellow, green,         edamaranth,
                         red,       yellow,     coquelicot,  pink,          apple,         hotmagenta,
                         green,     orangered,  brightgreen, dodgerblue,    screamingreen, blue,
                         white,     darkviolet, chromeyellow };     

// Definition of the digits 0 - 9, 3 wide, 5 high. 
// 10111
// 10101
// 11101 turn your head 90 degrees left and you see a TWO 
const byte PROGMEM Getal[10][3][5]  = { 
                     { {1, 1, 1, 1, 1}, {1, 0, 0, 0, 1}, {1, 1, 1, 1, 1} },  //0
                     { {1, 0, 0, 0, 1}, {1, 1, 1, 1, 1}, {0, 0, 0, 0, 1} },  //1
                     { {1, 0, 1, 1, 1}, {1, 0, 1, 0, 1}, {1, 1, 1, 0, 1} },  //2
                     { {1, 0, 1, 0, 1}, {1, 0, 1, 0, 1}, {1, 1, 1, 1, 1} },  //3
                     { {1, 1, 1, 0, 0}, {0, 0, 1, 0, 0}, {1, 1, 1, 1, 1} },  //4
                     { {1, 1, 1, 0, 1}, {1, 0, 1, 0, 1}, {1, 0, 1, 1, 1} },  //5
//                   { {1, 1, 1, 0, 1}, {1, 0, 1, 0, 1}, {1, 0, 0, 1, 1} },  //5 An other 5
                     { {1, 1, 1, 1, 1}, {0, 0, 1, 0, 1}, {0, 0, 1, 1, 1} },  //6
                     { {1, 1, 0, 0, 0}, {1, 0, 0, 0, 0}, {1, 1, 1, 1, 1} },  //7
                     { {1, 1, 1, 1, 1}, {1, 0, 1, 0, 1}, {1, 1, 1, 1, 1} },  //8
                     { {1, 1, 1, 0, 1}, {1, 0, 1, 0, 1}, {1, 1, 1, 1, 1} }   //9                   
                     }; 

       
                                                                                              // Names shown by the 'Q' command (Q0..Q9) - word-clock colour schemes.
const char* const QchoiceNames[] =
  { "Default yellow colour", "Hourly colour", "All white", "All Own colour",
    "Own colour, HETISWAS changing", "Wheel colour", "Digital display",
    "Hourly colour, HETISWAS changing", "Rainbow colour", "Fast colour" };
const byte LASTITEM = sizeof(QchoiceNames) / sizeof(QchoiceNames[0]) - 1;                     // highest valid Q index (Q0..Q9)




//--------------------------------------------                                                //
// DISPLAY Place digits 0 - 9 in Matrix display
// First row and column = 0, PosX,PosY is left top position of 3x5 digit
// Calculate position LED #define MATRIX_WIDTH 12 #define MATRIX_HEIGHT 12
//--------------------------------------------
void Zet_Pixel(byte Cijfer,byte Pos_X, byte Pos_Y) 
{ 
 uint32_t LEDnum;
 for(int i=0;i<3;i++)
  {  
   for(int j=0;j<5;j++)
   {
    int c = pgm_read_byte_near ( &Getal[Cijfer][i][j]);                                       // Reading a byte of data stored in program memory (PROGMEM area) at a specified address.
    if ( c )                                                                                  // if Digit == 1 then turn that light on
     {                                                                                        // Serial.print(strip.getPixelColor(LEDnum) & 0X00FFFFFF,HEX); Serial.print(" ");
      if((Pos_Y+j)%2) LEDnum =((MATRIX_WIDTH-1) - (Pos_X + i) + (Pos_Y + j) * (MATRIX_HEIGHT));
      else            LEDnum =                    (Pos_X + i) + (Pos_Y + j) * (MATRIX_HEIGHT); 
      ColorLed(LEDnum, (uint32_t)(LetterColor));
     }
   }
  }
}
//--------------------------------------------                                                //
// DISPLAY Time in four digits in display
//--------------------------------------------
void TimePlaceDigit(byte uur, byte minuut)
{   
 Zet_Pixel(    uur / 10, 2, 1);  Zet_Pixel(    uur % 10, 7, 1);
 Zet_Pixel( minuut / 10, 2, 7);  Zet_Pixel( minuut % 10, 7, 7);
}

//--------------------------------------------                                                //
// CLOCK Set second colour
// Set the colour per second of 'IS' and 'WAS'
//---------------------------------------------
void SetSecondColour(void)
{
                             #if defined FOURLANGUAGECLOCK || defined VIERTALENKLOK
  return;                                                                                     // 4-language clock uses SetColours() once per minute
                             #endif  //FOURLANGUAGECLOCK    
  static byte kleur = 0;                                                                      // Colour increases per second in RAINBOW
  MINColor = FuncCRGBW(15 + timeinfo.tm_min * 4, 255 - timeinfo.tm_min * 4, 0, 0); 
  SECColor = FuncCRGBW(15 + timeinfo.tm_sec * 4, 255 - timeinfo.tm_sec * 4, 0, 0 );   
  
  #if defined(NLEDSOFT) || defined(NLM1M2M3M4)
  Mem.OwnColour = Mem.ColourNL;
                            #elif defined(UK144CLOCK)                                          // English display for 12 x 12 Front
  Mem.OwnColour = Mem.ColourUK;
                            #elif defined(FR144CLOCK)                                          // French display for 12 x 12 Front
  Mem.OwnColour = Mem.ColourFR;
                            #elif defined(DE144CLOCK)                                          // German display for 12 x 12 Front
  Mem.OwnColour = Mem.ColourDE;
                            #endif
                                                                                               // Light up  IS or WAS with the proper colour  
 switch (Mem.DisplayChoice)
  {
   case DEFAULTCOLOUR: LetterColor = DefaultColor;                                     break; // Yellow text with changing MIN and SEC  
   case HOURLYCOLOUR : LetterColor = MINColor = SECColor = HourColor[timeinfo.tm_hour];break; // A colour every hour, not changing MIN and SEC
   case WHITECOLOR   : LetterColor = MINColor = SECColor = white;                      break; // All white
   case OWNCOLOUR    : LetterColor = MINColor = SECColor = Mem.OwnColour;              break; // Own colour
   case OWNHETISWAS  : LetterColor = Mem.OwnColour;                                    break; // Own colour except HET IS WAS  
   case WHEELCOLOR   : LetterColor = MINColor = SECColor = Wheel((timeinfo.tm_min*4)); break; // Colour of all letters changes per minute
   case DIGITAL      : MINColor = SECColor = Mem.DimmedLetter;                         break; // Digital display of time. No IS WAS turn color off in display
   case HOURHETISWAS : LetterColor = HourColor[timeinfo.tm_hour];                      break; // A colour every hour changing MIN and SEC
   case RAINBOW      : LetterColor = MINColor = SECColor = Wheel(kleur++);             break; // Colour of all letters changes per second 
   case FASTRAINBOW  : LetterColor = MINColor = SECColor = Wheel(kleur+=4);            break; // Colour of all letters changes 4x faster per second
  }
 if(Mem.HetIsWasOff){MINColor = SECColor = Mem.DimmedLetter;}                                 // If HET IS WAS is turned off in menu
 NoTextInLeds  = true;                                                                        // Flag to control printing of the text IS en WAS in serial
                            #if defined(NLEDSOFT) || defined(NLM1M2M3M4) 
 if(Is) {IS;}  else {WAS;} 
                            #elif defined(UK144CLOCK)                                          // English display for 12 x 12 Front
 if(Is) {ISUK;} else {WASUK;}    
                            #elif defined(FR144CLOCK)                                          // French display for 12 x 12 Front
 if(Is) {EST;} else {ETAIT;}    
                            #elif defined(DE144CLOCK)                                          // German display for 12 x 12 Front
  if(Is) {IST;} else {WAR;}    
                            #endif
 if (Mem.DisplayChoice == RAINBOW || Mem.DisplayChoice == FASTRAINBOW) Displaytime();         // RAINBOW/FASTRAINBOW: refresh the whole display every second
 NoTextInLeds  = false;                                                                       // Flag to control printing of the text IS en WAS in serial
 ShowLeds();                                                                                  // Updating IS and WAS with ShowLeds is done here to avoid updating all letters every second with Displaytime function
 
 // Serial.print("   SecColor: ");    Serial.println(SECColor,HEX);  
 // Serial.print("   MinColor: ");    Serial.println(MINColor,HEX);  
 // Serial.print("LetterColor: ");    Serial.println(LetterColor,HEX);         
}

                     #if defined FOURLANGUAGECLOCK || defined VIERTALENKLOK
//--------------------------------------------                                                //
// CLOCK Set second for 4 language clock
//--------------------------------------------
void SetColours(void)
{  
 static byte kleur = 0;                                                                       //  color increases per second in RAINBOW 
 switch (Mem.DisplayChoice)
  {
   case DEFAULTCOLOUR: LetterColor   = DefaultColor; 
                       UKLetterColor = UKDefaultColor;
                       DELetterColor = DEDefaultColor;
                       FRLetterColor = FRDefaultColor;                      break;            // HET IS WAS changing   
   case HOURHETISWAS :
   case HOURLYCOLOUR : LetterColor   = HourColor[timeinfo.tm_hour];
                       UKLetterColor = HourColor[(timeinfo.tm_hour+1)%24];
                       DELetterColor = HourColor[(timeinfo.tm_hour+2)%24];
                       FRLetterColor = HourColor[(timeinfo.tm_hour+3)%24];  break;            // A colour every hour   
   case WHITECOLOR   : LetterColor   = white; 
                       UKLetterColor = white;
                       DELetterColor = white;
                       FRLetterColor = white;                               break;            // All white
   case OWNCOLOUR    :  
   case OWNHETISWAS  : LetterColor   = Mem.ColourNL; 
                       UKLetterColor = Mem.ColourUK;
                       DELetterColor = Mem.ColourDE;
                       FRLetterColor = Mem.ColourFR;                       break;            // Own colour HET IS WAS changing  
   case WHEELCOLOR   : LetterColor   = Wheel((timeinfo.tm_min*4));       
                       UKLetterColor = Wheel((60 +timeinfo.tm_min*4));
                       DELetterColor = Wheel((120+timeinfo.tm_min*4));
                       FRLetterColor = Wheel((180+timeinfo.tm_min*4));      break;            // Colour of all letters changes per second
   case RAINBOW      : LetterColor   = Wheel((     kleur++));       
                       UKLetterColor = Wheel((60 + kleur));
                       DELetterColor = Wheel((120+ kleur));
                       FRLetterColor = Wheel((180+ kleur));                 break;            // Colour of all letters changes per second
   case FASTRAINBOW  : LetterColor   = Wheel((     kleur+=4));
                       UKLetterColor = Wheel((60 + kleur));
                       DELetterColor = Wheel((120+ kleur));
                       FRLetterColor = Wheel((180+ kleur));                 break;            // Colour of all letters changes 4x faster
   case DIGITAL      :                                                      break;            // LetterColor = Mem.OwnColour;                 break;            // digital display of time. No IS WAS turn color off in display
  }
if (Mem.HetIsWasOff)
      { 
        MINColor   = SECColor   = Mem.DimmedLetter;      
        UKMINColor = UKSECColor = Mem.DimmedLetter;
        DEMINColor = DESECColor = Mem.DimmedLetter;
        FRMINColor = FRSECColor = Mem.DimmedLetter;         
        }                                                                                      // If HET IS WAS is turned off in menu
 else {
        MINColor   = SECColor   = LetterColor;
        UKMINColor = UKSECColor = UKLetterColor;
        DEMINColor = DESECColor = DELetterColor;
        FRMINColor = FRSECColor = FRLetterColor;   
     }

}  

                   #endif  //FOURLANGUAGECLOCK  VIERTALENKLOK    

//------------------------------------------------------------------------------
// CLOCK Self test sequence
//------------------------------------------------------------------------------

void SelftestFlash(int Delaymsec)
{
 ShowLeds();
 if (Zelftest) delay(Delaymsec);                                                             // Skip delay if selftest was stopped
 LedsOff();
 CheckDevices();                                                                              // Check for input from input devices
}

void Selftest(int Delayms)
{ 
 GetTijd(true);                                                                                  // Prints time in Serial monitor
 LedsOff(); 
                      #if defined NLM1M2M3M4
 HET;   SelftestFlash(Delayms); IS;    SelftestFlash(Delayms); MVIJF; SelftestFlash(Delayms); MTIEN;   SelftestFlash(Delayms); MVOOR;  SelftestFlash(Delayms); OVER;   SelftestFlash(Delayms);
 KWART; SelftestFlash(Delayms); HALF;  SelftestFlash(Delayms); OVER2; SelftestFlash(Delayms); VOOR;    SelftestFlash(Delayms); EEN;    SelftestFlash(Delayms); TWEE;   SelftestFlash(Delayms);  
 DRIE;  SelftestFlash(Delayms); VIER;  SelftestFlash(Delayms); VIJF;  SelftestFlash(Delayms); ZES;     SelftestFlash(Delayms); ZEVEN;  SelftestFlash(Delayms); ACHT;   SelftestFlash(Delayms); 
 NEGEN; SelftestFlash(Delayms); TIEN;  SelftestFlash(Delayms); ELF;   SelftestFlash(Delayms); TWAALF;  SelftestFlash(Delayms); UUR;    SelftestFlash(Delayms); 
 MIN1;  SelftestFlash(Delayms); MIN2;  SelftestFlash(Delayms); MIN3;  SelftestFlash(Delayms); MIN4;    SelftestFlash(Delayms);
                      #endif  //NLM1M2M3M4
                      #ifdef NL144CLOCK                    
 HET;   SelftestFlash(Delayms); IS;    SelftestFlash(Delayms); WAS;    SelftestFlash(Delayms); PRECIES; SelftestFlash(Delayms); MTIEN;  SelftestFlash(Delayms);  MVIJF; SelftestFlash(Delayms);    
 KWART; SelftestFlash(Delayms); VOOR;  SelftestFlash(Delayms); OVER;   SelftestFlash(Delayms); HALF;    SelftestFlash(Delayms); MIDDER; SelftestFlash(Delayms);  VIJF;  SelftestFlash(Delayms);
 TWEE;  SelftestFlash(Delayms); EEN;   SelftestFlash(Delayms); VIER;   SelftestFlash(Delayms); TIEN;    SelftestFlash(Delayms); TWAALF; SelftestFlash(Delayms);  DRIE;  SelftestFlash(Delayms);
 NEGEN; SelftestFlash(Delayms); NACHT; SelftestFlash(Delayms); ACHT;   SelftestFlash(Delayms); ZES;     SelftestFlash(Delayms); ZEVEN;  SelftestFlash(Delayms);  ELF;   SelftestFlash(Delayms); 
 NOEN;  SelftestFlash(Delayms); UUR;   SelftestFlash(Delayms); EDSOFT; SelftestFlash(Delayms);
                      #endif  //NL144CLOCK
                      #ifdef NL92CLOCK                    
 HET;   SelftestFlash(Delayms); IS;    SelftestFlash(Delayms); WAS;    SelftestFlash(Delayms); PRECIES; SelftestFlash(Delayms); MTIEN;  SelftestFlash(Delayms);  MVIJF; SelftestFlash(Delayms);    
 KWART; SelftestFlash(Delayms); VOOR;  SelftestFlash(Delayms); OVER;   SelftestFlash(Delayms); HALF;    SelftestFlash(Delayms);  VIJF;  SelftestFlash(Delayms);
 TWEE;  SelftestFlash(Delayms); EEN;   SelftestFlash(Delayms); VIER;   SelftestFlash(Delayms); TIEN;    SelftestFlash(Delayms); TWAALF; SelftestFlash(Delayms);  DRIE;  SelftestFlash(Delayms);
 NEGEN; SelftestFlash(Delayms); ACHT;  SelftestFlash(Delayms); ZES;    SelftestFlash(Delayms); ZEVEN;   SelftestFlash(Delayms);  ELF;   SelftestFlash(Delayms); 
 UUR;   SelftestFlash(Delayms); //EDSOFT; SelftestFlash(Delayms);
                      #endif  //NL92CLOCK
                      #ifdef DE144CLOCK                    
 ES;    SelftestFlash(Delayms);   IST;     SelftestFlash(Delayms); WAR;    SelftestFlash(Delayms); GENAU; SelftestFlash(Delayms); MZEHN; SelftestFlash(Delayms);  MFUNF; SelftestFlash(Delayms);    
 VIERTEL; SelftestFlash(Delayms); ZWANZIG; SelftestFlash(Delayms); KURZ;   SelftestFlash(Delayms); VOR;   SelftestFlash(Delayms); NACH;  SelftestFlash(Delayms);  HALB;  SelftestFlash(Delayms);
 FUNF;  SelftestFlash(Delayms);   EINS;    SelftestFlash(Delayms); VIERDE; SelftestFlash(Delayms); ZEHN;  SelftestFlash(Delayms); ZWOLF; SelftestFlash(Delayms);  DREI;  SelftestFlash(Delayms);
 NEUN;  SelftestFlash(Delayms);   ACHTDE;  SelftestFlash(Delayms); SECHS;  SelftestFlash(Delayms); SIEBEN;SelftestFlash(Delayms); ZWEI;  SelftestFlash(Delayms);  ELFDE; SelftestFlash(Delayms); 
 UHR;   SelftestFlash(Delayms); 
                      #endif  //DE144CLOCK 

                      #ifdef FOURLANGUAGECLOCK
 // Nederlands (NL)
                      #ifdef NL
 HET;    SelftestFlash(Delayms); IS;      SelftestFlash(Delayms); WAS;    SelftestFlash(Delayms); PRECIES; SelftestFlash(Delayms);
 MTIEN;  SelftestFlash(Delayms); MVIJF;   SelftestFlash(Delayms); KWART;  SelftestFlash(Delayms); VOOR;    SelftestFlash(Delayms);
 OVER;   SelftestFlash(Delayms); HALF;    SelftestFlash(Delayms); MIDDER; SelftestFlash(Delayms); VIJF;    SelftestFlash(Delayms);
 TWEE;   SelftestFlash(Delayms); EEN;     SelftestFlash(Delayms); VIER;   SelftestFlash(Delayms); TIEN;    SelftestFlash(Delayms);
 TWAALF; SelftestFlash(Delayms); DRIE;    SelftestFlash(Delayms); NEGEN;  SelftestFlash(Delayms); ACHT;    SelftestFlash(Delayms);
 NACHT;  SelftestFlash(Delayms); ZES;     SelftestFlash(Delayms); ZEVEN;  SelftestFlash(Delayms); ELF;     SelftestFlash(Delayms);
 NOEN;   SelftestFlash(Delayms); UUR;     SelftestFlash(Delayms);
 #endif //NL

 // English (UK)
                      #ifdef UK
 IT;     SelftestFlash(Delayms); ISUK;    SelftestFlash(Delayms); WASUK;   SelftestFlash(Delayms); EXACTUK; SelftestFlash(Delayms);
 HALFUK; SelftestFlash(Delayms); TWENTY;  SelftestFlash(Delayms); MFIVE;   SelftestFlash(Delayms); QUARTER; SelftestFlash(Delayms);
 MTEN;   SelftestFlash(Delayms); PAST;    SelftestFlash(Delayms); TO;      SelftestFlash(Delayms); SIXUK;   SelftestFlash(Delayms);
 TWO;    SelftestFlash(Delayms); FIVE;    SelftestFlash(Delayms); TWELVE;  SelftestFlash(Delayms); TEN;     SelftestFlash(Delayms);
 ELEVEN; SelftestFlash(Delayms); FOUR;    SelftestFlash(Delayms); NINE;    SelftestFlash(Delayms); THREE;   SelftestFlash(Delayms);
 EIGHT;  SelftestFlash(Delayms); ONE;     SelftestFlash(Delayms); SEVEN;   SelftestFlash(Delayms); OCLOCK;  SelftestFlash(Delayms);
                      #endif //UK

 // Deutsch (DE)
                      #ifdef DE
 ES;     SelftestFlash(Delayms); IST;     SelftestFlash(Delayms); WAR;     SelftestFlash(Delayms); GENAU;   SelftestFlash(Delayms);
 MZEHN;  SelftestFlash(Delayms); MFUNF;   SelftestFlash(Delayms); VIERTEL; SelftestFlash(Delayms); ZWANZIG; SelftestFlash(Delayms);
 KURZ;   SelftestFlash(Delayms); VOR;     SelftestFlash(Delayms); NACH;    SelftestFlash(Delayms); HALB;    SelftestFlash(Delayms);
 FUNF;   SelftestFlash(Delayms); EINS;    SelftestFlash(Delayms); VIERDE;  SelftestFlash(Delayms); ZEHN;    SelftestFlash(Delayms);
 ZWOLF;  SelftestFlash(Delayms); DREI;    SelftestFlash(Delayms); NEUN;    SelftestFlash(Delayms); ACHTDE;  SelftestFlash(Delayms);
 SECHS;  SelftestFlash(Delayms); SIEBEN;  SelftestFlash(Delayms); ZWEI;    SelftestFlash(Delayms); ELFDE;   SelftestFlash(Delayms);
 UHR;    SelftestFlash(Delayms);
                      #endif //DE

 // Français (FR)
                      #ifdef FR
 IL;     SelftestFlash(Delayms); EST;     SelftestFlash(Delayms); ETAIT;   SelftestFlash(Delayms); EXACT;   SelftestFlash(Delayms);
 SIX;    SelftestFlash(Delayms); DEUX;    SelftestFlash(Delayms); TROIS;   SelftestFlash(Delayms); ONZE;    SelftestFlash(Delayms);
 QUATRE; SelftestFlash(Delayms); MINUIT;  SelftestFlash(Delayms); DIX;     SelftestFlash(Delayms); CINQ;    SelftestFlash(Delayms);
 NEUF;   SelftestFlash(Delayms); MIDI;    SelftestFlash(Delayms); HUIT;    SelftestFlash(Delayms); SEPT;    SelftestFlash(Delayms);
 UNE;    SelftestFlash(Delayms); HEURE;   SelftestFlash(Delayms); HEURES;  SelftestFlash(Delayms); ET;      SelftestFlash(Delayms);
 MOINS;  SelftestFlash(Delayms); LE;      SelftestFlash(Delayms); DEMI;    SelftestFlash(Delayms); QUART;   SelftestFlash(Delayms);
 MDIX;   SelftestFlash(Delayms); VINGT;   SelftestFlash(Delayms); MCINQ;   SelftestFlash(Delayms);
                      #endif //FR
                           #endif //FOURLANGUAGECLOCK

                           #ifdef VIERTALENKLOK
 // Nederlands (NL4)
                      #ifdef NL4
 HET;    SelftestFlash(Delayms); IS;     SelftestFlash(Delayms); MVIJF;  SelftestFlash(Delayms); MTIEN;  SelftestFlash(Delayms);
 MVOOR;  SelftestFlash(Delayms); OVER;   SelftestFlash(Delayms); KWART;  SelftestFlash(Delayms); OVER2;  SelftestFlash(Delayms);
 HALF;   SelftestFlash(Delayms); VOOR;   SelftestFlash(Delayms); EEN;    SelftestFlash(Delayms); DRIE;   SelftestFlash(Delayms);
 TWEE;   SelftestFlash(Delayms); VIER;   SelftestFlash(Delayms); VIJF;   SelftestFlash(Delayms); ZES;    SelftestFlash(Delayms);
 NEGEN;  SelftestFlash(Delayms); ZEVEN;  SelftestFlash(Delayms); ACHT;   SelftestFlash(Delayms); TIEN;   SelftestFlash(Delayms);
 ELF;    SelftestFlash(Delayms); TWAALF; SelftestFlash(Delayms); UUR;    SelftestFlash(Delayms);
 MIN1;   SelftestFlash(Delayms); MIN2;   SelftestFlash(Delayms); MIN3;   SelftestFlash(Delayms); MIN4;   SelftestFlash(Delayms);
 #endif //NL4

 // English (UK4)
                      #ifdef UK4
 IT;     SelftestFlash(Delayms); ISUK;   SelftestFlash(Delayms); EXACTUK; SelftestFlash(Delayms); HALFUK; SelftestFlash(Delayms);
 TWENTY; SelftestFlash(Delayms); MFIVE;  SelftestFlash(Delayms); QUARTER; SelftestFlash(Delayms); MTEN;   SelftestFlash(Delayms);
 PAST;   SelftestFlash(Delayms); TO;     SelftestFlash(Delayms); SIXUK;   SelftestFlash(Delayms); TWO;    SelftestFlash(Delayms);
 FIVE;   SelftestFlash(Delayms); TWELVE; SelftestFlash(Delayms); TEN;     SelftestFlash(Delayms); ELEVEN; SelftestFlash(Delayms);
 FOUR;   SelftestFlash(Delayms); NINE;   SelftestFlash(Delayms); THREE;   SelftestFlash(Delayms); EIGHT;  SelftestFlash(Delayms);
 ONE;    SelftestFlash(Delayms); SEVEN;  SelftestFlash(Delayms); OCLOCK;  SelftestFlash(Delayms);
 #endif //UK4

 // Deutsch (DE4)
                      #ifdef DE4
 ES;     SelftestFlash(Delayms); IST;    SelftestFlash(Delayms); GENAU;   SelftestFlash(Delayms); MZEHN;  SelftestFlash(Delayms);
 MFUNF;  SelftestFlash(Delayms); VIERTEL; SelftestFlash(Delayms); ZWANZIG; SelftestFlash(Delayms); KURZ;  SelftestFlash(Delayms);
 VOR;    SelftestFlash(Delayms); NACH;   SelftestFlash(Delayms); HALB;    SelftestFlash(Delayms); FUNF;   SelftestFlash(Delayms);
 EINS;   SelftestFlash(Delayms); VIERDE; SelftestFlash(Delayms); ZEHN;    SelftestFlash(Delayms); ZWOLF;  SelftestFlash(Delayms);
 DREI;   SelftestFlash(Delayms); NEUN;   SelftestFlash(Delayms); ACHTDE;  SelftestFlash(Delayms); SECHS;  SelftestFlash(Delayms);
 SIEBEN; SelftestFlash(Delayms); ZWEI;   SelftestFlash(Delayms); ELFDE;   SelftestFlash(Delayms); UHR;    SelftestFlash(Delayms);
                      #endif //DE4

 // Français (FR4)
                      #ifdef FR4
 IL;     SelftestFlash(Delayms); EST;    SelftestFlash(Delayms); EXACT;   SelftestFlash(Delayms); SIX;    SelftestFlash(Delayms);
 DEUX;   SelftestFlash(Delayms); TROIS;  SelftestFlash(Delayms); ONZE;    SelftestFlash(Delayms); QUATRE; SelftestFlash(Delayms);
 MINUIT; SelftestFlash(Delayms); DIX;    SelftestFlash(Delayms); CINQ;    SelftestFlash(Delayms); NEUF;   SelftestFlash(Delayms);
 MIDI;   SelftestFlash(Delayms); HUIT;   SelftestFlash(Delayms); SEPT;    SelftestFlash(Delayms); UNE;    SelftestFlash(Delayms);
 HEURE;  SelftestFlash(Delayms); HEURES; SelftestFlash(Delayms); ET;      SelftestFlash(Delayms); MOINS;  SelftestFlash(Delayms);
 LE;     SelftestFlash(Delayms); DEMI;   SelftestFlash(Delayms); QUART;   SelftestFlash(Delayms); MDIX;   SelftestFlash(Delayms);
 VINGT;  SelftestFlash(Delayms); MCINQ;  SelftestFlash(Delayms);
                       #endif //FR4
                             #endif //VIERTALENKLOK

 Tekstprintln("*");      
}
void Selftest(void) {Selftest(900);}
//
//--------------------------------------------                                                //
// CLOCK Say the time and load the LEDs 
// with the proper colour and intensity
//--------------------------------------------
void Displaytime(void)
{ 
 LedsOff();                                                                                  // Start by clearing the display to a known state   
 if( Mem.DisplayChoice == DIGITAL ) { TimePlaceDigit(timeinfo.tm_hour,timeinfo.tm_min); }
 else                                                                                        // If not a digital display 
   {
                     #if defined NLEDSOFT
    Dutch();                                                                                 //
                     #endif  //NLEDSOFT
                     #ifdef DE144CLOCK
    German();                                                                                 //   
                     #endif  //DE144CLOCK
                     #if defined NLM1M2M3M4
    Dutch();                                                                                 //  
                     #endif  //NLM1M2M3M4
                     #if defined FOURLANGUAGECLOCK || defined VIERTALENKLOK
    SetColours();                                                                            // Set the colours for the chosen palette   
    Dutch();
    English();
    German();
    French();
                     #endif  //FOURLANGUAGECLOCK || VIERTALENKLOK      
   }  
 ShowLeds();                                                                                  // And turn on the LEDs
}

// Select the language-specific words. Only the macros differ per clock;
// the three Blink functions below are identical for every language.
                       #ifdef DE144CLOCK
#define BLINKHOUR     UHR                  // "o'clock"
#define BLINKISWAS    ES; IST; WAR         // "it is / was"
#define BLINKTWELVE   ZWOLF                // "twelve"
                     #elif defined(UK144CLOCK)
#define BLINKHOUR     OCLOCK
#define BLINKISWAS    IT; ISUK; WASUK
#define BLINKTWELVE   TWELVE
                     #elif defined(FR144CLOCK)
#define BLINKHOUR     HEURES
#define BLINKISWAS    IL; EST; ETAIT
#define BLINKTWELVE   MIDI
                     #else                                                                       // Dutch (default)
#define BLINKHOUR     UUR
#define BLINKISWAS    HET; IS; WAS
#define BLINKTWELVE   TWAALF
                     #endif //DE144CLOCK / UK144CLOCK / FR144CLOCK
//--------------------------------------------                                                //
// CLOCK Blink the "o'clock" word (UHR / OCLOCK / HEURES / UUR)
//--------------------------------------------
void BlinkUUR(int NoofBlinks, int Delayms)
{
for (int n=0 ; n<=NoofBlinks; n++) { LedsOff(); SelftestFlash(Delayms); BLINKHOUR; SelftestFlash(Delayms); }
}
//--------------------------------------------                                                //
// CLOCK Blink the "it is / was" words
//--------------------------------------------
void BlinkHETISWAS (int NoofBlinks, int Delayms)
{
for (int n=0 ; n<=NoofBlinks; n++) { LedsOff(); SelftestFlash(Delayms); BLINKISWAS; SelftestFlash(Delayms); }
}
//--------------------------------------------                                                //
// CLOCK Blink the "twelve" word (ZWOLF / TWELVE / MIDI / TWAALF)
//--------------------------------------------
void BlinkTWAALF(int NoofBlinks, int Delayms)
{
for (int n=0 ; n<=NoofBlinks; n++) { LedsOff(); SelftestFlash(Delayms); BLINKTWELVE; SelftestFlash(Delayms); }
}
//------------------------------------------------------------------------------              //
// CLOCK Demo mode
//------------------------------------------------------------------------------
void Demomode(void)
{
 if ( millis() - msTick == 10)  SetNanoLED13(0);                                              // Turn OFF the second on pin 13
 if ( millis() - msTick >= MilliSecondValue)                                                  // Flash the onboard Pin 13 Led so we know something is happening
 {    
  msTick = millis();                                                                          // second++; 
  SetNanoLED13(100);                                                                          // Turn ON the second on pin 13
  if( ++timeinfo.tm_sec >59) { timeinfo.tm_sec = 0; }
  if( ++timeinfo.tm_min >59) { timeinfo.tm_min = 0; timeinfo.tm_sec = 0; timeinfo.tm_hour++;}
  if( timeinfo.tm_hour >24) timeinfo.tm_hour = 0;                                             // If hour is after 12 o'clock 
  DimLeds(false);
  SetSecondColour();                                                                          // Set the colour per second of 'IS' and 'WAS' 
  Displaytime(); Tekstprintln("");
  Tekstprintln("");
  SerialCheck();
 }
}
//------------------------------------------------------------------------------              //
// CLOCK Fire display on MATRIX_HEIGHT MATRIX_WIDTH
//------------------------------------------------------------------------------
void FireMode(void)
{
  Fire2023_2D();
  SerialCheck();
}
//--------------------------------------------                                                //
// FIRE
//--------------------------------------------
// ==== FIRE EFFECT SETTINGS ====
#define COOLING     90
#define SPARKING    40
#define FIRE_DELAY  50

uint8_t heat[MATRIX_HEIGHT][MATRIX_WIDTH];  // 2D heat map
//--------------------------------------------                                                //
// FIRE Utils
//--------------------------------------------
uint8_t clamp_sub(uint8_t val, uint8_t sub) { return (val > sub) ? (val - sub) : 0;}
uint8_t clamp_add(uint8_t val, uint8_t add) { return (val + add > 255) ? 255 : (val + add);}
void HeatColor(uint8_t temperature, uint8_t &r, uint8_t &g, uint8_t &b) 
{
 uint8_t t192 = (temperature * 192) / 255;
 uint8_t heatramp = (t192 & 0x3F) << 2;
 if      (t192 > 128) { r = 255;      g = 255;      b = heatramp; } 
 else if (t192 > 64)  { r = 255;      g = heatramp; b = 0;        } 
 else                 { r = heatramp; g = 0;        b = 0;        }
}

uint16_t XY(uint8_t x, uint8_t y) 
{  return (y % 2 == 0) ? (y * MATRIX_WIDTH + x) : (y * MATRIX_WIDTH + (MATRIX_WIDTH-1-x)); }  // Serpentine index mapping

//--------------------------------------------                                                //
// FIRE ==== Fire Logic ====
//--------------------------------------------
void Fire2023_2D() 
{
 for (uint8_t x = 0; x < MATRIX_WIDTH; x++)                                                   // Step 1: Cool down
     for (uint8_t y = 0; y < MATRIX_HEIGHT; y++) 
         heat[y][x] = clamp_sub(heat[y][x], random(0, ((COOLING*10) / MATRIX_HEIGHT) + 2));
 for (uint8_t x = 0; x < MATRIX_WIDTH; x++)                                                   // Step 2: Heat falls down
   {                                                 
    for (int y = 0; y < MATRIX_HEIGHT - 2; y++) 
        heat[y][x] = (heat[y + 1][x] + heat[y + 2][x] + heat[y + 2][x]) / 3;
    heat[MATRIX_HEIGHT - 2][x]=(heat[MATRIX_HEIGHT - 1][x] + heat[MATRIX_HEIGHT - 1][x]) / 2; // Safe heat fall for second-to-top row
   }
 for (uint8_t x = 0; x < MATRIX_WIDTH; x++)                                                   // Step 3: Sparking at the top
    if (random(255) < SPARKING)  
       heat[MATRIX_HEIGHT - 1][x] = clamp_add(heat[MATRIX_HEIGHT - 1][x], random(160, 255));
 for (uint8_t y = 0; y < MATRIX_HEIGHT; y++)                                                  // Step 4: Draw pixels from heat map
   {
    for (uint8_t x = 0; x < MATRIX_WIDTH; x++) 
      {
       uint8_t r, g, b;
       HeatColor(heat[y][x], r, g, b);
       LEDstrip.setPixelColor(XY(x, y), g, r, b, 0);                                          // Add W=0
      }
    }   
 LEDstrip.show();
 delay(FIRE_DELAY);
}

//--------------------------------------------                                                //
// CLOCK Dutch clock display for NL144 & NL92 clock
//--------------------------------------------
                      #if defined(NL) || defined(NL144CLOCK)  || defined (NL92CLOCK)
void Dutch(void)
{
//if(Mem.EdSoftLEDSOn) EDSOFT;                                                                // EDSOFT On or Off
HET; 
                      #if defined(NL) || defined(NL144CLOCK)                                                                                          // HET  is always on
 if (timeinfo.tm_hour == 12 && timeinfo.tm_min == 0 && random(2)==0) { IS; NOEN; return; }
 if (timeinfo.tm_hour == 00 && timeinfo.tm_min == 0 && random(2)==0) { IS; MIDDER; NACHT; return; }
                      #endif //#if defined(NL) || defined(NL144CLOCK)  
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
  case 16: IS;  KWART; OVER; break;
  case 17: 
  case 18: WAS; KWART; OVER; break;
  case 19: 
  case 20: 
  case 21: IS;  MTIEN; VOOR; HALF; break;
  case 22: 
  case 23: WAS; MTIEN; VOOR; HALF; break;
  case 24: 
  case 25: 
  case 26: IS;  MVIJF; VOOR; HALF; break;
  case 27: 
  case 28: WAS; MVIJF; VOOR; HALF; break;
  case 29: IS;  HALF; break;
  case 30: IS;  PRECIES; HALF; break;
  case 31: IS;  HALF; break;
  case 32: 
  case 33: WAS; HALF; break;
  case 34: 
  case 35: 
  case 36: IS;  MVIJF; OVER; HALF; break;
  case 37: 
  case 38: WAS; MVIJF; OVER; HALF; break;
  case 39: 
  case 40: 
  case 41: IS;  MTIEN; OVER; HALF; break;
  case 42: 
  case 43: WAS; MTIEN; OVER; HALF; break;
  case 44: 
  case 45: 
  case 46: IS;  KWART; VOOR; break;
  case 47: 
  case 48: WAS; KWART; VOOR; break;
  case 49: 
  case 50: 
  case 51: IS;  MTIEN; VOOR;  break;
  case 52: 
  case 53: WAS; MTIEN; VOOR;  break;
  case 54: 
  case 55: 
  case 56: IS;  MVIJF; VOOR; break;
  case 57: 
  case 58: WAS; MVIJF; VOOR; break;
  case 59: IS;  break;
}
//if (timeinfo.tm_hour >=0 && hour <12) digitalWrite(AMPMpin,0); else digitalWrite(AMPMpin,1);

 sayhour = timeinfo.tm_hour;
 if (timeinfo.tm_min > 18 )  sayhour = timeinfo.tm_hour+1;
 if (sayhour == 24) sayhour = 0;

switch (sayhour)
 {
  case 13:  
  case 1: EEN; break;
  case 14:
  case 2: TWEE; break;
  case 15:
  case 3: DRIE; break;
  case 16:
  case 4: VIER; break;
  case 17:
  case 5: VIJF; break;
  case 18:
  case 6: ZES; break;
  case 19:
  case 7: ZEVEN; break;
  case 20:
  case 8: ACHT; break;
  case 21:
  case 9: NEGEN; break;
  case 22:
  case 10: TIEN; break;
  case 23:
  case 11: ELF; break;
  case 0:
  case 12: TWAALF; break;
 } 
 switch (timeinfo.tm_min)
 {
  case 59: 
  case  0: 
  case  1: 
  case  2: 
  case  3: UUR;  break; 
 }
//                        #if defined(NL) || defined(NL144CLOCK) 
//   switch (timeinfo.tm_min % 5)                                                                // The minutes modulo 5
//  {
//   case  0: break;
//   case  1: MIN1; break;
//   case  2: MIN1; MIN2; break;
//   case  3: MIN1; MIN2; MIN3; break;
//   case  4: MIN1; MIN2; MIN3; MIN4;  
//  }  
//                       #endif // defined(NL) || defined(NL144CLOCK) 
}

                                    #endif //#if defined(NL) || defined(NL144CLOCK)  || defined (NL92CLOCK)

                                    #if defined NLM1M2M3M4 
                                    //|| defined VIERTALENKLOK || defined NL4
//--------------------------------------------                                                //
// CLOCK Dutch clock display for NLM1M2M3M4 clock
//--------------------------------------------
void Dutch(void)
{
if(Mem.EdSoftLEDSOn) EDSOFT;     
HET;                                                                                          // HET  is always on
switch (timeinfo.tm_min)
 {
  case  0: 
  case  1:  
  case  2: 
  case  3: 
  case  4: IS;  break;                                                                        // HET IS .. UUR
  case  5: 
  case  6: 
  case  7: 
  case  8: 
  case  9: IS;  MVIJF; OVER; break;                                                           // HET IS VIJF OVER .. 
  case 10: 
  case 11: 
  case 12: 
  case 13: 
  case 14: IS;  MTIEN; OVER; break;                                                           // Et cetera
  case 15: 
  case 16: 
  case 17: 
  case 18: 
  case 19: IS;  KWART; OVER2; break;
  case 20: 
  case 21: 
  case 22: 
  case 23: 
  case 24: IS;  MTIEN; MVOOR; HALF; break;
  case 25: 
  case 26: 
  case 27: 
  case 28:
  case 29: IS;  MVIJF; MVOOR; HALF; break;
  case 30:
  case 31:
  case 32:
  case 33:
  case 34: IS;  HALF; break;
  case 35: 
  case 36: 
  case 37:
  case 38:
  case 39: IS;  MVIJF; OVER; HALF; break;
  case 40: 
  case 41:
  case 42:
  case 43:
  case 44: IS;  MTIEN; OVER; HALF; break;
  case 45: 
  case 46: 
  case 47:
  case 48:
  case 49: IS;  KWART; VOOR; break;
  case 50:
  case 51:
  case 52:
  case 53:
  case 54: IS;  MTIEN; VOOR;  break;
  case 55:
  case 56: 
  case 57: 
  case 58:
  case 59: IS;  MVIJF; VOOR; break;
}
//if (timeinfo.tm_hour >=0 && hour <12) digitalWrite(AMPMpin,0); else digitalWrite(AMPMpin,1);

 sayhour = timeinfo.tm_hour;
 if (timeinfo.tm_min > 19 )  sayhour = timeinfo.tm_hour+1;
 if (sayhour == 24) sayhour = 0;

switch (sayhour)
 {
  case 13:  
  case 1: EEN; break;
  case 14:
  case 2: TWEE; break;
  case 15:
  case 3: DRIE; break;
  case 16:
  case 4: VIER; break;
  case 17:
  case 5: VIJF; break;
  case 18:
  case 6: ZES; break;
  case 19:
  case 7: ZEVEN; break;
  case 20:
  case 8: ACHT; break;
  case 21:
  case 9: NEGEN; break;
  case 22:
  case 10: TIEN; break;
  case 23:
  case 11: ELF; break;
  case 0:
  case 12: TWAALF; break;
 } 
 switch (timeinfo.tm_min)
 {
//  case 59: 
  case  0: 
  case  1: 
  case  2: 
  case  3: 
  case  4: UUR;  break; 
 }

 switch (timeinfo.tm_min % 5)                                                                // The minutes modulo 5
 {
  case  0: break;
  case  1: MIN1; break;
  case  2: MIN1; MIN2; break;
  case  3: MIN1; MIN2; MIN3; break;
  case  4: MIN1; MIN2; MIN3; MIN4;  
 }
}
                     #endif  //NLM1M2M3M4

                     #ifdef UK
//--------------------------------------------                                                //
// CLOCK English clock display
//--------------------------------------------
void English(void)
{
 IT;                                                                                          // IT is always on
// if (timeinfo.tm_hour == 00 && timeinfo.tm_min == 0 && random(2)==0) { ISUK; MID; NIGHT; return; } 
 switch (timeinfo.tm_min)
 {
  case  0: ISUK;  EXACTUK; break;
  case  1: ISUK;  break;
  case  2: 
  case  3: WASUK; break;
  case  4: 
  case  5: 
  case  6: ISUK;  MFIVE; PAST; break;
  case  7: 
  case  8: WASUK; MFIVE; PAST; break;
  case  9: 
  case 10: 
  case 11: ISUK;  MTEN; PAST; break;
  case 12: 
  case 13: WASUK; MTEN; PAST; break;
  case 14: 
  case 15: 
  case 16: ISUK;  QUARTER; PAST; break;
  case 17: 
  case 18: WASUK; QUARTER; PAST; break;
  case 19: 
  case 20: 
  case 21: ISUK;  TWENTY; PAST; break;
  case 22: 
  case 23: WASUK; TWENTY; PAST; break;
  case 24: 
  case 25: 
  case 26: ISUK;  TWENTY; MFIVE; PAST; break;
  case 27: 
  case 28: WASUK; TWENTY; MFIVE; PAST; break;
  case 29: ISUK;  HALFUK; PAST; break;
  case 30: ISUK;  EXACTUK; HALFUK; PAST; break;
  case 31: ISUK;  HALFUK; PAST; break;
  case 32: 
  case 33: WASUK; HALFUK; PAST; break;
  case 34: 
  case 35: 
  case 36: ISUK;  TWENTY; MFIVE; TO; break;
  case 37: 
  case 38: WASUK; TWENTY; MFIVE; TO; break;
  case 39: 
  case 40: 
  case 41: ISUK;  TWENTY; TO; break;
  case 42: 
  case 43: WASUK; TWENTY; TO break;
  case 44: 
  case 45: 
  case 46: ISUK;  QUARTER; TO; break;
  case 47: 
  case 48: WASUK; QUARTER; TO; break;
  case 49: 
  case 50: 
  case 51: ISUK;  MTEN; TO;  break;
  case 52: 
  case 53: WASUK; MTEN; TO;  break;
  case 54: 
  case 55: 
  case 56: ISUK;  MFIVE; TO; break;
  case 57: 
  case 58: WASUK; MFIVE; TO; break;
  case 59: ISUK;  break;
}
//if (timeinfo.tm_hour >=0 && hour <12) digitalWrite(AMPMpin,0); else digitalWrite(AMPMpin,1);

 sayhour = timeinfo.tm_hour;
 if (timeinfo.tm_min > 33 ) sayhour = timeinfo.tm_hour+1;
 if (sayhour == 24) sayhour = 0;

switch (sayhour)
 {
  case 13:  
  case 1:  ONE; break;
  case 14:
  case 2:  TWO; break;
  case 15:
  case 3:  THREE; break;
  case 16:
  case 4:  FOUR; break;
  case 17:
  case 5:  FIVE; break;
  case 18:
  case 6:  SIXUK; break;
  case 19:
  case 7:  SEVEN; break;
  case 20:
  case 8:  EIGHT; break;
  case 21:
  case 9:  NINE; break;
  case 22:
  case 10: TEN; break;
  case 23:
  case 11: ELEVEN; break;
  case 0:
  case 12: TWELVE; break;
 } 
 switch (timeinfo.tm_min)
 {
  case 59: 
  case  0: 
  case  1: 
  case  2: 
  case  3: OCLOCK;  break; 
 }
}
                     #endif //UK
                     #if defined DE || defined DE144CLOCK
//--------------------------------------------                                                //
// CLOCK German clock display
//--------------------------------------------
void German(void)
{
  ES;                                                                                         // ES is always on
// if (timeinfo.tm_hour == 00 && timeinfo.tm_min == 0 && random(2)==0) {IST; MITTER; NACHTDE; return; } 
 switch (timeinfo.tm_min)
 {
  case  0: IST;  GENAU; break;
  case  1: IST; KURZ; NACH; break;
  case  2: 
  case  3: WAR; break;
  case  4: 
  case  5: 
  case  6: IST; MFUNF; NACH; break;
  case  7: 
  case  8: WAR; MFUNF; NACH; break;
  case  9: 
  case 10: 
  case 11: IST; MZEHN; NACH; break;
  case 12: 
  case 13: WAR; MZEHN; NACH; break;
  case 14: 
  case 15: 
  case 16: IST; VIERTEL; NACH; break;
  case 17: 
  case 18: WAR; VIERTEL; NACH; break;
  case 19: 
  case 20: 
  case 21: IST; MZEHN; VOR; HALB; break;
  case 22: 
  case 23: WAR; MZEHN; VOR; HALB; break;
  case 24: 
  case 25: 
  case 26: IST; MFUNF; VOR; HALB; break;
  case 27: 
  case 28: WAR; MFUNF; VOR; HALB; break;
  case 29: IST; KURZ;  VOR; HALB; break;
  case 30: IST; GENAU; HALB; break;
  case 31: IST; KURZ;  NACH; HALB; break;
  case 32: 
  case 33: WAR; HALB; break;
  case 34: 
  case 35: 
  case 36: IST; MFUNF; NACH; HALB; break;
  case 37: 
  case 38: WAR; MFUNF; NACH; HALB; break;
  case 39: 
  case 40: 
  case 41: IST; MZEHN; NACH; HALB; break;
  case 42: 
  case 43: WAR; MZEHN; NACH; HALB; break;
  case 44: 
  case 45: 
  case 46: IST; VIERTEL; VOR; break;
  case 47: 
  case 48: WAR; VIERTEL; VOR; break;
  case 49: 
  case 50: 
  case 51: IST; MZEHN; VOR;  break;
  case 52: 
  case 53: WAR; MZEHN; VOR;  break;
  case 54: 
  case 55: 
  case 56: IST; MFUNF; VOR; break;
  case 57: 
  case 58: WAR; MFUNF; VOR; break;
  case 59: IST;  break;
}
//if (timeinfo.tm_hour >=0 && hour <12) digitalWrite(AMPMpin,0); else digitalWrite(AMPMpin,1);

 sayhour = timeinfo.tm_hour;
 if (timeinfo.tm_min > 18 ) sayhour = timeinfo.tm_hour+1;
 if (sayhour == 24) sayhour = 0;

switch (sayhour)
 {
  case 13:  
  case 1: EINS; break;
  case 14:
  case 2: ZWEI; break;
  case 15:
  case 3: DREI; break;
  case 16:
  case 4: VIERDE; break;
  case 17:
  case 5: FUNF; break;
  case 18:
  case 6: SECHS; break;
  case 19:
  case 7: SIEBEN; break;
  case 20:
  case 8: ACHTDE; break;
  case 21:
  case 9: NEUN; break;
  case 22:
  case 10: ZEHN; break;
  case 23:
  case 11: ELFDE; break;
  case 0:
  case 12: ZWOLF; break;
 } 
 switch (timeinfo.tm_min)
 {
  case 59: 
  case  0: 
  case  1: 
  case  2: 
  case  3: UHR;  break; 
 }
}
                     #endif //DE
                     #ifdef FR
//--------------------------------------------                                                //
// CLOCK French clock display
//--------------------------------------------
void French(void)
{
 IL;                                                                                          // IL is always on
 switch (timeinfo.tm_min)
 {
  case  0: EST;   EXACT; DITLEHEURE; break;
  case  1: EST;   DITLEHEURE; break;
  case  2: 
  case  3: ETAIT; DITLEHEURE; break;
  case  4: 
  case  5: 
  case  6: EST;   DITLEHEURE; MCINQ; break;
  case  7: 
  case  8: ETAIT; DITLEHEURE; MCINQ; break;
  case  9: 
  case 10: 
  case 11: EST;   DITLEHEURE; MDIX;  break;
  case 12: 
  case 13: ETAIT; DITLEHEURE; MDIX;  break;
  case 14: 
  case 15: 
  case 16: EST;   DITLEHEURE; ET; QUART; break;
  case 17: 
  case 18: ETAIT; DITLEHEURE; ET; QUART; break;
  case 19: 
  case 20: 
  case 21: EST;   DITLEHEURE; VINGT; break;
  case 22: 
  case 23: ETAIT; DITLEHEURE; VINGT; break;
  case 24: 
  case 25: 
  case 26: EST;   DITLEHEURE; VINGT; MCINQ; break;
  case 27: 
  case 28: ETAIT; DITLEHEURE; VINGT; MCINQ; break;
  case 29: EST;   DITLEHEURE; ET; DEMI; break;
  case 30: EST;   EXACT; DITLEHEURE;  ET; DEMI; break;
  case 31: EST;   DITLEHEURE; ET; DEMI; break;
  case 32: 
  case 33: ETAIT; DITLEHEURE; ET; DEMI; break;
  case 34: 
  case 35: 
  case 36: EST;   DITLEHEURE; MOINS; VINGT; MCINQ; break;
  case 37: 
  case 38: ETAIT; DITLEHEURE; MOINS; VINGT; MCINQ; break;
  case 39: 
  case 40: 
  case 41: EST;   DITLEHEURE; MOINS; VINGT;  break;
  case 42: 
  case 43: ETAIT; DITLEHEURE; MOINS; VINGT;  break;
  case 44: 
  case 45: 
  case 46: EST;   DITLEHEURE; MOINS; LE; QUART; break;
  case 47: 
  case 48: ETAIT; DITLEHEURE; MOINS; LE; QUART; break;
  case 49: 
  case 50: 
  case 51: EST;   DITLEHEURE; MOINS; MDIX;   break;
  case 52: 
  case 53: ETAIT; DITLEHEURE; MOINS; MDIX;   break;
  case 54: 
  case 55: 
  case 56: EST;   DITLEHEURE; MOINS; MCINQ;  break;
  case 57: 
  case 58: ETAIT; DITLEHEURE; MOINS; MCINQ;  break;
  case 59: EST;   DITLEHEURE;  break;
 }
}

void DitLeHeure(void)
{
 byte sayhour = timeinfo.tm_hour;
 if (timeinfo.tm_min > 33 ) sayhour = timeinfo.tm_hour+1;
 if (sayhour == 24) sayhour = 0;

switch (sayhour)
 {
  case 13:  
  case 1:  UNE;    HEURE;  break;
  case 14:
  case 2:  DEUX;   HEURES;  break;
  case 15:
  case 3:  TROIS;  HEURES;  break;
  case 16:
  case 4:  QUATRE; HEURES; break;
  case 17:
  case 5:  CINQ;   HEURES;   break;
  case 18:
  case 6:  SIX;    HEURES;   break;
  case 19:
  case 7:  SEPT;   HEURES;  break;
  case 20:
  case 8:  HUIT;   HEURES; break;
  case 21:
  case 9:  NEUF;   HEURES; break;
  case 22:
  case 10: DIX;    HEURES; break;
  case 23:
  case 11: ONZE;   HEURES; break;
  case 0:  MINUIT; break;
  case 12: MIDI;   break;
 } 
}
                     #endif //FR


              #ifdef VIERTALENKLOK
                                    #if defined NL4
//--------------------------------------------                                                //
// CLOCK Dutch clock display for NL Viertalen klok Ulrich Vertessen
//--------------------------------------------
void Dutch(void)
{
if(Mem.EdSoftLEDSOn) EDSOFT;     
HET;                                                                                          // HET  is always on
switch (timeinfo.tm_min)
 {
  case  0: 
  case  1:  
  case  2: 
  case  3: 
  case  4: IS;  break;                                                                        // HET IS .. UUR
  case  5: 
  case  6: 
  case  7: 
  case  8: 
  case  9: IS;  MVIJF; OVER; break;                                                           // HET IS VIJF OVER .. 
  case 10: 
  case 11: 
  case 12: 
  case 13: 
  case 14: IS;  MTIEN; OVER; break;                                                           // Et cetera
  case 15: 
  case 16: 
  case 17: 
  case 18: 
  case 19: IS;  KWART; OVER2; break;
  case 20: 
  case 21: 
  case 22: 
  case 23: 
  case 24: IS;  MTIEN; MVOOR; HALF; break;
  case 25: 
  case 26: 
  case 27: 
  case 28:
  case 29: IS;  MVIJF; MVOOR; HALF; break;
  case 30:
  case 31:
  case 32:
  case 33:
  case 34: IS;  HALF; break;
  case 35: 
  case 36: 
  case 37:
  case 38:
  case 39: IS;  MVIJF; OVER; HALF; break;
  case 40: 
  case 41:
  case 42:
  case 43:
  case 44: IS;  MTIEN; OVER; HALF; break;
  case 45: 
  case 46: 
  case 47:
  case 48:
  case 49: IS;  KWART; VOOR; break;
  case 50:
  case 51:
  case 52:
  case 53:
  case 54: IS;  MTIEN; VOOR;  break;
  case 55:
  case 56: 
  case 57: 
  case 58:
  case 59: IS;  MVIJF; VOOR; break;
}
//if (timeinfo.tm_hour >=0 && hour <12) digitalWrite(AMPMpin,0); else digitalWrite(AMPMpin,1);

 sayhour = timeinfo.tm_hour;
 if (timeinfo.tm_min > 19 )  sayhour = timeinfo.tm_hour+1;
 if (sayhour == 24) sayhour = 0;

switch (sayhour)
 {
  case 13:  
  case 1: EEN; break;
  case 14:
  case 2: TWEE; break;
  case 15:
  case 3: DRIE; break;
  case 16:
  case 4: VIER; break;
  case 17:
  case 5: VIJF; break;
  case 18:
  case 6: ZES; break;
  case 19:
  case 7: ZEVEN; break;
  case 20:
  case 8: ACHT; break;
  case 21:
  case 9: NEGEN; break;
  case 22:
  case 10: TIEN; break;
  case 23:
  case 11: ELF; break;
  case 0:
  case 12: TWAALF; break;
 } 
 switch (timeinfo.tm_min)
 {
//  case 59: 
  case  0: 
  case  1: 
  case  2: 
  case  3: 
  case  4: UUR;  break; 
 }

 switch (timeinfo.tm_min % 5)                                                                // The minutes modulo 5
 {
  case  0: break;
  case  1: MIN1; break;
  case  2: MIN1; MIN2; break;
  case  3: MIN1; MIN2; MIN3; break;
  case  4: MIN1; MIN2; MIN3; MIN4;  
 }
}
                     #endif  //NL4

                     #ifdef UK4
//--------------------------------------------                                                //
// CLOCK English clock display
//--------------------------------------------
void English(void)
{
 IT;                                                                                          // IT is always on
// if (timeinfo.tm_hour == 00 && timeinfo.tm_min == 0 && random(2)==0) { ISUK; MID; NIGHT; return; } 
 switch (timeinfo.tm_min)
 {
  case  0:
  case  1: 
  case  2: 
  case  3: 
  case  4: ISUK;  break;
  case  5: 
  case  6: 
  case  7: 
  case  8: 
  case  9: ISUK;  MFIVE; PAST; break;
  case 10: 
  case 11: 
  case 12: 
  case 13: 
  case 14: ISUK;  MTEN; PAST; break;
  case 15: 
  case 16: 
  case 17: 
  case 18: 
  case 19: ISUK;  QUARTER; PAST; break;
  case 20: 
  case 21: 
  case 22: 
  case 23: 
  case 24: ISUK;  TWENTY; PAST; break;
  case 25: 
  case 26: 
  case 27: 
  case 28: 
  case 29: ISUK;  TWENTY; MFIVE; PAST; break;
  case 30:
  case 31: 
  case 32: 
  case 33: 
  case 34: ISUK;  HALFUK; PAST; break;
  case 35: 
  case 36: 
  case 37: 
  case 38: 
  case 39: ISUK;  TWENTY; MFIVE; TO; break;
  case 40: 
  case 41: 
  case 42: 
  case 43:
  case 44: ISUK;  TWENTY; TO; break;
  case 45: 
  case 46:
  case 47: 
  case 48: 
  case 49: ISUK;  QUARTER; TO; break;
  case 50: 
  case 51: 
  case 52: 
  case 53: 
  case 54: ISUK;  MTEN; TO;  break;
  case 55: 
  case 56: 
  case 57: 
  case 58: 
  case 59: ISUK;  MFIVE; TO; break;
}
//if (timeinfo.tm_hour >=0 && hour <12) digitalWrite(AMPMpin,0); else digitalWrite(AMPMpin,1);

 sayhour = timeinfo.tm_hour;
 if (timeinfo.tm_min > 34 ) sayhour = timeinfo.tm_hour+1;
 if (sayhour == 24) sayhour = 0;

switch (sayhour)
 {
  case 13:  
  case 1:  ONE; break;
  case 14:
  case 2:  TWO; break;
  case 15:
  case 3:  THREE; break;
  case 16:
  case 4:  FOUR; break;
  case 17:
  case 5:  FIVE; break;
  case 18:
  case 6:  SIXUK; break;
  case 19:
  case 7:  SEVEN; break;
  case 20:
  case 8:  EIGHT; break;
  case 21:
  case 9:  NINE; break;
  case 22:
  case 10: TEN; break;
  case 23:
  case 11: ELEVEN; break;
  case 0:
  case 12: TWELVE; break;
 } 
 switch (timeinfo.tm_min)
 {
  
  case  0: 
  case  1: 
  case  2:
  case  3:   
  case  4: OCLOCK;  break; 
 }
}
                     #endif //UK
                     #if defined DE4
//--------------------------------------------                                                //
// CLOCK German clock display
//--------------------------------------------
void German(void)
{
  ES;                                                                                         // ES is always on
// if (timeinfo.tm_hour == 00 && timeinfo.tm_min == 0 && random(2)==0) {IST; MITTER; NACHTDE; return; } 
 switch (timeinfo.tm_min)
 {
  case  0:
  case  1:
  case  2: 
  case  3: 
  case  4: IST; break;
  case  5: 
  case  6: 
  case  7: 
  case  8: 
  case  9: IST; MFUNF; NACH; break;
  case 10: 
  case 11: 
  case 12: 
  case 13:
  case 14: IST; MZEHN; NACH; break;
  case 15: 
  case 16: 
  case 17: 
  case 18: 
  case 19: IST; VIERTEL; NACH; break;
  case 20: 
  case 21: 
  case 22: 
  case 23: 
  case 24: IST; MZEHN; VOR; HALB; break;
  case 25: 
  case 26: 
  case 27: 
  case 28:
  case 29: IST; MFUNF; VOR; HALB; break; 
  case 30:
  case 31: 
  case 32: 
  case 33: 
  case 34: IST; HALB; break;
  case 35: 
  case 36:
  case 37: 
  case 38: 
  case 39: IST; MFUNF; NACH; HALB; break;
  case 40: 
  case 41: 
  case 42: 
  case 43: 
  case 44: IST; MZEHN; NACH; HALB; break;
  case 45: 
  case 46: 
  case 47: 
  case 48: 
  case 49: IST; VIERTEL; VOR; break;
  case 50: 
  case 51: 
  case 52: 
  case 53: 
  case 54: IST; MZEHN; VOR;  break;
  case 55: 
  case 56: 
  case 57: 
  case 58: 
  case 59: IST; MFUNF; VOR; break;
}
//if (timeinfo.tm_hour >=0 && hour <12) digitalWrite(AMPMpin,0); else digitalWrite(AMPMpin,1);

 sayhour = timeinfo.tm_hour;
 if (timeinfo.tm_min > 19 ) sayhour = timeinfo.tm_hour+1;
 if (sayhour == 24) sayhour = 0;

switch (sayhour)
 {
  case 13:  
  case 1: EINS; break;
  case 14:
  case 2: ZWEI; break;
  case 15:
  case 3: DREI; break;
  case 16:
  case 4: VIERDE; break;
  case 17:
  case 5: FUNF; break;
  case 18:
  case 6: SECHS; break;
  case 19:
  case 7: SIEBEN; break;
  case 20:
  case 8: ACHTDE; break;
  case 21:
  case 9: NEUN; break;
  case 22:
  case 10: ZEHN; break;
  case 23:
  case 11: ELFDE; break;
  case 0:
  case 12: ZWOLF; break;
 } 
 switch (timeinfo.tm_min)
 {
 
  case  0: 
  case  1: 
  case  2:
  case  3:   
  case  4: UHR;  break; 
 }
}
                     #endif //DE4
                     #ifdef FR4
//--------------------------------------------                                                //
// CLOCK French clock display
//--------------------------------------------
void French(void)
{
 IL;                                                                                          // IL is always on
 switch (timeinfo.tm_min)
 {
  case  0: EST;  DITLEHEURE; break;
  case  1: 
  case  2: 
  case  3: 
  case  4: EST;   DITLEHEURE; break;
  case  5: 
  case  6: 
  case  7: 
  case  8: 
  case  9: EST;   DITLEHEURE; MCINQ; break;
  case 10: 
  case 11: 
  case 12: 
  case 13: 
  case 14: EST;   DITLEHEURE; MDIX;  break;
  case 15: 
  case 16: 
  case 17: 
  case 18: 
  case 19: EST;   DITLEHEURE; ET; QUART; break;
  case 20: 
  case 21: 
  case 22: 
  case 23: 
  case 24: EST;   DITLEHEURE; VINGT; break;
  case 25: 
  case 26: 
  case 27: 
  case 28: 
  case 29: EST;   DITLEHEURE; VINGT; MCINQ; break;
  case 30:
  case 31: 
  case 32: 
  case 33: 
  case 34: EST;   DITLEHEURE; ET; DEMI; break;
  case 35: 
  case 36: 
  case 37: 
  case 38: 
  case 39: EST;   DITLEHEURE; MOINS; VINGT; MCINQ; break;
  case 40: 
  case 41: 
  case 42: 
  case 43: 
  case 44: EST;   DITLEHEURE; MOINS; VINGT;  break;
  case 45: 
  case 46: 
  case 47: 
  case 48: 
  case 49: EST;   DITLEHEURE; MOINS; LE; QUART; break;
  case 50: 
  case 51: 
  case 52: 
  case 53: 
  case 54: EST;   DITLEHEURE; MOINS; MDIX;   break;
  case 55: 
  case 56: 
  case 57: 
  case 58: 
  case 59: EST;   DITLEHEURE; MOINS; MCINQ;  break;
 }
}

void DitLeHeure(void)
{
 byte sayhour = timeinfo.tm_hour;
 if (timeinfo.tm_min > 34 ) sayhour = timeinfo.tm_hour+1;
 if (sayhour == 24) sayhour = 0;

 switch (sayhour)
 {
  case 13:  
  case 1:  UNE;    HEURE;  break;
  case 14:
  case 2:  DEUX;   HEURES; break;
  case 15:
  case 3:  TROIS;  HEURES; break;
  case 16:
  case 4:  QUATRE; HEURES; break;
  case 17:
  case 5:  CINQ;   HEURES; break;
  case 18:
  case 6:  SIX;    HEURES; break;
  case 19:
  case 7:  SEPT;   HEURES; break;
  case 20:
  case 8:  HUIT;   HEURES; break;
  case 21:
  case 9:  NEUF;   HEURES; break;
  case 22:
  case 10: DIX;    HEURES; break;
  case 23:
  case 11: ONZE;   HEURES; break;
  case 0:  MINUIT;         break;
  case 12: MIDI;           break;
 } 
}
                     #endif //FR4

                #endif  // VIERTALENKLOK                                             

// ===== 2. COLOUR PICKER PAGE (ColourPick_html[]) ============================
const char ColourPick_html[] = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>Word Clock – LED Colour Settings</title>
<style>
  :root {
    --bg:     #0a0c0f;
    --panel:  #111418;
    --border: #1e2530;
    --text:   #c8d4e0;
    --dim:    #4a5568;
  }
  *, *::before, *::after { box-sizing: border-box; margin: 0; padding: 0; }

  body {
    background: var(--bg);
    color: var(--text);
    font-family: 'Courier New', Courier, monospace;
    min-height: 100vh;
    display: flex;
    flex-direction: column;
    align-items: center;
    padding: 32px 16px 56px;
  }

  header { text-align: center; margin-bottom: 36px; }
  header h1 {
    font-family: 'Courier New', Courier, monospace;
    font-weight: 900;
    font-size: clamp(1.3rem, 5vw, 1.9rem);
    letter-spacing: .15em;
    color: #fff;
    text-shadow: 0 0 18px rgba(255,255,255,.2);
  }
  header p { margin-top: 6px; font-size: .75rem; color: #ffd700; letter-spacing: .08em; }

  .card {
    background: var(--panel);
    border: 1px solid var(--border);
    border-radius: 12px;
    width: 100%; max-width: 580px;
    overflow: hidden;
    box-shadow: 0 8px 40px rgba(0,0,0,.6);
  }

  .lang-row {
    display: grid;
    grid-template-columns: 52px 1fr 90px auto;
    align-items: center;
    gap: 14px;
    padding: 16px 20px;
    border-bottom: 1px solid var(--border);
    transition: background .15s;
  }
  .lang-row:last-child { border-bottom: none; }
  .lang-row:hover { background: #161b22; }

  .badge {
    font-family: 'Courier New', Courier, monospace;
    font-weight: 700;
    font-size: .82rem;
    letter-spacing: .12em;
    text-align: center;
    padding: 5px 0;
    border-radius: 5px;
    border: 1px solid currentColor;
    flex-shrink: 0;
  }

  .picker-wrap {
    position: relative;
    height: 36px;
    border-radius: 6px;
    overflow: hidden;
    border: 1px solid var(--border);
    cursor: pointer;
    transition: border-color .2s;
  }
  .picker-wrap:hover { border-color: #3a4556; }
  .picker-wrap input[type="color"] {
    position: absolute;
    inset: -6px;
    width: calc(100% + 12px);
    height: calc(100% + 12px);
    border: none;
    cursor: pointer;
    opacity: 0;
  }
  .colour-preview {
    position: absolute;
    inset: 0;
    pointer-events: none;
    transition: background .1s;
  }
  .colour-preview::after {
    content: '';
    position: absolute;
    bottom: 0; left: 0; right: 0;
    height: 4px;
    background: transparent;
    transition: background .15s;
  }
  .colour-preview.is-white::after { background: rgba(255,255,255,.6); }

  .hex-val {
    font-size: .71rem;
    letter-spacing: .04em;
    color: #c8d4e0;
    text-align: right;
    font-variant-numeric: tabular-nums;
    white-space: nowrap;
  }

  .var-name {
    font-size: .70rem;
    color: #c8d4e0;
    letter-spacing: .04em;
    white-space: nowrap;
  }

  .off-btn {
    font-size: .65rem;
    font-family: 'Courier New', Courier, monospace;
    letter-spacing: .06em;
    color: var(--dim);
    background: transparent;
    border: 1px solid var(--border);
    border-radius: 4px;
    padding: 3px 8px;
    cursor: pointer;
    flex: none;
    transition: color .2s, border-color .2s;
    width: auto;
  }
  .off-btn:hover { color: #c8d4e0; border-color: #3a4556; }

  .w-tag {
    display: inline-block;
    font-size: .58rem;
    letter-spacing: .08em;
    color: #fff;
    background: rgba(255,255,255,.12);
    border: 1px solid rgba(255,255,255,.25);
    border-radius: 3px;
    padding: 1px 5px;
    margin-left: 4px;
    vertical-align: middle;
    opacity: 0;
    transition: opacity .2s;
  }
  .w-tag.show { opacity: 1; }

  .output-wrap { width: 100%; max-width: 580px; margin-top: 26px; }
  pre#output {
    background: var(--panel);
    border: 1px solid var(--border);
    border-radius: 10px;
    padding: 16px 20px;
    font-family: 'Courier New', Courier, monospace;
    font-size: .80rem;
    line-height: 1.75;
    color: #a8bbd0;
    overflow-x: auto;
  }
  pre#output .kw  { color: #79b8ff; }
  pre#output .var { color: #e3f0ff; }
  pre#output .num { color: #b5f06a; }
  pre#output .pun { color: #6a8096; }
  pre#output .cmt { color: #4a6070; }

  .btn-row {
    margin-top: 18px;
    display: flex;
    gap: 12px;
    width: 100%; max-width: 580px;
  }
  button {
    flex: 1;
    padding: 12px 0;
    border: none;
    border-radius: 8px;
    font-family: 'Courier New', Courier, monospace;
    font-size: .72rem;
    font-weight: 700;
    letter-spacing: .12em;
    text-transform: uppercase;
    cursor: pointer;
    transition: opacity .15s, transform .1s;
  }
  button:active { transform: scale(.97); }
  #btn-send {
    background: linear-gradient(135deg, #00b4d8, #0077b6);
    color: #fff;
    box-shadow: 0 0 16px rgba(0,180,216,.3);
  }
  #btn-home {
    background: var(--panel);
    color: var(--text);
    border: 1px solid var(--border);
  }
  #btn-home:hover { background: #161b22; }

  #status {
    margin-top: 14px;
    width: 100%; max-width: 580px;
    font-size: .74rem;
    text-align: center;
    color: var(--dim);
    min-height: 1.4em;
    transition: color .3s;
  }
  #status.ok  { color: #69ff8a; }
  #status.err { color: #ff4d6d; }

  #toast {
    position: fixed;
    bottom: 28px; left: 50%;
    transform: translateX(-50%) translateY(20px);
    background: #1a2535; color: #a8ffc4;
    border: 1px solid #2a4a35;
    padding: 9px 22px;
    border-radius: 30px;
    font-size: .76rem; letter-spacing: .07em;
    opacity: 0;
    transition: opacity .3s, transform .3s;
    pointer-events: none; white-space: nowrap;
  }
  #toast.show { opacity: 1; transform: translateX(-50%) translateY(0); }

  .led-dots { display: flex; gap: 5px; align-items: center; margin-bottom: 16px; }
  .dot { width: 7px; height: 7px; border-radius: 50%; animation: pulse 2.4s ease-in-out infinite; }
  @keyframes pulse {
    0%,100% { opacity:.3; transform:scale(1); }
    50%      { opacity:1; transform:scale(1.45); box-shadow: 0 0 8px currentColor; }
  }
</style>
</head>
<body>

<header>
  <div class="led-dots">
    <span class="dot"></span><span class="dot"></span>
    <span class="dot"></span><span class="dot"></span>
  </div>
  <h1>%%CNAME%%</h1>
  <p>LED Colour Settings &nbsp;&middot;&nbsp; NL / DE / FR / UK</p>
</header>

<div class="card" id="card"></div>

<div class="output-wrap">
  <pre id="output"></pre>
</div>

<div class="btn-row">
  <button id="btn-send">&#9654;&nbsp; Send to Clock</button>
  <button id="btn-home" onclick="location.href='/'">&#8592;&nbsp; Main Page</button>
</div>
<div id="status"></div>
<div id="toast"></div>

<script>
const LANGS = [
  { id:'nl', label:'NL', varName:'ColourNL',    packed:'%%COLNL%%' },
  { id:'de', label:'DE', varName:'ColourDE',    packed:'%%COLDE%%' },
  { id:'fr', label:'FR', varName:'ColourFR',    packed:'%%COLFR%%' },
  { id:'uk', label:'UK', varName:'ColourUK',    packed:'%%COLUK%%' },
  { id:'bg', label:'BG', varName:'DimmedLetter', packed:'%%COLBG%%' },
];

// Unpack 0xWWRRGGBB → '#rrggbb' for the colour picker.
// If W byte is set, it represents a gray/white shade on the W channel
// (e.g. 0x22000000 = dim gray, 0xFF000000 = full white) — reconstruct
// it as an equal R=G=B value so the picker shows the correct shade.
function unpackToHex(s) {
  const n = parseInt(s, 16) >>> 0;
  const w = (n >>> 24) & 0xFF;
  if (w !== 0) {
    const hex2 = w.toString(16).padStart(2, '0');
    return '#' + hex2 + hex2 + hex2;
  }
  return '#' + (n & 0x00FFFFFF).toString(16).padStart(6, '0');
}

function packColour(hex) {
  if (hex.replace('#','').toLowerCase() === 'ffffff') return 0xFF000000 >>> 0;
  return (parseInt(hex.replace('#',''), 16) & 0x00FFFFFF) >>> 0;
}

function toHex8(n)   { return '0x' + (n>>>0).toString(16).toUpperCase().padStart(8,'0'); }
function isWhite(h)  { return h.replace('#','').toLowerCase() === 'ffffff'; }
// true when R=G=B (a gray/white shade that the ESP32 may store on the W channel)
function isGrayShade(h) {
  const n = parseInt(h.replace('#',''), 16);
  const r=(n>>16)&0xff, g=(n>>8)&0xff, b=n&0xff;
  return r === g && g === b && r > 0;
}
function hexToRGB(h) { const n=parseInt(h.replace('#',''),16); return {r:(n>>16)&0xff,g:(n>>8)&0xff,b:n&0xff}; }

const state = {};
LANGS.forEach(l => { state[l.id] = unpackToHex(l.packed); });

// Build card rows
const card = document.getElementById('card');
LANGS.forEach(({ id, label, varName }) => {
  const offBtn = (id === 'bg')
    ? `<button class="off-btn" id="off-btn-bg">Off</button>` : '';
  card.insertAdjacentHTML('beforeend', `
  <div class="lang-row" id="row-${id}">
    <span class="badge" id="badge-${id}">${label}</span>
    <div class="picker-wrap">
      <div class="colour-preview" id="prev-${id}"></div>
      <input type="color" id="col-${id}" value="${state[id]}">
    </div>
    <span class="hex-val" id="hex-${id}"></span>
    <span class="var-name">${varName}<span class="w-tag" id="wtag-${id}">W</span> ${offBtn}</span>
  </div>`);
});

// Off button — sets BG to black
document.getElementById('off-btn-bg').addEventListener('click', () => {
  state['bg'] = '#000000';
  document.getElementById('col-bg').value = '#000000';
  updateRow('bg');
});

const dots = document.querySelectorAll('.dot');

function updateRow(id) {
  const rgb    = state[id];
  const packed = packColour(rgb);
  const white  = isWhite(rgb);

  document.getElementById('prev-'  + id).style.background = rgb;
  document.getElementById('prev-'  + id).classList.toggle('is-white', white);
  document.getElementById('hex-'   + id).textContent = toHex8(packed);
  document.getElementById('wtag-'  + id).classList.toggle('show', isGrayShade(rgb));

  const glow = (rgb === '#000000') ? '#555' : rgb;
  const badge = document.getElementById('badge-' + id);
  badge.style.color = badge.style.borderColor = glow;
  badge.style.boxShadow = '0 0 10px ' + glow;

  const dotIdx = LANGS.findIndex(l => l.id === id);
  if (dotIdx >= 0 && dotIdx < dots.length)
    dots[dotIdx].style.background = glow;

  updateOutput();
}

function updateOutput() {
  const maxLen = Math.max(...LANGS.map(l => l.varName.length));
  const lines = LANGS.map(({ id, varName }) => {
    const rgb   = state[id];
    const hex8  = toHex8(packColour(rgb));
    const {r,g,b} = hexToRGB(rgb);
    const pad   = ' '.repeat(maxLen - varName.length + 1);
    const cmt   = isGrayShade(rgb)
      ? `<span class="cmt">// W=${r} (gray/white channel)</span>`
      : `<span class="cmt">// R=${r} G=${g} B=${b}</span>`;
    return `<span class="kw">uint32_t</span> <span class="var">${varName}</span>${pad}<span class="pun">=</span> <span class="num">${hex8}</span><span class="pun">;</span>  ${cmt}`;
  });
  document.getElementById('output').innerHTML = lines.join('\n');
}

// Bind pickers
LANGS.forEach(({ id }) => {
  document.getElementById('col-' + id).addEventListener('input', e => {
    state[id] = e.target.value;
    updateRow(id);
  });
  updateRow(id);
});

// Send to clock
document.getElementById('btn-send').addEventListener('click', () => {
  const params = LANGS.map(({ id }) => {
    const key    = (id === 'bg') ? 'BG' : id.toUpperCase();
    const hexStr = packColour(state[id]).toString(16).toUpperCase().padStart(8,'0');
    return key + '=' + hexStr;
  }).join('&');
  const st = document.getElementById('status');
  st.className = ''; st.textContent = 'Sending\u2026';
  fetch('/setcolours?' + params)
    .then(r => r.ok ? r.text() : Promise.reject(r.status))
    .then(() => { st.className='ok'; st.textContent='Saved \u2713  '+new Date().toLocaleTimeString(); showToast('Colours saved!'); })
    .catch(e  => { st.className='err'; st.textContent='Error: '+e; });
});

function showToast(msg) {
  const t = document.getElementById('toast');
  t.textContent = msg; t.classList.add('show');
  setTimeout(() => t.classList.remove('show'), 2800);
}
</script>
</body>
</html>
)rawliteral";
// ===== 3. SETTINGS MENU PAGE (menu_html[]) =================================
const char menu_html[] =
R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>WordClock Menu</title>
<style>
:root {
  --bg:     #0f172a;
  --card:   #1e293b;
  --border: #334155;
  --text:   #e2e8f0;
  --muted:  #64748b;
  --cyan:   #22d3ee;
  --green:  #22c55e;
  --red:    #ef4444;
}
* { box-sizing: border-box; margin: 0; padding: 0; }
body {
  font-family: 'Segoe UI', system-ui, sans-serif;
  background: var(--bg);
  color: var(--text);
  padding: 16px;
  max-width: 480px;
  margin: 0 auto;
  min-height: 100vh;
}
a { color: var(--cyan); text-decoration: none; }
h1 {
  text-align: center;
  font-size: 1.3rem;
  color: var(--cyan);
  letter-spacing: 3px;
  margin-bottom: 4px;
}
.sub { text-align: center; color: var(--muted); font-size: 0.72rem; margin-bottom: 20px; }

.card {
  background: var(--card);
  border: 1px solid var(--border);
  border-radius: 14px;
  padding: 14px 16px;
  margin-bottom: 16px;
}
.card-title {
  font-size: 0.65rem;
  font-weight: 700;
  text-transform: uppercase;
  letter-spacing: 2px;
  color: var(--muted);
  margin-bottom: 10px;
}
.sect { margin-top: 16px; padding-top: 14px; border-top: 1px solid var(--border); }

/* H radio cards */
.hgrid { display: grid; grid-template-columns: 1fr 1fr 1fr; gap: 8px; margin-bottom: 10px; }
.hcard {
  padding: 7px 4px 6px;
  background: var(--bg);
  border: 2px solid var(--border);
  border-radius: 10px;
  cursor: pointer;
  text-align: center;
  transition: border-color 0.2s, background 0.2s, box-shadow 0.2s;
  -webkit-tap-highlight-color: transparent;
  user-select: none;
}
.hcard:hover { border-color: var(--cyan); }
.hcard.sel  { border-color: var(--cyan);  background: #0e2d38;   box-shadow: 0 0 0 1px var(--cyan); }
.hcard.ton  { border-color: var(--green); background: #14532d33; box-shadow: 0 0 0 1px var(--green); }
.hcard .ico { font-size: 1.3rem; display: block; margin-bottom: 2px; }
.hcard .cod { font-size: 0.6rem; color: var(--cyan);  font-weight: 700; letter-spacing: 1px; display: block; }
.hcard.ton .cod { color: var(--green); }
.hcard .nam { font-size: 0.72rem; display: block; }

/* 2-column button grid */
.agrid { display: grid; grid-template-columns: 1fr 1fr; gap: 8px; }

.abtn, .tbtn {
  padding: 6px 6px;
  border: 1px solid var(--border);
  border-radius: 8px;
  background: var(--bg);
  color: var(--text);
  font-size: 0.82rem;
  cursor: pointer;
  text-align: center;
  transition: background 0.15s, transform 0.1s, border-color 0.15s, color 0.15s;
  -webkit-tap-highlight-color: transparent;
  user-select: none;
}
.abtn:hover          { background: #1e3a5f55; border-color: #3b82f6; }
.abtn:active, .tbtn:active { transform: scale(0.96); }
.abtn.danger         { border-color: #7f1d1d; color: #fca5a5; }
.abtn.danger:hover   { background: #7f1d1d44; border-color: var(--red); }
.tbtn                { border-color: var(--border); color: var(--muted); padding: 5px 6px; }
.tbtn.on             { border-color: var(--green); color: var(--green); background: #14532d33; }
.tbtn:hover          { background: #1e3a5f55; }
.tbtn.wide           { grid-column: 1 / -1; }

/* Sliders — plain grey track, coloured thumb only */
.slrow { margin-bottom: 6px; }
.slrow:last-child { margin-bottom: 0; }
.sllbl {
  display: flex; justify-content: space-between;
  font-size: 0.75rem; color: var(--muted); margin-bottom: 2px;
}
.sllbl span { color: var(--cyan); font-weight: 600; }
input[type=range] {
  -webkit-appearance: none; appearance: none;
  width: 100%; background: transparent; cursor: pointer;
}
input[type=range]::-webkit-slider-runnable-track {
  height: 4px; background: var(--border); border-radius: 2px;
}
input[type=range]::-webkit-slider-thumb {
  -webkit-appearance: none;
  width: 22px; height: 22px; border-radius: 50%;
  background: var(--cyan); margin-top: -9px;
}
input[type=range]::-moz-range-track {
  height: 4px; background: var(--border); border-radius: 2px;
}
input[type=range]::-moz-range-thumb {
  width: 22px; height: 22px; border-radius: 50%;
  background: var(--cyan); border: none;
}

/* Q slider tick numbers */
.q-ticks {
  display: flex; justify-content: space-between;
  font-size: 0.65rem; color: var(--muted);
  margin-bottom: 4px; padding: 0 2px;
}
#qlabel { text-align: center; font-size: 0.78rem; color: var(--cyan); margin-top: 6px; }

/* Brightness + clock side by side */
.bright-row { display: flex; gap: 12px; align-items: center; }
.bright-sliders { flex: 1; min-width: 0; }
.clock-wrap { display: flex; flex-direction: column; align-items: center; flex-shrink: 0; width: 120px; }
#nclock { width: 120px; touch-action: none; }
#ntxt { margin-top: 4px; font-size: 0.68rem; color: var(--cyan); text-align: center; }

/* Toast */
#toast {
  position: fixed; bottom: 22px; left: 50%;
  transform: translateX(-50%) translateY(60px);
  background: var(--card); color: var(--text);
  padding: 9px 20px; border-radius: 20px;
  border: 1px solid var(--cyan); font-size: 0.82rem;
  opacity: 0; transition: transform 0.28s, opacity 0.28s;
  pointer-events: none; white-space: nowrap; z-index: 999;
}
#toast.show { transform: translateX(-50%) translateY(0); opacity: 1; }
</style>
</head>
<body>

<h1 id="cname">WordClock</h1>
<p class="sub"><a href="/colourpick">&#127912; Colour Picker</a> &nbsp;|&nbsp; <a href="/">&#8592; Main page</a></p>

<!-- ===== TOP CARD: Q + Brightness + Clock + Toggles + Actions ===== -->
<div class="card">

  <!-- Q Display colour -->
  <div class="card-title">Display Colour &mdash; Q0&ndash;Q9</div>
  <div class="q-ticks">
    <span>0</span><span>1</span><span>2</span><span>3</span><span>4</span>
    <span>5</span><span>6</span><span>7</span><span>8</span><span>9</span>
  </div>
  <input type="range" id="qslider" min="0" max="9" step="1" value="0"
         onchange="setQ(this.value)">
  <div id="qlabel">Q0 &mdash; Default colour</div>

  <!-- Brightness sliders + 24h clock side by side -->
  <div class="sect">
    <div class="card-title">Brightness &amp; Display Hours</div>
    <div class="bright-row">
      <div class="bright-sliders">
        <div class="slrow">
          <div class="sllbl">Slope &nbsp;<span id="sv">50</span></div>
          <input type="range" id="sslider" min="1" max="255" step="1" value="50"
                 oninput="document.getElementById('sv').textContent=this.value"
                 onchange="send('S'+this.value)">
        </div>
        <div class="slrow">
          <div class="sllbl">Min &nbsp;<span id="lv">5</span></div>
          <input type="range" id="lslider" min="0" max="250" step="1" value="5"
                 oninput="document.getElementById('lv').textContent=this.value"
                 onchange="send('L'+this.value)">
        </div>
        <div class="slrow">
          <div class="sllbl">Max &nbsp;<span id="mv">255</span></div>
          <input type="range" id="mslider" min="1" max="255" step="1" value="255"
                 oninput="document.getElementById('mv').textContent=this.value"
                 onchange="send('M'+this.value)">
        </div>
      </div>
      <div class="clock-wrap">
        <svg id="nclock" viewBox="0 0 200 200" xmlns="http://www.w3.org/2000/svg"></svg>
        <div id="ntxt">Always on</div>
      </div>
    </div>
  </div>

  <!-- Toggle buttons -->
  <div class="sect">
    <div class="card-title">Toggle Settings</div>
    <div class="agrid">
      <button class="tbtn" id="t0"    data-cmd=")"  onclick="togbtn(this)">HET IS WAS</button>
      <button class="tbtn" id="t1"    data-cmd="("  onclick="togbtn(this)">EdSoft LEDs</button>
      <button class="tbtn" id="t2"    data-cmd="O"  onclick="togbtn(this)">Display</button>
      <button class="tbtn" id="t3"    data-cmd="P"  onclick="togbtn(this)">Status LEDs</button>
      <button class="tbtn" id="t5"    data-cmd="X"  onclick="togbtn(this)">NTP</button>
      <button class="tbtn" id="t8"    data-cmd="}"  onclick="togbtn(this)">Ring Buffer Log</button>
      <button class="tbtn" id="kldr"  data-cmd="K"  onclick="togbtn(this)">LDR/sec</button>
      <button class="tbtn" id="kmin"  data-cmd="K1" onclick="togbtn(this)">Time/min</button>
      <button class="tbtn" id="khour" data-cmd="K2" onclick="togbtn(this)">Time/hour</button>
      <button class="tbtn" id="koff"  data-cmd="K0" onclick="togbtn(this)">Time/off</button>
      <button class="tbtn wide" id="tj" onclick="togDS3231()">DS3231 RTC</button>
    </div>
    <div id="rtcstatus" style="font-size:0.75em;color:#ccc;background:#333;padding:3px 6px;border-radius:4px;margin-top:6px;text-align:center;">RTC: loading...</div>
  </div>

  <!-- Log window -->
  <div class="sect">
    <div class="card-title">Log</div>
    <textarea id="lastlog" readonly style="font-family:monospace;font-size:0.72rem;color:var(--text);background:var(--bg);border:1px solid var(--border);border-radius:8px;padding:6px;box-sizing:border-box;height:110px;width:100%;resize:none;"></textarea>
  </div>

  <!-- Action buttons -->
  <div class="sect">
    <div class="card-title">Actions</div>
    <div class="agrid">
      <button class="abtn" onclick="send('Y')">&#128161; LED Test</button>
      <button class="abtn" onclick="send('!')">&#128336; RTC Info</button>
      <button class="abtn" onclick="send('&')">&#128257; RTC Update</button>
      <button class="abtn" onclick="send('Z')">&#128246; WPS</button>
      <button class="abtn" onclick="send('{')">&#128247; Learn IR</button>
      <button class="abtn" onclick="send('I')">&#128203; Version</button>
      <button class="abtn" onclick="send('#')">&#128302; Self Test</button>
      <button class="abtn" onclick="send('U')">&#127916; Demo Mode</button>
      <button class="abtn danger" onclick="cfm('R','Reset all settings to default?')">&#9888; Reset</button>
      <button class="abtn danger" onclick="cfm('@','Restart the clock now?')">&#128257; Restart</button>
    </div>
  </div>

</div>

<!-- ===== INPUT METHOD CARD ===== -->
<div class="card">
  <div class="card-title">Input Method &mdash; select one (H00&ndash;H05)</div>
  <div class="hgrid">
    <div class="hcard" id="h00" onclick="selH('H00','h00')">
      <span class="ico">&#8709;</span>
      <span class="cod">H00</span>
      <span class="nam">None / NTP</span>
    </div>
    <div class="hcard" id="h01" onclick="selH('H01','h01')">
      <span class="ico">&#128260;</span>
      <span class="cod">H01</span>
      <span class="nam">Rotary</span>
    </div>
    <div class="hcard" id="h02" onclick="selH('H02','h02')">
      <span class="ico">&#128306;</span>
      <span class="cod">H02</span>
      <span class="nam">Membrane</span>
    </div>
    <div class="hcard" id="h03" onclick="selH('H03','h03')">
      <span class="ico">&#127894;</span>
      <span class="cod">H03</span>
      <span class="nam">IR Large</span>
    </div>
    <div class="hcard" id="h04" onclick="selH('H04','h04')">
      <span class="ico">&#127899;</span>
      <span class="cod">H04</span>
      <span class="nam">IR Tiny</span>
    </div>
    <div class="hcard" id="h05" onclick="togH05()">
      <span class="ico">&#8986;</span>
      <span class="cod">H05</span>
      <span class="nam">Time Receiver</span>
    </div>
  </div>
  <p style="font-size:0.68rem;color:var(--muted);">&#9888; Restart required after H00&ndash;H04 change</p>
</div>

<div id="toast"></div>

<script>
// ── Q slider ──────────────────────────────────────────────
var qnames = ['Default colour','Hourly colour','White','Own colour',
              'Own + HetIsWas','Wheel colour','Digital','Hourly + HetIsWas','Rainbow','Fast colour'];
function updateQlabel(v) {
  document.getElementById('qlabel').textContent = 'Q' + v + ' — ' + qnames[v];
}
function setQ(v) { updateQlabel(v); send('Q' + v); }

// ── 24h clock picker ──────────────────────────────────────
var nOff = 0, nOn = 0, nDrag = null;
var CX = 100, CY = 100, CR = 78;

function p2(n) { return n < 10 ? '0' + n : '' + n; }

function hXY(h) {
  var a = (h / 24) * 2 * Math.PI - Math.PI / 2;
  return [CX + CR * Math.cos(a), CY + CR * Math.sin(a)];
}

function arcD(h1, h2) {
  var p1 = hXY(h1), p2b = hXY(h2);
  var span = (h2 - h1 + 24) % 24;
  if (span === 0) return '';
  return 'M' + p1[0].toFixed(1)  + ',' + p1[1].toFixed(1) +
         ' A' + CR + ',' + CR + ' 0 ' + (span > 12 ? 1 : 0) + ',1 ' +
         p2b[0].toFixed(1) + ',' + p2b[1].toFixed(1);
}

function buildClock() {
  var svg = document.getElementById('nclock');
  var ns = 'http://www.w3.org/2000/svg';
  function el(tag, attrs) {
    var e = document.createElementNS(ns, tag);
    for (var k in attrs) e.setAttribute(k, attrs[k]);
    return e;
  }
  // Grey ring
  svg.appendChild(el('circle', {cx:CX, cy:CY, r:CR, fill:'none', stroke:'#334155', 'stroke-width':'6'}));
  // 24 tick marks
  for (var h = 0; h < 24; h++) {
    var a = (h / 24) * 2 * Math.PI - Math.PI / 2;
    var r1 = h % 6 === 0 ? CR - 8 : CR - 5;
    var r2 = CR + 1;
    svg.appendChild(el('line', {
      x1: (CX + r1 * Math.cos(a)).toFixed(1), y1: (CY + r1 * Math.sin(a)).toFixed(1),
      x2: (CX + r2 * Math.cos(a)).toFixed(1), y2: (CY + r2 * Math.sin(a)).toFixed(1),
      stroke:'#475569', 'stroke-width': h % 6 === 0 ? '2' : '1'
    }));
  }
  // Hour labels 0 6 12 18
  [[0,'0'],[6,'6'],[12,'12'],[18,'18']].forEach(function(p) {
    var lr = CR - 18;
    var a2 = (p[0] / 24) * 2 * Math.PI - Math.PI / 2;
    var t = el('text', {
      x: (CX + lr * Math.cos(a2)).toFixed(1),
      y: (CY + lr * Math.sin(a2)).toFixed(1),
      'text-anchor':'middle', 'dominant-baseline':'central',
      fill:'#64748b', 'font-size':'11'
    });
    t.textContent = p[1];
    svg.appendChild(t);
  });
  // Off-period arc (dimmed / off hours, shown in orange)
  svg.appendChild(el('path', {id:'narc', d:'', fill:'none', stroke:'#f97316', 'stroke-width':'6', 'stroke-linecap':'round'}));
  // ON handle (green)
  var hon = el('circle', {id:'non-h', r:'10', fill:'#22c55e', cursor:'pointer'});
  hon.addEventListener('mousedown',  function(e){e.preventDefault(); nDrag='on';});
  hon.addEventListener('touchstart', function(e){e.preventDefault(); nDrag='on';}, {passive:false});
  svg.appendChild(hon);
  // OFF handle (orange/red)
  var hoff = el('circle', {id:'noff-h', r:'10', fill:'#f97316', cursor:'pointer'});
  hoff.addEventListener('mousedown',  function(e){e.preventDefault(); nDrag='off';});
  hoff.addEventListener('touchstart', function(e){e.preventDefault(); nDrag='off';}, {passive:false});
  svg.appendChild(hoff);
}

function drawClock() {
  var op = hXY(nOff), np = hXY(nOn);
  document.getElementById('narc').setAttribute('d', arcD(nOff, nOn));
  document.getElementById('noff-h').setAttribute('cx', op[0].toFixed(1));
  document.getElementById('noff-h').setAttribute('cy', op[1].toFixed(1));
  document.getElementById('non-h').setAttribute('cx', np[0].toFixed(1));
  document.getElementById('non-h').setAttribute('cy', np[1].toFixed(1));
  document.getElementById('ntxt').textContent =
    (nOff === 0 && nOn === 0)
      ? 'Always on'
      : 'Off ' + p2(nOff) + ':00 – On ' + p2(nOn) + ':00';
}

function evXY(e) {
  return e.touches ? {x: e.touches[0].clientX, y: e.touches[0].clientY}
                   : {x: e.clientX, y: e.clientY};
}

function xyToHour(e) {
  var pt = evXY(e);
  var rect = document.getElementById('nclock').getBoundingClientRect();
  var dx = pt.x - (rect.left + rect.width  / 2);
  var dy = pt.y - (rect.top  + rect.height / 2);
  var angle = Math.atan2(dy, dx) + Math.PI / 2;
  if (angle < 0) angle += 2 * Math.PI;
  return Math.round(angle / (2 * Math.PI / 24)) % 24;
}

document.addEventListener('mousemove', function(e) {
  if (!nDrag) return;
  if (nDrag === 'off') nOff = xyToHour(e); else nOn = xyToHour(e);
  drawClock();
});
document.addEventListener('touchmove', function(e) {
  if (!nDrag) return;
  e.preventDefault();
  if (nDrag === 'off') nOff = xyToHour(e); else nOn = xyToHour(e);
  drawClock();
}, {passive: false});
function nEnd() {
  if (!nDrag) return;
  nDrag = null;
  send('N' + p2(nOff) + p2(nOn));
}
document.addEventListener('mouseup',  nEnd);
document.addEventListener('touchend', nEnd);

buildClock();
drawClock();

// ── Load state from clock ─────────────────────────────────
function loadState() {
  fetch('/menustate')
    .then(function(r) { return r.json(); })
    .then(function(s) {
      if (s.cname) document.getElementById('cname').textContent = s.cname;
      document.querySelectorAll('.tbtn').forEach(function(b) {
        if (s[b.id] === 1) b.classList.add('on'); else b.classList.remove('on');
      });
      document.querySelectorAll('.hcard').forEach(function(c) { c.classList.remove('sel'); });
      var hel = document.getElementById(s.hi);
      if (hel) hel.classList.add('sel');
      var h05 = document.getElementById('h05');
      if (s.t9 === 1) h05.classList.add('ton'); else h05.classList.remove('ton');
      var qv = s.qval || 0;
      document.getElementById('qslider').value = qv;
      updateQlabel(qv);
      document.getElementById('sslider').value = s.sval || 50;
      document.getElementById('lslider').value = s.lval || 5;
      document.getElementById('mslider').value = s.mval || 255;
      document.getElementById('sv').textContent = s.sval || 50;
      document.getElementById('lv').textContent = s.lval || 5;
      document.getElementById('mv').textContent = s.mval || 255;
      nOff = s.noff || 0;
      nOn  = s.non  || 0;
      drawClock();
      var hw = s.ds3231hw ? 'DS3231 installed' : 'DS3231 not found';
      var src = 'Active: ' + (s.rtcsrc || 'ESP32');
      document.getElementById('rtcstatus').textContent = hw + ' · ' + src;
      document.getElementById('tj').textContent = s.ds3231hw ? 'Use DS3231 RTC' : 'Use ESP32 RTC';
    })
    .catch(function() {});
}
loadState();

// ── Button handlers ───────────────────────────────────────
function togbtn(el) { el.classList.toggle('on'); send(el.getAttribute('data-cmd')); setTimeout(loadState, 300); }

function togDS3231() { send('J'); setTimeout(loadState, 300); }

function togH05() {
  var el = document.getElementById('h05');
  el.classList.toggle('ton');
  send('H05');
}

function selH(cmd, elId) {
  document.querySelectorAll('.hcard').forEach(function(c) { c.classList.remove('sel'); });
  document.getElementById(elId).classList.add('sel');
  send(cmd);
}

function send(cmd) {
  fetch('/get?input1=' + encodeURIComponent(cmd)).catch(function(){});
  showToast('Sent: ' + cmd);
}

function cfm(cmd, msg) { if (confirm(msg)) send(cmd); }

function showToast(msg) {
  var t = document.getElementById('toast');
  t.textContent = msg;
  t.classList.add('show');
  setTimeout(function() { t.classList.remove('show'); }, 1800);
}

// ── Log window ────────────────────────────────────────────
function fetchLastLog() {
  fetch('/tekstprint').then(function(r){return r.text();}).then(function(t){
    var lines = t.split('\n').filter(function(l){return l.trim()!='';});
    var el = document.getElementById('lastlog');
    var atBottom = (el.scrollHeight - el.scrollTop - el.clientHeight) < 30;
    el.value = lines.slice(-50).join('\n');
    if (atBottom) el.scrollTop = el.scrollHeight;
  }).catch(function(){});
  setTimeout(fetchLastLog, 1000);
}
fetchLastLog();
</script>

</body>
</html>
)rawliteral";
