// save to SPIFFS instead of SD Card?
// #define USE_SPIFFS 1
#define CONFIG_FILE_NAME "Eicd_info"

// #define TimeOut
#define WAIT_TIME_OUT   10000  // 10ms

// sample rate for the system
#define RECORD_TIME   30  // seconds, The maximum value is 240
#define WAV_FILE_NAME "/sound.wav"

// do not change for best

#define WAV_HEADER_SIZE 44

// PCM1802의 표기법으로 표시 / CJMCU 모델은 SCK = System Clock / BCK = Bit Clock
#define PIN_I2S_SCK  17  // PCM1802 BCK
#define PIN_I2S_WS   18 // PCM1802 LRCK
#define PIN_I2S_SDIN 16  // PCM1802 DOUT
#define PIN_I2S_MCLK 8  // PCM1802 SCK

#define I2S_SAMPLE_RATE   44100 //(44100)  // COW 35,000
#define I2S_SAMPLE_BITS   (32)
#define WAV_HEADER_SIZE   (44)
// 24bit 일경우 16,777,216 인데. 
// PCM1802의 최대 값은 420,426임으로 최대 Gain은 수의 범위상 39배가 한계임.
#define VOLUME_GAIN       (2)
#define I2S_REC_SIZE      ((I2S_SAMPLE_RATE * ( I2S_SAMPLE_BITS / 8 )) * RECORD_TIME)  // Max 8M 이지만 7M 넘지 않도록 조정 필요.
// 소리 감지를 위하여 기다리는 시간.
#define WAIT_SOUND_DETECT (10 * 60 * 1000000) // 분, 초 = 10분

// LED Pin
#define RLED GPIO_NUM_40
#define YLED GPIO_NUM_41
#define BLED GPIO_NUM_42

// Servo Mortor Pin
#define SG90_Pin                    GPIO_NUM_39
#define TIMER_INTERRUPT_DEBUG       0
#define ISR_SERVO_DEBUG             1
// For ESP32_C3, select ESP32 timer number (0-1)
// For ESP32 and ESP32_S2, select ESP32 timer number (0-3)
#if defined( ARDUINO_ESP32C3_DEV )
	#define USE_ESP32_TIMER_NO          1
#else
	#define USE_ESP32_TIMER_NO          3
#endif
// Published values for SG90 servos; adjust if needed
#define SG90_MIN_MICROS      544  //544
#define SG90_MAX_MICROS      2450

// SSD를 위한 정의 처음 한번만 실행하면 된 이후는 false로 변경.
//#define FORMAT_SPIFFS_IF_FAILED true
#define FORMAT_FFAT true
// 온습도센서 연결 Pin 정의
#define DHTPIN     GPIO_NUM_7 //GPIO_NUM_1   // 온습도 센서 핀
#define DHTTYPE    DHT22     // DHT 22 (AM2302)

// Co2센서 연결 Pin 정의
#define MCUBoard                ("ESP-32") // Wemos ESP-32 or other board, whatever have ESP32 core.
#define Co2Pin                  GPIO_NUM_15 // GPIO_NUM_2// Co2 Sensor Pin
#define Co2_Control             GPIO_NUM_6 // Co2 Sensor Power Control Pin
#define SensorType              ("MQ-135") //MQ132 or other MQ Sensor, if change this verify your a and b values.
#define Voltage_Resolution      (3.3) // 3V3 <- IMPORTANT.
#define ADC_Bit_Resolution      (13) // ESP32-S3 bit resolution. 
#define RatioMQ135CleanAir      3.6  //RS / R0 = 3.6 ppm 

// Web Server On Switch 연결 Pin 정의.
#define WS_On_Pin GPIO_NUM_1// GPIO_NUM_0  // Web Server On 스위치.

// Status View Led ( 하단 ) Pin 정의.
#define St_Led GPIO_NUM_2 //GPIO_NUM_5  // Device Status Led.
#define St_Led_Cnt 1  // Device Status Led Count.

// Serial Print On / Off Pin 정의 // Serial.Print 기능을 수행할 지 말지를 결정.
#define SerialPrint_OnOff_Pin GPIO_NUM_5  // Serial Print On&Off Pin


// 환경정보수집 장치 정보.
struct EicdInfo {
  String ssid;   // wifi-ssid.
  String pass;  // wifi-password
  String did;  // Device ID : 장치 고유 아이디  
  String dsu; // Data 전송을 위한 Datacenter Server URL - 환경정보
  String dsu1; // Data 전송을 위한 Datacenter Server URL  - Data Upload Path
  String dsu2; // Data 전송을 위한 Datacenter Server URL  - wave file Upload Path 
  int dsp; // Data 전송을 위한 Datacenter Server Http Port
  int ctm;  // 온도/습도/Co2 측정 주기 : 분단위
  int dsv;  // 소리 감지 최소 단위
};

// Search for parameter in HTTP POST request
const char* W_arg_1 = "ssid";
const char* W_arg_2 = "pass";
const char* W_arg_3 = "did";
const char* W_arg_4 = "dsu";
const char* W_arg_5 = "dsu1";
const char* W_arg_6 = "dsu2";
const char* W_arg_7 = "dsp";
const char* W_arg_8 = "ctm";
const char* W_arg_9 = "dsv";
