#include "macros.h"

#ifdef TERARANGER

#ifndef TERARANGER_H
#define TERARANGER_H

#include "_main.h"
#include "serial.h"
#include <stdint.h>


#define TR_SYNCH 'T'

/**
 * @brief Structure TeraRanger Packet
 * @param payload  Payload to be send (uint8_t)
 * @param chksumBuff Checksum buffer  (uint8_t)
 * @param chksum  Checksum of the payload (uint8_t)
 * @param ser Serial used to communication between uC and teraRanger (serial*)
 */
typedef struct _tRPacket{
	uint8_t payload[3], chksumBuff[3];
	uint8_t chksum;
	serial * ser;
} tRPacket;

/**
 * @brief Structure TeraRanger
 * @param rcv_pack Packet to be read (tRPacket)
 * @param distance Distance read by TeraRanger (uint16_t)
 */
typedef struct _tRanger{
	tRPacket rcv_pack;
	uint16_t distance;
	uint32_t threshold;
	uint32_t last_tim;
} tRanger;

typedef tRanger rangeFinder;

/**
 * @brief Init TeraRanger
 * @param tera Structure TeraRanger (Pointer of structure tRanger )
 * @param ser Serial used for communication (serial*)
 */
void initRangeFinder(tRanger *tera, serial *ser);

/**
 * @brief Read distance from TeraRanger
 * @param tera (Pointer of structure tRanger )
 * @return Distance (SENSOR_STATUS)
 */
SENSOR_STATUS readRangeFinder(tRanger *tera);

#endif
#endif
