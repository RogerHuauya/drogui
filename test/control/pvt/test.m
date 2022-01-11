close all
%plot(Trpy.RAW_YAW*180/pi)
%figure
%plotSync([Trpy.MAG_X, Trpy.MAG_Y, Trpy.MAG_Z])
coder.cinclude('mahony.h');
mahony = coder.opaque('mahony');

coder.ceval('initMahony', mahony, 2, 0.1, 100)
% % evaluate the C function
% added = coder.ceval('adder', in1, in2); 
% multed = in1*in2; 
% figure
% xp = Txyz.GPS_VX.*cos( Trpy.RAW_YAW) + Txyz.GPS_VY.*sin( Trpy.RAW_YAW);
% yp = -Txyz.GPS_VX.*sin( Trpy.RAW_YAW) + Txyz.GPS_VY.*cos( Trpy.RAW_YAW);
% plotHold([Txyz.GPS_VX, xp])
% figure
% plotHold([Txyz.GPS_VY, yp])
