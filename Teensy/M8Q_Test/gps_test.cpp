#include "ubloxM8Q.h"

ubxPacket myPacket, readPacket;

int flag = 0, head = 0;
uint8_t buff[1000];
int _main(){

    Serial1.begin(9600);
    cfgPort(&myPacket, defaultCfgPort);
    delay(100);
    Serial1.begin(460800);
    cfgRate(&myPacket, defaultCfgRate);
    cfgMsg(&myPacket, defaultCfgMsg);

    delay(100);
    while(1){
        if(Serial1.available()){
            if(readM8Q(&readPacket)){
                printPacket(&readPacket);
            }
        }
    }
} 