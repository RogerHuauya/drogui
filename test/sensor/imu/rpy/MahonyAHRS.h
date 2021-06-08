#ifndef MahonyAHRS_h
#define MahonyAHRS_h


void MahonyAHRSupdate(float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz);
void MahonyAHRSupdateIMU(float gx, float gy, float gz, float ax, float ay, float az);
double * getMahonyQuaternion();
double * getMahonyEuler();
#endif