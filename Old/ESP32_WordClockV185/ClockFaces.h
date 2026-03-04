//------------------------------------------------------------------------------              //
// ARDUINO Definition of installed language word clock
//------------------------------------------------------------------------------
                     #ifdef FOURLANGUAGECLOCK
#define NL
#define UK
#define DE
#define FR    
                     #endif //FOURLANGUAGECLOCK
                     #if defined NLM1M2M3M4L94 ||defined NLM1M2M3M4L114 || (defined NLM1M2M3M4L256) || (defined NLM1M2M3M4L144) || (defined NLM1M2M3M4L161)
#define NLM1M2M3M4
                     #endif

                     #ifdef NL144CLOCK
const uint32_t NUM_LEDS  =  144+4;                                    // How many leds in  strip? + 4 for the minutes
const byte MATRIX_WIDTH  =  12;                                       // Grid size For digital display mode.  
const byte MATRIX_HEIGHT =  12;                                       // Grid size For digital display mode.
// const uint32_t NUM_LEDS  =  256;                                   // Temporary for testinf with 16x16LED WS2812. To clear all LEDs. During uploading LEDs get randomly On and Off
// const byte MATRIX_WIDTH  =  16;                                    // Grid size For digital display mode.
// const byte MATRIX_HEIGHT =  16;                                    // Grid size For digital display mode.

#define HET     ColorLeds("Het",     0,   2, MINColor);   
#define IS      ColorLeds("is",      4,   5, SECColor);  ColorLeds("", 8,10, 0); Is = true;   // Turn off the WAS LEDs
#define WAS     ColorLeds("was",     8,  10, SECColor);  ColorLeds("", 4, 5, 0); Is = false;  // Turn off the IS LEDs
#define PRECIES ColorLeds("precies", 17, 23, LetterColor);
#define MTIEN   ColorLeds("tien",    12, 15, LetterColor); 
#define MVIJF   ColorLeds("vijf",    25, 28, LetterColor); 
#define KWART   ColorLeds("kwart",   30, 34, LetterColor);
#define VOOR    ColorLeds("voor",    44, 47, LetterColor);
#define OVER    ColorLeds("over",    38, 41, LetterColor);
#define HALF    ColorLeds("half",    48, 51, LetterColor);
#define MIDDER  ColorLeds("midder",  53, 58, LetterColor);
#define VIJF    ColorLeds("vijf",    66, 69, LetterColor);
#define TWEE    ColorLeds("twee",    60, 63, LetterColor);
#define EEN     ColorLeds("een",     72, 74, LetterColor);
#define VIER    ColorLeds("vier",    77, 80, LetterColor);
#define TIEN    ColorLeds("tien",    91, 94, LetterColor);
#define TWAALF  ColorLeds("twaalf",  84, 89, LetterColor);
#define DRIE    ColorLeds("drie",    96, 99, LetterColor);
#define NEGEN   ColorLeds("negen",  102,106, LetterColor);
#define ACHT    ColorLeds("acht",   114,117, LetterColor);
#define NACHT   ColorLeds("nacht",  114,118, LetterColor);
#define ZES     ColorLeds("zes",    110,112, LetterColor);
#define ZEVEN   ColorLeds("zeven",  121,125, LetterColor);
#define ELF     ColorLeds("elf",    128,130, LetterColor);
#define NOEN    ColorLeds("noen",   139,142, LetterColor);
#define UUR     ColorLeds("uur",    134,136, LetterColor);
#define EDSOFT  ColorLeds("EdSoft", 148,157, LetterColor);                                    // Optional extra LEDs Turn ON/OFF in menu with (  
#define MIN1    ColorLeds("M1",     144,144, LetterColor); 
#define MIN2    ColorLeds("M2",     145,145, LetterColor);                                    // MINColor); 
#define MIN3    ColorLeds("M3",     146,146, LetterColor);
#define MIN4    ColorLeds("M4",     147,147, LetterColor);  
#define X_OFF   ColorLeds("",         0,  2, 0);
#define X_ON    ColorLeds("",         0,  2, LetterColor);
                     #endif //NL144CLOCK
                     #ifdef NL92CLOCK
