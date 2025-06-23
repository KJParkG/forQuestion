// System 변수 생성. 및 초기 선언.
// 시스템 계속 실행 유무 Loop에 적용됨.
bool _RunWeb = false;         // 웹 실행 중인가 ?
bool _EndMsg = false;         // End Message Flag
bool _ret = true;             // Sub Routine Return을 기다려 순차 실행을 위한 결과값.
int _retV = 0;                // Int Type Return Value
volatile bool _Run_Ap_Mode = false;  // Network이 접속 불가이면 자동 AP 모드 실행을 위한 Flag
uint64_t _ReBoot_Time = ( 10 * 60 * 1000000 );   // 분 x 초 x 1/1000000초 600초= 10분.
// 타이머.
uint64_t collectiontime = 0;  // 온도,습도,Co2 검침 시간
uint64_t Reset_time_value = 0;  // AP 모드 자동 재시작 타이며, 10분. / Sound 감지 시간 타이머.

bool _Reboot = false; // Reboot Status Flag  : true => ENC Init Cancel.
// 온습도 센서
DHT_Unified dht(DHTPIN, DHTTYPE);
uint32_t DHT_delayMS;            // 최소 측정간 시간
sensors_event_t DHT_T_event;   // Get 온도 event and print its value.
sensors_event_t DHT_H_event;   // Get 습도 event and print its value.
sensor_t  DHT_Sensor_info;   // 센서정보.

// Server로 전송된 값
String HttpSend_Values = "";

//Co2 센서
//Declare Co2 Sensor
MQUnifiedsensor MQ135(MCUBoard, Voltage_Resolution, ADC_Bit_Resolution, Co2Pin, SensorType);

// Device Information Struct
struct EicdInfo _EDI;

// WebServer Object 선언.
WebServer w_server(80);

// Servo Object 선언.
Servo SG90;

//Audio Recoding Time - Server 전송용.
String _rectime = "YYYYMMDDHHmmSS";
String _Boundary = String("**") + String("T1o2p3M4a5n") + String("**");

// Audio용 I2S 선언
I2SClass FSD_I2S;
char* i2s_read_buff;
uint8_t* Post_Send_Buff;

// System Log
String _Sys_Log = "System Run Log<BR>";

// Status LED Object Define
// Adafruit_NeoPixel Device_Status_Led = Adafruit_NeoPixel(St_Led_Cnt, St_Led, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel Device_Status_Led(St_Led_Cnt, St_Led, NEO_GRBW + NEO_KHZ800);

// Self Check
String _Self_Check_Result = "";