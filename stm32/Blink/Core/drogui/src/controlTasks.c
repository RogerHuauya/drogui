#include "controlTasks.h"
#include "filter.h"
#include "task.h"
#include "pwm.h"
#include "usart.h"
#include <stdio.h>
#include <string.h>

pwm m1, m2, m3, m4;
pid roll2w, pitch2w, yaw2w; 
pid wroll_control, wpitch_control, wyaw_control;
pid z_control, x_control, y_control;

filter filter_wroll, filter_wpitch, filter_wyaw;
filter filter_m1, filter_m2, filter_m3, filter_m4; 

float  H, H_comp, R, P, Y, H_ref, X_C, Y_C, R_MAX = PI/22.0 , P_MAX = PI/22.0;
float M1,M2,M3,M4;

float roll_off = 0 , pitch_off = 0, yaw_off = 0, x_off = 0, y_off = 0, z_off = 0;
float wroll_ref, wpitch_ref, wyaw_ref, roll_ref, pitch_ref, yaw_ref, x_ref, y_ref, z_ref;
float wroll_err,wpitch_err,wyaw_err; 

//float aux_wref, aux_wref2;

char buffc[500] = "hola\n";
void saturateM(float H){
    float f_max = 1;
    float arr_M[] = {M1, M2, M3, M4};
    for(int i = 0; i < 4 ; i++){
        float delta = fmax(fmax(arr_M[i] + H - 10000, -arr_M[i]-H), 0);
        f_max = fmax(f_max, fabs(arr_M[i] / (fabs(arr_M[i]) - delta + 0.0000001)) );
    }

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
        
        case PID_Z:
            z_control.kp[0] = getReg(Z_KP);
            z_control.ki[0] = getReg(Z_KI);
            z_control.kd[0] = getReg(Z_KD);
        break;
    }
    roll2w.N_filt = pitch2w.N_filt = yaw2w.N_filt = wroll_control.N_filt = wpitch_control.N_filt = wyaw_control.N_filt = getReg(N_FILTER);
}


void wControlTask(){ 
    
    wroll_err = fmax( fmin( wroll_ref - gx , 20), -20);
    wpitch_err = fmax( fmin( wpitch_ref - gy , 20), -20);
    wyaw_err = fmax( fmin( wyaw_ref - gz , 20), -20);

    R = computePid(&wroll_control, wroll_err, TIME, 0);
    P = computePid(&wpitch_control, wpitch_err, TIME, 0);
    Y = -computePid(&wyaw_control, wyaw_err, TIME, 0);
    
    setReg(DER_GYRO_X, wroll_control.errd);
    setReg(DER_GYRO_Y, wpitch_control.errd);

    setReg(ROLL_U, R);
    setReg(PITCH_U, P);
    setReg(YAW_U, Y);
    setReg(Z_U, H_comp);

    M1 = -R + P + Y;
    M2 = R + P - Y;
    M3 = R - P + Y;
    M4 = -R - P - Y;

    saturateM(H_comp*H_comp);

    setReg(MOTOR_1, M1);
    setReg(MOTOR_2, M2);
    setReg(MOTOR_3, M3);
    setReg(MOTOR_4, M4);

    if(security){
        M1 = M2 = M3 = M4 = 0;
        resetPid(&wroll_control, TIME);
        resetPid(&wpitch_control, TIME);
        resetPid(&wyaw_control, TIME);
    }

    setPwm(&m3, fmin(fmax(M1,0), 100));
    setPwm(&m1, fmin(fmax(M2,0), 100));
    setPwm(&m2, fmin(fmax(M3,0), 100));
    setPwm(&m4, fmin(fmax(M4,0), 100));

}

