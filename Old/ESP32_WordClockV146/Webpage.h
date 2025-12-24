const char* PARAM_INPUT_1 = "input1";
#define MAXSIZE_HTML_INFO 2048
#define MAXSIZE_HTML_PAGE 6000
char html_info[MAXSIZE_HTML_INFO] = {0};
char HTML_page[MAXSIZE_HTML_PAGE] = {0};
const char index_html_top[]  = 
R"rawliteral(
<!DOCTYPE HTML><html><head>

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
    .green-bold {color: #00FF00; font-weight: bold; font-size: smaller;}
    .blue-bold {color: #0000FF; font-weight: bold; font-size: smaller;}
    .trueblue-bold {color: #0066CC;}
  </style>
  <script>
    // Handle form submission via JavaScript
    function handleFormSubmit(event) {
      event.preventDefault(); // Prevent default form submission

      const form = event.target;
      const input1Value = form.querySelector('input[name="input1"]').value;

      fetch(`/get?submit=Send&input1=${encodeURIComponent(input1Value)}`)
        .then(response => {
          window.location.href = window.location.origin; // Reload page
        })
        .catch(error => {
          console.error('Error submitting form:', error);
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
    <tr><td colspan="4"  class="center">
      <span class="trueblue-bold"><h2>ESP32-Nano Word Clock</h2></span>
    </td></tr>
    <tr>
      <td style="width: 60px"><a href="https://github.com/ednieuw/Arduino-ESP32-Nano-Wordclock">Home</a></td>
      <td style="width: 100px"><a href="https://github.com/ednieuw/Arduino-ESP32-Nano-Wordclock/blob/main/Manual-Instructions/Manual_ArduinoESP32Nano.pdf">Manual UK</a></td>
      <td style="width: 100px"><a href="https://github.com/ednieuw/Arduino-ESP32-Nano-Wordclock/blob/main/Manual-Instructions/Handleiding_ArduinoESP32Nano-NL.pdf">Manual NL</a></td>
      <td style="width: 80px"><a href="https://www.ednieuw.nl">ednieuw.nl</a></td>
    </tr>
    
  </table>
  <table style="width: 272px">
	  <tr>
		  <td><a href="/log">Log view</a></td>
		  <td><a href="/tekstdownload">All log files</a></td>
	  </tr>
  </table>
  <form id="commandForm" action="/get">
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



