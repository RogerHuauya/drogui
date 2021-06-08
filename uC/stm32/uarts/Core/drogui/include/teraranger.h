#ifndef TERARANGER_H
#define TERARANGER_H
#include "_main.h"
#include "serial.h"
#include <stdint.h>


#define TR_SYNCH 'T'
typedef struct _tRPacket{
	uint8_t payload[3], chksumBuff[3];
    uint8_t chksum; 
    serial * ser;
} tRPacket;

typedef struct _tRanger{
    tRPacket rcv_pack;
    uint16_t distance;
} tRanger;

void initTeraRanger(tRanger *tera, serial *ser);
SENSOR_STATUS readTRanger(tRPacket *tpacket, uint32_t timeout);
SENSOR_STATUS readTeraRange(tRanger *tera);

#endif