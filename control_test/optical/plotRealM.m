close all
figure
M1 = Txyz.MOTOR_1.^2 - 100*Txyz.Z_U;
M2 = Txyz.MOTOR_2.^2 - 100*Txyz.Z_U;
M3 = Txyz.MOTOR_3.^2 - 100*Txyz.Z_U;
M4 = Txyz.MOTOR_4.^2 - 100*Txyz.Z_U;
H = M1 + M2 + M3 + M4;
R = (M4 + M1 - M2 - M3)/4;
P = (M3 + M4 - M1 - M2)/4;

plotHold([R, Trpy.ROLL_U]);
figure
plotHold([P, Trpy.PITCH_U])
figure
plotSync([M1, M2, M3, M4])
figure
plotSync([Txyz.MOTOR_1, Txyz.MOTOR_2, Txyz.MOTOR_3, Txyz.MOTOR_4]);