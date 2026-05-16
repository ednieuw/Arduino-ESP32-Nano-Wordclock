#include "ClockFaces.h"

extern struct EEPROMstorage Mem;
extern void Displaytime(void);
//--------------------------------------------                                                //
// CLOCK Set second colour
// Set the colour per second of 'IS' and 'WAS'
//---------------------------------------------
void SetSecondColour(void)
{
                             #if defined FOURLANGUAGECLOCK || defined VIERTALENKLOK
  return;                                                                                     // 4-language clock uses SetColours() once per minute
                             #endif  //FOURLANGUAGECLOCK    
 static byte kleur = 0;                                                                       //  color increases per second in RAINBOW
 MINColor = FuncCRGBW(15 + timeinfo.tm_min * 4, 255 - timeinfo.tm_min * 4, 0, 0); 
 SECColor = FuncCRGBW(15 + timeinfo.tm_sec * 4, 255 - timeinfo.tm_sec * 4, 0, 0 );   
                                                                                              // Light up  IS or WAS with the proper colour  
 switch (Mem.DisplayChoice)
  {
   case DEFAULTCOLOUR: LetterColor = DefaultColor;                                     break; // Yellow text with changing MIN and SEC  
   case HOURLYCOLOUR : LetterColor = MINColor = SECColor = HourColor[timeinfo.tm_hour];break; // A colour every hour, not changing MIN and SEC
   case WHITECOLOR   : LetterColor = MINColor = SECColor = white;                      break; // All white
   case OWNCOLOUR    : LetterColor = MINColor = SECColor = Mem.OwnColour;              break; // Own colour
   case OWNHETISWAS  : LetterColor = Mem.OwnColour;                                    break; // Own colour except HET IS WAS  
   case WHEELCOLOR   : LetterColor = MINColor = SECColor = Wheel((timeinfo.tm_min*4)); break; // Colour of all letters changes per minute
   case DIGITAL      : MINColor = SECColor = 0;                                        break; // Digital display of time. No IS WAS turn color off in display
   case HOURHETISWAS : LetterColor = HourColor[timeinfo.tm_hour];                      break; // A colour every hour changing MIN and SEC
   case RAINBOW      : LetterColor = MINColor = SECColor = Wheel(kleur++);             break; // Colour of all letters changes per second 
  }
 if(Mem.HetIsWasOff){MINColor = SECColor = 0;}                                                // If HET IS WAS is turned off in menu with Q9
 NoTextInLeds  = true;                                                                        // Flag to control printing of the text IS en WAS in serial
                    #ifdef DE144CLOCK  
 if(Is) {IST;} else {WAR;}                      
                    #endif  //DE144CLOCK                      
 if(Is) {IS;}  else {WAS;} 
 if (Mem.DisplayChoice == RAINBOW ) Displaytime();                                            // Now we have to send the time to the display every second 
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
   case OWNHETISWAS  : LetterColor   = Mem.OwnColour; 
                       UKLetterColor = Mem.OwnColour;
                       DELetterColor = Mem.OwnColour;
                       FRLetterColor = Mem.OwnColour;                       break;            // Own colour HET IS WAS changing  
   case WHEELCOLOR   : LetterColor   = Wheel((timeinfo.tm_min*4));       
                       UKLetterColor = Wheel((60 +timeinfo.tm_min*4));
                       DELetterColor = Wheel((120+timeinfo.tm_min*4));
                       FRLetterColor = Wheel((180+timeinfo.tm_min*4));      break;            // Colour of all letters changes per second
   case RAINBOW      : LetterColor   = Wheel((     kleur++));       
                       UKLetterColor = Wheel((60 + kleur));
                       DELetterColor = Wheel((120+ kleur));
                       FRLetterColor = Wheel((180+ kleur));                 break;            // Colour of all letters changes per second                       
   case DIGITAL      :                                                      break;            //LetterColor = Mem.OwnColour;                 break;            // digital display of time. No IS WAS turn color off in display
  }
}  
                   #endif  //FOURLANGUAGECLOCK       

//------------------------------------------------------------------------------
// CLOCK Self test sequence
//------------------------------------------------------------------------------

