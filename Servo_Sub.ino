/*
** Servo Mortor 초기 설정.
*/ 

bool _Init_Servo(){
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  SG90.setPeriodHertz(50); // PWM frequency for SG90
  //sg90.attach(Servo_Pin, 500, 2400); // Minimum and maximum pulse width (in µs) to go from 0° to 180
  SG90.attach(SG90_Pin, SG90_MIN_MICROS, SG90_MAX_MICROS); // Minimum and maximum pulse width (in µs) to go from 0° to 180  
  delay(500);
  if(SG90.attached()){
    SG90.write(90);  // Servo 초기화. 위치로.
    return true;
  }
  return false;
  
}
// Servo Mortor 구동 함수.
bool Servo_Move(bool LR){
  int sposition = 90;

  //Serial.println("Servo:"+String(LR));
  for(int i = 0; i < 90; i++){
    if(LR){
      sposition = 90 - i;
    }else{
      sposition = i;
    }
    SG90.write(sposition);
    delay(30);
  }
  return true;
}