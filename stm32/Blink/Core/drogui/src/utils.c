#include "utils.h"


uint8_t i2cReg[300][4];
bool security;

// Fs = 100Hz


//Lowpass freq = 10Hz
float coeffA_10Hz[] = {-6.1252,17.2079,-28.7647,31.1789,-22.3898,10.3890,-2.8462,0.3526};
float coeffB_10Hz[] = {0.0027,-0.0085,0.0164,-0.0210,0.0231,-0.0210,0.0164,-0.0085,0.0027};

//Lowpass freq = 8Hz
float coeffA_8Hz[] = {-5.709477e+00,1.442617e+01,-2.083473e+01,1.853513e+01,-1.014308e+01,3.158832e+00,-4.317046e-01};
float coeffB_8Hz[] = {2.052007e-03,-5.805565e-03,7.962321e-03,-3.642807e-03,-3.642807e-03,7.962321e-03,-5.805565e-03,2.052007e-03};

//Lowpass freq = 2Hz
float coeffB_2Hz[] = {5.703936e-04,-1.574753e-03,1.011169e-03,1.011169e-03,-1.574753e-03,5.703936e-04};
float coeffA_2Hz[] = {-4.763156e+00,9.099046e+00,-8.713129e+00,4.182120e+00,-8.048677e-01};

//Lowpass freq = 5Hz
/**
 * @brief waaat 
 */
float coeffB_5Hz[] = {1.113479e-03,-4.496681e-03,7.102010e-03,-3.694502e-03,-3.694502e-03,7.102010e-03,-4.496681e-03,1.113479e-03};
float coeffA_5Hz[] = {-6.292710e+00,1.716382e+01,-2.628801e+01,2.440554e+01,-1.372942e+01,4.332229e+00,-5.913992e-01};


// Fs = 1000Hz

//lowpass freq = 150Hz
float coeffB_150Hz[] = { 0.0056, -0.0070, 0.0170, -0.0072, 0.0100, 0.0100, -0.0072, 0.0170 , -0.0070, 0.0056};
float coeffA_150Hz[] = { -5.3336, 14.3803, -24.7331, 29.5687, -25.2823, 15.4142, -6.4538, 1.6872, -0.2108};

//Lowpass freq  = 100 Hz
float coeffB_100Hz[] = { 0.0027, -0.0085, 0.0164, -0.0210, 0.0231, -0.0210, 0.0164, -0.0085, 0.0027}; 
float coeffA_100Hz[] = { -6.1252, 17.2079, -28.7647, 31.1789, -22.3898, 10.3890, -2.8462, 0.3526 };

//Lowpass freq = 300 Hz
float coeffB_300Hz[] = { 0.0523, 0.3005, 0.9285, 1.9192, 2.8985, 3.3135, 2.8985, 1.9192, 0.9285, 0.3005, 0.0523 }; 
float coeffA_300Hz[] = { 1.2129, 3.5334, 2.4263, 3.9286, 1.3120, 1.8104, 0.0802, 0.3968, -0.0573, 0.0478 }; 


// Fs = 500Hz

float coeffB_50Hz[] = {2.712427e-03,-8.492766e-03,1.639262e-02,-2.098534e-02,2.308792e-02,-2.098534e-02,1.639262e-02,-8.492766e-03,2.712427e-03};
float coeffA_50Hz[] = {-6.125230e+00,1.720786e+01,-2.876469e+01,3.117890e+01,-2.238977e+01,1.038896e+01,-2.846250e+00,3.525782e-01};


// Fc/Fs = 1/10
int sz_1_10 = 4;
float k_1_10[] = {3.269648e-01,-7.541241e-01,8.955497e-01,-8.705845e-01};
float v_1_10[] = {9.923580e-03,4.324221e-02,9.622375e-02,9.899868e-02,3.930249e-02};

// Fc/Fs = 3/10
int sz_3_10 = 5;
float k_3_10[] = {-2.697967e-02,2.384924e-01,9.148143e-02,8.721701e-01,3.546651e-01};
float v_3_10[] = {1.218396e-01,4.440312e-01,5.297272e-01,7.356926e-02,-1.524383e-01,-3.933188e-02};

// Fc/Fs = 5/100
int sz_5_100 = 4;
float k_5_100[] = {0.570001631825510, -0.930138552823475, 0.973018195256895, -0.970196251743969 };
float v_5_100[] = { 0.002648137189435, 0.007518846486390, 0.013531091846070, 0.010565966787239, 0.005217326465024};

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