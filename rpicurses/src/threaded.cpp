

#include "threaded.h"
#include "sim7600.h"
#include "registerMap.h"
extern rasp_I2C rasp_i2c;

extern bool logging_state;

void *logging(void *threadid){
    unsigned long long tim = 0;
    unistd::sleep(10);
    while(!logging_state){};
    while(1){
        std::ofstream log_gps;
        //while(!logging_state){}
        std::string name_log = str_datetime(); 
        //std::string name_log = std::to_string(rasp_i2c.readFloat(ROLL_KP)) + "_" + std::to_string(rasp_i2c.readFloat(ROLL_KI)) + "_" + std::to_string(rasp_i2c.readFloat(ROLL_KD));
	log_gps.open("logs/"+name_log+ "_control"+ ".txt");
        
	    log_gps.precision(10);
	
        /*std::ofstream log_imu; 
        log_imu.open("logs/"+name_log+ "_imu"+ ".txt");
	    log_imu.precision(10);*/
        
        /*
        log_file << "H_VAL   H_STEP_SIZE " << rasp_i2c.readFloat(H_VAL) << " " << rasp_i2c.readFloat(H_STEP_SIZE)<< std::endl;
        log_file << "ROLL KP KI KD " << rasp_i2c.readFloat(ROLL_KP) << " " <<rasp_i2c.readFloat(ROLL_KI) << " " <<rasp_i2c.readFloat(ROLL_KD)<<std::endl;
        log_file << "PITCH KP KI KD " << rasp_i2c.readFloat(PITCH_KP) << " " <<rasp_i2c.readFloat(PITCH_KI) << " " <<rasp_i2c.readFloat(PITCH_KD)<<std::endl;
        log_file << "YAW KP KI KD " << rasp_i2c.readFloat(YAW_KP) << " " <<rasp_i2c.readFloat(YAW_KI) << " " <<rasp_i2c.readFloat(YAW_KD)<<std::endl;
        */
       
        while(1){
            /*if(tim%50 == 0){
                log_gps << tim/50 << "\t" << sim7600.pos_x << "\t"<< sim7600.pos_y << "\t" << sim7600.Lat << "\t" << sim7600.Log <<std::endl;
            }*/

            if(logging_state){
                /*log_gps << tim/100.0 << "\t" << rasp_i2c.readFloat(MOTOR_1) << "\t" << rasp_i2c.readFloat(MOTOR_2) << "\t"\
				     << rasp_i2c.readFloat(MOTOR_3) << "\t" << rasp_i2c.readFloat(MOTOR_4) <<std::endl;*/
		/*log_gps << tim/100.0 << "\t" << rasp_i2c.readFloat(GYRO_X_REF) << "\t" << rasp_i2c.readFloat(GYRO_X) << "\t"\
				     << rasp_i2c.readFloat(DER_GYRO_X) << std::endl;// "\t" << rasp_i2c.readFloat(GYRO_Y) <<std::endl;*/
		
		// GYRO CONTROL
		/*log_gps << tim/50.0 << "\t" << rasp_i2c.readFloat(GYRO_X_REF) << "\t" << rasp_i2c.readFloat(GYRO_Y_REF) << "\t"\
                                     << rasp_i2c.readFloat(GYRO_X) << "\t" << rasp_i2c.readFloat(GYRO_Y) << "\t"\
				     << rasp_i2c.readFloat(ROLL_U) << "\t" << rasp_i2c.readFloat(PITCH_U) << std::endl;// "\t" << rasp_i2c.readFloat(GYRO_Y) <<std::endl;
                */
		
		//RPY CONTROL
		
		log_gps << tim/50.0 << "\t" << rasp_i2c.readFloat(ROLL_REF) 	<< "\t" << rasp_i2c.readFloat(PITCH_REF) 	<< "\t"\
                                    	    << rasp_i2c.readFloat(ROLL_VAL) 	<< "\t" << rasp_i2c.readFloat(PITCH_VAL) 	<< "\t"\
				     	    << rasp_i2c.readFloat(GYRO_X_REF) 	<< "\t"	<< rasp_i2c.readFloat(GYRO_Y_REF) 	<< "\t"\
                                    	    << rasp_i2c.readFloat(GYRO_X) 	<< "\t" << rasp_i2c.readFloat(GYRO_Y) 		<< std::endl;
		
		/*log_gps << tim/50.0 << "\t" << rasp_i2c.readFloat(GYRO_X) << "\t" << rasp_i2c.readFloat(GYRO_Y) << "\t"\
					    << rasp_i2c.readFloat(GYRO_Z) << std::endl;
                log_file<<rasp_i2c.readFloat(H_VAL);
                log_file<<"\t";
                log_imu << tim/5.0 << ",\t";
                log_imu<<rasp_i2c.readFloat(ROLL_VAL);
                log_imu<<"\t";
                log_imu<<rasp_i2c.readFloat(PITCH_VAL);
                log_imu<<"\t";
                log_imu<<rasp_i2c.readFloat(YAW_VAL);
                log_imu<<"\t";
                log_imu<<rasp_i2c.readFloat(ACC_X);
                log_imu<<"\t";
                log_imu<<rasp_i2c.readFloat(ACC_Y);
                log_imu<<"\t";
                log_imu<<rasp_i2c.readFloat(ACC_Z);
                log_imu<<"\t";
                log_imu<<rasp_i2c.readFloat(X_VAL);
                log_imu<<"\t";
                log_imu<<rasp_i2c.readFloat(Y_VAL);
                log_imu<<"\t";
                log_imu<<rasp_i2c.readFloat(Z_VAL);
                log_imu<< std::endl;*/
            }
            unistd::usleep(20000);
	    tim++;

        }
    }
}

void *gps_data(void *threadid){
    
    sim7600.GPSStart();
    unistd::sleep(10);
    
    if(sim7600.GPSGet()){
        sim7600.offset_Log = sim7600.Log;
        sim7600.offset_Lat = sim7600.Lat;
    }
    
    while(1){
        if(sim7600.GPSGet()){
            sim7600.pos_x = (sim7600.Log - sim7600.offset_Log)/8.99871924359995e-06;
            sim7600.pos_y = (sim7600.Lat - sim7600.offset_Lat)/8.99871924359995e-06;
            sim7600.pos_x = sim7600.pos_x-(sim7600.pos_y/5.658034);
	    rasp_i2c.sendFloat(GPS_X, sim7600.pos_x);
            rasp_i2c.sendFloat(GPS_Y, sim7600.pos_y);
            rasp_i2c.sendFloat(GPS_AVAILABLE, 1.0);
        }
        unistd::usleep(100000);
    }
}
