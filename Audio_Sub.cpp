/*
** Audio 관련 Unit 초기 설정 및 연결 **
*/

bool _set_Mic(){
  // Audio용 I2S Setup ADC
  // ESP32-S3 to PCM1802

  FSD_I2S.setPins(PIN_I2S_SCK, PIN_I2S_WS, -1, PIN_I2S_SDIN, PIN_I2S_MCLK); //SCK(BCK), WS, SDOUT, SDIN, MCLK(SCK)
  if (!FSD_I2S.begin(I2S_MODE_STD, I2S_SAMPLE_RATE, I2S_DATA_BIT_WIDTH_32BIT, I2S_SLOT_MODE_MONO, I2S_STD_SLOT_LEFT)) {
    //_Put_SysMsg("Failed to initialize I2S bus!");
    return false;
  }
  return true;
}

/*
** Audio Recoding Sub **
*/
bool Sound_Rec(){
  int rec_size = I2S_REC_SIZE;
  int Rp = 0;
  int Wp = 0;
  //int8_t pm_val = 0;
  //int32_t S_Val = 0;
  size_t read_bytes = 0;
  //bool pm = true; // true = + , flase = -

  _Set_St_Led(3);
  //Server 전송을 위한 녹음시작시간 기록.
  _rectime = _Get_LocalTime();

  if(FSD_I2S.available() > 0){
      read_bytes = FSD_I2S.readBytes(i2s_read_buff, rec_size);
  }
  _Set_St_Led(2);
  return true;
}

bool generate_wav_header(byte* header, int wavSize){
  header[0] = 'R';
  header[1] = 'I';
  header[2] = 'F';
  header[3] = 'F';
  //(전체 파일크기 - 8 byte) 
  unsigned int fileSize = wavSize + WAV_HEADER_SIZE - 8;
  header[4] = (byte)(fileSize & 0xFF);
  header[5] = (byte)((fileSize >> 8) & 0xFF);
  header[6] = (byte)((fileSize >> 16) & 0xFF);
  header[7] = (byte)((fileSize >> 24) & 0xFF);
  //파일 형식
  header[8] = 'W';
  header[9] = 'A';
  header[10] = 'V';
  header[11] = 'E';
  // Chunk ID "fmt "
  header[12] = 'f';
  header[13] = 'm';
  header[14] = 't';
  header[15] = ' ';
  //Chunk Size (4byte) Little Endian Header에서 뒤에이어지는 값들의 사이즈 16 고정값.
  header[16] = 0x10;
  header[17] = 0x00;
  header[18] = 0x00;
  header[19] = 0x00;
  //Audio Format (2byte) Little Endian , 고정값 1.

  header[20] = 0x01;
  header[21] = 0x00;
  //Number Of Channel (2byte) Little Endian mono : 1 , stereo : 2
  header[22] = 0x01;
  header[23] = 0x00;
  //sample Rate (4byte) Little Endian
  uint32_t srate = I2S_SAMPLE_RATE;
  header[24] = (byte)(srate & 0xFF);
  header[25] = (byte)((srate >> 8) & 0xFF);
  header[26] = (byte)((srate >> 16) & 0xFF);
  header[27] = (byte)((srate >> 24) & 0xFF);

  uint32_t byteRate = I2S_SAMPLE_RATE * (I2S_SAMPLE_BITS / 8);
  //uint32_t byteRate = ADC_SAMPLE_RATE * (Mono=1, Stereo=2) * (I2S_SAMPLE_BITS / 8);  //4Byte type
  header[28] = (byte)(byteRate & 0xFF);
  header[29] = (byte)((byteRate >> 8) & 0xFF);
  header[30] = (byte)((byteRate >> 16) & 0xFF);
  header[31] = (byte)((byteRate >> 24) & 0xFF);

  uint16_t blockAlign = I2S_SAMPLE_BITS / 8;  // 3Byte
  //uint16_t blockAlign = (Mono=1, Stereo=2) *  I2S_SAMPLE_BITS / 8;  WAVE_BITS  
  //uint16_t blockAlign = 24 / 8;    // 3Byte
  header[32] = (byte)(blockAlign & 0xFF);
  header[33] = (byte)((blockAlign >> 8) & 0xFF);

  //Bit Per Sample (2byte) Little Endian
  uint16_t bpsm = I2S_SAMPLE_BITS;  
  //uint16_t bpsm = I2S_SAMPLE_BITS;  
  //uint16_t bpsm = 24;  
  header[34] = (byte)(bpsm & 0xFF);
  header[35] = (byte)((bpsm >> 8) & 0xFF);

  header[36] = 'd';
  header[37] = 'a';
  header[38] = 't';
  header[39] = 'a';
  header[40] = (byte)(wavSize & 0xFF);
  header[41] = (byte)((wavSize >> 8) & 0xFF);
  header[42] = (byte)((wavSize >> 16) & 0xFF);
  header[43] = (byte)((wavSize >> 24) & 0xFF);
  return true;
}