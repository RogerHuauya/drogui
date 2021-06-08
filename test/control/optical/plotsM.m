%close all
M1 = Trpy.ROLL_U - Trpy.PITCH_U + 100*Txyz.Z_U;
M1 = sqrt(max(M1,0));

M2 = -Trpy.ROLL_U - Trpy.PITCH_U + 100*Txyz.Z_U; 
M2 = sqrt(max(M2,0));

M3 = -Trpy.ROLL_U + Trpy.PITCH_U + 100*Txyz.Z_U; 
M3 = sqrt(max(M3,0));

M4 = +Trpy.ROLL_U + Trpy.PITCH_U + 100*Txyz.Z_U; 
M4 = sqrt(max(M4,0));

plotSync([M1, M2, M3, M4])