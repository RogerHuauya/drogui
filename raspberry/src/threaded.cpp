
#include "threaded.h"
extern rasp_I2C rasp_i2c;

extern bool logging_state;
void *logging(void *threadid){
    
    while(1){
        std::ofstream log_file;
        while(!logging_state){}
        std::string name_log = str_datetime(); 
        log_file.open("logs/"+name_log+".txt");
        /*
        log_file << "H_VAL   H_STEP_SIZE " << rasp_i2c.readFloat(H_VAL) << " " << rasp_i2c.readFloat(H_STEP_SIZE)<< std::endl;
        log_file << "ROLL KP KI KD " << rasp_i2c.readFloat(ROLL_KP) << " " <<rasp_i2c.readFloat(ROLL_KI) << " " <<rasp_i2c.readFloat(ROLL_KD)<<std::endl;
        log_file << "PITCH KP KI KD " << rasp_i2c.readFloat(PITCH_KP) << " " <<rasp_i2c.readFloat(PITCH_KI) << " " <<rasp_i2c.readFloat(PITCH_KD)<<std::endl;
        log_file << "YAW KP KI KD " << rasp_i2c.readFloat(YAW_KP) << " " <<rasp_i2c.readFloat(YAW_KI) << " " <<rasp_i2c.readFloat(YAW_KD)<<std::endl;
        */
        while(1){
            /*
            log_file<<rasp_i2c.readFloat(H_VAL);
            log_file<<"\t";
            log_file<<rasp_i2c.readFloat(ROLL_DEG)*180.0/pi+180;
            log_file<<"\t";
            log_file<<rasp_i2c.readFloat(PITCH_DEG)*180.0/pi+180;
            log_file<<"\t";
            log_file<<rasp_i2c.readFloat(YAW_DEG)*180.0/pi+180;
            log_file<<"\t";
            log_file<<rasp_i2c.readFloat(ROLL_REF);
            log_file<<"\t";
            log_file<<rasp_i2c.readFloat(PITCH_REF);
            log_file<<"\t";
            log_file<<rasp_i2c.readFloat(YAW_REF);
	    log_file<<"\t";
	    log_file<<rasp_i2c.readFloat(PRESS_ABS) << std::endl;
        */
       /*
            log_file<<rasp_i2c.readFloat(RAW_TEMP);
            log_file<<"\t";           
            log_file<<rasp_i2c.readFloat(TEMP_ABS);
            log_file<<"\t";
            log_file<<rasp_i2c.readFloat(RAW_PRESS);
            log_file<<"\t";
            log_file<<rasp_i2c.readFloat(PRESS_ABS);
            */
            log_file<<rasp_i2c.readFloat(Z_REF);
            log_file<<"\t";
            log_file<<rasp_i2c.readFloat(Z_U);
            log_file<<"\t";
            log_file<<rasp_i2c.readFloat(Z_VAL);
            log_file<<"\t";
            log_file<<std::endl;
            //unistd::uunistd::sleep(50000); // takes microseconds
            unistd::sleep(1);
            if(!logging_state) break;
        }
    }
}

void *gps_data(void *threadid){
    sim7600.GPSStart();
    unistd::usleep(100000ip00);
    float offset_x = 0, offset_y = 0, r = 6371;
    float aux_offx = 0,aux_offy = 0;
    int cont = 0;
    while( sim7600.GPSGet() && (sim7600.Log > 0)  && ( sim7600.Lat > 0 ) && (cont<10)   ){
       aux_offx =  r*sim7600.Log*cos(sim7600.Lat*pi/180);
       aux_offy = r*sim7600.Lat;
       offset_x += aux_offx;
       offset_y += aux_offy;
       
       //printf("Off_x: %.6lf Off_y: %.6lf it: %d",aux_offx,aux_offy,cont); std::cout << std::endl;
       cont++;
    }
    offset_x /= 10;
    offset_y /= 10;
    sim7600.offset_x = offset_x;
    sim7600.offset_y = offset_y;
    while(1){
        if(sim7600.GPSGet()){
            sim7600.pos_x = r*sim7600.Log*cos(sim7600.Lat*pi/180) - offset_x;
            sim7600.pos_y =  r*sim7600.Lat - offset_y;
            rasp_i2c.sendFloat(GPS_X, sim7600.pos_x);
            rasp_i2c.sendFloat(GPS_Y, sim7600.pos_y);
            rasp_i2c.sendFloat(GPS_AVAILABLE, 1.0);
        }
        unistd::usleep(100000);
    }
}
