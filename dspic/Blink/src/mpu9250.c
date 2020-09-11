#include "mpu9250.h"
#include <libpic30.h>
#include <stdlib.h> 
#include <math.h>
char s[80];
// TODO: Add setter methods for this hard coded stuff
// Specify sensor full scale
uint8_t Gscale = GFS_500DPS;
uint8_t Ascale = AFS_2G;
// Choose either 14-bit or 16-bit magnetometer resolution
uint8_t Mscale = MFS_16BITS;

// 2 for 8 Hz, 6 for 100 Hz continuous magnetometer data read
uint8_t Mmode = M_8HZ;

float temperature;   // Stores the real internal chip temperature in Celsius
int16_t tempCount;   // Temperature raw count output
uint32_t delt_t = 0; // Used to control display output rate

uint32_t count = 0, sumCount = 0; // used to control display output rate
float deltat = 0.0f, sum = 0.0f;  // integration interval for both filter schemes
uint32_t lastUpdate = 0, firstUpdate = 0; // used to calculate integration interval
uint32_t Now = 0;        // used to calculate integration interval


// Factory mag calibration and mag bias
float factoryMagCalibration[3] = {0, 0, 0}, factoryMagBias[3] = {0, 0, 0};
// Bias corrections for gyro, accelerometer, and magnetometer



void initMPU9250(mpu9250 *mpu, uint8_t n, double clock_frequency ){
    initI2C(&(mpu->mpuI2C), n, MPU9250_ADDRESS_AD0, clock_frequency);
    initI2C(&(mpu->magI2C), n, AK8963_ADDRESS, clock_frequency);
    
    uint8_t c = readByteWire(&(mpu->mpuI2C), WHO_AM_I_MPU9250);
    if (c == 0x71)  serialWriteString("MPU9250 is online...\n"); // WHO_AM_I should always be 0x71
    else serialWriteString("Something is wrong ..\n");
    
    selfTestMPU9250(mpu);
    calibrateMPU9250(mpu);
    serialWriteString("Calibration finished\n");
    awakeMPU9250(mpu);
    float dest[5];
    initAK8963(mpu, dest);

    c = readByteWire(&(mpu->magI2C), WHO_AM_I_AK8963);
    if (c == 0x48)serialWriteString("MPU9250 is online...\n");
    else serialWriteString("Something is wrong ..\n");
    getAres(mpu);
    getGres(mpu);
    getMres(mpu);

}
void awakeMPU9250(mpu9250 *mpu){
  // Clear sleep mode bit (6), enable all sensors
  writeByteWire(&(mpu->mpuI2C), PWR_MGMT_1, 0x00);
  __delay_ms(100); // Wait for all registers to reset

  // Get stable time source
  // Auto select clock source to be PLL gyroscope reference if ready else
  writeByteWire(&(mpu->mpuI2C), PWR_MGMT_1, 0x01);
  __delay_ms(200);

  // Configure Gyro and Thermometer
  // Disable FSYNC and set thermometer and gyro bandwidth to 41 and 42 Hz,
  // respectively;
  // minimum delay time for this setting is 5.9 ms, which means sensor fusion
  // update rates cannot be higher than 1 / 0.0059 = 170 Hz
  // DLPF_CFG = bits 2:0 = 011; this limits the sample rate to 1000 Hz for both
  // With the MPU9250, it is possible to get gyro sample rates of 32 kHz (!),
  // 8 kHz, or 1 kHz
  writeByteWire(&(mpu->mpuI2C), CONFIG, 0x03);

  // Set sample rate = gyroscope output rate/(1 + SMPLRT_DIV)
  // Use a 200 Hz rate; a rate consistent with the filter update rate
  // determined inset in CONFIG above.
  writeByteWire(&(mpu->mpuI2C), SMPLRT_DIV, 0x04);

  // Set gyroscope full scale range
  // Range selects FS_SEL and AFS_SEL are 0 - 3, so 2-bit values are
  // left-shifted into positions 4:3

  // get current GYRO_CONFIG register value
  uint8_t c = readByteWire(&(mpu->mpuI2C), GYRO_CONFIG);
  // c = c & ~0xE0; // Clear self-test bits [7:5]
  c = c & ~0x02; // Clear Fchoice bits [1:0]
  c = c & ~0x18; // Clear AFS bits [4:3]
  c = c | Gscale << 3; // Set full scale range for the gyro
  // Set Fchoice for the gyro to 11 by writing its inverse to bits 1:0 of
  // GYRO_CONFIG
  // c =| 0x00;
  // Write new GYRO_CONFIG value to register
  writeByteWire(&(mpu->mpuI2C), GYRO_CONFIG, c );

  // Set accelerometer full-scale range configuration
  // Get current ACCEL_CONFIG register value
  c = readByteWire(&(mpu->mpuI2C), ACCEL_CONFIG);
  // c = c & ~0xE0; // Clear self-test bits [7:5]
  c = c & ~0x18;  // Clear AFS bits [4:3]
  c = c | Ascale << 3; // Set full scale range for the accelerometer
  // Write new ACCEL_CONFIG register value
  writeByteWire(&(mpu->mpuI2C), ACCEL_CONFIG, c);

  // Set accelerometer sample rate configuration
  // It is possible to get a 4 kHz sample rate from the accelerometer by
  // choosing 1 for accel_fchoice_b bit [3]; in this case the bandwidth is
  // 1.13 kHz
  // Get current ACCEL_CONFIG2 register value
  c = readByteWire(&(mpu->mpuI2C), ACCEL_CONFIG2);
  c = c & ~0x0F; // Clear accel_fchoice_b (bit 3) and A_DLPFG (bits [2:0])
  c = c | 0x03;  // Set accelerometer rate to 1 kHz and bandwidth to 41 Hz
  // Write new ACCEL_CONFIG2 register value
  writeByteWire(&(mpu->mpuI2C), ACCEL_CONFIG2, c);
  // The accelerometer, gyro, and thermometer are set to 1 kHz sample rates,
  // but all these rates are further reduced by a factor of 5 to 200 Hz because
  // of the SMPLRT_DIV setting

  // Configure Interrupts and Bypass Enable
  // Set interrupt pin active high, push-pull, hold interrupt pin level HIGH
  // until interrupt cleared, clear on read of INT_STATUS, and enable
  // I2C_BYPASS_EN so additional chips can join the I2C bus and all can be
  // controlled by the Arduino as master.
  writeByteWire(&(mpu->mpuI2C), INT_PIN_CFG, 0x22);
  // Enable data ready (bit 0) interrupt
  writeByteWire(&(mpu->mpuI2C), INT_ENABLE, 0x01);
  __delay_ms(100);

}

