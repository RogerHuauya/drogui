#define ICM20948_ADDRESS 0x69
#define ICM_MAG_ADDRESS 0x0C
#define CHANGE_BANK 0x7F
#define PWR_MGT 0x06
#define AK09916_CNTL3 0x32 ///< Magnetometer

#define INT_PIN_CFG 0x0F
#define USER_CTRL 0x03
#include <Wire.h>
typedef enum {
  AK09916_MAG_DATARATE_SHUTDOWN = 0x0, ///< Stops measurement updates
  AK09916_MAG_DATARATE_SINGLE =
      0x1, ///< Takes a single measurement then switches to
           ///< AK09916_MAG_DATARATE_SHUTDOWN
  AK09916_MAG_DATARATE_10_HZ = 0x2,  ///< updates at 10Hz
  AK09916_MAG_DATARATE_20_HZ = 0x4,  ///< updates at 20Hz
  AK09916_MAG_DATARATE_50_HZ = 0x6,  ///< updates at 50Hz
  AK09916_MAG_DATARATE_100_HZ = 0x8, ///< updates at 100Hz
} ak09916_data_rate_t;

enum ICM_DATA_REGS{
    ICM_ACCEL_XOUT_H = 0x2D,
    ICM_ACCEL_XOUT_L = 0x2E,
    ICM_ACCEL_YOUT_H = 0x2F,
    ICM_ACCEL_YOUT_L = 0x30,
    ICM_ACCEL_ZOUT_H = 0x31,
    ICM_ACCEL_ZOUT_L = 0x32,

    ICM_GYRO_XOUT_H = 0x33,
    ICM_GYRO_XOUT_L = 0x34,
    ICM_GYRO_YOUT_H = 0x35,
    ICM_GYRO_YOUT_L = 0x36,
    ICM_GYRO_ZOUT_H = 0x37,
    ICM_GYRO_ZOUT_L = 0x38,
};


// Bank 2
enum ICM_CONFIGS{
    ICM_GYRO_SMPLRT_DIV = 0x00,
    ICM_GYRO_CONFIG1 = 0x01,
    ICM_GYRO_CONFIG2 = 0x02,
    ICM_ACCEL_SMPLRT_DIV_MSB = 0x10,
    ICM_ACCEL_SMPLRT_DIV_LSB = 0x11,
    ICM_ACCEL_CONFIG1 = 0x14,
    ICM_ACCEL_CONFIG2 = 0x15
};

enum ICM_GYRO_SCALE{
    ICM_GYRO_FULL_SCALE_250_DPS  = 0x00,
    ICM_GYRO_FULL_SCALE_500_DPS  = 0x02,    
    ICM_GYRO_FULL_SCALE_1000_DPS =  0x04,
    ICM_GYRO_FULL_SCALE_2000_DPS =  0x06
};

enum ICM_ACCEL_SCALE{
    ICM_ACC_FULL_SCALE_2_G  = 0x00, 
    ICM_ACC_FULL_SCALE_4_G  = 0x02,
    ICM_ACC_FULL_SCALE_8_G  = 0x04,
    ICM_ACC_FULL_SCALE_16_G = 0x06
};


typedef struct _icm20948{  
    
    float ax, ay, az; 
    float gx, gy, gz; 
    float mx, my, mz; 
    
    float raw_ax, raw_ay, raw_az;
    float raw_gx, raw_gy, raw_gz;
    float raw_mx, raw_my, raw_mz;

    float filt_ax, filt_ay, filt_az;
    float filt_gx, filt_gy, filt_gz;
    
    float off_ax, off_ay, off_az, scl_acc;
    float off_gx, off_gy, off_gz;
    float off_mx, off_my, off_mz, scl_magx, scl_magy, scl_magz;
    
    int accScale, gyroScale, magScale;
    bool isGyroCalibrated;
    bool isAccelCalibrated;
    bool isMagCalibrated;


} icm20948;

