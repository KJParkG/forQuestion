// HTML 페이지
/* Wi-Fi 접속 */
bool _Set_Wifi(){
  bool __Ret = false;  
	//WiFi 접속
  WiFi.disconnect(true); // Clear
	WiFi.begin(_EDI.ssid, _EDI.pass);
  delay(1000); 
  yield();

  for(int i = 0; i < 10; i++){
    if (WiFi.status() == WL_CONNECTED) {
      Serial.print("\nWifi IP: ");
      Serial.println(WiFi.localIP());  //Show ESP32 IP on serial     
      delay(1000);
      __Ret = true;
      break;
    }
    _Put_SysMsg("W");      
    _Set_Led(4);      
    delay(1000);
    yield();      
  }
  return __Ret;
}

/* Wi-Fi Connect Check */
bool _Check_Wifi(){
	//WiFi 접속
  bool __Ret = true;
  if (WiFi.status() != WL_CONNECTED) {  //if we lost connection, retry
    _Put_SysMsg("WiFi ReConn..."); 
    if(!_Set_Wifi()){
      __Ret=false;
    }
  }
  return __Ret;
}

/* AP Mode start */
void _Set_ApMode(){
  /* Put your SSID & Password */
  const char* _ap_ssid = "FarmSonic_AP";  // Enter SSID here
  const char* _ap_password = "admin1234";  //Enter Password here

  /* Put IP Address details */
  IPAddress _ap_local_ip(192,168,1,1);
  IPAddress _ap_gateway(192,168,1,1);
  IPAddress _ap_subnet(255,255,255,0);

  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(_ap_ssid, _ap_password);
  WiFi.softAPConfig(_ap_local_ip, _ap_gateway, _ap_subnet); 
  delay(100);  
}

/* Web Server 설정 */
bool _Init_WebServer(){
	//페이지 요청 처리 함수 등록
	w_server.on("/", Handle_Root);
  w_server.on("/setup", Handle_Setup);
  w_server.on("/reset", Handle_Reset);  
  w_server.on("/reset_r", Handle_Reset_r);  
  w_server.onNotFound(HandleNotFound);

  w_server.begin();
	_Put_SysMsg("HTTP Server Start");
  delay(1000);
  return true;
}

/* Web Server 종료 */
void _End_WebServer(){
  w_server.close();
	_Put_SysMsg("HTTP Server End");
}

/*
** AP Mode Index Page
*/
void Handle_Root()
{
  String index_html = HTML_head01;
  index_html += CSS02;
  index_html += index_html0;

  index_html += _EDI.ssid;
  index_html += index_html1;
  index_html += _EDI.pass;
  index_html += index_html2;
  index_html += _EDI.did;
  index_html += index_html3;
  index_html += _EDI.dsu;
  index_html += index_html4;
  index_html += _EDI.dsu1;
  index_html += index_html5;
  index_html += _EDI.dsu2;
  index_html += index_html6;
  index_html += _EDI.dsp;
  index_html += index_html7;
  index_html += _EDI.ctm;
  index_html += index_html8;
  index_html += _EDI.dsv;
  index_html += index_html9;
  index_html += index_html10;  
  index_html += _Sys_Log;
  index_html += index_html11;
  index_html += index_html12;

  w_server.send(200, "text/html", index_html);
}

/*
** AP Mode, Deive Info Reset Page
*/
void Handle_Reset()
{
  String index_html = HTML_head01;
  index_html += CSS01;
  index_html += init_html0;

  index_html += _EDI.did;
  index_html += init_html1;

  w_server.send(200, "text/html", index_html);
}

