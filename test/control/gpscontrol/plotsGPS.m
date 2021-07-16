close all;
Txyz = Dxyz(end).datos;
Trpy = Drpy(end).datos;
figure(1)
plot(Txyz.X_VAL,Txyz.Y_VAL);
figure(2)   
plotHold([Txyz.XP_REF,Txyz.XP_VAL]);
figure(3)
plotHold([Txyz.YP_REF,Txyz.YP_VAL]);
figure(4)
plotHold([Trpy.ROLL_SCURVE, Trpy.ROLL_VAL]);
figure(5)
plotHold([Trpy.PITCH_SCURVE, Trpy.PITCH_VAL]);