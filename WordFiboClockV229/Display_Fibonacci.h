//
// Display_Fibonacci.h - FIBONACCI variant display layer.
// ---------------------------------------------------------------------------
// Provides the Fibonacci/chrono display: Displaytime() + the cell logic and
// colour palettes (extracted verbatim from ESP32ArduinoFibonacciV011), the LED
// layout constants the shared trunk expects, and no-op stubs for the word-clock
// display functions the trunk calls. Included once from the .ino when FIBONACCI
// is selected.
// ---------------------------------------------------------------------------

// --- LED layout ----------------------------------------------------------- //
// NUM_LEDS selects the physical Fibonacci cell mapping in setPixel().
// Options: 12 / 14 / 17 / 24 / 32 / 36 / 174  (compile-time for now).
const uint32_t NUM_LEDS      = 17;
const uint32_t LEDNEARLDR    = 9999;   // no LED sits near the LDR
const byte     MATRIX_WIDTH  = 1;      // no digital matrix display
const byte     MATRIX_HEIGHT = 1;

#define CLOCK_PIXELS    5                                           // Number of cells in clock = 5 (1,1,2,3,5)
byte    bits[CLOCK_PIXELS+1];                                       // Stores hours=1 / minutes=2 to set in LEDsetTime()
byte    BitSet[CLOCK_PIXELS+1];                                     // For calculation of the bits to set

// --- Forward declarations (.h files get no Arduino auto-prototypes) -------- //
void LEDsetTime(byte hours, byte minutes);
void MakeFibonacciList(byte Value);
void MakeChronoList(byte Hours, byte Minutes, byte Seconds);
void setPixel(byte pixel, uint32_t kleur);

// --- Fibonacci colour palettes (Q0..Q9) ----------------------------------- //
const uint32_t colors[][5] =                                        // The FIBONACCI colour palettes
   {//off   hours   minutes both;
   { white, red   , yellow , blue  , green },  // #0 Mondriaan
   { white, red   , dyellow, blue  , green },  // #1 Mondriaan1 
   { white, red   , green  , blue  , green },  // #2 RGB  
   { white, apple , green  , grass , blue  },  // #3 Greens
   { white, red   , grass  , purple, green },  // #4 Pastel                                                                 
   { white, orange, green  , marine, blue  },  // #5 Modern
   { white, sky   , purple , blue  , green },  // #6 Cold
   { white, red   , yellow , orange, green },  // #7 Warm
   { wgray, red   , dyellow, blue  , white },  // #8 Mondriaan 2
   { wgray, red   , yellow,  blue  , white }}; // #9 Mondriaan 3

// Names shown by the 'Q' command (Q0..Q9) - Fibonacci colour palettes.
const char* const QchoiceNames[] =
  { "Mondriaan1", "Mondriaan2", "RGB", "Greens", "Pastel",
    "Modern", "Cold", "Warm", "Mondriaan3", "Mondriaan4" };
const byte LASTITEM = sizeof(QchoiceNames) / sizeof(QchoiceNames[0]) - 1;   // highest valid Q index (Q0..Q9)


// --- Display functions (verbatim from the Fibonacci sketch) --------------- //
//--------------------------------------------
void Displaytime(void)
{ 
 if (Mem.FiboChrono) LEDsetTime(timeinfo.tm_hour , timeinfo.tm_min);                         // Fibonacci display   
 else            MakeChronoList(timeinfo.tm_hour , timeinfo.tm_min, timeinfo.tm_sec);        // Chrono (clock display) display  


// Turned off:   if(Mem.HC12Time) SendHC12TimeString();                                                       // HC12 Send time via HC-12 on D4
}

void LEDsetTime(byte hours, byte minutes)
{ 
// hours %=12;                                                                                // Keep the hours between 0 and 12
 if (!(hours==12 && minutes<5)) hours %=12;                                                   // Keep the hours between 0 and 12 but display red when 12:00-12:04
 for(int i=0; i<CLOCK_PIXELS; i++) { bits[i] = 0; BitSet[i] = 0; }                            // Clear all bits  
 MakeFibonacciList(hours);
 for(int i=0; i<CLOCK_PIXELS; i++) if(BitSet[i]) bits[i] +=1;                                 // If hour must be lit add 1
 for(int i=0; i<CLOCK_PIXELS; i++)    BitSet[i] = 0;                                          // Clear  bits  
 MakeFibonacciList(minutes/5);                                                                // Block is 5 minutes  
  for(int i=0; i<CLOCK_PIXELS; i++)  
 {
   if( BitSet[i]) bits[i] +=2;    
   setPixel(i, colors[Mem.DisplayChoice][bits[i]]); 
  // Serial.print(hours); Serial.print(":"); Serial.print(minutes);  Serial.print(" ");  
  // Serial.println(colors[Mem.DisplayChoice][bits[i]],HEX);
  }
}

//--------------------------------------------                                                //
// FIBONACCI Calculate the proper Fibonacci-numbers (Pixels)
//--------------------------------------------
void MakeFibonacciList(byte Value)
{
 byte CalcValue = 0;
 byte pos = CLOCK_PIXELS;
 while (Value != CalcValue  )
  {
   byte Fibonaccireeks[] = {1,1,2,3,5,0};                          // Set up Fibonacci array with 6 numbers.
   for(int i=0; i<=CLOCK_PIXELS; i++) BitSet[i] = 0;               // Clear all bits. NB CLOCK_PIXELS is noof cells / strips in the clock
   CalcValue = 0;
   while ( (Value != CalcValue) &&  CalcValue <=  Value)   
    {
     do { pos = random(CLOCK_PIXELS); } while(Fibonaccireeks[pos] == 0 );   
     CalcValue += Fibonaccireeks[pos];
     BitSet[pos] = 1;                                              // Set pos in array for valid number    
     Fibonaccireeks[pos] = 0;                                      // Number taken from array 
    }
  }
}

