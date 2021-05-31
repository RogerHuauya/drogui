#include "M8Q.h"
#include "serial.h"
#include "task.h"

// https://www.u-blox.com/en/ubx-viewer/view/u-blox8-M8_ReceiverDescrProtSpec_UBX-13003221?url=https%3A%2F%2Fwww.u-blox.com%2Fsites%2Fdefault%2Ffiles%2Fproducts%2Fdocuments%2Fu-blox8-M8_ReceiverDescrProtSpec_UBX-13003221.pdf#%5B%7B%22num%22%3A775%2C%22gen%22%3A0%7D%2C%7B%22name%22%3A%22XYZ%22%7D%2C0%2C748.35%2Cnull%5D
uint8_t defaultCfgPort[20] = { 1, 0, 0, 0, 0xC0, 8, 0, 0, 0x00, 0x08, 0x07, 0, 7, 0, 1, 0, 0, 0, 0, 0};

// https://www.u-blox.com/en/ubx-viewer/view/u-blox8-M8_ReceiverDescrProtSpec_UBX-13003221?url=https%3A%2F%2Fwww.u-blox.com%2Fsites%2Fdefault%2Ffiles%2Fproducts%2Fdocuments%2Fu-blox8-M8_ReceiverDescrProtSpec_UBX-13003221.pdf#%5B%7B%22num%22%3A808%2C%22gen%22%3A0%7D%2C%7B%22name%22%3A%22XYZ%22%7D%2C0%2C612.28%2Cnull%5D
//uint8_t defaultCfgRate[6]  = { 0xF4, 1, 1, 0, 1, 0 };
uint8_t defaultCfgRate[6]  = { 0x7D, 0, 1, 0, 1, 0 }; // 8hz

//https://www.u-blox.com/en/ubx-viewer/view/u-blox8-M8_ReceiverDescrProtSpec_UBX-13003221?url=https%3A%2F%2Fwww.u-blox.com%2Fsites%2Fdefault%2Ffiles%2Fproducts%2Fdocuments%2Fu-blox8-M8_ReceiverDescrProtSpec_UBX-13003221.pdf#%5B%7B%22num%22%3A691%2C%22gen%22%3A0%7D%2C%7B%22name%22%3A%22XYZ%22%7D%2C0%2C654.8%2Cnull%5D
uint8_t defaultCfgMsg[3]   = {UBX_CLASS_NAV, UBX_NAV_POSLLH, 1};

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


SENSOR_STATUS initM8Q(m8q *mg, serial* ser){
	int ret;
	
	mg->rcv_pack.ser = ser; 
	mg->snd_pack.ser = ser;

    changeBaudrate(mg->rcv_pack.ser,9600);

	HAL_Delay(1000);
    cfgM8QPort(&(mg->snd_pack), defaultCfgPort);
    
    HAL_Delay(100);
    changeBaudrate(mg->rcv_pack.ser,460800);
    HAL_Delay(100);

    cfgM8QRate(&(mg->snd_pack), defaultCfgRate);
	
    ret = readM8Q(&(mg->rcv_pack), 10000);
	if(ret != OK) return ret; 

    cfgM8QMsg(&(mg->snd_pack), defaultCfgMsg);

    ret = readM8Q(&(mg->rcv_pack), 10000);
	if(ret != OK) return ret;

    HAL_Delay(100);

	mg->cnt = 0;
	return OK;
}


void cfgM8Q(ubxPacket *mp, uint8_t id,uint8_t len, uint8_t *cfgArray){
	mp->cls = UBX_CLASS_CFG;
	mp->id  = id;
	mp->len = len;
	for( int i = 0; i < len; i++ ) mp->payload[i] = cfgArray[i];
	calcChecksum(mp);
	sendSerialCommand(mp);
}


void cfgM8QPort(ubxPacket *mp, uint8_t *cfgPortArray){
	mp->cls = UBX_CLASS_CFG;
	mp->id  = UBX_CFG_PRT;
	mp->len = 20;
	for( int i = 0; i < 20; i++ ) mp->payload[i] = cfgPortArray[i];
	calcChecksum(mp);
	sendSerialCommand(mp);
}

void cfgM8QMsg(ubxPacket *mp, uint8_t *cfgMsgArray){

	mp->cls = UBX_CLASS_CFG;
	mp->id = UBX_CFG_MSG;
	mp->len = 3;
	for( int i = 0; i < 3; i++ ) mp->payload[i] = cfgMsgArray[i];

	calcChecksum(mp);
	sendSerialCommand(mp);
}

void cfgM8QRate(ubxPacket *mp, uint8_t *cfgRateArray){

	mp->cls = UBX_CLASS_CFG;
	mp->id = UBX_CFG_RATE;
	mp->len = 6;
	for( int i = 0; i < 6; i++ ) mp->payload[i] = cfgRateArray[i];

	calcChecksum(mp);
	sendSerialCommand(mp);
}



SENSOR_STATUS readM8Q(ubxPacket *mp, uint32_t timeout){
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


void printPacket(ubxPacket *mp){
    serialPrintf(mp->ser,"Class:\t %d \tID:\t %d \tlen:\t %d \n", mp->cls, mp->id, mp->len); 

	for( int i = 0; i < mp->len ; i++) 
		serialPrintf(mp->ser,"%x ", mp->payload[i]);
	serialPrint(mp->ser,"\n");
}


SENSOR_STATUS readLatLon(m8q *mg){
	if(serialAvailable(mg->rcv_pack.ser)){ 
		int ret = readM8Q(&(mg->rcv_pack), 1000); 
		//serialFlush();
		if( ret != OK) return ret;
		

		if(mg->rcv_pack.cls == 1 && mg->rcv_pack.id == 2){
		
			mg->latitude = 0, mg->longitud = 0;
			for(int i = 0 ; i < 4 ; i++) 
				mg->latitude = (mg->latitude << 8) | (mg->rcv_pack.payload[7-i]);
			for(int i = 0 ; i < 4 ; i++) 
				mg->longitud = (mg->longitud << 8) | (mg->rcv_pack.payload[11-i]);
			
			return OK;
		}
		else{
			return WRG_ID;
		}
	}
	return NO_DATA;
}
