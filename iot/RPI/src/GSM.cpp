#include "GSM.h"
#include "arduPi.h"

GSM::GSM(){
}

GSM::~GSM(){
}


/**************************Power on GSM**************************/
void GSM::PowerOn(int PowerKey = powerkey){
   uint8_t answer = 0;

	Serial.begin(115200);

	// checks if the module is started
	answer = sendATcommand("AT", "OK", 2000);
	if (answer == 0)
	{
		printf("Starting up...\n");

		
		pinMode(PowerKey, OUTPUT);
		// power on pulse
		digitalWrite(PowerKey, HIGH);
		delay(600);
		digitalWrite(PowerKey, LOW);
		
		// waits for an answer from the module
		while (answer == 0) {     // Send AT every two seconds and wait for the answer
			answer = sendATcommand("AT", "OK", 2000);
		}

	}

	delay(5000);

	while ((sendATcommand("AT+CREG?", "+CREG: 0,1", 500) || sendATcommand("AT+CREG?", "+CREG: 0,5", 500)) == 0)
		delay(500);
}
/**************************GPS positoning**************************/
bool GSM::GPSPositioning(){

    uint8_t answer = 0;
    bool RecNull = true;
	int i = 0;
	char RecMessage[200];
    char LatDD[2],LatMM[9],LogDD[3],LogMM[9],DdMmYy[6] ,UTCTime[6];
    int DayMonthYear;
    float Lat,Log;

	printf("Start GPS session...\n");
    sendATcommand("AT+CGPS=1,1", "OK:", 1000);    // start GPS session, standalone mode

    delay(2000);

    while(RecNull)
    {
        answer = sendATcommand("AT+CGPSINFO", "+CGPSINFO: ", 1000);    // start GPS session, standalone mode

        if (answer == 1)
        {
            answer = 0;
            while(Serial.available() == 0);
            // this loop reads the data of the SMS
            do{
                // if there are data in the UART input buffer, reads it and checks for the asnwer
                if(Serial.available() > 0){    
                    RecMessage[i] = Serial.read();
                    i++;
                    // check if the desired answer (OK) is in the response of the module
                    if (strstr(RecMessage, "OK") != NULL)    
                    {
                        answer = 1;
                    }
                }
            }while(answer == 0);    // Waits for the asnwer with time out
            
            RecMessage[i] = '\0';
            
            printf("%s\n",RecMessage); 


            if (strstr(RecMessage, ",,,,,,,,") != NULL) 
            {
                memset(RecMessage, '\0', i);    // Initialize the string
                RecNull = true;
                i = 0;
                answer = 0;
                delay(1000);
            }
            else
            {
                RecNull = false;
            } 
              
            
        }
        else
        {
            printf("error %o\n",answer);
            sendATcommand("AT+CGPS=0", "OK:", 1000);
            return false;
        }
        delay(1500);

    }

    strncpy(LatDD,RecMessage,2);
    strncpy(LatMM,RecMessage+2,9);
    Lat = atoi(LatDD) + (atof(LatMM)/60);
    if(RecMessage[12] == 'N')
        printf("Latitude is %f N\n",Lat);
    else if(RecMessage[12] == 'S')
        printf("Latitude is %f S\n",Lat);
    else
        return false;

    strncpy(LogDD,RecMessage+14,3);
    strncpy(LogMM,RecMessage+17,9);
    Log = atoi(LogDD) + (atof(LogMM)/60);
    if(RecMessage[27] == 'E')
        printf("Longitude is %f E\n",Log);
    else if(RecMessage[27] == 'W')
        printf("Longitude is %f W\n",Log);
    else
        return false;

    strncpy(DdMmYy,RecMessage+29,6);
    DdMmYy[6] = '\0';
    printf("Day Month Year is %s\n",DdMmYy);

    strncpy(UTCTime,RecMessage+36,6);
    UTCTime[6] = '\0';
    printf("UTC time is %s\n",UTCTime);

    sendATcommand("AT+CGPS=0", "OK:", 1000);
	return true;
}

/**************************Other functions**************************/
char GSM::sendATcommand(const char* ATcommand, unsigned int timeout) {
	uint8_t x = 0, answer = 0;
	char response[100];
	unsigned long previous;
	memset(response, '\0', 100);    // Initialize the string

	delay(100);

	while (Serial.available() > 0) Serial.read();    // Clean the input buffer

	Serial.println(ATcommand);    // Send the AT command 

	previous = millis();

	// this loop waits for the answer
	do {
		// if there are data in the UART input buffer, reads it and checks for the asnwer
		if (Serial.available() != 0) {
			response[x] = Serial.read();
			printf("%c", response[x]);
			x++;
		}
		
	} while ((answer == 0) && ((millis() - previous) < timeout));

	return answer;
}

char GSM::sendATcommand(const char* ATcommand, const char* expected_answer, unsigned int timeout) {

	char x = 0, answer = 0;
	char response[100];
	unsigned long previous;

	memset(response, '\0', 100);    // Initialize the string

	delay(100);

	while (Serial.available() > 0) Serial.read();    // Clean the input buffer

	Serial.println(ATcommand);    // Send the AT command 


	x = 0;
	previous = millis();

	// this loop waits for the answer
	do {
		if (Serial.available() != 0) {
			// if there are data in the UART input buffer, reads it and checks for the asnwer
			response[x] = Serial.read();
			printf("%c", response[x]);
			x++;
			// check if the desired answer  is in the response of the module
			if (strstr(response, expected_answer) != NULL)
			{
				printf("\n");
				answer = 1;
			}
		}
	}
	// Waits for the asnwer with time out
	while ((answer == 0) && ((millis() - previous) < timeout));

	return answer;
}

char GSM::sendATcommand2(const char* ATcommand, const char* expected_answer1, const char* expected_answer2, unsigned int timeout){
	uint8_t x=0,  answer=0;
    char response[100];
    unsigned long previous;

    memset(response, '\0', 100);    // Initialize the string

    delay(100);

    while( Serial.available() > 0) Serial.read();    // Clean the input buffer

    Serial.println(ATcommand);    // Send the AT command 

    x = 0;
    previous = millis();

    // this loop waits for the answer
    do{
        // if there are data in the UART input buffer, reads it and checks for the asnwer
        if(Serial.available() != 0){    
            response[x] = Serial.read();
            printf("%c",response[x]);
            x++;
            // check if the desired answer 1  is in the response of the module
            if (strstr(response, expected_answer1) != NULL)    
            {
				printf("\n");
                answer = 1;
            }
            // check if the desired answer 2 is in the response of the module
            else if (strstr(response, expected_answer2) != NULL)    
            {
				printf("\n");
                answer = 2;
            }
        }
    }
    // Waits for the asnwer with time out
    while((answer == 0) && ((millis() - previous) < timeout));    

    return answer;

}

GSM sim7600 = GSM();

