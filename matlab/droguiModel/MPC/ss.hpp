#ifndef SS_HPP
#define SS_HPP

typedef struct _droneModel{

	float a[3], ap[3], app[3], rpp[3], rp[3], r[3];
	float F[4], M[4];

} droneModel;

void computeModel(droneModel *dm, float u[4], float dt);
void initModel(droneModel *dm);
droneModel copyModel(droneModel *dm);
#endif
