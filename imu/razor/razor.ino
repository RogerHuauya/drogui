/* MPU9250 Basic Example Code
 by: Kris Winer
 date: April 1, 2014
 license: Beerware - Use this code however you'd like. If you
 find it useful you can buy me a beer some time.
 Modified by Brent Wilkins July 19, 2016

 Demonstrate basic MPU-9250 functionality including parameterizing the register
 addresses, initializing the sensor, getting properly scaled accelerometer,
 gyroscope, and magnetometer data out. Added display functions to allow display
 to on breadboard monitor. Addition of 9 DoF sensor fusion using open source
 Madgwick and Mahony filter algorithms. Sketch runs on the 3.3 V 8 MHz Pro Mini
 and the Teensy 3.1.

 SDA and SCL should have external pull-up resistors (to 3.3V).
 10k resistors are on the EMSENSR-9250 breakout board.

 Hardware setup:
 MPU9250 Breakout --------- Arduino
 VDD ---------------------- 3.3V
 VDDI --------------------- 3.3V
 SDA ----------------------- A4
 SCL ----------------------- A5
 GND ---------------------- GND
 */

#include "quaternionFilters.h"
#include "MPU9250.h"

#define SerialPort SerialUSB

#define AHRS false         // Set to false for basic data read
#define SerialDebug true  // Set to true to get Serial output for debugging

// Pin definitions
int intPin = 12;  // These can be changed, 2 and 3 are the Arduinos ext int pins
int myLed  = 13;  // Set up pin 13 led for toggling

#define I2Cclock 400000
#define I2Cport Wire
//#define MPU9250_ADDRESS MPU9250_ADDRESS_AD0   // Use either this line or the next to select which I2C address your device is using
#define MPU9250_ADDRESS MPU9250_ADDRESS_AD1

MPU9250 myIMU(MPU9250_ADDRESS, I2Cport, I2Cclock);

