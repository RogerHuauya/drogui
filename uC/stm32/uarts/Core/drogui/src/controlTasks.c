#include "controlTasks.h"
#include "filter.h"
#include "task.h"
#include "scurve.h"
#include "pwm.h"
#include "serial.h"


pwm m1, m2, m3, m4;
pid roll2w, pitch2w, yaw2w; 
pid wroll_control, wpitch_control, wyaw_control;
pid z_control, x_control, y_control;

filter filter_wroll, filter_wpitch, filter_wyaw;
filter filter_R, filter_P, filter_Y, filter_H;

scurve z_sp, x_sp, y_sp, roll_sp, pitch_sp, yaw_sp, H_sp;

float  H, H_comp, R, P, Y, H_ref, X_C, Y_C, ANG_MAX = 3*PI/180.0;
float M1,M2,M3,M4;

float wroll_ref, wpitch_ref, wyaw_ref;
float roll_ref, pitch_ref, yaw_ref;
float x_ref = 0, y_ref = 0, z_ref = 0;
float Ixx = 0, Iyy = 0;
//float aux_wref, aux_wref2;

void saturateM(float H){
    float f_max = 1;
    /*
    float arr_M[] = {M1, M2, M3, M4};
    for(int i = 0; i < 4 ; i++){
        float delta = fmax(fmax(arr_M[i] + H - 10000, -arr_M[i]-H), 0);
        f_max = fmax(f_max, fabs(arr_M[i] / (fabs(arr_M[i]) - delta + 0.0000001)) );
    }*/

    M1 = sqrt(M1 / f_max + H);
    M2 = sqrt(M2 / f_max + H);
    M3 = sqrt(M3 / f_max + H);
    M4 = sqrt(M4 / f_max + H);
}


void updatePID(){
    int index = getReg(PID_INDEX), var = getReg(PID_VAR);

    switch(var){

        case PID_ROLL:
            if(index == 0)
                roll2w.kp[0] = getReg(ROLL_KP),  roll2w.ki[0] = getReg(ROLL_KI),  roll2w.kd[0] = getReg(ROLL_KD);
            else if(index == 1)
                wroll_control.kp[0] = getReg(ROLL_KP),  wroll_control.ki[0] = getReg(ROLL_KI),  wroll_control.kd[0] = getReg(ROLL_KD);

        break;
        
        case PID_PITCH:
            if(index == 0)
                pitch2w.kp[0] = getReg(PITCH_KP),  pitch2w.ki[0] = getReg(PITCH_KI),  pitch2w.kd[0] = getReg(PITCH_KD);
            else if(index == 1)
                wpitch_control.kp[0] = getReg(PITCH_KP),  wpitch_control.ki[0] = getReg(PITCH_KI),  wpitch_control.kd[0] = getReg(PITCH_KD);

        break;

        case PID_YAW:
            if(index == 0)
                yaw2w.kp[0] = getReg(YAW_KP),  yaw2w.ki[0] = getReg(YAW_KI),  yaw2w.kd[0] = getReg(YAW_KD);
            else if(index == 1)
                wyaw_control.kp[0] = getReg(YAW_KP),  wyaw_control.ki[0] = getReg(YAW_KI),  wyaw_control.kd[0] = getReg(YAW_KD);

        break;

        case PID_X:
            x_control.kp[0] = getReg(X_KP);
            x_control.ki[0] = getReg(X_KI);
            x_control.kd[0] = getReg(X_KD);
        break;

        case PID_Y:
            y_control.kp[0] = getReg(Y_KP);
            y_control.ki[0] = getReg(Y_KI);
            y_control.kd[0] = getReg(Y_KD);
        break;

        case PID_Z:
            z_control.kp[0] = getReg(Z_KP);
            z_control.ki[0] = getReg(Z_KI);
            z_control.kd[0] = getReg(Z_KD);
        break;

    
    }
    roll2w.N_filt = pitch2w.N_filt = yaw2w.N_filt = wroll_control.N_filt = wpitch_control.N_filt = wyaw_control.N_filt = getReg(N_FILTER);
}


