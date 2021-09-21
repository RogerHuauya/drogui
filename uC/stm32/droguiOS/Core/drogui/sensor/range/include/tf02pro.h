#include "macros.h"
#ifdef TF02PRO

#ifndef TF02PRO_H
#define TF02PRO_H

#include "_main.h"
#include "serial.h"

#define TF_SYNCH1 0x59
#define TF_SYNCH2 0x59

/**
 * @brief Structure TeraRanger Packet
 * @param payload  Payload to be send (uint8_t)
 * @param chksumBuff Checksum buffer  (uint8_t)
 * @param chksum  Checksum of the payload (uint8_t)
 * @param ser Serial used to communication between uC and teraRanger (serial*)
 */
typedef struct _tfPacket{
	uint8_t payload[3], chksumBuff[3];
	uint8_t chksum;
	serial * ser;
} tfPacket;

/**
 * @brief Structure TeraRanger
 * @param rcv_pack Packet to be read (tRPacket)
 * @param distance Distance read by TeraRanger (uint16_t)
 */
typedef struct _tf02{
	tfPacket rcv_pack;
	uint16_t distance;
	uint16_t strength;
	uint8_t credibility;
	uint32_t threshold;
	uint32_t last_tim;
} tf02;

typedef tf02 rangeFinder;

/**
 * @brief Init TeraRanger
 * @param tera Structure TeraRanger (Pointer of structure tRanger )
 * @param ser Serial used for communication (serial*)
 */
void initRangeFinder(tf02 *tf, serial *ser);

/**
 * @brief Read distance from TeraRanger
 * @param tera (Pointer of structure tRanger )
 * @return Distance (SENSOR_STATUS)
 */
SENSOR_STATUS readRangeFinder(tf02 *tf);


#endif
#endif
