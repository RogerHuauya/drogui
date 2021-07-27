#include "macros.h"

#ifdef ZED_F9P

#ifndef ZED_F9P_H
#define ZED_F9P_H

#include "_main.h"
#include "serial.h"
#include <stdbool.h>
#include "utils.h"

//Registers
#define UBX_SYNCH_1  0xB5
#define UBX_SYNCH_2  0x62

//The following are UBX Class IDs. Descriptions taken from ZED-F9P Interface Description Document page 32, NEO-M8P Interface Description page 145
#define UBX_CLASS_NAV  0x01	 //Navigation Results Messages: Position, Speed, Time, Acceleration, Heading, DOP, SVs used
#define UBX_CLASS_ACK  0x05	 //Ack/Nak Messages: Acknowledge or Reject messages to UBX-CFG input messages
#define UBX_CLASS_CFG  0x06	 //Configuration Input Messages: Configure the receiver.

//Class: CFG
//The following are used for configuration. Descriptions are from the ZED-F9P Interface Description pg 33-34 and NEO-M9N Interface Description pg 47-48
#define UBX_CFG_MSG  0x01		//Poll a message configuration, or Set Message Rate(s), or Set Message Rate
#define UBX_CFG_PRT  0x00		//Used to configure port specifics. Polls the configuration for one I/O Port, or Port configuration for UART ports, or Port configuration for USB port, or Port configuration for SPI port, or Port configuration for DDC port
#define UBX_CFG_RATE  0x08		//Navigation/Measurement Rate Settings. Used to set port baud rates.

//Class: NAV
//The following are used to configure the NAV UBX messages (navigation results messages). Descriptions from UBX messages overview (ZED_F9P Interface Description Document page 35-36)
#define UBX_NAV_GEOFENCE  0x39	//Geofencing status. Used to poll the geofence status
#define UBX_NAV_HPPOSECEF  0x13 //High Precision Position Solution in ECEF. Used to find our positional accuracy (high precision).
#define UBX_NAV_HPPOSLLH  0x14	//High Precision Geodetic Position Solution. Used for obtaining lat/long/alt in high precision
#define UBX_NAV_ODO  0x09		//Odometer Solution
#define UBX_NAV_POSECEF  0x01	//Position Solution in ECEF
#define UBX_NAV_POSLLH  0x02	//Geodetic Position Solution
#define UBX_NAV_PVT  0x07		//All the things! Position, velocity, time, PDOP, height, h/v accuracies, number of satellites. Navigation Position Velocity Time Solution.
#define UBX_NAV_SAT  0x35		//Satellite Information

// Class: ACK
#define UBX_ACK_NACK  0x00
#define UBX_ACK_ACK  0x01

//The following consts are used to configure the various ports and streams for those ports. See -CFG-PRT.
#define COM_PORT_I2C  0
#define COM_PORT_UART1  1
#define COM_PORT_UART2  2
#define COM_PORT_USB  3
#define COM_PORT_SPI  4

#define COM_TYPE_UBX  (1 << 0)
#define COM_TYPE_NMEA  (1 << 1)
#define COM_TYPE_RTCM3  (1 << 5)

/**
 * @brief Structure ubxPacket
 * @param cls Class (uint8_t)
 * @param id  ID (uint8_t)
 * @param len Length of packet (uint16_t)
 * @param counter (uint16_t)
 * @param payload Payload to be sent (uint8_t)
 * @param checksumA Lower Checksum value  (uint8_t)
 * @param checksumB Checksum value (uint8_t)
 * @param ser (Pointer of structure serial)
 */
typedef struct _ubxPacket{

	uint8_t cls, id;
	uint16_t len, counter;
	uint8_t payload[500];
	uint8_t checksumA, checksumB;
	serial* ser;
} ubxPacket;

/**
 * @brief Structure M8Q
 * @param snd_pack Packet to be sent (ubxPacket)
 * @param rcv_pack Packet to be received (ubxPacket)
 * @param latitude Latitude value (int)
 * @param longitud Longitud value (int)
 * @param off_x Offset in x direction (int)
 * @param off_y Offset in y direction (int)
 * @param cnt Auxiliar counter (int)
 */
typedef struct _zed{
	ubxPacket snd_pack, rcv_pack;
	int latitude, longitud;
	int north_vel, east_vel;
	int off_x, off_y;
	uint32_t threshold;
	uint32_t last_tim;
	int cnt;
} zed;

typedef zed gps;

/**
 * @brief Read Latitude and Longitde
 * @param mg (Pointer of structure zed)
 * @return OK,WRG_ID OR NO_DATA from SENSOR_STATUS
 */
SENSOR_STATUS readGPS(zed* mg);

/**
 * @brief Initialize SAM M8Q
 * @param mg (Pointer of structure zed)
 * @param ser (Pointer of structure serial)
 * @return (SENSOR_STATUS)
 */
SENSOR_STATUS initGPS(zed *mg, serial* ser);

/**
 * @brief Read data from SAM M8Q
 * @param mp (Pointer of structure ubxPacket)
 * @param timeout  (uint32_t)
 * @return It depends if the checksum was received or the time limit passed (SENSOR_STATUS)
 */
SENSOR_STATUS readPacket(ubxPacket *mp, uint32_t timeout);

#endif

#endif
