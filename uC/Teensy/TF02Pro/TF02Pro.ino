void setup(){
	Serial.begin(9600);
	Serial1.begin(115200);
}

void loop(){

	while(Serial1.available()){
		Serial.println(Serial1.read(), HEX);
	}
}