void getMres(mpu9250 *mpu){
  switch (Mscale)
  {
    // Possible magnetometer scales (and their register bit settings) are:
    // 14 bit resolution (0) and 16 bit resolution (1)
    case MFS_14BITS:
      mpu->mRes = 10.0f * 4912.0f / 8190.0f; // Proper scale to return milliGauss
      break;
    case MFS_16BITS:
      mpu->mRes = 10.0f * 4912.0f / 32760.0f; // Proper scale to return milliGauss
      break;
  }
}

void getGres(mpu9250 *mpu)
{
  switch (Gscale)
  {
    // Possible gyro scales (and their register bit settings) are:
    // 250 DPS (00), 500 DPS (01), 1000 DPS (10), and 2000 DPS (11).
    // Here's a bit of an algorith to calculate DPS/(ADC tick) based on that
    // 2-bit value:
    case GFS_250DPS:
      mpu->gRes = 250.0f / 32768.0f;
      break;
    case GFS_500DPS:
      mpu->gRes = 500.0f / 32768.0f;
      break;
    case GFS_1000DPS:
      mpu->gRes = 1000.0f / 32768.0f;
      break;
    case GFS_2000DPS:
      mpu->gRes = 2000.0f / 32768.0f;
      break;
  }
}

void getAres(mpu9250 *mpu)
{
  switch (Ascale)
  {
    // Possible accelerometer scales (and their register bit settings) are:
    // 2 Gs (00), 4 Gs (01), 8 Gs (10), and 16 Gs  (11).
    // Here's a bit of an algorith to calculate DPS/(ADC tick) based on that
    // 2-bit value:
    case AFS_2G:
      mpu->aRes = 2.0f / 32768.0f;
      break;
    case AFS_4G:
      mpu->aRes = 4.0f / 32768.0f;
      break;
    case AFS_8G:
      mpu->aRes = 8.0f / 32768.0f;
      break;
    case AFS_16G:
      mpu->aRes = 16.0f / 32768.0f;
      break;
  }
}


void readAccelData(mpu9250 * mpu )
{
  uint8_t rawData[6];  // x/y/z accel register data stored here
  // Read the six raw data registers into data array
  readBytesWire(&(mpu->mpuI2C), ACCEL_XOUT_H, 6, &rawData[0]);

  // Turn the MSB and LSB into a signed 16-bit value
  mpu->accelCount[0] = ((int16_t)rawData[0] << 8) | rawData[1] ;
  mpu->accelCount[1] = ((int16_t)rawData[2] << 8) | rawData[3] ;
  mpu->accelCount[2] = ((int16_t)rawData[4] << 8) | rawData[5] ;
}

