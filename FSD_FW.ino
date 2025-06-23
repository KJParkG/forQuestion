/*****************************
Program :가축농장 환경정보 수집장치 ( Livestock Farms Environmental Information Collection Device )  Device Name : FarmSonic Drive ( FSD )
Version : 3.13 < ESP32 Type - LOLIN S3 PRO>
Co - Create : (C)AtoZSystem  - topman@atozsystem.co.kr
CrDate / Last Update: 2024.07.20 / 2024.11.04
======
※ Compile Option
1. USB CDC On Boot = Enabled

※ 구성 Unit
0. Main MCU ( ESP32 S3 N16R8 )
1. SSD ( FATFS )
  : 16M Flash ( 3MB APP / 9.9MB FATFS )
2. 온습도 센서 ( DHT22 / AM2302 )
  : SetPin A6 / 7
3. Co2 센서 ( MQ135 )
  : SetPin A1  / 2 / 15
4. Co2 전원 제어 ( TR NPN 2N2222 )
  : SetPin A5 / 6 / Digital Mode
5. 마이크 센서 ( PCM1802 / KB100 )
  : I2S 통신 방식 // PCM1802를 통한 통신.
  : SetPin (17, 18, -1, 16, 8); //SCK, WS, SDOUT, SDIN, MCLK
  : 해당 마이크는 기본 24bit 임 / 32bit로 읽어서 32bit를 통으로 사용.
6. 상태표시 Led ( WS2812B )
  : SetPin 5 / 2
7. Servo Mortor ( SG90 ) ESP32S3servo Lib 사용
  : SetPin 39
*/
#include <Arduino.h>
#include "FS.h"
#include "SPI.h"
#include "esp_err.h"
#include <WiFi.h>
#include <esp_wifi.h>
#include "ESP_I2S.h"
#include "FFat.h"
#include <esp_now.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <MQUnifiedsensor.h>
#include <ArduinoJson.h>
#include <WiFiClient.h>
#include <HTTPClient.h>
#include <WebServer.h>
#include <ESP32Servo.h>
#include <Preferences.h>
#include <Adafruit_NeoPixel.h>

// ESP32에서만 실행됨에 따라 실행전 확인.
// 향후 : 본 프로그램에서는 삭제
#ifndef ESP32
#pragma message(This Program is for ESP32 Only !)
#error Select ESP32 board.
#endif

// 실행 환경 정의 Config
#include "Config.h"
#include "Html_Page.h"
#include "PublicVal.h"
#include "Sub_Def.h"

void setup() {
  pinMode(WS_On_Pin, INPUT_PULLUP); // Interrupt Pin
  pinMode(SerialPrint_OnOff_Pin, INPUT_PULLUP); // Serial Print Pin  
  pinMode(RLED, OUTPUT);
  pinMode(YLED, OUTPUT);
  pinMode(BLED, OUTPUT);
  pinMode(Co2_Control, OUTPUT);  

  Device_Status_Led.begin();
  Device_Status_Led.show();
  Device_Status_Led.setBrightness(50);

  Serial.begin(115200);
  _Put_SysMsg("system Start"); 

  // Start Led
  _Led_Start();

  // SSD 연결.
  if (!_Set_SSD()) {
    // SSD가 오류라면 Ap 모드를 실행한다.
    _Set_Log("ERR:SSD", true);
    _Run_Ap_Mode = true;
  }else{
    _Set_Log("OK:SSD", true);
      // Device 기본 정보 Read
    if(Get_Device_info()){
      _Set_Log("OK:Config", true);
    }else{
      // Device 기초 정보가 없음으로 생성한다.
      Set_Device_info();
      _Set_Log("Reset:Config", true);
    }
    // Device기본 정보 Log Print
    _Set_Log(Print_Device_Info(), false);
    // Reboot 모드인가 ?
    // SSD가 연결된 경우만 가능함.
    if(Get_Reboot_info()){
      // Reboot 모드가 true 이면 AP 모드를 무조건 실행함. -> 외부에서 Ap 모드 Switch를 이용한 거임.
      // 더불어 다음 Reboot을 위하여 flag 초기화.
      _ret = Set_Reboot_info(false);
      _Run_Ap_Mode = true;
      _Set_Log("CALL:AP Switch", true);
    }    
  }

  // Network Wi-FI Mode만지원함.
  if (!_Set_Wifi()) {
    _Set_Log("ERR:WiFi", true);
    _Run_Ap_Mode = true;
  }else{
    _Set_Log("OK:WiFi", true);

      // NTP Date&Time Get.
      // Network Not Connect 인경우 NTP 가져 오지 않음.
    if (_set_ntp()) {
      _Set_Log("OK:NPT", true);
    }else{
      _Set_Log("ERR:NPT", true);
      _Run_Ap_Mode = true;
    }
  }
  // 온습도 센서 init
  if(_DHT_Init()){
    _Set_Log("OK:DHT", true);
  }else{
    _Set_Log("ERR:DHT", true);
    _Run_Ap_Mode = true;
  }

  // Co2 센서.
  if (_Set_Co2()) {
    _Set_Log("OK:Co2", true);
  }else{
    _Set_Log("ERR:Co2", true);
    _Run_Ap_Mode = true;
  }
  Co2_OnOff(false);

  // Mic Unit Mount.
  if (_set_Mic()) {
    _Set_Log("OK:Mic", true);
  }else{
    _Set_Log("ERR:Mic", true);    
    _Run_Ap_Mode = true;
  }

  //Servo
  if (_Init_Servo()) {
    _Set_Log("OK:Servo", true);
  }else{
    _Set_Log("ERR:Servo", true);    
    _Run_Ap_Mode = true;
  }  

  //Sound Buffer Error 인 경우만 처리.
  if (!_I2S_Buffer_Init()) {
    _Run_Ap_Mode = true;
  }  
  // 스케줄 시작을 위하여 시스템 시작 시간을 기록.
  collectiontime = esp_timer_get_time();
  _Set_Log("RUN:System", true);
  _Put_SysMsg("System Init Information");
  _Put_SysMsg(_Sys_Log);  
  _Put_SysMsg("System Begin");

  // AP Mode Run Switch Start.
  //attachInterrupt(digitalPinToInterrupt(WS_On_Pin), _Run_ApMode, FALLING);
  attachInterrupt(WS_On_Pin, _Run_ApMode, FALLING);
  _Set_St_Led(2); 
}

