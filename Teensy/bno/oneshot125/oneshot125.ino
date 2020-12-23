void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  analogWriteResolution(16);
  pinMode(5,OUTPUT);
  analogWriteFrequency(5,3500);
}

void loop() {
  double t_total = 10000.0/35; // 
  int pwm_total = ((1<<16)-1);
  double ini = 125, fin = 255;
  for(int i = 0 ; i < 100 ; i++){
    Serial.println((i*(fin-ini)/100+ini)/t_total);
    analogWrite(5,(i*(fin-ini)/100+ini)/t_total*(pwm_total));
    delay(10);
  }
  // put your main code here, to run repeatedly
}
