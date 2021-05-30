#include "opticalFlow.h"
#include "serial.h"
#include "task.h"

uint8_t crc8_dvb_s2(uint8_t crc, uint8_t a){
    crc ^= a;
    for (int ii = 0; ii < 8; ++ii) {
        if (crc & 0x80) {
            crc = (crc << 1) ^ 0xD5;
        } else {
            crc = crc << 1;
        }
    }
    return crc; 
}

uint8_t crc8_func(int len, uint8_t* msg){
    uint8_t crc8 = 0;
    for(int i = 0; i < len; i++){
        crc8 = crc8_dvb_s2(crc8, msg[i]);
    }
    return crc8;
}


void calcChksum(optPacket *msg){
    msg -> chksum = crc8_func((msg -> sz)+5, msg -> chksumBuff);
}



void initOptFlow(optFlow *of, serial* ser){
	changeBaudrate(ser, 115200);
    of->off_x = 0;
    of->off_y = 0;
    of->vel_x = 0;
    of->vel_y = 0;
    of->dis   = 0;
	of->rcv_pack.ser = ser;
}




int readOptFlow(optPacket *op, uint32_t timeout){
	uint8_t sync1 = 0, sync2 = 0;
    uint8_t cntLSB, cntMSB;
    uint8_t idMSB, idLSB;
	uint8_t checksum, flag = 0;
	uint32_t tim = TIME;

	while(TIME - tim < timeout){
		if(serialAvailable(op->ser)){
			sync1 = sync2;
			sync2 = serialRead(op->ser);
		}

		if(sync1 == OPT_SYNCH_1 && sync2 == OPT_SYNCH_2){
			
            while(!serialAvailable(op->ser)){};
			op->dir = serialRead(op->ser);

            while(!serialAvailable(op->ser)){};
			op->chksumBuff[0] = op->flag = serialRead(op->ser);

			while(!serialAvailable(op->ser)){};
			op->chksumBuff[1] = idLSB = serialRead(op->ser);
			while(!serialAvailable(op->ser)){};
			op->chksumBuff[2] = idMSB = serialRead(op->ser);

            op->type = (uint16_t) (idMSB << 8) | (idLSB & 0xFF);

			while(!serialAvailable(op->ser));
			op->chksumBuff[3] = cntLSB = serialRead(op->ser);
			while(!serialAvailable(op->ser));
			op->chksumBuff[4] = cntMSB = serialRead(op->ser);

			op->sz = (uint16_t) (cntMSB << 8) | (cntLSB & 0xFF);
            

			for(int i = 0; i < op->sz ; i++){
				while(!serialAvailable(op->ser));
				op->chksumBuff[5+i] = op->payload[i] = serialRead(op->ser);
			}

			while(!serialAvailable(op->ser));
			checksum = serialRead(op->ser);

			calcChksum(op);
			flag = 1;
		}

		if(flag == 1) break;

	}

	if(flag == 0) return TIMEOUT;
	if(op->chksum == checksum) return OK;
	else return WRG_CHKSUM;
}

/*
void printPacket(ubxPacket *mp){
    serialPrintf("Class:\t %d \tID:\t %d \tlen:\t %d \n", mp->cls, mp->id, mp->len); 

	for( int i = 0; i < mp->len ; i++) 
		serialPrintf("%x ", mp->payload[i]);
	serialPrint("\n");
}*/


int readFlowRange(optFlow *of){
	if(serialAvailable(of->rcv_pack.ser)){ 
		int ret = readOptFlow(&(of->rcv_pack), 1000); 
		serialFlush(of->rcv_pack.ser);
		if( ret != OK) return ret;
		

		if(of->rcv_pack.type == OPT_FLOW){
		
			of -> q_vel = (of->rcv_pack).payload[0];
            of->vel_x = 0, of->vel_y = 0;
            for(int i = 3 ; i >= 0 ; i--)
                of->vel_x = (of->vel_x << 8) | (of->rcv_pack).payload[1+i];
              
            for(int i = 3 ; i >= 0 ; i--)
                of->vel_y = (of->vel_y << 8) | (of->rcv_pack).payload[5+i];
			
            return OPT_VEL;
		}
		else if(of->rcv_pack.type == RNG_FNDR){
            of->q_rng = (of->rcv_pack).payload[0];
            of->dis = 0;
            for(int i = 3 ; i >= 0 ; i--)
                of->dis = (of->dis << 8) | (of->rcv_pack).payload[1+i];
            
            return OPT_RNG;
		}
        else{
            return WRG_ID;
        }
	}
	return NO_DATA;
}