const uint32_t NUM_LEDS  =  92;                                                               // How many leds in  strip?
const byte MATRIX_WIDTH  =  10;                                                               // Grid size For digital display mode.  
const byte MATRIX_HEIGHT =  10;                                                               // Grid size For digital display mode.

#define HET     ColorLeds("Het",     0,   2, MINColor); 
#define IS      ColorLeds("is",      3,   4, SECColor);  ColorLeds("", 5, 7, 0); Is = true;   // Turn off the WAS LEDs
#define WAS     ColorLeds("was",     5,   7, SECColor);  ColorLeds("", 3, 4, 0); Is = false;  // Turn off the IS LEDs
#define MVIJF   ColorLeds("vijf",    12, 15, LetterColor); 
#define MTIEN   ColorLeds("tien",    8,  11, LetterColor);
#define KWART   ColorLeds("kwart",   16, 20, LetterColor);
#define VOOR    ColorLeds("voor",    21, 24, LetterColor);
#define OVER    ColorLeds("over",    32, 35, LetterColor);
#define PRECIES ColorLeds("precies", 25, 31, LetterColor);
#define HALF    ColorLeds("half",    36, 39, LetterColor);
#define ELF     ColorLeds("elf",     40, 42, LetterColor);
#define VIJF    ColorLeds("vijf",    47, 50, LetterColor);
#define TWEE    ColorLeds("twee",    43, 46, LetterColor);
#define EEN     ColorLeds("een",     51, 53, LetterColor);
#define VIER    ColorLeds("vier",    54, 57, LetterColor);
#define TIEN    ColorLeds("tien",    64, 67, LetterColor);
#define TWAALF  ColorLeds("twaalf",  58, 63, LetterColor);
#define DRIE    ColorLeds("drie",    68, 71, LetterColor);
#define NEGEN   ColorLeds("negen",   72, 76, LetterColor);
#define ACHT    ColorLeds("acht",    80, 83, LetterColor);
#define ZES     ColorLeds("zes",     77, 79, LetterColor);
#define ZEVEN   ColorLeds("zeven",   84, 88, LetterColor);
#define UUR     ColorLeds("uur",     89, 91, LetterColor);
#define EDSOFT  ColorLeds("EdSoft",  92, 92, LetterColor);                                    // Optional extra LEDs Turn ON/OFF in menu with (
#define X_OFF   ColorLeds("",         0,  2, 0);
#define X_ON    ColorLeds("",         0,  2, LetterColor);
#define MIN1    ColorLeds("M1",     144,144, LetterColor); 
#define MIN2    ColorLeds("M2",     145,145, LetterColor);                                    // MINColor); 
#define MIN3    ColorLeds("M3",     146,146, LetterColor);
#define MIN4    ColorLeds("M4",     147,147, LetterColor);  
#define X_OFF   ColorLeds("",         0,  2, 0);
#define X_ON    ColorLeds("",         0,  2, LetterColor);
                     #endif //NL92CLOCK

                     #ifdef DE144CLOCK