/*
** AP Mode, submit Setup Page
*/
void Handle_Setup()
{
  int i = 0;
  String Html_Str = Save_OK01;
  Html_Str += "You have successfully saved the settings information.";
  Html_Str += Save_OK02;

  if (w_server.args() == 0) {
    return w_server.send(500, "text/plain", "BAD ARGS");
  }else{
    for (i = 0; i < w_server.args(); i++) {
      if (w_server.argName(i) == W_arg_1) {
        _EDI.ssid  = w_server.arg(i).c_str();
      }else if (w_server.argName(i) == W_arg_2) {
        _EDI.pass  = w_server.arg(i).c_str();
      }else if (w_server.argName(i) == W_arg_3) {
        _EDI.did  = w_server.arg(i).c_str();
      }else if (w_server.argName(i) == W_arg_4) {
        _EDI.dsu  = w_server.arg(i).c_str();
      }else if (w_server.argName(i) == W_arg_5) {
        _EDI.dsu1  = w_server.arg(i).c_str();
      }else if (w_server.argName(i) == W_arg_6) {
        _EDI.dsu2  = w_server.arg(i).c_str();
      }else if (w_server.argName(i) == W_arg_7) {
        _EDI.dsp  = w_server.arg(i).toInt();
      }else if (w_server.argName(i) == W_arg_8) {
        _EDI.ctm  = w_server.arg(i).toInt();
      }else if (w_server.argName(i) == W_arg_9) {
        _EDI.dsv  = w_server.arg(i).toInt();
      }   
    }
    _ret = Set_Device_info();
    _Put_SysMsg("Request Value Set");    
  }
  w_server.send(200, "text/html", Html_Str);  
  delay(10000); // Reboot전 10초간의 기둘림.
  ESP.restart();
}

/*
** AP Mode, submit reset Page
*/
void Handle_Reset_r()
{
  String Html_Str = Save_OK01;
  Html_Str += "It has been reset to factory settings.";
  Html_Str += Save_OK02;

  if (w_server.args() == 0) {
    return w_server.send(500, "text/plain", "BAD ARGS");
  }else{
    if(w_server.argName(0) == "did") {
      _EDI.did  = w_server.arg(0).c_str();
    }else{
      return w_server.send(500, "text/plain", "BAD ARGS");
    }      
    _ret = ReSet_Device_info();
    _Put_SysMsg("Request Value Set");
  }
  w_server.send(200, "text/html", Html_Str);  
  delay(10000); // Reboot전 10초간의 기둘림.
  ESP.restart();
}