void setup()
{
  Wire.begin();
  // TWBR = 12;  // 400 kbit/sec I2C speed
  SerialPort.begin(38400);

  while(!Serial){};

  // Set up the interrupt pin, its set as active high, push-pull
  pinMode(intPin, INPUT);
  digitalWrite(intPin, LOW);
  pinMode(myLed, OUTPUT);
  digitalWrite(myLed, HIGH);

  // Read the WHO_AM_I register, this is a good test of communication
  byte c = myIMU.readByte(MPU9250_ADDRESS, WHO_AM_I_MPU9250);
  SerialPort.print(F("MPU9250 I AM 0x"));
  SerialPort.print(c, HEX);
  SerialPort.print(F(" I should be 0x"));
  SerialPort.println(0x71, HEX);

  if (c == 0x71) // WHO_AM_I should always be 0x71
  {
    SerialPort.println(F("MPU9250 is online..."));

    // Start by performing self test and reporting values
    myIMU.MPU9250SelfTest(myIMU.selfTest);
    SerialPort.print(F("x-axis self test: acceleration trim within : "));
    SerialPort.print(myIMU.selfTest[0],1); SerialPort.println("% of factory value");
    SerialPort.print(F("y-axis self test: acceleration trim within : "));
    SerialPort.print(myIMU.selfTest[1],1); SerialPort.println("% of factory value");
    SerialPort.print(F("z-axis self test: acceleration trim within : "));
    SerialPort.print(myIMU.selfTest[2],1); SerialPort.println("% of factory value");
    SerialPort.print(F("x-axis self test: gyration trim within : "));
    SerialPort.print(myIMU.selfTest[3],1); SerialPort.println("% of factory value");
    SerialPort.print(F("y-axis self test: gyration trim within : "));
    SerialPort.print(myIMU.selfTest[4],1); SerialPort.println("% of factory value");
    SerialPort.print(F("z-axis self test: gyration trim within : "));
    SerialPort.print(myIMU.selfTest[5],1); SerialPort.println("% of factory value");

    // Calibrate gyro and accelerometers, load biases in bias registers
    myIMU.calibrateMPU9250(myIMU.gyroBias, myIMU.accelBias);


    myIMU.initMPU9250();
    // Initialize device for active mode read of acclerometer, gyroscope, and
    // temperature
    SerialPort.println("MPU9250 initialized for active data mode....");

    // Read the WHO_AM_I register of the magnetometer, this is a good test of
    // communication
    byte d = myIMU.readByte(AK8963_ADDRESS, WHO_AM_I_AK8963);
    SerialPort.print("AK8963 ");
    SerialPort.print("I AM 0x");
    SerialPort.print(d, HEX);
    SerialPort.print(" I should be 0x");
    SerialPort.println(0x48, HEX);


    if (d != 0x48)
    {
      // Communication failed, stop here
      SerialPort.println(F("Communication failed, abort!"));
      SerialPort.flush();
      abort();
    }

    // Get magnetometer calibration from AK8963 ROM
    myIMU.initAK8963(myIMU.factoryMagCalibration);
    // Initialize device for active mode read of magnetometer
    SerialPort.println("AK8963 initialized for active data mode....");

    if (SerialDebug)
    {
      //  SerialPort.println("Calibration values: ");
      SerialPort.print("X-Axis factory sensitivity adjustment value ");
      SerialPort.println(myIMU.factoryMagCalibration[0], 2);
      SerialPort.print("Y-Axis factory sensitivity adjustment value ");
      SerialPort.println(myIMU.factoryMagCalibration[1], 2);
      SerialPort.print("Z-Axis factory sensitivity adjustment value ");
      SerialPort.println(myIMU.factoryMagCalibration[2], 2);
    }


    // Get sensor resolutions, only need to do this once
    myIMU.getAres();
    myIMU.getGres();
    myIMU.getMres();

    // The next call delays for 4 seconds, and then records about 15 seconds of
    // data to calculate bias and scale.
//    myIMU.magCalMPU9250(myIMU.magBias, myIMU.magScale);
    SerialPort.println("AK8963 mag biases (mG)");
    SerialPort.println(myIMU.magBias[0]);
    SerialPort.println(myIMU.magBias[1]);
    SerialPort.println(myIMU.magBias[2]);

    SerialPort.println("AK8963 mag scale (mG)");
    SerialPort.println(myIMU.magScale[0]);
    SerialPort.println(myIMU.magScale[1]);
    SerialPort.println(myIMU.magScale[2]);
//    delay(2000); // Add delay to see results before serial spew of data

    if(SerialDebug)
    {
      SerialPort.println("Magnetometer:");
      SerialPort.print("X-Axis sensitivity adjustment value ");
      SerialPort.println(myIMU.factoryMagCalibration[0], 2);
      SerialPort.print("Y-Axis sensitivity adjustment value ");
      SerialPort.println(myIMU.factoryMagCalibration[1], 2);
      SerialPort.print("Z-Axis sensitivity adjustment value ");
      SerialPort.println(myIMU.factoryMagCalibration[2], 2);
    }

  } // if (c == 0x71)
  else
  {
    SerialPort.print("Could not connect to MPU9250: 0x");
    SerialPort.println(c, HEX);

    // Communication failed, stop here
    SerialPort.println(F("Communication failed, abort!"));
    SerialPort.flush();
    abort();
  }
}

