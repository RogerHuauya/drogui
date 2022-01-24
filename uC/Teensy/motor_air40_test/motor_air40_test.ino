
typedef struct _pwm{
    int n;
    float period; 
    float min_time, range_time;
} pwm;

void initPwmPin(pwm* p, int n){
    p->n = n;
    //analogWriteResolution(16);
    pinMode(n, OUTPUT);
}


void setPwmFrecuency(pwm* p, float freq){
    //analogWriteFrequency(p->n, freq);
    p->period = 1000000.0/freq;
}

void setPwmDuty(pwm* p, float percent){
    analogWrite(p->n, ((1 << 8)-1)*(percent/100)  );
}

void setPwmDutyLimits(pwm *p, int min_time,int max_time){
    p->min_time = min_time;
    p->range_time = max_time - min_time;
}

void setPwmDutyTime(pwm *p, float percent){
    float tim = (p->range_time)*percent/100.0 + p -> min_time;
    analogWrite(p->n, tim/(p->period) * ((1 << 8)-1));

}

void initPWM(pwm* p, int pin){
    initPwmPin(p, pin);
    setPwmDutyLimits(p, 1000, 2000);
    setPwmFrecuency(p, 490); //  PWM frequency of D3 is 490 Hz
    setPwmDutyTime(p, 0);
}

pwm m1;

void setup(){
	Serial.begin(9600);
    initPWM(&m1, 3);
}

int main(){
	int pot = 0;
    while(1){
        //int pot = analogRead(24);
		    pot = Serial.parseInt();
        Serial.println(pot);
        setPwmDutyTime(&m1, constrain(pot,0 ,100));
        digitalWrite(13, HIGH);
        delay(100);
    }
}
