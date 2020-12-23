#include <MM7150_T4.h>
uint8_t state = 0;
uint8_t result;
accel acc;
compass com;
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

	acc.init(100,20);
    com.init(100,1);
}

void loop() {
	com.read();
	sprintf(out, "Com:: alp: %4.3f",com.dDataX);
	Serial.println(out);

	delay(10);
}