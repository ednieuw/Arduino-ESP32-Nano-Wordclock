const char* PARAM_INPUT_1 = "input1";
#define MAXSIZE_HTML_INFO 6000
#define MAXSIZE_HTML_PAGE 10000
char html_info[MAXSIZE_HTML_INFO] = {0};
char HTML_page[MAXSIZE_HTML_PAGE] = {0};
const char index_html_top[]  = 
R"rawliteral(
<!DOCTYPE HTML>
<html lang="en">
<head>
  <meta name="google" content="notranslate" />
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style type="text/css">
    a:visited {color: #FFFF00;}
    a:active {color: #FFCC00;}
    a:hover {color: #66FF33;}
    a {color: #CC66FF;}
    .verdana-black {font-size: smaller; font-family: Verdana, Geneva, Tahoma, sans-serif; background-color: #000000; color: #FFFFFF;}
    .center {text-align: center;}
    .verdana-red-smaller {font-size: smaller; font-family: Verdana, Geneva, Tahoma, sans-serif; background-color: #000000; color: #FF0000;}
    .verdana-red {font-family: Verdana, Geneva, Tahoma, sans-serif; background-color: #000000; color: #FF0000;}
    .button {font-size: large; font-family: Verdana, Geneva, Tahoma, sans-serif; background-color: #FFCC00; color: #FF0000;}
    .borders-table {border: 0px solid #000000; font-family: Verdana, Arial, Helvetica, sans-serif; text-align: center; font-size: 14px;}
    .courier {font-family: "Courier New", Courier, mono; font-weight: bold;}
    .gray-bold {color: #999999; font-weight: bold;}
    .red-bold {color: #FF0000; font-weight: bold;}
    .green-bold {color: #00FF00; font-weight: bold;}
    .blue-bold {color: #0000FF; font-weight: bold;}
    .trueblue-bold {color: #0066CC;}
    .darkorange {color: #FF8C00;}
    .verdana-warning { font-family: Verdana, Geneva, Tahoma, sans-serif; background-color: #000000;
                       color: #ff8800;  font-weight: bold;  animation: flashOrange 1s infinite; }

@keyframes flashOrange {
    0%   { color: #ff8800; }
    50%  { color: #000000; }   /* off (black background) */
    100% { color: #ff8800; }
}

.verdana-warningbg { font-family: Verdana, Geneva, Tahoma, sans-serif; background-color: #000000;
                       color: #ff8800; font-weight: bold;  animation: flashOrangeBg 1s infinite;}

@keyframes flashOrangeBg {
    0%   { background-color: #000000; color: #ff8800; }
    50%  { background-color: #ff8800; color: #000000; }
    100% { background-color: #000000; color: #ff8800; }
}
  </style>
  <script>
    // Handle form submission via JavaScript
    function handleFormSubmit(event) {
      event.preventDefault(); // Prevent default form submission

      const form = event.target;
      const input1Value = form.querySelector('input[name="input1"]').value;
      const url = `/get?submit=Send&input1=${encodeURIComponent(input1Value)}`;

      fetch(url)
        .then(response => {
          window.location.href = window.location.origin; // Reload page
        })
        .catch(error => {
          console.warn('First fetch failed (stale connection?), retrying...', error);
          // Retry once on network error (e.g. stale keep-alive connection after idle)
          fetch(url)
            .then(response => {
              window.location.href = window.location.origin; // Reload page
            })
            .catch(error2 => {
              console.error('Retry also failed:', error2);
              alert('Could not reach the clock. Check WiFi connection.');
            });
        });
    }

    // Update date/time display
    function updateDateTime() {
      const now = new Date();
      const day = String(now.getDate()).padStart(2, '0');
      const monthNames = ["Jan", "Feb", "Mar", "Apr", "May", "Jun",
                          "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"];
      const month = monthNames[now.getMonth()];
      const year = now.getFullYear();
      const hours = String(now.getHours()).padStart(2, '0');
      const minutes = String(now.getMinutes()).padStart(2, '0');
      const seconds = String(now.getSeconds()).padStart(2, '0');
      const formatted = `${day}-${month}-${year} ${hours}:${minutes}:${seconds}`;
      document.getElementById('datetime').innerText = formatted;
    }

    // Initialize on page load
    window.onload = function() {
      document.getElementById('commandForm').addEventListener('submit', handleFormSubmit);
      setInterval(updateDateTime, 1000);          // Updates the digital clock on the web page every second 
      updateDateTime();
      document.getElementById('input1').focus();  // Automatically focus the input
    };
  </script>
</head>
<body style="color: #FFFFFF; background-color: #000000; font-family: Verdana, Geneva, Tahoma, sans-serif;">
  <table class="borders-table">
    <tr>
      <td style="width: 60px"><a href="https://github.com/ednieuw/Arduino-ESP32-Nano-Wordclock">Home</a></td>
      <td style="width: 95px"><a href="https://github.com/ednieuw/Arduino-ESP32-Nano-Wordclock/blob/main/Manual-Instructions/Manual_ArduinoESP32Nano.pdf">Manual UK</a></td>
      <td style="width: 95px"><a href="https://github.com/ednieuw/Arduino-ESP32-Nano-Wordclock/blob/main/Manual-Instructions/Handleiding_ArduinoESP32Nano-NL.pdf">Manual NL</a></td>
      <td style="width: 90px"><a href="https://www.ednieuw.nl">ednieuw.nl</a></td>
    </tr>
    <tr>
        <td style="width: 60px"><label><input type="checkbox" id="logToggle" checked> Log</label></td>
        <td style="width: 95px"><a href="/log">Log view</a></td>
        <td style="width: 95px"><a href="/tekstdownload">All log files</a></td>
        <td style="width: 90px"><a href=/menu>&#9776; Menu</a></td>
    </tr>
  </table>
  <textarea id="lastlog" readonly style="font-family:monospace; font-size:12px; color:#aaa; background-color:#111; border:1px solid #444; padding:4px; box-sizing:border-box; height:100px; width:354px; resize:none; margin-bottom:8px;"></textarea>
  <script>
    var logEnabled = true;
    var logTimer = null;
    document.getElementById('logToggle').addEventListener('change', function() {
      logEnabled = this.checked;
      var el = document.getElementById('lastlog');
      if (!logEnabled) { clearTimeout(logTimer); el.style.display = 'none'; }
      else { el.style.display = ''; fetchLastLog(); }
    });
    function fetchLastLog() {
      if (!logEnabled) return;
      fetch('/tekstprint').then(function(r){return r.text();}).then(function(t){
        var lines = t.split('\n').filter(function(l){return l.trim()!='';}); 
        var el = document.getElementById('lastlog');
        var atBottom = (el.scrollHeight - el.scrollTop - el.clientHeight) < 30;
        el.value = lines.slice(-50).join('\n');
        if (atBottom) el.scrollTop = el.scrollHeight;
      }).catch(function(){});
      logTimer = setTimeout(fetchLastLog, 1000);
    }
    fetchLastLog();
  </script>
  <form id="commandForm" >
    <strong>
      <input name="submit" type="submit" class="button" style="height: 40px" value="Send">
    </strong>&nbsp;
    <input type="text" id="input1" name="input1" style="width: 272px">
  </form>
  <span class="verdana-black">
    <div id="datetime" style="text-align:left; font-size:16px; margin-top:20px;"></div>
  </span>
  <span class="verdana-black">

)rawliteral";