void readGyroData(mpu9250 * mpu)
{
  uint8_t rawData[6];  // x/y/z gyro register data stored here
  // Read the six raw data registers sequentially into data array
  readBytesWire(&(mpu->mpuI2C), GYRO_XOUT_H, 6, &rawData[0]);

  // Turn the MSB and LSB into a signed 16-bit value
  mpu->gyroCount[0] = ((int16_t)rawData[0] << 8) | rawData[1] ;
  mpu->gyroCount[1] = ((int16_t)rawData[2] << 8) | rawData[3] ;
  mpu->gyroCount[2] = ((int16_t)rawData[4] << 8) | rawData[5] ;
}

void readMagData(mpu9250 * mpu)
{
  // x/y/z gyro register data, ST2 register stored here, must read ST2 at end
  // of data acquisition
  uint8_t rawData[7];
  // Wait for magnetometer data ready bit to be set
  if (readByteWire(&(mpu->magI2C), AK8963_ST1) & 0x01)
  {
    // Read the six raw data and ST2 registers sequentially into data array
    readBytesWire(&(mpu->magI2C), AK8963_XOUT_L, 7, &rawData[0]);
    uint8_t c = rawData[6]; // End data read by reading ST2 register
    // Check if magnetic sensor overflow set, if not then report data
    if (!(c & 0x08))
    {
      // Turn the MSB and LSB into a signed 16-bit value
      // Data stored as little Endian
      mpu->magCount[0] = ((int16_t)rawData[1] << 8) | rawData[0];
      mpu->magCount[1] = ((int16_t)rawData[3] << 8) | rawData[2];
      mpu->magCount[2] = ((int16_t)rawData[5] << 8) | rawData[4];
    }
  }
}

void readAll(mpu9250 * mpu){
  readAccelData(mpu);
  readGyroData(mpu);
  readMagData(mpu);

  mpu->ax = mpu->accelCount[0] * mpu->aRes;
  mpu->ay = mpu->accelCount[1] * mpu->aRes;
  mpu->az = mpu->accelCount[2] * mpu->aRes;

  mpu->gx = mpu->gyroCount[0] * mpu->gRes;
  mpu->gy = mpu->gyroCount[1] * mpu->gRes;
  mpu->gz = mpu->gyroCount[2] * mpu->gRes;

  mpu->mx = mpu->magCount[0] * mpu->mRes;
  mpu->my = mpu->magCount[1] * mpu->mRes;
  mpu->mz = mpu->magCount[2] * mpu->mRes;
}

void printIMU(mpu9250 * mpu){
  sprintf(s, "%.3f %.3f %.3f %.3f %.3f %.3f %.3f %.3f %.3f \n", mpu->ax, mpu->ay, mpu->az, mpu->gx, mpu->gy, mpu->gz, mpu->mx, mpu->my, mpu->mz);
  serialWriteString(s);
}
int16_t readTempData(mpu9250 * mpu){
  uint8_t rawData[2]; // x/y/z gyro register data stored here
  // Read the two raw data registers sequentially into data array
  readBytesWire(&(mpu->mpuI2C), TEMP_OUT_H, 2, &rawData[0]);
  // Turn the MSB and LSB into a 16-bit value
  return ((int16_t)rawData[0] << 8) | rawData[1];
}


void initAK8963(mpu9250 * mpu, float * destination)
{
  // First extract the factory calibration for each magnetometer axis
  uint8_t rawData[3];  // x/y/z gyro calibration data stored here
  // TODO: Test this!! Likely doesn't work
  writeByteWire(&(mpu->magI2C), AK8963_CNTL, 0x00); // Power down magnetometer
  __delay_ms(10);
  writeByteWire(&(mpu->magI2C), AK8963_CNTL, 0x0F); // Enter Fuse ROM access mode
  __delay_ms(10);

  // Read the x-, y-, and z-axis calibration values
  readBytesWire(&(mpu->magI2C), AK8963_ASAX, 3, &rawData[0]);

  // Return x-axis sensitivity adjustment values, etc.
  destination[0] =  (float)(rawData[0] - 128)/256. + 1.;
  destination[1] =  (float)(rawData[1] - 128)/256. + 1.;
  destination[2] =  (float)(rawData[2] - 128)/256. + 1.;
  writeByteWire(&(mpu->magI2C), AK8963_CNTL, 0x00); // Power down magnetometer
  __delay_ms(10);

  // Configure the magnetometer for continuous read and highest resolution.
  // Set Mscale bit 4 to 1 (0) to enable 16 (14) bit resolution in CNTL
  // register, and enable continuous mode data acquisition Mmode (bits [3:0]),
  // 0010 for 8 Hz and 0110 for 100 Hz sample rates.

  // Set magnetometer data resolution and sample ODR
  writeByteWire(&(mpu->magI2C), AK8963_CNTL, Mscale << 4 | Mmode);
  __delay_ms(10);

}