//  Loop
void loop() {
  // device Setup Mode
  if (_Run_Ap_Mode) {
    // Web Server Mode
    if (!_RunWeb) {
      _Put_SysMsg("AP RUN");
      // 웹서버가 실행중이 아니라면 실행한다.

      // 자가진단후 웹서버 실행.
      if(Self_Unit_Check()){
        _Set_Log("OK:Self", true);
      }else{
        _Set_Log("ERR:Self", true);
      }

      _Set_ApMode();
      _Set_St_Led(4);
      Reset_time_value = esp_timer_get_time() + _ReBoot_Time; // 10분
      _RunWeb = _Init_WebServer();
    }
    if (Reset_time_value < esp_timer_get_time()) {
      _Run_ApMode();
    }    
    // loop 때마다 server로 오는 요청에 응답하기 위하여 시행함.
    w_server.handleClient();
    return;
  }else if (_RunWeb) {
    _End_WebServer();
    _Device_Restart();
    return;
  }
  // Log Clear
  _Sys_Log = "";

  if (collectiontime < esp_timer_get_time()) {
  // Co2 센서.
    if (_Set_Co2()) {
      _Set_Log("OK:Co2", true);
    }else{
      _Set_Log("ERR:Co2", true);
      Co2_OnOff(false); 
      _Run_Ap_Mode = true;
      return;
    }
    _ret = Servo_Move(true);
    delay(60000); // 센서 박스를 내리고 나서 60초 대기후 측정. 
    HttpSend_Values = "i=" + _EDI.did;
    HttpSend_Values = HttpSend_Values + "&d=";
    HttpSend_Values = HttpSend_Values + _Get_LocalTime();
    _ret = get_DHT();
    _ret = get_Co2();
    delay(100); // 측정 종료후 2초 대기.
    Co2_OnOff(false);    
    _ret = Servo_Move(false);
    delay(500);

    if(!Server_Send_Value(HttpSend_Values)){
      //Error 발생시 AP 모드로.
      _ReBoot_Time = ( 5 * 60 * 1000000 );
      _Run_Ap_Mode = true;
      _Set_Log("ERR:Wifi_S", true);
      _Led_Err();
      return;
    }

    collectiontime = esp_timer_get_time() + (_EDI.ctm * 60 * 1000000);
  }
  if(Detect_Sound()){ // Sound가 감지되지 않으면 녹음하지 않음.
    // Recoding 오류가 발생하면 전송도 하지 않는다.
    if(Sound_Rec()){
      if(!Server_Send_Audio()){
        //Error 발생시 AP 모드로.
        _ReBoot_Time = ( 5 * 60 * 1000000 );
        _Run_Ap_Mode = true;
        _Set_Log("ERR:Wifi_A", true);
        _Led_Err();
      }else{
        delay(1000);      
      }
    }else{
      //Error 발생시 AP 모드로.
      _ReBoot_Time = ( 5 * 60 * 1000000 );
      _Run_Ap_Mode = true;
      _Set_Log("ERR:Rec", true);
      _Led_Err();
    }
  }
}
