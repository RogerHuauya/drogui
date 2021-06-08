#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
/* Set the delay between fresh samples */
#define BNO055_SAMPLERATE_DELAY_MS (100)

// Check I2C device address and correct line below (by default address is 0x29 or 0x28)
//                                   id, address
Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28);

/**************************************************************************/
/*
    Displays some basic information on this sensor from the unified
    sensor API sensor_t type (see Adafruit_Sensor for more information)
*/
/**************************************************************************/
void displaySensorDetails(void)
{
  sensor_t sensor;
  bno.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" xxx");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" xxx");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" xxx");
  Serial.println("------------------------------------");
  Serial.println("");
  delay(500);
}

/**************************************************************************/
/*
    Arduino setup function (automatically called at startup)
*/
/**************************************************************************/
void setup(void)
{
  Serial.begin(115200);
  Serial.println("Orientation Sensor Test"); Serial.println("");

  /* Initialise the sensor */
  if(!bno.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }
   
  delay(1000);

  /* Use external crystal for better accuracy */
  bno.setExtCrystalUse(true);
   
  /* Display some basic information on this sensor */
  displaySensorDetails();
}


void loop() {
  // put your main code here, to run repeatedly:
  adafruit_bno055_offsets_t test;
  test.accel_offset_x=  -24;
  test.accel_offset_y=  -25;
  test.accel_offset_z=  33;
  test.mag_offset_x=  93;
  test.mag_offset_y=  268;
  test.mag_offset_z=  64;
  test.gyro_offset_x= 1;
  test.gyro_offset_y= 0;
  test.gyro_offset_z= -1;
  test.accel_radius=  1000;
  test.mag_radius=  291;
  bno.setSensorOffsets(test);

  while(1){
    sensors_event_t event;
    bno.getEvent(&event);  
    Serial.print(F("Orientation: "));
    Serial.print((float)event.orientation.x);
    Serial.print(F(" "));
    Serial.print((float)event.orientation.y);
    Serial.print(F(" "));
    Serial.print((float)event.orientation.z);
    Serial.println(F(""));

    uint8_t sys, gyro, accel, mag = 0;
    bno.getCalibration(&sys, &gyro, &accel, &mag);
    Serial.print(F("Calibration: "));
    Serial.print(sys, DEC);
    Serial.print(F(" "));
    Serial.print(gyro, DEC);
    Serial.print(F(" "));
    Serial.print(accel, DEC);
    Serial.print(F(" "));
    Serial.println(mag, DEC);
  
    delay(BNO055_SAMPLERATE_DELAY_MS);
  }

  /*
  while(!bno.isFullyCalibrated()){
    uint8_t sys, gyro, accel, mag = 0;
    bno.getCalibration(&sys, &gyro, &accel, &mag);
    Serial.print(F("Calibration: "));
    Serial.print(sys, DEC);
    Serial.print(F(" "));
    Serial.print(gyro, DEC);
    Serial.print(F(" "));
    Serial.print(accel, DEC);
    Serial.print(F(" "));
    Serial.println(mag, DEC);
  
    delay(BNO055_SAMPLERATE_DELAY_MS);
  }
  bno.getSensorOffsets(test);

  Serial.print("test.accel_offset_x=\t");
  Serial.print(test.accel_offset_x);
  Serial.print(";\ntest.accel_offset_y=\t");
  Serial.print(test.accel_offset_y);
  Serial.print(";\ntest.accel_offset_z=\t");
  Serial.print(test.accel_offset_z);

  Serial.print(";\ntest.mag_offset_x=\t");
  Serial.print(test.mag_offset_x);
  Serial.print(";\ntest.mag_offset_y=\t");
  Serial.print(test.mag_offset_y);
  Serial.print(";\ntest.mag_offset_z=\t");
  Serial.print(test.mag_offset_z);

  Serial.print(";\ntest.gyro_offset_x=\t");
  Serial.print(test.gyro_offset_x);
  Serial.print(";\ntest.gyro_offset_y=\t");
  Serial.print(test.gyro_offset_y);
  Serial.print(";\ntest.gyro_offset_z=\t");
  Serial.print(test.gyro_offset_z);

  Serial.print(";\ntest.accel_radius=\t");
  Serial.print(test.accel_radius);
  Serial.print(";\ntest.mag_radius=\t");
  Serial.print(test.mag_radius);
  Serial.print(";\n");

  while(1){
    sensors_event_t event;
    bno.getEvent(&event);
    delay(BNO055_SAMPLERATE_DELAY_MS);
  }*/
}
