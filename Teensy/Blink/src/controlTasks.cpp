#include "..\headers\controlTasks.h"
#include "..\headers\filter.h"


pwm m1, m2, m3, m4;
pid roll2w, pitch2w, yaw2w; 
pid wroll_control, wpitch_control, wyaw_control;
pid z_control, x_control, y_control;


float  H, H_comp, R, P, Y, H_ref, X_C, Y_C, R_MAX = pi/22.0 , P_MAX = pi/22.0;
float M1,M2,M3,M4;

float roll_off = 0 , pitch_off = 0, yaw_off = 0, x_off = 0, y_off = 0, z_off = 0;
float wroll_ref, wpitch_ref, wyaw_ref, roll_ref, pitch_ref, yaw_ref, x_ref, y_ref, z_ref;
float wroll_err,wpitch_err,wyaw_err; 

timer timer_wcontrol, timer_rpycontrol, timer_xyzcontrol;

void saturateM(float H){
    float f_max = 1;
    float arr_M[] = {M1, M2, M3, M4};
    for(int i = 0; i < 4 ; i++){
        float delta = max(max(arr_M[i] + H - 10000, -arr_M[i]-H), 0);
        f_max = max(f_max, abs(arr_M[i] / (abs(arr_M[i]) - delta + 0.0000001)) );
    }

    M1 = sqrt(M1 / f_max + H);
    M2 = sqrt(M2 / f_max + H);
    M3 = sqrt(M3 / f_max + H);
    M4 = sqrt(M4 / f_max + H);
}