const uint32_t NUM_LEDS  =  144;                                                              // How many leds in  strip? 
const byte MATRIX_WIDTH  =  12;                                                               // Grid size For digital display mode.  
const byte MATRIX_HEIGHT =  12;                                                               // Grid size For digital display mode.
#define ES      ColorLeds("Es",       1,   2, LetterColor);   
#define IST     ColorLeds("ist",      4,   6, LetterColor);  ColorLeds("", 8, 10, 0); Is = true;
#define WAR     ColorLeds("war",      8,  10, LetterColor);  ColorLeds("", 4,  6, 0); Is = false;
#define GENAU   ColorLeds("genau",   18,  22, LetterColor);
#define MZEHN   ColorLeds("zehn",    13,  16, LetterColor);
#define MFUNF   ColorLeds("funf",   383, 386, LetterColor);
#define VIERTEL ColorLeds("viertel",375, 381, LetterColor);
#define ZWANZIG ColorLeds("zwanzig",413, 419, LetterColor);
#define KURZ    ColorLeds("kurz",   421, 424, LetterColor);
#define VOR     ColorLeds("vor",    433, 435, LetterColor);
#define NACH    ColorLeds("nach",   427, 430, LetterColor);
#define HALB    ColorLeds("halb",   465, 468, LetterColor);
#define FUNF    ColorLeds("funf",   471, 474, LetterColor);
#define EINS    ColorLeds("eins",   483, 486, LetterColor);
#define VIERDE  ColorLeds("vier",   478, 481, LetterColor);
#define ZEHN    ColorLeds("zehn",   514, 517, LetterColor);
#define ZWOLF   ColorLeds("zwolf",  520, 524, LetterColor);
#define DREI    ColorLeds("drei",   532, 535, LetterColor);
#define NEUN    ColorLeds("neun",   526, 529, LetterColor);
#define ACHTDE  ColorLeds("acht",   565, 568, LetterColor);
#define SECHS   ColorLeds("sechs",  570, 574, LetterColor);
#define SIEBEN  ColorLeds("sieben", 580, 585, LetterColor);
#define ZWEI    ColorLeds("zwei",   575, 578, LetterColor);
#define ELFDE   ColorLeds("elf",    614, 616, LetterColor);
#define UHR     ColorLeds("uhr",    621, 623, LetterColor);
                     #endif //DE144CLOCK
                     #ifdef NLM1M2M3M4L94
const uint32_t NUM_LEDS  =  94;                                    // How many leds in  strip? + 4 for the minutes
const byte MATRIX_WIDTH  =  10;                                    // NA Not used in this clock. 10 is fine 
const byte MATRIX_HEIGHT =  10;                                    // NA Not used in this clock. 10 is fine 
                   
#define HET     ColorLeds("Het",      0,   2, MINColor);           // Special adapted clock face versie Kris. IS and WAS are equal. There is no WAS
#define IS      ColorLeds("is",       3,   4, SECColor);     Is = true;    //ColorLeds("", 3, 4, 0); 
#define WAS     ColorLeds("was",      3,   4, SECColor);     Is = false;  // ColorLeds("", 3, 4, 0); 
#define MVIJF   ColorLeds("vijf",     5,   8, LetterColor); 
#define MVOOR   ColorLeds("voor",     9,  12, LetterColor);
#define MTIEN   ColorLeds("tien",    13,  16, LetterColor); 
#define OVER    ColorLeds("over",    17,  20, LetterColor);
#define KWART   ColorLeds("kwart",   21,  25, LetterColor);
#define OVER2   ColorLeds("over",    26,  29, LetterColor);
#define HALF    ColorLeds("half",    30,  33, LetterColor);
#define VOOR    ColorLeds("voor",    34,  37, LetterColor);
#define EEN     ColorLeds("een",     38,  40, LetterColor);
#define DRIE    ColorLeds("drie",    41,  44, LetterColor);
#define TWEE    ColorLeds("twee",    45,  48, LetterColor);
#define VIER    ColorLeds("vier",    49,  52, LetterColor);
#define VIJF    ColorLeds("vijf",    53,  56, LetterColor);
#define ZES     ColorLeds("zes",     57,  59, LetterColor);
#define NEGEN   ColorLeds("negen",   60,  64, LetterColor);
#define ZEVEN   ColorLeds("zeven",   65,  69, LetterColor);
#define ACHT    ColorLeds("acht",    70,  73, LetterColor);
#define TIEN    ColorLeds("tien",    74,  77, LetterColor);
#define ELF     ColorLeds("elf",     78,  80, LetterColor);
#define TWAALF  ColorLeds("twaalf",  84,  89, LetterColor);
#define UUR     ColorLeds("uur",     81,  83, LetterColor);
#define MIN1    ColorLeds("M1",      90,  90, LetterColor); 
#define MIN2    ColorLeds("M2",      91,  91, LetterColor);       //MINColor); 
#define MIN3    ColorLeds("M3",      92,  92, LetterColor);
#define MIN4    ColorLeds("M4",      93,  93, LetterColor);  
#define EDSOFT  ColorLeds("EdSoft",  94,  94, LetterColor);                                    // Optional extra LEDs Turn ON/OFF in menu with (  
                     #endif //NLM1M2M3M4L94
                     #ifdef NLM1M2M3M4L114 
