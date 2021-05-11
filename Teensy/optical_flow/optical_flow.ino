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
  long t = millis();
  
  while(millis() - t < 10000000){
      bool flag = false;
      while(Serial1.available()){
          Serial.print(char(Serial1.read()));
          flag = true;
      }  
      if(flag)
        Serial.println();
  }
  Serial.println();
  // put your main code here, to run repeatedly:

}
