#include <SparkFunMPU9250-DMP.h>
#include "Wire.h"


MPU9250_DMP imu;
char  receivedCommands[20];

void setup() 
{

  // Call imu.begin() to verify communication and initialize
  if (imu.begin() != INV_SUCCESS)
  {
    while (1)
    {
      digitalWrite(13, HIGH);
      delay(500);
      digitalWrite(13, LOW);
      delay(500);
    }
  }
  imu.dmpBegin(DMP_FEATURE_6X_LP_QUAT | // Enable 6-axis quat
                  DMP_FEATURE_GYRO_CAL, // Use gyro calibration
                  5); // Set DMP FIFO rate to 10 Hz
  // DMP_FEATURE_LP_QUAT can also be used. It uses the 
  // accelerometer in low-power mode to estimate quat's.
  // DMP_FEATURE_LP_QUAT and 6X_LP_QUAT are mutually exclusive
  Wire.begin(0x08);
  Wire.onRequest(requestEvent);
  Wire.onReceive(receiveEvent);
}

void loop(){

  // Check for new data in the FIFO
  if ( imu.fifoAvailable() ){
    // Use dmpUpdateFifo to update the ax, gx, mx, etc. values
    if ( imu.dmpUpdateFifo() == INV_SUCCESS)
    {
      // computeEulerAngles can be used -- after updating the
      // quaternion values -- to estimate roll, pitch, and yaw
      imu.computeEulerAngles();
    }
  }
}

void printIMUData(void)
{  
  // After calling dmpUpdateFifo() the ax, gx, mx, etc. values
  // are all updated.
  // Quaternion values are, by default, stored in Q30 long
  // format. calcQuat turns them into a float between -1 and 1
  float q0 = imu.calcQuat(imu.qw);
  float q1 = imu.calcQuat(imu.qx);
  float q2 = imu.calcQuat(imu.qy);
  float q3 = imu.calcQuat(imu.qz);

}

void requestEvent(){
    char buff[50];
    sprintf(buff,"Hola");
    Wire.send(buff, 4);  //Set the buffer up to send all 14 bytes of data
}

 

void receiveEvent(int bytesReceived){
    for (int a = 0; a < bytesReceived; a++){

        if ( a < 10) receivedCommands[a] = Wire.read();
        else Wire.read();  // if we receive more data then allowed just throw it away
    }
}