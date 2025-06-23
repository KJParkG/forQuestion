// 온.습도 센서
//dht.setup(TmPin, DHTesp::DHT22);
bool _DHT_Init(){
  int Err_Cnt = 10;
  bool s_Ret = true;

  dht.begin();
  dht.temperature().getSensor(&DHT_Sensor_info);
  DHT_delayMS = DHT_Sensor_info.min_delay / 1000;

  // 정상 작동인지 검사.
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
  }
  return s_Ret;
}

/*****************************  MQ CAlibration ********************************************/ 
// MQ135.serialDebug(true);
//MQ135.setA(110.47); MQ135.setB(-2.862); // Configure the equation to to calculate CO2 concentration
/*
  Exponential regression:
GAS      | a      | b
CO       | 605.18 | -3.937  
Alcohol  | 77.255 | -3.18 
CO2      | 110.47 | -2.862
Toluen  | 44.947 | -3.445
NH4      | 102.2  | -2.473
Aceton  | 34.668 | -3.369
*/
bool _Set_Co2(){
  float calcR0 = 0;
  int Err_Cnt = 10;
  bool s_Ret = true;
  Co2_OnOff(true); // 센서에 전원을 먼저 이입.
  
  while(Err_Cnt > 0){
    MQ135.setRegressionMethod(1); //_PPM =  a*ratio^b
    MQ135.setA(110.47); MQ135.setB(-2.862); // Configure the equation to to calculate CO2 concentration    
    MQ135.init();

    //_Put_SysMsg("Co2 Sensor Calibrating.");
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
    //  _Put_SysMsg("Warning: Conection issue, R0 is infinite (Open circuit detected) please check your wiring and supply");
      s_Ret=false;
    }
    if(calcR0 == 0){
  //    _Put_SysMsg("Warning: Conection issue found, R0 is zero (Analog pin shorts to ground) please check your wiring and supply");
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

/*
** MQ Sensor 전원 On & Off
** true = ON , false = Off
*/
bool Co2_OnOff(bool _OnOff){
  if(_OnOff){
    digitalWrite(Co2_Control, HIGH);
    delay(3000);
  }else{
    digitalWrite(Co2_Control, LOW);
  }
  return _OnOff;
}

bool get_DHT() {
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
  if(s_Ret){
    // 정상임.
      HttpSend_Values = HttpSend_Values + "&t=";
      HttpSend_Values = HttpSend_Values + DHT_T_event.temperature;
      HttpSend_Values = HttpSend_Values + "&h=";
      HttpSend_Values = HttpSend_Values + DHT_H_event.relative_humidity;    
  }else{
      _Put_SysMsg("ERR:DHT");    
      HttpSend_Values = HttpSend_Values + "&t=";
      HttpSend_Values = HttpSend_Values + "0";
      HttpSend_Values = HttpSend_Values + "&h=";
      HttpSend_Values = HttpSend_Values + "0";
  }
  return s_Ret;
}

bool get_Co2(){
  MQ135.setA(110.47); MQ135.setB(-2.862); // Configure the equation to to calculate CO2 concentration
  MQ135.update();
  float ppmCo2 =  MQ135.readSensor() + 400;  // 기본 Base가 400 임에 따라 Plus 한다.
  HttpSend_Values = HttpSend_Values + "&c=";
  HttpSend_Values = HttpSend_Values + String(ppmCo2);
  return true;
}

// Sound Detect
// 일정소리 이상이 감지되면 녹음 시작을 위하여 소리 감지.
bool Detect_Sound(){
  bool s_Ret = false;
  int dac_value = 0;
  Reset_time_value = esp_timer_get_time() + WAIT_SOUND_DETECT; // 10분
  while(Reset_time_value > esp_timer_get_time()){
    _Set_St_Led(3);
    dac_value = abs(FSD_I2S.read());
    dac_value = (int)(dac_value / 15000000);
    if(dac_value > _EDI.dsv){
      s_Ret = true;
      _Put_SysMsg("Sound Detected");
      break;
    }
    delay(100);
    _Set_St_Led(2);    
    delay(100);
  }
  return s_Ret;  
}