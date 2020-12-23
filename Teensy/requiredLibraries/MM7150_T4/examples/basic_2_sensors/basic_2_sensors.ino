#include <MM7150_T4.h>
uint8_t state = 0;
uint8_t result;
accel acc;
orient ori;
char out[40];
void setup() {
  
	pinMode(LED_BUILTIN,OUTPUT); // LED
	pinMode(17,OUTPUT);
	pinMode(PIN_INT, INPUT);
	Wire.setClock(400000);
	Wire.begin(); 
	Wire.setSDA(18);
	Wire.setSCL(19);
	digitalWrite(LED_BUILTIN, LOW);
	attachInterrupt(digitalPinToInterrupt(PIN_INT), IntPinHandler, CHANGE);
	delay(1000);
	result = VREG_init();    
	Serial.println(result);            // Initialize VREG registers
	digitalWrite(LED_BUILTIN, HIGH);

	acc.init(100,1);
	ori.init(50,200);
}

void loop() {
	acc.read();
	ori.read();
	sprintf(out, " %4.9f\t%4.9f\t%4.9f",100*acc.dDataX, 100*acc.dDataY, 100*acc.dDataZ);
	Serial.println(out);
	delay(10);
}
