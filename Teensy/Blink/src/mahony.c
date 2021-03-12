#include "..\headers\mahony.h"
typedef uint64_t timeUs_t;
typedef int32_t timeDelta_t;
#define DEGREES_TO_RADIANS(angle) ((angle) * 0.0174532925f)
#define SPIN_RATE_LIMIT 20
#define sq(x) ((x)*(x))
#define M_PIf 3.14159265358979323846f
const float dcmKpGain = 10;
const float dcmKi = 1;

typedef struct _quaternion{
    float w,x,y,z;
} quaternion;
typedef struct _attitude{
    float roll, pitch, yaw;
} attitude;
typedef struct _quaternionProducts{
    float ww,wx,wy,wz,xx,xy,xz,yy,yz,zz;
} quaternionProducts;


mat rMat;

quaternion q ;
quaternionProducts qP;
attitude att;

extern mpu9250 myIMU;
float sin_approx(float x)
{
    #define sinPolyCoef3 -1.666665710e-1f                                          // Double: -1.666665709650470145824129400050267289858e-1
    #define sinPolyCoef5  8.333017292e-3f                                          // Double:  8.333017291562218127986291618761571373087e-3
    #define sinPolyCoef7 -1.980661520e-4f                                          // Double: -1.980661520135080504411629636078917643846e-4
    #define sinPolyCoef9  2.600054768e-6f                                          // Double:  2.600054767890361277123254766503271638682e-6

    int32_t xint = x;
    if (xint < -32 || xint > 32) return 0.0f;                               // Stop here on error input (5 * 360 Deg)
    while (x >  M_PIf) x -= (2.0f * M_PIf);                                 // always wrap input angle to -PI..PI
    while (x < -M_PIf) x += (2.0f * M_PIf);
    if (x >  (0.5f * M_PIf)) x =  (0.5f * M_PIf) - (x - (0.5f * M_PIf));   // We just pick -90..+90 Degree
    else if (x < -(0.5f * M_PIf)) x = -(0.5f * M_PIf) - ((0.5f * M_PIf) + x);
    float x2 = x * x;
    return x + x * x2 * (sinPolyCoef3 + x2 * (sinPolyCoef5 + x2 * (sinPolyCoef7 + x2 * sinPolyCoef9)));
}

float cos_approx(float x)
{
    return sin_approx(x + (0.5f * M_PIf));
}

float atan2_approx(float y, float x)
{
    #define atanPolyCoef1  3.14551665884836e-07f
    #define atanPolyCoef2  0.99997356613987f
    #define atanPolyCoef3  0.14744007058297684f
    #define atanPolyCoef4  0.3099814292351353f
    #define atanPolyCoef5  0.05030176425872175f
    #define atanPolyCoef6  0.1471039133652469f
    #define atanPolyCoef7  0.6444640676891548f

    float res, absX, absY;
    absX = fabsf(x);
    absY = fabsf(y);
    res  = max(absX, absY);
    if (res) res = min(absX, absY) / res;
    else res = 0.0f;
    res = -((((atanPolyCoef5 * res - atanPolyCoef4) * res - atanPolyCoef3) * res - atanPolyCoef2) * res - atanPolyCoef1) / ((atanPolyCoef7 * res + atanPolyCoef6) * res + 1.0f);
    if (absY > absX) res = (M_PIf / 2.0f) - res;
    if (x < 0) res = M_PIf - res;
    if (y < 0) res = -res;
    return res;
}

void imuInitMahony(){
    matInit(&rMat, 3, 3);
};

void imuQuaternionComputeProducts(quaternion *quat, quaternionProducts *quatProd)
{
    quatProd->ww = quat->w * quat->w;
    quatProd->wx = quat->w * quat->x;
    quatProd->wy = quat->w * quat->y;
    quatProd->wz = quat->w * quat->z;
    quatProd->xx = quat->x * quat->x;
    quatProd->xy = quat->x * quat->y;
    quatProd->xz = quat->x * quat->z;
    quatProd->yy = quat->y * quat->y;
    quatProd->yz = quat->y * quat->z;
    quatProd->zz = quat->z * quat->z;
}

void imuComputeRotationMatrix(void){
    imuQuaternionComputeProducts(&q, &qP);
    setMatVal(rMat, 0, 0, 1.0f - 2.0f * qP.yy - 2.0f * qP.zz);
    setMatVal(rMat, 0, 1, 2.0f * (qP.xy + -qP.wz));
    setMatVal(rMat, 0, 2, 2.0f * (qP.xz - -qP.wy));
    setMatVal(rMat, 1, 0, 2.0f * (qP.xy - -qP.wz));
    setMatVal(rMat, 1, 1, 1.0f - 2.0f * qP.xx - 2.0f * qP.zz);
    setMatVal(rMat, 1, 2, 2.0f * (qP.yz + -qP.wx));
    setMatVal(rMat, 2, 0, 2.0f * (qP.xz + -qP.wy));
    setMatVal(rMat, 2, 1, 2.0f * (qP.yz - -qP.wx));
    setMatVal(rMat, 2, 2, 1.0f - 2.0f * qP.xx - 2.0f * qP.yy);
}