//--------------------------------------------                                                //
// FIBONACCI Calculate the proper chronological numbers (Pixels)
//--------------------------------------------
void MakeChronoList(byte Hours, byte Minutes,byte Seconds)
{
 Hours %=12;                                                                                  // Keep the hours between 0 and 12
 byte Secsegment = Seconds / 5;
 byte Minsegment = Minutes / 5;
 byte Bit;
 uint32_t Kleur;                                                                              // Color
 for(int i=0; i<12; i++)
  {
   Bit = 0;
   if(i < Hours)        Bit+= 1;                                                              // If hours use the second colour
   if(i < Minsegment)   Bit+= 2;                                                              // If minute use the third colour. If hours was set the fourth colour is displayed 
   if(Mem.NoExUl>0 && i == Secsegment)  Bit = 4;                                              // If second use the fifth colour to display 
   Kleur = colors[Mem.DisplayChoice][Bit];
   if(Mem.NoExUl>1) {if(i<Minutes%5 && Seconds%5<1) Kleur=purple;}                            // If in Ultimate mode
   ColorLed(i,Kleur); 
   }
}
//                                            
//--------------------------------------------                                                //
// FIBONACCI Turn on the right pixels and colours for 24 hour 
//--------------------------------------------
void setPixel(byte pixel, uint32_t kleur)
{
 switch(NUM_LEDS)
 {
  case 12:
        switch(pixel)                                                                         // 12 LEDs 
          {
            case 0:      ColorLeds("", 0, 0,kleur); break;
            case 1:      ColorLeds("", 1, 1,kleur); break;
            case 2:      ColorLeds("", 2, 3,kleur); break;
            case 3:      ColorLeds("", 4, 6,kleur); break;
            case 4:      ColorLeds("", 7,11,kleur); break;
          }
          break;
  case 14:
           switch(pixel)                                                                      // 14 LEDs    
           {
            case 0:      ColorLeds("", 0, 0,kleur); break;
            case 1:      ColorLeds("", 1, 1,kleur); break;
            case 2:      ColorLeds("", 2, 3,kleur); break;
            case 3:      ColorLeds("", 4, 7,kleur); break;
            case 4:      ColorLeds("", 8,13,kleur); break;
           }
          break;

   case 17:
           switch(pixel)                                                                      // 14 LEDs    
           {
            case 0:      ColorLeds("", 0, 0,kleur); break;
            case 1:      ColorLeds("", 1, 1,kleur); break;
            case 2:      ColorLeds("", 2, 3,kleur); break;
            case 3:      ColorLeds("", 4, 7,kleur); break;
            case 4:      ColorLeds("", 8,16,kleur); break;
           }
          break;
  case 24:
         switch(pixel)                                                                        // 24 LEDs    
          {
            case 0:      ColorLeds("", 0, 1,kleur); break;
            case 1:      ColorLeds("", 2, 3,kleur); break;
            case 2:      ColorLeds("", 4, 7,kleur); break;
            case 3:      ColorLeds("", 8, 13,kleur); break;
            case 4:      ColorLeds("", 14,23,kleur); break;
          }
          break;    
 case 32:
  switch(pixel)                                                                               // for 32 LEDs, 4 strips of 8 LEDs
           {
             case 0:      ColorLeds("", 2, 3,kleur);                            break;
             case 1:      ColorLeds("",12,13,kleur);                            break;
             case 2:      ColorLeds("", 0, 1,kleur); ColorLeds("",14,15,kleur); break;
             case 3:      ColorLeds("",16,19,kleur); ColorLeds("",28,31,kleur); break;
             case 4:      ColorLeds("", 4,11,kleur); ColorLeds("",20,27,kleur); break;
           }
         break;
  case 36:                                                                                      // For case with 3 x 12 LEDs clock 
           switch(pixel)  
          {
            case 0:      ColorLeds("", 0,   2,kleur); break;
            case 1:      ColorLeds("", 3,   5,kleur); break;
            case 2:      ColorLeds("", 6,  11,kleur); break;
            case 3:      ColorLeds("", 12, 20,kleur); break;
            case 4:      ColorLeds("", 21, 35,kleur); break; 
          }
         break;
 case 174:                                                                                     // For 50x50 cm case with 174 LEDs    
         switch(pixel)  
          {
            case 0:      ColorLeds("",  0,   15,kleur); break;
            case 1:      ColorLeds("", 16,   31,kleur); break;
            case 2:      ColorLeds("", 32,   63,kleur); break;
            case 3:      ColorLeds("", 64,  103,kleur); break;
            case 4:      ColorLeds("", 104, 173,kleur); break; 
          }
         break;         
  default:
           switch(pixel)                                                                      // 14 LEDs    
           {
            case 0:      ColorLeds("", 0, 0,kleur); break;
            case 1:      ColorLeds("", 1, 1,kleur); break;
            case 2:      ColorLeds("", 2, 3,kleur); break;
            case 3:      ColorLeds("", 4, 7,kleur); break;
            case 4:      ColorLeds("", 8,13,kleur); break;
           }
          break;
 }  
}

// --- No-op stubs for the word-clock display functions the trunk calls ----- //
void SetSecondColour(void) {}
void Demomode(void)        {}
void FireMode(void)        {}
void Selftest(int Delayms) {}
void Selftest(void)        {}
void BlinkUUR(int NoofBlinks, int Delayms)      {}
void BlinkHETISWAS(int NoofBlinks, int Delayms) {}
void BlinkTWAALF(int NoofBlinks, int Delayms)   {}
