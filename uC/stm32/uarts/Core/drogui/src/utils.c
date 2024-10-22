#include "utils.h"
#include "i2c.h"

uint8_t i2cReg[300][4];
bool security;
int calib_status;
STATES state;

float k_1_10[] = {3.460276e-01,-7.771395e-01,9.035554e-01,-9.244740e-01,9.585879e-01,-8.644216e-01};
float v_1_10[] = {3.635609e-03,1.256157e-02,2.776979e-02,3.332425e-02,2.536337e-02,9.166384e-03,1.466144e-03};

float k_3_10[] = {-2.697967e-02,2.384924e-01,9.148143e-02,8.721701e-01,3.546651e-01};
float v_3_10[] = {1.218396e-01,4.440312e-01,5.297272e-01,7.356926e-02,-1.524383e-01,-3.933188e-02};

float k_1_20[] = {0.570001631825510, -0.930138552823475, 0.973018195256895, -0.970196251743969 };
float v_1_20[] = { 0.002648137189435, 0.007518846486390, 0.013531091846070, 0.010565966787239, 0.005217326465024};

float k_1_100[] = {-8.856142e-01,9.965195e-01,-9.983138e-01};
float v_1_100[] = {7.065026e-04,1.517713e-03,5.599321e-04,3.787484e-04};

float k_1_50[] = {-7.844177e-01,9.861931e-01,-9.932240e-01};
float v_1_50[] = {1.600364e-03,4.245443e-03,4.170715e-03,2.811345e-03};

float k_1_250[] = { 0.942157132445299, -0.998953614796242};
float v_1_250[] = { 0.001472815900340, 0.001920796559440, 0.002003978561397};

float k_3_200[] = {-8.334607e-01,9.921960e-01,-9.961988e-01};
float v_3_200[] = {1.115184e-03,2.650827e-03,1.825176e-03,1.232745e-03};

float hk_1_500[] = {9.556187e-01,-9.996168e-01,9.999276e-01,-9.999372e-01,9.999928e-01,-9.999328e-01};
float hv_1_500[] = {9.663555e-01,-4.391543e-02,-1.010283e-03,3.193904e-07,4.875738e-08,-6.060041e-12,-1.437628e-12};

float hk_1_200[] = {8.741497e-01,-9.966733e-01,9.993446e-01,-9.995081e-01,9.998978e-01,-9.995419e-01,9.999924e-01,-9.995050e-01};
float hv_1_200[] = {9.241645e-01,-1.246364e-01,-7.900981e-03,2.297352e-05,3.424981e-06,-4.595506e-09,-9.406023e-10,3.064216e-13,6.472600e-14};

float SG7[] = {0.10714, 0.07143, 0.03571, 0, -0.03571, -0.07143, -0.10714};
float SG10[] = { 0.05455, 0.04242, 0.0303, 0.01818, 0.00606, -0.00606, -0.01818, -0.0303, -0.04242, -0.05455};
float SG13[] = {  0.03297, 0.02747, 0.02198, 0.01648, 0.01099, 0.00549, 0, -0.00549, -0.01099, -0.01648, -0.02198, -0.02747, -0.03297};

int32_t bytestoint32(uint8_t *bytesint32){
    int32_t val = (int32_t) (int32_t)(bytesint32[0]) << 24 |
            (int32_t)(bytesint32[1]) << 16 |
            (int32_t)(bytesint32[2]) << 8 |
            (int32_t)(bytesint32[3]);
    return val;
}

void int32tobytes(int32_t n,uint8_t *bytesint32){
    bytesint32[0] =(uint8_t) (n >> 24) & 0xFF;
    bytesint32[1] =(uint8_t) (n >> 16) & 0xFF;
    bytesint32[2] =(uint8_t) (n >> 8) & 0xFF;
    bytesint32[3] =(uint8_t)  n & 0xFF;
}

void floattobytes(float n, uint8_t* bytesfloat){
    memcpy(bytesfloat, &n, sizeof n);
}

float bytestofloat(uint8_t *bytesfloat){
    float val;
    memcpy(&val, bytesfloat, sizeof(val));
    return val;
}

float getReg(uint8_t reg){
    
    float val;
    uint8_t arr[4];
    for(int i = 0; i < 4 ; i++){
        arr[i] = i2cReg[reg][i];
    }
    val = bytestofloat(arr); 
    return val; 
}

void setReg(uint8_t reg, float val){
    
    uint8_t arr[4];
    floattobytes(val, arr);

    for(int i = 0; i < 4 ; i++) i2cReg[reg][i] = arr[i];
    return;
}


float angle_dif(float angle1, float angle2){
    if(angle1 > angle2){
        if((angle1 - angle2) > (2*pi - angle1 + angle2)) return -2*pi + angle1 - angle2;
        else return angle1 - angle2;
    }
    else{
        if((angle2 - angle1) > (2*pi - angle2 + angle1)) return 2*pi - angle2 + angle1;
        else return angle1 - angle2;
    }
}

void getEuler(float q0, float q1, float q2, float q3, float* roll, float *pitch, float* yaw){
	
    // roll (x-axis rotation)
    float sinr_cosp = 2 * (q0 * q1 + q2 * q3);
    float cosr_cosp = 1 - 2 * (q1 * q1 + q2 * q2);
    *roll = atan2(sinr_cosp, cosr_cosp);

    // pitch (y-axis rotation)
    float sinp = 2 * (q0 * q2 - q3 * q1);
    if (fabs(sinp) >= 1)
        *pitch = copysign(pi / 2, sinp); // use 90 degrees if out of range
    else
        *pitch = asin(sinp);

    // yaw (z-axis rotation)
    float siny_cosp = 2 * (q0 * q3 + q1 * q2);
    float cosy_cosp = 1 - 2 * (q2 * q2 + q3 * q3);
    *yaw = atan2(siny_cosp, cosy_cosp);
}


void rampValue(float *var, float desired, float step){

    (*var) += fabs(desired - (*var) ) >= step  ? copysign(step, desired - (*var)) : (desired - (*var));
}  

void I2Cread(uint8_t Address, uint8_t Register, uint8_t Nbytes, uint8_t* Data){
    HAL_I2C_Master_Transmit(&hi2c1, (Address << 1), &Register, 1, 1000);
    HAL_I2C_Master_Receive(&hi2c1, (Address << 1) | 1, Data, Nbytes, 1000);
}
 
void I2CwriteByte(uint8_t Address, uint8_t Register, uint8_t Data){
    uint8_t data[] = {Register, Data};
    HAL_I2C_Master_Transmit(&hi2c1, (Address << 1), data, 2, 10000);
}

float dis3d(float x,float y,float z, float a, float b, float c){
    return sqrt((x - a)*(x - a) + (y - b)*(y - b) + (z - c)*(z - c));
}
