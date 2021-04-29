#include "threaded.h"
#include "sim7600.h"
#include "registerMap.h"
#include <chrono>
#include "utils.h"

#define max(x,y) ( ( (x) > (y) ) ? (x) : (y) )

extern rasp_I2C rasp_i2c;

extern bool logging_state;

std::ofstream log_xyz, log_rpy;

int gps_arr[] = {GPS_X, GPS_Y, GPS_STATE, X_VAL, Y_VAL, ACC_X, ACC_Y, ACC_Z, RAW_ROLL, RAW_PITCH, RAW_YAW};
int gps_sz = 11;

int z_arr[] = {ROLL_REF, PITCH_REF, ROLL_VAL,  PITCH_VAL, GYRO_X_REF, GYRO_Y_REF, GYRO_X, GYRO_Y, Z_VAL, Z_U};
int z_sz = 10;

int rpy_arr[] = {ROLL_SCURVE, PITCH_SCURVE, GYRO_X_REF, GYRO_Y_REF, GYRO_X, GYRO_Y, DER_GYRO_X, DER_GYRO_Y, ROLL_VAL, PITCH_VAL, DER_ROLL, DER_PITCH, ROLL_U, PITCH_U};
int rpy_sz = 14;

int xyz_arr[] = { GPS_X, GPS_Y, X_SCURVE, Y_SCURVE, X_VAL, Y_VAL, ROLL_SCURVE, PITCH_SCURVE, ROLL_VAL, PITCH_VAL, RAW_YAW };
int xyz_sz = 11;

void exitLog(){
	if(logging_state) log_rpy.close();
	if(logging_state) log_xyz.close();
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


void logTitle(std::ofstream *log, int *arr, int sz){
    *log << "TIME\t";
    for(int i = 0 ; i < sz ; i++){
	*log << varNames[arr[i]];
	if(i < sz-1) *log << "\t";
	else *log << "\n";
    }

}


void logArr(std::ofstream *log, int *arr, int sz){

    for(int i = 0 ; i < sz ; i++){
	if(arr[i] == ROLL_VAL || arr[i] == ROLL_SCURVE \
	|| arr[i] == PITCH_VAL || arr[i] == PITCH_SCURVE\
	|| arr[i] == YAW_VAL || arr[i] == YAW_SCURVE )
		*log << 180.0/pi*rasp_i2c.readFloat(arr[i]);	
	else *log << rasp_i2c.readFloat(arr[i]);
	if(i < sz-1) *log << "\t";
	else *log << "\n";
	unistd::usleep(200);
    }

}




void *logging(void *threadid){
    unsigned long long tim = 0;
    std::chrono::time_point<std::chrono::system_clock> end_clock, start_clock;
    while(!logging_state){}

    std::string name_log = str_datetime();
    log_rpy.open("logs/"+name_log+ "_rpy"+ ".txt");
    log_rpy.precision(10);

    log_xyz.open("logs/"+name_log+ "_xyz"+ ".txt");
    log_xyz.precision(10);

    start_clock = std::chrono::system_clock::now();
    end_clock = std::chrono::system_clock::now();

    logTitle(&log_rpy, rpy_arr, rpy_sz);
    logTitle(&log_xyz, xyz_arr, xyz_sz);

    while(1){

	while(std::chrono::duration_cast<std::chrono::microseconds> (end_clock-start_clock).count() < (10000*tim))
	    end_clock = std::chrono::system_clock::now();

        if(logging_state){

	    log_rpy << tim/100.0 << "\t";
	    log_xyz << tim/100.0 << "\t";
	    logArr(&log_rpy, rpy_arr, rpy_sz);
	    logArr(&log_xyz, xyz_arr, xyz_sz);
	}
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
