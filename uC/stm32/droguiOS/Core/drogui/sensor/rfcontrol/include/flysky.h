#include "macros.h"

#ifdef FLYSKY

#ifndef FLYSKY_H
#define FLYSKY_H

#include "_main.h"
#include "serial.h"
#include <stdint.h>


#define FS_SYNCH1 0x20
#define FS_SYNCH2 0x40

#define CHANNEL_NUM 6
#define IBUS_BUFFSIZE 32

/**
 * @brief Structure RadioControl Packet
 * @param payload  Payload to be send (uint8_t)
 * @param chksumBuff Checksum buffer  (uint8_t)
 * @param chksum  Checksum of the payload (uint8_t)
 * @param ser Serial used to communication between uC and receiver Flysky (serial*)
 */
typedef struct _fsPacket{
	uint8_t payload[IBUS_BUFFSIZE], chksumBuff[IBUS_BUFFSIZE];
	uint16_t chksum;
	serial * ser;
} fsPacket;

/**
 * @brief Structure RadioControl
 * @param rcv_pack Packet to be read (tRPacket)
 * @param distance Distance read by RadioControl (uint16_t)
 */
typedef struct _fsReceiver{
	fsPacket rcv_pack;
	int16_t channel_val[CHANNEL_NUM];
	uint32_t threshold;
	uint32_t last_tim;
} fsReceiver;

typedef fsReceiver rfControl;

/**
 * @brief Init RadioControl
 * @param tera Structure RadioControl (Pointer of structure tRanger )
 * @param ser Serial used for communication (serial*)
 */
void initFsReceiver(fsReceiver *fsRec, serial *ser);

/**
 * @brief Read distance from RadioControl
 * @param tera (Pointer of structure tRanger )
 * @return Distance (SENSOR_STATUS)
 */
SENSOR_STATUS readFsReceiver(fsReceiver *fsRec);

#endif
#endif
