close all
Gr = 1;
Ts = 0.1;

shif = 0;
rng = (1:1:(height(Trpy)- shif ) );


ax = Trpy.ACC_X(rng)*Gr;
ay = Trpy.ACC_Y(rng)*Gr;
az = Trpy.ACC_Z(rng)*Gr;

gx = Trpy.GYRO_X(rng)*pi/180;
gy = Trpy.GYRO_Y(rng)*pi/180;
gz = Trpy.GYRO_Z(rng)*pi/180;


% gx(abs(gx)< 0.5) = 0;
% gy(abs(gy)< 0.5) = 0;


roll = Trpy.ROLL_VAL(rng)*pi/180;
pitch = Trpy.PITCH_VAL(rng)*pi/180;
yaw = Trpy.YAW_VAL(rng)*pi/180;

roll_e = zeros(size(Trpy.ROLL_VAL(rng)));
pitch_e = zeros(size(Trpy.PITCH_VAL(rng)));

ox = Txyz.XP_VAL(rng+shif);
oy = Txyz.YP_VAL(rng+shif);
z = Txyz.Z_VAL(rng);

vz = diff([0; Txyz.Z_VAL(rng)])/Ts;
N = length(rng);

ax_rot = zeros(size(Trpy.ACC_X(rng)));
ay_rot = zeros(size(Trpy.ACC_Y(rng)));
az_rot = zeros(size(Trpy.ACC_Z(rng)));


vx = zeros(1,N);
vy = zeros(1,N);

g = [0 0 1]'*Gr;
P = eye(4)*1;
H = [eye(3) zeros(3,3)];
Q = eye(4)*1;
R = eye(4)*0.1;

N=14;

for i= 1:N
    if mod(i,10) == 0

    roll_e(i) = roll_e(i) + Ts*(oy(i) - vy(i)/z(i));
    pitch_e(i) = pitch_e(i) + Ts*(-ox(i) + vx(i)/z(i));
    
    Rq = rpy2R(roll_e(i), pitch_e(i), 0);
 

    ax_rot(i) = ax(i)*cos(pitch_e(i)) - az(i)*sin(pitch_e(i));
    ay_rot(i) = ay(i)*cos(roll_e(i)) + az(i)*cos(pitch_e(i))*sin(roll_e(i)) + ax(i)*sin(pitch_e(i))*sin(roll_e(i));
%     az_rot(i) = u_rot(3);

   

    vx(i) = vx(i) + Ts*(ax_rot(i));
    vy(i) = vy(i) + Ts*(ay_rot(i));
    
    F = [0, 0, 0, -1/z(i);...
         0, 0, 1/z(i), 0; ...
         0, -az(i)*cos(pitch_e(i)) - ax(i)*sin(pitch_e(i)), 0, 0; ...
        az(i)*cos(pitch_e(i))*cos(roll_e(i)) - ay(i)*sin(roll_e(i)) + ax(i)*cos(roll_e(i))*sin(pitch_e(i)), ax(i)*cos(pitch_e(i))*sin(roll_e(i)) - az(i)*sin(pitch_e(i))*sin(roll_e(i)), 0, 0];
     
    H = [eye(2) zeros(2,2); 0 0 0 -1/z(i); 0 0 1/z(i) 0];
    
    
    P = F*P*F' + Q;
    if mod(i,10) == 0

        K = P*H'/(H*P*H' + R)
        xe = [roll_e(i), pitch_e(i), oy(i)-vy(i)/z(i), -ox(i)+vx(i)/z(i)]';
        xm = [roll(i) pitch(i) gx(i) gy(i)]';
        ye = xm - xe;
        xe = xe + K*ye;
        roll_e(i) = xe(1);
        pitch_e(i) = xe(2);
        %vx(i) = z(i)*( xe(4) + ox(i));
        %vy(i) = z(i)*(-xe(3) + oy(i));

        P = (eye(4) - K*H)*P; 
    end 
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
