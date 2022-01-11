#include <SD.h>


const int chipSelect = BUILTIN_SDCARD;

void setup(){
	// Open serial communications and wait for port to open:
	Serial.begin(9600);
	//Serial1.begin(2000000);
	while (!Serial);

	Serial.print("Initializing SD card...");

	if (!SD.begin(chipSelect)) {
		Serial.println("Card failed, or not present");
		return;
	}
	Serial.println("card initialized.");
}

void loop()
{
	String dataString = "";
	while(Serial.available()){
		dataString += Serial.read();
	}

	if(dataString != ""){

		File dataFile = SD.open("log.txt", FILE_WRITE);

		if (dataFile) {
			dataFile.println(dataString);
			dataFile.close();
			Serial.println(dataString);
		}
		else {
			Serial.println("error opening datalog.txt");
		}
	}
}