void readIcmRawAcc(icm20948* m){ // m/s^2
    uint8_t Buf[6];
    I2Cread(ICM20948_ADDRESS, ICM_ACCEL_XOUT_H, 6, Buf);
    int16_t _ax =   (Buf[0]<<8) | Buf[1];
    int16_t _ay =   (Buf[2]<<8) | Buf[3];
    int16_t _az =   (Buf[4]<<8) | Buf[5];
    
    m -> raw_ax = _ax;
    m -> raw_ay = _ay;
    m -> raw_az = _az; 
}
void initIcm(icm20948* m){
    I2CwriteByte(ICM20948_ADDRESS, CHANGE_BANK, 0);
    I2CwriteByte(ICM20948_ADDRESS, PWR_MGT, 9);
    I2CwriteByte(ICM20948_ADDRESS, CHANGE_BANK, 2<<4);
    I2CwriteByte(ICM20948_ADDRESS, ICM_GYRO_SMPLRT_DIV, 0);
    I2CwriteByte(ICM20948_ADDRESS, ICM_GYRO_CONFIG1, ICM_GYRO_FULL_SCALE_2000_DPS);
    I2CwriteByte(ICM20948_ADDRESS, ICM_ACCEL_CONFIG1,ICM_ACC_FULL_SCALE_2_G);
    I2CwriteByte(ICM20948_ADDRESS, ICM_ACCEL_SMPLRT_DIV_MSB, 0);
    I2CwriteByte(ICM20948_ADDRESS, ICM_ACCEL_SMPLRT_DIV_LSB, 0);
    I2CwriteByte(ICM20948_ADDRESS, CHANGE_BANK, 0);
    delay(1000);

    // Init magnetometer
    I2CwriteByte(ICM20948_ADDRESS, INT_PIN_CFG, 2);
    I2CwriteByte(ICM20948_ADDRESS, USER_CTRL, 0);
    I2CwriteByte(ICM_MAG_ADDRESS, AK09916_CNTL3, AK09916_MAG_DATARATE_SHUTDOWN);
    delay(100);
    I2CwriteByte(ICM_MAG_ADDRESS, AK09916_CNTL3, AK09916_MAG_DATARATE_100_HZ);
 
    m->scl_acc = 1;

}

void readIcmRawMag(icm20948* m){ // m/s^2

    uint8_t Buf[7];
    I2Cread(ICM_MAG_ADDRESS, 0x11, 7, Buf);
    int16_t _mx = -((Buf[3]<<8) | Buf[2]);
    int16_t _my = -((Buf[1]<<8) | Buf[0]);
    int16_t _mz = -((Buf[5]<<8) | Buf[4]);
    m -> raw_mx = _mx;
    m -> raw_my = _my;
    m -> raw_mz = _mz; 
}
icm20948 imu;
void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  Serial.begin(9600);
  initIcm(&imu);
}

void loop() {
  // put your main code here, to run repeatedly:
  readIcmRawMag(&imu);
  readIcmRawAcc(&imu);
  uint8_t a;
  I2Cread(ICM_MAG_ADDRESS, 0x00, 1, &a);
  Serial.print(int(a), HEX); Serial.print("\t");
  Serial.print(imu.raw_mx); Serial.print("\t");
  
  Serial.print(imu.raw_my); Serial.print("\t");
  
  Serial.print(imu.raw_mz); Serial.print("\n");
  delay(100);
}

void I2CwriteByte(uint8_t addr, uint8_t reg, uint8_t val){
  Wire.beginTransmission(addr);
  Wire.write(reg);
  Wire.write(val);
  Wire.endTransmission();
  }

void I2Cread(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buff){
  Wire.beginTransmission(addr);
  Wire.write(reg);
  Wire.endTransmission();

  Wire.requestFrom(addr, len);
  for(int i = 0; i < len; i++){
    buff[i] = Wire.read();
  }
}