// Function which accumulates gyro and accelerometer data after device
// initialization. It calculates the average of the at-rest readings and then
// loads the resulting offsets into accelerometer and gyro bias registers.
//void calibrateMPU9250(mpu9250 * mpu, float * gyroBias, float * accelBias)
void calibrateMPU9250(mpu9250 * mpu)
{
  uint8_t data[12]; // data array to hold accelerometer and gyro x, y, z, data
  uint16_t ii, packet_count, fifo_count;
  int32_t gyro_bias[3]  = {0, 0, 0}, accel_bias[3] = {0, 0, 0};

  // reset device
  // Write a one to bit 7 reset bit; toggle reset device
  writeByteWire(&(mpu->mpuI2C), PWR_MGMT_1, READ_FLAG);
  __delay_ms(100);

  // get stable time source; Auto select clock source to be PLL gyroscope
  // reference if ready else use the internal oscillator, bits 2:0 = 001
  writeByteWire(&(mpu->mpuI2C), PWR_MGMT_1, 0x01);
  writeByteWire(&(mpu->mpuI2C), PWR_MGMT_2, 0x00);
  __delay_ms(200);

  // Configure device for bias calculation
  // Disable all interrupts
  writeByteWire(&(mpu->mpuI2C), INT_ENABLE, 0x00);
  // Disable FIFO
  writeByteWire(&(mpu->mpuI2C), FIFO_EN, 0x00);
  // Turn on internal clock source
  writeByteWire(&(mpu->mpuI2C), PWR_MGMT_1, 0x00);
  // Disable I2C master
  writeByteWire(&(mpu->mpuI2C), I2C_MST_CTRL, 0x00);
  // Disable FIFO and I2C master modes
  writeByteWire(&(mpu->mpuI2C), USER_CTRL, 0x00);
  // Reset FIFO and DMP
  writeByteWire(&(mpu->mpuI2C), USER_CTRL, 0x0C);
  __delay_ms(15);

  // Configure MPU6050 gyro and accelerometer for bias calculation
  // Set low-pass filter to 188 Hz
  writeByteWire(&(mpu->mpuI2C), CONFIG, 0x01);
  // Set sample rate to 1 kHz
  writeByteWire(&(mpu->mpuI2C), SMPLRT_DIV, 0x00);
  // Set gyro full-scale to 250 degrees per second, maximum sensitivity
  writeByteWire(&(mpu->mpuI2C), GYRO_CONFIG, 0x00);
  // Set accelerometer full-scale to 2 g, maximum sensitivity
  writeByteWire(&(mpu->mpuI2C), ACCEL_CONFIG, 0x00);

  uint16_t  gyrosensitivity  = 131;   // = 131 LSB/degrees/sec
  uint16_t  accelsensitivity = 16384; // = 16384 LSB/g

  // Configure FIFO to capture accelerometer and gyro data for bias calculation
  writeByteWire(&(mpu->mpuI2C), USER_CTRL, 0x40);  // Enable FIFO
  // Enable gyro and accelerometer sensors for FIFO  (max size 512 bytes in
  // MPU-9150)
  writeByteWire(&(mpu->mpuI2C), FIFO_EN, 0x78);
  __delay_ms(40);  // accumulate 40 samples in 40 milliseconds = 480 bytes

  // At end of sample accumulation, turn off FIFO sensor read
  // Disable gyro and accelerometer sensors for FIFO
  writeByteWire(&(mpu->mpuI2C), FIFO_EN, 0x00);
  // Read FIFO sample count
  readBytesWire(&(mpu->mpuI2C), FIFO_COUNTH, 2, &data[0]);
  fifo_count = ((uint16_t)data[0] << 8) | data[1];

  // How many sets of full gyro and accelerometer data for averaging
  packet_count = fifo_count/12;

  for (ii = 0; ii < packet_count; ii++)
  {
    int16_t accel_temp[3] = {0, 0, 0}, gyro_temp[3] = {0, 0, 0};
    // Read data for averaging
    readBytesWire(&(mpu->mpuI2C), FIFO_R_W, 12, &data[0]);
    // Form signed 16-bit integer for each sample in FIFO
    accel_temp[0] = (int16_t) (((int16_t)data[0] << 8) | data[1]  );
    accel_temp[1] = (int16_t) (((int16_t)data[2] << 8) | data[3]  );
    accel_temp[2] = (int16_t) (((int16_t)data[4] << 8) | data[5]  );
    gyro_temp[0]  = (int16_t) (((int16_t)data[6] << 8) | data[7]  );
    gyro_temp[1]  = (int16_t) (((int16_t)data[8] << 8) | data[9]  );
    gyro_temp[2]  = (int16_t) (((int16_t)data[10] << 8) | data[11]);

    // Sum individual signed 16-bit biases to get accumulated signed 32-bit
    // biases.
    accel_bias[0] += (int32_t) accel_temp[0];
    accel_bias[1] += (int32_t) accel_temp[1];
    accel_bias[2] += (int32_t) accel_temp[2];
    gyro_bias[0]  += (int32_t) gyro_temp[0];
    gyro_bias[1]  += (int32_t) gyro_temp[1];
    gyro_bias[2]  += (int32_t) gyro_temp[2];
  }
  // Sum individual signed 16-bit biases to get accumulated signed 32-bit biases
  accel_bias[0] /= (int32_t) packet_count;
  accel_bias[1] /= (int32_t) packet_count;
  accel_bias[2] /= (int32_t) packet_count;
  gyro_bias[0]  /= (int32_t) packet_count;
  gyro_bias[1]  /= (int32_t) packet_count;
  gyro_bias[2]  /= (int32_t) packet_count;

  // Sum individual signed 16-bit biases to get accumulated signed 32-bit biases
  if (accel_bias[2] > 0L)
  {
    accel_bias[2] -= (int32_t) accelsensitivity;
  }
  else
  {
    accel_bias[2] += (int32_t) accelsensitivity;
  }

  // Construct the gyro biases for push to the hardware gyro bias registers,
  // which are reset to zero upon device startup.
  // Divide by 4 to get 32.9 LSB per deg/s to conform to expected bias input
  // format.
  data[0] = (-gyro_bias[0]/4  >> 8) & 0xFF;
  // Biases are additive, so change sign on calculated average gyro biases
  data[1] = (-gyro_bias[0]/4)       & 0xFF;
  data[2] = (-gyro_bias[1]/4  >> 8) & 0xFF;
  data[3] = (-gyro_bias[1]/4)       & 0xFF;
  data[4] = (-gyro_bias[2]/4  >> 8) & 0xFF;
  data[5] = (-gyro_bias[2]/4)       & 0xFF;

  // Push gyro biases to hardware registers
  writeByteWire(&(mpu->mpuI2C), XG_OFFSET_H, data[0]);
  writeByteWire(&(mpu->mpuI2C), XG_OFFSET_L, data[1]);
  writeByteWire(&(mpu->mpuI2C), YG_OFFSET_H, data[2]);
  writeByteWire(&(mpu->mpuI2C), YG_OFFSET_L, data[3]);
  writeByteWire(&(mpu->mpuI2C), ZG_OFFSET_H, data[4]);
  writeByteWire(&(mpu->mpuI2C), ZG_OFFSET_L, data[5]);

  // Output scaled gyro biases for display in the main program
  mpu->accelBias[0] = (float) gyro_bias[0]/(float) gyrosensitivity;
  mpu->accelBias[1] = (float) gyro_bias[1]/(float) gyrosensitivity;
  mpu->accelBias[2] = (float) gyro_bias[2]/(float) gyrosensitivity;

  // Construct the accelerometer biases for push to the hardware accelerometer
  // bias registers. These registers contain factory trim values which must be
  // added to the calculated accelerometer biases; on boot up these registers
  // will hold non-zero values. In addition, bit 0 of the lower byte must be
  // preserved since it is used for temperature compensation calculations.
  // Accelerometer bias registers expect bias input as 2048 LSB per g, so that
  // the accelerometer biases calculated above must be divided by 8.

  // A place to hold the factory accelerometer trim biases
  int32_t accel_bias_reg[3] = {0, 0, 0};
  // Read factory accelerometer trim values
  readBytesWire(&(mpu->mpuI2C), XA_OFFSET_H, 2, &data[0]);
  accel_bias_reg[0] = (int32_t) (((int16_t)data[0] << 8) | data[1]);
  readBytesWire(&(mpu->mpuI2C), YA_OFFSET_H, 2, &data[0]);
  accel_bias_reg[1] = (int32_t) (((int16_t)data[0] << 8) | data[1]);
  readBytesWire(&(mpu->mpuI2C), ZA_OFFSET_H, 2, &data[0]);
  accel_bias_reg[2] = (int32_t) (((int16_t)data[0] << 8) | data[1]);

  // Define mask for temperature compensation bit 0 of lower byte of
  // accelerometer bias registers
  uint32_t mask = 1uL;
  // Define array to hold mask bit for each accelerometer bias axis
  uint8_t mask_bit[3] = {0, 0, 0};

  for (ii = 0; ii < 3; ii++)
  {
    // If temperature compensation bit is set, record that fact in mask_bit
    if ((accel_bias_reg[ii] & mask))
    {
      mask_bit[ii] = 0x01;
    }
  }

  // Construct total accelerometer bias, including calculated average
  // accelerometer bias from above
  // Subtract calculated averaged accelerometer bias scaled to 2048 LSB/g
  // (16 g full scale)
  accel_bias_reg[0] -= (accel_bias[0]/8);
  accel_bias_reg[1] -= (accel_bias[1]/8);
  accel_bias_reg[2] -= (accel_bias[2]/8);

  data[0] = (accel_bias_reg[0] >> 8) & 0xFF;
  data[1] = (accel_bias_reg[0])      & 0xFF;
  // preserve temperature compensation bit when writing back to accelerometer
  // bias registers
  data[1] = data[1] | mask_bit[0];
  data[2] = (accel_bias_reg[1] >> 8) & 0xFF;
  data[3] = (accel_bias_reg[1])      & 0xFF;
  // Preserve temperature compensation bit when writing back to accelerometer
  // bias registers
  data[3] = data[3] | mask_bit[1];
  data[4] = (accel_bias_reg[2] >> 8) & 0xFF;
  data[5] = (accel_bias_reg[2])      & 0xFF;
  // Preserve temperature compensation bit when writing back to accelerometer
  // bias registers
  data[5] = data[5] | mask_bit[2];

  // Apparently this is not working for the acceleration biases in the MPU-9250
  // Are we handling the temperature correction bit properly?
  // Push accelerometer biases to hardware registers
  writeByteWire(&(mpu->mpuI2C), XA_OFFSET_H, data[0]);
  writeByteWire(&(mpu->mpuI2C), XA_OFFSET_L, data[1]);
  writeByteWire(&(mpu->mpuI2C), YA_OFFSET_H, data[2]);
  writeByteWire(&(mpu->mpuI2C), YA_OFFSET_L, data[3]);
  writeByteWire(&(mpu->mpuI2C), ZA_OFFSET_H, data[4]);
  writeByteWire(&(mpu->mpuI2C), ZA_OFFSET_L, data[5]);

  // Output scaled accelerometer biases for display in the main program
  mpu->accelBias[0] = (float)accel_bias[0]/(float)accelsensitivity;
  mpu->accelBias[1] = (float)accel_bias[1]/(float)accelsensitivity;
  mpu->accelBias[2] = (float)accel_bias[2]/(float)accelsensitivity;
}


