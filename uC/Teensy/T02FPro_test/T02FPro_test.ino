#define SYNC 0x59
uint8_t buff[8];
void setup(){
	Serial.begin(9600);
	Serial1.begin(115200);
}

void loop(){
	if(Serial1.available()){
		uint8_t head1 = Serial1.read();
		if(head1 == SYNC){
			while(!Serial1.available()){}
			uint8_t head2 = Serial1.read();
			uint8_t chksum_cal = SYNC;
			uint8_t chksum;
			if(head2 == SYNC){
				for(int i = 0; i < 6; i++){
					while(!Serial1.available()){}
					buff[i] = Serial1.read();
					chksum_cal += buff[i];
					//Serial.print(buff[i], HEX);
					//Serial.print(" ");
				}
				while(!Serial1.available()){}
				chksum = Serial1.read();
				//Serial.println();
			}
			Serial.print(chksum, HEX);
			Serial.print(" ");
			Serial.println(chksum, HEX);
			//Serial.printf("Distance: %d, Strength: %d, SIG: %d\n", (int)(buff[1] << 8) | buff[0], (int)(buff[3] << 8) | buff[2], (int) buff[4]);
		}
	}
	//while(Serial1.available()){Serial.println(Serial1.read(), HEX);}
	//delay(100);
}
