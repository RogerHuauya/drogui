#include "threaded.h"
#include "sim7600.h"
#include "registerMap.h"
extern rasp_I2C rasp_i2c;

extern bool logging_state;

std::ofstream log_gps;

void exitLog(){
	if(logging_state) log_gps.close();
}



std::string do_console_command_get_result (char* command)
{
	FILE* pipe = popen(command, "r");
	if (!pipe)
		return "ERROR";
	
	char buffer[128];
	std::string result = "";
	while(!feof(pipe))
	{
		if(fgets(buffer, 128, pipe) != NULL)
			result += buffer;
	}
	pclose(pipe);
	return(result);
}




void *logging(void *threadid){
    unsigned long long tim = 0;
    //unistd::sleep(10);
    while(!logging_state){}
    std::string name_log = str_datetime(); 
    log_gps.open("logs/"+name_log+ "_control"+ ".txt");
    
    log_gps.precision(10);

    /*std::ofstream log_imu; 
    log_imu.open("logs/"+name_log+ "_imu"+ ".txt");
    log_imu.precision(10);
    */
    /*
    log_file << "H_VAL   H_STEP_SIZE " << rasp_i2c.readFloat(H_VAL) << " " << rasp_i2c.readFloat(H_STEP_SIZE)<< std::endl;
    log_file << "ROLL KP KI KD " << rasp_i2c.readFloat(ROLL_KP) << " " <<rasp_i2c.readFloat(ROLL_KI) << " " <<rasp_i2c.readFloat(ROLL_KD)<<std::endl;
    log_file << "PITCH KP KI KD " << rasp_i2c.readFloat(PITCH_KP) << " " <<rasp_i2c.readFloat(PITCH_KI) << " " <<rasp_i2c.readFloat(PITCH_KD)<<std::endl;
    log_file << "YAW KP KI KD " << rasp_i2c.readFloat(YAW_KP) << " " <<rasp_i2c.readFloat(YAW_KI) << " " <<rasp_i2c.readFloat(YAW_KD)<<std::endl;
    */
    
    while(1){
        if(logging_state){
            
	    log_gps << tim/100.0 << "\t";

            log_gps << rasp_i2c.readFloat(GPS_X) 	<< "\t"; unistd::usleep(300);
            log_gps << rasp_i2c.readFloat(GPS_Y) 	<< "\t"; unistd::usleep(300);
            log_gps << rasp_i2c.readFloat(GPS_STATUS) 	<< "\t"; unistd::usleep(300);
            //log_gps << rasp_i2c.readFloat(GPS_CNT)      << "\n"; unistd::usleep(300);
            log_gps << rasp_i2c.readFloat(X_VAL) 	<< "\t"; unistd::usleep(300);
	    log_gps << rasp_i2c.readFloat(Y_VAL) 	<< "\t"; unistd::usleep(300);
	    log_gps << rasp_i2c.readFloat(ACC_X) 	<< "\t"; unistd::usleep(300);
	    log_gps << rasp_i2c.readFloat(ACC_Y) 	<< "\t"; unistd::usleep(300);
	    log_gps << rasp_i2c.readFloat(ACC_Z) 	<< "\t"; unistd::usleep(300);
	    log_gps << rasp_i2c.readFloat(YAW_VAL)      << "\n"; unistd::usleep(300);
	    /*log_gps << rasp_i2c.readFloat(ROLL_REF)     << "\t"; unistd::usleep(300);
            log_gps << rasp_i2c.readFloat(PITCH_REF) 	<< "\t"; unistd::usleep(300);
            log_gps << rasp_i2c.readFloat(ROLL_VAL) 	<< "\t"; unistd::usleep(300);
            log_gps << rasp_i2c.readFloat(PITCH_VAL) 	<< "\t"; unistd::usleep(300);
            log_gps << rasp_i2c.readFloat(GYRO_X_REF) 	<< "\t"; unistd::usleep(300);
            log_gps << rasp_i2c.readFloat(GYRO_Y_REF) 	<< "\t"; unistd::usleep(300);
            log_gps << rasp_i2c.readFloat(GYRO_X) 	<< "\t"; unistd::usleep(300);
            log_gps << rasp_i2c.readFloat(GYRO_Y) 	<< "\t"; unistd::usleep(300);
            log_gps << rasp_i2c.readFloat( Z_VAL ) 	<< "\t"; unistd::usleep(300);
            log_gps << rasp_i2c.readFloat( Z_U ) 	<< "\n"; unistd::usleep(300);*/
	    /*log_gps << tim/50.0 << "\t";
            log_gps << rasp_i2c.readFloat(MOTOR_1)      << "\t"; unistd::usleep(300);
            log_gps << rasp_i2c.readFloat(MOTOR_2)      << "\t"; unistd::usleep(300);
            log_gps << rasp_i2c.readFloat(MOTOR_3)      << "\t"; unistd::usleep(300);
            log_gps << rasp_i2c.readFloat(MOTOR_4)      << "\t"; unistd::usleep(300);
	    log_gps << rasp_i2c.readFloat(ROLL_VAL) 	<< "\t"; unistd::usleep(300);
            log_gps << rasp_i2c.readFloat(PITCH_VAL) 	<< "\t"; unistd::usleep(300);
            log_gps << rasp_i2c.readFloat(GYRO_X_REF) 	<< "\t"; unistd::usleep(300);
            log_gps << rasp_i2c.readFloat(GYRO_Y_REF) 	<< "\t"; unistd::usleep(300);
            log_gps << rasp_i2c.readFloat(GYRO_X) 	<< "\t"; unistd::usleep(300);
            log_gps << rasp_i2c.readFloat(GYRO_Y) 	<< "\n"; unistd::usleep(300);*/
	}
        else{
            unistd::usleep(2700);
        }
        unistd::usleep(7300);
        tim++;
    }
}


void *wifiCheck(void *threadid){
    
    int cnt = 0;
    
    while(1){
        unistd::usleep(200000);
        
        std::string CommandResult = do_console_command_get_result((char*)"cat /sys/class/net/wlan0/operstate");
	if (CommandResult.find("up") != 0){
           //std::cout<< "No connected"  << std::endl;
	    rasp_i2c.sendFloat(Z_MG, 55);
            cnt++;
	}
        else cnt = 0;

        if(cnt >=  20) rasp_i2c.sendFloat(Z_REF, 0);
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
