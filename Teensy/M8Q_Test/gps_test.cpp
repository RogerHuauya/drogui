#include "ubloxM8Q.h"

ubxPacket myPacket;

void calcChecksum(ubxPacket *msg)
{
  msg->checksumA = 0;
  msg->checksumB = 0;

  msg->checksumA += msg->cls;
  msg->checksumB += msg->checksumA;

  msg->checksumA += msg->id;
  msg->checksumB += msg->checksumA;

  msg->checksumA += (msg->len & 0xFF);
  msg->checksumB += msg->checksumA;

  msg->checksumA += (msg->len >> 8);
  msg->checksumB += msg->checksumA;

  for (uint16_t i = 0; i < msg->len; i++)
  {
    msg->checksumA += msg->payload[i];
    msg->checksumB += msg->checksumA;
  }
}

void sendSerialCommand(ubxPacket *outgoingUBX)
{
  //Write header bytes
  Serial1.write(UBX_SYNCH_1); //μ - oh ublox, you're funny. I will call you micro-blox from now on.
  Serial1.write(UBX_SYNCH_2); //b
  Serial1.write(outgoingUBX->cls);
  Serial1.write(outgoingUBX->id);
  Serial1.write(outgoingUBX->len & 0xFF); //LSB
  Serial1.write(outgoingUBX->len >> 8);   //MSB

  //Write payload.
  for (int i = 0; i < outgoingUBX->len; i++)
  {
    Serial1.write(outgoingUBX->payload[i]);
  }

  //Write checksum
  Serial1.write(outgoingUBX->checksumA);
  Serial1.write(outgoingUBX->checksumB);
}

uint8_t cfgPortArray[20] = { 1, 0, 0, 0, 0xC0, 8, 0, 0, 0x00, 0x08, 0x07, 0, 7, 0, 1, 0, 0, 0, 0, 0};
void cfgPort(){
  myPacket.cls = UBX_CLASS_CFG;
  myPacket.id  = UBX_CFG_PRT;
  myPacket.len = 20;
  for( int i = 0; i < 20; i++ ) myPacket.payload[i] = cfgPortArray[i];
  calcChecksum(&myPacket);
  sendSerialCommand(&myPacket);
}

void setAutoPVT(){

    myPacket.cls = UBX_CLASS_CFG;
    myPacket.id = UBX_CFG_MSG;
    myPacket.len = 3;
    myPacket.startingSpot = 0;
    myPacket.payload[0] = UBX_CLASS_NAV;
    myPacket.payload[1] = UBX_NAV_PVT;
    myPacket.payload[2] = 1; 

    calcChecksum(&myPacket);
    sendSerialCommand(&myPacket);
}

uint8_t cfgRateArray[6] = { 0xE8, 3, 1, 0, 1, 0 };
void cfgRate(){

    myPacket.cls = UBX_CLASS_CFG;
    myPacket.id = UBX_CFG_RATE;
    myPacket.len = 6;
    for( int i = 0; i < 6; i++ ) myPacket.payload[i] = cfgRateArray[i];

    calcChecksum(&myPacket);
    sendSerialCommand(&myPacket);
}
void cfgRate2(){

    myPacket.cls = UBX_CLASS_MON;
    myPacket.id = UBX_MON_VER;
    myPacket.len = 0;

    calcChecksum(&myPacket);
    sendSerialCommand(&myPacket);
}

int flag = 0, head = 0;
uint8_t buff[100000];
int _main(){

    Serial1.begin(9600);
    cfgPort();
    delay(100);
    Serial1.begin(460800);
    cfgRate();
    cfgRate2();

    setAutoPVT();
    
    
    //setAutoPVT();
    delay(100);
    while(1){
        if(Serial1.available()){
            uint8_t c = Serial1.read();
            buff[head++] = c;
            //Serial.print(c, HEX);
            //Serial.print(" ");
        }
        if( head > 1000 ) head = 0;
        if(head > 2 && buff[head-1] == 0x62 && buff[head-2] == 0xB5 ){
          for( int i = 0; i < head-2 ; i++) Serial.print((char)buff[i]), Serial.print(" ");
          Serial.print(head-2-2-2-2-2);
          Serial.println("");
          buff[0] = 0xB5;
          buff[1] = 0x62;
          head = 2;
        }
    }
} 