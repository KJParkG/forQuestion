#include "esp32-hal-gpio.h"
// Led SubRoutine
void _Set_Led(int led_state);
void _Tic_Led(int led_state);
void _Set_St_Led(int led_state);
bool _Led_Err();
bool _Led_Start();

// SSD SubRoutine
bool _Set_SSD();
bool Get_Device_info();
bool Get_Reboot_info();
bool Set_Device_info();
bool ReSet_Device_info();
bool Set_Reboot_info(bool _Setval);
String Print_Device_Info();

// Web Server SubRoutine
bool _Set_Wifi();
bool _Check_Wifi();
void _Set_ApMode();
bool _Init_WebServer();
void _End_WebServer();
void Handle_Root();
void Handle_Reset();
void Handle_Setup();
void Handle_Reset_r();
void HandleNotFound();
String header_str(int _length);
String body_str(String _name , String _val);
bool readResponse(WiFiClient *_wc);
bool Server_Send_Value(String _Sv);
bool Server_Send_Audio();

// Audio SubRoutine
bool _set_Mic();
bool Sound_Rec();
bool generate_wav_header(byte* header, int wavSize);

// Sensor SubRoutine
bool _DHT_Init();
bool _Set_Co2();
bool get_DHT();
bool get_Co2();
bool Co2_OnOff(bool _OnOff);
bool Detect_Sound();

// Servo Mortor SubRoutine;
bool _Init_Servo();
bool Servo_Move(bool LR);

// Self Check Subroutine
bool check_DHT();
bool check_CO2();
bool check_SOUND();
void Set_Check_Val(String _Sen, bool _Val);
bool Self_Unit_Check();

// Lib Sub
bool _I2S_Buffer_Init();
void _Device_Restart();
void IRAM_ATTR _Run_ApMode();
void _Set_Log(String _Log, bool _prn = false);  // 함수의 인수 초기값은 함수 선언시에만 해야함. =false
void _Put_SysMsg(String _Msg);
bool _set_ntp();
String _Get_LocalTime();
