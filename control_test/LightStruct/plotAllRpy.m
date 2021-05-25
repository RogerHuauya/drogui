close all
figure('Name', "Figure Roll");
plotHold([Trpy.ROLL_SCURVE, Trpy.ROLL_VAL])
figure('Name', "Figure Pitch");
plotHold([Trpy.PITCH_SCURVE, Trpy.PITCH_VAL])
% figure('Name', "Figure Gyro X");
% plotHold([Trpy.GYRO_X_REF, Trpy.GYRO_X])
% figure('Name', "Figure Gyro Y");
% plotHold([Trpy.GYRO_Y_REF, Trpy.GYRO_Y])
% 
% figure('Name', "Figure U");
% plotHold([Trpy.ROLL_U, Trpy.PITCH_U])