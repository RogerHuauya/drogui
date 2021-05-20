close all
figure('Name', "Figure Roll");
plotHold([T_rpy.ROLL_SCURVE, T_rpy.ROLL_VAL])
figure('Name', "Figure Pitch");
plotHold([T_rpy.PITCH_SCURVE, T_rpy.PITCH_VAL])
figure('Name', "Figure Gyro X");
plotHold([T_rpy.GYRO_X_REF, T_rpy.GYRO_X])
figure('Name', "Figure Gyro Y");
plotHold([T_rpy.GYRO_Y_REF, T_rpy.GYRO_Y])

figure('Name', "Figure U");
plotHold([T_rpy.ROLL_U, T_rpy.PITCH_U])