void wControlTask(){ 
    
    if(state == DESCEND || state == CONTROL_LOOP){
        float wroll_err  = wroll_ref - gx;
        float wpitch_err = wpitch_ref - gy;
        float wyaw_err   = wyaw_ref - gz;
        
        R = computePid(&wroll_control, wroll_err, TIME, 0);
        P = computePid(&wpitch_control, wpitch_err, TIME, 0);
        Y = computePid(&wyaw_control, wyaw_err, TIME, 0);

        M1 = + R  - P - Y;
        M2 = - R  - P + Y;
        M3 = - R  + P - Y;
        M4 = + R  + P + Y;

        saturateM(H_comp*100);
    }

    

    if(state == SEC_STOP){
        M1 = M2 = M3 = M4 = 0;
    }

    if(state == ARM_MOTORS){
        M1 = M2 = M3 = M4 = 30;
        resetPid(&wroll_control, TIME);
        resetPid(&wpitch_control, TIME);
        resetPid(&wyaw_control, TIME);
    }

    setPwm(&m1, fmin(fmax(M1,0), 100));
    setPwm(&m2, fmin(fmax(M2,0), 100));
    setPwm(&m3, fmin(fmax(M3,0), 100));
    setPwm(&m4, fmin(fmax(M4,0), 100));
    
    setReg(DER_GYRO_X, wroll_control.errd);
    setReg(DER_GYRO_Y, wpitch_control.errd);

    setReg(ROLL_U, R);
    setReg(PITCH_U, P);
    setReg(YAW_U, Y);
    setReg(Z_U, H_comp);
    
    setReg(MOTOR_1, M1);
    setReg(MOTOR_2, M2);
    setReg(MOTOR_3, M3);
    setReg(MOTOR_4, M4);

}

void rpyControlTask(){
    if(state == CONTROL_LOOP || state == DESCEND){
        if(getReg(START_XYC) > 0){
                
                /*roll_ref = -Y_C*cos(raw_yaw) - X_C*sin(raw_yaw);
                pitch_ref = -Y_C*sin(raw_yaw) + X_C*cos(raw_yaw);

                float rel = roll_ref/(pitch_ref + EPS);
                
                if( fabs(rel) < 1  &&  fabs(pitch_ref) >= ANG_MAX  ){
                    pitch_ref = copysign(ANG_MAX, pitch_ref);
                    roll_ref = pitch_ref * rel;
                }
                else if (fabs(rel) >= 1 && fabs(roll_ref) >= ANG_MAX  ){
                    roll_ref = copysign(ANG_MAX, roll_ref);
                    pitch_ref = roll_ref/rel;
                }
                */
                roll_ref = -Y_C;
                pitch_ref = X_C;
        }
        else{

            if(getReg(ROLL_REF) != roll_sp.fin) 
                setTrayectory(&roll_sp, roll_sp.fin, getReg(ROLL_REF), getReg(ROLL_PERIOD), TIME);
            roll_ref =  getSetpoint(&roll_sp, TIME);

            if(getReg(PITCH_REF) != pitch_sp.fin) 
                setTrayectory(&pitch_sp, pitch_sp.fin, getReg(PITCH_REF), getReg(PITCH_PERIOD), TIME);
            pitch_ref =  getSetpoint(&pitch_sp, TIME);
            
        }
        yaw_ref = getReg(YAW_REF);

        wroll_ref = computePid(&roll2w, angle_dif(roll_ref, roll), TIME, 0);
        wpitch_ref = computePid(&pitch2w, angle_dif(pitch_ref, pitch),TIME, 0);
        wyaw_ref = computePid(&yaw2w, angle_dif(yaw_ref, yaw),TIME, 0);
    }
    
    if(state == SEC_STOP){
        setTrayectory(&roll_sp, 0, 0, 1, TIME);
        setTrayectory(&pitch_sp, 0, 0, 1, TIME);
        setReg(ROLL_REF, 0), setReg(PITCH_REF, 0), setReg(YAW_REF, 0);
    }

    if(state == ARM_MOTORS){
        wroll_ref = wpitch_ref = wyaw_ref = 0;
        resetPid(&roll2w, TIME);
        resetPid(&pitch2w, TIME);
        resetPid(&yaw2w, TIME);
    }
    
    setReg(DER_ROLL,roll2w.errd); 
    setReg(DER_PITCH,pitch2w.errd); 
    setReg(DER_YAW,yaw2w.errd); 

    setReg(GYRO_X_REF,wroll_ref);
    setReg(GYRO_Y_REF,wpitch_ref);
    setReg(GYRO_Z_REF,wyaw_ref);
    
    setReg(ROLL_SCURVE, roll_ref);
    setReg(PITCH_SCURVE, pitch_ref);
    setReg(YAW_SCURVE, yaw_ref);
}
bool descend = false;

