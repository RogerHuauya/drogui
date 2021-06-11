#ifndef SERIAL_H
#define SERIAL_H

#include "utils.h"

#define SER_BUFF_SZ 300

/**
 * @brief Structure Serial
 * @param rcv_buff Buffer used to received data through serial communication (uint8_t)
 * @param rcv_head Head of buffer recieved (uint8_t)
 * @param rcv_tail Tail of buffer received (uint8_t)
 * @param snd_buff Buffer used to send data through serial communication (uint8_t)
 * @param snd_head Head of buffer sent (uint8_t)
 * @param snd_tail Tail of buffer sent (uint8_t)
 * @param cr (uint32_t*)
 * @param brr (uint32_t*)
*/
typedef struct _serial{
    uint8_t rcv_buff[SER_BUFF_SZ], rcv_head, rcv_tail;
    uint8_t snd_buff[SER_BUFF_SZ], snd_head, snd_tail;
    volatile uint32_t* cr, *brr;
} serial;


extern serial serial2, serial3, serial4, serial5;

/**
 * @brief Initialize all serials 
*/
void serialsBegin();

/**
 * @brief Initialize serial structure
 * @param ser (Pointer of structure serial)
 * @param cr (uint32_t*)
 * @param brr (uint32_t*)
*/
void serialBegin(serial* ser, volatile uint32_t * cr, volatile uint32_t * brr);

/**
 * @brief Print data, which only has characters, in a specific serial
 * @param ser (Pointer of structure serial)
 * @param s Array of characters(char*)
*/
void serialPrint(serial* ser, char *s);

/**
 * @brief Print data, which has characters and numeric values, in a specific serial
 * @param ser (Pointer of structure serial)
 * @param s Array of characters and numeric values (const char*)
*/
void serialPrintf(serial* ser, const char *s, ...);

/**
 * @brief Write character in serial 
 * @param ser (Pointer of structure serial)
 * @param c Character to be written (char)
*/
void serialWrite(serial* ser, char c);

/**
 * @brief Know if serial port is available
 * @param ser (Pointer of structure serial)
 * return Boolean value
*/
bool serialAvailable(serial* ser);

/**
 * @brief Read character in serial 
 * @param ser (Pointer of structure serial)
*/
char serialRead(serial* ser);

/**
 * @brief Change baurate of serial 
 * @param ser (Pointer of structure serial)
 * @param baudrate (int) 
*/
void changeBaudrate(serial* ser, int baudrate);

/**
 * @brief Reset serial 
 * @param ser (Pointer of structure serial)
*/
void serialFlush(serial* ser);

#endif