const uint32_t NUM_LEDS  =  114+10;                                                              // How many leds in  strip? + 4 for the minutes
const byte MATRIX_WIDTH  =  11;                                                               // NA Not used in this clock. 10 is fine 
const byte MATRIX_HEIGHT =  10;                                                               // NA Not used in this clock. 10 is fine 
                   
#define HET     ColorLeds("Het",      0,   2, MINColor);                                      // Special adapted clock face
#define IS      ColorLeds("is",       4,   5, SECColor);     Is = true;    //ColorLeds("", 4, 5, 0); IS and WAS are equal. There is no WAS
#define WAS     ColorLeds("was",      4,   5, SECColor);     Is = false;  // ColorLeds("", 4, 5, 0); 
#define MVIJF   ColorLeds("vijf",     7,  10, LetterColor); 
#define MVOOR   ColorLeds("voor",    11,  14, LetterColor);
#define MTIEN   ColorLeds("tien",    18,  21, LetterColor); 
#define OVER    ColorLeds("over",    22,  25, LetterColor);
#define KWART   ColorLeds("kwart",   28,  32, LetterColor);
#define OVER2   ColorLeds("over",    33,  36, LetterColor);
#define HALF    ColorLeds("half",    40,  43, LetterColor);
#define VOOR    ColorLeds("voor",    44,  47, LetterColor);
#define EEN     ColorLeds("een",     51,  53, LetterColor);
#define DRIE    ColorLeds("drie",    55,  58, LetterColor);
#define TWEE    ColorLeds("twee",    62,  65, LetterColor);
#define VIER    ColorLeds("vier",    66,  69, LetterColor);
#define VIJF    ColorLeds("vijf",    70,  73, LetterColor);
#define ZES     ColorLeds("zes",     74,  76, LetterColor);
#define NEGEN   ColorLeds("negen",   77,  81, LetterColor);
#define ZEVEN   ColorLeds("zeven",   83,  87, LetterColor);
#define ACHT    ColorLeds("acht",    88,  91, LetterColor);
#define TIEN    ColorLeds("tien",    92,  95, LetterColor);
#define ELF     ColorLeds("elf",     96,  98, LetterColor);
#define TWAALF  ColorLeds("twaalf", 104, 109, LetterColor);
#define UUR     ColorLeds("uur",     99, 101, LetterColor);
#define MIN1    ColorLeds("M1",     110, 110, LetterColor); 
#define MIN2    ColorLeds("M2",     111, 111, LetterColor);                                   // MINColor); 
#define MIN3    ColorLeds("M3",     112, 112, LetterColor);
#define MIN4    ColorLeds("M4",     113, 113, LetterColor);    
#define EDSOFT  ColorLeds("EdSoft", 114, 124, LetterColor);                                   // Optional extra LEDs Turn ON/OFF in menu with (
                     #endif //NLM1M2M3M4L114

                     #ifdef NLM1M2M3M4L144
const uint32_t NUM_LEDS  =  144;                                                              // How many leds in  strip? + 4 for the minutes
const byte MATRIX_WIDTH  =  12;                                                               // NA Not used in this clock. 10 is fine 
const byte MATRIX_HEIGHT =  12;                                                               // NA Not used in this clock. 10 is fine 
                   
