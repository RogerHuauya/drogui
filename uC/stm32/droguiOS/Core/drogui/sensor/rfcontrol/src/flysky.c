#include "macros.h"

#ifdef FLYSKY

#include "flysky.h"
#include "serial.h"
#include "task.h"


void initFsReceiver(fsReceiver *fsRec, serial *ser){
	changeBaudrate(ser, 115200);

	for(int i = 0; i < 6; i++) fsRec->channel_val[i] = 0;
	fsRec->rcv_pack.ser = ser;
	fsRec->rcv_pack.chksum = 0;
	fsRec->last_tim = TIME;
	fsRec->threshold = 500000;
}

static void calcChecksum(fsPacket *fspacket){
	uint16_t chksum = 0xFFFF;
	for(int i = 0; i < IBUS_BUFFSIZE-2; i++) chksum -= fspacket->chksumBuff[i];
	fspacket -> chksum = chksum;
}

SENSOR_STATUS readFsPacket(fsPacket *fspacket, uint32_t timeout){
	uint8_t sync1 = 0,sync2 = 0;
	uint16_t checksum = 0x00;
	uint32_t tim = TIME;
	bool flag = false;


	while(TIME - tim < timeout){
		if(serialAvailable(fspacket->ser)) sync1 = sync2, sync2 = serialRead(fspacket->ser); 
		if(sync1 == FS_SYNCH1 && sync2 == FS_SYNCH2){
			fspacket->chksumBuff[0] = FS_SYNCH1;
			fspacket->chksumBuff[1] = FS_SYNCH2;

			for(int i = 0; i < IBUS_BUFFSIZE-4 ; i++){
				while(!serialAvailable(fspacket->ser)){
					if( TIME - tim > timeout ) return TIMEOUT;
				}
				fspacket->chksumBuff[i+2] = fspacket->payload[i] = serialRead(fspacket->ser);

			}

			while(!serialAvailable(fspacket->ser)){
				if( TIME - tim > timeout ) return TIMEOUT;
			}

			checksum = serialRead(fspacket->ser);
			checksum = checksum  + (((uint16_t)serialRead(fspacket->ser)) << 8);
			calcChecksum(fspacket);

			flag = 1;
		}

		if(flag == 1) break;

	}

	if(flag == 0) return TIMEOUT;
	if(fspacket->chksum == checksum) return OK;
	else return WRG_CHKSUM;
}

SENSOR_STATUS readFsReceiver(fsReceiver *fsRec){
	while(serialAvailable(fsRec->rcv_pack.ser)){
		int ret = readFsPacket(&(fsRec->rcv_pack), 1000000);
		serialFlush(fsRec->rcv_pack.ser);

		if( ret != OK){
			if(TIME - fsRec->last_tim > fsRec->threshold) return CRASHED;
			return ret;
		}

		for(int i = 0; i < CHANNEL_NUM; i++){ 
			fsRec->raw_channel_val[i] = (fsRec->rcv_pack.payload[2*i+1] << 8) + fsRec->rcv_pack.payload[2*i];
			fsRec->channel_val[i] = fsRec->raw_channel_val[i] - fsRec->channel_offset[i];
			if( i == 2) fsRec->channel_val[i] /= 500.0;
			else if( i < 4 )fsRec->channel_val[i] /= 1000.0;
		}

		fsRec->last_tim = TIME;
		return OK;
	}
	if(TIME - fsRec->last_tim > fsRec->threshold) return CRASHED;
	return NO_DATA;
}

void calibrateFsReceiver(fsReceiver *fsRec){
	
	readFsReceiver(fsRec);
	for(int i = 0; i < CHANNEL_NUM-2; i++){
		fsRec->channel_offset[i] = fsRec->raw_channel_val[i];
		HAL_Delay(1);
	}
}

#endif
