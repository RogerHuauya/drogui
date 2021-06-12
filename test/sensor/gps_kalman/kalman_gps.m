close all;
Gr = 9.81;
Ts = 0.01;


N = height(Trpy.ACC_X);

shif = 8;
rng = (1:1:N);



ax = Trpy.ACC_X(rng)*Gr;
ay = Trpy.ACC_Y(rng)*Gr;
az = Trpy.ACC_Z(rng)*Gr;

gx = Trpy.GYRO_X(rng)*pi/180;
gy = Trpy.GYRO_Y(rng)*pi/180;
gz = Trpy.GYRO_Z(rng)*pi/180;

mx = Trpy.MAG_X(rng)*pi/180;
mx = Trpy.MAG_Y(rng)*pi/180;
mx = Trpy.MAG_Z(rng)*pi/180;

gpsX = Txyz.GPS_X;
gpsY = Txyz.GPS_Y;

ox = Txyz.XP_VAL(rng+shif);
oy = Txyz.YP_VAL(rng+shif);



roll = Trpy.ROLL_VAL(rng)*pi/180;
pitch = Trpy.PITCH_VAL(rng)*pi/180;
yaw = Trpy.YAW_VAL(rng)*pi/180;

z = Txyz.Z_VAL(rng);


roll_e = zeros(size(Trpy.ROLL_VAL(rng)));
pitch_e = zeros(size(Trpy.PITCH_VAL(rng)));


vz = diff([0; Txyz.Z_VAL(rng)])/Ts;
N = length(rng);

ax_rot = zeros(size(Trpy.ACC_X(rng)));
ay_rot = zeros(size(Trpy.ACC_Y(rng)));
az_rot = zeros(size(Trpy.ACC_Z(rng)));


vx = zeros(1,N);
vy = zeros(1,N);

g = [0 0 1]'*Gr;
P = eye(4)*100;
H = [eye(3) zeros(3,3)];
Q = eye(4)*10000;
R = eye(4)*0.00001;


p = zeros(3,N);
v = zeros(3,N);
accR = zeros(3,N);

for i = 2:N

    Rq = rpy2R(roll(i), pitch(i), yaw(i));
    accR(:,i) = Rq*[ax(i) ay(i) az(i)]' - g;

    p(:,i) = p(:,i-1) + Ts*v(i-1) + (Ts^2)/2*(accR(:,i))
    v(:,i) = v(:,i-1) + Ts*(accR(:,i))

    F = [    eye(3),  Ts*eye(3), zeros(3,3);
         zeros(3,3),     eye(3),      Ts*Rq;
         zeros(3,3), zeros(3,3),     eye(3); ]

    G = [   zeros(3,6);
            Ts*Rq zeros(3,3);
            zeros(3,3) eye(3); ]
    
    
    P = (eye(4) - K*H)P(eye(4) - K*H)' + K*R*K';
end

figure
plotHold([(ox + gy).*z, vx'])

figure
plotHold([(oy - gx).*z, vy'])

%%
figure
plotHold([roll, roll_e])

figure
plotHold([pitch, pitch_e])

figure
plotHold([vx', vy'])
figure
plotHold([ax_rot, ay_rot])