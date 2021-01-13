#include "utils.h"

std::string str_datetime(){
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    std::stringstream ss;
    ss << std::put_time(&tm, "%d-%m-%Y %H-%M-%S");
	return ss.str();
}

rasp_I2C::rasp_I2C(const int ADDRESS){
    adress = ADDRESS;
    printf("%X", adress);
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
    uint8_t buff[5];
    rasp_I2C::floattobytes(val, buff);
    rasp_I2C::writeMCU(reg, buff);
    return;
}
float rasp_I2C::readFloat(uint8_t reg){
    uint8_t buff[5];
    rasp_I2C::readMCU(reg, buff);
    return bytestofloat(buff);
}

void rasp_I2C::print4bytes(uint8_t *data){
    for (int i = 0; i < 4; ++i){
        std::cout<<+data[i]<<" ";
    }
    std::cout<<std::endl;
};


void rasp_I2C::setup(){
    char *filename = (char*)"/dev/i2c-1";
    if ((file_id = open(filename, O_RDWR)) < 0){

        printf("Failed to open the i2c bus");
        return;
    }
    if (ioctl(file_id, I2C_SLAVE, adress) < 0){

        printf("Failed to acquire bus access and/or talk to slave.\n");
        return;
    }
    printf("initialized\n");
} 

void rasp_I2C::finish(){
	close(file_id);
}


void rasp_I2C::writeMCU(uint8_t reg, uint8_t* val){
    
    unsigned char buff[10];
    buff[0] = reg | 1;
    for(int i = 0; i < 4; i++) buff[i+1] = val[i];
    write(file_id, buff, 5);
}

void rasp_I2C::readMCU(uint8_t reg, uint8_t * val){
    unsigned char buff[10];
    buff[0] = reg;
    write(file_id, buff, 1);
    read(file_id, val, 4);
}

void cls(){
    system("clear");
}

