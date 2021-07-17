#include "ubloxM8Q.h"

ubxPacket myPacket, readPacket;
uint32_t lat_pres = 0;
uint32_t lon_pres = 0;
int flag = 0, head = 0;
int32_t headMot = 0;
int32_t velN = 0;
int32_t velE = 0;
int32_t velD = 0;
float fvelN, auxvelN = 0;
float fvelE, auxvelE = 0;
float fvelD, auxvelD = 0;
float fheadMot, auxheadMot = 0;
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
                
                printPacket(&readPacket);
                
                int latitude = 0, longitud = 0;
                
                for(int i = 0 ; i < 4 ; i++) 
                    latitude = (latitude << 8) | (readPacket.payload[27-i] & 0xFF);
                for(int i = 0 ; i < 4 ; i++) 
                    longitud = (longitud << 8) | (readPacket.payload[31-i] & 0xFF);
                for( int i = 0; i < 4; i++)
                    velN = (velN << 8) | (readPacket.payload[51-i] & 0xFF);                                
                for( int i = 0; i < 4; i++)
                    velE = (velE << 8) | (readPacket.payload[55-i] & 0xFF);                                
                for( int i = 0; i < 4; i++)
                    velD = (velD << 8) | (readPacket.payload[59-i] & 0xFF);                                
                for( int i = 0; i < 4; i++)
                    headMot = (headMot << 8) | (readPacket.payload[69-i] & 0xFF);                                
                
                fheadMot = headMot/100000.0;
                fvelN = velN/1000.0;
                fvelE = velE/1000.0;
                fvelD = velD/1000.0; 
                
                
                //Serial.println(fheadMot,5);
                //Serial.print("\t");
                //Serial.print(fvelN*100.0,3);
                //Serial.print("\t");
                //Serial.println(fvelE*100.0,3);

                /*int latitude = 0, longitud = 0;
                for(int i = 0 ; i < 4 ; i++) 
                    latitude = (latitude << 8) | (readPacket.payload[7-i] & 0xFF);
                for(int i = 0 ; i < 4 ; i++) 
                    longitud = (longitud << 8) | (readPacket.payload[11-i] & 0xFF);
                for(int i = 0 ; i < 4 ; i++) 
                    lat_pres = (lat_pres << 8) | (readPacket.payload[23-i] & 0xFF);
                for(int i = 0 ; i < 4 ; i++) 
                    lon_pres = (lon_pres << 8) | (readPacket.payload[27-i] & 0xFF);

                float lat, lon;
                
                lat = latitude/10000.0;
                lon = longitud/10000.0;*/

                /*Serial.print(lat_pres);
                Serial.print("\t");
                Serial.println(lon_pres);*/

                /*Serial.print(latitude);
                Serial.print("\t");
                Serial.print(longitud);
                Serial.print("\t");
                Serial.print(lat,DEC);
                Serial.print("\t");
                Serial.println(lon,DEC);*/

            }

        }
        //Serial.println("Configured: ");
        //delay(1000);
    }
} 