void updatePID(){
    int index = getReg(PID_INDEX), var = getReg(PID_VAR);
    if(index >= 0) {
        switch(var){

            case PID_ROLL:
                if(index == 0)
                    roll2w.kp[0] = getReg(ROLL_KP),  roll2w.ki[0] = getReg(ROLL_KI),  roll2w.kd[0] = getReg(ROLL_KD);
                else
                    wroll_control.kp[0] = getReg(ROLL_KP),  wroll_control.ki[0] = getReg(ROLL_KI),  wroll_control.kd[0] = getReg(ROLL_KD);

            break;
            
            case PID_PITCH:
                if(index == 0)
                    pitch2w.kp[0] = getReg(PITCH_KP),  pitch2w.ki[0] = getReg(PITCH_KI),  pitch2w.kd[0] = getReg(PITCH_KD);
                else
                    wpitch_control.kp[0] = getReg(PITCH_KP),  wpitch_control.ki[0] = getReg(PITCH_KI),  wpitch_control.kd[0] = getReg(PITCH_KD);

            break;

            case PID_YAW:
                if(index == 0)
                    yaw2w.kp[0] = getReg(YAW_KP),  yaw2w.ki[0] = getReg(YAW_KI),  yaw2w.kd[0] = getReg(YAW_KD);
                else
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
}

void wControlInterrupt(){

    
    wroll_err = max( min( wroll_ref - gx , 100), -100);
    wpitch_err = max( min( wpitch_ref - gy , 100), -100);
    wyaw_err = max( min( wyaw_ref - gz , 100), -100);


    R = computePid(&wroll_control, wroll_err, time, 0);
    P = computePid(&wpitch_control, wpitch_err, time, 0);
    Y = computePid(&wyaw_control, wyaw_err, time, 0);
    
    setReg(DER_GYRO_X, wroll_control.errd);
    setReg(DER_GYRO_Y, wpitch_control.errd);
    

    /*R = getReg(ROLL_REF);
    P = getReg(PITCH_REF);
    Y = getReg(YAW_REF);*/
    
    setReg(ROLL_U, R);
    setReg(PITCH_U, P);
    setReg(YAW_U, Y);
    setReg(Z_U, H_comp);

    M1 = R + P + Y;
    M2 = R - P - Y;
    M3 = -R - P + Y;
    M4 = -R + P - Y;

    saturateM(H_comp*H_comp);

    setReg(MOTOR_1, M1);
    setReg(MOTOR_2, M2);
    setReg(MOTOR_3, M3);
    setReg(MOTOR_4, M4);

    if(security){
        M1 = M2 = M3 = M4 = 0;
        resetPid(&wroll_control, time);
        resetPid(&wpitch_control, time);
        resetPid(&wyaw_control, time);
    }
    
    setPwmDutyTime(&m1, min(max(M1,0), 100));
    setPwmDutyTime(&m2, min(max(M2,0), 100));
    setPwmDutyTime(&m3, min(max(M3,0), 100));
    setPwmDutyTime(&m4, min(max(M4,0), 100));
}

void rpyControlInterrupt(){
    
    wroll_ref = computePid(&roll2w, angle_dif(roll_ref, roll), time, 0);
    wpitch_ref = computePid(&pitch2w, angle_dif(pitch_ref, pitch),time, 0);
    wyaw_ref = -computePid(&yaw2w, angle_dif(yaw_ref, yaw),time, 0);

/*
    Serial.print("\nu values: ");
    for(int i = 0 ; i < 13; i++) Serial.print(filter_wroll.arr_u.values[i]), Serial.print('\t');
    Serial.print("\nu coeff: ");
    for(int i = 0 ; i < 13; i++) Serial.print(filter_wroll.arr_u.coeff[i]), Serial.print('\t');
    Serial.print("\ny values: ");
    for(int i = 0 ; i < 12; i++) Serial.print(filter_wroll.arr_y.values[i]), Serial.print('\t');
    Serial.print("\ny coeff: ");
    for(int i = 0 ; i < 12; i++) Serial.print(filter_wroll.arr_y.coeff[i]), Serial.print('\t');
    Serial.println("****************************************************************************");
*/
    setReg(GYRO_X_REF,wroll_ref);
    setReg(GYRO_Y_REF,wpitch_ref);
    setReg(GYRO_Z_REF,wyaw_ref);
    
    if(security){
        resetPid(&roll2w, time);
        resetPid(&pitch2w, time);
        resetPid(&yaw2w, time);
    }
}

void xyzControlInterrupt(){
    
    X_C = computePid(&x_control, -x, time, H);
    Y_C = computePid(&y_control, -y, time, H);

    rampValue(&z_ref, getReg(Z_REF), getReg(Z_REF_SIZE));

    H_ref = computePid(&z_control, z_ref - z, time,0) + getReg(Z_MG);
    rampValue(&H, H_ref, 0.2);

    H_comp = H;

    rampValue(&roll_ref, getReg(ROLL_REF) + roll_off, 0.015);
    rampValue(&pitch_ref, getReg(PITCH_REF) + pitch_off, 0.015);
    yaw_ref = getReg(YAW_REF) + yaw_off;

    if(security){
        H = 0; z_ref = 0;
        resetPid(&x_control, time);
        resetPid(&y_control, time);
        resetPid(&z_control, time);
    }
}


void initControlTasks(){
    initOneshot125(&m1, 5);
    initOneshot125(&m2, 4);
    initOneshot125(&m3, 3);
    initOneshot125(&m4, 2);

    initPid(&z_control, 0, 0, 0, 0, 1 , 100000, 15, NORMAL);
    initPid(&x_control, 0, 0, 0, 0, 1 , 100000, 0.09, NORMAL);
    initPid(&y_control, 0, 0, 0, 0, 1 , 100000,0.09, NORMAL);

    initPid(&roll2w, 0, 0, 0, time, 50, 1.57*0.5,80, (P2ID & D_INT));
    initPid(&pitch2w, 0, 0, 0, time, 50, 1.57*0.5,80, (P2ID & D_INT));
    initPid(&yaw2w, 0, 0, 0, time, 50, 1.57*0.5,80, (P2ID & D_INT));

    initPid(&wroll_control, 0, 0, 0, time, 50,  80, 3000, (P2ID & D_INT) );
    initPid(&wpitch_control, 0, 0, 0, time, 50, 80, 3000, (P2ID & D_INT ) );
    initPid(&wyaw_control, 0, 0, 0, time, 50, 80, 3000, (P2ID & D_INT) );
    
    setReg(PID_INDEX, -1);
    setReg(PID_VAR, -1);
    setReg(N_FILTER, 50);
    
    initTimer(&timer_wcontrol, &wControlInterrupt, 1000);
    initTimer(&timer_rpycontrol, &rpyControlInterrupt, 500);
    initTimer(&timer_xyzcontrol, &xyzControlInterrupt, 100);
}

void executeControlTasks(){
    
    if(timerReady(&timer_wcontrol))  executeTimer(&timer_wcontrol);
    if(timerReady(&timer_rpycontrol))  executeTimer(&timer_rpycontrol);
    if(timerReady(&timer_xyzcontrol))  executeTimer(&timer_xyzcontrol);

}