void 
void imuCalculateEstimatedAttitude(timeUs_t currentTimeUs) {
    static timeUs_t previousIMUUpdateTime = 0;

    const timeDelta_t deltaT = currentTimeUs - previousIMUUpdateTime;
    previousIMUUpdateTime = currentTimeUs;


    imuMahonyAHRSupdate(deltaT * 1e-6f, myIMU.ax, myIMU.ay, myIMU.az,
                        DEGREES_TO_RADIANS(myIMU.gx), DEGREES_TO_RADIANS(myIMU.gy), DEGREES_TO_RADIANS(myIMU.gz),
                        myIMU.mx, myIMU.my, myIMU.mz);

    imuUpdateEulerAngles();

}



void imuMahonyAHRSupdate(float dt, float ax, float ay, float az, float gx, float gy, float gz, float mx, float my, float mz)
{

    static float integralFBx = 0.0f,  integralFBy = 0.0f, integralFBz = 0.0f;    // integral error terms scaled by Ki

    // Calculate general spin rate (rad/s)
    const float spin_rate = sqrtf(sq(gx) + sq(gy) + sq(gz));

    // Use raw heading error (from GPS or whatever else)
    float ex = 0, ey = 0, ez = 0;


    // Use measured magnetic field vector
    float mxRot = mx;
    float myRot = my;
    float mzRot = mz;
    float recipMagNorm = sq(mx) + sq(my) + sq(mz);
    if (recipMagNorm > 0.01f) {
        // Normalise magnetometer measurement
        recipMagNorm = 1/sqrtf(recipMagNorm);
        mxRot *= recipMagNorm;
        myRot *= recipMagNorm;
        mzRot *= recipMagNorm;

        // For magnetometer correction we make an assumption that magnetic field is perpendicular to gravity (ignore Z-component in EF).
        // This way magnetic field will only affect heading and wont mess roll/pitch angles

        // (hx; hy; 0) - measured mag field vector in EF (assuming Z-component is zero)
        // (bx; 0; 0) - reference mag field vector heading due North in EF (assuming Z-component is zero)
        const float hx = rMat[0][0] * mxRot + rMat[0][1] * myRot + rMat[0][2] * mzRot;
        const float hy = rMat[1][0] * mxRot + rMat[1][1] * myRot + rMat[1][2] * mzRot;
        const float bx = sqrtf(hx * hx + hy * hy);

        // magnetometer error is cross product between estimated magnetic north and measured magnetic north (calculated in EF)
        const float ez_ef = -(hy * bx);

        // Rotate mag error vector back to BF and accumulate
        ex += rMat[2][0] * ez_ef;
        ey += rMat[2][1] * ez_ef;
        ez += rMat[2][2] * ez_ef;
    }


    // Use measured acceleration vector
    float recipAccNorm = sq(ax) + sq(ay) + sq(az);
    if (recipAccNorm > 0.01f) {
        // Normalise accelerometer measurement
        recipAccNorm = 1/sqrtf(recipAccNorm);
        ax *= recipAccNorm;
        ay *= recipAccNorm;
        az *= recipAccNorm;

        // Error is sum of cross product between estimated direction and measured direction of gravity
        ex += (ay * rMat[2][2] - az * rMat[2][1]);
        ey += (az * rMat[2][0] - ax * rMat[2][2]);
        ez += (ax * rMat[2][1] - ay * rMat[2][0]);
    }

    // Compute and apply integral feedback if enabled
    if (dcmKi > 0.0f) {
        // Stop integrating if spinning beyond the certain limit
        if (spin_rate < DEGREES_TO_RADIANS(SPIN_RATE_LIMIT)) {
            const float dcmKiGain = dcmKi;
            integralFBx += dcmKiGain * ex * dt;    // integral error scaled by Ki
            integralFBy += dcmKiGain * ey * dt;
            integralFBz += dcmKiGain * ez * dt;
        }
    } else {
        integralFBx = 0.0f;    // prevent integral windup
        integralFBy = 0.0f;
        integralFBz = 0.0f;
    }

    // Apply proportional and integral feedback
    gx += dcmKpGain * ex + integralFBx;
    gy += dcmKpGain * ey + integralFBy;
    gz += dcmKpGain * ez + integralFBz;

    // Integrate rate of change of quaternion
    gx *= (0.5f * dt);
    gy *= (0.5f * dt);
    gz *= (0.5f * dt);

    quaternion buffer;
    buffer.w = q.w;
    buffer.x = q.x;
    buffer.y = q.y;
    buffer.z = q.z;

    q.w += (-buffer.x * gx - buffer.y * gy - buffer.z * gz);
    q.x += (+buffer.w * gx + buffer.y * gz - buffer.z * gy);
    q.y += (+buffer.w * gy - buffer.x * gz + buffer.z * gx);
    q.z += (+buffer.w * gz + buffer.x * gy - buffer.y * gx);

    // Normalise quaternion
    float recipNorm = 1/sqrtf(sq(q.w) + sq(q.x) + sq(q.y) + sq(q.z));
    q.w *= recipNorm;
    q.x *= recipNorm;
    q.y *= recipNorm;
    q.z *= recipNorm;

    // Pre-compute rotation matrix from quaternion
    imuComputeRotationMatrix();
}


void imuUpdateEulerAngles(){
    att.roll = lrintf(atan2_approx(rMat[2][1], rMat[2][2]) * (1800.0f / M_PIf));
    att.pitch = lrintf(((0.5f * M_PIf) - acos_approx(-rMat[2][0])) * (1800.0f / M_PIf));
    att.yaw = lrintf((-atan2_approx(rMat[1][0], rMat[0][0]) * (1800.0f / M_PIf)));

    if (att.yaw < 0) {
        att.yaw += 3600;
    }
}
