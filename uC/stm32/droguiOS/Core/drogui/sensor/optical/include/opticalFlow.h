#ifndef OPTICALFLOW_H
#define OPTICALFLOW_H


#include "_main.h"
#include "serial.h"
#include "utils.h"

typedef enum OPT_VAR {OPT_VEL, OPT_RNG} OPT_VAR;

#define RNG_FNDR 0x1F01
#define OPT_FLOW 0x1F02
#define OPT_SYNCH_1 '$'
#define OPT_SYNCH_2 'X'

/**
 * @brief Structure optical flow packet
 * @param type Define if we read optical flow or range finder (uint16_t)
 * @param sz Size of payload (uint16_t)
 * @param payload Payload to be sent (uint8_t)
 * @param chksumBuff Checksum buffer (uint8_t)
 * @param chksum Checksum of payload (uint8_t)
 * @param dir (uint8_t)
 * @param flag Defie status of sensor readind (uint8_t)
 * @param ser (Pointer of structure serial)
 */
typedef struct _optPacket{
	int16_t type, sz;
	uint8_t payload[100], chksumBuff[100];
	uint8_t chksum, dir, flag;
	serial * ser;
} optPacket;

/**
 * @brief Structure optical flow packet
 * @param rcv_pack Optical flow packet(optPacket)
 * @param q_vel (uint8_t)
 * @param q_rng (uint8_t)
 * @param vel_x Velocity in x direction (int)
 * @param vel_y Velocity in y direction (int)
 * @param off_x Offset in x direction (int)
 * @param off_y Offset in y direction (int)
 * @param dis Distance (int)
 */
typedef struct _optFlow{
	optPacket rcv_pack;
	uint8_t q_vel, q_rng;
	int vel_x, vel_y;
	int off_x, off_y;
	int dis;
	uint32_t last_tim, threshold;

} optFlow;

/**
 * @brief Get checksum of payload
 * @param op (Pointer of structure optPacket)
 */
void calcChksum(optPacket *op);

/**
 * @brief Initialize optical flow structure
 * @param of (Pointer of structure optFlow)
 * @param ser (Pointer of structure serial)
 */
void initOptFlow(optFlow *of, serial* ser);

/**
 * @brief Know if it is possible to get sensor data
 * @param op (Pointer of structure optPacket)
 * @param timeout (uint32_t)
 * @return Value from Sensor Status enum
 */
SENSOR_STATUS readOptFlow(optPacket *op, uint32_t timeout);

/**
 * @brief Get distance or range finder
 * @param of (Pointer of structure optFlow)
 * @param var Choose if you read distance or range finder(OPT_VAR)
 */
SENSOR_STATUS readFlowRange(optFlow *of, OPT_VAR *var);


#endif
