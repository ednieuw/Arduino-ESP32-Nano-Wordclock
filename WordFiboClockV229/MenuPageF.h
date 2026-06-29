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
  text-transform: uppercase;
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

<h1>Fibonacci</h1>
<p class="sub"><a href="/">&#8592; Main page</a></p>

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