void rpyControlTask(){

    float wroll_ref_d = computePid(&roll2w, angle_dif(roll_ref, roll), TIME, 0);
    float wpitch_ref_d = computePid(&pitch2w, angle_dif(pitch_ref, pitch),TIME, 0);
    float wyaw_ref_d = -computePid(&yaw2w, angle_dif(yaw_ref, yaw),TIME, 0);

    wroll_ref_d = computeFilter(&filter_wroll,  wroll_ref_d);
    wpitch_ref_d = computeFilter(&filter_wpitch, wpitch_ref_d);
    wyaw_ref_d = computeFilter(&filter_wyaw, wyaw_ref_d);

    rampValue(&wroll_ref, wroll_ref_d, 0.2);
    rampValue(&wpitch_ref, wpitch_ref_d, 0.2);
    
    setReg(GYRO_X_REF,wroll_ref);
    setReg(GYRO_Y_REF,wpitch_ref);
    setReg(GYRO_Z_REF,wyaw_ref_d);
    
    if(security){
        resetPid(&roll2w, TIME);
        resetPid(&pitch2w, TIME);
        resetPid(&yaw2w, TIME);
    }
}

void xyzControlTask(){
    X_C = computePid(&x_control, -x, TIME, H);
    Y_C = computePid(&y_control, -y, TIME, H);


    rampValue(&z_ref, getReg(Z_REF), getReg(Z_REF_SIZE));

    H_ref = computePid(&z_control, z_ref - z, TIME,0) + getReg(Z_MG);// + getReg(AMP_SIN)*sin(2*PI*getReg(FREQ_SIN)*TIME/1000000);
    rampValue(&H, H_ref, 0.2);

    H_comp = H/(cos(roll)*cos(pitch));

    rampValue(&roll_ref, getReg(ROLL_REF) + roll_off, 0.015);
    rampValue(&pitch_ref, getReg(PITCH_REF) + pitch_off, 0.015);
    yaw_ref = getReg(YAW_REF) + yaw_off;

    if(security){
        H = 0; z_ref = 0;
        resetPid(&x_control, TIME);
        resetPid(&y_control, TIME);
        resetPid(&z_control, TIME);
    }
}


void initControlTasks(){
    
    initPwm(&m1, &htim3, TIM_CHANNEL_1, &(htim3.Instance->CCR1));
    initPwm(&m2, &htim3, TIM_CHANNEL_2, &(htim3.Instance->CCR2));
    initPwm(&m3, &htim4, TIM_CHANNEL_3, &(htim4.Instance->CCR3));
    initPwm(&m4, &htim4, TIM_CHANNEL_4, &(htim4.Instance->CCR4));

    initPid(&z_control, 0, 0, 0, 0, 50 , 10, 15, (P2ID & D_INT));
    initPid(&x_control, 0, 0, 0, 0, 50 , 10, 0.09, NORMAL);
    initPid(&y_control, 0, 0, 0, 0, 50 , 10,0.09, NORMAL);

    initPid(&roll2w,    200, 0, 20, TIME, 50, 0.785, 60, (P2ID & D_INT));
    initPid(&pitch2w,   200, 0, 20, TIME, 50, 0.785, 60, (P2ID & D_INT));
    initPid(&yaw2w,     0, 0, 0, TIME, 50, 0.785, 60, (P2ID & D_INT));

    initPid(&wroll_control, 15, 0, 30, TIME, 50, 80, 3000, (P2ID & D_INT));
    initPid(&wpitch_control,15, 0, 30, TIME, 50, 80, 3000, (P2ID & D_INT));
    initPid(&wyaw_control, 50, 0, 30, TIME, 50, 80, 3000, (P2ID & D_INT));
    
    initFilter(&filter_wroll, 4, k_1_20, v_1_20);
    initFilter(&filter_wpitch, 4, k_1_20, v_1_20);
    initFilter(&filter_wyaw, 4, k_1_20, v_1_20);

    setReg(PID_INDEX, -1);
    setReg(PID_VAR, -1);
    setReg(N_FILTER, 50);

    addTask(&wControlTask, 1000, 1);
    addTask(&rpyControlTask, 2000, 1);
    addTask(&xyzControlTask, 10000, 1);
    
}