// Accelerometer and gyroscope self test; check calibration wrt factory settings
// Should return percent deviation from factory trim values, +/- 14 or less
// deviation is a pass.
void selfTestMPU9250(mpu9250 * mpu)
{
  uint8_t rawData[6] = {0, 0, 0, 0, 0, 0};
  uint8_t selfTest[6];
  int32_t gAvg[3] = {0}, aAvg[3] = {0}, aSTAvg[3] = {0}, gSTAvg[3] = {0};
  float factoryTrim[6];
  uint8_t FS = GFS_250DPS;
   
  writeByteWire(&(mpu->mpuI2C), SMPLRT_DIV, 0x00);    // Set gyro sample rate to 1 kHz
  writeByteWire(&(mpu->mpuI2C), CONFIG, 0x02);        // Set gyro sample rate to 1 kHz and DLPF to 92 Hz
  writeByteWire(&(mpu->mpuI2C), GYRO_CONFIG, FS<<3);  // Set full scale range for the gyro to 250 dps
  writeByteWire(&(mpu->mpuI2C), ACCEL_CONFIG2, 0x02); // Set accelerometer rate to 1 kHz and bandwidth to 92 Hz
  writeByteWire(&(mpu->mpuI2C), ACCEL_CONFIG, FS<<3); // Set full scale range for the accelerometer to 2 g

  for( int ii = 0; ii < 200; ii++) {  // get average current values of gyro and acclerometer
  
    readBytesWire(&(mpu->mpuI2C), ACCEL_XOUT_H, 6, &rawData[0]);        // Read the six raw data registers into data array
    aAvg[0] += (int16_t)(((int16_t)rawData[0] << 8) | rawData[1]) ;  // Turn the MSB and LSB into a signed 16-bit value
    aAvg[1] += (int16_t)(((int16_t)rawData[2] << 8) | rawData[3]) ;  
    aAvg[2] += (int16_t)(((int16_t)rawData[4] << 8) | rawData[5]) ; 

    readBytesWire(&(mpu->mpuI2C), GYRO_XOUT_H, 6, &rawData[0]);       // Read the six raw data registers sequentially into data array
    gAvg[0] += (int16_t)(((int16_t)rawData[0] << 8) | rawData[1]) ;  // Turn the MSB and LSB into a signed 16-bit value
    gAvg[1] += (int16_t)(((int16_t)rawData[2] << 8) | rawData[3]) ;  
    gAvg[2] += (int16_t)(((int16_t)rawData[4] << 8) | rawData[5]) ; 

  }

  // Get average of 200 values and store as average current readings
  for (int ii =0; ii < 3; ii++){
    aAvg[ii] /= 200;
    gAvg[ii] /= 200;
  }
  // Configure the accelerometer for self-test
  // Enable self test on all three axes and set accelerometer range to +/- 2 g
  writeByteWire(&(mpu->mpuI2C), ACCEL_CONFIG, 0xE0);
  // Enable self test on all three axes and set gyro range to +/- 250 degrees/s
  writeByteWire(&(mpu->mpuI2C), GYRO_CONFIG,  0xE0);
  __delay_ms(25);  // Delay a while to let the device stabilize

  // Get average self-test values of gyro and acclerometer
  for (int ii = 0; ii < 200; ii++)
  {
    // Read the six raw data registers into data array
    readBytesWire(&(mpu->mpuI2C), ACCEL_XOUT_H, 6, &rawData[0]);
    // Turn the MSB and LSB into a signed 16-bit value
    aSTAvg[0] += (int16_t)(((int16_t)rawData[0] << 8) | rawData[1]) ;
    aSTAvg[1] += (int16_t)(((int16_t)rawData[2] << 8) | rawData[3]) ;
    aSTAvg[2] += (int16_t)(((int16_t)rawData[4] << 8) | rawData[5]) ;

    // Read the six raw data registers sequentially into data array
    readBytesWire(&(mpu->mpuI2C), GYRO_XOUT_H, 6, &rawData[0]);
    // Turn the MSB and LSB into a signed 16-bit value
    gSTAvg[0] += (int16_t)(((int16_t)rawData[0] << 8) | rawData[1]) ;
    gSTAvg[1] += (int16_t)(((int16_t)rawData[2] << 8) | rawData[3]) ;
    gSTAvg[2] += (int16_t)(((int16_t)rawData[4] << 8) | rawData[5]) ;
  }

  // Get average of 200 values and store as average self-test readings
  for (int ii =0; ii < 3; ii++)
  {
    aSTAvg[ii] /= 200;
    gSTAvg[ii] /= 200;
  }

  // Configure the gyro and accelerometer for normal operation
  writeByteWire(&(mpu->mpuI2C), ACCEL_CONFIG, 0x00);
  writeByteWire(&(mpu->mpuI2C), GYRO_CONFIG,  0x00);
  __delay_ms(25);  // Delay a while to let the device stabilize

  // Retrieve accelerometer and gyro factory Self-Test Code from USR_Reg
  // X-axis accel self-test results
  selfTest[0] = readByteWire(&(mpu->mpuI2C), SELF_TEST_X_ACCEL);
  // Y-axis accel self-test results
  selfTest[1] = readByteWire(&(mpu->mpuI2C), SELF_TEST_Y_ACCEL);
  // Z-axis accel self-test results
  selfTest[2] = readByteWire(&(mpu->mpuI2C), SELF_TEST_Z_ACCEL);
  // X-axis gyro self-test results
  selfTest[3] = readByteWire(&(mpu->mpuI2C), SELF_TEST_X_GYRO);
  // Y-axis gyro self-test results
  selfTest[4] = readByteWire(&(mpu->mpuI2C), SELF_TEST_Y_GYRO);
  // Z-axis gyro self-test results
  selfTest[5] = readByteWire(&(mpu->mpuI2C), SELF_TEST_Z_GYRO);

  // Retrieve factory self-test value from self-test code reads
  // FT[Xa] factory trim calculation
  factoryTrim[0] = (float)(2620/1<<FS)*(pow(1.01 ,((float)selfTest[0] - 1.0) ));
  // FT[Ya] factory trim calculation
  factoryTrim[1] = (float)(2620/1<<FS)*(pow(1.01 ,((float)selfTest[1] - 1.0) ));
  // FT[Za] factory trim calculation
  factoryTrim[2] = (float)(2620/1<<FS)*(pow(1.01 ,((float)selfTest[2] - 1.0) ));
  // FT[Xg] factory trim calculation
  factoryTrim[3] = (float)(2620/1<<FS)*(pow(1.01 ,((float)selfTest[3] - 1.0) ));
  // FT[Yg] factory trim calculation
  factoryTrim[4] = (float)(2620/1<<FS)*(pow(1.01 ,((float)selfTest[4] - 1.0) ));
  // FT[Zg] factory trim calculation
  factoryTrim[5] = (float)(2620/1<<FS)*(pow(1.01 ,((float)selfTest[5] - 1.0) ));

  // Report results as a ratio of (STR - FT)/FT; the change from Factory Trim
  // of the Self-Test Response
  // To get percent, must multiply by 100
  for (int i = 0; i < 3; i++)
  {
    // Report percent differences
    
    mpu->selfTest[i] = 100.0 * ((float)(aSTAvg[i] - aAvg[i])) / factoryTrim[i]- 100.;
    // Report percent differences
    mpu->selfTest[i+3] = 100.0*((float)(gSTAvg[i] - gAvg[i])) / factoryTrim[i+3]- 100.;
    //sprintf(s,"%.3f")
  }
}

