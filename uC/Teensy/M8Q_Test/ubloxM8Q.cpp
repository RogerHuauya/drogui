#include "ubloxM8Q.h"

uint8_t defaultCfgPort[20] = { 1, 0, 0, 0, 0xC0, 8, 0, 0, 0x00, 0x08, 0x07, 0, 7, 0, 1, 0, 0, 0, 0, 0};
uint8_t defaultCfgRate[6]  = { 0x7D, 0, 1, 0, 1, 0 };
uint8_t defaultCfgMsg[3]   = {UBX_CLASS_NAV, UBX_NAV_PVT, 1};

Stream *ser = &Serial1;
void calcChecksum(ubxPacket *msg){

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
	ser->write(UBX_SYNCH_1); //μ - oh ublox, you're funny. I will call you micro-blox from now on.
	ser->write(UBX_SYNCH_2); //b
	ser->write(outgoingUBX->cls);
	ser->write(outgoingUBX->id);
	ser->write(outgoingUBX->len & 0xFF); //LSB
	ser->write(outgoingUBX->len >> 8);   //MSB

	//Write payload.
	for (int i = 0; i < outgoingUBX->len; i++)
	{
		ser->write(outgoingUBX->payload[i]);
	}

	//Write checksum
	ser->write(outgoingUBX->checksumA);
	ser->write(outgoingUBX->checksumB);
}


void cfgM8Q(ubxPacket *mp, uint8_t id,uint8_t len, uint8_t *cfgArray){
	mp->cls = UBX_CLASS_CFG;
	mp->id  = id;
	mp->len = len;
	for( int i = 0; i < len; i++ ) mp->payload[i] = cfgArray[i];
	calcChecksum(mp);
	sendSerialCommand(mp);
}


void cfgPort(ubxPacket *mp, uint8_t *cfgPortArray){
	mp->cls = UBX_CLASS_CFG;
	mp->id  = UBX_CFG_PRT;
	mp->len = 20;
	for( int i = 0; i < 20; i++ ) mp->payload[i] = cfgPortArray[i];
	calcChecksum(mp);
	sendSerialCommand(mp);
}

void cfgMsg(ubxPacket *mp, uint8_t *cfgMsgArray){

	mp->cls = UBX_CLASS_CFG;
	mp->id = UBX_CFG_MSG;
	mp->len = 3;
	mp->startingSpot = 0;
	for( int i = 0; i < 3; i++ ) mp->payload[i] = cfgMsgArray[i];

	calcChecksum(mp);
	sendSerialCommand(mp);
}

void cfgRate(ubxPacket *mp, uint8_t *cfgRateArray){

	mp->cls = UBX_CLASS_CFG;
	mp->id = UBX_CFG_RATE;
	mp->len = 6;
	for( int i = 0; i < 6; i++ ) mp->payload[i] = cfgRateArray[i];

	calcChecksum(mp);
	sendSerialCommand(mp);
}

void cfgRate2(ubxPacket *mp){

	mp->cls = UBX_CLASS_MON;
	mp->id = UBX_MON_VER;
	mp->len = 0;

	calcChecksum(mp);
	sendSerialCommand(mp);
}


bool readM8Q(ubxPacket *mp){
	uint8_t sync1 = 0, sync2 = 0, cntLSB, cntMSB;
	uint8_t checksumA, checksumB, flag = 0;
	while(1){
		if(ser->available()){
			sync1 = sync2;
			sync2 = ser->read();
		}

		if(sync1 == UBX_SYNCH_1 && sync2 == UBX_SYNCH_2){
			while(!ser->available());
			mp->cls = ser->read();
			while(!ser->available());
			mp->id = ser->read();

			while(!ser->available());
			cntLSB = ser->read();
			while(!ser->available());
			cntMSB = ser->read();

			mp->len = (uint16_t) (cntMSB << 8) | (cntLSB & 0xFF);

			for(int i = 0; i < mp->len ; i++){
				while(!ser->available());
				mp->payload[i] = ser->read();
			}

			while(!ser->available());
			checksumA = ser->read();

			while(!ser->available());
			checksumB = ser->read();

			calcChecksum(mp);
			flag = 1;
		}

		if(flag == 1) break;

	}

	return (mp->checksumA == checksumA) && (mp->checksumB == checksumB);
}


void printPacket(ubxPacket *mp){
	Serial.print("Class:\t"); 
	Serial.print(mp->cls);
	Serial.print("\tID:\t");
	Serial.print(mp->id);
	Serial.print("\tlen:\t");
	Serial.println(mp->len);

	for( int i = 0; i < mp->len ; i++) 
		Serial.print(mp->payload[i],HEX), Serial.print(" ");
	Serial.println();

}