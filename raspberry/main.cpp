#include <stdio.h>
#include "utils.h"

int main(){

    int32_t n1 = 4231,test1;
    float n2 = 14.215,test2;
    unsigned char bytesint32[4];
    unsigned char bytesfloat[4];

    std::cout<<"send: "<<n1<<std::endl;
    int32unpack(n1,bytesint32);
    print4bytes(bytesint32);
    int32pack(bytesint32,test1);
    std::cout<<"result: "<<test1<<std::endl;

    std::cout<<"send: "<<n2<<std::endl;
    floatunpack(n2,bytesfloat);
    print4bytes(bytesfloat);
    floatpack(bytesfloat,test2);
    std::cout<<"result: "<<test2<<std::endl;

    printf("Hello world\n");
    return 0;
}