// Function which accumulates magnetometer data after device initialization.
// It calculates the bias and scale in the x, y, and z axes.
void magCalMPU9250(mpu9250 * mpu, float * bias_dest, float * scale_dest)

{
  uint16_t ii = 0, sample_count = 0;
  int32_t mag_bias[3]  = {0, 0, 0},
          mag_scale[3] = {0, 0, 0};
  int16_t mag_max[3]  = {-32768, -32768, -32768},		// Wrote out decimal (signed) values to remove a conversion warning
          mag_min[3]  = {32767, 32767, 32767};
  // Make sure resolution has been calculated
  getMres(mpu);

  serialWriteString("Mag Calibration: Wave device in a figure 8 until done!");
  serialWriteString("  4 seconds to get ready followed by 15 seconds of sampling)");
  __delay_ms(4000);

  // shoot for ~fifteen seconds of mag data
  // at 8 Hz ODR, new mag data is available every 125 ms
  if (Mmode == M_8HZ)
  {
    sample_count = 128;
  }
  // at 100 Hz ODR, new mag data is available every 10 ms
  if (Mmode == M_100HZ)
  {
    sample_count = 1500;
  }

  for (ii = 0; ii < sample_count; ii++)
  {
    readMagData(mpu);  // Read the mag data

    for (int jj = 0; jj < 3; jj++)
    {
      if (mpu->magCount[jj] > mag_max[jj])
      {
        mag_max[jj] = mpu->magCount[jj];
      }
      if (mpu->magCount[jj] < mag_min[jj])
      {
        mag_min[jj] = mpu->magCount[jj];
      }
    }

    if (Mmode == M_8HZ)
    {
      __delay_ms(135); // At 8 Hz ODR, new mag data is available every 125 ms
    }
    if (Mmode == M_100HZ)
    {
      __delay_ms(12);  // At 100 Hz ODR, new mag data is available every 10 ms
    }
  }

  // serialWriteString("mag x min/max:"); serialWriteString(mag_max[0]); serialWriteString(mag_min[0]);
  // serialWriteString("mag y min/max:"); serialWriteString(mag_max[1]); serialWriteString(mag_min[1]);
  // serialWriteString("mag z min/max:"); serialWriteString(mag_max[2]); serialWriteString(mag_min[2]);

  // Get hard iron correction
  // Get 'average' x mag bias in counts
  mag_bias[0]  = (mag_max[0] + mag_min[0]) / 2;
  // Get 'average' y mag bias in counts
  mag_bias[1]  = (mag_max[1] + mag_min[1]) / 2;
  // Get 'average' z mag bias in counts
  mag_bias[2]  = (mag_max[2] + mag_min[2]) / 2;

  // Save mag biases in G for main program
  bias_dest[0] = (float)mag_bias[0] * mpu->mRes * factoryMagCalibration[0];
  bias_dest[1] = (float)mag_bias[1] * mpu->mRes * factoryMagCalibration[1];
  bias_dest[2] = (float)mag_bias[2] * mpu->mRes * factoryMagCalibration[2];

  // Get soft iron correction estimate
  // Get average x axis max chord length in counts
  mag_scale[0]  = (mag_max[0] - mag_min[0]) / 2;
  // Get average y axis max chord length in counts
  mag_scale[1]  = (mag_max[1] - mag_min[1]) / 2;
  // Get average z axis max chord length in counts
  mag_scale[2]  = (mag_max[2] - mag_min[2]) / 2;

  float avg_rad = mag_scale[0] + mag_scale[1] + mag_scale[2];
  avg_rad /= 3.0;

  scale_dest[0] = avg_rad / ((float)mag_scale[0]);
  scale_dest[1] = avg_rad / ((float)mag_scale[1]);
  scale_dest[2] = avg_rad / ((float)mag_scale[2]);

  serialWriteString("Mag Calibration done!");
}


