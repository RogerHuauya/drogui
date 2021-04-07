#include "ubloxM8Q.h"

ubxPacket myPacket, readPacket;

int flag = 0, head = 0;
uint8_t buff[1000];
int _main(){

    delay(1000);
    Serial1.begin(9600);
    cfgPort(&myPacket, defaultCfgPort);
    delay(100);
    Serial1.begin(460800);

    cfgRate(&myPacket, defaultCfgRate);
    //readM8Q(&readPacket);
    cfgMsg(&myPacket, defaultCfgMsg);
    //readM8Q(&readPacket);
    delay(100);

    while(1){
        /*if(Serial1.available()){
            if(readM8Q(&readPacket)){
                
                //printPacket(&readPacket);

                /*int latitude = 0, longitud = 0;
                for(int i = 0 ; i < 4 ; i++) 
                    latitude = (latitude << 8) | (readPacket.payload[27-i] & 0xFF);
                for(int i = 0 ; i < 4 ; i++) 
                    longitud = (longitud << 8) | (readPacket.payload[31-i] & 0xFF);
                
                Serial.print(latitude);
                Serial.print("\t");
                Serial.println(longitud);
                
            }

        }*/
        Serial.println("Configured: ");
        delay(1000);
    }
} 