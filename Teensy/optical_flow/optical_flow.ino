uint8_t crc8_dvb_s2(uint8_t crc, uint8_t a){
    crc ^= a;
    for (int ii = 0; ii < 8; ++ii) {
        if (crc & 0x80) {
            crc = (crc << 1) ^ 0xD5;
        } else {
            crc = crc << 1;
        }
    }
    return crc;
}

uint8_t crc8_func(int len, uint8_t* msg){
    /*for(int i= 0; i < len; i++) {
      Serial.print(int(msg[i]), HEX); Serial.print(" ");
    }
    Serial.println();*/
    uint8_t crc8t = 0;
    for(int i = 0; i < len; i++){
        crc8t = crc8_dvb_s2(crc8t, msg[i]);
    }
    return crc8t;
}
void setup() {
  // put your setup code here, to run once:
  Serial1.begin(115000);
  Serial.begin(9600);

}

void loop() {
  uint8_t send_buff[] = {'$', 'M', '<', 0, 7};
  for(int i = 0 ; i < 5 ; i++) Serial1.write(send_buff[i]);
  char comando[50];
  int idx = 0;
  uint8_t q1, q2;
  int32_t fx = 0, fy = 0;
  int32_t rng;
  long t = millis();
  while(1){
      if(Serial1.available()){
          comando[idx++] = Serial1.read();
      }  
      if(idx > 2 && comando[idx-1] == 'X' && comando [idx-2] == '$'){
          if(comando[4] == 2){
            /*Serial.print(comando[0]);
            Serial.print(comando[1]);
            Serial.print(comando[2]);*/
            
            q1 = comando[8];
            fx = 0, fy = 0;
           for(int i = 3 ; i >= 0 ; i--)
              /*Serial.print(int(comando[9+i]), HEX), Serial.print(" "),*/fx = (fx << 8) | comando[9+i];
              
          for(int i = 3 ; i >= 0 ; i--)
              /*Serial.print(int(comando[13+i]), HEX), Serial.print(" "),*/fy = (fy << 8) | comando[13+i];
            
            /*
            for(int i = 3 ; i < idx-2; i++){
                Serial.print(int(comando[i]), HEX), Serial.print(" ");
            }
            Serial.println();*/
            
            Serial.print(fx);
            Serial.print("\t");
            Serial.print(fy);
            Serial.print("\t");
            t = millis();
          }
          else if(comando[4] == 1){
              q2 = comando[8];
              rng = 0;
              for(int i = 3 ; i >= 0 ; i--)
                  rng = (rng << 8) | comando[9+i];
          
          
            
            Serial.print(rng);
            Serial.print("\t");
          }
          /*
          Serial.print(comando[0]);
            Serial.print(comando[1]);
            Serial.print(comando[2]);
            for(int i = 3; i < idx-2; i++)
            Serial.print(int(comando[i]), HEX), Serial.print(" ");
            Serial.print("CRC calc: "); Serial.print(int(crc8_func(idx-6, comando + 3)), HEX);
            Serial.println();
          */
          
            Serial.print("\n");
          comando[0] = '$';
          comando[1] = 'X';
          idx = 2;
      }
  }
  Serial.println();
  // put your main code here, to run repeatedly:

}