uint8_t writeByteWire(i2c * device, uint8_t registerAddress, uint8_t data)
{
  	i2cStart(device);  	// Initialize the Tx buffer
    i2cStartWrite(device);
    i2cWrite(device, registerAddress);
    i2cWrite(device, data);
    i2cStop(device);
  	return 0;
}



uint8_t readByteWire(i2c * device, uint8_t registerAddress)
{
    uint8_t data; // `data` will store the register data
    i2cStart(device);  	// Initialize the Tx buffer
    
    i2cStartWrite(device);
    
    i2cWrite(device, registerAddress);
    
    i2cRestart(device);
    
    i2cStartRead(device);
    
    data =  i2cRead(device); i2cSendNACK(device);
    
    i2cStop(device);
    return data;
}

uint8_t readBytesWire(i2c * device, uint8_t registerAddress,  uint8_t count, uint8_t * dest)
{ 
    i2cStart(device);  	
    i2cStartWrite(device);
    i2cWrite(device, registerAddress); 
    i2cRestart(device);
    i2cStartRead(device);

    uint8_t i = 0;
    for(i = 0; i < count-1; i++){
        dest[i] = i2cRead(device); i2cSendACK(device); idleI2C(device);
    }
    dest[count-1] = i2cRead(device); i2cSendNACK(device);
    i2cStop(device);
    return i; 
}