void Selftest(int Delayms)
{ 
 GetTijd(true);                                                                                  // Prints time in Serial monitor
 LedsOff(); 
                      #if defined NLM1M2M3M4
 HET;   Laatzien(Delayms); IS;    Laatzien(Delayms); MVIJF; Laatzien(Delayms); MTIEN;   Laatzien(Delayms); MVOOR;  Laatzien(Delayms); OVER;   Laatzien(Delayms);
 KWART; Laatzien(Delayms); HALF;  Laatzien(Delayms); OVER2; Laatzien(Delayms); VOOR;    Laatzien(Delayms); EEN;    Laatzien(Delayms); TWEE;   Laatzien(Delayms);  
 DRIE;  Laatzien(Delayms); VIER;  Laatzien(Delayms); VIJF;  Laatzien(Delayms); ZES;     Laatzien(Delayms); ZEVEN;  Laatzien(Delayms); ACHT;   Laatzien(Delayms); 
 NEGEN; Laatzien(Delayms); TIEN;  Laatzien(Delayms); ELF;   Laatzien(Delayms); TWAALF;  Laatzien(Delayms); UUR;    Laatzien(Delayms); 
 MIN1;  Laatzien(Delayms); MIN2;  Laatzien(Delayms); MIN3;  Laatzien(Delayms); MIN4;    Laatzien(Delayms);
                      #endif  //NLM1M2M3M4
                      #ifdef NL144CLOCK                    
 HET;   Laatzien(Delayms); IS;    Laatzien(Delayms); WAS;    Laatzien(Delayms); PRECIES; Laatzien(Delayms); MTIEN;  Laatzien(Delayms);  MVIJF; Laatzien(Delayms);    
 KWART; Laatzien(Delayms); VOOR;  Laatzien(Delayms); OVER;   Laatzien(Delayms); HALF;    Laatzien(Delayms); MIDDER; Laatzien(Delayms);  VIJF;  Laatzien(Delayms);
 TWEE;  Laatzien(Delayms); EEN;   Laatzien(Delayms); VIER;   Laatzien(Delayms); TIEN;    Laatzien(Delayms); TWAALF; Laatzien(Delayms);  DRIE;  Laatzien(Delayms);
 NEGEN; Laatzien(Delayms); NACHT; Laatzien(Delayms); ACHT;   Laatzien(Delayms); ZES;     Laatzien(Delayms); ZEVEN;  Laatzien(Delayms);  ELF;   Laatzien(Delayms); 
 NOEN;  Laatzien(Delayms); UUR;   Laatzien(Delayms); EDSOFT; Laatzien(Delayms);
                      #endif  //NL144CLOCK
                      #ifdef NL92CLOCK                    
 HET;   Laatzien(Delayms); IS;    Laatzien(Delayms); WAS;    Laatzien(Delayms); PRECIES; Laatzien(Delayms); MTIEN;  Laatzien(Delayms);  MVIJF; Laatzien(Delayms);    
 KWART; Laatzien(Delayms); VOOR;  Laatzien(Delayms); OVER;   Laatzien(Delayms); HALF;    Laatzien(Delayms);  VIJF;  Laatzien(Delayms);
 TWEE;  Laatzien(Delayms); EEN;   Laatzien(Delayms); VIER;   Laatzien(Delayms); TIEN;    Laatzien(Delayms); TWAALF; Laatzien(Delayms);  DRIE;  Laatzien(Delayms);
 NEGEN; Laatzien(Delayms); ACHT;  Laatzien(Delayms); ZES;    Laatzien(Delayms); ZEVEN;   Laatzien(Delayms);  ELF;   Laatzien(Delayms); 
 UUR;   Laatzien(Delayms); //EDSOFT; Laatzien(Delayms);
                      #endif  //NL92CLOCK
                      #ifdef DE144CLOCK                    
 ES;    Laatzien(Delayms);   IST;     Laatzien(Delayms); WAR;    Laatzien(Delayms); GENAU; Laatzien(Delayms); MZEHN; Laatzien(Delayms);  MFUNF; Laatzien(Delayms);    
 VIERTEL; Laatzien(Delayms); ZWANZIG; Laatzien(Delayms); KURZ;   Laatzien(Delayms); VOR;   Laatzien(Delayms); NACH;  Laatzien(Delayms);  HALB;  Laatzien(Delayms);
 FUNF;  Laatzien(Delayms);   EINS;    Laatzien(Delayms); VIERDE; Laatzien(Delayms); ZEHN;  Laatzien(Delayms); ZWOLF; Laatzien(Delayms);  DREI;  Laatzien(Delayms);
 NEUN;  Laatzien(Delayms);   ACHTDE;  Laatzien(Delayms); SECHS;  Laatzien(Delayms); SIEBEN;Laatzien(Delayms); ZWEI;  Laatzien(Delayms);  ELFDE; Laatzien(Delayms); 
 UHR;   Laatzien(Delayms); 
                      #endif  //DE144CLOCK 

                      #ifdef FOURLANGUAGECLOCK
 // Nederlands (NL)
                      #ifdef NL
 HET;    Laatzien(Delayms); IS;      Laatzien(Delayms); WAS;    Laatzien(Delayms); PRECIES; Laatzien(Delayms);
 MTIEN;  Laatzien(Delayms); MVIJF;   Laatzien(Delayms); KWART;  Laatzien(Delayms); VOOR;    Laatzien(Delayms);
 OVER;   Laatzien(Delayms); HALF;    Laatzien(Delayms); MIDDER; Laatzien(Delayms); VIJF;    Laatzien(Delayms);
 TWEE;   Laatzien(Delayms); EEN;     Laatzien(Delayms); VIER;   Laatzien(Delayms); TIEN;    Laatzien(Delayms);
 TWAALF; Laatzien(Delayms); DRIE;    Laatzien(Delayms); NEGEN;  Laatzien(Delayms); ACHT;    Laatzien(Delayms);
 NACHT;  Laatzien(Delayms); ZES;     Laatzien(Delayms); ZEVEN;  Laatzien(Delayms); ELF;     Laatzien(Delayms);
 NOEN;   Laatzien(Delayms); UUR;     Laatzien(Delayms);
 #endif //NL

 // English (UK)
                      #ifdef UK
 IT;     Laatzien(Delayms); ISUK;    Laatzien(Delayms); WASUK;   Laatzien(Delayms); EXACTUK; Laatzien(Delayms);
 HALFUK; Laatzien(Delayms); TWENTY;  Laatzien(Delayms); MFIVE;   Laatzien(Delayms); QUARTER; Laatzien(Delayms);
 MTEN;   Laatzien(Delayms); PAST;    Laatzien(Delayms); TO;      Laatzien(Delayms); SIXUK;   Laatzien(Delayms);
 TWO;    Laatzien(Delayms); FIVE;    Laatzien(Delayms); TWELVE;  Laatzien(Delayms); TEN;     Laatzien(Delayms);
 ELEVEN; Laatzien(Delayms); FOUR;    Laatzien(Delayms); NINE;    Laatzien(Delayms); THREE;   Laatzien(Delayms);
 EIGHT;  Laatzien(Delayms); ONE;     Laatzien(Delayms); SEVEN;   Laatzien(Delayms); OCLOCK;  Laatzien(Delayms);
                      #endif //UK

 // Deutsch (DE)
                      #ifdef DE
 ES;     Laatzien(Delayms); IST;     Laatzien(Delayms); WAR;     Laatzien(Delayms); GENAU;   Laatzien(Delayms);
 MZEHN;  Laatzien(Delayms); MFUNF;   Laatzien(Delayms); VIERTEL; Laatzien(Delayms); ZWANZIG; Laatzien(Delayms);
 KURZ;   Laatzien(Delayms); VOR;     Laatzien(Delayms); NACH;    Laatzien(Delayms); HALB;    Laatzien(Delayms);
 FUNF;   Laatzien(Delayms); EINS;    Laatzien(Delayms); VIERDE;  Laatzien(Delayms); ZEHN;    Laatzien(Delayms);
 ZWOLF;  Laatzien(Delayms); DREI;    Laatzien(Delayms); NEUN;    Laatzien(Delayms); ACHTDE;  Laatzien(Delayms);
 SECHS;  Laatzien(Delayms); SIEBEN;  Laatzien(Delayms); ZWEI;    Laatzien(Delayms); ELFDE;   Laatzien(Delayms);
 UHR;    Laatzien(Delayms);
                      #endif //DE

 // Français (FR)
                      #ifdef FR
 IL;     Laatzien(Delayms); EST;     Laatzien(Delayms); ETAIT;   Laatzien(Delayms); EXACT;   Laatzien(Delayms);
 SIX;    Laatzien(Delayms); DEUX;    Laatzien(Delayms); TROIS;   Laatzien(Delayms); ONZE;    Laatzien(Delayms);
 QUATRE; Laatzien(Delayms); MINUIT;  Laatzien(Delayms); DIX;     Laatzien(Delayms); CINQ;    Laatzien(Delayms);
 NEUF;   Laatzien(Delayms); MIDI;    Laatzien(Delayms); HUIT;    Laatzien(Delayms); SEPT;    Laatzien(Delayms);
 UNE;    Laatzien(Delayms); HEURE;   Laatzien(Delayms); HEURES;  Laatzien(Delayms); ET;      Laatzien(Delayms);
 MOINS;  Laatzien(Delayms); LE;      Laatzien(Delayms); DEMI;    Laatzien(Delayms); QUART;   Laatzien(Delayms);
 MDIX;   Laatzien(Delayms); VINGT;   Laatzien(Delayms); MCINQ;   Laatzien(Delayms);
                      #endif //FR
                           #endif //FOURLANGUAGECLOCK

                           #ifdef VIERTALENKLOK
 // Nederlands (NL4)
                      #ifdef NL4
 HET;    Laatzien(Delayms); IS;     Laatzien(Delayms); MVIJF;  Laatzien(Delayms); MTIEN;  Laatzien(Delayms);
 MVOOR;  Laatzien(Delayms); OVER;   Laatzien(Delayms); KWART;  Laatzien(Delayms); OVER2;  Laatzien(Delayms);
 HALF;   Laatzien(Delayms); VOOR;   Laatzien(Delayms); EEN;    Laatzien(Delayms); DRIE;   Laatzien(Delayms);
 TWEE;   Laatzien(Delayms); VIER;   Laatzien(Delayms); VIJF;   Laatzien(Delayms); ZES;    Laatzien(Delayms);
 NEGEN;  Laatzien(Delayms); ZEVEN;  Laatzien(Delayms); ACHT;   Laatzien(Delayms); TIEN;   Laatzien(Delayms);
 ELF;    Laatzien(Delayms); TWAALF; Laatzien(Delayms); UUR;    Laatzien(Delayms);
 MIN1;   Laatzien(Delayms); MIN2;   Laatzien(Delayms); MIN3;   Laatzien(Delayms); MIN4;   Laatzien(Delayms);
 #endif //NL4

 // English (UK4)
                      #ifdef UK4
 IT;     Laatzien(Delayms); ISUK;   Laatzien(Delayms); EXACTUK; Laatzien(Delayms); HALFUK; Laatzien(Delayms);
 TWENTY; Laatzien(Delayms); MFIVE;  Laatzien(Delayms); QUARTER; Laatzien(Delayms); MTEN;   Laatzien(Delayms);
 PAST;   Laatzien(Delayms); TO;     Laatzien(Delayms); SIXUK;   Laatzien(Delayms); TWO;    Laatzien(Delayms);
 FIVE;   Laatzien(Delayms); TWELVE; Laatzien(Delayms); TEN;     Laatzien(Delayms); ELEVEN; Laatzien(Delayms);
 FOUR;   Laatzien(Delayms); NINE;   Laatzien(Delayms); THREE;   Laatzien(Delayms); EIGHT;  Laatzien(Delayms);
 ONE;    Laatzien(Delayms); SEVEN;  Laatzien(Delayms); OCLOCK;  Laatzien(Delayms);
 #endif //UK4

 // Deutsch (DE4)
                      #ifdef DE4
 ES;     Laatzien(Delayms); IST;    Laatzien(Delayms); GENAU;   Laatzien(Delayms); MZEHN;  Laatzien(Delayms);
 MFUNF;  Laatzien(Delayms); VIERTEL; Laatzien(Delayms); ZWANZIG; Laatzien(Delayms); KURZ;  Laatzien(Delayms);
 VOR;    Laatzien(Delayms); NACH;   Laatzien(Delayms); HALB;    Laatzien(Delayms); FUNF;   Laatzien(Delayms);
 EINS;   Laatzien(Delayms); VIERDE; Laatzien(Delayms); ZEHN;    Laatzien(Delayms); ZWOLF;  Laatzien(Delayms);
 DREI;   Laatzien(Delayms); NEUN;   Laatzien(Delayms); ACHTDE;  Laatzien(Delayms); SECHS;  Laatzien(Delayms);
 SIEBEN; Laatzien(Delayms); ZWEI;   Laatzien(Delayms); ELFDE;   Laatzien(Delayms); UHR;    Laatzien(Delayms);
                      #endif //DE4

 // Français (FR4)
                      #ifdef FR4
 IL;     Laatzien(Delayms); EST;    Laatzien(Delayms); EXACT;   Laatzien(Delayms); SIX;    Laatzien(Delayms);
 DEUX;   Laatzien(Delayms); TROIS;  Laatzien(Delayms); ONZE;    Laatzien(Delayms); QUATRE; Laatzien(Delayms);
 MINUIT; Laatzien(Delayms); DIX;    Laatzien(Delayms); CINQ;    Laatzien(Delayms); NEUF;   Laatzien(Delayms);
 MIDI;   Laatzien(Delayms); HUIT;   Laatzien(Delayms); SEPT;    Laatzien(Delayms); UNE;    Laatzien(Delayms);
 HEURE;  Laatzien(Delayms); HEURES; Laatzien(Delayms); ET;      Laatzien(Delayms); MOINS;  Laatzien(Delayms);
 LE;     Laatzien(Delayms); DEMI;   Laatzien(Delayms); QUART;   Laatzien(Delayms); MDIX;   Laatzien(Delayms);
 VINGT;  Laatzien(Delayms); MCINQ;  Laatzien(Delayms);
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
                     #if defined NL144CLOCK || defined NL92CLOCK
    Dutch();                                                                                 //     
                     #endif  //NL144CLOCK || defined NL92CLOCK
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

                       #ifdef DE144CLOCK                                        
//--------------------------------------------
// CLOCK Blink UHR
//--------------------------------------------
void BlinkUUR(int NoofBlinks, int Delayms)
{
for (int n=0 ; n<=NoofBlinks; n++) { LedsOff(); Laatzien(Delayms);  UHR; Laatzien(Delayms); } 
}
//--------------------------------------------                                                //
// CLOCK Blink ES IST WAR
//--------------------------------------------
void BlinkHETISWAS (int NoofBlinks, int Delayms)
{ 
for (int n=0 ; n<=NoofBlinks; n++) { LedsOff(); Laatzien(Delayms);  ES; IST; WAR; Laatzien(Delayms); } 
}
//--------------------------------------------                                                //
// CLOCK Blink ZWOLF
//--------------------------------------------
void BlinkTWAALF(int NoofBlinks, int Delayms)
{
for (int n=0 ; n<=NoofBlinks; n++) { LedsOff(); Laatzien(Delayms); ZWOLF; Laatzien(Delayms); } 
}
                     #else
//--------------------------------------------                                                //
// CLOCK Blink UUR
//--------------------------------------------
void BlinkUUR(int NoofBlinks, int Delayms)
{
for (int n=0 ; n<=NoofBlinks; n++) { LedsOff(); Laatzien(Delayms);  UUR; Laatzien(Delayms); } 
}
//--------------------------------------------                                                //
// CLOCK Blink HET IS WAS
//--------------------------------------------
void BlinkHETISWAS (int NoofBlinks, int Delayms)
{
for (int n=0 ; n<=NoofBlinks; n++) { LedsOff(); Laatzien(Delayms);  HET; IS; WAS; Laatzien(Delayms); } 
}
//--------------------------------------------                                                //
// CLOCK Blink TWAALF
//--------------------------------------------
void BlinkTWAALF(int NoofBlinks, int Delayms)
{
for (int n=0 ; n<=NoofBlinks; n++) { LedsOff(); Laatzien(Delayms); TWAALF; Laatzien(Delayms); } 
}
                     #endif //DE144CLOCK
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