#define HET     ColorLeds("Het",      0,   4, MINColor);           // Special adapted clock face
#define IS      ColorLeds("is",       5,   7, SECColor);     Is = true;    //ColorLeds("", 4, 5, 0); IS and WAS are equal. There is no WAS
#define WAS     ColorLeds("was",      5,   7, SECColor);     Is = false;  // ColorLeds("", 4, 5, 0); 
#define MVIJF   ColorLeds("vijf",     8,  13, LetterColor); 
#define MVOOR   ColorLeds("voor",    14,  19, LetterColor);
#define MTIEN   ColorLeds("tien",    20,  25, LetterColor); 
#define OVER    ColorLeds("over",    26,  31, LetterColor);
#define KWART   ColorLeds("kwart",   32,  39, LetterColor);
#define OVER2   ColorLeds("over",    39,  45, LetterColor);
#define HALF    ColorLeds("half",    46,  51, LetterColor);
#define VOOR    ColorLeds("voor",    52,  57, LetterColor);
#define EEN     ColorLeds("een",     58,  62, LetterColor);
#define DRIE    ColorLeds("drie",    63,  68, LetterColor);
#define TWEE    ColorLeds("twee",    69,  74, LetterColor);
#define VIER    ColorLeds("vier",    75,  80, LetterColor);
#define VIJF    ColorLeds("vijf",    81,  86, LetterColor);
#define ZES     ColorLeds("zes",     87,  91, LetterColor);
#define NEGEN   ColorLeds("negen",   92,  99, LetterColor);
#define ZEVEN   ColorLeds("zeven",  100, 107, LetterColor);
#define ACHT    ColorLeds("acht",   108, 113, LetterColor);
#define TIEN    ColorLeds("tien",   114, 119, LetterColor);
#define ELF     ColorLeds("elf",    120, 124, LetterColor);
#define TWAALF  ColorLeds("twaalf", 130, 139, LetterColor);
#define UUR     ColorLeds("uur",    125, 129, LetterColor);
#define MIN1    ColorLeds("M1",     140, 140, LetterColor); 
#define MIN2    ColorLeds("M2",     141, 141, LetterColor);       //MINColor); 
#define MIN3    ColorLeds("M3",     142, 142, LetterColor);
#define MIN4    ColorLeds("M4",     143, 143, LetterColor); 
#define EDSOFT  ColorLeds("EdSoft", 144, 144, LetterColor);                                   // Optional extra LEDs Turn ON/OFF in menu with (   
                     #endif //NLM1M2M3M4L144

                     #ifdef NLM1M2M3M4L161
const uint32_t NUM_LEDS  =  161;                                                              // How many leds in  strip? + 4 for the minutes
const byte MATRIX_WIDTH  =  13;                                                               // NA Not used in this clock. 10 is fine 
const byte MATRIX_HEIGHT =  13;                                                               // NA Not used in this clock. 10 is fine 
                   
#define HET     ColorLeds("Het",      0,   4, MINColor);                                      // Special adapted clock face. IS and WAS are equal. There is no WAS
#define IS      ColorLeds("is",       5,   7, SECColor);     Is = true;                       //ColorLeds("", 4, 5, 0); 
#define WAS     ColorLeds("was",      5,   7, SECColor);     Is = false;                      // ColorLeds("", 4, 5, 0); 
#define MVIJF   ColorLeds("vijf",     8,  14, LetterColor); 
#define MVOOR   ColorLeds("voor",    15,  21, LetterColor);
#define MTIEN   ColorLeds("tien",    22,  28, LetterColor); 
#define OVER    ColorLeds("over",    29,  35, LetterColor);
#define KWART   ColorLeds("kwart",   36,  44, LetterColor);
#define OVER2   ColorLeds("over",    45,  51, LetterColor);
#define HALF    ColorLeds("half",    52,  58, LetterColor);
#define VOOR    ColorLeds("voor",    59,  65, LetterColor);
#define EEN     ColorLeds("een",     66,  70, LetterColor);
#define DRIE    ColorLeds("drie",    71,  77, LetterColor);
#define TWEE    ColorLeds("twee",    78,  84, LetterColor);
#define VIER    ColorLeds("vier",    85,  91, LetterColor);
#define VIJF    ColorLeds("vijf",    92,  98, LetterColor);
#define ZES     ColorLeds("zes",     99, 103, LetterColor);
#define NEGEN   ColorLeds("negen",  104, 112, LetterColor);
#define ZEVEN   ColorLeds("zeven",  113, 121, LetterColor);
#define ACHT    ColorLeds("acht",   122, 128, LetterColor);
#define TIEN    ColorLeds("tien",   129, 135, LetterColor);
#define ELF     ColorLeds("elf",    136, 140, LetterColor);
#define UUR     ColorLeds("uur",    141, 145, LetterColor);
#define TWAALF  ColorLeds("twaalf", 146, 156, LetterColor);
#define MIN1    ColorLeds("M1",     160, 160, LetterColor); 
#define MIN2    ColorLeds("M2",     159, 159, LetterColor);       //MINColor); 
#define MIN3    ColorLeds("M3",     158, 158, LetterColor);
#define MIN4    ColorLeds("M4",     157, 157, LetterColor); 
#define EDSOFT  ColorLeds("EdSoft", 161, 161, LetterColor);   
                     #endif //NLM1M2M3M4L161

                     #ifdef NLM1M2M3M4L256
