//
// Fibonacci.h - all FIBONACCI-specific code in one file.
// ---------------------------------------------------------------------------
// Included once from the .ino when FIBONACCI is selected. Contains, in order:
//   1. Display layer  (was Display_Fibonacci.h)
//   2. Colour picker  HTML page  ColourPick_html[]   (was ColourPickFibo.h)
//   3. Settings menu  HTML page  menu_html[]         (was MenuPageF.h)
// ---------------------------------------------------------------------------

// ===== 1. DISPLAY LAYER =====================================================
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
uint32_t colors[][5] =                                              // The FIBONACCI colour palettes (non-const: Q3 "Own" row is filled from Mem at runtime)
   {//off   hours   minutes  both   sec in Chrono
   { white, red   , yellow , blue  , green },  // #0 Mondriaan
   { white, red   , dyellow, blue  , green },  // #1 Mondriaan1
   { white, red   , green  , blue  , green },  // #2 RGB
   { white, red   , yellow , blue  , green },  // #3 Own  - placeholder, overwritten by RefreshOwnColours() from Mem.Colour NL/UK/DE/FR + DimmedLetter
   { white, red   , grass  , purple, green },  // #4 Pastel
   { white, orange, green  , marine, blue  },  // #5 Modern
   { white, sky   , purple , blue  , green },  // #6 Cold
   { white, red   , yellow , orange, green },  // #7 Warm
   { wgray, red   , dyellow, blue  , white },  // #8 Mondriaan 2
   { white, apple , green  , grass , blue  }}; // #9 Greens

// Names shown by the 'Q' command (Q0..Q9) - Fibonacci colour palettes.
const char* const QchoiceNames[] =
  { "Mondriaan1", "Mondriaan2", "RGB", "Own", "Pastel",
    "Modern", "Cold", "Warm", "Mondriaan3", "Greens" };
const byte LASTITEM = sizeof(QchoiceNames) / sizeof(QchoiceNames[0]) - 1;   // highest valid Q index (Q0..Q9)
constexpr const char* CLOCK_VARIANT = "FIBONACCI";                                            // Build variant name (shown in version/log)


// --- Display functions (verbatim from the Fibonacci sketch) --------------- //
//--------------------------------------------                                                //
// FIBONACCI Copy the user-defined colours (set via the colour picker) into the Q3 "Own" row.
//--------------------------------------------
void RefreshOwnColours(void)
{
 colors[3][0] = Mem.ColourNL;                                                                // off
 colors[3][1] = Mem.ColourUK;                                                                // hours
 colors[3][2] = Mem.ColourDE;                                                                // minutes
 colors[3][3] = Mem.ColourFR;                                                                // both
 colors[3][4] = Mem.DimmedLetter;                                                            // sec (Chrono)
}

