#ifndef BMP280_H
#define BMP280_H
#include <stdint.h>
#include "spi.h"

double bmp280CompensateTemperature(int32_t adc);
double bmp280CompensatePressure(int32_t adc);
void initBmp280();
int32_t bmpReadTemperature();
int32_t bmpReadPressure();

#endif