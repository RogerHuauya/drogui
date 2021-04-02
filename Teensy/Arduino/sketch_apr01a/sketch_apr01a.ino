int en_a[4] = { 4, 0, 0, 0} ;
int en_b[4] = { 3, 0, 0, 0} ;
int dir[4] = { 3, 5, 7, 9};
int pwm_mot[4] = {2, 4, 6, 8};
int state = 1;
void setup() {
  // put your setup code here, to run once:
  pinMode(13,OUTPUT);
  for( int i = 0; i < 4; i++ ) 
    pinMode(dir[i],OUTPUT), 
    pinMode(pwm_mot[i], OUTPUT);
}

void loop() {
  digitalWrite(13,state);
  for( int i = 0; i < 4; i++ ) analogWrite(pwm_mot[i],120);
  digitalWrite(dir[0],LOW), digitalWrite(dir[1],LOW),
  digitalWrite(dir[2],HIGH), digitalWrite(dir[3],HIGH);
  
  state = 1- state;
  delay(50);
}