//--------------------------------------------
void Displaytime(void)
{
 RefreshOwnColours();                                                                        // Keep the Q3 "Own" palette in sync with Mem
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
void RotPressOne(void)   {}
void RotPressTwo(void)   {}
void RotPressThree(void) {}

// ===== 2. COLOUR PICKER PAGE (ColourPick_html[]) ============================
const char ColourPick_html[] = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>Fibonacci &ndash; LED Colour Settings</title>
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
    grid-template-columns: 64px 1fr 90px 78px;
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
  <p>Fibonacci Colour Settings</p>
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
// Fibonacci cell roles. id/key drive /setcolours (unchanged): NL/UK/DE/FR -> hours/minutes/etc, BG -> Sec.
// label/varName are just what the user sees; the mapping matches RefreshOwnColours() in Display_Fibonacci.h.
const LANGS = [
  { id:'nl', label:'OFF', varName:'Off',     packed:'%%COLNL%%' },
  { id:'uk', label:'HRS', varName:'Hours',   packed:'%%COLUK%%' },
  { id:'de', label:'MIN', varName:'Minutes', packed:'%%COLDE%%' },
  { id:'fr', label:'BOTH',varName:'Both',    packed:'%%COLFR%%' },
  { id:'bg', label:'SEC', varName:'Second',  packed:'%%COLBG%%' },
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
  card.insertAdjacentHTML('beforeend', `
  <div class="lang-row" id="row-${id}">
    <span class="badge" id="badge-${id}">${label}</span>
    <div class="picker-wrap">
      <div class="colour-preview" id="prev-${id}"></div>
      <input type="color" id="col-${id}" value="${state[id]}">
    </div>
    <span class="hex-val" id="hex-${id}"></span>
    <span class="var-name">${varName}<span class="w-tag" id="wtag-${id}">W</span></span>
  </div>`);
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
  st.className = ''; st.textContent = 'Sending…';
  fetch('/setcolours?' + params)
    .then(r => r.ok ? r.text() : Promise.reject(r.status))
    .then(() => { st.className='ok'; st.textContent='Saved ✓  '+new Date().toLocaleTimeString(); showToast('Colours saved!'); })
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
<meta name="google" content="notranslate" />
<title>Fibonacci Menu</title>
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

/* Sliders - plain grey track, coloured thumb only */
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

<h1 id="cname">Fibonacci</h1>
<p class="sub"><a href="/colourpick">&#127912; Colour Picker</a> &nbsp;|&nbsp; <a href="/">&#8592; Main page</a></p>

<!-- ===== TOP CARD: Q + Brightness + Clock + Toggles + Actions ===== -->
<div class="card">

  <!-- Q Display palette -->
  <div class="card-title">Colour Palette &mdash; Q0&ndash;Q9</div>
  <div class="q-ticks">
    <span>0</span><span>1</span><span>2</span><span>3</span><span>4</span>
    <span>5</span><span>6</span><span>7</span><span>8</span><span>9</span>
  </div>
  <input type="range" id="qslider" min="0" max="9" step="1" value="0"
         onchange="setQ(this.value)">
  <div id="qlabel">Q0 &mdash; Mondriaan1</div>

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
      <button class="tbtn" id="tf"    data-cmd="FF" onclick="togbtn(this)">Fibonacci / Chrono</button>
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
      <button class="abtn" onclick="send('G')">&#128246; Scan WIFI</button>
      <button class="abtn" onclick="send('!')">&#128336; RTC Info</button>
      <button class="abtn" onclick="send('&')">&#128257; RTC Update</button>
      <button class="abtn" onclick="send('Z')">&#128246; WPS</button>
      <button class="abtn" onclick="send('{')">&#128247; Learn IR</button>
      <button class="abtn" onclick="send('I')">&#128203; Version</button>
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
// -- Q palette slider --------------------------------------
var qnames = ['Mondriaan1','Mondriaan2','RGB','Greens','Pastel',
              'Modern','Cold','Warm','Mondriaan3','Mondriaan4'];
function updateQlabel(v) {
  document.getElementById('qlabel').textContent = 'Q' + v + ' - ' + qnames[v];
}
function setQ(v) { updateQlabel(v); send('Q' + v); }

// -- 24h clock picker --------------------------------------
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
  svg.appendChild(el('circle', {cx:CX, cy:CY, r:CR, fill:'none', stroke:'#334155', 'stroke-width':'6'}));
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
  svg.appendChild(el('path', {id:'narc', d:'', fill:'none', stroke:'#f97316', 'stroke-width':'6', 'stroke-linecap':'round'}));
  var hon = el('circle', {id:'non-h', r:'10', fill:'#22c55e', cursor:'pointer'});
  hon.addEventListener('mousedown',  function(e){e.preventDefault(); nDrag='on';});
  hon.addEventListener('touchstart', function(e){e.preventDefault(); nDrag='on';}, {passive:false});
  svg.appendChild(hon);
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
      : 'Off ' + p2(nOff) + ':00 - On ' + p2(nOn) + ':00';
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

// -- Load state from clock ---------------------------------
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
      var hw  = s.ds3231hw ? 'DS3231 installed' : 'DS3231 not found';
      var src = 'Active: ' + (s.rtcsrc || 'ESP32');
      document.getElementById('rtcstatus').textContent = hw + ' · ' + src;
      document.getElementById('tj').textContent = s.ds3231hw ? 'Use DS3231 RTC' : 'Use ESP32 RTC';
    })
    .catch(function() {});
}
loadState();

// -- Button handlers ---------------------------------------
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

// -- Log window --------------------------------------------
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
