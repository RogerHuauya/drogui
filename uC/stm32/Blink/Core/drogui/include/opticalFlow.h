#ifndef OPTICALFLOW_H
#define OPTICALFLOW_H


#include "_main.h"

enum OPTFLOW_STATUS {OPT_NO_DATA, OPT_TIMEOUT, OPT_WRG_ID, OPT_VEL, OPT_RNG, OPT_OK, OPT_WRG_CHKSUM};


#define RNG_FNDR 0x1F01
#define OPT_FLOW 0x1F02
#define OPT_SYNCH_1 '$'
#define OPT_SYNCH_2 'X'

typedef struct _optPacket{
    int16_t type, sz;
	uint8_t payload[100], chksumBuff[100];
    uint8_t chksum, dir, flag; 
} optPacket;


typedef struct _optFlow{
    optPacket rcv_pack;
    uint8_t q_vel, q_rng;
    int vel_x, vel_y;
	int off_x, off_y;
    int dis;
} optFlow;

void calcChksum(optPacket *op);
void initOptFlow(optFlow *of);
int readOptFlow(optPacket *op, uint32_t timeout);
int readFlowRange(optFlow *of);


#endif