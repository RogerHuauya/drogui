#ifndef SS_HPP
#define SS_HPP
#define pi 3.14159265359

typedef struct _droneModel{

	double a[3], ap[3], app[3], rpp[3], rp[3], r[3];
	double U[4];

} droneModel;

void computeModel(droneModel *dm, double u[4], double dt);
void initModel(droneModel *dm);
droneModel copyModel(droneModel *dm);
#endif
