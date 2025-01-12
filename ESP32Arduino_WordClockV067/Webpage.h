const char* PARAM_INPUT_1 = "input1";
#define MAXSIZE_HTML_INFO 2048
#define MAXSIZE_HTML_PAGE 10000
char html_info[MAXSIZE_HTML_INFO] = {0};
char HTML_page[MAXSIZE_HTML_PAGE] = {0};
const char index_html_top[]  = 
R"rawliteral(
<!DOCTYPE HTML><html><head>
  <title>Nano ESP32 Word clock</title>
  <meta name="google" content="notranslate" />
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style type="text/css">
  .verdana-black{ font-family: Verdana, Geneva, Tahoma, sans-serif; background-color: #FFFFFF;  }
  .center { text-align: center; }
  .verdana-red { font-family: Verdana, Geneva, Tahoma, sans-serif; background-color: #FFFFFF; color: #FF0000;  }
  .verdana-large-red { font-size: large; font-family: Verdana, Geneva, Tahoma, sans-serif; background-color: #FFFFFF; color: #FF0000;  }
  .borders-table {	border-left: 0px solid #000000;	border-right: 0px solid #000000;	0px border-top: solid #000000;
	                border-bottom: 0px solid #000000;	font-family: Verdana, Arial, Helvetica, sans-serif;  }
  .font-smaller {font-size: smaller}
  .font-small {font-size: small}
  .courier {	font-family: "Courier New", Courier, mono;	font-weight: bold;}
  .gray-bold {	color: #999999;	font-weight: bold;}
  .red-bold {	color: #FF0000;	font-weight: bold;}
  .green-bold {	color: #00FF00;	font-weight: bold;	font-size: smaller;}
  .blue-bold {	color: #0000FF;	font-weight: bold;	font-size: smaller;}
  .trueblue-bold {color: #0066CC}
    </style>
  </head>
  <body>
   <table style="width: auto" class="borders-table">
     <tr>
     <td colspan="3" class="center">
   <span class="verdana-large-red"><a href="https://github.com/ednieuw/Arduino-ESP32-Nano-Wordclock">ESP32-Nano Word Clock</a></span> 
     <tr>
     </tr>
      <tr>
       <td style="width: 50px">&nbsp;</td>
       <td style="width: 125px"><a href="https://github.com/ednieuw/Arduino-ESP32-Nano-Wordclock/blob/main/Manual_ArduinoESP32Nano.pdf">Manual</a></td>
       <td style="width: 125px"><a href="https://www.ednieuw.nl">ednieuw.nl</a></td>
     </tr>
   </table>
   <form action="/get">
       <strong>     
       <input name="submit" type="submit" class="verdana-red" style="height: 22px" value="Send">
       </strong>&nbsp;
     <input type="text" name="input1" style="width: 272px"></form>
<span class="verdana-black">
)rawliteral";

const char index_html_footer[]  = 
R"rawliteral(
<br>
</span>
</body></html>
)rawliteral";