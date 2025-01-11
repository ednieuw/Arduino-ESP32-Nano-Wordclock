const char* PARAM_INPUT_1 = "input1";
char html_info[4096] = {0};
 char HTML_page[10000] ={0};
const char index_html_top[]  = 
R"rawliteral(
<!DOCTYPE HTML><html><head>
  <title>Nano ESP32 Word clock</title>
  <meta name="google" content="notranslate" />
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style type="text/css">
  .auto-style1 { font-family: Verdana, Geneva, Tahoma, sans-serif; background-color: #FFFFFF;  }
  .auto-style2 { text-align: center; }
  .auto-style3 { font-family: Verdana, Geneva, Tahoma, sans-serif; background-color: #FFFFFF; color: #FF0000;  }
  .auto-style11 {	border-left: 0px solid #000000;	border-right: 0px solid #000000;	border-top: thin solid #000000;
	                border-bottom: thin solid #000000;	font-family: Verdana, Arial, Helvetica, sans-serif;  }
  .style1 {font-size: smaller}
  .style2 {font-size: small}
  .style3 {	font-family: "Courier New", Courier, mono;	font-weight: bold;}
  .style4 {	color: #999999;	font-weight: bold;}
  .style5 {	color: #FF0000;	font-weight: bold;	font-size: smaller;}
  .style7 {	color: #00FF00;	font-weight: bold;	font-size: smaller;}
  .style8 {	color: #0000FF;	font-weight: bold;	font-size: smaller;}
  .style9 {color: #0066CC}
  .style10 {font-family: "Courier New", Courier, mono; font-weight: bold; color: #0066CC; }  </style>
  </head>
  <body>
   <table style="width: auto" class="auto-style11">
     <tr>
       <td colspan="3" class="auto-style2">
   <span class="auto-style3"><a href="https://github.com/ednieuw/Arduino-ESP32-Nano-Wordclock">ESP32-Nano Word Clock</a> 
     <tr>
       <td width="123" style="width: 108px">&nbsp;</td>
       <td width="98" style="width: 98px">&nbsp;</td>
       <td width="157" style="width: 125px">&nbsp;</td>
     </tr>
      <tr>
       <td style="width: 108px"><a href="https://github.com/ednieuw/Arduino-ESP32-Nano-Wordclock/blob/main/Manual_ArduinoESP32Nano.pdf">Manual</a></td>
       <td style="width: 98px">&nbsp;</td>
       <td style="width: 125px"><a href="https://www.ednieuw.nl" class="style2">ednieuw.nl</a></td>
     </tr>
   </table>
   <form action="/get">
       <strong>     
       <input name="submit" type="submit" class="auto-style3" style="height: 22px" value="Send">
       </strong>&nbsp;
     <input type="text" name="input1" style="width: 272px"></form><br>
<br>
<span class="auto-style1">
)rawliteral";

const char index_html_footer[]  = 
R"rawliteral(
<br>
</span>
</body></html>
)rawliteral";