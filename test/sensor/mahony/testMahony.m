N = length(Trpy.TIME);
close all
gx = Trpy.GYRO_X;
gy = Trpy.GYRO_Y;
gz = Trpy.GYRO_Z;

ax = Trpy.ACC_X;
ay = Trpy.ACC_Y;
az = Trpy.ACC_Z;

mx = Trpy.MAG_X;
my = Trpy.MAG_Y;
mz = Trpy.MAG_Z;
a = 5;
myMahony = mahony(2*a,0.1*a,100);
myYaw = zeros(N,1);
myRoll = zeros(N,1);

for i=1:N
    myMahony = mahonyUpdate(myMahony, gx(i)*pi/180, gy(i)*pi/180, gz(i)*pi/180, ax(i), ay(i), az(i), mx(i), my(i), mz(i));
    [myRoll(i), ~ ,myYaw(i)] = getMahonyEuler(myMahony); 
end
plotHold([myRoll, Trpy.RAW_ROLL])
figure
plotHold([myYaw, Trpy.RAW_YAW-0.5*pi])