const uint32_t NUM_LEDS  =  256;                                                              // How many leds in  strip? + 4 for the minutes
const byte MATRIX_WIDTH  =  16;                                                               // NA Not used in this clock. 16 is fine 
const byte MATRIX_HEIGHT =  16;                                                               // NA Not used in this clock. 16 is fine 
                 
#define HET     ColorLeds("Het",       0,   7, MINColor);                                     // Special adapted clock face. IS and WAS are equal. There is no WAS
#define IS      ColorLeds("is",        8,  12, SECColor);     Is = true;    //ColorLeds("", 8,10, 0); 
#define WAS     ColorLeds("was",       8,  12, SECColor);     Is = false;  // ColorLeds("", 4, 5, 0); 
#define MVIJF   ColorLeds("vijf",     13,  23, LetterColor); 
#define MVOOR   ColorLeds("voor",     24,  35, LetterColor);
#define MTIEN   ColorLeds("tien",     36,  46, LetterColor); 
#define OVER    ColorLeds("over",     47,  57, LetterColor);
#define KWART   ColorLeds("kwart",    58,  72, LetterColor);
#define OVER2   ColorLeds("over",     73,  83, LetterColor);
#define HALF    ColorLeds("half",     84,  94, LetterColor);
#define VOOR    ColorLeds("voor",     95, 105, LetterColor);
#define EEN     ColorLeds("een",     106, 113, LetterColor);
#define DRIE    ColorLeds("drie",    114, 124, LetterColor);
#define TWEE    ColorLeds("twee",    125, 135, LetterColor);
#define VIER    ColorLeds("vier",    136, 146, LetterColor);
#define VIJF    ColorLeds("vijf",    147, 157, LetterColor);
#define ZES     ColorLeds("zes",     158, 165, LetterColor);
#define NEGEN   ColorLeds("negen",   166, 180, LetterColor);
#define ZEVEN   ColorLeds("zeven",   181, 195, LetterColor);
#define ACHT    ColorLeds("acht",    196, 206, LetterColor);
#define TIEN    ColorLeds("tien",    207, 217, LetterColor);
#define ELF     ColorLeds("elf",     218, 225, LetterColor);
#define TWAALF  ColorLeds("twaalf",  234, 251, LetterColor);
#define UUR     ColorLeds("uur",     226, 233, LetterColor);
#define MIN1    ColorLeds("M1",      252, 252, LetterColor); 
#define MIN2    ColorLeds("M2",      253, 253, LetterColor);       //MINColor); 
#define MIN3    ColorLeds("M3",      254, 254, LetterColor);
#define MIN4    ColorLeds("M4",      255, 255, LetterColor); 
#define EDSOFT  ColorLeds("EdSoft",  256, 256, LetterColor);
                     #endif //NLM1M2M3M4L256
                                             #ifdef FOURLANGUAGECLOCK                                             
const uint32_t  NUM_LEDS = 625;                                                               // How many leds in  strip?
const byte MATRIX_WIDTH  =  25;                                                               // Grid size For digital display mode.
const byte MATRIX_HEIGHT =  25;                                                               // Grid size For digital display mode.
                                             #endif //FOURLANGUAGECLOCK                    
                     #ifdef NL
