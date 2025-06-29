const char* PARAM_INPUT_1 = "input1";
#define MAXSIZE_HTML_INFO 2048
#define MAXSIZE_HTML_PAGE 6000
char html_info[MAXSIZE_HTML_INFO] = {0};
char HTML_page[MAXSIZE_HTML_PAGE] = {0};
const char index_html_top[]  = 
R"rawliteral(
<!DOCTYPE HTML><html><head>
  <title>Nano ESP32 Word clock</title>
  <meta name="google" content="notranslate" />
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style type="text/css">
  a:visited {color: #FFFF00;}
  a:active {color: #FFCC00;}
  a:hover {color: #66FF33;}
  a {	color: #CC66FF;}
  .verdana-black{ font-size: smaller; font-family: Verdana, Geneva, Tahoma, sans-serif; background-color: #000000; color: #FFFFFF; }
  .center { text-align: center; }
  .verdana-red-smaller { font-size: smaller; font-family: Verdana, Geneva, Tahoma, sans-serif; background-color: FFFFFF; color: #FF0000;  }
  .verdana-red { font-family: Verdana, Geneva, Tahoma, sans-serif; background-color: FFFFFF; color: #FF0000;  } 
  .button { font-size: large; font-family: Verdana, Geneva, Tahoma, sans-serif; background-color: #FFCC00; color: #FF0000;  }
  .borders-table {	border-left: 0px solid #000000;	border-right: 0px solid #000000;	0px border-top: solid #000000;
	                border-bottom: 0px solid #000000;	font-family: Verdana, Arial, Helvetica, sans-serif;  }
  .font-smaller {font-size: smaller}
  .font-small ollownt-size: small}
  .courier {font-family: "Courier New", Courier, mono;	font-weight: bold;}
  .gray-bold {color: #999999;	font-weight: bold;}
  .red-bold {	color: #FF0000;	font-weight: bold;}
  .green-bold {color: #00FF00;	font-weight: bold;	font-size: smaller;}
  .blue-bold {color: #0000FF;	font-weight: bold;	font-size: smaller;}
  .trueblue-bold {color: #0066CC}
    </style>
  <script>
    // Function to handle form submission
    function handleFormSubmit(event) {
      event.preventDefault(); // Prevent the default form submission
      
      // Get the form data
      const form = event.target;
      const input1Value = form.querySelector('input[name="input1"]').value;
      
      // Send the data using fetch API
      fetch(`/get?submit=Send&input1=${encodeURIComponent(input1Value)}`)
        .then(response => {
          // After successful submission, redirect back to base URL
          window.location.href = window.location.origin;
        })
        .catch(error => {
          console.error('Error submitting form:', error);
        });
    }

    // Set up the date/time display
    function updateDateTime() 
    {
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

    // Initialize everything when the page loads
    window.onload = function() {
      // Add the form submit event listener
      document.getElementById('commandForm').addEventListener('submit', handleFormSubmit);
      
      // Set up the clock
      setInterval(updateDateTime, 1000);
      updateDateTime(); // Initial call
    };
  </script>
  </head>
  <body>
  <body style="color: #FFFFFF; background-color: #000000" font-family: Verdana, Geneva, Tahoma, sans-serif;>  
  <table style="width: auto" class="borders-table">
     <tr>
     <td colspan="4" class="center">
     <span class="trueblue-bold"><h3>ESP32-Nano Word Clock</h3></span>
      <tr>
       <td style="width: 60px"><a href="https://github.com/ednieuw/Arduino-ESP32-Nano-Wordclock">Home</a> </td>
       <td style="width: 100px"><a href="https://github.com/ednieuw/Arduino-ESP32-Nano-Wordclock/blob/main/Manual-Instructions/Manual_ArduinoESP32Nano.pdf">Manual UK</a></td>
       <td style="width: 100px"><a href="https://github.com/ednieuw/Arduino-ESP32-Nano-Wordclock/blob/main/Manual-Instructions/Handleiding_ArduinoESP32Nano-NL.pdf">Manual NL</a></td>       
       <td style="width: 80px"><a href="https://www.ednieuw.nl">ednieuw.nl</a></td>
     </tr>
   </table>
   <form id="commandForm" action="/get">
       <strong>     
       <input name="submit" type="submit" class="button" style="height: 40px" value="Send">
       </strong>&nbsp;
     <input type="text" name="input1" style="width: 272px">
   </form>
<span class="verdana-black">
<div id="datetime" style="text-align:left; font-family: Verdana, Geneva, Tahoma, sans-serif; font-size:16px; margin-top:20px;"></div>
</span>
<span class="verdana-black">
)rawliteral";



