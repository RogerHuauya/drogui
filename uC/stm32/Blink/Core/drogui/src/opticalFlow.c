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

/*
void sendSerialCommand(ubxPacket *outgoingUBX)
{
	//Write header bytes
	serialWrite(UBX_SYNCH_1); //μ - oh ublox, you're funny. I will call you micro-blox from now on.
	serialWrite(UBX_SYNCH_2); //b
	serialWrite(outgoingUBX->cls);
	serialWrite(outgoingUBX->id);
	serialWrite(outgoingUBX->len & 0xFF); //LSB
	serialWrite(outgoingUBX->len >> 8);   //MSB

	//Write payload.
	for (int i = 0; i < outgoingUBX->len; i++)
	{
		serialWrite(outgoingUBX->payload[i]);
	}

	//Write checksum
	serialWrite(outgoingUBX->checksumA);
	serialWrite(outgoingUBX->checksumB);
}*/



void initOptFlow(optFlow *of){
    of->off_x = 0;
    of->off_y = 0;
    of->vel_x = 0;
    of->vel_y = 0;
    of->dis   = 0;
}




int readOptFlow(optPacket *op, uint32_t timeout){
	uint8_t sync1 = 0, sync2 = 0;
    uint8_t cntLSB, cntMSB;
    uint8_t idMSB, idLSB;
	uint8_t checksum, flag = 0;
	uint32_t tim = TIME;

	while(TIME - tim < timeout){
		if(serialAvailable()){
			sync1 = sync2;
			sync2 = serialRead();
		}

		if(sync1 == OPT_SYNCH_1 && sync2 == OPT_SYNCH_2){
			
            while(!serialAvailable()){};
			op->dir = serialRead();

            while(!serialAvailable()){};
			op->chksumBuff[0] = op->flag = serialRead();

			while(!serialAvailable()){};
			op->chksumBuff[1] = idLSB = serialRead();
			while(!serialAvailable()){};
			op->chksumBuff[2] = idMSB = serialRead();

            op->type = (uint16_t) (idMSB << 8) | (idLSB & 0xFF);

			while(!serialAvailable());
			op->chksumBuff[3] = cntLSB = serialRead();
			while(!serialAvailable());
			op->chksumBuff[4] = cntMSB = serialRead();

			op->sz = (uint16_t) (cntMSB << 8) | (cntLSB & 0xFF);
            

			for(int i = 0; i < op->sz ; i++){
				while(!serialAvailable());
				op->chksumBuff[5+i] = op->payload[i] = serialRead();
			}

			while(!serialAvailable());
			checksum = serialRead();

			calcChksum(op);
			flag = 1;
		}

		if(flag == 1) break;

	}

	if(flag == 0) return OPT_TIMEOUT;
	if(op->chksum == checksum) return OPT_OK;
	else return OPT_WRG_CHKSUM;
}

/*
void printPacket(ubxPacket *mp){
    serialPrintf("Class:\t %d \tID:\t %d \tlen:\t %d \n", mp->cls, mp->id, mp->len); 

	for( int i = 0; i < mp->len ; i++) 
		serialPrintf("%x ", mp->payload[i]);
	serialPrint("\n");
}*/


int readFlowRange(optFlow *of){
	if(serialAvailable()){ 
		int ret = readOptFlow(&(of->rcv_pack), 1000); 
		serialFlush();
		if( ret != OPT_OK) return ret;
		

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
            return OPT_WRG_ID;
        }
	}
	return OPT_NO_DATA;
}