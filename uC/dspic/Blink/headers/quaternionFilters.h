#ifndef _QUATERNIONFILTERS_H_
#define _QUATERNIONFILTERS_H_

#include <math.h>
#define PI 3.14159264
void MadgwickQuaternionUpdate(double ax, double ay, double az, double gx, double gy,
                              double gz, double mx, double my, double mz,
                              double deltat);
void MahonyQuaternionUpdate(double ax, double ay, double az, double gx, double gy,
                            double gz, double mx, double my, double mz,
                            double deltat);
const double * getQ();

#endif // _QUATERNIONFILTERS_H_
