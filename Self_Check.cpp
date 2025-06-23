/**
** Self Unit Check
**/

/**
** 온습도 Self Check
**/
bool check_DHT(){
  int Err_Cnt = 10;
  bool s_Ret = true;
  String th = "";

  // Error가 발생하면 10회 정도 까지 제검침하여 처리.
  while(Err_Cnt > 0){
    delay(DHT_delayMS);
    dht.temperature().getEvent(&DHT_T_event);
    dht.humidity().getEvent(&DHT_H_event);

    if (isnan(DHT_T_event.temperature) || isnan(DHT_H_event.relative_humidity)) {
      Err_Cnt--;
      s_Ret=false;
    }else{
      s_Ret=true;
      break;
    }
    delay(100);
  }
  return s_Ret;
}

/**
** Co2 Self Check
**/
bool check_CO2(){
  float calcR0 = 0;
  int Err_Cnt = 10;
  bool s_Ret = true;
  
  while(Err_Cnt > 0){
    MQ135.setRegressionMethod(1); //_PPM =  a*ratio^b
    MQ135.setA(110.47); MQ135.setB(-2.862); // Configure the equation to to calculate CO2 concentration    
    MQ135.init();

    calcR0 = 0;
    for(int i = 1; i<=10; i ++)
    {
      MQ135.update(); // Update data, the arduino will read the voltage from the analog pin
      calcR0 += MQ135.calibrate(RatioMQ135CleanAir);
    }
    MQ135.setR0(calcR0/10);
    //Serial.println("  done!.");
    s_Ret=true;
    if(isinf(calcR0)) {
      s_Ret=false;
    }
    if(calcR0 == 0){
      s_Ret=false;
    }
    if(s_Ret){
      break;
    }else{
      Err_Cnt--;
      delay(100);
    }
  }

  return s_Ret;
}

/**
** Sound Module Self Check
**/
bool check_SOUND(){
  uint64_t R_Sound = 0;
  int Err_Cnt = 10;
  bool s_Ret = false;
  
  if(_set_Mic()){
    for(int i = 0; i < 1000; i++){
      R_Sound = R_Sound + abs(FSD_I2S.read());
      delay(10);
    }
    R_Sound = R_Sound / 1000;
    if(R_Sound > 1000000){
      s_Ret = true;  
    }else{
      s_Ret = false;  
    }
  }else{
    s_Ret = false;
  }

  return s_Ret;
}

void Set_Check_Val(String _Sen, bool _Val){
  _Self_Check_Result = _Self_Check_Result + "(";
  _Self_Check_Result = _Self_Check_Result + _Sen;
  if(_Val){
    _Self_Check_Result = _Self_Check_Result + ":OK";
  } else {
    _Self_Check_Result = _Self_Check_Result + ":ERR";
  }
  _Self_Check_Result = _Self_Check_Result + ")";
}

// Server Send Http Post Values
// 측정된 값을 Server로 전송 하는 함수
bool Server_Send_Check_Value(){
  HTTPClient http;
  int Re_Cnt = 5;
  String Server_Str = "http://" + _EDI.dsu;
  Server_Str = Server_Str + ":";
  Server_Str = Server_Str + String(_EDI.dsp);
  Server_Str = Server_Str + "/farmdata/api/loginput.do?i=";
  Server_Str = Server_Str + _EDI.did;
  Server_Str = Server_Str + "&d=";
  Server_Str = Server_Str + _Get_LocalTime();
  Server_Str = Server_Str + "&l=";
  Server_Str = Server_Str + _Self_Check_Result;
// GET
  if(_Check_Wifi()){
    //_Put_SysMsg(Server_Str);
    while(Re_Cnt > 0){
      if(http.begin(Server_Str)){
        delay(100);
        _retV = http.GET();
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

bool Self_Unit_Check(){
  bool s_Ret = false;
  _Self_Check_Result = "";

  _Set_St_Led(5);

  Co2_OnOff(true); 
  //Step 1 : DHT
  _ret = Servo_Move(true);
  delay(60000); // 센서 박스를 내리고 나서 60초 대기후 측정.

  Set_Check_Val("DHT", check_DHT());
  Set_Check_Val("Co2", check_CO2());

  Co2_OnOff(false);
  _ret = Servo_Move(false);
  delay(3000); // 잡음제거를 위하여 3초 대기.

  Set_Check_Val("Sound", check_SOUND());

  s_Ret = Server_Send_Check_Value();

  _Set_St_Led(2);
  return s_Ret;
}