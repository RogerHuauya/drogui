#define BMP388_TEST
#ifdef BMP388_TEST

#include "_main.h"
#include "BMP388.h"
#include "filter.h"
#include "macros.h"
#include "serial.h"
#include "utils.h"
#define N_BMP 50

char buffer_bmp[500];
bmp388 _myBMP;

emaFilter _ema_bmp;
mvAvgFilter _mvAvg_bmp;

void _main(){

	serialsBegin();
	changeBaudrate(SER_DBG, 1000000);
	serialPrint(SER_DBG, "BMP388_TEST\n");
	initBmp388(&_myBMP, 10);

	initMvAvgFilter(&_mvAvg_bmp, N_BMP);
	initEmaFilter(&_ema_bmp, 0.9, 0.1, 0.8);

	while(1){
		float bmp_alt;

		HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
		bmp388ReadAltitude(&_myBMP);

		bmp_alt = computeMvAvgFilter( &_mvAvg_bmp, _myBMP.altitude );
		bmp_alt = computeEmaFilter( &_ema_bmp, bmp_alt );

		serialPrintf(SER_DBG,"%f %f ;\n", 100*_myBMP.altitude, 100*bmp_alt);
		HAL_Delay(5);
	}
}

#endif
