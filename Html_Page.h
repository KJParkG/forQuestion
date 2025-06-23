// HTML 페이지
static const char HTML_head01[] PROGMEM = R"==(<!DOCTYPE html><html>)==";
static const char CSS01[] PROGMEM = R"==(
<style>html {font-family: Arial, Helvetica, sans-serif; display: inline-block; text-align: center;}h1 {font-size: 20px; color: white; text-align: center;}co {font-size: 10px; color: #3e94ec; text-align: center;}msg {font-size: 14px; color: #3e94ec; text-align: center;}
body {background-color: #3e94ec;font-family: helvetica, arial, sans-serif;font-size: 12px;text-rendering: optimizeLegibility;}.table-fill {background: white;border-radius:3px;border-collapse: collapse;margin: auto;max-width: 600px;padding:5px;width: 100%;box-shadow: 0 5px 10px rgba(0, 0, 0, 0.1);animation: float 5s infinite;}
tr {border-top: 1px solid #C1C3D1;border-bottom-: 1px solid #C1C3D1;color:#666B85;font-weight:normal;text-shadow: 0 1px 1px rgba(256, 256, 256, 0.1);}
tr:hover td {background:#4E5066;color:#FFFFFF;border-top: 1px solid #22262e;}tr:first-child {border-top:none;}
tr:last-child {border-bottom:none;}tr:nth-child(odd) td {background:#EBEBEB;}tr:nth-child(odd):hover td {background:#4E5066;}tr:last-child td:first-child {border-bottom-left-radius:3px;}
tr:last-child td:last-child {border-bottom-right-radius:3px;}td {background:#FFFFFF;padding:5px;text-align:left;vertical-align:middle;text-shadow: -1px -1px 1px rgba(0, 0, 0, 0.1);border-right: 1px solid #C1C3D1;}
td:last-child {border-right: 0px;}td.text-left {text-align: left;}
input[type=submit] {border: none;color: #FEFCFB;background-color: #034078;padding: 10px 10px;text-align: center;text-decoration: none;display: inline-block;font-size: 16px;width: 100px;margin-right: 10px;border-radius: 4px;transition-duration: 0.4s;}
input[type=submit]:hover {background-color: #1282A2;}
input[type=text], input[type=number], select {width: 100%;padding: 5px 5px;margin: 2px;display: inline-block;border: 1px solid #ccc;border-radius: 5px;box-sizing: border-box;}
</style>)==";
static const char CSS02[] PROGMEM = R"==(
<style>html {font-family: Arial, Helvetica, sans-serif; display: inline-block; text-align: center;}h1 {font-size: 20px; color: white; text-align: center;}co {font-size: 10px; color: #3e94ec; text-align: center;}msg {font-size: 14px; color: #3e94ec; text-align: center;}
body {background-color: #3e94ec;font-family: helvetica, arial, sans-serif;font-size: 12px;text-rendering: optimizeLegibility;}.table-fill {background: white;border-radius:3px;border-collapse: collapse;margin: auto;max-width: 600px;padding:5px;width: 100%;box-shadow: 0 5px 10px rgba(0, 0, 0, 0.1);animation: float 5s infinite;}
tr {border-top: 1px solid #C1C3D1;border-bottom-: 1px solid #C1C3D1;color:#666B85;font-weight:normal;text-shadow: 0 1px 1px rgba(256, 256, 256, 0.1);}
tr:hover td {background:#4E5066;color:#FFFFFF;border-top: 1px solid #22262e;}tr:first-child {border-top:none;}
tr:last-child {border-bottom:none;}tr:nth-child(odd) td {background:#EBEBEB;}tr:nth-child(odd):hover td {background:#4E5066;}tr:last-child td:first-child {border-bottom-left-radius:3px;}
tr:last-child td:last-child {border-bottom-right-radius:3px;}td {background:#FFFFFF;padding:5px;text-align:left;vertical-align:middle;text-shadow: -1px -1px 1px rgba(0, 0, 0, 0.1);border-right: 1px solid #C1C3D1;}
td:last-child {border-right: 0px;}td.text-left {text-align: left;}
input[type=submit] {border: none;color: #FEFCFB;background-color: #034078;padding: 10px 10px;text-align: center;text-decoration: none;display: inline-block;font-size: 16px;width: 100px;margin-right: 10px;border-radius: 4px;transition-duration: 0.4s;}
input[type=submit]:hover {background-color: #1282A2;}
input[type=text], input[type=number], select {width: 100%;padding: 5px 5px;margin: 2px;display: inline-block;border: 1px solid #ccc;border-radius: 5px;box-sizing: border-box;}
.slidecontainer {width: 100%;margin-top: 10px;}
.slider {-webkit-appearance: none;width: 100%;height: 15px;border-radius: 5px;background: #ccc;outline: none;}
.slider::-webkit-slider-thumb {-webkit-appearance: none;appearance: none;width: 25px;height: 25px;border-radius: 50%;background: #FFFFFF;cursor: pointer;}
.slider::-moz-range-thumb {width: 25px;height: 25px;border-radius: 50%;background: #FFFFFF;cursor: pointer;}
</style>)==";
static const char index_html0[] PROGMEM = R"==(
<head><meta charset="UTF-8"><title>FarmSonic Setup Manager</title><meta name="viewport" content="width=device-width, initial-scale=1"></head>
<body>
<h1>FarmSonic Setup Manager</h1><form action="/setup" method="POST"><table class="table-fill"><tr>
<td class="text-left">Wi-Fi : SSID</td><td class="text-left"><input type="text" id ="ssid" name="ssid" value=")==";
static const char index_html1[] PROGMEM = R"==("></td></tr><tr><td class="text-left">Wi-Fi : Password</td><td class="text-left"><input type="text" id ="pass" name="pass" value=")==";
static const char index_html2[] PROGMEM = R"==("></td></tr><tr><td class="text-left">Device ID</td><td class="text-left"><input type="text" id ="did" name="did" value=")==";
static const char index_html3[] PROGMEM = R"==("></td></tr><tr><td class="text-left">DataCenter URL<BR>Env. Data</td><td class="text-left"><input type="text" id ="dsu" name="dsu" value=")==";
static const char index_html4[] PROGMEM = R"==("></td></tr><tr><td class="text-left">DataCenter<BR>Data Upload Path</td><td class="text-left"><input type="text" id ="dsu1" name="dsu1" value=")==";
static const char index_html5[] PROGMEM = R"==("></td></tr><tr><td class="text-left">DataCenter<BR>Audio Upload Path</td><td class="text-left"><input type="text" id ="dsu2" name="dsu2" value=")==";
static const char index_html6[] PROGMEM = R"==("></td></tr><tr><td class="text-left">DataCenter<BR>Server Port</td><td class="text-left"><input type="text" id ="dsp" name="dsp" value=")==";
static const char index_html7[] PROGMEM = R"==("></td></tr><tr><td class="text-left">Measurement cycle (MIN)</td><td class="text-left"><input type="text" id ="ctm" name="ctm" value=")==";
static const char index_html8[] PROGMEM = R"==("></td></tr><tr><td class="text-left">Sound detection Level</td><td class="text-left"><div class="slidecontainer"><input type="range" min="1" max="10" value=")==";
static const char index_html9[] PROGMEM = R"==(" class="slider" id="dsv" name="dsv"><p>MIN Level : <span id="value"></span></p></div>)==";
static const char index_html10[] PROGMEM = R"==(</td></tr><tr><td colspan=2 style="text-align: center"><input type ="submit" value ="SAVE"><BR><co>(C)Copyright 2024. AtoZSystem.co.kr<co></td></tr></table></form><P style="font-size: 12px;color: white;text-align: left">)==";
static const char index_html11[] PROGMEM = R"==(</P><script>var slider = document.getElementById("dsv");var output = document.getElementById("value");output.innerHTML = slider.value;slider.oninput = function() {output.innerHTML = this.value;}</script>)==";
static const char index_html12[] PROGMEM = R"==(</body></html>)==";

static const char Save_OK01[] PROGMEM = R"==(<!DOCTYPE html><html><style>html {font-family: Arial, Helvetica, sans-serif;display: inline-block;text-align: center;}
body {background-color: #3e94ec;font-family: arial, sans-serif;font-size: 16px;text-rendering: optimizeLegibility;}
.table-title h3 {color: #fafafa;font-size: 30px;text-align:center;font-style:normal;font-family: arial, sans-serif;text-shadow: -1px -1px 1px rgba(0, 0, 0, 0.1);}
.table-title h2 {color: #FFFF00;font-size: 24px;text-align:center;font-style:normal;font-family: arial, sans-serif;text-shadow: -1px -1px 1px rgba(0, 0, 0, 0.1);}</style>
<head><meta charset="UTF-8"><title>FarmSonic Setup Manager</title><meta name="viewport" content="width=device-width, initial-scale=1"></head>
<body onload="mycnt()"><div class="table-title"><h3>)==";
static const char Save_OK02[] PROGMEM = R"==(</h3><h2>Auto restart in <a id="cnt">10</a> seconds..</h2></div>
<script>function mycnt(){var Tcnt = 10;let Disp_Cnt = setInterval(function(){if(Tcnt >= 0){document.getElementById("cnt").innerHTML = Tcnt;Tcnt--;}else{clearInterval(Disp_Cnt);}}, 1000)}</script></body></html>)==";

static const char init_html0[] PROGMEM = R"==(
<head><meta charset="UTF-8"><title>FarmSonic Device Initialization</title><meta name="viewport" content="width=device-width, initial-scale=1"></head>
<body>
<h1>FarmSonic Device Initialization</h1><form action="/reset_r" method="POST"><table class="table-fill"><tr>
<td class="text-left">Device ID</td><td class="text-left"><input type="text" id ="did" name="did" value=")==";
static const char init_html1[] PROGMEM = R"==("></td></tr><tr><td colspan=2 class="text-left"><msg>※ 공장출고 상태로 초기화 하시겠습니까 ?<BR>( Do you want to reset the information to factory settings ? )<msg></td></tr>
<tr><td colspan=2 style="text-align: center"><input type ="submit" value ="RESET"><BR><co>(C)Copyright 2024. AtoZSystem.co.kr<co></td></tr></table></form></body></html>)==";
