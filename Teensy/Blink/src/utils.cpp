#include "..\headers\utils.h"
#include "..\headers\i2c.h"



elapsedMicros time = 0;
bool security;
//Lowpass freq = 10Hz
float coeffA_10Hz[] = {-6.1252,17.2079,-28.7647,31.1789,-22.3898,10.3890,-2.8462,0.3526};
float coeffB_10Hz[] = {0.0027,-0.0085,0.0164,-0.0210,0.0231,-0.0210,0.0164,-0.0085,0.0027};

//Lowpass freq = 2Hz
float coeffB_2Hz[] = {0.0001, -0.0012, 0.0064, -0.0209, 0.0462, -0.0731, 0.0850, -0.0731, 0.0462, -0.0209, 0.0064, -0.0012, 0.0001};
float coeffA_2Hz[] = {-11.8247, 64.1475, -211.1035, 469.3812, -742.8519, 858.0511, -728.8477, 451.8482, -199.3839, 59.4422, -10.7503, 0.8919};

//lowpass freq = 150Hz
float coeffB_150Hz[] = { 0.0056, -0.0070, 0.0170, -0.0072, 0.0100, 0.0100, -0.0072, 0.0170 , -0.0070, 0.0056};
float coeffA_150Hz[] = { -5.3336, 14.3803, -24.7331, 29.5687, -25.2823, 15.4142, -6.4538, 1.6872, -0.2108};

//Lowpass freq  = 100 Hz
float coeffB_100Hz[] = { 0.0027, -0.0085, 0.0164, -0.0210, 0.0231, -0.0210, 0.0164, -0.0085, 0.0027}; 
float coeffA_100Hz[] = { -6.1252, 17.2079, -28.7647, 31.1789, -22.3898, 10.3890, -2.8462, 0.3526 };

//Lowpass freq = 300 Hz
float coeffB_300Hz[] = { 0.0523, 0.3005, 0.9285, 1.9192, 2.8985, 3.3135, 2.8985, 1.9192, 0.9285, 0.3005, 0.0523 }; 
float coeffA_300Hz[] = { 1.2129, 3.5334, 2.4263, 3.9286, 1.3120, 1.8104, 0.0802, 0.3968, -0.0573, 0.0478 }; 

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
    
    double val;
    uint8_t arr[4];
    for(int i = 0; i < 4 ; i++){
     arr[i] = i2c1Reg[reg][i];
     //if(reg == 72) Serial.print(arr[i]);
    }
    val = bytestofloat(arr); 
     //if(reg == 72)Serial.println("");
    return val; 
}

void setReg(uint8_t reg, float val){
    uint8_t arr[4];
    //Serial.print(reg),Serial.print("\t");
    floattobytes(val, arr);

    for(int i = 0; i < 4 ; i++) i2c1Reg[reg][i] = arr[i]/*,Serial.print(arr[i]),Serial.print("\t")*/;
    //Serial.println("");
    return;
}


double angle_dif(double angle1, double angle2){
    if(angle1 > angle2){
        if((angle1 - angle2) > (2*pi - angle1 + angle2)) return -2*pi + angle1 - angle2;
        else return angle1 - angle2;
    }
    else{
        if((angle2 - angle1) > (2*pi - angle2 + angle1)) return 2*pi - angle2 + angle1;
        else return angle1 - angle2;
    }
}

void getEuler(double q0, double q1, double q2, double q3, double* roll, double *pitch, double* yaw){
	
    // roll (x-axis rotation)
    double sinr_cosp = 2 * (q0 * q1 + q2 * q3);
    double cosr_cosp = 1 - 2 * (q1 * q1 + q2 * q2);
    *roll = atan2(sinr_cosp, cosr_cosp);

    // pitch (y-axis rotation)
    double sinp = 2 * (q0 * q2 - q3 * q1);
    if (fabs(sinp) >= 1)
        *pitch = copysign(pi / 2, sinp); // use 90 degrees if out of range
    else
        *pitch = asin(sinp);

    // yaw (z-axis rotation)
    double siny_cosp = 2 * (q0 * q3 + q1 * q2);
    double cosy_cosp = 1 - 2 * (q2 * q2 + q3 * q3);
    *yaw = atan2(siny_cosp, cosy_cosp);
}


void rampValue(double *var, double desired, double step){

    (*var) += fabs(desired - (*var) ) >= step  ? copysign(step, desired - (*var)) : (desired - (*var));
}  