#include "utils.h"

void int32pack(unsigned char *bytesint32,int32_t &n){
    n = int((unsigned char)(bytesint32[0]) << 24 |
            (unsigned char)(bytesint32[1]) << 16 |
            (unsigned char)(bytesint32[2]) << 8 |
            (unsigned char)(bytesint32[3]));
};
void int32unpack(int32_t n,unsigned char *bytesint32){
    bytesint32[0] = (n >> 24) & 0xFF;
    bytesint32[1] = (n >> 16) & 0xFF;
    bytesint32[2] = (n >> 8) & 0xFF;
    bytesint32[3] =  n & 0xFF;
};
void floatunpack(float n,unsigned char *bytesfloat){
    memcpy(bytesfloat, &n, sizeof n);
};
void floatpack(unsigned char *bytesfloat,float &n){
    memcpy(&n, bytesfloat, sizeof n);
};

void print4bytes(unsigned char *data){
    for (int i = 0; i < 4; ++i){
        std::cout<<+data[i]<<" ";
    }
    std::cout<<std::endl;
};
