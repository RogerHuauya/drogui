#include "BMP388.h"
#include "i2c.h"
#include "matlib.h"
#include "registerMap.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>
#include "usart.h"
char buffs[100] = "";

typedef struct _calib{
	uint16_t T1, T2;
	int8_t T3;

	int16_t P1, P2, P9;
	int8_t P3, P4, P7, P8, P10, P11;
	uint16_t P5, P6;

	int64_t T_fine;

} calib;


void buildCalib(uint8_t* buff, calib * cal){
	cal -> T1 = (uint16_t) (buff[1] << 8 | buff[0]);
	cal -> T2 = (uint16_t) (buff[3] << 8 | buff[2]);
	cal -> T3 = (int8_t) (buff[4]);

	cal -> P1 =  (int16_t) (buff[6]<< 8 | buff[5]);
	cal -> P2 =  (int16_t) (buff[8] << 8 | buff[7]);
	cal -> P3 =  (int8_t) (buff[9]);
	cal -> P4 =  (int8_t) (buff[10]);
	cal -> P5 =  (uint16_t) (buff[12] << 8 | buff[11]);
	cal -> P6 =  (uint16_t) (buff[14] << 8 | buff[13]);
	cal -> P7 =  (int8_t) (buff[15]);
	cal -> P8 =  (int8_t) (buff[16]);
	cal -> P9 =  (int16_t) (buff[18] << 8 | buff[17]);
	cal -> P10 = (int8_t) (buff[19]);
	cal -> P11 = (int8_t) (buff[20]);
}

calib calib_bmp;

void initBmp388(bmp388* b, int ntemp){

	uint8_t buff[21];
	I2CwriteByte(BMP388_DEFAULT_ADDRESS, BMP388_REG_PWR_CTRL, 0x33);
	HAL_Delay(10);
	I2CwriteByte(BMP388_DEFAULT_ADDRESS, BMP388_REG_OSR, 0x00);
	HAL_Delay(10);
	I2CwriteByte(BMP388_DEFAULT_ADDRESS, BMP388_REG_ODR, 0x00);
	HAL_Delay(10);
	I2Cread(BMP388_DEFAULT_ADDRESS, BMP388_REG_CALIB_DATA, 21, buff);
	buildCalib(buff, &calib_bmp);
	initMvAvgFilter(&(b->f), 25);
	b -> temp = b -> press = b -> altitude = b -> cont = 0;
	b -> alt_offset = b -> seaLevel = b -> raw_altitude =  0;
	b -> temp_cont = ntemp;
}

int64_t bmp388CompensateTemp( uint32_t u32RegData){

	uint64_t partial_data1;
	uint64_t partial_data2;
	uint64_t partial_data3;
	int64_t partial_data4;
	int64_t partial_data5;
	int64_t partial_data6;
	int64_t comp_temp;

	partial_data1 = (uint64_t)(u32RegData - (256 * (uint64_t)(calib_bmp.T1)));
	partial_data2 = (uint64_t)(calib_bmp.T2 * partial_data1);
	partial_data3 = (uint64_t)(partial_data1 * partial_data1);
	partial_data4 = (int64_t)(((int64_t)partial_data3) * ((int64_t)calib_bmp.T3));
	partial_data5 = ((int64_t)(((int64_t)partial_data2) * 262144) + (int64_t)partial_data4);
	partial_data6 = (int64_t)(((int64_t)partial_data5) / 4294967296);

	calib_bmp.T_fine = partial_data6;
	comp_temp = (int64_t)((partial_data6 * 25)  / 16384);
	return comp_temp;
}

int64_t bmp388CompensatePress(uint32_t u32RegData){

	int64_t partial_data1;
	int64_t partial_data2;
	int64_t partial_data3;
	int64_t partial_data4;
	int64_t partial_data5;
	int64_t partial_data6;
	int64_t offset;
	int64_t sensitivity;
	uint64_t comp_press;

	partial_data1 = calib_bmp.T_fine * calib_bmp.T_fine;
	partial_data2 = partial_data1 / 64;
	partial_data3 = (partial_data2 * calib_bmp.T_fine) / 256;
	partial_data4 = (calib_bmp.P8 * partial_data3) / 32;
	partial_data5 = (calib_bmp.P7 * partial_data1) * 16;
	partial_data6 = (calib_bmp.P6 * calib_bmp.T_fine) * 4194304;
	offset = (int64_t)((int64_t)(calib_bmp.P5) * (int64_t)140737488355328) +
		partial_data4 + partial_data5 + partial_data6;

	partial_data2 = (((int64_t)calib_bmp.P4) * partial_data3) / 32;
	partial_data4 = (calib_bmp.P3 * partial_data1) * 4;
	partial_data5 = ((int64_t)(calib_bmp.P2) - 16384) * ((int64_t)calib_bmp.T_fine) * 2097152;
	sensitivity = (((int64_t)(calib_bmp.P1) - 16384) * (int64_t)70368744177664) +
		partial_data2 + partial_data4 + partial_data5;

	partial_data1 = (sensitivity / 16777216) * u32RegData;
	partial_data2 = (int64_t)(calib_bmp.P10) * (int64_t)(calib_bmp.T_fine);
	partial_data3 = partial_data2 + (65536 * (int64_t)(calib_bmp.P9));
	partial_data4 = (partial_data3 * u32RegData) / 8192;
	partial_data5 = (partial_data4 * u32RegData) / 512;
	partial_data6 = (int64_t)((uint64_t)u32RegData * (uint64_t)u32RegData);
	partial_data2 = ((int64_t)(calib_bmp.P11) * (int64_t)(partial_data6)) / 65536;
	partial_data3 = (partial_data2 * u32RegData) / 128;
	partial_data4 = (offset / 4) + partial_data1 + partial_data5 + partial_data3;
	comp_press = (((uint64_t)partial_data4 * 25) / (uint64_t)1099511627776);

	return comp_press;
}

void bmpReadTemperature(bmp388* b){
	uint8_t dat[3];
	I2Cread(BMP388_DEFAULT_ADDRESS, BMP388_REG_DATA_TEMP, 3, dat);
	b -> temp = bmp388CompensateTemp((uint32_t) (dat[2] << 16) | (dat[1]  << 8)  | (dat[0]) );
}

void bmpReadPressure(bmp388* b){
	uint8_t dat[3];
	I2Cread(BMP388_DEFAULT_ADDRESS, BMP388_REG_DATA_PRESS, 3, dat);
	b -> press = bmp388CompensatePress( (uint32_t)(dat[2] << 16) | (dat[1]  << 8)  | (dat[0]) );
}


void bmp388ReadAltitude(bmp388* b){


	if( b -> cont == 0) bmpReadTemperature(b);
	b -> cont = (b -> cont + 1)%(b -> temp_cont);

	bmpReadPressure(b);

	if( b->seaLevel == 0 ) b->seaLevel = b->press;

	b -> raw_altitude = 44330.0 * (1.0 -  pow( 1.0*b->press / b->seaLevel,0.1903));
	b -> raw_altitude = computeMvAvgFilter( &(b->f), b->raw_altitude);
	b -> altitude = b -> raw_altitude + b -> alt_offset;
}

void updateBmp388Offset(bmp388 * b){
	b -> alt_offset = - b -> raw_altitude;
}

