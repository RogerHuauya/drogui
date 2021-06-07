const int pwm_1=5;

void setup() {
  // put your setup code here, to run once:
pinMode(pwm_1,OUTPUT);
}

void loop() {

for(int i=0; i<255; i++){
analogWrite(pwm_1,i); delay(100);}
delay(100);
for(int i=255; i>1; i--){
analogWrite(pwm_1,i);delay(100);}
delay(100);
}
