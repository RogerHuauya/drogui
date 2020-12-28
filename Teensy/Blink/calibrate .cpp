//#define CALIBRATE
#ifdef CALIBRATE

#include "calibrate.h"

Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28);
adafruit_bno055_offsets_t off_imu;

void initializeSystem(){
    if(!bno.begin()){

        Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
        while(1);
    }
    
    delay(1000);

    bno.setExtCrystalUse(true);
    delay(500);
    
}

int main(void){

    initializeSystem();
    while(1){    
        sensors_event_t event;
        bno.getEvent(&event);

        uint8_t sys, gyro, accel, mag = 0;
        bno.getCalibration(&sys, &gyro, &accel, &mag);
        Serial.print(sys); Serial.print("\t");
        Serial.print(gyro); Serial.print("\t");
        Serial.print(accel); Serial.print("\t");
        Serial.print(mag); Serial.print("\n");

        if(bno.isFullyCalibrated()){
            bno.getSensorOffsets(off_imu);
            Serial.print("off_imu.accel_offset_x = "); Serial.print(off_imu.accel_offset_x); Serial.print(";\n");
            Serial.print("off_imu.accel_offset_y = "); Serial.print(off_imu.accel_offset_y); Serial.print(";\n");
            Serial.print("off_imu.accel_offset_z = "); Serial.print(off_imu.accel_offset_z); Serial.print(";\n");
            Serial.print("off_imu.gyro_offset_x = "); Serial.print(off_imu.gyro_offset_x); Serial.print(";\n");
            Serial.print("off_imu.gyro_offset_y = ");Serial.print(off_imu.gyro_offset_y); Serial.print(";\n");
            Serial.print("off_imu.gyro_offset_z = ");Serial.print(off_imu.gyro_offset_z); Serial.print(";\n");
            Serial.print("off_imu.mag_offset_x = ");Serial.print(off_imu.mag_offset_x); Serial.print(";\n");
            Serial.print("off_imu.mag_offset_y = ");Serial.print(off_imu.mag_offset_y); Serial.print(";\n");
            Serial.print("off_imu.mag_offset_z = ");Serial.print(off_imu.mag_offset_z); Serial.print(";\n");
            Serial.print("off_imu.accel_radius = "); Serial.print(off_imu.accel_radius); Serial.print(";\n");
            Serial.print("off_imu.mag_radius = ");Serial.print(off_imu.mag_radius);Serial.print(";\n");
            delay(5000);
        }
        delay(10);

    }
    return 0;
}
#endif
