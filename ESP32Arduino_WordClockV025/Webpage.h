const char* PARAM_INPUT_1 = "input1";
const char index_html[]  = 
R"rawliteral(
<!DOCTYPE HTML><html><head>
  <title>Nano ESP32 Word clock</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style type="text/css">
  .auto-style1 {
    font-family: Verdana, Geneva, Tahoma, sans-serif;
    background-color: #FFFFFF;
  }
  .auto-style2 {
    text-align: center;
  }
  .auto-style3 {
    font-family: Verdana, Geneva, Tahoma, sans-serif;
    background-color: #FFFFFF;
    color: #FF0000;
  }
  .auto-style11 {
	border-left: 0px solid #000000;
	border-right: 0px solid #000000;
	border-top: thin solid #000000;
	border-bottom: thin solid #000000;
	font-family: Verdana, Arial, Helvetica, sans-serif;
  }
  .auto-style15 {
    border-top-style: none;
    border-top-width: medium;
  }

  .style1 {font-size: smaller}
  .style2 {font-size: small}
  .style3 {
	font-family: "Courier New", Courier, mono;
	font-weight: bold;
}
  </style>
  </head>
  <body>
   <table style="width: auto" class="auto-style11">
     <tr>
       <td class="auto-style2" colspan="3">
   <span class="auto-style3"><strong><a href="https://github.com/ednieuw/Arduino-ESP32-Nano-Wordclock">ESP32-Nano Word Clock</a></strong></td>
     </tr>
     <tr>
       <td width="123" style="width: 108px"> <strong>A</strong> SSID</td>
       <td width="98" style="width: 98px"><strong>B</strong> Password</td>
       <td width="157" style="width: 125px"><strong>C</strong> BLE beacon</td>
     </tr>
     <tr>
       <td colspan="3"><strong>D</strong> Date <span class="auto-style4 style1">(D15012021)</span><strong>&nbsp; T</strong> Time                <span class="auto-style4 style1">(T132145)</span></td>
     </tr>
     <tr>
       <td colspan="3" class="auto-style16"><strong>E</strong> Set Timezone <span class="auto-style4 style1"> E<-02>2 or E<+01>-1</span></td>
     </tr>
     <tr>
       <td colspan="3" class="auto-style16"><strong>J</strong> Toggle use of DS3231 time module </td>
     </tr>
     <tr>
       <td colspan="3" class="auto-style16"><div align="center" class="style3">Language</div></td>
     </tr>
     <tr>
       <td style="width: 98px" class="auto-style15"><strong>L0</strong>&nbsp;NL</td>
       <td class="auto-style15" style="width: 125px"> <strong>L1</strong> UK</td>
       <td class="auto-style15" style="width: 125px"><strong>L2</strong> DE</td>
     </tr>
     <tr>
       <td style="width: 98px"><strong>L3</strong> FR</td>
       <td style="width: 125px"><strong>L4</strong> Wheel</td>
       <td style="width: 125px">&nbsp;</td>
     </tr>
     <tr>
       <td colspan="3"><strong>N</strong> Display Off between Nhhhh <span class="auto-style4 style1">
       (N2208)</span></td>
     </tr>
     <tr>
       <td colspan="3" style="height: 22px"><strong>O</strong> Display <span class="auto-style4"> toggle On/Off</span>  <span class="auto-style4"><strong> </strong></span></td>
     </tr>
     <tr>
       <td colspan="3" style="height: 22px"><span class="auto-style4"><strong>P</strong> Status LED toggle On/Off </span></td>
     </tr>
     <tr>
       <td colspan="3"><strong>F</strong> Own colour design <span class="auto-style4">(0-F)&nbsp; 
           <span class="style1">(P00FF00)</span></span></td>
     </tr>
     <tr>
       <td colspan="3"> <div align="center" class="style3">Display colour choice</div></td>
     </tr>
     <tr>
       <td style="width: 108px"><strong>Q0</strong> Yellow</td>
       <td style="width: 98px"><strong>Q1</strong> Hourly</td>
       <td style="width: 125px"><strong>Q2</strong> White</td>
     </tr>
     <tr>
       <td style="width: 108px"><strong>Q3</strong> All Own</td>
       <td style="width: 98px"><strong>Q4</strong> Own</td>
       <td style="width: 125px"><strong>Q5</strong> Wheel</td>
     </tr>
	  <tr>
       <td colspan="3"> <div align="center"> <span class="style3">Light intensity setting</span> <span class="style1">(0-255) </span></div></td>
     </tr>
     <tr>
       <td style="width: 108px"><strong>S</strong> Slope </td>
       <td style="width: 98px"><strong>V</strong> Min </td>
       <td style="width: 125px"><strong>U</strong> Max </td>
     </tr>
     <tr>
       <td colspan="3" style="width: 108px"><div align="center" class="style3">Communication</div></td>
     </tr>
     <tr>
       <td style="width: 108px"><strong>W</strong> WIFI</td>
       <td style="width: 98px"> <strong>X</strong> NTP<span class="auto-style4"><strong>&amp;</strong></span><br class="auto-style4">       </td>
       <td style="width: 125px">   <strong>Y</strong> BLE</td>
     </tr>
     <tr>
       <td style="width: 108px"><strong>R</strong> Reset</td>
       <td style="width: 98px"><strong>@</strong> Restart<br class="auto-style4">       </td>
       <td style="width: 125px"><strong>Z</strong> Fast BLE</td>
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
    
<br class="auto-style1">
</body></html>
)rawliteral";