void xyzControlTask(){

    if(state == CONTROL_LOOP){
        
        if(getReg(X_REF) != x_sp.fin) 
            setTrayectory(&x_sp, x_sp.fin, getReg(X_REF), getReg(X_PERIOD), TIME);
        x_ref =  getSetpoint(&x_sp, TIME);

        if(getReg(Y_REF) != y_sp.fin) 
            setTrayectory(&y_sp, y_sp.fin, getReg(Y_REF), getReg(Y_PERIOD), TIME);
        y_ref =  getSetpoint(&y_sp, TIME);

        if(getReg(Z_REF) != z_sp.fin) 
            setTrayectory(&z_sp, z_sp.fin, getReg(Z_REF), getReg(Z_PERIOD), TIME);
        z_ref =  getSetpoint(&z_sp, TIME);

        X_C = computePid(&x_control, x_ref - xp, TIME, H);
        Y_C = computePid(&y_control, y_ref - yp, TIME, H);

        H_ref = computePid(&z_control, z_ref - z, TIME,0) + getReg(Z_MG);
        rampValue(&H, H_ref, 0.15);
        H_comp = H/(cos(roll)*cos(pitch));
    }
    
    if(state == DESCEND){
        if(!descend) 
            setTrayectory(&H_sp, H_comp, 0, fmin(z*10,3), TIME), descend = true;
        H_comp = getSetpoint(&H_sp, TIME); 
        z_ref = 0;
        
        if(H_comp == 0) state = SEC_STOP;
        
        resetPid(&z_control, TIME);
    
    }else{
        descend = false;   
    }

    if(state == SEC_STOP){
        setTrayectory(&x_sp, 0, 0, 0, TIME);
        setTrayectory(&y_sp, 0, 0, 0, TIME);
        setTrayectory(&z_sp, 0, 0, 0, TIME);
        //setReg(X_REF, 0), setReg(Y_REF, 0), setReg(Z_REF, 0);
        //setReg(Z_MG, 10);
    }

    if(state == ARM_MOTORS){
        resetPid(&x_control, TIME),
        resetPid(&y_control, TIME),
        resetPid(&z_control, TIME);
        
        H = 10;
        H_comp = H/(cos(roll)*cos(pitch));
    }

    
    setReg(X_SCURVE, x_ref);
    setReg(Y_SCURVE, y_ref);
    setReg(Z_SCURVE, z_ref);
    setReg(DER_X,x_control.errd);
    setReg(DER_Y,y_control.errd);
    setReg(DER_Z,z_control.errd);

}


void initControlTasks(){
    
    initFilter(&filter_R, 3, k_3_200, v_3_200);
    initFilter(&filter_P, 3, k_3_200, v_3_200);
    initFilter(&filter_Y, 3, k_3_200, v_3_200);
    //initFilter(&filter_R, 3, k_3_200, v_3_200);

    initPwm(&m4, &htim3, TIM_CHANNEL_1, &(htim3.Instance->CCR1));
    initPwm(&m1, &htim3, TIM_CHANNEL_2, &(htim3.Instance->CCR2));
    initPwm(&m3, &htim4, TIM_CHANNEL_3, &(htim4.Instance->CCR3));
    initPwm(&m2, &htim4, TIM_CHANNEL_4, &(htim4.Instance->CCR4));

    initPid(&z_control,  10, 750,    2, 0, 50 , 10, 30, (NORMAL | D_SG));
    initPid(&x_control, 0.2, 1.5, 0.08, 0, 50 , 10, ANG_MAX, D_SG);
    initPid(&y_control, 0.2, 1.5, 0.08, 0, 50 , 10, ANG_MAX, D_SG);

    initPidFilter(&roll2w,  500, -1000, 20, TIME, 50, pi/9, 3000, (D_SG | D_FILTER), 4, k_1_20, v_1_20 );
    initPidFilter(&pitch2w, 300, -1000, 20, TIME, 50, pi/9, 3000, (D_SG | D_FILTER), 4, k_1_20, v_1_20 );
    initPidFilter(&yaw2w,     0,   0,  0, TIME, 50, pi/9, 3000, (D_SG | D_FILTER), 4, k_1_20, v_1_20 );
 
    initPidFilter(&wroll_control,   5, 500, 10, TIME, 50, 80, 3000, ( D_SG | D_FILTER),  6, k_1_10, v_1_10 );
    initPidFilter(&wpitch_control,  8, 500, 16, TIME, 50, 80, 3000, ( D_SG | D_FILTER),  6, k_1_10, v_1_10 );
    initPidFilter(&wyaw_control,   10, 500, 30, TIME, 50, 80, 3000, ( D_SG | D_FILTER),  6, k_1_10, v_1_10 );
    
    initFilter(&filter_wroll, 4, k_1_20, v_1_20);
    initFilter(&filter_wpitch, 4, k_1_20, v_1_20);
    initFilter(&filter_wyaw, 4, k_1_20, v_1_20);

    setTrayectory(&x_sp, 0, 0, 1, TIME);
    setTrayectory(&y_sp, 0, 0, 1, TIME);
    setTrayectory(&z_sp, 0, 0, 1, TIME);

    setTrayectory(&roll_sp, 0, 0, 1, TIME);
    setTrayectory(&pitch_sp, 0, 0, 1, TIME);
    
    setTrayectory(&H_sp, 0, 0, 1, TIME);

    setReg(PID_INDEX, -1);
    setReg(PID_VAR, -1);
    setReg(N_FILTER, 50);

    addTask(&wControlTask, 1000, 1);
    addTask(&rpyControlTask, 2000, 1);
    addTask(&xyzControlTask, 10000, 1);
    
}