#define HET     ColorLeds("Het",      0,   2, LetterColor);   
#define IS      ColorLeds("is",       4,   5, LetterColor); 
#define WAS     ColorLeds("was",      8,  10, LetterColor); 
#define PRECIES ColorLeds("precies", 43,  49, LetterColor);
#define MTIEN   ColorLeds("tien",    38,  41, LetterColor); 
#define MVIJF   ColorLeds("vijf",    51,  54, LetterColor); 
#define KWART   ColorLeds("kwart",   56,  60, LetterColor);
#define VOOR    ColorLeds("voor",    96,  99, LetterColor);
#define OVER    ColorLeds("over",    90,  93, LetterColor);
#define HALF    ColorLeds("half",   100, 103, LetterColor);
#define MIDDER  ColorLeds("midder", 105, 110, LetterColor);
#define VIJF    ColorLeds("vijf",   144, 147, LetterColor);
#define TWEE    ColorLeds("twee",   138, 141, LetterColor);
#define EEN     ColorLeds("een",    150, 152, LetterColor);
#define VIER    ColorLeds("vier",   155, 158, LetterColor);
#define TIEN    ColorLeds("tien",   195, 198, LetterColor);
#define TWAALF  ColorLeds("twaalf", 188, 193, LetterColor);
#define DRIE    ColorLeds("drie",   200, 203, LetterColor);
#define NEGEN   ColorLeds("negen",  206, 210, LetterColor);
#define ACHT    ColorLeds("acht",   244, 247, LetterColor);
#define NACHT   ColorLeds("nacht",  244, 248, LetterColor);
#define ZES     ColorLeds("zes",    240, 242, LetterColor);
#define ZEVEN   ColorLeds("zeven",  251, 255, LetterColor);
#define ELF     ColorLeds("elf",    258, 260, LetterColor);
#define NOEN    ColorLeds("noen",   295, 298, LetterColor);
#define UUR     ColorLeds("uur",    290, 292, LetterColor);
#define EDSOFT  ColorLeds("EdSoft", 300, 311, LetterColor);
#define X_OFF   ColorLeds("",         0,   2, 0);
#define X_ON    ColorLeds("",         0,   2, LetterColor);
                     #endif //NL
                     #ifdef UK
#define IT      ColorLeds("| It",   347, 348, UKLetterColor);   
#define ISUK    ColorLeds("is",     344, 345, UKLetterColor); 
#define WASUK   ColorLeds("was",    340, 342, UKLetterColor);
#define EXACTUK ColorLeds("exact",  351, 355, UKLetterColor);
#define HALFUK  ColorLeds("half",   357, 360, UKLetterColor); 
#define TWENTY  ColorLeds("twenty", 394, 399, UKLetterColor); 
#define MFIVE   ColorLeds("five",   388, 391, UKLetterColor);
#define QUARTER ColorLeds("quarter",400, 406, UKLetterColor);
#define MTEN    ColorLeds("ten",    409, 411, UKLetterColor);
#define PAST    ColorLeds("past",   446, 449, UKLetterColor);
#define TO      ColorLeds("to",     443, 444, UKLetterColor);
#define SIXUK   ColorLeds("six",    439, 441, UKLetterColor);
#define TWO     ColorLeds("two",    451, 453, UKLetterColor);
#define FIVE    ColorLeds("five",   457, 460, UKLetterColor);
#define TWELVE  ColorLeds("twelve", 494, 499, UKLetterColor);
#define TEN     ColorLeds("ten",    488, 490, UKLetterColor);
#define ELEVEN  ColorLeds("eleven", 500, 505, UKLetterColor);
#define FOUR    ColorLeds("four",   507, 510, UKLetterColor);
#define NINE    ColorLeds("nine",   545, 548, UKLetterColor);
#define THREE   ColorLeds("three",  538, 542, UKLetterColor);
#define EIGHT   ColorLeds("eight",  553, 557, UKLetterColor);
#define ONE     ColorLeds("one",    597, 599, UKLetterColor);
#define SEVEN   ColorLeds("seven",  589, 593, UKLetterColor);
#define OCLOCK  ColorLeds("O'clock",605, 610, UKLetterColor);
                      #endif //UK
                      #ifdef DE
