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
    text-transform: uppercase;
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
  <h1>Word Clock</h1>
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