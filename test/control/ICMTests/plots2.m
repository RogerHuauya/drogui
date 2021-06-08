close all
plotSync([T.GYRO_X, T.GYRO_X_REF, T.GYRO_Y, T.GYRO_Y_REF])
figure
plotSync([T.ROLL_VAL, T.ROLL_U, T.PITCH_VAL, T.PITCH_U])
figure
plotSync([T.GYRO_X - T.GYRO_X_REF, T.GYRO_Y - T.GYRO_Y_REF])