/**
*** Server Call Error Routine ***
**/
void HandleNotFound() {
//  digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += w_server.uri();
  message += "\nMethod: ";
  message += (w_server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += w_server.args();
  message += "\n";

  for (uint8_t i = 0; i < w_server.args(); i++) {
    message += " " + w_server.argName(i) + ": " + w_server.arg(i) + "\n";
  }

  w_server.send(404, "text/plain", message);
}

// Http PosT Multipart 전송을 위한 Head 만들기.
String header_str(int _length){
  String  _data;

  _data =  F("POST ");
  _data += _EDI.dsu2;
  _data += F(" HTTP/1.1\r\n");
  _data += F("HOST: ");
  _data += _EDI.dsu;
  _data += F("\r\n");
  _data += F("Content-Type: multipart/form-data; boundary=");
  _data += _Boundary;
  _data += F("\r\n");
  _data += F("Content-Length: ");
  _data += String(_length);
  _data += F("\r\n");
  _data += F("Connection: close\r\n\r\n");  
  return (_data);
}

// Send Body 생성.
String body_str(String _name , String _val){
  String _data;
  _data = "--";
  _data += _Boundary;
  _data += F("\r\n");
  if (_name == "wav"){
    // Name = wav 이면 audio 파일 전송을 위한 Body 생성.
    _data += "Content-Disposition: form-data; name=\"awfile\"; filename=\"" + _val + "\"\r\n";
    _data += "Content-Type: audio/wav\r\n\r\n";
  }
  else
  {
    _data += "Content-Disposition: form-data; name=\"" + _name + "\"\r\n";
    _data += "\r\n";
    _data += _val;
    _data += "\r\n";
  }
  return (_data);
}

// Server Send Http Post Values
// 측정된 값을 Server로 전송 하는 함수
bool Server_Send_Value(String _Sv){
  HTTPClient http;
  int Re_Cnt = 5;
  String Server_Str = "http://" + _EDI.dsu;
  Server_Str = Server_Str + ":";
  Server_Str = Server_Str + String(_EDI.dsp);
  Server_Str = Server_Str + _EDI.dsu1;  
  Server_Str = Server_Str + "?";
  Server_Str = Server_Str + _Sv;  
// GET
  if(_Check_Wifi()){
    //_Put_SysMsg(Server_Str);
    while(Re_Cnt > 0){
      if(http.begin(Server_Str)){
        delay(100);
        _retV = http.GET();
        //Serial.printf("[HTTP] GET... code: %d\n", _retV);
        if(_retV == HTTP_CODE_OK){
          Server_Str = http.getString();
          http.end();
          return true;
        }else{
          _Set_Log("ERR:HTTP_"+http.errorToString(_retV), true);
          http.end();
        }
      }
      delay(500);
      Re_Cnt--;
    }
    _Set_Log("ERR:HTTP_"+http.errorToString(_retV), true);      
  }else{
    _Set_Log("ERR:Wifi_V", true);
  }
  return false;
}

bool Server_Send_Audio(){
  WiFiClient client;
  uint32_t wav_len = WAV_HEADER_SIZE + I2S_REC_SIZE;
  uint32_t full_length;
  String Post_S = "";
  String End_Body = "";
  int check_Con = 10;
  int _Send = 0;
  int _Retry = 100;
// Grnerate header to the WAV file`
  byte wav_header[WAV_HEADER_SIZE];
  _ret = generate_wav_header(wav_header, I2S_REC_SIZE);  
  // 전송할 값에 따른 Body 생성.
  Post_S = body_str("i", _EDI.did);
  Post_S = Post_S + body_str("d", _rectime);
  Post_S = Post_S + body_str("wav", (_EDI.did + _rectime + ".WAV"));

  End_Body = String("\r\n--") + _Boundary + String("--\r\n");
  full_length = Post_S.length() + wav_len  + End_Body.length();

  //Check WiFi 또는 rth 상태 connection status
  if(_Check_Wifi()){
    while(_Retry > 0){
      check_Con = 10;
      while(!client.connect(_EDI.dsu.c_str(), _EDI.dsp, 600000)){  // timeout을 좀 길계 5초로 줌.
        if(check_Con < 0){
          _Set_Log("ERR:Ser_C", true);
          return false;
        }else{
          check_Con--;
          delay(100);
        }
      }
      client.print(header_str(full_length));
      client.print(Post_S);
      client.write((uint8_t*)wav_header, WAV_HEADER_SIZE);
      _Send = client.write((uint8_t*)i2s_read_buff, I2S_REC_SIZE);
      _Put_SysMsg("Send Size = " + String(_Send));
      if(_Send == I2S_REC_SIZE){
        client.print(End_Body);
        break;
      }else{
        client.stop();
        _Retry--;
      }
    }
    if(_Retry == 0){
      // 서버 응답 확인
      _Put_SysMsg("Server Send Error");
      return false;
    }
    _ret = readResponse(&client);
    client.stop();
  }else {
    _Set_Log("ERR:Net", true);
    return false;
  }
  return true;
}


// Client Response Check;

bool readResponse(NetworkClient *client) {
  unsigned long timeout = millis();
  while (client->available() == 0) {
    if (millis() - timeout > WAIT_TIME_OUT) {
      _Set_Log("ERR:Client Timeout !", true);
      client->stop();
      return false;
    }
  }

  // Read all the lines of the reply from server and print them to Serial
  while (client->available()) {
    String line = client->readStringUntil('\r');
    //Serial.print(line);
  }

  return true;
}