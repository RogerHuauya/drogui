#include "sim7600.h"
#include "arduPi.h"
#include "string"
Sim7600::Sim7600(){}

Sim7600::~Sim7600(){}


/**************************Power on Sim7600**************************/
void Sim7600::PowerOn(int PowerKey = powerkey){
   uint8_t answer = 0;

	Serial.begin(115200);

	// checks if the module is started
	answer = sendATcommand("AT", "OK", 2000, true);
	if (answer == 0)
	{
		printf("Starting up...\n");

		
		pinMode(PowerKey, OUTPUT);
		delay(100);
		// power on pulse
		
		digitalWrite(PowerKey, HIGH);
		delay(2000);
		digitalWrite(PowerKey, LOW);
		delay(20000);
		// waits for an answer from the module
		while (answer == 0) {     // Send AT every two seconds and wait for the answer
			answer = sendATcommand("AT", "OK", 2000, false);
		}

	}
	printf("GPS Ready\n");
	delay(5000);

	//while ((sendATcommand("AT+CREG?", "+CREG: 0,1", 500) || sendATcommand("AT+CREG?", "+CREG: 0,5", 500)) == 0)
	//	delay(500);
}

/**************************Phone Calls**************************/
void Sim7600::PhoneCall(const char* PhoneNumber) {
	char aux_str[30];

//	printf("Enter the phone number:");

//	scanf("%s", PhoneNumber);

	sprintf(aux_str, "ATD%s;", PhoneNumber);
	sendATcommand(aux_str, "OK", 10000, true);

	// press the button for hang the call 
	//while (digitalRead(button) == 1);

	delay(20000);

	Serial.println("AT+CHUP");            // disconnects the existing call
	printf("Call disconnected\n");
}


/**************************GPS positoning**************************/
bool Sim7600::GPSPositioning(){

    uint8_t answer = 0;
    bool RecNull = true;
    int i = 0;
    char RecMessage[100];
    char LatDD[2],LatMM[9],LogDD[3],LogMM[9],DdMmYy[6] ,UTCTime[6];
    int DayMonthYear;
    char aux[100]={};  	     
	//printf("Start GPS session...\n");
    sendATcommand("AT+CGPS=1,1", "OK:", 1000, true);    // start GPS session, standalone mode

    delay(2000);

    while(RecNull)
    {
        answer = sendATcommand("AT+CGPSINFO", "+CGPSINFO: ", 1000, true);    // start GPS session, standalone mode
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
            sendATcommand("AT+CGPS=0", "OK:", 1000, true);
            return false;
        }
        delay(1500);

    }
    //data.clear();	
    //data.append(RecMessage);
    printf("Data: %s raa\n",RecMessage);
    //strncpy(aux,RecMessage,80);
    //printf("Data: %s ra \n",aux);
    
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

    sendATcommand("AT+CGPS=0", "OK:", 1000, true);
	return true;
}

/**************************Other functions**************************/
char Sim7600::sendATcommand(const char* ATcommand, unsigned int timeout, bool debug) {
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
			if(debug) printf("_%c", response[x]);
			x++;
		}
		if(debug) printf("%d", x);
	} while ((answer == 0) && ((millis() - previous) < timeout));

	return answer;
}

char Sim7600::sendATcommand(const char* ATcommand, const char* expected_answer, unsigned int timeout, bool debug) {

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
			if(debug) printf("%c", response[x]);
			x++;
			// check if the desired answer  is in the response of the module
			if (strstr(response, expected_answer) != NULL)
			{
				if(debug) printf("\n");
				answer = 1;
			}
		}
	}
	// Waits for the asnwer with time out
	while ((answer == 0) && ((millis() - previous) < timeout));

	return answer;
}

char Sim7600::sendATcommand2(const char* ATcommand, const char* expected_answer1, const char* expected_answer2, unsigned int timeout){
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

void Sim7600::GPSStart(){
    sendATcommand("AT+CGPS=0,0", "OK:", 1000, false);
    sendATcommand("AT+CGPS=1,1", "OK:", 1000, false);    // start GPS session, standalone mode
    delay(5000);

}
bool Sim7600::GPSGet(){
    uint8_t answer = 0;
    bool RecNull = true;
    int i = 0;
    char RecMessage[200];
    char LatDD[2],LatMM[9],LogDD[3],LogMM[9],DdMmYy[6] ,UTCTime[6];
    int DayMonthYear;
    memset(RecMessage, '\0', sizeof(RecMessage));    

    answer = sendATcommand("AT+CGPSINFO", "+CGPSINFO: ", 1000, false);    // start GPS session, standalone mode
    if (answer == 1){
        answer = 0;
        while(Serial.available() == 0);
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
        
/*
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
        }*/ 
            
        
    }
    else{
        sendATcommand("AT+CGPS=0", "OK:", 1000, false);
        return false;
    }
    delay(100);
    int DLat, DLon;
    sscanf(RecMessage, "%2d%lf,%*c,%3d%lf,%*c,%*lf,%*lf,%lf,%lf,%lf", &DLat,&Lat,&DLon, &Log, &Alt, &Vel, &Curso);
    //printf(RecMessage);
    Lat = DLat + Lat/60.0;
    Log = DLon + Log/60.0;
    //printf("%.10lf, %.10lf\n", Lat, Log);
    //strcpy(data, RecMessage);
    //printf("%s\n",RecMessage);
    /*
    strncpy(LatDD,RecMessage,2);
    strncpy(LatMM,RecMessage+2,9);
    Lat = atoi(LatDD) + (atof(LatMM)/60);
    if(RecMessage[12] != 'N' && RecMessage[12] != 'S')
        return false;

    strncpy(LogDD,RecMessage+14,3);
    strncpy(LogMM,RecMessage+17,9);
    Log = atoi(LogDD) + (atof(LogMM)/60);
    if(RecMessage[27] != 'E' && RecMessage[27] != 'W')
        return false;
    
    strncpy(DdMmYy,RecMessage+29,6);
    DdMmYy[6] = '\0';

    strncpy(UTCTime,RecMessage+36,6);
    UTCTime[6] = '\0';    
	return true;*/
    return true;
}
Sim7600 sim7600 = Sim7600();
