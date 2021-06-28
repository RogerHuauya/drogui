//#ifndef REGISTER_MAP_H
//#define REGISTER_MAP_H


//enum PIDVAR {PID_ROLL, PID_PITCH, PID_YAW, PID_X, PID_Y, PID_Z};


///**********************************
// * Calibration
// **********************************/

const int CAL_SYS = 	0x02;
const int CAL_GYR = 	0x04;
const int CAL_ACC = 	0x06;
const int CAL_MAG = 	0x08;

const int CAL_GYR_TRG = 	0x0A;
const int CAL_ACC_TRG = 	0x0C;
const int CAL_MAG_TRG = 	0x0E;


const int ROLL_OFFSET = 	0x10;
const int PITCH_OFFSET = 	0x12;
const int YAW_OFFSET = 	0x14;

const int GYR_X_OFF = 	0x16;
const int GYR_Y_OFF = 	0x18;
const int GYR_Z_OFF = 	0x1A;

const int ACC_X_OFF = 	0x1C;
const int ACC_Y_OFF = 	0x1E;
const int ACC_Z_OFF = 	0x20;
const int ACC_SCALE = 	0x22;

const int MAG_X_OFF = 	0x24;
const int MAG_Y_OFF = 	0x26;
const int MAG_Z_OFF = 	0x28;
const int MAG_X_SCALE = 	0x2A;
const int MAG_Y_SCALE = 	0x2C;
const int MAG_Z_SCALE = 	0x2E;


///**********************************
// * Control Constants
// **********************************/

const int ROLL_KP = 	0x30;
const int ROLL_KI = 	0x32;
const int ROLL_KD = 	0x34;

const int PITCH_KP = 	0x36;
const int PITCH_KI = 	0x38;
const int PITCH_KD = 	0x3A;

const int YAW_KP = 	0x3C;
const int YAW_KI = 	0x3E;
const int YAW_KD = 	0x40;

const int X_KP = 	0x42;
const int X_KI = 	0x44;
const int X_KD = 	0x46;

const int Y_KP = 	0x48;
const int Y_KI = 	0x4A;
const int Y_KD = 	0x4C;

const int Z_KP = 	0x4E;
const int Z_KI = 	0x50;
const int Z_KD = 	0x52;

const int PID_INDEX = 	0x54;
const int PID_VAR = 	0x56;
const int N_FILTER = 	0x58;
const int START_XYC = 	0x5A;


///**********************************
// * Control Law
// **********************************/


const int ROLL_U = 	0x5C;
const int PITCH_U = 	0x5E;
const int YAW_U = 	0x60;
const int X_U = 	0x62;
const int Y_U = 	0x64;
const int Z_U = 	0x66;

const int MOTOR_1 = 	0x68;
const int MOTOR_2 = 	0x6A;
const int MOTOR_3 = 	0x6C;
const int MOTOR_4 = 	0x6E;

const int Z_MG = 	0x70;

///**********************************
// * Control Setpoint
// **********************************/

const int ROLL_REF = 	0x72;
const int PITCH_REF = 	0x74;
const int YAW_REF = 	0x76;
const int X_REF = 	0x78;
const int Y_REF = 	0x7A;
const int Z_REF = 	0x7C;

const int ROLL_REF_SIZE = 	0x7E;
const int PITCH_REF_SIZE = 	0x80;
const int YAW_REF_SIZE = 	0x82;
const int X_REF_SIZE = 	0x84;
const int Y_REF_SIZE = 	0x86;
const int Z_REF_SIZE = 	0x88;

const int ROLL_PERIOD = 	0x8A;
const int PITCH_PERIOD = 	0x8C;
const int YAW_PERIOD = 	0x8E;
const int X_PERIOD = 	0x90;
const int Y_PERIOD = 	0x92;
const int Z_PERIOD = 	0x94;

const int ROLL_SCURVE = 	0x96;
const int PITCH_SCURVE = 	0x98;
const int YAW_SCURVE = 	0x9A;
const int X_SCURVE = 	0x9C;
const int Y_SCURVE = 	0x9E;
const int Z_SCURVE = 	0xA0;

const int GYRO_X_REF = 	0xA2;
const int GYRO_Y_REF = 	0xA4;
const int GYRO_Z_REF = 	0xA6;

///**********************************
// * State
// **********************************/

const int ROLL_VAL = 	0xA8;
const int PITCH_VAL = 	0xAA;
const int YAW_VAL = 	0xAC;

const int X_VAL = 	0xAE;
const int Y_VAL = 	0xB0;
const int Z_VAL = 	0xB2;



///**********************************
// * Sensors
// **********************************/

//// XYZ
const int DER_X = 	0xB4;
const int DER_Y = 	0xB6;
const int DER_Z = 	0xB8;

//// RPY
const int RAW_ROLL = 	0xBA;
const int RAW_PITCH = 	0xBC;
const int RAW_YAW = 	0xBE;

const int DER_ROLL = 	0xC0;
const int DER_PITCH = 	0xC2;
const int DER_YAW = 	0xC4;

////// BMP
////#define	RAW_TEMP
////#define	TEMP_ABS
////#define	RAW_PRESS
////#define	PRESS_ABS

//// GPS
const int GPS_X = 	0xC6;
const int GPS_Y = 	0xC8;
const int GPS_CNT = 	0xCA;
const int GPS_STATE = 	0xCC;
const int GPS_AVAILABLE = 	0xCE;
const int START_GPS = 	0xD0;

//// Accelerometer
const int ACC_X = 	0xD2;
const int ACC_Y = 	0xD4;
const int ACC_Z = 	0xD6;

//// Gyroscope
const int GYRO_X = 	0xD8;
const int GYRO_Y = 	0xDA;
const int GYRO_Z = 	0xDC;

const int DER_GYRO_X = 	0xDE;
const int DER_GYRO_Y = 	0xE0;

//// Magnetometer
const int MAG_X = 	0xE2;
const int MAG_Y = 	0xE4;
const int MAG_Z = 	0xE6;

//// Optical Flow
const int XP_VAL = 	0xE8;
const int YP_VAL = 	0xEA;
const int Z_RNG = 	0xEC;
const int OPT_STATE = 	0xEE;
const int Q_OF = 	0xF0;

//// State Machine
const int DESC = 	0xF2;
const int STOP = 	0xF4;
const int START = 	0xF6;
const int ARM = 	0xF8;


//#endif 		