void loop()
{
  // If intPin goes high, all data registers have new data
  // On interrupt, check if data ready interrupt
  if (myIMU.readByte(MPU9250_ADDRESS, INT_STATUS) & 0x01)
  {
    myIMU.readAccelData(myIMU.accelCount);  // Read the x/y/z adc values

    // Now we'll calculate the accleration value into actual g's
    // This depends on scale being set
    myIMU.ax = (float)myIMU.accelCount[0] * myIMU.aRes; // - myIMU.accelBias[0];
    myIMU.ay = (float)myIMU.accelCount[1] * myIMU.aRes; // - myIMU.accelBias[1];
    myIMU.az = (float)myIMU.accelCount[2] * myIMU.aRes; // - myIMU.accelBias[2];

    myIMU.readGyroData(myIMU.gyroCount);  // Read the x/y/z adc values

    // Calculate the gyro value into actual degrees per second
    // This depends on scale being set
    myIMU.gx = (float)myIMU.gyroCount[0] * myIMU.gRes;
    myIMU.gy = (float)myIMU.gyroCount[1] * myIMU.gRes;
    myIMU.gz = (float)myIMU.gyroCount[2] * myIMU.gRes;

    myIMU.readMagData(myIMU.magCount);  // Read the x/y/z adc values

    // Calculate the magnetometer values in milliGauss
    // Include factory calibration per data sheet and user environmental
    // corrections
    // Get actual magnetometer value, this depends on scale being set
    myIMU.mx = (float)myIMU.magCount[0] * myIMU.mRes
               * myIMU.factoryMagCalibration[0] - myIMU.magBias[0];
    myIMU.my = (float)myIMU.magCount[1] * myIMU.mRes
               * myIMU.factoryMagCalibration[1] - myIMU.magBias[1];
    myIMU.mz = (float)myIMU.magCount[2] * myIMU.mRes
               * myIMU.factoryMagCalibration[2] - myIMU.magBias[2];
  } // if (readByte(MPU9250_ADDRESS, INT_STATUS) & 0x01)

  // Must be called before updating quaternions!
  myIMU.updateTime();

  // Sensors x (y)-axis of the accelerometer is aligned with the y (x)-axis of
  // the magnetometer; the magnetometer z-axis (+ down) is opposite to z-axis
  // (+ up) of accelerometer and gyro! We have to make some allowance for this
  // orientationmismatch in feeding the output to the quaternion filter. For the
  // MPU-9250, we have chosen a magnetic rotation that keeps the sensor forward
  // along the x-axis just like in the LSM9DS0 sensor. This rotation can be
  // modified to allow any convenient orientation convention. This is ok by
  // aircraft orientation standards! Pass gyro rate as rad/s
  MahonyQuaternionUpdate(myIMU.ax, myIMU.ay, myIMU.az, myIMU.gx * DEG_TO_RAD,
                         myIMU.gy * DEG_TO_RAD, myIMU.gz * DEG_TO_RAD, myIMU.my,
                         myIMU.mx, myIMU.mz, myIMU.deltat);

  if (!AHRS)
  {
    myIMU.delt_t = millis() - myIMU.count;
    if (myIMU.delt_t > 500)
    {
      if(SerialDebug)
      {
        // Print acceleration values in milligs!
        SerialPort.print("X-acceleration: "); SerialPort.print(1000 * myIMU.ax);
        SerialPort.print(" mg ");
        SerialPort.print("Y-acceleration: "); SerialPort.print(1000 * myIMU.ay);
        SerialPort.print(" mg ");
        SerialPort.print("Z-acceleration: "); SerialPort.print(1000 * myIMU.az);
        SerialPort.println(" mg ");

        // Print gyro values in degree/sec
        SerialPort.print("X-gyro rate: "); SerialPort.print(myIMU.gx, 3);
        SerialPort.print(" degrees/sec ");
        SerialPort.print("Y-gyro rate: "); SerialPort.print(myIMU.gy, 3);
        SerialPort.print(" degrees/sec ");
        SerialPort.print("Z-gyro rate: "); SerialPort.print(myIMU.gz, 3);
        SerialPort.println(" degrees/sec");

        // Print mag values in degree/sec
        SerialPort.print("X-mag field: "); SerialPort.print(myIMU.mx);
        SerialPort.print(" mG ");
        SerialPort.print("Y-mag field: "); SerialPort.print(myIMU.my);
        SerialPort.print(" mG ");
        SerialPort.print("Z-mag field: "); SerialPort.print(myIMU.mz);
        SerialPort.println(" mG");

        myIMU.tempCount = myIMU.readTempData();  // Read the adc values
        // Temperature in degrees Centigrade
        myIMU.temperature = ((float) myIMU.tempCount) / 333.87 + 21.0;
        // Print temperature in degrees Centigrade
        SerialPort.print("Temperature is ");  SerialPort.print(myIMU.temperature, 1);
        SerialPort.println(" degrees C");
      }

      myIMU.count = millis();
      digitalWrite(myLed, !digitalRead(myLed));  // toggle led
    } // if (myIMU.delt_t > 500)
  } // if (!AHRS)
  else
  {
    // Serial print and/or display at 0.5 s rate independent of data rates
    myIMU.delt_t = millis() - myIMU.count;

    // update LCD once per half-second independent of read rate
    if (myIMU.delt_t > 500)
    {
      if(SerialDebug)
      {
        SerialPort.print("ax = ");  SerialPort.print((int)1000 * myIMU.ax);
        SerialPort.print(" ay = "); SerialPort.print((int)1000 * myIMU.ay);
        SerialPort.print(" az = "); SerialPort.print((int)1000 * myIMU.az);
        SerialPort.println(" mg");

        SerialPort.print("gx = ");  SerialPort.print(myIMU.gx, 2);
        SerialPort.print(" gy = "); SerialPort.print(myIMU.gy, 2);
        SerialPort.print(" gz = "); SerialPort.print(myIMU.gz, 2);
        SerialPort.println(" deg/s");

        SerialPort.print("mx = ");  SerialPort.print((int)myIMU.mx);
        SerialPort.print(" my = "); SerialPort.print((int)myIMU.my);
        SerialPort.print(" mz = "); SerialPort.print((int)myIMU.mz);
        SerialPort.println(" mG");

        SerialPort.print("q0 = ");  SerialPort.print(*getQ());
        SerialPort.print(" qx = "); SerialPort.print(*(getQ() + 1));
        SerialPort.print(" qy = "); SerialPort.print(*(getQ() + 2));
        SerialPort.print(" qz = "); SerialPort.println(*(getQ() + 3));
      }

// Define output variables from updated quaternion---these are Tait-Bryan
// angles, commonly used in aircraft orientation. In this coordinate system,
// the positive z-axis is down toward Earth. Yaw is the angle between Sensor
// x-axis and Earth magnetic North (or true North if corrected for local
// declination, looking down on the sensor positive yaw is counterclockwise.
// Pitch is angle between sensor x-axis and Earth ground plane, toward the
// Earth is positive, up toward the sky is negative. Roll is angle between
// sensor y-axis and Earth ground plane, y-axis up is positive roll. These
// arise from the definition of the homogeneous rotation matrix constructed
// from quaternions. Tait-Bryan angles as well as Euler angles are
// non-commutative; that is, the get the correct orientation the rotations
// must be applied in the correct order which for this configuration is yaw,
// pitch, and then roll.
// For more see
// http://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles
// which has additional links.
      myIMU.yaw   = atan2(2.0f * (*(getQ()+1) * *(getQ()+2) + *getQ()
                    * *(getQ()+3)), *getQ() * *getQ() + *(getQ()+1)
                    * *(getQ()+1) - *(getQ()+2) * *(getQ()+2) - *(getQ()+3)
                    * *(getQ()+3));
      myIMU.pitch = -asin(2.0f * (*(getQ()+1) * *(getQ()+3) - *getQ()
                    * *(getQ()+2)));
      myIMU.roll  = atan2(2.0f * (*getQ() * *(getQ()+1) + *(getQ()+2)
                    * *(getQ()+3)), *getQ() * *getQ() - *(getQ()+1)
                    * *(getQ()+1) - *(getQ()+2) * *(getQ()+2) + *(getQ()+3)
                    * *(getQ()+3));
      myIMU.pitch *= RAD_TO_DEG;
      myIMU.yaw   *= RAD_TO_DEG;

      // Declination of SparkFun Electronics (40°05'26.6"N 105°11'05.9"W) is
      // 	8° 30' E  ± 0° 21' (or 8.5°) on 2016-07-19
      // - http://www.ngdc.noaa.gov/geomag-web/#declination
      myIMU.yaw  -= 8.5;
      myIMU.roll *= RAD_TO_DEG;

      if(SerialDebug)
      {
        SerialPort.print("Yaw, Pitch, Roll: ");
        SerialPort.print(myIMU.yaw, 2);
        SerialPort.print(", ");
        SerialPort.print(myIMU.pitch, 2);
        SerialPort.print(", ");
        SerialPort.println(myIMU.roll, 2);

        SerialPort.print("rate = ");
        SerialPort.print((float)myIMU.sumCount / myIMU.sum, 2);
        SerialPort.println(" Hz");
      }

      myIMU.count = millis();
      myIMU.sumCount = 0;
      myIMU.sum = 0;
    } // if (myIMU.delt_t > 500)
  } // if (AHRS)
}
