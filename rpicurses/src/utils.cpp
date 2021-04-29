#include "utils.h"
#include <math.h>

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
    float ans = bytestofloat(buff);
    if( fabs(ans) > 100000 || isnan(ans) ) ans = 0;
    return ans;
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
    buff[0] = reg|1;
    for(int i = 0; i < 4; i++) buff[i+1] = val[i];
    write(file_id, buff, 5);
}

void rasp_I2C::readMCU(uint8_t reg, uint8_t * val){
    unsigned char buff[10];
    buff[0] = reg;
    write(file_id, buff, 1);
    read(file_id, val, 4);
    //for(int i = 0; i < 4 ; i++) std::cout << (int)val[i] << " ";
    //std::cout << std::endl;
}

void cls(){
    system("clear");
}

std::string varNames[] = {"","","CAL_SYS","","CAL_GYR","","CAL_ACC","","CAL_MAG","","CAL_GYR_TRG","","CAL_ACC_TRG","","CAL_MAG_TRG","","ROLL_OFFSET","","PITCH_OFFSET","","YAW_OFFSET","","GYR_X_OFF","","GYR_Y_OFF","","GYR_Z_OFF","","ACC_X_OFF","","ACC_Y_OFF","","ACC_Z_OFF","","ACC_SCALE","","MAG_X_OFF","","MAG_Y_OFF","","MAG_Z_OFF","","MAG_X_SCALE","","MAG_Y_SCALE","","MAG_Z_SCALE","","ROLL_KP","","ROLL_KI","","ROLL_KD","","PITCH_KP","","PITCH_KI","","PITCH_KD","","YAW_KP","","YAW_KI","","YAW_KD","","X_KP","","X_KI","","X_KD","","Y_KP","","Y_KI","","Y_KD","","Z_KP","","Z_KI","","Z_KD","","PID_INDEX","","PID_VAR","","N_FILTER","","START_XYC","","ROLL_U","","PITCH_U","","YAW_U","","X_U","","Y_U","","Z_U","","MOTOR_1","","MOTOR_2","","MOTOR_3","","MOTOR_4","","Z_MG","","ROLL_REF","","PITCH_REF","","YAW_REF","","X_REF","","Y_REF","","Z_REF","","ROLL_REF_SIZE","","PITCH_REF_SIZE","","YAW_REF_SIZE","","X_REF_SIZE","","Y_REF_SIZE","","Z_REF_SIZE","","ROLL_PERIOD","","PITCH_PERIOD","","YAW_PERIOD","","X_PERIOD","","Y_PERIOD","","Z_PERIOD","","ROLL_SCURVE","","PITCH_SCURVE","","YAW_SCURVE","","X_SCURVE","","Y_SCURVE","","Z_SCURVE","","GYRO_X_REF","","GYRO_Y_REF","","GYRO_Z_REF","","ROLL_VAL","","PITCH_VAL","","YAW_VAL","","X_VAL","","Y_VAL","","Z_VAL","","RAW_ROLL","","RAW_PITCH","","RAW_YAW","","DER_ROLL","","DER_PITCH","","DER_YAW","","RAW_TEMP","","TEMP_ABS","","RAW_PRESS","","PRESS_ABS","","GPS_X","","GPS_Y","","GPS_CNT","","GPS_STATE","","GPS_AVAILABLE","","START_GPS","","ACC_X","","ACC_Y","","ACC_Z","","GYRO_X","","GYRO_Y","","GYRO_Z","","DER_GYRO_X","","DER_GYRO_Y"};

