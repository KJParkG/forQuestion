void _Set_Led(int led_state) {
  switch (led_state) {
    case 0:  // all Off
      digitalWrite(RLED, LOW);
      digitalWrite(YLED, LOW);
      digitalWrite(BLED, LOW);
      break;
    case 1:  // 작동준비
      digitalWrite(RLED, HIGH);
      digitalWrite(YLED, HIGH);
      digitalWrite(BLED, HIGH);
      break;
    case 2:  // 정상
      digitalWrite(RLED, LOW);
      digitalWrite(YLED, LOW);
      digitalWrite(BLED, HIGH);
      break;
    case 3:  // 녹음
      digitalWrite(RLED, LOW);
      digitalWrite(YLED, HIGH);
      digitalWrite(BLED, HIGH);
      break;
    case 4:  // Setup Mode 
      digitalWrite(RLED, LOW);
      digitalWrite(YLED, HIGH);
      digitalWrite(BLED, LOW);
      break;
    case 5:  // Self Check Mode 
      digitalWrite(RLED, HIGH);
      digitalWrite(YLED, HIGH);
      digitalWrite(BLED, LOW);
      break;      
    default:  // 동작 불가 오류. // System Restart.
      digitalWrite(RLED, HIGH);
      digitalWrite(YLED, LOW);
      digitalWrite(BLED, LOW);
      break;
  }
}
// Led 깜빡임.
void _Tic_Led(int led_state) {
  _Set_Led(0);
  delay(100);
  _Set_Led(led_state);
}

// 디바이스 상태 표시 Led 설정.
void _Set_St_Led(int led_state){
  uint32_t _st_colorValue;
  _Set_Led(led_state);  
/*
  Device_Status_Led.setPixelColor(0, Device_Status_Led.Color(0, 0, 0)); // 내부 G , R, B/ 외부 R,G,B
  Device_Status_Led.show();
*/  
  switch (led_state) {
    case 0:  // Off
      _st_colorValue = Device_Status_Led.Color(0, 0, 0);
      break;
    case 1:  // White  // 작동준비
      _st_colorValue = Device_Status_Led.Color(255, 255, 255);
      break;
    case 2:  // Blue  // 정상.
      _st_colorValue = Device_Status_Led.Color(0, 255, 0);
      break;      
    case 3:  // Green  // 녹음중.
      _st_colorValue = Device_Status_Led.Color(0, 0, 255);
      break;
    case 4:  // Yellow  // AP 모드
      _st_colorValue = Device_Status_Led.Color(255, 255, 0);
      break;
    case 5:  // Yellow  // Self Check 모드
      _st_colorValue = Device_Status_Led.Color(255, 0, 255);
      break;            
    default:  // Red // 동작 불가 오류. // System Restart.
      _st_colorValue = Device_Status_Led.Color(255, 0, 0);
      break;
  }
  Device_Status_Led.clear();
  Device_Status_Led.setPixelColor(0, _st_colorValue); // 내부 G , R, B/ 외부 R,G,B
  Device_Status_Led.show(); 
  return;
}

bool _Led_Start(){
  _Set_St_Led(6);
  delay(500);
  _Set_St_Led(4);
  delay(500);
  _Set_St_Led(2);
  delay(500);
  _Set_St_Led(1);
  return true;
}

bool _Led_Err(){
  _Set_St_Led(6);   
  for(int i=0; i< 10;i++){
    _Tic_Led(5);
    delay(500);
  }
  return true;
}
