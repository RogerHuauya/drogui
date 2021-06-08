#ifndef BMP388_H
#define BMP388_H
#include <stdint.h>
#include "filter.h" 
#define BMP388_DEFAULT_ADDRESS (0x77) 

/**\name Register Address */
#define BMP388_REG_CHIP_ID                        UINT8_C(0x00)
#define BMP388_REG_ERR                            UINT8_C(0x02)
#define BMP388_REG_SENS_STATUS                    UINT8_C(0x03)
#define BMP388_REG_DATA_PRESS                     UINT8_C(0x04)
#define BMP388_REG_DATA_TEMP                      UINT8_C(0x07)
#define BMP388_REG_EVENT                          UINT8_C(0x10)
#define BMP388_REG_INT_STATUS                     UINT8_C(0x11)
#define BMP388_REG_FIFO_LENGTH                    UINT8_C(0x12)
#define BMP388_REG_FIFO_DATA                      UINT8_C(0x14)
#define BMP388_REG_FIFO_WM                        UINT8_C(0x15)
#define BMP388_REG_FIFO_CONFIG_1                  UINT8_C(0x17)
#define BMP388_REG_FIFO_CONFIG_2                  UINT8_C(0x18)
#define BMP388_REG_INT_CTRL                       UINT8_C(0x19)
#define BMP388_REG_IF_CONF                        UINT8_C(0x1A)
#define BMP388_REG_PWR_CTRL                       UINT8_C(0x1B)
#define BMP388_REG_OSR                            UINT8_C(0X1C)
#define BMP388_REG_ODR                            UINT8_C(0x1D)
#define BMP388_REG_CONFIG                         UINT8_C(0x1F)
#define BMP388_REG_CALIB_DATA                UINT8_C(0x31)
#define BMP388_REG_CMD                            UINT8_C(0x7E)


typedef struct _bmp388{
    int temp_cont;
    int cont;
    float alt_offset, altitude,raw_altitude;
    int64_t temp, press, seaLevel;
    mvAvgFilter f;
} bmp388;



void initBmp388(bmp388* b, int ntemp);
void bmpReadTemperature(bmp388* b);
void bmpReadPressure(bmp388* b);

void bmp388ReadAltitude(bmp388* b);
void updateBmp388Offset(bmp388 * b);

#endif