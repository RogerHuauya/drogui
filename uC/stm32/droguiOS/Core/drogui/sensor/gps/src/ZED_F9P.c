#include "macros.h"

#ifdef ZED_F9P

#include "ZED_F9P.h"
#include "serial.h"
#include "task.h"
#include "macros.h"
#include <stdint.h>


static uint8_t defaultCfgPort[20] = { 1, 0, 0, 0, 0xC0, 8, 0, 0, 0x00, 0x10, 0x0E, 0, 1, 0, 1, 0, 0, 0, 0, 0};

//uint8_t defaultCfgRate[6]  = { 0xF4, 1, 1, 0, 1, 0 };
static uint8_t defaultCfgRate[6]  = { 0x64, 0, 1, 0, 1, 0 }; // 8hz

static uint8_t defaultCfgMsg[3]   = {UBX_CLASS_NAV, UBX_NAV_PVT, 1};

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

static void sendSerialCommand(ubxPacket *outgoingUBX){
	//Write header bytes
	serialWrite(outgoingUBX->ser, UBX_SYNCH_1); //μ - oh ublox, you're funny. I will call you micro-blox from now on.
	serialWrite(outgoingUBX->ser, UBX_SYNCH_2); //b
	serialWrite(outgoingUBX->ser, outgoingUBX->cls);
	serialWrite(outgoingUBX->ser, outgoingUBX->id);
	serialWrite(outgoingUBX->ser, outgoingUBX->len & 0xFF); //LSB
	serialWrite(outgoingUBX->ser, outgoingUBX->len >> 8);   //MSB

	//Write payload.
	for (int i = 0; i < outgoingUBX->len; i++)
	{
		serialWrite(outgoingUBX->ser, outgoingUBX->payload[i]);
	}

	//Write checksum
	serialWrite(outgoingUBX->ser, outgoingUBX->checksumA);
	serialWrite(outgoingUBX->ser, outgoingUBX->checksumB);
}

SENSOR_STATUS readPacket(ubxPacket *mp, uint32_t timeout){
	uint8_t sync1 = 0, sync2 = 0, cntLSB, cntMSB;
	uint8_t checksumA, checksumB, flag = 0;
	uint32_t tim = TIME;
	while(TIME - tim < timeout){
		if(serialAvailable(mp->ser)){
			sync1 = sync2;
			sync2 = serialRead(mp->ser);
		}

		if(sync1 == UBX_SYNCH_1 && sync2 == UBX_SYNCH_2){
			while(!serialAvailable(mp->ser));
			mp->cls = serialRead(mp->ser);
			while(!serialAvailable(mp->ser));
			mp->id = serialRead(mp->ser);

			while(!serialAvailable(mp->ser));
			cntLSB = serialRead(mp->ser);
			while(!serialAvailable(mp->ser));
			cntMSB = serialRead(mp->ser);

			mp->len = (uint16_t) (cntMSB << 8) | (cntLSB & 0xFF);

			for(int i = 0; i < mp->len ; i++){
				while(!serialAvailable(mp->ser));
				mp->payload[i] = serialRead(mp->ser);
			}

			while(!serialAvailable(mp->ser));
			checksumA = serialRead(mp->ser);

			while(!serialAvailable(mp->ser));
			checksumB = serialRead(mp->ser);

			calcChecksum(mp);
			flag = 1;
		}

		if(flag == 1) break;

	}

	if(flag == 0) return TIMEOUT;
	if((mp->checksumA == checksumA) && (mp->checksumB == checksumB)) return OK;
	else return WRG_CHKSUM;
}


static void printPacket(ubxPacket *mp){
	serialPrintf(SER_DBG,"Class:\t %d \tID:\t %d \tlen:\t %d \n", mp->cls, mp->id, mp->len);

	for( int i = 0; i < mp->len ; i++)
		serialPrintf(SER_DBG,"%x ", mp->payload[i]);
	serialPrint(SER_DBG,"\n");
}

void sndUBX(ubxPacket *mp, uint8_t class, uint8_t id,uint8_t len, uint8_t *array){
	mp->cls = class;
	mp->id  = id;
	mp->len = len;
	for( int i = 0; i < len; i++ ) mp->payload[i] = array[i];
	calcChecksum(mp);
	sendSerialCommand(mp);
}


SENSOR_STATUS initGPS(zed *mg, serial* ser){
	SENSOR_STATUS ret;

	mg->last_tim = TIME;
	mg->threshold = 2000000;

	mg->rcv_pack.ser = ser;
	mg->snd_pack.ser = ser;

	changeBaudrate(mg->rcv_pack.ser,38400);

	HAL_Delay(1000);
	sndUBX(&(mg->snd_pack), UBX_CLASS_CFG, UBX_CFG_PRT, 20, defaultCfgPort);

	HAL_Delay(100);
	changeBaudrate(mg->rcv_pack.ser,921600);
	HAL_Delay(100);

	sndUBX(&(mg->snd_pack), UBX_CLASS_CFG, UBX_CFG_RATE, 6, defaultCfgRate);

	ret = readPacket(&(mg->rcv_pack), 10000);
	if(ret != OK) return ret;

	sndUBX(&(mg->snd_pack), UBX_CLASS_CFG, UBX_CFG_MSG, 3, defaultCfgMsg);

	ret = readPacket(&(mg->rcv_pack), 10000);
	if(ret != OK) return ret;

	HAL_Delay(100);

	mg->cnt = 0;
	return OK;
}


SENSOR_STATUS readGPS(zed *mg){
	if(serialAvailable(mg->rcv_pack.ser)){
		int ret = readPacket(&(mg->rcv_pack), 1000);
		//serialFlush();
		//printPacket(&(mg->rcv_pack));

		if( ret != OK){
			if(  TIME - mg->last_tim > mg->threshold )  return CRASHED;
			return ret;
		}

		if(mg->rcv_pack.cls == 1 && mg->rcv_pack.id == 7){

			mg->latitude = 0, mg->longitud = 0;
			for(int i = 0 ; i < 4 ; i++)
				mg->latitude = (mg->latitude << 8) | (mg->rcv_pack.payload[31-i]);
			for(int i = 0 ; i < 4 ; i++)
				mg->longitud = (mg->longitud << 8) | (mg->rcv_pack.payload[27-i]);

			mg->north_vel = 0, mg->east_vel = 0;
			for(int i = 0 ; i < 4 ; i++)
				mg->north_vel = (mg->north_vel << 8) | (mg->rcv_pack.payload[51-i]);
			for(int i = 0 ; i < 4 ; i++)
				mg->east_vel = (mg->east_vel << 8) | (mg->rcv_pack.payload[55-i]);

			mg->last_tim = TIME;
			return OK;
		}
		else{
			if(  TIME - mg->last_tim > mg->threshold )  return CRASHED;
			return WRG_ID;
		}
	}
	if(  TIME - mg->last_tim > mg->threshold )  return CRASHED;
	return NO_DATA;
}
#endif
