// SSD  연결
bool _Set_SSD(){
  if (FORMAT_FFAT) {
    FFat.format();
  }

  if (!FFat.begin()) {
    _Put_SysMsg("FFat Mount Failed");
    return false;
  }

  return true;
}
/*
** Reboot 정보 파일 읽어 오기
*/
bool Get_Reboot_info() {
  // Reboot 정보 설정
  // 값이 없으면 false;
  Preferences preferences;
  preferences.begin( "RebootInfo", false );
  _Reboot = preferences.getBool( "Reboot", false );
  preferences.end();
  return _Reboot;
}

/*
** Reboot 정보 저장
*/
bool Set_Reboot_info(bool _Setval) {
  // Reboot 정보 설정
  Preferences preferences;
  noInterrupts();
  preferences.begin( "RebootInfo", false );
  preferences.putBool( "Reboot", _Setval );
  preferences.end();
  interrupts();
  return true;
}

/*
** Device  기초 정보 읽어 오기. **
** 정보가 없는 경우 : 초기값으로 설정됨.
*/
bool  Get_Device_info() {
  bool __Ret = true;

  Preferences preferences;
  preferences.begin( "FSDInfo", false );
  
  __Ret = preferences.isKey("SSID");
  //__Ret = false;
  if(__Ret){
    _EDI.ssid = preferences.getString("SSID");
    _EDI.pass = preferences.getString("PASS");
    _EDI.did = preferences.getString("DID");
    _EDI.dsu = preferences.getString("DSU");
    _EDI.dsu1 = preferences.getString("DSU1");
    _EDI.dsu2 = preferences.getString("DSU2");
    _EDI.dsp = preferences.getInt("DSP");
    _EDI.ctm = preferences.getInt("CTM");
    _EDI.dsv = preferences.getInt("DSV", 2);    
  }else{
    _EDI.ssid = "ATOZ_LAB_FS";
    _EDI.pass = "atoz9897!";
    _EDI.did = "FSD20241113002";
    _EDI.dsu = "106.245.78.214";
    _EDI.dsu1 = "/farmdata/api/datainput.do";    
    _EDI.dsu2 = "/farmdata/api/fileUpload.do";
    _EDI.dsp = 8080;    
    _EDI.ctm = 20;
    _EDI.dsv = 2; 
  }
  preferences.end();

  return __Ret;
}

bool Set_Device_info() {
  // Device 정보 설정
  Preferences preferences;

  noInterrupts();
  preferences.begin( "FSDInfo", false );

  preferences.putString( "SSID", _EDI.ssid.c_str());
  preferences.putString( "PASS", _EDI.pass.c_str());
  preferences.putString( "DID", _EDI.did.c_str());
  preferences.putString( "DSU", _EDI.dsu.c_str());
  preferences.putString( "DSU1", _EDI.dsu1.c_str());
  preferences.putString( "DSU2", _EDI.dsu2.c_str());
  preferences.putInt( "DSP", _EDI.dsp);
  preferences.putInt( "CTM", _EDI.ctm);
  preferences.putInt( "DSV", _EDI.dsv);  

  preferences.end();
  interrupts();
  _Put_SysMsg("Config Save!");
  return true;
}

bool ReSet_Device_info() {
  // Device 정보 설정
  Preferences preferences;
  noInterrupts();
  preferences.begin( "FSDInfo", false );

  preferences.putString( "SSID", "ATOZ_LAB_FS");
  preferences.putString( "PASS", "atoz9897!");
  preferences.putString( "DID", _EDI.did.c_str());
  preferences.putString( "DSU", "106.245.78.214");
  preferences.putString( "DSU1", "/farmdata/api/datainput.do");
  preferences.putString( "DSU2", "/farmdata/api/fileUpload.do");
  preferences.putInt( "DSP", 8080);
  preferences.putInt( "CTM", 20);
  preferences.putInt( "DSV", 2);


  preferences.end();
  interrupts();
  _Put_SysMsg("Reset config!");
  return true;
}

String Print_Device_Info() {
  // Device 정보 출력
  // Log 처리.
  String __Ret = "SSID:";
  __Ret += _EDI.ssid;
  __Ret += " PASS:";
  __Ret += _EDI.pass;
  __Ret += " Device ID:";
  __Ret += _EDI.did;
  __Ret += " DS_URL:";
  __Ret += _EDI.dsu;
  __Ret += " D Up_Path:";
  __Ret += _EDI.dsu1;
  __Ret += " A Up_Path:";
  __Ret += _EDI.dsu2;
  __Ret += " S_Port";
  __Ret += String(_EDI.dsp);
  __Ret += " CTime M:";
  __Ret += String(_EDI.ctm);
  __Ret += " DSV:";
  __Ret += String(_EDI.dsv);  
  return __Ret;
}
