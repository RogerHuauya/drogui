#include "utils.h"

void int32pack(uint8_t *bytesint32,int32_t *n){
    *n = (int32_t)((int32_t)(bytesint32[0]) << 24 |
            (int32_t)(bytesint32[1]) << 16 |
            (int32_t)(bytesint32[2]) << 8 |
            (int32_t)(bytesint32[3]));
};
void int32unpack(int32_t n,uint8_t *bytesint32){
    bytesint32[0] = (n >> 24) & 0xFF;
    bytesint32[1] = (n >> 16) & 0xFF;
    bytesint32[2] = (n >> 8) & 0xFF;
    bytesint32[3] =  n & 0xFF;
};
void floatunpack(float n,uint8_t *bytesfloat){
    memcpy(bytesfloat, &n, sizeof n);
};
void floatpack(uint8_t *bytesfloat,float *n){
    memcpy(n, bytesfloat, sizeof n);
};

void print4bytes(uint8_t *data){
    for (int i = 0; i < 4; ++i){
        std::cout<<+data[i]<<" ";
    }
    std::cout<<std::endl;
};
