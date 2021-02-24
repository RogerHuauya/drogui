

#include "threaded.h"
#include "sim7600.h"
#include "registerMap.h"
extern rasp_I2C rasp_i2c;

extern bool logging_state;



string do_console_command_get_result (char* command)
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
    int cnt = 0;
    std::ofstream log_gps;
    //while(!logging_state){}
    std::string name_log = str_datetime(); 
    log_gps.open("logs/"+name_log+ "_control"+ ".txt");
    
    log_gps.precision(10);

    std::ofstream log_imu; 
    log_imu.open("logs/"+name_log+ "_imu"+ ".txt");
    log_imu.precision(10);
    
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
            
	    log_gps << tim/50.0 << "\t" << rasp_i2c.readFloat(ROLL_REF) 	<< "\t" << rasp_i2c.readFloat(PITCH_REF) 	<< "\t"\
                                    	    << rasp_i2c.readFloat(ROLL_VAL) 	<< "\t" << rasp_i2c.readFloat(PITCH_VAL) 	<< "\t"\
				     	    << rasp_i2c.readFloat(GYRO_X_REF) 	<< "\t"	<< rasp_i2c.readFloat(GYRO_Y_REF) 	<< "\t"\
                                    	    << rasp_i2c.readFloat(GYRO_X) 	<< "\t" << rasp_i2c.readFloat(GYRO_Y) 		<< std::endl;
        }
        unistd::usleep(20000);
        tim++;
        
        std::string CommandResult = do_console_command_get_result((char*)"cat /sys/class/net/wlan0/operstate");
	if (CommandResult.find("up") != 0){
            rasp_i2c.sendFloat(Z_MG, 55);
            cnt++;
	}
        else cnt = 0;

        if(cnt >=  250) rasp_i2c.sendFloat(Z_REF, 0);

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
