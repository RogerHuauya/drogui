N = length(dataout(:,4));
close all
gx = dataout(:,4);
gy = dataout(:,5);
gz = dataout(:,6);

ax = dataout(:,1);
ay = dataout(:,2);
az = dataout(:,3);

mx = dataout(:,7);
my = dataout(:,8);
mz = dataout(:,9);
%%
Ts = 1/500;
G = 9.81;
P = 0.001*ones(4,4);
Q = (1e-6)*diag(ones(1,4));
R1 = 0.6*diag([1,2,3]);
R2 = 0.5*diag([1,2,1]); 
%%
a = 5;
myMahony = mahony(2*a,0.01*a,500);
myTkf = tkf(Ts, G, P, Q, R1, R2);

myYawM = zeros(N,1);
myRollM = zeros(N,1);
myPitchM = zeros(N,1);
myYawT = zeros(N,1);
myRollT = zeros(N,1);
myPitchT = zeros(N,1);
%%
clc
myTkf = tkf(Ts, G, P, Q, R1, R2);
for i=1:10
    %myMahony = mahonyUpdate(myMahony, gx(i)*pi/180, gy(i)*pi/180, gz(i)*pi/180, ax(i), ay(i), az(i), mx(i), my(i), mz(i));
    myTkf = updateStage1(myTkf, 1, 1, 0, 0, 0, 9.81);
    myTkf = updateStage2(myTkf, 1, 0.5, 0.5);
    
    %[myRollM(i), myPitchM(i), myYawM(i)] = getMahonyEuler(myMahony);
    [myRollT(i), myPitchT(i) , myYawT(i)] = getTkfEuler(myTkf);    
end
%disp(myRollT)
%%
figure(1)
plotHold([myRollM(10:end)*180/pi, myRollT(10:end)*180/pi , dataout(10:end,10)])
legend({'Mahony', 'TKF', 'STM32'})
title('Roll')
figure(2)
plotHold([myPitchM(10:end)*180/pi, myPitchT(10:end)*180/pi , dataout(10:end,11)])
legend({'Mahony', 'TKF', 'STM32'})
title('Pitch')
figure(3)
offsetyaw = 280;
plotHold([myYawM*180/pi, offsetyaw+myYawT*180/pi, dataout(:,12)- 90])
legend({'Mahony', 'TKF', 'STM32'})
title('Yaw')