#include "BMP280.h"
#include "serial.h"
#include <stdio.h>
#include <string.h>
uint32_t t_fine;


int32_t const_T[5];

int64_t const_P[10];

void initBmp280(){
    initSPI();
    spiStart();
    spiExchangeByte(0x74);
    spiExchangeByte(0x57);
    /*
    spiExchangeByte(0x75);
    spiExchangeByte(---);*/
    spiStop();

    int16_t lsb, msb;
    uint16_t ulsb, umsb;
    int i = 0;
    spiStart();
    spiExchangeByte(0x88);

    ulsb = spiExchangeByte(0) & 0xFF;
    umsb = spiExchangeByte(0) & 0xFF;
    const_T[1] = umsb << 8 | ulsb; 
    //sprintf(s, "- %hhx %hhx %ld -", ulsb, umsb, const_T[1]);
    //serialWriteString(s); 

    
    for(i = 2 ; i <= 3 ; i++){
        lsb = spiExchangeByte(0) & 0xFF;
        msb = spiExchangeByte(0) & 0xFF;
        const_T[i] = ((msb << 8) | lsb); 
        //sprintf(s, "-  %hhx %hhx %ld - ",lsb, msb, const_T[i]);
        //serialWriteString(s);       
    }
    //serialWriteChar('\n');
    
    ulsb = spiExchangeByte(0) & 0xFF;
    umsb = spiExchangeByte(0) & 0xFF;
    const_P[1] = (umsb << 8) | ulsb; 
    //const_P[1] = 39477;
    //sprintf(s, "- %hhx %hhx %lld -", ulsb, umsb, const_P[1]);
    //serialWriteString(s); 


    for(i = 2 ; i <= 9 ; i++){
        lsb = spiExchangeByte(0) & 0xFF;
        msb = spiExchangeByte(0) & 0xFF;
        const_P[i] = (msb << 8) | lsb;        
        //sprintf(s, "- %hhx %hhx %lld -", lsb, msb, const_P[i]);
        //serialWriteString(s);
    }

    //serialWriteChar('\n');

    spiStop();
}

int32_t bmpReadTemperature(){
    int32_t dat[3];
    spiStart();
    spiExchangeByte(0xFA);
    dat[0] = spiExchangeByte(0) & 0xFF;
    dat[1] = spiExchangeByte(0) & 0xFF;
    dat[2] = spiExchangeByte(0) & 0xFF;
    spiStop();
    return (((dat[0] << 8) | (dat[1]) ) << 4)  | (dat[2] >> 4);

    
}
int32_t bmpReadPressure(){
    int32_t dat[3];
    spiStart();
    spiExchangeByte(0xF7);
    dat[0] = spiExchangeByte(0) & 0xFF;
    dat[1] = spiExchangeByte(0) & 0xFF;
    dat[2] = spiExchangeByte(0) & 0xFF;
    spiStop();

    return (((dat[0] << 8) | (dat[1]) ) << 4)  | (dat[2] >> 4);

}


double bmp280CompensateTemperature(int32_t adc){
    int32_t var_1, var_2, T;
    var_1 = ((((adc >> 3) - ((int32_t) const_T[1] << 1))) * ((int32_t) const_T[2])) >> 11;
    var_2 = (((((adc>>4) - ((int32_t) const_T[1])) * ((adc>>4) - ((int32_t) const_T[1])))  >>12) \
                 * ((int32_t) const_T[3])) >> 14;
    t_fine = var_1 + var_2;
    T = (t_fine*5+128) >> 8;
    /*sprintf(s, "Press Compensate%ld %ld %ld\n", var_1, var_2, t_fine);
    serialWriteString(s);*/

    return T / 100.0;
}


double bmp280CompensatePressure(int32_t adc){
    int64_t var_1, var_2, p;
    var_1 = ((int64_t) t_fine) -128000;
    var_2 = var_1*var_1*((int64_t) const_P[6]);
    var_2 += ((var_1* (int64_t) const_P[5])<<17);
    var_2 += (((int64_t) const_P[4]) << 35);
    var_1 = ((var_1*var_1*(int64_t) const_P[3])>>8) + ((var_1 * (int64_t)const_P[2]) << 12);
    var_1 = (((((int64_t)1) << 47) + var_1))*((int64_t) const_P[1])>>33;
    if(var_1 ==0) return 0; //division by cero
    p = 1048576 - adc;
    p = (((p<<31) - var_2)*3125)/var_1;
    var_1 = (((int64_t) const_P[9]) * (p>>13) * (p>>13)) >>25;
    var_2 = ((( int64_t ) const_P[8]) * p) >>19;
    p = ((p + var_1 + var_2) >> 8) + (((int64_t) const_P[7]) << 4);
    return p/256.0;
}
