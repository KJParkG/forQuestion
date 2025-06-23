/*
** Sound Recoding Buffer Allocation
*/
bool _I2S_Buffer_Init(){
  i2s_read_buff = (char*) ps_calloc(I2S_REC_SIZE, sizeof(char));
  if(i2s_read_buff == NULL){
    _Set_Log("ERR:R_Buffer", true);
    return false;
  }
  return true;  
}
/*
** Device Restart **
*/
void _Device_Restart(){
  noInterrupts();
	_Put_SysMsg("Device Restart");
  _Set_St_Led(1);   
  for(int i=0; i< 10;i++){
    _Tic_Led(1);
    delay(500);
  }
  //ESP.restart();  
  abort();
}

// Interrupt
/*
* 외부 Set Button을 누르면 실행되는 인터럽트 루틴.
*/
void IRAM_ATTR _Run_ApMode(){
  // Ap Mode를 활성 / 비활성화 시킨다.
  //noInterrupts();
  detachInterrupt(WS_On_Pin);
  Serial.println("interrupt");
  if(_RunWeb){
    _ret = Set_Reboot_info(false);
  }else{
    _ret = Set_Reboot_info(true);
  }
  _Device_Restart();
}

// System Log
void _Set_Log(String _Log, bool _prn){
  if(_prn){
    if(digitalRead(SerialPrint_OnOff_Pin) == HIGH){
      Serial.println(_Log);
    }
  }
  _Sys_Log = _Sys_Log + "[";
  _Sys_Log = _Sys_Log + _Log;
  _Sys_Log = _Sys_Log + "]";  
}

// System Status Msg
void _Put_SysMsg(String _Msg){
  _Msg = ">>" + _Msg;
  if(digitalRead(SerialPrint_OnOff_Pin) == HIGH){
    Serial.println(_Msg);
  }
}

/*
** 한국 ntp server로 부터 기준 시간을 불러 온다.  **
*/
bool _set_ntp(){
  configTime(32400, 0, "kr.pool.ntp.org");
  delay(500);
  for(int i = 0; i < 10; i++){
    if(_Get_LocalTime() != "00000000000000" ){
      return true;
    }
    _Tic_Led(4);      
    delay(1000);    
  }
  return false;  
}


/*
** 현재 년월일시분초를 불러와 YYYYMMDDHHmmss 형식의 문자열로 변환하여 주는 함수. **
*/
String _Get_LocalTime() {
  struct tm timeinfo;
  String Ret = "";
  if (!getLocalTime(&timeinfo)) {
    // YYYYMMDDHHmmss
    Ret = "00000000000000";
  } else {
    Ret = String(timeinfo.tm_year + 1900);
    if((timeinfo.tm_mon + 1) < 10){
      Ret = Ret +"0"+ String(timeinfo.tm_mon + 1);
    }else{
      Ret = Ret + String(timeinfo.tm_mon + 1);
    }
    if(timeinfo.tm_mday < 10){
      Ret = Ret +"0"+ String(timeinfo.tm_mday);
    }else{
      Ret = Ret + String(timeinfo.tm_mday);
    }    
    if(timeinfo.tm_hour < 10){
      Ret = Ret +"0"+ String(timeinfo.tm_hour);
    }else{
      Ret = Ret + String(timeinfo.tm_hour);
    }
    if(timeinfo.tm_min < 10){
      Ret = Ret +"0"+ String(timeinfo.tm_min);
    }else{
      Ret = Ret + String(timeinfo.tm_min);
    }     
    if(timeinfo.tm_sec < 10){
      Ret = Ret +"0"+ String(timeinfo.tm_sec);
    }else{
      Ret = Ret + String(timeinfo.tm_sec);
    }         
  }
  return Ret;
}