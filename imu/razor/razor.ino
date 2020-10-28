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
#include <Wire.h>
#include "MPU9250.h"

#define SerialPort Serial1


// Pin definitions
int intPin = 12;  // These can be changed, 2 and 3 are the Arduinos ext int pins
int myLed  = 13;  // Set up pin 13 led for toggling

#define I2Cclock 400000
#define I2Cport Wire
#define MPU9250_ADDRESS MPU9250_ADDRESS_AD0   // Use either this line or the next to select which I2C address your device is using
//#define MPU9250_ADDRESS MPU9250_ADDRESS_AD1

MPU9250 myIMU(MPU9250_ADDRESS, I2Cclock);

void setup()
{
 
  // TWBR = 12;  // 400 kbit/sec I2C 
  Wire.begin();
  SerialPort.begin(115200);
  
  // Set up the interrupt pin, its set as active high, push-pull
  pinMode(intPin, INPUT);
  digitalWrite(intPin, LOW);
  pinMode(myLed, OUTPUT);
  digitalWrite(myLed, HIGH);

  // Read the WHO_AM_I register, this is a good test of communication
  byte c = myIMU.readByte(MPU9250_ADDRESS, WHO_AM_I_MPU9250);
  
  if (c == 0x71) // WHO_AM_I should always be 0x71
  {
    
    // Start by performing self test and reporting values
    myIMU.MPU9250SelfTest(myIMU.selfTest);


    // Calibrate gyro and accelerometers, load biases in bias registers
    myIMU.calibrateMPU9250(myIMU.gyroBias, myIMU.accelBias);


    myIMU.initMPU9250();
    // Initialize device for active mode read of acclerometer, gyroscope, and
    // temperature
    
    // Read the WHO_AM_I register of the magnetometer, this is a good test of
    // communication
    byte d = myIMU.readByte(AK8963_ADDRESS, WHO_AM_I_AK8963);
    

    if (d != 0x48)
    {
      abort();
    }

    // Get magnetometer calibration from AK8963 ROM
    myIMU.initAK8963(myIMU.factoryMagCalibration);
    // Initialize device for active mode read of magnetometer
    


    // Get sensor resolutions, only need to do this once
    myIMU.getAres();
    myIMU.getGres();
    myIMU.getMres();




  } // if (c == 0x71)
  else
  {
    abort();
  }
}

int sumDigits(long long x){
	int ans = 0;
	while(x > 0){
		ans += x%10;
		x/=10;
	}
	return ans;
}

void loop()
{ 
  // If intPin goes high, all data registers have new data
  // On interrupt, check if data ready interrupt
	if (myIMU.readByte(MPU9250_ADDRESS, INT_STATUS) & 0x01){

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
							myIMU.mx, -myIMU.mz, myIMU.deltat);


    // Serial print and/or display at 0.5 s rate independent of data rates
		// roll (x-axis rotation)
    myIMU.delt_t = millis() - myIMU.count;

    // update LCD once per half-second independent of read rate
    if (myIMU.delt_t > 20)
    {
		double sinr_cosp = 2 * ((*getQ()) * (*(getQ()+1)) + (*(getQ()+2)) * (*(getQ()+3)));
		double cosr_cosp = 1 - 2 * ((*(getQ()+1)) * (*(getQ()+1)) + (*(getQ()+2)) * (*(getQ()+2)));
		myIMU.roll = atan2(sinr_cosp, cosr_cosp);

		// pitch (y-axis rotation)
		double sinp = 2 * ((*getQ()) * (*(getQ()+2)) - (*(getQ()+3)) * (*(getQ()+1)));
		if (abs(sinp) >= 1)
			myIMU.pitch = copysign(PI / 2, sinp); // use 90 degrees if out of range
		else
			myIMU.pitch = asin(sinp);

		// yaw (z-axis rotation)
		double siny_cosp = 2 * ((*getQ()) * (*(getQ()+3)) + (*(getQ()+1)) * (*(getQ()+2)));
		double cosy_cosp = 1 - 2 * ((*(getQ()+2)) * (*(getQ()+2)) + (*(getQ()+3)) * (*(getQ()+3)));
		myIMU.yaw = atan2(siny_cosp, cosy_cosp);

		myIMU.pitch *= RAD_TO_DEG;
		myIMU.yaw   *= RAD_TO_DEG;
		myIMU.roll *= RAD_TO_DEG;

		//SerialPort.print("Yaw, Pitch, Roll: ");
		int r,p,y;

		r = myIMU.roll + 180;
		p = myIMU.pitch+ 180;
		y = myIMU.yaw+ 180;
		SerialPort.print(myIMU.roll);
		SerialPort.print('\t');
		SerialPort.print(myIMU.pitch);
		SerialPort.print('\t');
		SerialPort.print(myIMU.yaw);
		SerialPort.print('\n');
		/*
		char buffer[80];
		long long data = r + p*360 + y*360*360;
		int dat1 = data%10000LL;
		int dat2 = data/10000;
		int dig = sumDigits(data);
		sprintf(buffer, "%d%04d#%d$",dat2,dat1, dig);
		SerialPort.print(buffer);*/
		/*SerialPort.print(dat1);
		SerialPort.print('#');
		SerialPort.print(dig);
		SerialPort.print('$');*/

		//SerialPort.println(1.0*data/1000.0);

	/*
		SerialPort.print("rate = ");
		SerialPort.print((float)myIMU.sumCount / myIMU.sum, 2);
		SerialPort.println(" Hz");
		
	*/
		myIMU.count = millis();
		myIMU.sumCount = 0;
		myIMU.sum = 0;
    } // if (myIMU.delt_t > 500)

}