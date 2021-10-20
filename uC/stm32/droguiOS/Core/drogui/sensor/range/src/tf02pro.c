#include "macros.h"

#ifdef TF02PRO

#include "tf02pro.h"
#include "serial.h"
#include "task.h"

static char cfg_rate[8] = {0xAA, 0x55, 0xF0, 0x00, 0x64, 0x00, 0x00, 0xF2};
static char cfg_baudrate[8] = {0xAA, 0x55, 0xF0, 0x00, 0x08, 0x00, 0x00, 0x60};

void initRangeFinder(tf02 *tf, serial *ser){
	changeBaudrate(ser, 115200);

	for(int i = 0 ; i < 8 ; i++) serialWrite(ser, cfg_rate[i]);
	//for(int i = 0 ; i < 8 ; i++) serialWrite(ser, cfg_baudrate[i]);

	//changeBaudrate(ser, 256000);

	tf->distance = 0;
	tf->rcv_pack.ser = ser;
	tf->rcv_pack.chksum = 0;
	tf->last_tim = TIME;
	tf->threshold = 500000;
}


static void calcChecksum(tfPacket *tpacket){
	uint8_t chksum = 0;
	for(int i = 0; i < 8; i++) chksum += tpacket->chksumBuff[i];
	tpacket -> chksum = chksum;
}

SENSOR_STATUS readTfPacket(tfPacket *tpacket, uint32_t timeout){
	uint8_t sync1 = 0,sync2 = 0;
	uint8_t checksum;
	uint32_t tim = TIME;
	bool flag = false;


	while(TIME - tim < timeout){
		if(serialAvailable(tpacket->ser)) sync1 = sync2, sync2 = serialRead(tpacket->ser);
		if(sync1 == TF_SYNCH1 && sync2 == TF_SYNCH2){
			tpacket->chksumBuff[0] = TF_SYNCH1;
			tpacket->chksumBuff[1] = TF_SYNCH1;

			for(int i = 0; i < 6 ; i++){
				while(!serialAvailable(tpacket->ser)){
					if( TIME - tim > timeout ) return TIMEOUT;
				}
				tpacket->chksumBuff[i+2] = tpacket->payload[i] = serialRead(tpacket->ser);

			}

			while(!serialAvailable(tpacket->ser)){
				if( TIME - tim > timeout ) return TIMEOUT;
			}

			checksum = serialRead(tpacket->ser);

			calcChecksum(tpacket);
			flag = 1;
		}

		if(flag == 1) break;

	}

	if(flag == 0) return TIMEOUT;
	if(tpacket->chksum == checksum) return OK;
	else return WRG_CHKSUM;
}

SENSOR_STATUS readRangeFinder(tf02 *tf){
	while(serialAvailable(tf->rcv_pack.ser)){
		int ret = readTfPacket(&(tf->rcv_pack), 100000);
		serialFlush(tf->rcv_pack.ser);

		if( ret != OK){
			if(TIME - tf->last_tim > tf->threshold) return CRASHED;
			return ret;
		}

		tf->distance = ((uint16_t) tf->rcv_pack.payload[1] << 8) | tf->rcv_pack.payload[0];
		tf->distance *= 10.0;
		tf->strength = ((uint16_t) tf->rcv_pack.payload[3] << 8) | tf->rcv_pack.payload[2];
		tf->credibility = (uint8_t) tf->rcv_pack.payload[4];

		tf->last_tim = TIME;
		return OK;
	}
	if(TIME - tf->last_tim > tf->threshold) return CRASHED;
	return NO_DATA;
}
#endif
