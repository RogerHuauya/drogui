#include "utils.h"

std::string str_datetime(){
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    std::stringstream ss;
    ss << std::put_time(&tm, "%d-%m-%Y %H-%M-%S");
	return ss.str();
}

rasp_I2C::rasp_I2C(const int ADDRESS){
    rasp_I2C::fd = wiringPiI2CSetup(ADDRESS);
}

int32_t rasp_I2C::bytestoint32(uint8_t *bytesint32){
    int32_t val = (int32_t) (int32_t)(bytesint32[0]) << 24 |
            (int32_t)(bytesint32[1]) << 16 |
            (int32_t)(bytesint32[2]) << 8 |
            (int32_t)(bytesint32[3]);
    return val;
}

void rasp_I2C::int32tobytes(int32_t n,uint8_t *bytesint32){
    bytesint32[0] =(uint8_t) (n >> 24) & 0xFF;
    bytesint32[1] =(uint8_t) (n >> 16) & 0xFF;
    bytesint32[2] =(uint8_t) (n >> 8) & 0xFF;
    bytesint32[3] =(uint8_t)  n & 0xFF;
}


void rasp_I2C::floattobytes(float n, uint8_t* bytesfloat){
    memcpy(bytesfloat, &n, sizeof n);
}

float rasp_I2C::bytestofloat(uint8_t *bytesfloat){
    float val;
    memcpy(&val, bytesfloat, sizeof(val));
    return val;
}
void rasp_I2C::sendFloat(uint8_t reg, float val){
    uint8_t buff[4];
    rasp_I2C::floattobytes(val, buff);
    for(uint8_t i = 0; i < 4; i++) rasp_I2C::writeMCU(reg+i, buff[i]);
    return;
}
float rasp_I2C::readFloat(uint8_t reg){
    uint8_t buff[4];
    for (uint8_t i=0;i<4;i++)  buff[i] = rasp_I2C::readMCU(reg+i);
    return bytestofloat(buff);
}

void rasp_I2C::print4bytes(uint8_t *data){
    for (int i = 0; i < 4; ++i){
        std::cout<<+data[i]<<" ";
    }
    std::cout<<std::endl;
};

void rasp_I2C::writeMCU(uint8_t reg, uint8_t val){
	wiringPiI2CWriteReg8 (rasp_I2C::fd, reg, val);
    //cout<<"Register "<<reg<<" has been written with "<<val<<endl;
}
uint8_t rasp_I2C::readMCU(uint8_t reg){
    uint8_t val;
	val = wiringPiI2CReadReg8(rasp_I2C::fd, reg);
    //cout<<"Register "<<reg<<" has been written with "<<val<<endl;
    return val;
}

void cls(){
    system("clear");
}

