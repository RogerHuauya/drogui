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
    readM8Q(&readPacket);
    cfgMsg(&myPacket, defaultCfgMsg);
    readM8Q(&readPacket);
    delay(100);

    while(1){

        if(Serial1.available()){
            if(readM8Q(&readPacket)){
                
                //printPacket(&readPacket);

                int latitude = 0, longitud = 0;
                for(int i = 0 ; i < 4 ; i++) 
                    latitude = (latitude << 8) | (readPacket.payload[7-i] & 0xFF);
                for(int i = 0 ; i < 4 ; i++) 
                    longitud = (longitud << 8) | (readPacket.payload[11-i] & 0xFF);
                
                float lat, lon;
                
                lat = latitude/10000.0;
                lon = longitud/10000.0;

                Serial.print(latitude);
                Serial.print("\t");
                Serial.print(longitud);
                Serial.print("\t");
                Serial.print(lat,DEC);
                Serial.print("\t");
                Serial.println(lon,DEC);

            }

        }
        //Serial.println("Configured: ");
        //delay(1000);
    }
} 
