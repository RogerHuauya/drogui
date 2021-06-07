close all
figure('Name', "Figure XP, YP");
plotHold([Txyz.XP_VAL, Txyz.YP_VAL])
figure('Name', "Figure Z");
plotHold([Txyz.Z_SCURVE, Txyz.Z_VAL])
figure('Name', "Figure ZU");
plotHold([Txyz.Z_U])
figure('Name', "Figure DXP, DYP");
plotHold([Txyz.DER_X, Txyz.DER_Y])
% figure('Name', "Figure MOTORS");
% plotSync([Txyz.MOTOR_1, Txyz.MOTOR_2, Txyz.MOTOR_3, Txyz.MOTOR_4])