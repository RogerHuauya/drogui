close all
plot(Trpy.RAW_YAW*180/pi)
figure
plotHold([Txyz.XP_VAL, Txyz.YP_VAL])
% figure
% xp = Txyz.GPS_VX.*cos( Trpy.RAW_YAW) + Txyz.GPS_VY.*sin( Trpy.RAW_YAW);
% yp = -Txyz.GPS_VX.*sin( Trpy.RAW_YAW) + Txyz.GPS_VY.*cos( Trpy.RAW_YAW);
% plotHold([Txyz.GPS_VX, xp])
% figure
% plotHold([Txyz.GPS_VY, yp])
figure
plot(Trpy.ROLL_SCURVE)
figure
plot(Trpy.PITCH_SCURVE)
figure
plot(Txyz.Z_VAL)