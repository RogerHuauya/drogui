close all
%%
figure()
plotSync([Txyz.Z_VAL, Txyz.Z_SCURVE, Txyz.Z_U, Txyz.DER_Z])
%%
figure()
title('ROLL')
plotSync([Trpy.ROLL_VAL, Trpy.ROLL_SCURVE])
%%
figure()
title('PITCH')
plotSync([Trpy.PITCH_VAL, Trpy.PITCH_SCURVE])
%%
figure()
plotSync([Trpy.GYRO_X, Trpy.GYRO_X_REF, Trpy.ROLL_U])
%%
figure()
plotSync([Trpy.GYRO_Y, Trpy.GYRO_Y_REF, Trpy.PITCH_U])
%%
figure()
plotSync([Txyz.MOTOR_1, Txyz.MOTOR_2, Txyz.MOTOR_3, Txyz.MOTOR_4]);
%%
figure()
plotSync([Txyz.Z_VAL, Trpy.ROLL_VAL, Trpy.PITCH_VAL])
%%
figure()
plotSync([Txyz.Z_VAL, Txyz.MOTOR_1, Txyz.MOTOR_2, Txyz.MOTOR_3, Txyz.MOTOR_4]);