#define ES      ColorLeds("\nEs",   334, 335, DELetterColor);   
#define IST     ColorLeds("ist",    330, 332, DELetterColor); 
#define WAR     ColorLeds("war",    326, 328, DELetterColor); 
#define GENAU   ColorLeds("genau",  364, 368, DELetterColor);
#define MZEHN   ColorLeds("zehn",   370, 373, DELetterColor);
#define MFUNF   ColorLeds("funf",   383, 386, DELetterColor);
#define VIERTEL ColorLeds("viertel",375, 381, DELetterColor);
#define ZWANZIG ColorLeds("zwanzig",413, 419, DELetterColor);
#define KURZ    ColorLeds("kurz",   421, 424, DELetterColor);
#define VOR     ColorLeds("vor",    433, 435, DELetterColor);
#define NACH    ColorLeds("nach",   427, 430, DELetterColor);
#define HALB    ColorLeds("halb",   465, 468, DELetterColor);
#define FUNF    ColorLeds("funf",   471, 474, DELetterColor);
#define EINS    ColorLeds("eins",   483, 486, DELetterColor);
#define VIERDE  ColorLeds("vier",   478, 481, DELetterColor);
#define ZEHN    ColorLeds("zehn",   514, 517, DELetterColor);
#define ZWOLF   ColorLeds("zwolf",  520, 524, DELetterColor);
#define DREI    ColorLeds("drei",   532, 535, DELetterColor);
#define NEUN    ColorLeds("neun",   526, 529, DELetterColor);
#define ACHTDE  ColorLeds("acht",   565, 568, DELetterColor);
#define SECHS   ColorLeds("sechs",  570, 574, DELetterColor);
#define SIEBEN  ColorLeds("sieben", 580, 585, DELetterColor);
#define ZWEI    ColorLeds("zwei",   575, 578, DELetterColor);
#define ELFDE   ColorLeds("elf",    614, 616, DELetterColor);
#define UHR     ColorLeds("uhr",    621, 623, DELetterColor);
                      #endif //DE
                      #ifdef FR
#define IL      ColorLeds("| Il",    13,  14, FRLetterColor);   
#define EST     ColorLeds("est",     16,  18, FRLetterColor); 
#define ETAIT   ColorLeds("etait",   20,  24, FRLetterColor);
#define EXACT   ColorLeds("exact",   31,  35, FRLetterColor);
#define SIX     ColorLeds("six",     27,  29, FRLetterColor); 
#define DEUX    ColorLeds("deux",    64,  67, FRLetterColor); 
#define TROIS   ColorLeds("trois",   69,  73, FRLetterColor);
#define ONZE    ColorLeds("onze",    83,  86, FRLetterColor);
#define QUATRE  ColorLeds("quatre",  75,  80, FRLetterColor);
#define MINUIT  ColorLeds("minuit", 113, 118, FRLetterColor);
#define DIX     ColorLeds("dix",    120, 122, FRLetterColor);
#define CINQ    ColorLeds("cinq",   133, 136, FRLetterColor);
#define NEUF    ColorLeds("neuf",   129, 132, FRLetterColor);
#define MIDI    ColorLeds("midi",   125, 128, FRLetterColor);
#define HUIT    ColorLeds("huit",   164, 167, FRLetterColor);
#define SEPT    ColorLeds("sept",   171, 174, FRLetterColor);
#define UNE     ColorLeds("une",    184, 186, FRLetterColor);
#define HEURE   ColorLeds("heure",  178, 182, FRLetterColor);
#define HEURES  ColorLeds("heures", 177, 182, FRLetterColor);
#define ET      ColorLeds("et",     213, 214, FRLetterColor);
#define MOINS   ColorLeds("moins",  216, 220, FRLetterColor);
#define LE      ColorLeds("le",     222, 223, FRLetterColor);
#define DEMI    ColorLeds("demie",  229, 233, FRLetterColor);
#define QUART   ColorLeds("quart",  263, 267, FRLetterColor);
#define MDIX    ColorLeds("dix",    270, 272, FRLetterColor);
#define VINGT   ColorLeds("vingt",  282, 286, FRLetterColor);
#define MCINQ   ColorLeds("cinq",   277 ,280, FRLetterColor);
#define DITLEHEURE DitLeHeure();
                      #endif //FR