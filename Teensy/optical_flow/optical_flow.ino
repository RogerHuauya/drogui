#define Optical Serial2

void setup() {
  // put your setup code here, to run once:
  Optical.begin(115200);
  Serial.begin(9600);

}

void loop() {
  /*uint8_t send_buff[] = {'$', 'M', '<', 0, 7};
  for(int i = 0 ; i < 5 ; i++) Serial1.write(send_buff[i]);*/
  char comando[50];
  int idx = 0;
  uint8_t q1, q2;
  int32_t fx = 0, fy = 0;
  int32_t rng;
  long t = millis();
  while(1){
      if(Optical.available()){
          comando[idx++] = Optical.read();
      }  
      if(idx > 2 && comando[idx-1] == 'X' && comando [idx-2] == '$'){
          if(comando[4] == 2){
            /*Serial.print(comando[0]);
            Serial.print(comando[1]);
            Serial.print(comando[2]);*/
            
            q1 = comando[8];
            fx = 0, fy = 0;
            for(int i = 3 ; i >= 0 ; i--)
              /*Serial.print(int(comando[9+i]), HEX), Serial.print(" "),*/ fx = (fx << 8) | comando[9+i];
              
            for(int i = 3 ; i >= 0 ; i--)
              /*Serial.print(int(comando[13+i]), HEX), Serial.print(" "),*/ fy = (fy << 8) | comando[13+i];
            
            /*
            for(int i = 3 ; i < idx-2; i++){
                Serial.print(int(comando[i]), HEX), Serial.print(" ");
            }
            Serial.println();*/
            //Serial.print(millis()-t);
            /*Serial.print("\t");
            Serial.print(q1);
            Serial.print("\t");
  
            Serial.print(fx);
            Serial.print("\t");
            Serial.print(fy);
            Serial.print("\n");*/
            //t = millis();
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

          }
          /*Serial.print(q2);
          Serial.print("\t");
          
          Serial.print(rng);
          Serial.print("\t");
          */
          
          /*Serial.print(rng);
          Serial.print("\t");*/
          Serial.print(fx);
          Serial.print("\t");
          Serial.print(fy);
          Serial.print("\n");
          t = millis();

          comando[0] = '$';
          comando[1] = 'X';
          idx = 2;
      }
  }
  Serial.println();
  // put your main code here, to run repeatedly:

}
